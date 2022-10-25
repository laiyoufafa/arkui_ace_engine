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

#include "core/components_ng/pattern/scroll/scroll_model_ng.h"

#include "base/utils/utils.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/scroll/scroll_event_hub.h"
#include "core/components_ng/pattern/scroll/scroll_paint_property.h"
#include "core/components_ng/pattern/scroll/scroll_pattern.h"
#include "core/components_ng/pattern/scroll/scroll_position_controller.h"
#include "core/components_ng/pattern/scroll/scroll_spring_effect.h"
#include "core/components_v2/inspector/inspector_constants.h"

namespace OHOS::Ace::NG {
namespace {
const std::vector<DisplayMode> DISPLAY_MODE = { DisplayMode::OFF, DisplayMode::AUTO, DisplayMode::ON };
}

void ScrollModelNG::Create()
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::GetOrCreateFrameNode(
        V2::SCROLL_ETS_TAG, nodeId, []() { return AceType::MakeRefPtr<ScrollPattern>(); });
    stack->Push(frameNode);
    SetEdgeEffect(EdgeEffect::NONE);
    auto pattern = frameNode->GetPattern<ScrollPattern>();
    CHECK_NULL_VOID(pattern);
    auto positionController = AceType::MakeRefPtr<NG::ScrollPositionController>();
    pattern->SetScrollPositionController(positionController);
    positionController->SetScrollPattern(pattern);
}

RefPtr<ScrollControllerBase> ScrollModelNG::GetOrCreateController()
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_RETURN(frameNode, nullptr);
    auto pattern = frameNode->GetPattern<ScrollPattern>();
    CHECK_NULL_RETURN(pattern, nullptr);
    if (!pattern->GetScrollPositionController()) {
        auto controller = AceType::MakeRefPtr<NG::ScrollPositionController>();
        pattern->SetScrollPositionController(controller);
    }
    return pattern->GetScrollPositionController();
}

void ScrollModelNG::SetAxis(Axis axis)
{
    ACE_UPDATE_LAYOUT_PROPERTY(ScrollLayoutProperty, Axis, axis);
    ACE_UPDATE_LAYOUT_PROPERTY(ScrollPaintProperty, Axis, axis);
}

void ScrollModelNG::SetOnScrollBegin(NG::ScrollBeginEvent&& event)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto eventHub = frameNode->GetEventHub<ScrollEventHub>();
    CHECK_NULL_VOID(eventHub);
    eventHub->SetOnScrollBegin(std::move(event));
}

void ScrollModelNG::SetOnScroll(NG::OnScrollEvent&& event)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto eventHub = frameNode->GetEventHub<ScrollEventHub>();
    CHECK_NULL_VOID(eventHub);
    eventHub->SetOnScroll(std::move(event));
}

void ScrollModelNG::SetOnScrollEdge(NG::ScrollEdgeEvent&& event)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto eventHub = frameNode->GetEventHub<ScrollEventHub>();
    CHECK_NULL_VOID(eventHub);
    eventHub->SetOnScrollEdge(std::move(event));
}

void ScrollModelNG::SetOnScrollEnd(NG::ScrollEndEvent&& event)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto eventHub = frameNode->GetEventHub<ScrollEventHub>();
    CHECK_NULL_VOID(eventHub);
    eventHub->SetOnScrollEnd(std::move(event));
}

void ScrollModelNG::SetScrollBarProxy(const RefPtr<ScrollBarProxy>& proxy) {}

void ScrollModelNG::InitScrollBar(const RefPtr<ScrollBarTheme>& theme, const std::pair<bool, Color>& color,
    const std::pair<bool, Dimension>& width, EdgeEffect effect)
{}

void ScrollModelNG::SetDisplayMode(int displayMode)
{
    if (displayMode < 0 || displayMode >= static_cast<int32_t>(DISPLAY_MODE.size())) {
        return;
    }
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto paintProperty = frameNode->GetPaintProperty<ScrollPaintProperty>();
    CHECK_NULL_VOID(paintProperty);
    paintProperty->SetDisplayMode(DISPLAY_MODE[displayMode]);
}

void ScrollModelNG::SetScrollBarWidth(const Dimension& dimension)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto paintProperty = frameNode->GetPaintProperty<ScrollPaintProperty>();
    paintProperty->SetScrollBarWidth(dimension);
}

void ScrollModelNG::SetScrollBarColor(const Color& color)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto paintProperty = frameNode->GetPaintProperty<ScrollPaintProperty>();
    CHECK_NULL_VOID(paintProperty);
    paintProperty->SetScrollBarColor(color);
}

void ScrollModelNG::SetEdgeEffect(EdgeEffect edgeEffect)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto pattern = frameNode->GetPattern<ScrollPattern>();
    CHECK_NULL_VOID(pattern);
    auto effect = static_cast<EdgeEffect>(edgeEffect);
    RefPtr<ScrollEdgeEffect> scrollEdgeEffect;
    if (effect == EdgeEffect::SPRING) {
        scrollEdgeEffect = AceType::MakeRefPtr<ScrollSpringEffect>();
    } else if (effect == EdgeEffect::FADE) {
        LOGW("current not support fade egde effect");
        return;
    } else {
        scrollEdgeEffect = AceType::MakeRefPtr<ScrollEdgeEffect>(EdgeEffect::NONE);
    }
    pattern->SetScrollEdgeEffect(scrollEdgeEffect);
    auto layoutProperty = frameNode->GetLayoutProperty<ScrollLayoutProperty>();
    CHECK_NULL_VOID(layoutProperty);
    layoutProperty->SetScrollEdgeEffect(scrollEdgeEffect);
}

} // namespace OHOS::Ace::NG
