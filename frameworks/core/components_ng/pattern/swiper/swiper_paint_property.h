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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SWIPER_SWIPER_PAINT_PROPERTY_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SWIPER_SWIPER_PAINT_PROPERTY_H

#include "src/core/SkPaintPriv.h"
#include "core/animation/curve.h"
#include "core/components/common/layout/constants.h"
#include "core/components/common/properties/color.h"
#include "core/components_ng/render/paint_property.h"

namespace OHOS::Ace::NG {
// PaintProperty are used to set paint properties.
class SwiperPaintProperty : public PaintProperty {
    DECLARE_ACE_TYPE(SwiperPaintProperty, PaintProperty)

public:
    SwiperPaintProperty() = default;
    ~SwiperPaintProperty() override = default;

    RefPtr<PaintProperty> Clone() const override
    {
        auto paintProperty = MakeRefPtr<SwiperPaintProperty>();
        paintProperty->UpdatePaintProperty(this);
        paintProperty->propLoop_ = CloneLoop();
        paintProperty->propAutoPlay_ = CloneAutoPlay();
        paintProperty->propAutoPlayInterval_ = CloneAutoPlayInterval();
        paintProperty->propDuration_ = CloneDuration();
        paintProperty->propCurve_ = CloneCurve();
        paintProperty->propEnabled_ = CloneEnabled();
        paintProperty->propDisableSwipe_ = CloneDisableSwipe();
        paintProperty->propEdgeEffect_ = CloneEdgeEffect();
        paintProperty->propFadeColor_ = CloneFadeColor();
        return paintProperty;
    }

    void Reset() override
    {
        PaintProperty::Reset();
        ResetLoop();
        ResetAutoPlay();
        ResetAutoPlayInterval();
        ResetDuration();
        ResetCurve();
        ResetEnabled();
        ResetDisableSwipe();
        ResetEdgeEffect();
        ResetFadeColor();
    }

    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Loop, bool, PROPERTY_UPDATE_RENDER);

    // TODO use group
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(AutoPlay, bool, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(AutoPlayInterval, int32_t, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Duration, int32_t, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Curve, RefPtr<Curve>, PROPERTY_UPDATE_RENDER);

    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(Enabled, bool, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(DisableSwipe, bool, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(EdgeEffect, EdgeEffect, PROPERTY_UPDATE_RENDER);
    ACE_DEFINE_PROPERTY_ITEM_WITHOUT_GROUP(FadeColor, Color, PROPERTY_UPDATE_RENDER);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SWIPER_SWIPER_PAINT_PROPERTY_H
