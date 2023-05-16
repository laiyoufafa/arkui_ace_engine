/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "core/components_ng/pattern/checkbox/checkbox_model_ng.h"

#include "base/utils/utils.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/checkbox/checkbox_pattern.h"

namespace OHOS::Ace::NG {

void CheckBoxModelNG::Create(
    const std::optional<std::string>& name, const std::optional<std::string>& groupName, const std::string& tagName)
{
    auto* stack = ViewStackProcessor::GetInstance();
    int32_t nodeId = stack->ClaimNodeId();
    auto frameNode =
        FrameNode::GetOrCreateFrameNode(tagName, nodeId, []() { return AceType::MakeRefPtr<CheckBoxPattern>(); });
    ViewStackProcessor::GetInstance()->Push(frameNode);

    auto eventHub = frameNode->GetEventHub<NG::CheckBoxEventHub>();
    if (name.has_value()) {
        eventHub->SetName(name.value());
    }
    if (groupName.has_value()) {
        eventHub->SetGroupName(groupName.value());
    }

    auto pipeline = PipelineBase::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto checkBoxTheme = pipeline->GetTheme<CheckboxTheme>();
    CHECK_NULL_VOID(checkBoxTheme);
    auto width = checkBoxTheme->GetWidth();
    auto height = checkBoxTheme->GetHeight();
    auto padding = checkBoxTheme->GetDefaultPadding();
    auto layoutProperty = frameNode->GetLayoutProperty();
    CHECK_NULL_VOID(layoutProperty);
    layoutProperty->UpdateUserDefinedIdealSize(CalcSize(CalcLength(width), CalcLength(height)));
    PaddingProperty defaultPadding(
        { CalcLength(padding), CalcLength(padding), CalcLength(padding), CalcLength(padding) });
    ACE_UPDATE_LAYOUT_PROPERTY(LayoutProperty, Padding, defaultPadding);
}

void CheckBoxModelNG::SetSelect(bool isSelected)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxSelect, isSelected);
}

void CheckBoxModelNG::SetSelectedColor(const Color& color)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxSelectedColor, color);
}

void CheckBoxModelNG::SetUnSelectedColor(const Color& color)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxUnSelectedColor, color);
}

void CheckBoxModelNG::SetCheckMarkColor(const Color& color)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxCheckMarkColor, color);
}

void CheckBoxModelNG::SetCheckMarkSize(const Dimension& size)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxCheckMarkSize, size);
}

void CheckBoxModelNG::SetCheckMarkWidth(const Dimension& width)
{
    ACE_UPDATE_PAINT_PROPERTY(CheckBoxPaintProperty, CheckBoxCheckMarkWidth, width);
}

void CheckBoxModelNG::SetOnChange(ChangeEvent&& onChange)
{
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    CHECK_NULL_VOID(frameNode);
    auto eventHub = frameNode->GetEventHub<CheckBoxEventHub>();
    CHECK_NULL_VOID(eventHub);
    eventHub->SetOnChange(std::move(onChange));
}

void CheckBoxModelNG::SetWidth(const Dimension& width) {}

void CheckBoxModelNG::SetHeight(const Dimension& height) {}

void CheckBoxModelNG::SetPadding(const NG::PaddingPropertyF& args) {}

} // namespace OHOS::Ace::NG
