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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TEXT_FIELD_TEXT_FIELD_OVERLAY_MODIFIER_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TEXT_FIELD_TEXT_FIELD_OVERLAY_MODIFIER_H

#include "base/memory/ace_type.h"
#include "core/components/common/properties/color.h"
#include "core/components_ng/base/modifier.h"
#include "core/components_ng/pattern/pattern.h"
#include "core/components_ng/pattern/text_field/text_field_paint_property.h"
#include "core/components_ng/property/property.h"
#include "core/components_ng/render/animation_utils.h"
#include "core/components_ng/render/drawing.h"

namespace OHOS::Ace::NG {
class TextFieldOverlayModifier : public OverlayModifier {
    DECLARE_ACE_TYPE(TextFieldOverlayModifier, OverlayModifier);

public:
    TextFieldOverlayModifier(const WeakPtr<OHOS::Ace::NG::Pattern>& pattern);
    ~TextFieldOverlayModifier() override = default;

    void onDraw(DrawingContext& context) override;

    void SetCursorColor(Color& value);
    void SetCursorWidth(float value);
    void SetSelectedBackGroundColor(Color& value);
    void SetCursorVisible(bool value);
    void SetContentSize(SizeF& value);
    void SetContentOffset(OffsetF& value);
    void SetCursorOffset(OffsetF& value);
    void SetCursorOffsetX(float value);
    void SetInputStyle(InputStyle& value);

private:
    void PaintSelection(DrawingContext& context) const;
    void PaintCursor(DrawingContext& context) const;

private:
    WeakPtr<Pattern> pattern_;
    RefPtr<AnimatablePropertyColor> cursorColor_;
    RefPtr<AnimatablePropertyFloat> cursorWidth_;
    RefPtr<AnimatablePropertyColor> selectedColor_;
    RefPtr<AnimatablePropertyFloat> cursorOffsetX_;
    RefPtr<PropertyBool> cursorVisible_;
    RefPtr<PropertySizeF> contentSize_;
    RefPtr<PropertyOffsetF> contentOffset_;
    InputStyle inputStyle_ = InputStyle::DEFAULT;

    ACE_DISALLOW_COPY_AND_MOVE(TextFieldOverlayModifier);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TEXT_FIELD_TEXT_FIELD_OVERLAY_MODIFIER_H
