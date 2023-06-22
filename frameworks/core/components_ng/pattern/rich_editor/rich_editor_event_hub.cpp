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

#include "core/components_ng/pattern/rich_editor/rich_editor_event_hub.h"

#include "core/components_ng/base/frame_node.h"
namespace OHOS::Ace::NG {
void RichEditorEventHub::SetOnReady(std::function<void()>&& func)
{
    onReady_ = std::move(func);
}

void RichEditorEventHub::FireOnReady()
{
    if (onReady_) {
        onReady_();
        auto host = GetFrameNode();
        CHECK_NULL_VOID(host);
        host->PostTask([host]() { host->MarkDirtyNode(PROPERTY_UPDATE_MEASURE); });
    }
}
} // namespace OHOS::Ace::NG
