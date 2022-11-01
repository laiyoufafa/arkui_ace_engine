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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_NAVIGATION_NAV_BAR_LAYOUT_PROPERTY_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_NAVIGATION_NAV_BAR_LAYOUT_PROPERTY_H

#include "base/geometry/dimension.h"
#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/layout/layout_property.h"
#include "core/components_ng/pattern/navigation/navigation_declaration.h"
#include "core/components_ng/property/property.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT NavBarLayoutProperty : public LayoutProperty {
    DECLARE_ACE_TYPE(NavBarLayoutProperty, LayoutProperty);

public:
    NavBarLayoutProperty() = default;

    ~NavBarLayoutProperty() override = default;

    RefPtr<LayoutProperty> Clone() const override
    {
        auto copy = MakeRefPtr<NavBarLayoutProperty>();
        copy->LayoutProperty::UpdateLayoutProperty(DynamicCast<LayoutProperty>(this));
        copy->propTitleMode_ = CloneTitleMode();
        copy->propHideTitleBar_ = CloneHideTitleBar();
        copy->propHideToolBar_ = CloneHideToolBar();
        copy->propHideBackButton_ = CloneHideBackButton();
        return copy;
    }

    void Reset() override
    {
        LayoutProperty::Reset();
        ResetTitleMode();
        ResetHideTitleBar();
        ResetHideToolBar();
        ResetHideBackButton();
    }

    std::string GetTitleModeString() const
    {
        switch (GetTitleModeValue(NavigationTitleMode::FREE)) {
            case NavigationTitleMode::FREE:
                return "NavigationTitleMode.Free";
            case NavigationTitleMode::FULL:
                return "NavigationTitleMode.Full";
            case NavigationTitleMode::MINI:
                return "NavigationTitleMode.Mini";
            default:
                return "NavigationTitleMode.Free";
        }
    }

    void ToJsonValue(std::unique_ptr<JsonValue>& json) const override
    {
        json->Put("titleMode", GetTitleModeString().c_str());
        json->Put("hideBackButton", GetHideBackButtonValue(false) ? "true" : "false");
        json->Put("hideTitleBar", GetHideTitleBarValue(false) ? "true" : "false");
        json->Put("hideToolBar", GetHideToolBarValue(false) ? "true" : "false");
    }
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TitleMode, NavigationTitleMode, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(HideTitleBar, bool, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(HideToolBar, bool, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(HideBackButton, bool, PROPERTY_UPDATE_MEASURE);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_NAVIGATION_NAV_BAR_LAYOUT_PROPERTY_H