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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_OPTION_OPTION_PAINT_PROPERTY_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_OPTION_OPTION_PAINT_PROPERTY_H

#include "core/components/common/properties/color.h"
#include "core/components/common/properties/text_style.h"
#include "core/components_ng/render/paint_property.h"

namespace OHOS::Ace::NG {

struct OptionPaintBackground {
    ACE_DEFINE_PROPERTY_GROUP_ITEM(BackgroundColor, Color);
    ACE_DEFINE_PROPERTY_GROUP_ITEM(SelectedBackgroundColor, Color);
    ACE_DEFINE_PROPERTY_GROUP_ITEM(Hover, bool);
};

struct OptionPaintDivider {
    ACE_DEFINE_PROPERTY_GROUP_ITEM(DividerOffset, OffsetF);
    ACE_DEFINE_PROPERTY_GROUP_ITEM(DividerLength, float);
};

// PaintProperty are used to set paint properties.
class ACE_EXPORT OptionPaintProperty : public PaintProperty {
    DECLARE_ACE_TYPE(OptionPaintProperty, PaintProperty)

public:
    OptionPaintProperty() = default;
    ~OptionPaintProperty() override = default;

    RefPtr<PaintProperty> Clone() const override
    {
        auto paintProperty = MakeRefPtr<OptionPaintProperty>();
        paintProperty->propPaintBackground_ = ClonePaintBackground();
        paintProperty->propPaintDivider_ = ClonePaintDivider();

        return paintProperty;
    }

    void Reset() override
    {
        PaintProperty::Reset();
        ResetPaintBackground();
        ResetPaintDivider();
    }

    ACE_DEFINE_PROPERTY_GROUP(PaintBackground, OptionPaintBackground);
    ACE_DEFINE_PROPERTY_ITEM_WITH_GROUP(PaintBackground, BackgroundColor, Color, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITH_GROUP(PaintBackground, SelectedBackgroundColor, Color, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITH_GROUP(PaintBackground, Hover, bool, PROPERTY_UPDATE_RENDER);

    ACE_DEFINE_PROPERTY_GROUP(PaintDivider, OptionPaintDivider);
    ACE_DEFINE_PROPERTY_ITEM_WITH_GROUP(PaintDivider, DividerOffset, OffsetF, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITH_GROUP(PaintDivider, DividerLength, float, PROPERTY_UPDATE_RENDER);

};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_OPTION_OPTION_PAINT_PROPERTY_H
