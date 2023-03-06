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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_RATING_RATING_MODIFIER_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_RATING_RATING_MODIFIER_H

#include "base/memory/referenced.h"
#include "base/utils/utils.h"
#include "core/components/common/properties/animation_option.h"
#include "core/components/rating/rating_theme.h"
#include "core/components_ng/base/modifier.h"
#include "core/components_ng/render/animation_utils.h"
#include "core/components_ng/render/drawing.h"
#include "core/pipeline/pipeline_base.h"

namespace OHOS::Ace::NG {
class RatingModifier : public ContentModifier {
    DECLARE_ACE_TYPE(RatingModifier, ContentModifier);

public:
    RatingModifier();
    ~RatingModifier() override = default;

    enum class RatingAnimationType {
        NONE = 0,
        HOVER,
        HOVERTOPRESS,
        PRESSTOHOVER,
        PRESS,
    };

    void PaintBoard(DrawingContext& context);
    void PaintStar(DrawingContext& context);

    void onDraw(DrawingContext& context) override;

    void UpdateCanvasImage(const RefPtr<CanvasImage>& foregroundImageCanvas,
        const RefPtr<CanvasImage>& secondaryImageCanvas, const RefPtr<CanvasImage>& backgroundImageCanvas,
        const ImagePaintConfig& singleStarImagePaintConfig)
    {
        if (!JudgeCanvasImage(foregroundImageCanvas, secondaryImageCanvas, backgroundImageCanvas)) {
            return;
        }
        SetNeedDraw(true);
        foregroundImageCanvas_ = foregroundImageCanvas;
        secondaryImageCanvas_ = secondaryImageCanvas;
        backgroundImageCanvas_ = backgroundImageCanvas;
        foregroundImageCanvas_->SetPaintConfig(singleStarImagePaintConfig);
        secondaryImageCanvas_->SetPaintConfig(singleStarImagePaintConfig);
        backgroundImageCanvas_->SetPaintConfig(singleStarImagePaintConfig);
    }

    bool JudgeCanvasImage(const RefPtr<CanvasImage>& foreground, const RefPtr<CanvasImage>& secondary,
        const RefPtr<CanvasImage>& background)
    {
        if (foreground != foregroundImageCanvas_) {
            return true;
        }
        if (secondary != secondaryImageCanvas_) {
            return true;
        }
        if (background != backgroundImageCanvas_) {
            return true;
        }
        return false;
    }

    void SetNeedDraw(bool flag)
    {
        if (needDraw_) {
            needDraw_->Set(flag);
        }
    }

    void SetBoardColor(LinearColor color, int32_t times, const RefPtr<CubicCurve>& curve)
    {
        if (boardColor_) {
            AnimationOption option = AnimationOption();
            option.SetDuration(times);
            option.SetCurve(curve);
            AnimationUtils::Animate(option, [&]() { boardColor_->Set(color); });
        }
    }

    void SetContentOffset(OffsetF contentOffset)
    {
        if (contentOffset_) {
            contentOffset_->Set(contentOffset);
        }
    }

    void SetDrawScore(double drawScore)
    {
        if (drawScore_) {
            drawScore_->Set(static_cast<float>(drawScore));
        }
    }

    void SetStepSize(double stepSize)
    {
        if (stepSize_) {
            stepSize_->Set(static_cast<float>(stepSize));
        }
    }

    void SetTouchStar(int32_t touchStar)
    {
        if (touchStar_) {
            touchStar_->Set(touchStar);
        }
    }

    void SetStartNum(int32_t starNum)
    {
        if (starNum_) {
            starNum_->Set(starNum);
        }
    }

    void SetHoverState(const RatingAnimationType& state)
    {
        auto pipeline = PipelineBase::GetCurrentContext();
        CHECK_NULL_VOID(pipeline);
        auto ratingTheme = pipeline->GetTheme<RatingTheme>();
        CHECK_NULL_VOID(ratingTheme);
        auto hoverDuration = static_cast<int32_t>(ratingTheme->GetHoverAnimationDuration());
        auto pressDuration = static_cast<int32_t>(ratingTheme->GetPressAnimationDuration());
        switch (state) {
            case RatingAnimationType::HOVER:
                SetBoardColor(LinearColor(ratingTheme->GetHoverColor()), hoverDuration, Curves::FRICTION);
                break;
            case RatingAnimationType::HOVERTOPRESS:
                SetBoardColor(LinearColor(ratingTheme->GetPressColor()), pressDuration, Curves::SHARP);
                break;
            case RatingAnimationType::PRESSTOHOVER:
                SetBoardColor(LinearColor(ratingTheme->GetHoverColor()), pressDuration, Curves::SHARP);
                break;
            case RatingAnimationType::PRESS:
                SetBoardColor(LinearColor(ratingTheme->GetPressColor()), hoverDuration, Curves::SHARP);
                break;
            case RatingAnimationType::NONE:
                SetBoardColor(LinearColor(Color::TRANSPARENT), hoverDuration, Curves::FRICTION);
                break;
            default:
                break;
        }
    }

private:
    RefPtr<CanvasImage> foregroundImageCanvas_;
    RefPtr<CanvasImage> secondaryImageCanvas_;
    RefPtr<CanvasImage> backgroundImageCanvas_;
    // non-animatable property
    RefPtr<PropertyBool> needDraw_;
    RefPtr<PropertyInt> starNum_;
    RefPtr<PropertyInt> touchStar_;
    RefPtr<PropertyFloat> drawScore_;
    RefPtr<PropertyFloat> stepSize_; 
    RefPtr<PropertyOffsetF> contentOffset_;
    // animatable property
    RefPtr<AnimatablePropertyColor> boardColor_;
    ACE_DISALLOW_COPY_AND_MOVE(RatingModifier);
};
} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_RATING_RATING_MODIFIER_H
