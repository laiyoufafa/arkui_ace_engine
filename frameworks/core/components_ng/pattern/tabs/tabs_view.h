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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TABS_VIEW_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TABS_VIEW_H

#include "base/geometry/axis.h"
#include "base/geometry/dimension.h"
#include "base/memory/referenced.h"
#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/base/group_node.h"
#include "core/components_ng/pattern/swiper/swiper_event_hub.h"
#include "core/components_ng/pattern/swiper/swiper_layout_property.h"
#include "core/components_ng/pattern/tabs/tab_bar_layout_property.h"

namespace OHOS::Ace::NG {

class SwiperPaintProperty;
class ACE_EXPORT TabsView {
public:
    static void Create();
    static void Pop();
    static void SetIndex(int32_t index);
    static void SetTabBarPosition(BarPosition tabBarPosition);
    static void SetTabBarMode(TabBarMode tabBarMode);
    static void SetTabBarWidth(const Dimension& tabBarWidth);
    static void SetTabBarHeight(const Dimension& tabBarHeight);
    static void SetAxis(Axis axis);
    static void SetScrollable(bool scrollable);
    static void SetAnimationDuration(int32_t duration);
    static void SetOnChange(ChangeEvent&& onChange);

    static RefPtr<SwiperController> GetSwiperController();

    static RefPtr<GroupNode> CreateTabsNode(
        const std::string& tag, int32_t nodeId, const std::function<RefPtr<Pattern>(void)>& patternCreator);

private:
    static RefPtr<TabBarLayoutProperty> GetTabBarLayoutProperty();
    static RefPtr<SwiperLayoutProperty> GetSwiperLayoutProperty();
    static RefPtr<SwiperPaintProperty> GetSwiperPaintProperty();
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TABS_VIEW_H
