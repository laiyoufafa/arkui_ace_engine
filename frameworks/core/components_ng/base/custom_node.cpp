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

#include "core/components_ng/base/custom_node.h"

#include "base/log/dump_log.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/pattern/custom/custom_node_pattern.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/pipeline_ng/pipeline_context.h"
#include "core/pipeline_ng/ui_task_scheduler.h"

namespace OHOS::Ace::NG {
RefPtr<CustomNode> CustomNode::CreateCustomNode(const std::string& id)
{
    auto node = MakeRefPtr<CustomNode>(id);
    node->InitializePatternAndContext();
    return node;
}

CustomNode::CustomNode(const std::string& id) : FrameNode(V2::JS_VIEW_ETS_TAG, id, MakeRefPtr<CustomNodePattern>()) {}

CustomNode::~CustomNode() = default;

void CustomNode::OnContextAttached()
{
    if (UpdateJsViewFunction_) {
        UpdateJsViewFunction_(WeakClaim(this));
    }
    if (appearFunction_) {
        appearFunction_();
    }
}

void CustomNode::Rebuild() {}

void CustomNode::MarkNeedRebuild()
{
    auto context = context_.Upgrade();
    if (!context) {
        LOGE("context is nullptr, fail to push async task");
        return;
    }
    if (needRebuild_) {
        return;
    }
    needRebuild_ = true;
    context->AddDirtyComposedNode(Claim(this));
}
} // namespace OHOS::Ace::NG
