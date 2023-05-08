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

#include "core/components_ng/pattern/scroll_bar/scroll_bar_layout_algorithm.h"

#include <algorithm>

#include "base/geometry/axis.h"
#include "base/geometry/ng/offset_t.h"
#include "base/geometry/ng/size_t.h"
#include "base/log/ace_trace.h"
#include "base/utils/utils.h"
#include "core/components_ng/pattern/scroll_bar/scroll_bar_layout_property.h"
#include "core/components_ng/pattern/scroll_bar/scroll_bar_pattern.h"
#include "core/components_ng/property/layout_constraint.h"
#include "core/components_ng/property/measure_property.h"
#include "core/components_ng/property/measure_utils.h"

namespace OHOS::Ace::NG {
namespace {

void UpdateChildConstraint(Axis axis, const OptionalSizeF& selfIdealSize, LayoutConstraintF& contentConstraint)
{
    contentConstraint.parentIdealSize = selfIdealSize;
    if (axis == Axis::VERTICAL) {
        contentConstraint.maxSize.SetHeight(Infinity<float>());
    } else {
        contentConstraint.maxSize.SetWidth(Infinity<float>());
    }
}

void UpdateIdealSize(Axis axis, const SizeF& childSize, const OptionalSizeF& parentSize, OptionalSizeF& idealSize)
{
    if (axis == Axis::HORIZONTAL) {
        if (!idealSize.Height()) {
            idealSize.SetHeight(childSize.Height());
        }
        if (!idealSize.Width() && parentSize.Width().has_value()) {
            idealSize.SetWidth(parentSize.Width().value());
        }
        return;
    }
    if (!idealSize.Width()) {
        idealSize.SetWidth(childSize.Width());
    }
    if (!idealSize.Height() && parentSize.Height().has_value()) {
        idealSize.SetHeight(parentSize.Height().value());
    }
}

} // namespace

void ScrollBarLayoutAlgorithm::Measure(FrameNode* frameNode)
{
    auto layoutProperty = AceType::DynamicCast<ScrollBarLayoutProperty>(frameNode->GetLayoutProperty());
    CHECK_NULL_VOID(layoutProperty);

    auto axis = layoutProperty->GetAxis().value_or(Axis::VERTICAL);
    auto constraint = layoutProperty->GetLayoutConstraint();
    auto idealSize = CreateIdealSize(constraint.value(), axis, MeasureType::MATCH_CONTENT);
    auto parentSize = CreateIdealSize(constraint.value(), axis, MeasureType::MATCH_PARENT);

    // Calculate child layout constraint.
    auto childLayoutConstraint = layoutProperty->CreateChildConstraint();
    UpdateChildConstraint(axis, idealSize, childLayoutConstraint);

    // Measure child.
    auto child = frameNode->GetFrameNodeByIndex(0);
    CHECK_NULL_VOID(child);
    child->Measure(childLayoutConstraint);

    // Use child size when self idea size of scroll is not setted.
    auto childSize = child->GetGeometryNode()->GetMarginFrameSize();
    UpdateIdealSize(axis, childSize, parentSize, idealSize);
    auto selfSize = idealSize.ConvertToSizeT();
    selfSize.Constrain(constraint->minSize, constraint->maxSize);
    frameNode->GetGeometryNode()->SetFrameSize(selfSize);
}

void ScrollBarLayoutAlgorithm::Layout(FrameNode* frameNode)
{

    auto layoutProperty = AceType::DynamicCast<ScrollBarLayoutProperty>(frameNode->GetLayoutProperty());
    CHECK_NULL_VOID(layoutProperty);
    auto axis = layoutProperty->GetAxis().value_or(Axis::VERTICAL);
    auto geometryNode = frameNode->GetGeometryNode();
    CHECK_NULL_VOID(geometryNode);
    auto child = frameNode->GetFrameNodeByIndex(0);
    CHECK_NULL_VOID(child);
    auto childGeometryNode = child->GetGeometryNode();
    CHECK_NULL_VOID(childGeometryNode);
    auto size = geometryNode->GetFrameSize();

    auto padding = layoutProperty->CreatePaddingAndBorder();
    MinusPaddingToSize(padding, size);
    auto childSize = childGeometryNode->GetMarginFrameSize();
    scrollableDistance_ = std::abs(GetMainAxisSize(size, axis) - GetMainAxisSize(childSize, axis));
    auto scrollBarPattern = AceType::DynamicCast<ScrollBarPattern>(frameNode->GetPattern());
    auto controlDistance = scrollBarPattern->GetControlDistance();
    auto scrollOffset = scrollBarPattern->GetScrollOffset();
    float currentOffset = 0.0f;
    if (!NearZero(controlDistance)) {
        currentOffset = scrollOffset * scrollableDistance_ / controlDistance;
    } else {
        LOGW("scroll bar scrollable distance is zero");
    }
    currentOffset = std::clamp(currentOffset, 0.0f, scrollableDistance_);
    if (scrollableDistance_ > 0.0f) {
        currentOffset_ = currentOffset;
    }
    scrollBarPattern->SetCurrentPosition(currentOffset_);
    auto currentAxisOffset = axis == Axis::VERTICAL ? OffsetF(0.0f, currentOffset_) : OffsetF(currentOffset_, 0.0f);
    childGeometryNode->SetMarginFrameOffset(padding.Offset() + currentAxisOffset);
    child->Layout();
}

} // namespace OHOS::Ace::NG
