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

#ifndef FOUNDATION_ACE_TEST_MOCK_CORE_COMMON_MOCK_UDMF_H
#define FOUNDATION_ACE_TEST_MOCK_CORE_COMMON_MOCK_UDMF_H

#include "gmock/gmock-function-mocker.h"
#include "gmock/gmock.h"

#include "base/memory/ace_type.h"
#include "core/common/udmf/udmf_client.h"

namespace OHOS::Ace {
class MockUdmfClient : public UdmfClient {
    DECLARE_ACE_TYPE(MockUdmfClient, UdmfClient);

public:
    MOCK_METHOD(RefPtr<UnifiedData>, TransformUnifiedData, (NativeValue* nativeValue), (override));
    MOCK_METHOD(NativeValue*, TransformUdmfUnifiedData, (RefPtr<UnifiedData>& UnifiedData), (override));
    MOCK_METHOD(NativeValue*, TransformSummary, ((std::map<std::string, int64_t>& summary)), (override));
    MOCK_METHOD(RefPtr<UnifiedData>, CreateUnifiedData, (), (override));
    MOCK_METHOD(int32_t, SetData, (const RefPtr<UnifiedData>& unifiedData, std::string& key), (override));
    MOCK_METHOD(int32_t, GetData, (const RefPtr<UnifiedData>& unifiedData, const std::string& key), (override));
    MOCK_METHOD(int32_t, GetSummary, (std::string& key, (std::map<std::string, int64_t>& summaryMap)), (override));
    MOCK_METHOD(void, AddFormRecord,
        (const RefPtr<UnifiedData>& unifiedData, int32_t formId, const RequestFormInfo& cardInfo), (override));
    MOCK_METHOD(void, AddLinkRecord,
        (const RefPtr<UnifiedData>& unifiedData, const std::string& url, const std::string& description), (override));
    MOCK_METHOD(
        void, AddPixelMapRecord, (const RefPtr<UnifiedData>& unifiedData, (std::vector<uint8_t>& data)), (override));
    MOCK_METHOD(void, AddImageRecord, (const RefPtr<UnifiedData>& unifiedData, const std::string& uri), (override));
    MOCK_METHOD(
        void, AddTextRecord, (const RefPtr<UnifiedData>& unifiedData, const std::string& selectedStr), (override));
    MOCK_METHOD(std::string, GetSingleTextRecord, (const RefPtr<UnifiedData>& unifiedData), (override));
    MOCK_METHOD(int32_t, GetVideoRecordUri, (const RefPtr<UnifiedData>& unifiedData, std::string& uri), (override));
};
class MockUnifiedData : public UnifiedData {
    DECLARE_ACE_TYPE(MockUnifiedData, UnifiedData);

public:
    MockUnifiedData() {};
    ~MockUnifiedData() = default;
    MOCK_METHOD(int64_t, GetSize, (), (override));
};
} // namespace OHOS::Ace
#endif // FOUNDATION_ACE_TEST_MOCK_CORE_COMMON_MOCK_UDMF_H