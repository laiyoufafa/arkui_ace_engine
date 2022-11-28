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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_SHAPE_RECT_MODEL_NG_CPP
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_SHAPE_RECT_MODEL_NG_CPP

#include "core/components_ng/pattern/shape/rect_model_ng.h"

#include "base/utils/utils.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/shape/rect_pattern.h"
#include "core/components_v2/inspector/inspector_constants.h"

namespace OHOS::Ace::NG {

void RectModelNG::Create()
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode =
        FrameNode::GetOrCreateFrameNode(V2::RECT_ETS_TAG, nodeId, []() { return AceType::MakeRefPtr<RectPattern>(); });
    stack->Push(frameNode);
}

void RectModelNG::SetRadiusWidth(const Dimension& value)
{
    Radius radius;
    radius.SetX(value);
    RectModelNG::UpdateRadius(radius);
}

void RectModelNG::SetRadiusHeight(const Dimension& value)
{
    Radius radius;
    radius.SetY(value);
    RectModelNG::UpdateRadius(radius);
}

void RectModelNG::SetRadiusValue(const Dimension& radiusX, const Dimension& radiusY, int32_t index)
{
    NG::Radius radius = NG::Radius(radiusX, radiusY);
    switch (index) {
        case TOP_LEFT_RADIUS:
            RectModelNG::SetTopLeftRadius(radius);
            break;
        case TOP_RIGHT_RADIUS:
            RectModelNG::SetTopRightRadius(radius);
            break;
        case BOTTOM_RIGHT_RADIUS:
            RectModelNG::SetBottomRightRadius(radius);
            break;
        case BOTTOM_LEFT_RADIUS:
            RectModelNG::SetBottomLeftRadius(radius);
            break;
    }
}

void RectModelNG::UpdateRadius(const Radius& radius)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    auto castRectPaintProperty = frameNode->GetPaintProperty<RectPaintProperty>();
    if (castRectPaintProperty) {
        castRectPaintProperty->UpdateTopLeftRadius(radius);
        castRectPaintProperty->UpdateTopRightRadius(radius);
        castRectPaintProperty->UpdateBottomLeftRadius(radius);
        castRectPaintProperty->UpdateBottomRightRadius(radius);
    }
}

void RectModelNG::SetTopLeftRadius(const Radius& topLeftRadius)
{
    ACE_UPDATE_PAINT_PROPERTY(RectPaintProperty, TopLeftRadius, topLeftRadius);
}

void RectModelNG::SetTopRightRadius(const Radius& topRightRadius)
{
    ACE_UPDATE_PAINT_PROPERTY(RectPaintProperty, TopRightRadius, topRightRadius);
}

void RectModelNG::SetBottomLeftRadius(const Radius& bottomLeftRadius)
{
    ACE_UPDATE_PAINT_PROPERTY(RectPaintProperty, BottomLeftRadius, bottomLeftRadius);
}

void RectModelNG::SetBottomRightRadius(const Radius& bottomRightRadius)
{
    ACE_UPDATE_PAINT_PROPERTY(RectPaintProperty, BottomRightRadius, bottomRightRadius);
}

template<class T>
void RectModelNG::SetCallbackRadius(
    const RefPtr<T>& component, const Dimension& radiusX, const Dimension& radiusY, int32_t index)
{
    CHECK_NULL_VOID_NOLOG(component);
    Radius newRadius = Radius(Dimension(radiusX), Dimension(radiusY));
    switch (index) {
        case TOP_LEFT_RADIUS:
            component->SetTopLeftRadius(newRadius);
            break;
        case TOP_RIGHT_RADIUS:
            component->SetTopRightRadius(newRadius);
            break;
        case BOTTOM_RIGHT_RADIUS:
            component->SetBottomRightRadius(newRadius);
            break;
        case BOTTOM_LEFT_RADIUS:
            component->SetBottomLeftRadius(newRadius);
            break;
        default:
            break;
    }
}

} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_SHAPE_RECT_MODEL_NG_CPP