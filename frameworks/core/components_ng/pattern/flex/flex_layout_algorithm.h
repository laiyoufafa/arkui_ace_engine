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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FLEX_FLEX_LAYOUT_ALGORITHM_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FLEX_FLEX_LAYOUT_ALGORITHM_H

#include "core/components/common/layout/constants.h"
#include "core/components_ng/layout/layout_algorithm.h"
#include "core/components_ng/layout/layout_wrapper.h"
#include "core/components_ng/pattern/flex/flex_layout_styles.h"

namespace OHOS::Ace::NG {

struct FlexItemProperties {
    float totalShrink = 0.0f;
    float totalGrow = 0.0f;
    RefPtr<LayoutWrapper> lastShrinkChild;
    RefPtr<LayoutWrapper> lastGrowChild;
};

struct MagicLayoutNode {
    LayoutConstraintF layoutConstraint;
    RefPtr<LayoutWrapper> layoutWrapper;
    OptionalSizeF calcSize;
    bool needSecondMeasure = false;
};

struct BaselineProperties {
    float maxBaselineDistance = 0.0f;
    float maxDistanceAboveBaseline = 0.0f;
    float maxDistanceBelowBaseline = 0.0f;

    void Reset()
    {
        maxBaselineDistance = 0.0f;
        maxDistanceAboveBaseline = 0.0f;
        maxDistanceBelowBaseline = 0.0f;
    }
};

class ACE_EXPORT FlexLayoutAlgorithm : public LayoutAlgorithm {
    DECLARE_ACE_TYPE(FlexLayoutAlgorithm, LayoutAlgorithm);

public:
    FlexLayoutAlgorithm() = default;
    ~FlexLayoutAlgorithm() override = default;

    void Measure(LayoutWrapper* layoutWrapper) override;

    void Layout(LayoutWrapper* layoutWrapper) override;

    void SetLinearLayoutFeature()
    {
        isLinearLayoutFeature_ = true;
    }

private:
    void InitFlexProperties(LayoutWrapper* layoutWrapper);
    void TravelChildrenFlexProps(LayoutWrapper* layoutWrapper, const SizeF& realSize);
    void UpdateAllocatedSize(const RefPtr<LayoutWrapper>& layoutWrapper, float& crossAxisSize);
    float GetChildMainAxisSize(const RefPtr<LayoutWrapper>& layoutWrapper) const;
    float GetChildCrossAxisSize(const RefPtr<LayoutWrapper>& layoutWrapper) const;
    float GetSelfCrossAxisSize(const RefPtr<LayoutWrapper>& layoutWrapper) const;
    void CheckSizeValidity(const RefPtr<LayoutWrapper>& layoutWrapper);
    void CheckBaselineProperties(const RefPtr<LayoutWrapper>& layoutWrapper);
    void CalculateSpace(float remainSpace, float& frontSpace, float& betweenSpace) const;
    void PlaceChildren(
        LayoutWrapper* layoutWrapper, float frontSpace, float betweenSpace, const OffsetF& paddingOffset);
    FlexAlign GetSelfAlign(const RefPtr<LayoutWrapper>& layoutWrapper) const;
    float GetStretchCrossAxisLimit() const;
    void MeasureOutOfLayoutChildren(LayoutWrapper* layoutWrapper);
    void MeasureAndCleanMagicNodes(FlexItemProperties& flexItemProperties);
    void SecondaryMeasureByProperty(FlexItemProperties& flexItemProperties, LayoutWrapper* layoutWrapper);
    void UpdateLayoutConstraintOnMainAxis(LayoutConstraintF& layoutConstraint, float size);
    void UpdateLayoutConstraintOnCrossAxis(LayoutConstraintF& layoutConstraint, float size);
    void AdjustTotalAllocatedSize(LayoutWrapper* layoutWrapper);
    void getFlexFunc(std::function<float(const RefPtr<LayoutWrapper>&)>& getFlex, float remainSpace,
        float& spacePerFlex, FlexItemProperties& flexItemProperties, RefPtr<LayoutWrapper>& lastChild);
    void CheckBlankAndKeepMin(const RefPtr<LayoutWrapper>& childLayoutWrapper, float& flexSize);

    OptionalSizeF realSize_;
    float mainAxisSize_ = 0.0f;
    float crossAxisSize_ = 0.0f;
    float selfIdealCrossAxisSize_ = -1.0f;
    float allocatedSize_ = 0.0f;
    float space_ = 0.0f;
    float totalFlexWeight_ = 0.0f;
    int32_t maxDisplayPriority_ = 0;
    int32_t validSizeCount_ = 0;
    FlexAlign crossAxisAlign_ = FlexAlign::FLEX_START;
    FlexAlign mainAxisAlign_ = FlexAlign::FLEX_START;

    std::map<int32_t, std::list<MagicLayoutNode>> magicNodes_;
    std::map<int32_t, float> magicNodeWeights_;
    std::list<MagicLayoutNode> secondaryMeasureList_;
    std::list<RefPtr<LayoutWrapper>> outOfLayoutChildren_;

    FlexDirection direction_ = FlexDirection::ROW;
    friend class LinearLayoutUtils;
    BaselineProperties baselineProperties_;
    bool isLinearLayoutFeature_ = false;
    bool isInfiniteLayout_ = false;
    bool selfAdaptive_ = false;
    TextDirection textDir_ = TextDirection::LTR;
    bool childrenHasAlignSelfBaseLine_ = false;

    ACE_DISALLOW_COPY_AND_MOVE(FlexLayoutAlgorithm);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FLEX_FLEX_LAYOUT_ALGORITHM_H
