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
#include "base/geometry/ng/offset_t.h"
#include "base/utils/utils.h"
#include "core/components/checkable/checkable_theme.h"
#include "core/components/common/properties/color.h"
#include "core/components_ng/pattern/radio/radio_modifier.h"
#include "core/components_ng/render/animation_utils.h"
#include "core/components_ng/render/drawing.h"
#include "core/components_ng/render/drawing_prop_convertor.h"
#include "core/pipeline/pipeline_base.h"

namespace OHOS::Ace::NG {
namespace {
constexpr uint8_t ENABLED_ALPHA = 255;
constexpr uint8_t DISABLED_ALPHA = 102;
constexpr float CALC_RADIUS = 2.0f;
constexpr float DEFAULT_POINT_SCALE = 0.5f;
constexpr float DEFAULT_TOTAL_SCALE = 1.0f;
constexpr float DEFAULT_SHRINK_SCALE = 0.9f;
constexpr int32_t DEFAULT_RADIO_ANIMATION_DURATION = 300;
} // namespace

RadioModifier::RadioModifier()
{
    auto pipeline = PipelineBase::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto radioTheme = pipeline->GetTheme<RadioTheme>();

    pointColor_ = AceType::MakeRefPtr<AnimatablePropertyColor>(LinearColor(radioTheme->GetPointColor()));
    AttachProperty(pointColor_);

    activeColor_ = AceType::MakeRefPtr<AnimatablePropertyColor>(LinearColor(radioTheme->GetActiveColor()));
    AttachProperty(activeColor_);

    inactiveColor_ = AceType::MakeRefPtr<AnimatablePropertyColor>(LinearColor(radioTheme->GetInactiveColor()));
    AttachProperty(inactiveColor_);
    isOnAnimationFlag_ = AceType::MakeRefPtr<PropertyBool>(false);
    enabled_ = AceType::MakeRefPtr<PropertyBool>(true);
    isCheck_ = AceType::MakeRefPtr<PropertyBool>(false);
    uiStatus_ = AceType::MakeRefPtr<PropertyInt>(static_cast<int32_t>(UIStatus::UNSELECTED));
    offset_ = AceType::MakeRefPtr<AnimatablePropertyOffsetF>(OffsetF());
    size_ = AceType::MakeRefPtr<AnimatablePropertySizeF>(SizeF());
    totalScale_ = AceType::MakeRefPtr<AnimatablePropertyFloat>(DEFAULT_TOTAL_SCALE);
    pointScale_ = AceType::MakeRefPtr<AnimatablePropertyFloat>(DEFAULT_POINT_SCALE);
    ringPointScale_ = AceType::MakeRefPtr<AnimatablePropertyFloat>(0.0f);
    animateTouchHoverColor_ = AceType::MakeRefPtr<AnimatablePropertyColor>(LinearColor(Color::TRANSPARENT));

    AttachProperty(enabled_);
    AttachProperty(isCheck_);
    AttachProperty(uiStatus_);
    AttachProperty(offset_);
    AttachProperty(size_);
    AttachProperty(totalScale_);
    AttachProperty(pointScale_);
    AttachProperty(ringPointScale_);
    AttachProperty(animateTouchHoverColor_);

    InitializeParam();
}

void RadioModifier::InitializeParam()
{
    auto pipeline = PipelineBase::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto radioTheme = pipeline->GetTheme<RadioTheme>();
    shadowWidth_ = radioTheme->GetShadowWidth().ConvertToPx();
    borderWidth_ = radioTheme->GetBorderWidth().ConvertToPx();
    inactivePointColor_ = radioTheme->GetInactivePointColor();
    shadowColor_ = radioTheme->GetShadowColor();
    clickEffectColor_ = radioTheme->GetClickEffectColor();
    hoverColor_ = radioTheme->GetHoverColor();
    hotZoneHorizontalPadding_ = radioTheme->GetHotZoneHorizontalPadding();
    defaultPadding_ = radioTheme->GetDefaultPadding();
    hoverDuration_ = radioTheme->GetHoverDuration();
    hoverToTouchDuration_ = radioTheme->GetHoverToTouchDuration();
    touchDuration_ = radioTheme->GetTouchDuration();
}

void RadioModifier::UpdateAnimatableProperty()
{
    switch (touchHoverType_) {
        case TouchHoverAnimationType::HOVER:
            SetBoardColor(LinearColor(hoverColor_), hoverDuration_, Curves::FRICTION);
            break;
        case TouchHoverAnimationType::PRESS_TO_HOVER:
            SetBoardColor(LinearColor(hoverColor_), hoverToTouchDuration_, Curves::SHARP);
            break;
        case TouchHoverAnimationType::NONE:
            SetBoardColor(LinearColor(hoverColor_.BlendOpacity(0)), hoverDuration_, Curves::FRICTION);
            break;
        case TouchHoverAnimationType::HOVER_TO_PRESS:
            SetBoardColor(LinearColor(clickEffectColor_), hoverToTouchDuration_, Curves::SHARP);
            break;
        case TouchHoverAnimationType::PRESS:
            SetBoardColor(LinearColor(clickEffectColor_), hoverDuration_, Curves::FRICTION);
            break;
        default:
            break;
    }
}

void RadioModifier::UpdateIsOnAnimatableProperty(bool isCheck)
{
    AnimationOption delayOption;
    delayOption.SetDelay(DEFAULT_RADIO_ANIMATION_DURATION / 2);
    delayOption.SetDuration(DEFAULT_RADIO_ANIMATION_DURATION / 2);
    delayOption.SetCurve(Curves::FRICTION);

    AnimationOption halfDurationOption;
    halfDurationOption.SetDuration(DEFAULT_RADIO_ANIMATION_DURATION / 2);
    halfDurationOption.SetCurve(Curves::FRICTION);

    if (isOnAnimationFlag_->Get()) {
        pointScale_->Set(0);
        AnimationUtils::Animate(delayOption, [&]() { pointScale_->Set(DEFAULT_POINT_SCALE); });
        ringPointScale_->Set(1);
        AnimationUtils::Animate(halfDurationOption, [&]() { ringPointScale_->Set(0); });
    } else {
        pointScale_->Set(DEFAULT_POINT_SCALE);
        AnimationUtils::Animate(halfDurationOption, [&]() { pointScale_->Set(0); });
        ringPointScale_->Set(0);
        AnimationUtils::Animate(delayOption, [&]() { ringPointScale_->Set(1); });
    }

    totalScale_->Set(DEFAULT_TOTAL_SCALE);
    AnimationUtils::Animate(halfDurationOption, [&]() { totalScale_->Set(DEFAULT_SHRINK_SCALE); });
    totalScale_->Set(DEFAULT_SHRINK_SCALE);
    AnimationUtils::Animate(
        delayOption, [&]() { totalScale_->Set(1); },
        [isCheck, weakUiStatus = AceType::WeakClaim(AceType::RawPtr(uiStatus_))]() {
            auto uiStatus = weakUiStatus.Upgrade();
            if (uiStatus) {
                uiStatus->Set(static_cast<int32_t>(isCheck ? UIStatus::SELECTED : UIStatus::UNSELECTED));
            }
            auto context = PipelineBase::GetCurrentContext();
            if (context) {
                context->RequestFrame();
            }
        });
}

void RadioModifier::SetBoardColor(LinearColor color, int32_t duratuion, const RefPtr<CubicCurve>& curve)
{
    if (animateTouchHoverColor_) {
        AnimationOption option = AnimationOption();
        option.SetDuration(duratuion);
        option.SetCurve(curve);
        AnimationUtils::Animate(option, [&]() { animateTouchHoverColor_->Set(color); });
    }
}

void RadioModifier::PaintRadio(
    RSCanvas& canvas, bool /* checked */, const SizeF& contentSize, const OffsetF& contentOffset) const
{
    DrawTouchAndHoverBoard(canvas, contentSize, contentOffset);
    float outCircleRadius = contentSize.Width() / CALC_RADIUS;
    float centerX = contentOffset.GetX() + outCircleRadius;
    float centerY = contentOffset.GetY() + outCircleRadius;
    RSPen pen;
    RSPen outPen;
    RSBrush brush;
    RSBrush shadowBrush;
    pen.SetAntiAlias(true);
    pen.SetWidth(borderWidth_);
    outPen.SetAntiAlias(true);
    brush.SetAntiAlias(true);
    shadowBrush.SetAntiAlias(true);
    shadowBrush.SetColor(ToRSColor(shadowColor_));
    if (uiStatus_->Get() == static_cast<int32_t>(UIStatus::SELECTED)) {
        if (!enabled_->Get()) {
            brush.SetColor(
                ToRSColor(pointColor_->Get().BlendOpacity(static_cast<float>(DISABLED_ALPHA) / ENABLED_ALPHA)));
        } else {
            brush.SetColor(ToRSColor(pointColor_->Get()));
        }
        if (!NearZero(pointScale_->Get())) {
            // draw shadow
            canvas.AttachBrush(shadowBrush);
            canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius * pointScale_->Get() + shadowWidth_);
            // draw inner circle
            canvas.AttachBrush(brush);
            canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius * pointScale_->Get());
        }
        // draw ring circle
        if (!enabled_->Get()) {
            brush.SetColor(
                ToRSColor(inactivePointColor_.BlendOpacity(static_cast<float>(DISABLED_ALPHA) / ENABLED_ALPHA)));
        } else {
            brush.SetColor(ToRSColor(inactivePointColor_));
        }
        if (!NearZero(ringPointScale_->Get())) {
            canvas.AttachBrush(brush);
            canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius * ringPointScale_->Get());
        }
        // draw out circular ring
        if (!enabled_->Get()) {
            outPen.SetColor(
                ToRSColor(activeColor_->Get().BlendOpacity(static_cast<float>(DISABLED_ALPHA) / ENABLED_ALPHA)));
        } else {
            outPen.SetColor(ToRSColor(activeColor_->Get()));
        }
        auto outWidth = outCircleRadius * (totalScale_->Get() - pointScale_->Get() - ringPointScale_->Get());
        if (outWidth < borderWidth_) {
            outWidth = borderWidth_;
        }
        outPen.SetWidth(outWidth);
        canvas.AttachPen(outPen);
        canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius * totalScale_->Get() - outWidth / CALC_RADIUS);
    } else if (uiStatus_->Get() == static_cast<int32_t>(UIStatus::UNSELECTED)) {
        auto alphaCalculate = static_cast<float>(DISABLED_ALPHA) / ENABLED_ALPHA;
        if (!enabled_->Get()) {
            brush.SetColor(ToRSColor(inactivePointColor_.BlendOpacity(alphaCalculate)));
            pen.SetColor(ToRSColor(inactiveColor_->Get().BlendOpacity(alphaCalculate)));
        } else {
            brush.SetColor(ToRSColor(inactivePointColor_));
            pen.SetColor(ToRSColor(inactiveColor_->Get()));
        }
        canvas.AttachBrush(brush);
        canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius - borderWidth_);
        // draw border with unselected color
        canvas.AttachPen(pen);
        canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius - borderWidth_ / CALC_RADIUS);
    }
}

void RadioModifier::DrawTouchAndHoverBoard(RSCanvas& canvas, const SizeF& contentSize, const OffsetF& offset) const
{
    float outCircleRadius = contentSize.Width() / CALC_RADIUS;
    float centerX = outCircleRadius + offset.GetX();
    float centerY = outCircleRadius + offset.GetY();
    outCircleRadius += (defaultPadding_.ConvertToPx() + hotZoneHorizontalPadding_.ConvertToPx());
    RSBrush brush;
    brush.SetColor(ToRSColor(animateTouchHoverColor_->Get()));
    brush.SetAntiAlias(true);
    canvas.AttachBrush(brush);
    canvas.DrawCircle(RSPoint(centerX, centerY), outCircleRadius);
}

} // namespace OHOS::Ace::NG
