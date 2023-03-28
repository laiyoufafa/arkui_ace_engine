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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TABS_LAYOUT_PROPERTY_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TABS_LAYOUT_PROPERTY_H

#include "base/geometry/axis.h"
#include "base/geometry/dimension.h"
#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/layout/layout_property.h"
#include "core/components_ng/pattern/tabs/tabs_model.h"
#include "core/components_ng/property/property.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT TabsLayoutProperty : public LayoutProperty {
    DECLARE_ACE_TYPE(TabsLayoutProperty, LayoutProperty);

public:
    TabsLayoutProperty() = default;
    ~TabsLayoutProperty() override = default;

    RefPtr<LayoutProperty> Clone() const override
    {
        auto value = MakeRefPtr<TabsLayoutProperty>();
        value->LayoutProperty::UpdateLayoutProperty(AceType::DynamicCast<LayoutProperty>(this));
        value->propTabBarPosition_ = CloneTabBarPosition();
        value->propAxis_ = CloneAxis();
        value->propTabBarMode_ = CloneTabBarMode();
        value->propDivider_ = CloneDivider();
        value->propBarWidth_ = CloneBarWidth();
        value->propBarHeight_ = CloneBarHeight();
        value->propIndex_ = CloneIndex();
        return value;
    }

    void Reset() override
    {
        LayoutProperty::Reset();
        ResetTabBarPosition();
        ResetAxis();
        ResetTabBarMode();
        ResetDivider();
        ResetBarWidth();
        ResetBarHeight();
        ResetIndex();
    }

    void ToJsonValue(std::unique_ptr<JsonValue>& json) const override
    {
        LayoutProperty::ToJsonValue(json);
        json->Put("vertical", propAxis_.value_or(Axis::HORIZONTAL) == Axis::HORIZONTAL ? "false" : "true");
        json->Put("barPosition", propTabBarPosition_.value_or(BarPosition::START) == BarPosition::START
                                     ? "BarPosition.Start"
                                     : "BarPosition.End");
        if (propDivider_.has_value()) {
            auto divider = JsonUtil::Create(false);
            divider->Put("strokeWidth", propDivider_.value().strokeWidth.ToString().c_str());
            divider->Put("startMargin", propDivider_.value().startMargin.ToString().c_str());
            divider->Put("endMargin", propDivider_.value().endMargin.ToString().c_str());
            divider->Put("color", propDivider_.value().color.ColorToString().c_str());
            json->Put("divider", divider);
        } else {
            auto divider = JsonUtil::Create(false);
            json->Put("divider", divider);
        }
    }

    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TabBarPosition, BarPosition, PROPERTY_UPDATE_LAYOUT);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Axis, Axis, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(TabBarMode, TabBarMode, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Divider, TabsItemDivider, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(BarWidth, Dimension, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(BarHeight, Dimension, PROPERTY_UPDATE_MEASURE);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Index, int32_t, PROPERTY_UPDATE_NORMAL);
};

} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_TABS_TABS_LAYOUT_PROPERTY_H
