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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TAB_BAR_LAYOUT_PROPERTY_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TAB_BAR_LAYOUT_PROPERTY_H

#include "base/geometry/axis.h"
#include "base/geometry/dimension.h"
#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/layout/layout_property.h"
#include "core/components_ng/property/property.h"

namespace OHOS::Ace::NG {

using TabBarBuilderFunc = std::function<void()>;
struct TabBarParam {
    std::string text;
    std::string icon;
    TabBarBuilderFunc builder;
};

class ACE_EXPORT TabBarLayoutProperty : public LayoutProperty {
    DECLARE_ACE_TYPE(TabBarLayoutProperty, LayoutProperty);

public:
    TabBarLayoutProperty() = default;
    ~TabBarLayoutProperty() override = default;

    RefPtr<LayoutProperty> Clone() const override
    {
        auto value = MakeRefPtr<TabBarLayoutProperty>();
        value->LayoutProperty::UpdateLayoutProperty(AceType::DynamicCast<LayoutProperty>(this));
        value->propTabBarMode_ = CloneTabBarMode();
        value->propTabBarWidth_ = CloneTabBarWidth();
        value->propTabBarHeight_ = CloneTabBarHeight();
        value->propAxis_ = CloneAxis();
        return value;
    }

    void Reset() override
    {
        LayoutProperty::Reset();
        ResetTabBarMode();
        ResetTabBarWidth();
        ResetTabBarHeight();
        ResetAxis();
    }

    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TabBarMode, TabBarMode, PROPERTY_UPDATE_LAYOUT);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TabBarWidth, Dimension, PROPERTY_UPDATE_LAYOUT);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TabBarHeight, Dimension, PROPERTY_UPDATE_LAYOUT);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Axis, Axis, PROPERTY_UPDATE_LAYOUT);
};

} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TAB_BAR_LAYOUT_PROPERTY_H
