/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "base/log/ace_performance_check.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <memory>
#include <numeric>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base/i18n/localization.h"
#include "base/json/json_util.h"
#include "base/log/dump_log.h"
#include "base/utils/system_properties.h"
#include "base/utils/time_util.h"
#include "base/utils/utils.h"
#include "bridge/common/utils/engine_helper.h"
#include "core/common/container.h"

namespace OHOS::Ace {
namespace {
constexpr int32_t BASE_YEAR = 1900;
constexpr char DATE_FORMAT[] = "MM-dd HH:mm:ss";
constexpr int32_t CONVERT_NANOSECONDS = 1000000;
} // namespace

// ============================== survival interval of JSON files ============================================

std::unique_ptr<JsonValue> AcePerformanceCheck::performanceInfo_ = nullptr;

void AcePerformanceCheck::Start()
{
    if (SystemProperties::IsPerformanceCheckEnabled()) {
        LOGD("AcePerformanceCheck::Start()");
        performanceInfo_ = JsonUtil::Create(true);
    }
}

void AcePerformanceCheck::Stop()
{
    if (performanceInfo_) {
        LOGD("AcePerformanceCheck::Stop()");
        auto info = performanceInfo_->ToString();
        // output info to json file
        auto filePath = AceApplicationInfo::GetInstance().GetDataFileDirPath() + "/arkui_bestpractice.json";
        std::unique_ptr<std::ostream> ss = std::make_unique<std::ofstream>(filePath);
        CHECK_NULL_VOID(ss);
        DumpLog::GetInstance().SetDumpFile(std::move(ss));
        DumpLog::GetInstance().Print(info);
        DumpLog::GetInstance().Reset();
        performanceInfo_.reset(nullptr);
    }
}

// ============================== specific implementation ======================================================

AceScopedPerformanceCheck::AceScopedPerformanceCheck(const std::string& name)
{
    if (AcePerformanceCheck::performanceInfo_) {
        // micro time.
        markTime_ = GetSysTimestamp();
        name_ = name;
    }
}

AceScopedPerformanceCheck::~AceScopedPerformanceCheck()
{
    if (AcePerformanceCheck::performanceInfo_) {
        // convert micro time to ms with 1000.
        auto time = static_cast<int64_t>((GetSysTimestamp() - markTime_) / CONVERT_NANOSECONDS);
        RecordFunctionTimeout(time, name_);
    }
}

bool AceScopedPerformanceCheck::CheckIsRuleContainsPage(const std::string& ruleType, const std::string& pagePath)
{
    // check for the presence of rule json
    if (!AcePerformanceCheck::performanceInfo_->Contains(ruleType)) {
        AcePerformanceCheck::performanceInfo_->Put(ruleType.c_str(), JsonUtil::CreateArray(false));
        return false;
    }
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue(ruleType);
    auto size = ruleJson->GetArraySize();
    for (int32_t i = 0; i < size; i++) {
        auto indexJson = ruleJson->GetArrayItem(i);
        auto value = indexJson->GetString("pagePath", {});
        if (value == pagePath) {
            return true;
        }
    }
    return false;
}

std::string AceScopedPerformanceCheck::GetCurrentTime()
{
    // get system date and time
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto* local = std::localtime(&now);
    if (!local) {
        LOGE("Get localtime failed");
        return {};
    }

    // this is for i18n date time
    DateTime dateTime;
    dateTime.year = static_cast<uint32_t>(local->tm_year + BASE_YEAR);
    dateTime.month = static_cast<uint32_t>(local->tm_mon);
    dateTime.day = static_cast<uint32_t>(local->tm_mday);
    dateTime.hour = static_cast<uint32_t>(local->tm_hour);
    dateTime.minute = static_cast<uint32_t>(local->tm_min);
    dateTime.second = static_cast<uint32_t>(local->tm_sec);
    auto time = Localization::GetInstance()->FormatDateTime(dateTime, DATE_FORMAT);
    return time;
}

CodeInfo AceScopedPerformanceCheck::GetCodeInfo(int32_t row, int32_t col)
{
    auto container = Container::Current();
    CHECK_NULL_RETURN(container, {});
    auto frontend = container->GetFrontend();
    CHECK_NULL_RETURN(frontend, {});
    auto sourceMap = frontend->GetCurrentPageSourceMap();
    CHECK_NULL_RETURN(sourceMap, {});
    auto codeInfo = sourceMap->Find(row, col);
    LOGD("codeInfo=%{public}d, %{public}d, %{public}s", codeInfo.row, codeInfo.col, codeInfo.sources.c_str());
    return { codeInfo.row, codeInfo.col, codeInfo.sources };
}

bool AceScopedPerformanceCheck::CheckPage(const CodeInfo& codeInfo, const std::string& rule)
{
    if (!codeInfo.sources.empty() && CheckIsRuleContainsPage(rule, codeInfo.sources)) {
        return true;
    }
    return false;
}

void AceScopedPerformanceCheck::RecordPerformanceCheckData(const PerformanceCheckNodeMap& nodeMap, int64_t vsyncTimeout)
{
    LOGD("AcePerformanceCheck::RecoredPerformanceCheckData()");
    auto codeInfo = GetCodeInfo(1, 1);
    std::vector<PerformanceCheckNode> pageNodeList;
    std::vector<PerformanceCheckNode> flexNodeList;
    std::unordered_map<int32_t, PerformanceCheckNode> foreachNodeMap;
    int32_t itemCount = 0;
    int32_t maxDepth = 0;
    for (const auto& node : nodeMap) {
        if (node.second.childrenSize >=
            SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::NODE_CHILDREN)) {
            pageNodeList.emplace_back(node.second);
        }
        if (node.second.pageDepth > maxDepth) {
            maxDepth = node.second.pageDepth;
        }
        if (node.second.flexLayouts != 0 &&
            node.second.flexLayouts >=
                SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::FLEX_LAYOUTS)) {
            flexNodeList.emplace_back(node.second);
        }
        if (node.second.isForEachItem) {
            itemCount++;
            auto iter = foreachNodeMap.find(node.second.codeRow);
            if (iter != foreachNodeMap.end()) {
                iter->second.foreachItems++;
            } else {
                foreachNodeMap.insert(std::make_pair(node.second.codeRow, node.second));
            }
        }
    }
    RecordPageNodeCountAndDepth(nodeMap.size(), maxDepth, pageNodeList, codeInfo);
    RecordForEachItemsCount(itemCount, foreachNodeMap, codeInfo);
    RecordFlexLayoutsCount(flexNodeList, codeInfo);
    RecordVsyncTimeout(nodeMap, vsyncTimeout / CONVERT_NANOSECONDS, codeInfo);
}

void AceScopedPerformanceCheck::RecordPageNodeCountAndDepth(
    int32_t pageNodeCount, int32_t pageDepth, std::vector<PerformanceCheckNode>& pageNodeList, const CodeInfo& codeInfo)
{
    if ((pageNodeCount < SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::PAGE_NODES) &&
            pageDepth < SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::PAGE_DEPTH)) ||
        CheckPage(codeInfo, "9901")) {
        return;
    }
    LOGD("AcePerformanceCheck::RecordPageNodeCountAndDepth()");
    auto eventTime = GetCurrentTime();
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue("9901");
    auto pageJson = JsonUtil::Create(false);
    pageJson->Put("eventTime", eventTime.c_str());
    pageJson->Put("pagePath", codeInfo.sources.c_str());
    pageJson->Put("nodeCount", pageNodeCount);
    pageJson->Put("depth", pageDepth);
    // add children size > 100 of component to pageJson
    for (const auto& iter : pageNodeList) {
        auto componentJson = JsonUtil::Create(false);
        componentJson->Put("name", iter.nodeTag.c_str());
        componentJson->Put("items", iter.childrenSize);
        componentJson->Put("sourceLine", GetCodeInfo(iter.codeRow, iter.codeCol).row);
        std::unique_ptr<JsonValue> componentsJson;
        if (pageJson->Contains("components")) {
            componentsJson = pageJson->GetValue("components");
            componentsJson->Put(componentJson);
        } else {
            componentsJson = JsonUtil::CreateArray(false);
            componentsJson->Put(componentJson);
            pageJson->Put("components", componentsJson);
        }
    }
    ruleJson->Put(pageJson);
}

void AceScopedPerformanceCheck::RecordFunctionTimeout(int64_t time, const std::string& functionName)
{
    if (time < SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::FUNCTION_TIMEOUT)) {
        return;
    }
    auto codeInfo = GetCodeInfo(1, 1);
    if (!codeInfo.sources.empty() && CheckIsRuleContainsPage("9902", codeInfo.sources)) {
        return;
    }
    LOGD("AcePerformanceCheck::RecordFunctionTimeout()");
    auto eventTime = GetCurrentTime();
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue("9902");
    auto pageJson = JsonUtil::Create(false);
    pageJson->Put("eventTime", eventTime.c_str());
    pageJson->Put("pagePath", codeInfo.sources.c_str());
    pageJson->Put("functionName", functionName.c_str());
    pageJson->Put("costTime", time);
    ruleJson->Put(pageJson);
}

void AceScopedPerformanceCheck::RecordVsyncTimeout(
    const PerformanceCheckNodeMap& nodeMap, int64_t vsyncTimeout, const CodeInfo& codeInfo)
{
    if (vsyncTimeout < SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::VSYNC_TIMEOUT) ||
        CheckPage(codeInfo, "9903")) {
        return;
    }
    LOGD("AcePerformanceCheck::RecordVsyncTimeout()");
    auto eventTime = GetCurrentTime();
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue("9903");
    auto pageJson = JsonUtil::Create(false);
    pageJson->Put("eventTime", eventTime.c_str());
    pageJson->Put("pagePath", codeInfo.sources.c_str());
    pageJson->Put("costTime", vsyncTimeout);
    for (const auto& node : nodeMap) {
        int64_t layoutTime = node.second.layoutTime / CONVERT_NANOSECONDS;
        if (layoutTime != 0 &&
            layoutTime >= SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::NODE_TIMEOUT) &&
            node.second.nodeTag != "page" && node.second.nodeTag != "ContainerModal" &&
            node.second.nodeTag != "JsView") {
            auto componentJson = JsonUtil::Create(false);
            componentJson->Put("name", node.second.nodeTag.c_str());
            componentJson->Put("costTime", layoutTime);
            componentJson->Put("sourceLine", GetCodeInfo(node.second.codeRow, node.second.codeCol).row);
            std::unique_ptr<JsonValue> componentsJson;
            if (pageJson->Contains("components")) {
                componentsJson = pageJson->GetValue("components");
                componentsJson->Put(componentJson);
            } else {
                componentsJson = JsonUtil::CreateArray(false);
                componentsJson->Put(componentJson);
                pageJson->Put("components", componentsJson);
            }
        }
    }
    ruleJson->Put(pageJson);
}

void AceScopedPerformanceCheck::RecordForEachItemsCount(
    int32_t count, std::unordered_map<int32_t, PerformanceCheckNode>& foreachNodeMap, const CodeInfo& codeInfo)
{
    if (count == 0 ||
        count < SystemProperties::GetPerformanceParameterWithType(PerformanceParameterType::FOREACH_ITEMS) ||
        CheckPage(codeInfo, "9904")) {
        return;
    }
    LOGD("AcePerformanceCheck::RecordForEachItemsCount()");
    auto eventTime = GetCurrentTime();
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue("9904");
    auto pageJson = JsonUtil::Create(false);
    pageJson->Put("eventTime", eventTime.c_str());
    pageJson->Put("pagePath", codeInfo.sources.c_str());
    for (const auto& iter : foreachNodeMap) {
        auto componentJson = JsonUtil::Create(false);
        componentJson->Put("name", iter.second.nodeTag.c_str());
        componentJson->Put("items", iter.second.foreachItems + 1);
        componentJson->Put("sourceLine", GetCodeInfo(iter.second.codeRow, iter.second.codeCol).row);
        std::unique_ptr<JsonValue> componentsJson;
        if (pageJson->Contains("components")) {
            componentsJson = pageJson->GetValue("components");
            componentsJson->Put(componentJson);
        } else {
            componentsJson = JsonUtil::CreateArray(false);
            componentsJson->Put(componentJson);
            pageJson->Put("components", componentsJson);
        }
    }
    ruleJson->Put(pageJson);
}

void AceScopedPerformanceCheck::RecordFlexLayoutsCount(
    const std::vector<PerformanceCheckNode>& flexNodeList, const CodeInfo& codeInfo)
{
    if (flexNodeList.empty() || CheckPage(codeInfo, "9905")) {
        return;
    }
    LOGD("AcePerformanceCheck::RecordFlexLayoutsCount()");
    auto eventTime = GetCurrentTime();
    auto ruleJson = AcePerformanceCheck::performanceInfo_->GetValue("9905");
    auto pageJson = JsonUtil::Create(false);
    pageJson->Put("eventTime", eventTime.c_str());
    pageJson->Put("pagePath", codeInfo.sources.c_str());
    for (auto& node : flexNodeList) {
        auto componentJson = JsonUtil::Create(false);
        componentJson->Put("name", node.nodeTag.c_str());
        componentJson->Put("flexTime", node.flexLayouts);
        componentJson->Put("sourceLine", GetCodeInfo(node.codeRow, node.codeCol).row);
        std::unique_ptr<JsonValue> componentsJson;
        if (pageJson->Contains("components")) {
            componentsJson = pageJson->GetValue("components");
            componentsJson->Put(componentJson);
        } else {
            componentsJson = JsonUtil::CreateArray(false);
            componentsJson->Put(componentJson);
            pageJson->Put("components", componentsJson);
        }
    }
    ruleJson->Put(pageJson);
}
} // namespace OHOS::Ace
