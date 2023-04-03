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

#include "frameworks/core/components_ng/pattern/stack/stack_layout_algorithm.h"


#include "core/common/ace_application_info.h"
#include "core/components_ng/layout/layout_wrapper.h"
#include "core/components_ng/pattern/stack/stack_layout_property.h"

namespace OHOS::Ace::NG {

StackLayoutAlgorithm::StackLayoutAlgorithm() = default;

void StackLayoutAlgorithm::Layout(LayoutWrapper* layoutWrapper)
{
    PerformLayout(layoutWrapper);
    for (auto&& child : layoutWrapper->GetAllChildrenWithBuild()) {
        child->Layout();
    }
}

// Called to perform layout render node and child.
void StackLayoutAlgorithm::PerformLayout(LayoutWrapper* layoutWrapper)
{
    // update child position.
    auto size = layoutWrapper->GetGeometryNode()->GetFrameSize();
    const auto& padding = layoutWrapper->GetLayoutProperty()->CreatePaddingAndBorder();
    auto layoutDirection = layoutWrapper->GetLayoutProperty()->GetLayoutDirection();
    if (layoutDirection == TextDirection::AUTO) {
        layoutDirection = AceApplicationInfo::GetInstance().IsRightToLeft() ? TextDirection::RTL : TextDirection::LTR;
    }
    MinusPaddingToSize(padding, size);
    auto left = padding.left.value_or(0);
    auto top = padding.top.value_or(0);
    auto paddingOffset = OffsetF(left, top);
    auto align = Alignment::CENTER;
    auto layoutProperty = DynamicCast<StackLayoutProperty>(layoutWrapper->GetLayoutProperty());
    CHECK_NULL_VOID(layoutProperty);
    if (layoutProperty->HasAlignment()) {
        align = layoutProperty->GetAlignment().value_or(Alignment::CENTER);
    }
    if (layoutProperty->HasAlignmentContent()) {
        align = layoutProperty->GetAlignmentContent().value_or(Alignment::CENTER);
    }
    // Update child position.
    for (const auto& child : layoutWrapper->GetAllChildrenWithBuild()) {
        auto translate =
            Alignment::GetAlignPosition(size, child->GetGeometryNode()->GetMarginFrameSize(), align) + paddingOffset;
        if (layoutDirection == TextDirection::RTL) {
            translate.SetX(size.Width() - translate.GetX() - child->GetGeometryNode()->GetMarginFrameSize().Width());
        }
        child->GetGeometryNode()->SetMarginFrameOffset(translate);
    }
    // Update content position.
    const auto& content = layoutWrapper->GetGeometryNode()->GetContent();
    if (content) {
        auto translate = Alignment::GetAlignPosition(size, content->GetRect().GetSize(), align) + paddingOffset;
        if (layoutDirection == TextDirection::RTL) {
            translate.SetX(size.Width() - translate.GetX() - content->GetRect().GetSize().Width());
        }
        content->SetOffset(translate);
    }
}
} // namespace OHOS::Ace::NG