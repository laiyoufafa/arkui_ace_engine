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

#include "core/components_ng/syntax/if_else_model_ng.h"

#include "base/utils/utils.h"
#include "core/common/container.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/syntax/if_else_node.h"
#include "core/components_ng/syntax/syntax_item.h"

namespace OHOS::Ace::NG {

void IfElseModelNG::Create()
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto ifElseNode = IfElseNode::GetOrCreateIfElseNode(nodeId);
    stack->Push(ifElseNode);
}

void IfElseModelNG::Pop()
{
    ViewStackProcessor::GetInstance()->PopContainer();
}

void IfElseModelNG::SetBranchId(int32_t value)
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto node = AceType::DynamicCast<IfElseNode>(stack->GetMainElementNode());
    if (node == nullptr) {
        LOGE("IfElseNode is null, update IfElse failed"); 
        return;
    }
    node->SetBranchId(value);
}

int32_t IfElseModelNG::GetBranchId()
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto node = AceType::DynamicCast<IfElseNode>(stack->GetMainElementNode());
    if (node == nullptr) {
        LOGE("IfElseNode is null, update IfElse failed"); 
        return -1;  // invalid branchId
    }
    return node->GetBranchId();
}

} // namespace OHOS::Ace::NG
