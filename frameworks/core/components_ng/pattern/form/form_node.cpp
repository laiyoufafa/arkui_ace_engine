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

#include "core/components_ng/pattern/form/form_node.h"

#include "base/utils/utils.h"
#include "core/components/form/sub_container.h"
#include "core/components_ng/pattern/form/form_pattern.h"
#include "core/pipeline/pipeline_context.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace::NG {

HitTestResult FormNode::TouchTest(const PointF& globalPoint, const PointF& parentLocalPoint,
    const TouchRestrict& touchRestrict, TouchTestResult& result)
{
    const auto& rect = GetGeometryNode()->GetFrameRect();
    if (!rect.IsInRegion(parentLocalPoint)) {
        return HitTestResult::OUT_OF_REGION;
    }
    auto pattern = GetPattern<FormPattern>();
    CHECK_NULL_RETURN(pattern, HitTestResult::BUBBLING);
    auto subContainer = pattern->GetSubContainer();
    CHECK_NULL_RETURN(subContainer, HitTestResult::BUBBLING);

    auto subContext = DynamicCast<OHOS::Ace::PipelineBase>(subContainer->GetPipelineContext());
    CHECK_NULL_RETURN(subContext, HitTestResult::BUBBLING);
    auto selfGlobalOffset = GetOffsetRelativeToWindow();
    subContext->SetPluginEventOffset(Offset(selfGlobalOffset.GetX(), selfGlobalOffset.GetY()));

    auto context = GetContext();
    if (context) {
        context->SetTouchPipeline(WeakPtr<PipelineBase>(subContext));
    }

    return HitTestResult::BUBBLING;
}

RefPtr<FormNode> FormNode::GetOrCreateFormNode(
    const std::string& tag, int32_t nodeId, const std::function<RefPtr<Pattern>(void)>& patternCreator)
{
    auto formNode = ElementRegister::GetInstance()->GetSpecificItemById<FormNode>(nodeId);
    if (formNode) {
        if (formNode->GetTag() == tag) {
            return formNode;
        }
        ElementRegister::GetInstance()->RemoveItemSilently(nodeId);
        auto parent = formNode->GetParent();
        if (parent) {
            parent->RemoveChild(formNode);
        }
    }

    auto pattern = patternCreator ? patternCreator() : AceType::MakeRefPtr<Pattern>();
    formNode = AceType::MakeRefPtr<FormNode>(tag, nodeId, pattern, false);
    formNode->InitializePatternAndContext();
    ElementRegister::GetInstance()->AddUINode(formNode);
    return formNode;
}

} // namespace OHOS::Ace::NG