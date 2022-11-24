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
#include "core/components_ng/pattern/checkbox/checkbox_paint_method.h"

#include <optional>

#include "base/geometry/ng/offset_t.h"
#include "base/geometry/ng/rect_t.h"
#include "base/geometry/rect.h"
#include "base/geometry/rrect.h"
#include "base/utils/utils.h"
#include "core/components/checkable/checkable_theme.h"
#include "core/components/common/properties/alignment.h"
#include "core/components/common/properties/color.h"
#include "core/components/theme/theme_manager.h"
#include "core/components_ng/pattern/checkbox/checkbox_paint_property.h"
#include "core/components_ng/pattern/checkbox/checkbox_pattern.h"
#include "core/components_ng/render/drawing.h"
#include "core/components_ng/render/drawing_prop_convertor.h"

namespace OHOS::Ace::NG {
namespace {
constexpr uint8_t ENABLED_ALPHA = 255;
constexpr uint8_t DISABLED_ALPHA = 102;
constexpr float CHECK_MARK_START_X_POSITION = 0.25f;
constexpr float CHECK_MARK_START_Y_POSITION = 0.49f;
constexpr float CHECK_MARK_MIDDLE_X_POSITION = 0.44f;
constexpr float CHECK_MARK_MIDDLE_Y_POSITION = 0.68f;
constexpr float CHECK_MARK_END_X_POSITION = 0.76f;
constexpr float CHECK_MARK_END_Y_POSITION = 0.33f;
constexpr float CHECK_MARK_LEFT_ANIMATION_PERCENT = 0.45;
constexpr float CHECK_MARK_RIGHT_ANIMATION_PERCENT = 0.55;
constexpr float DEFAULT_MAX_CHECKBOX_SHAPE_SCALE = 1.0;
constexpr float DEFAULT_MIN_CHECKBOX_SHAPE_SCALE = 0.0;

} // namespace

CanvasDrawFunction CheckBoxPaintMethod::GetContentDrawFunction(PaintWrapper* paintWrapper)
{
    InitializeParam();
    auto paintFunc = [weak = WeakClaim(this), paintWrapper](RSCanvas& canvas) {
        auto checkbox = weak.Upgrade();
        if (checkbox) {
            checkbox->PaintCheckBox(canvas, paintWrapper);
        }
    };
    return paintFunc;
}

void CheckBoxPaintMethod::InitializeParam()
{
    auto pipeline = PipelineBase::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto checkBoxTheme = pipeline->GetTheme<CheckboxTheme>();
    CHECK_NULL_VOID(checkBoxTheme);
    borderWidth_ = checkBoxTheme->GetBorderWidth().ConvertToPx();
    borderRadius_ = checkBoxTheme->GetBorderRadius().ConvertToPx();
    checkStroke_ = checkBoxTheme->GetCheckStroke().ConvertToPx();
    activeColor_ = checkBoxTheme->GetActiveColor();
    inactiveColor_ = checkBoxTheme->GetInactiveColor();
    inactivePointColor_ = checkBoxTheme->GetInactivePointColor();
    shadowColor_ = checkBoxTheme->GetShadowColor();
    clickEffectColor_ = checkBoxTheme->GetClickEffectColor();
    hoverColor_ = checkBoxTheme->GetHoverColor();
    hoverRadius_ = checkBoxTheme->GetHoverRadius();
    hotZoneHorizontalPadding_ = checkBoxTheme->GetHotZoneHorizontalPadding();
}

void CheckBoxPaintMethod::PaintCheckBox(RSCanvas& canvas, PaintWrapper* paintWrapper) const
{
    CHECK_NULL_VOID(paintWrapper);
    auto paintProperty = DynamicCast<CheckBoxPaintProperty>(paintWrapper->GetPaintProperty());
    CHECK_NULL_VOID(paintProperty);
    auto contentSize = paintWrapper->GetContentSize();
    auto color = activeColor_;
    if (paintProperty->HasCheckBoxSelectedColor()) {
        color = paintProperty->GetCheckBoxSelectedColorValue();
    }
    auto offset = paintWrapper->GetContentOffset();
    RSPen pen;
    RSBrush brush;
    OffsetF paintOffset = OffsetF(offset.GetX(), offset.GetY());
    pen.SetWidth(borderWidth_);
    pen.SetAntiAlias(true);
    float strokeOffset = borderWidth_ / 2;
    paintOffset += OffsetF(strokeOffset, strokeOffset);
    contentSize.SetWidth(contentSize.Width() - borderWidth_);
    contentSize.SetHeight(contentSize.Height() - borderWidth_);
    if (isTouch_) {
        DrawTouchBoard(canvas, contentSize, paintOffset);
    }
    if (isHover_) {
        DrawHoverBoard(canvas, contentSize, paintOffset);
    }
    if (uiStatus_ == UIStatus::OFF_TO_ON) {
        brush.SetColor(ToRSColor(color));
        brush.SetAntiAlias(true);
        pen.SetColor(ToRSColor(Color::WHITE));
        if (!enabled_) {
            brush.SetColor(ToRSColor(color.BlendOpacity(float(DISABLED_ALPHA) / ENABLED_ALPHA)));
        }
        DrawActiveBorder(canvas, paintOffset, brush, contentSize);
        DrawAnimationOffToOn(canvas, paintOffset, pen, contentSize);
    } else if (uiStatus_ == UIStatus::ON_TO_OFF) {
        brush.SetColor(ToRSColor(color));
        brush.SetAntiAlias(true);
        pen.SetColor(ToRSColor(Color::WHITE));
        if (!enabled_) {
            brush.SetColor(ToRSColor(color.BlendOpacity(float(DISABLED_ALPHA) / ENABLED_ALPHA)));
        }
        DrawActiveBorder(canvas, paintOffset, brush, contentSize);
        DrawAnimationOnToOff(canvas, paintOffset, pen, contentSize);
    } else if (uiStatus_ == UIStatus::UNSELECTED) {
        pen.SetColor(ToRSColor(inactiveColor_));
        brush.SetColor(ToRSColor(inactivePointColor_));
        if (!enabled_) {
            pen.SetColor(ToRSColor(inactiveColor_.BlendOpacity(float(DISABLED_ALPHA) / ENABLED_ALPHA)));
        }
        DrawUnselected(canvas, paintOffset, pen, brush, contentSize);
    }
}

void CheckBoxPaintMethod::DrawUnselected(
    RSCanvas& canvas, const OffsetF& origin, RSPen& pen, RSBrush& brush, SizeF& paintSize) const
{
    float originX = origin.GetX();
    float originY = origin.GetY();
    auto rrect = RSRoundRect(
        { originX, originY, paintSize.Width() + originX, paintSize.Height() + originY }, borderRadius_, borderRadius_);
    canvas.AttachPen(pen);
    canvas.DrawRoundRect(rrect);
    canvas.AttachBrush(brush);
    canvas.DrawRoundRect(rrect);
}

void CheckBoxPaintMethod::DrawActiveBorder(
    RSCanvas& canvas, const OffsetF& paintOffset, RSBrush& brush, const SizeF& paintSize) const
{
    float originX = paintOffset.GetX();
    float originY = paintOffset.GetY();
    auto rrect = RSRoundRect(
        { originX, originY, paintSize.Width() + originX, paintSize.Height() + originY }, borderRadius_, borderRadius_);
    canvas.AttachBrush(brush);
    canvas.DrawRoundRect(rrect);
}

void CheckBoxPaintMethod::DrawAnimationOffToOn(
    RSCanvas& canvas, const OffsetF& origin, RSPen& pen, const SizeF& paintSize) const
{
    RSPath path;
    RSPen shadowPen = RSPen(pen);
    float originX = origin.GetX();
    float originY = origin.GetY();
    const Offset start =
        Offset(paintSize.Width() * CHECK_MARK_START_X_POSITION, paintSize.Width() * CHECK_MARK_START_Y_POSITION);
    const Offset middle =
        Offset(paintSize.Width() * CHECK_MARK_MIDDLE_X_POSITION, paintSize.Width() * CHECK_MARK_MIDDLE_Y_POSITION);
    const Offset end =
        Offset(paintSize.Width() * CHECK_MARK_END_X_POSITION, paintSize.Width() * CHECK_MARK_END_Y_POSITION);

    float deltaX = middle.GetX() - start.GetX();
    float deltaY = middle.GetY() - start.GetY();
    path.MoveTo(originX + start.GetX(), originY + start.GetY());
    shadowPen.SetCapStyle(RSPen::CapStyle::ROUND_CAP);
    shadowPen.SetWidth(checkStroke_);
    shadowPen.SetColor(ToRSColor(shadowColor_));
    pen.SetWidth(borderWidth_);
    float ratio = DEFAULT_MIN_CHECKBOX_SHAPE_SCALE;
    if (shapeScale_ < CHECK_MARK_LEFT_ANIMATION_PERCENT) {
        ratio = shapeScale_ / CHECK_MARK_LEFT_ANIMATION_PERCENT;
        path.LineTo(originX + start.GetX() + deltaX * ratio, originY + start.GetY() + deltaY * ratio);
    } else {
        path.LineTo(originX + middle.GetX(), originY + middle.GetY());
    }
    canvas.AttachPen(shadowPen);
    canvas.DrawPath(path);
    canvas.AttachPen(pen);
    canvas.DrawPath(path);
    if (shapeScale_ > CHECK_MARK_LEFT_ANIMATION_PERCENT) {
        deltaX = end.GetX() - middle.GetX();
        deltaY = middle.GetY() - end.GetY();
        path.MoveTo(originX + middle.GetX(), originY + middle.GetY());
        if (shapeScale_ == DEFAULT_MAX_CHECKBOX_SHAPE_SCALE) {
            path.LineTo(originX + end.GetX(), originY + end.GetY());
        } else {
            ratio = (shapeScale_ - CHECK_MARK_LEFT_ANIMATION_PERCENT) / CHECK_MARK_RIGHT_ANIMATION_PERCENT;
            path.LineTo(originX + middle.GetX() + deltaX * ratio, originY + middle.GetY() - deltaY * ratio);
        }
        canvas.AttachPen(shadowPen);
        canvas.DrawPath(path);
        canvas.AttachPen(pen);
        canvas.DrawPath(path);
    }
}

void CheckBoxPaintMethod::DrawAnimationOnToOff(
    RSCanvas& canvas, const OffsetF& origin, RSPen& pen, const SizeF& paintSize) const
{
    if (shapeScale_ == DEFAULT_MIN_CHECKBOX_SHAPE_SCALE) {
        return;
    }
    RSPath path;
    RSPen shadowPen = RSPen(pen);
    float originX = origin.GetX();
    float originY = origin.GetY();
    const Offset start =
        Offset(paintSize.Width() * CHECK_MARK_START_X_POSITION, paintSize.Width() * CHECK_MARK_START_Y_POSITION);
    const Offset middle =
        Offset(paintSize.Width() * CHECK_MARK_MIDDLE_X_POSITION, paintSize.Width() * CHECK_MARK_MIDDLE_Y_POSITION);
    const Offset end =
        Offset(paintSize.Width() * CHECK_MARK_END_X_POSITION, paintSize.Width() * CHECK_MARK_END_Y_POSITION);
    const Offset middlePoint = Offset(paintSize.Width() / 2, paintSize.Height() / 2);
    float deltaX = middlePoint.GetX() - start.GetX();
    float deltaY = middlePoint.GetY() - start.GetY();
    float ratio = DEFAULT_MAX_CHECKBOX_SHAPE_SCALE - shapeScale_;
    shadowPen.SetCapStyle(RSPen::CapStyle::ROUND_CAP);
    shadowPen.SetWidth(checkStroke_);
    shadowPen.SetColor(ToRSColor(shadowColor_));
    pen.SetWidth(borderWidth_);
    path.MoveTo(originX + start.GetX() + deltaX * ratio, originY + start.GetY() + deltaY * ratio);
    deltaX = middlePoint.GetX() - middle.GetX();
    deltaY = middle.GetY() - middlePoint.GetY();
    path.LineTo(originX + middle.GetX() + deltaX * ratio, originY + middle.GetY() - deltaY * ratio);
    canvas.AttachPen(shadowPen);
    canvas.DrawPath(path);
    canvas.AttachPen(pen);
    canvas.DrawPath(path);
    path.MoveTo(originX + middle.GetX() + deltaX * ratio, originY + middle.GetY() - deltaY * ratio);
    deltaX = end.GetX() - middlePoint.GetX();
    deltaY = middlePoint.GetY() - end.GetY();
    path.LineTo(originX + end.GetX() - deltaX * ratio, originY + end.GetY() + deltaY * ratio);
    canvas.AttachPen(shadowPen);
    canvas.DrawPath(path);
    canvas.AttachPen(pen);
    canvas.DrawPath(path);
}

void CheckBoxPaintMethod::DrawTouchBoard(RSCanvas& canvas, const SizeF& size, const OffsetF& offset) const
{
    RSBrush brush;
    brush.SetColor(ToRSColor(Color(clickEffectColor_)));
    brush.SetAntiAlias(true);
    float originX = offset.GetX() - hotZoneHorizontalPadding_.ConvertToPx();
    float originY = offset.GetY() - hotZoneHorizontalPadding_.ConvertToPx();
    float endX = size.Width() + originX + 2 * hotZoneHorizontalPadding_.ConvertToPx();
    float endY = size.Height() + originY + 2 * hotZoneHorizontalPadding_.ConvertToPx();
    auto rrect = RSRoundRect({ originX, originY, endX, endY }, hoverRadius_.ConvertToPx(), hoverRadius_.ConvertToPx());
    canvas.AttachBrush(brush);
    canvas.DrawRoundRect(rrect);
}

void CheckBoxPaintMethod::DrawHoverBoard(RSCanvas& canvas, const SizeF& size, const OffsetF& offset) const
{
    RSBrush brush;
    brush.SetColor(ToRSColor(Color(hoverColor_)));
    brush.SetAntiAlias(true);
    float originX = offset.GetX() - hotZoneHorizontalPadding_.ConvertToPx();
    float originY = offset.GetY() - hotZoneHorizontalPadding_.ConvertToPx();
    float endX = size.Width() + originX + 2 * hotZoneHorizontalPadding_.ConvertToPx();
    float endY = size.Height() + originY + 2 * hotZoneHorizontalPadding_.ConvertToPx();
    auto rrect = RSRoundRect({ originX, originY, endX, endY }, hoverRadius_.ConvertToPx(), hoverRadius_.ConvertToPx());
    canvas.AttachBrush(brush);
    canvas.DrawRoundRect(rrect);
}

} // namespace OHOS::Ace::NG
