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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_DECLARATION_NAVIGATION_NAVIGATION_DECLARATION_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_DECLARATION_NAVIGATION_NAVIGATION_DECLARATION_H

#include <string>

#include "base/geometry/dimension.h"
#include "core/components/common/properties/color.h"
#include "core/components/declaration/common/declaration.h"

namespace OHOS::Ace::NG {
// TODO：move some items to theme
// title bar back button
const std::string BACK_BUTTON = "Back";
constexpr InternalResource::ResourceId BACK_ICON = InternalResource::ResourceId::IC_BACK;
constexpr InternalResource::ResourceId MORE_ICON = InternalResource::ResourceId::IC_MORE;
// title bar and tool bar, bar item
constexpr float BAR_ITEM_WIDTH = 40.0f;
constexpr float BAR_ITEM_HEIGHT = 60.0f;
constexpr float BAR_TEXT_FONT_SIZE = 18.0f;
// title bar title and subtitle
constexpr float TITLE_WIDTH = 100.0f;
// single page maximum width
constexpr float SINGLE_PAGE_MAXIMUM_WIDTH = 720.0f;

// title
constexpr Dimension TITLE_FONT_SIZE = 30.0_vp; // ohos_id_text_size_headline6
constexpr Color TITLE_COLOR = Color(0x66182431); // ohos_id_color_text_tertiary
constexpr Dimension TITLE_HEIGHT = 56.0_vp;
// subtitle
constexpr Dimension SUBTITLE_FONT_SIZE = 14.0_vp; // ohos_id_text_size_sub_title3
constexpr Color SUBTITLE_COLOR = Color(0x99000000); // ohos_id_alpha_content_secondary
constexpr Dimension SUBTITLE_HEIGHT = 26.0_vp;
// back button
constexpr Dimension BACK_BUTTON_ICON_SIZE = 24.0_vp;
// title bar
constexpr Dimension TITLEBAR_HEIGHT_MINI = 56.0_vp;
constexpr Dimension TITLEBAR_HEIGHT_WITH_SUBTITLE = 137.0_vp;
constexpr Dimension TITLEBAR_HEIGHT_WITHOUT_SUBTITLE = 112.0_vp;
// toolbar item
constexpr Dimension TEXT_FONT_SIZE = 10.0_vp;
constexpr Color TEXT_COLOR = Color(0xE6000000);
constexpr Dimension ICON_SIZE = 24.0_vp;
constexpr Color ICON_COLOR = Color(0xE6000000);
// toolbar
constexpr Dimension TOOLBAR_HEIGHT = 56.0_vp;
constexpr Dimension ICON_PADDING = 10.0_vp;
constexpr Dimension TEXT_TOP_PADDING = 2.0_vp;

// divider
constexpr Dimension DIVIDER_WIDTH = 1.0_vp;
constexpr Color DIVIDER_COLOR = Color(0x08000000);

// navigation content
constexpr Dimension SINGLE_LINE_TITLEBAR_HEIGHT = 56.0_vp;
constexpr Dimension DOUBLE_LINE_TITLEBAR_HEIGHT = 82.0_vp;
constexpr Dimension MAX_PADDING_START = 24.0_vp;
constexpr Dimension MAX_PADDING_END = 24.0_vp;
constexpr Dimension TITLE_TEXT_SIZE = 24.0_vp;
constexpr Color TITLE_TEXT_COLOR = Color(0x66182431); // Light mode

// navBar
constexpr Dimension FULL_SINGLE_LINE_TITLEBAR_HEIGHT = 112.0_vp;
constexpr Dimension FULL_DOUBLE_LINE_TITLEBAR_HEIGHT = 138.0_vp;
constexpr Dimension MENU_HEIGHT = 56.0_vp;
constexpr Dimension HORIZONTAL_MARGIN = 16.0_vp; // ohos_id_elements_margin_horizontal_l
constexpr Dimension HORIZONTAL_MARGIN_M = 8.0_vp; // ohos_id_elements_margin_horizontal_m

struct BarItem {
    std::optional<std::string> text;
    std::optional<std::string> icon;
    std::function<void()> action;
    std::string ToString() const
    {
        std::string result;
        result.append("text: ");
        result.append(text.value_or("na"));
        result.append(", icon: ");
        result.append(icon.value_or("na"));
        return result;
    }
};

enum class TitleBarChildType {
    TITLE = 0,
    SUBTITLE,
    MENU,
};

enum class NavigationTitleMode {
    FREE = 0,
    FULL,
    MINI,
};

enum class NavigationMode {
    AUTO = 0,
    SPLIT,
    STACK,
    OVERLAY,
};

enum class NavBarPosition {
    START = 0,
    END,
};

enum class ChildNodeOperation {
    ADD,
    // remove case only used for back button
    REMOVE,
    REPLACE,
    NONE
};

enum class TitleHeight {
    MAINONLY,
    MAINWITHSUB
};

enum class TitleBarParentType {
    NAVBAR,
    NAVDESTINATION
};
} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_DECLARATION_NAVIGATION_NAVIGATION_DECLARATION_H