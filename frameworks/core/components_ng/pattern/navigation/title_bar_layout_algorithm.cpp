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

#include "core/components_ng/pattern/navigation/title_bar_layout_algorithm.h"

#include "base/geometry/ng/offset_t.h"
#include "base/geometry/ng/size_t.h"
#include "base/memory/ace_type.h"
#include "base/utils/measure_util.h"
#include "base/utils/utils.h"
#include "core/components/custom_paint/rosen_render_custom_paint.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/pattern/navigation/navigation_declaration.h"
#include "core/components_ng/pattern/navigation/navigation_layout_property.h"
#include "core/components_ng/pattern/navigation/title_bar_layout_property.h"
#include "core/components_ng/pattern/navigation/title_bar_node.h"
#include "core/components_ng/pattern/navigation/title_bar_pattern.h"
#include "core/components_ng/pattern/text/text_layout_property.h"
#include "core/components_ng/property/layout_constraint.h"
#include "core/components_ng/property/measure_property.h"
#include "core/components_ng/property/measure_utils.h"

namespace OHOS::Ace::NG {

namespace {
constexpr int32_t MAX_MENU_ITEMS_NUM = 3;
}

void TitleBarLayoutAlgorithm::MeasureBackButton(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty)
{
    auto backButtonNode = titleBarNode->GetBackButton();
    CHECK_NULL_VOID(backButtonNode);
    auto index = titleBarNode->GetChildIndexById(backButtonNode->GetId());
    auto backButton = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(backButton);
    auto constraint = titleBarLayoutProperty->CreateChildConstraint();

    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        constraint.selfIdealSize = OptionalSizeF(static_cast<float>(BACK_BUTTON_ICON_SIZE.ConvertToPx()),
            static_cast<float>(BACK_BUTTON_ICON_SIZE.ConvertToPx()));
        backButton->Measure(constraint);
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::MINI) {
        constraint.selfIdealSize = OptionalSizeF(0.0f, 0.0f);
        backButton->Measure(constraint);
        return;
    }

    if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
        constraint.selfIdealSize = OptionalSizeF(0.0f, 0.0f);
        backButton->Measure(constraint);
        return;
    }

    constraint.selfIdealSize = OptionalSizeF(
        static_cast<float>(BACK_BUTTON_SIZE.ConvertToPx()), static_cast<float>(BACK_BUTTON_SIZE.ConvertToPx()));
    backButton->Measure(constraint);
}

void TitleBarLayoutAlgorithm::MeasureSubtitle(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty, const SizeF& titleBarSize, float menuWidth)
{
    auto subtitleNode = titleBarNode->GetSubtitle();
    CHECK_NULL_VOID(subtitleNode);
    auto index = titleBarNode->GetChildIndexById(subtitleNode->GetId());
    auto subtitle = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(subtitle);
    auto constraint = titleBarLayoutProperty->CreateChildConstraint();
    constraint.maxSize.SetHeight(titleBarSize.Height());

    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        if (titleBarNode->GetBackButton()) {
            auto occupiedWidth = maxPaddingStart_ + BACK_BUTTON_ICON_SIZE + HORIZONTAL_MARGIN_M + maxPaddingEnd_;
            constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()));
            subtitle->Measure(constraint);
            return;
        }

        auto occupiedWidth = maxPaddingStart_ + maxPaddingEnd_;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()));
        subtitle->Measure(constraint);
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::MINI) {
        auto occupiedWidth = maxPaddingStart_ + maxPaddingEnd_ + HORIZONTAL_MARGIN;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
        subtitle->Measure(constraint);
        return;
    }

    if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
        auto occupiedWidth = maxPaddingStart_ + maxPaddingEnd_ + HORIZONTAL_MARGIN;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
        subtitle->Measure(constraint);
        return;
    }

    auto occupiedWidth =
        maxPaddingStart_ + BACK_BUTTON_ICON_SIZE + HORIZONTAL_MARGIN_M + HORIZONTAL_MARGIN + maxPaddingEnd_;
    constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
    subtitle->Measure(constraint);
}

void TitleBarLayoutAlgorithm::MeasureTitle(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty, const SizeF& titleBarSize, float menuWidth)
{
    auto titleNode = titleBarNode->GetTitle();
    CHECK_NULL_VOID(titleNode);
    auto index = titleBarNode->GetChildIndexById(titleNode->GetId());
    auto title = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(title);
    auto constraint = titleBarLayoutProperty->CreateChildConstraint();
    constraint.maxSize.SetHeight(titleBarSize.Height());

    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        if (titleBarNode->GetBackButton()) {
            auto occupiedWidth = maxPaddingStart_ + BACK_BUTTON_ICON_SIZE + maxPaddingEnd_;
            constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()));
            title->Measure(constraint);
            return;
        }

        auto occupiedWidth = maxPaddingStart_ + maxPaddingEnd_;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()));
        title->Measure(constraint);
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::MINI) {
        auto occupiedWidth = maxPaddingStart_ + defaultPaddingStart_ + HORIZONTAL_MARGIN;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
        title->Measure(constraint);
        return;
    }

    if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
        auto occupiedWidth = maxPaddingStart_ + defaultPaddingStart_ + HORIZONTAL_MARGIN;
        constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
        title->Measure(constraint);
        return;
    }

    auto occupiedWidth = defaultPaddingStart_ + BACK_BUTTON_ICON_SIZE + HORIZONTAL_MARGIN * 2 + defaultPaddingStart_;
    constraint.maxSize.SetWidth(titleBarSize.Width() - static_cast<float>(occupiedWidth.ConvertToPx()) - menuWidth);
    title->Measure(constraint);
}

float TitleBarLayoutAlgorithm::MeasureMenu(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty)
{
    auto menuNode = titleBarNode->GetMenu();
    CHECK_NULL_RETURN(menuNode, 0.0f);
    auto index = titleBarNode->GetChildIndexById(menuNode->GetId());
    auto menu = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_RETURN(menu, 0.0f);
    auto constraint = titleBarLayoutProperty->CreateChildConstraint();
    auto menuItemNum = static_cast<int32_t>(menuNode->GetChildren().size());
    float menuWidth = 0.0f;
    if (menuItemNum >= MAX_MENU_ITEMS_NUM) {
        menuWidth = static_cast<float>(BACK_BUTTON_SIZE.ConvertToPx()) * MAX_MENU_ITEMS_NUM;
    } else {
        menuWidth = static_cast<float>(BACK_BUTTON_SIZE.ConvertToPx()) * menuItemNum;
    }
    constraint.selfIdealSize = OptionalSizeF(menuWidth, static_cast<float>(menuHeight_.ConvertToPx()));
    menu->Measure(constraint);
    return menuWidth;
}

void TitleBarLayoutAlgorithm::LayoutBackButton(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty)
{
    auto backButtonNode = titleBarNode->GetBackButton();
    CHECK_NULL_VOID(backButtonNode);
    auto index = titleBarNode->GetChildIndexById(backButtonNode->GetId());
    auto backButton = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(backButton);
    auto geometryNode = backButton->GetGeometryNode();

    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        if (!titleBarNode->GetBackButton()) {
            return;
        }

        auto offsetY = (menuHeight_ - BACK_BUTTON_ICON_SIZE) / 2;
        OffsetF backButtonOffset =
            OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), static_cast<float>(offsetY.ConvertToPx()));
        geometryNode->SetMarginFrameOffset(backButtonOffset);
        backButton->Layout();
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::MINI) {
        OffsetF backButtonOffset = OffsetF(0.0f, 0.0f);
        geometryNode->SetMarginFrameOffset(backButtonOffset);
        backButton->Layout();
        return;
    }

    if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
        OffsetF backButtonOffset = OffsetF(0.0f, 0.0f);
        geometryNode->SetMarginFrameOffset(backButtonOffset);
        backButton->Layout();
        return;
    }

    auto offsetY = (menuHeight_ - BACK_BUTTON_SIZE) / 2;
    OffsetF backButtonOffset = OffsetF(static_cast<float>((defaultPaddingStart_ - BUTTON_PADDING).ConvertToPx()),
        static_cast<float>(offsetY.ConvertToPx()));
    geometryNode->SetMarginFrameOffset(backButtonOffset);
    backButton->Layout();
}

void TitleBarLayoutAlgorithm::LayoutTitle(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty, float subtitleHeight)
{
    auto titleNode = titleBarNode->GetTitle();
    CHECK_NULL_VOID(titleNode);
    auto index = titleBarNode->GetChildIndexById(titleNode->GetId());
    auto title = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(title);
    auto geometryNode = title->GetGeometryNode();

    auto titleHeight = geometryNode->GetFrameSize().Height();
    float offsetY = 0.0f;
    if (!NearZero(subtitleHeight)) {
        offsetY = (static_cast<float>(DOUBLE_LINE_TITLEBAR_HEIGHT.ConvertToPx()) - titleHeight - subtitleHeight) / 2;
    } else {
        offsetY = (static_cast<float>(SINGLE_LINE_TITLEBAR_HEIGHT.ConvertToPx()) - titleHeight) / 2;
    }

    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        auto backButtonNode = AceType::DynamicCast<FrameNode>(titleBarNode->GetBackButton());
        CHECK_NULL_VOID(backButtonNode);
        auto backButtonLayoutProperty = backButtonNode->GetLayoutProperty();
        CHECK_NULL_VOID(backButtonLayoutProperty);
        if (backButtonLayoutProperty->GetVisibilityValue(VisibleType::VISIBLE) != VisibleType::GONE) {
            OffsetF offset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()) +
                                         static_cast<float>(BACK_BUTTON_ICON_SIZE.ConvertToPx()) +
                                         static_cast<float>(HORIZONTAL_MARGIN_M.ConvertToPx()),
                offsetY);
            geometryNode->SetMarginFrameOffset(offset);
            title->Layout();
            return;
        }

        OffsetF offset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), offsetY);
        geometryNode->SetMarginFrameOffset(offset);
        title->Layout();
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) == NavigationTitleMode::MINI) {
        if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
            OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), offsetY);
            geometryNode->SetMarginFrameOffset(titleOffset);
            title->Layout();
            return;
        }

        auto occupiedWidth = defaultPaddingStart_ + BACK_BUTTON_ICON_SIZE + HORIZONTAL_MARGIN;
        OffsetF offset = OffsetF(static_cast<float>(occupiedWidth.ConvertToPx()), offsetY);
        geometryNode->SetMarginFrameOffset(offset);
        title->Layout();
        return;
    }
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::FREE) {
        OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()),
            static_cast<float>(menuHeight_.ConvertToPx()) + offsetY);
        geometryNode->SetMarginFrameOffset(titleOffset);
        title->Layout();
        return;
    }
    if (isInitialTitle_) {
        auto title = AceType::DynamicCast<FrameNode>(titleNode);
        CHECK_NULL_VOID(title);

        auto textLayoutProperty = title->GetLayoutProperty<TextLayoutProperty>();
        if (!textLayoutProperty) {
            title->Layout();
            return;
        }
        MeasureContext context;
        context.textContent = textLayoutProperty->GetContentValue();
        context.fontSize = titleFontSize_;
#ifdef ENABLE_ROSEN_BACKEND
        minTitleHeight_ = static_cast<float>(RosenRenderCustomPaint::MeasureTextSizeInner(context).Height());
#else
        minTitleHeight_ = 0.0;
#endif
        initialTitleOffsetY_ = static_cast<float>(menuHeight_.ConvertToPx()) + offsetY;
        isInitialTitle_ = false;
        OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), initialTitleOffsetY_);
        geometryNode->SetMarginFrameOffset(titleOffset);
        title->Layout();
        return;
    }

    auto titlePattern = titleBarNode->GetPattern<TitleBarPattern>();
    CHECK_NULL_VOID(titlePattern);
    if (NearZero(titlePattern->GetTempTitleOffsetY())) {
        initialTitleOffsetY_ = static_cast<float>(menuHeight_.ConvertToPx()) + offsetY;
        OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), initialTitleOffsetY_);
        geometryNode->SetMarginFrameOffset(titleOffset);
        title->Layout();
        return;
    }

    OffsetF titleOffset =
        OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), titlePattern->GetTempTitleOffsetY());
    geometryNode->SetMarginFrameOffset(titleOffset);
    title->Layout();
}

void TitleBarLayoutAlgorithm::LayoutSubtitle(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty, float titleHeight)
{
    auto subtitleNode = titleBarNode->GetSubtitle();
    CHECK_NULL_VOID(subtitleNode);
    auto index = titleBarNode->GetChildIndexById(subtitleNode->GetId());
    auto subtitle = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(subtitle);
    auto geometryNode = subtitle->GetGeometryNode();

    auto subtitleHeight = geometryNode->GetFrameSize().Height();
    float offsetY = 0.0f;
    if (!NearZero(titleHeight)) {
        offsetY = (static_cast<float>(DOUBLE_LINE_TITLEBAR_HEIGHT.ConvertToPx()) - titleHeight - subtitleHeight) / 2 +
                  titleHeight;
    } else {
        offsetY = (static_cast<float>(SINGLE_LINE_TITLEBAR_HEIGHT.ConvertToPx()) - subtitleHeight) / 2;
    }
    // navDestination title bar
    if (titleBarLayoutProperty->GetTitleBarParentTypeValue(TitleBarParentType::NAVBAR) ==
        TitleBarParentType::NAV_DESTINATION) {
        if (titleBarNode->GetBackButton()) {
            OffsetF offset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()) +
                                         static_cast<float>(BACK_BUTTON_ICON_SIZE.ConvertToPx()) +
                                         static_cast<float>(HORIZONTAL_MARGIN_M.ConvertToPx()),
                offsetY);
            geometryNode->SetMarginFrameOffset(offset);
            subtitle->Layout();
            return;
        }

        OffsetF offset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), offsetY);
        geometryNode->SetMarginFrameOffset(offset);
        subtitle->Layout();
        return;
    }

    // navBar title bar
    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) != NavigationTitleMode::MINI) {
        if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) == NavigationTitleMode::FREE) {
            if (isInitialSubtitle_) {
                initialSubtitleOffsetY_ = static_cast<float>(menuHeight_.ConvertToPx()) + offsetY;
                isInitialSubtitle_ = false;
                OffsetF titleOffset =
                    OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), initialSubtitleOffsetY_);
                geometryNode->SetMarginFrameOffset(titleOffset);
                subtitle->Layout();
                return;
            }

            auto titlePattern = titleBarNode->GetPattern<TitleBarPattern>();
            CHECK_NULL_VOID(titlePattern);
            if (NearZero(titlePattern->GetTempTitleOffsetY())) {
                OffsetF titleOffset =
                    OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), initialSubtitleOffsetY_);
                geometryNode->SetMarginFrameOffset(titleOffset);
                subtitle->Layout();
                return;
            }

            OffsetF titleOffset =
                OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), titlePattern->GetTempSubTitleOffsetY());
            geometryNode->SetMarginFrameOffset(titleOffset);
            subtitle->Layout();
            return;
        }

        OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()),
            static_cast<float>(menuHeight_.ConvertToPx()) + offsetY);
        geometryNode->SetMarginFrameOffset(titleOffset);
        subtitle->Layout();
        return;
    }

    if (titleBarLayoutProperty->GetHideBackButton().value_or(false)) {
        OffsetF titleOffset = OffsetF(static_cast<float>(maxPaddingStart_.ConvertToPx()), offsetY);
        geometryNode->SetMarginFrameOffset(titleOffset);
        subtitle->Layout();
        return;
    }

    auto occupiedWidth = maxPaddingStart_ + BACK_BUTTON_ICON_SIZE + HORIZONTAL_MARGIN_M;
    OffsetF offset = OffsetF(static_cast<float>(occupiedWidth.ConvertToPx()), offsetY);
    geometryNode->SetMarginFrameOffset(offset);
    subtitle->Layout();
}

void TitleBarLayoutAlgorithm::LayoutMenu(FrameNode* frameNode, const RefPtr<TitleBarNode>& titleBarNode,
    const RefPtr<TitleBarLayoutProperty>& titleBarLayoutProperty, float subtitleHeight)
{
    auto menuNode = titleBarNode->GetMenu();
    CHECK_NULL_VOID(menuNode);
    auto index = titleBarNode->GetChildIndexById(menuNode->GetId());
    auto menu = frameNode->GetFrameNodeByIndex(index);
    CHECK_NULL_VOID(menu);
    auto geometryNode = menu->GetGeometryNode();
    auto menuWidth = geometryNode->GetMarginFrameSize().Width();
    auto maxWidth = geometryNode->GetParentLayoutConstraint()->maxSize.Width();

    if (titleBarLayoutProperty->GetTitleModeValue(NavigationTitleMode::FREE) == NavigationTitleMode::FREE) {
        auto menuOffsetY = (SINGLE_LINE_TITLEBAR_HEIGHT - menuHeight_) / 2;
        OffsetF menuOffset(maxWidth - menuWidth - static_cast<float>(defaultPaddingStart_.ConvertToPx()),
            static_cast<float>(menuOffsetY.ConvertToPx()));
        geometryNode->SetMarginFrameOffset(menuOffset);
        menu->Layout();
        return;
    }

    Dimension menuOffsetY;
    if (!NearZero(subtitleHeight)) {
        menuOffsetY = (DOUBLE_LINE_TITLEBAR_HEIGHT - menuHeight_) / 2;
    } else {
        menuOffsetY = (SINGLE_LINE_TITLEBAR_HEIGHT - menuHeight_) / 2;
    }
    OffsetF menuOffset(maxWidth - menuWidth - static_cast<float>(defaultPaddingStart_.ConvertToPx()),
        static_cast<float>(menuOffsetY.ConvertToPx()));
    geometryNode->SetMarginFrameOffset(menuOffset);
    menu->Layout();
}

// set variables from theme
void TitleBarLayoutAlgorithm::InitializeTheme()
{
    auto theme = NavigationGetTheme();
    CHECK_NULL_VOID(theme);
    maxPaddingStart_ = theme->GetMaxPaddingStart();
    maxPaddingEnd_ = theme->GetMaxPaddingEnd();
    menuHeight_ = theme->GetHeight();
    defaultPaddingStart_ = theme->GetDefaultPaddingStart();
    iconSize_ = theme->GetMenuIconSize();
    titleFontSize_ = theme->GetTitleFontSize();
}

void TitleBarLayoutAlgorithm::Measure(FrameNode* frameNode)
{
    auto titleBarNode = AceType::DynamicCast<TitleBarNode>(Claim(frameNode));
    CHECK_NULL_VOID(titleBarNode);
    auto layoutProperty = AceType::DynamicCast<TitleBarLayoutProperty>(frameNode->GetLayoutProperty());
    CHECK_NULL_VOID(layoutProperty);
    const auto& constraint = layoutProperty->GetLayoutConstraint();
    CHECK_NULL_VOID(constraint);
    auto size = CreateIdealSize(constraint.value(), Axis::HORIZONTAL, MeasureType::MATCH_PARENT, true);
    const auto& padding = frameNode->GetLayoutProperty()->CreatePaddingAndBorder();
    MinusPaddingToSize(padding, size);
    InitializeTheme();

    MeasureBackButton(frameNode, titleBarNode, layoutProperty);
    auto menuWidth = MeasureMenu(frameNode, titleBarNode, layoutProperty);
    MeasureTitle(frameNode, titleBarNode, layoutProperty, size, menuWidth);
    MeasureSubtitle(frameNode, titleBarNode, layoutProperty, size, menuWidth);
    frameNode->GetGeometryNode()->SetFrameSize(size);
}

void TitleBarLayoutAlgorithm::Layout(FrameNode* frameNode)
{
    auto titleBarNode = AceType::DynamicCast<TitleBarNode>(Claim(frameNode));
    CHECK_NULL_VOID(titleBarNode);
    auto layoutProperty = AceType::DynamicCast<TitleBarLayoutProperty>(frameNode->GetLayoutProperty());
    CHECK_NULL_VOID(layoutProperty);
    LayoutBackButton(frameNode, titleBarNode, layoutProperty);

    float subtitleHeight = 0.0f;
    auto subtitleNode = titleBarNode->GetSubtitle();
    if (subtitleNode) {
        auto index = titleBarNode->GetChildIndexById(subtitleNode->GetId());
        auto subtitleWrapper = frameNode->GetFrameNodeByIndex(index);
        CHECK_NULL_VOID(subtitleWrapper);
        auto geometryNode = subtitleWrapper->GetGeometryNode();
        subtitleHeight = geometryNode->GetFrameSize().Height();
    }
    LayoutTitle(frameNode, titleBarNode, layoutProperty, subtitleHeight);

    float titleHeight = 0.0f;
    auto titleNode = titleBarNode->GetTitle();
    if (titleNode) {
        auto index = titleBarNode->GetChildIndexById(titleNode->GetId());
        auto titleWrapper = frameNode->GetFrameNodeByIndex(index);
        CHECK_NULL_VOID(titleWrapper);
        auto geometryNode = titleWrapper->GetGeometryNode();
        titleHeight = geometryNode->GetFrameSize().Height();
    }
    LayoutSubtitle(frameNode, titleBarNode, layoutProperty, titleHeight);

    LayoutMenu(frameNode, titleBarNode, layoutProperty, subtitleHeight);
}

} // namespace OHOS::Ace::NG
