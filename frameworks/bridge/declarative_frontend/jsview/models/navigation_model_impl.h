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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_NAVIGATION_MODEL_IMPL_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_NAVIGATION_MODEL_IMPL_H

#include "frameworks/core/components_ng/pattern/navigation/navigation_model.h"

namespace OHOS::Ace::Framework {
class NavigationModelImpl : public NavigationModel {
public:
    void Create() override;
    void SetNavigationStack() override;
    void SetNavigationStack(RefPtr<NG::NavigationStack>&& navigationStack) override;
    void SetNavigationStackProvided(bool provided) override;
    bool ParseCommonTitle(bool hasSubTitle, bool hasMainTitle, const std::string& subtitle,
        const std::string& title) override;
    void SetTitle(const std::string& title, bool hasSubTitle = false) override;
    void SetCustomTitle(const RefPtr<AceType>& customTitle) override;
    void SetTitleHeight(const Dimension& height) override;
    void SetTitleMode(NG::NavigationTitleMode mode) override;
    void SetSubtitle(const std::string& subtitle) override;
    void SetHideTitleBar(bool hideTitleBar) override;
    void SetHideNavBar(bool hideNavBar) override;
    void SetBackButtonIcon(const std::string& src, bool noPixMap, RefPtr<PixelMap>& pixMap) override;
    void SetHideBackButton(bool hideBackButton) override;
    void SetHideToolBar(bool hideToolBar) override;
    void SetCustomToolBar(const RefPtr<AceType>& customToolBar) override;
    bool NeedSetItems() override;
    void SetToolBarItems(std::vector<NG::BarItem>&& toolBarItems) override;
    void GetToolBarItems(std::list<RefPtr<AceType>>& items) override;
    void SetMenuItems(std::vector<NG::BarItem>&& menuItems) override;
    void GetMenuItems(std::list<RefPtr<AceType>>& items) override;
    void SetCustomMenu(const RefPtr<AceType>& customMenu) override;
    void SetOnTitleModeChange(std::function<void(NG::NavigationTitleMode)>&& onTitleModeChange,
        std::function<void(const BaseEventInfo* baseInfo)>&& eventInfo) override;
    void SetUsrNavigationMode(NG::NavigationMode mode) override;
    void SetNavBarPosition(NG::NavBarPosition mode) override;
    void SetNavBarWidth(const Dimension& value) override;
    void SetMinNavBarWidth(const Dimension& value) override {}
    void SetMaxNavBarWidth(const Dimension& value) override {}
    void SetMinContentWidth(const Dimension& value) override {}
    void SetOnNavBarStateChange(std::function<void(bool)>&& onNavBarStateChange) override;
    void SetNavigationMode(NG::NavigationMode mode) override;
    void SetNavDestination(std::function<void(std::string)>&& builder) override;
    RefPtr<NG::NavigationStack> GetNavigationStack() override;
    void SetMenuCount(int32_t menuCount) override;
};
} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_NAVIGATION_MODEL_IMPL_H
