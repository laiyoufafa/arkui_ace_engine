/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "core/common/test/mock/mock_clipboard.h"

#include "gtest/gtest.h"

#include "core/common/clipboard/clipboard_proxy.h"
namespace OHOS::Ace {
namespace {
std::optional<std::string> saveData;
}
ClipboardProxy* ClipboardProxy::inst_ = nullptr;
std::mutex ClipboardProxy::mutex_;

ClipboardProxy* ClipboardProxy::GetInstance()
{
    if (inst_ == nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (inst_ == nullptr) {
            inst_ = new ClipboardProxy();
        }
    }
    return inst_;
}

void ClipboardProxy::SetDelegate(std::unique_ptr<ClipboardInterface>&& delegate)
{
    delegate_ = std::move(delegate);
}

RefPtr<Clipboard> ClipboardProxy::GetClipboard(const RefPtr<TaskExecutor>& taskExecutor) const
{
    auto clipboard = AceType::MakeRefPtr<MockClipBoard>(taskExecutor);
    return clipboard;
}

void MockClipBoard::HasData(const std::function<void(bool hasData)>& callback)
{
    if (callback) {
        callback(saveData.has_value());
    }
}

void MockClipBoard::SetData(const std::string& data, CopyOptions copyOption, bool isDragData)
{
    saveData = data;
    GTEST_LOG_(INFO) << "SetData has called. data is { " << data << " }";
}

void MockClipBoard::GetData(const std::function<void(const std::string&)>& callback, bool syncMode)
{
    if (callback) {
        callback(saveData.value_or(""));
    }
    GTEST_LOG_(INFO) << "GetData has called";
}
} // namespace OHOS::Ace
