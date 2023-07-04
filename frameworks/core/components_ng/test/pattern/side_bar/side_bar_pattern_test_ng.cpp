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

#include "gtest/gtest.h"
#define private public
#define protected public
#include "base/memory/ref_counter.h"
#include "base/memory/referenced.h"
#include "core/animation/animator.h"
#include "core/animation/curve_animation.h"
#include "core/components_ng/base/ui_node.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/event/gesture_event_hub.h"
#include "core/components_ng/pattern/image/image_pattern.h"
#include "core/components_ng/pattern/pattern.h"
#include "core/components_ng/pattern/side_bar/side_bar_container_layout_property.h"
#include "core/components_ng/pattern/side_bar/side_bar_container_model_ng.h"
#include "core/components_ng/pattern/side_bar/side_bar_container_pattern.h"
#include "core/components_ng/pattern/side_bar/side_bar_theme.h"
#include "core/components_ng/test/mock/theme/mock_theme_manager.h"
#include "core/components_v2/extensions/extension.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/pipeline_ng/test/mock/mock_pipeline_base.h"

using namespace testing;
using namespace testing::ext;
namespace OHOS::Ace::NG {
namespace {
constexpr SideBarContainerType SIDE_BAR_CONTAINER_TYPE = SideBarContainerType::EMBED;
constexpr bool IS_SHOW = true;
constexpr bool SHOW_CONTROL_BUTTON = true;
constexpr Dimension SIDE_BAR_WIDTH = 50.0_vp;
constexpr Dimension MIN_SIDE_BAR_WIDTH = 50.0_vp;
constexpr Dimension MAX_SIDE_BAR_WIDTH = 50.0_vp;
constexpr Dimension MIN_CONTENT_WIDTH = 50.0_vp;
constexpr Dimension DEFAULT_MIN_SIDE_BAR_WIDTH = 200.0_vp;
constexpr Dimension DEFAULT_MAX_SIDE_BAR_WIDTH = 280.0_vp;
constexpr bool AUTO_HIDE = true;
constexpr SideBarPosition SIDE_BAR_POSITION = SideBarPosition::START;
constexpr Dimension WIDTH = 150.0_vp;
constexpr Dimension HEIGHT = 150.0_vp;
constexpr Dimension LEFT = 50.0_vp;
constexpr Dimension TOP = 50.0_vp;
constexpr float UPDATE_SIDE_BAR_POSITION_VALUE = 1.0f;
constexpr float REAL_SIDE_BAR_WIDTH_VALUE = 1.0f;
constexpr float CURRENT_OFFSET_VALUE = 1.0f;
constexpr float FLOAT_240 = 240.0f;
const std::string SHOW_ICON_STR = "123";
const std::string HIDDEN_ICON_STR = "123";
const std::string SWITCHING_ICON_STR = "123";
} // namespace

class SideBarPatternTestNg : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
};

void SideBarPatternTestNg::SetUpTestCase()
{
    MockPipelineBase::SetUp();
}

void SideBarPatternTestNg::TearDownTestCase()
{
    MockPipelineBase::TearDown();
}

/**
 * @tc.name: SideBarPatternTestNg001
 * @tc.desc: Test SideBar Create
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar and get frameNode.
     */
    SideBarContainerModelNG SideBarContainerModelInstance;
    SideBarContainerModelInstance.Create();
    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    EXPECT_NE(frameNode, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg002
 * @tc.desc: Test SideBar Set Func
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg002, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar and set the properties ,and then get frameNode.
     */
    SideBarContainerModelNG SideBarContainerModelInstance;
    SideBarContainerModelInstance.Create();
    SideBarContainerModelInstance.SetSideBarContainerType(SIDE_BAR_CONTAINER_TYPE);
    SideBarContainerModelInstance.SetShowSideBar(IS_SHOW);
    SideBarContainerModelInstance.SetShowControlButton(SHOW_CONTROL_BUTTON);
    SideBarContainerModelInstance.SetSideBarWidth(SIDE_BAR_WIDTH);
    SideBarContainerModelInstance.SetMinSideBarWidth(MIN_SIDE_BAR_WIDTH);
    SideBarContainerModelInstance.SetMaxSideBarWidth(MAX_SIDE_BAR_WIDTH);
    SideBarContainerModelInstance.SetMinContentWidth(MIN_CONTENT_WIDTH);
    SideBarContainerModelInstance.SetAutoHide(AUTO_HIDE);
    SideBarContainerModelInstance.SetSideBarPosition(SIDE_BAR_POSITION);
    SideBarContainerModelInstance.SetControlButtonWidth(WIDTH);
    SideBarContainerModelInstance.SetControlButtonHeight(HEIGHT);
    SideBarContainerModelInstance.SetControlButtonLeft(LEFT);
    SideBarContainerModelInstance.SetControlButtonTop(TOP);
    SideBarContainerModelInstance.SetControlButtonShowIconInfo(SHOW_ICON_STR, false, nullptr);
    SideBarContainerModelInstance.SetControlButtonHiddenIconInfo(HIDDEN_ICON_STR, false, nullptr);
    SideBarContainerModelInstance.SetControlButtonSwitchingIconInfo(SWITCHING_ICON_STR, false, nullptr);
    ChangeEvent eventOnChange = [](const bool) {};
    SideBarContainerModelInstance.SetOnChange(std::move(eventOnChange));
    SideBarContainerModelInstance.MarkNeedInitRealSideBarWidth();
    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    EXPECT_NE(frameNode, nullptr);

    /**
     * @tc.steps: step2. get the properties of all settings.
     * @tc.expected: step2. check whether the properties is correct.
     */
    auto sideBarLayoutProperty = frameNode->GetLayoutProperty<SideBarContainerLayoutProperty>();
    EXPECT_NE(sideBarLayoutProperty, nullptr);
    EXPECT_EQ(sideBarLayoutProperty->GetSideBarContainerType(), SIDE_BAR_CONTAINER_TYPE);
    EXPECT_EQ(sideBarLayoutProperty->GetShowSideBar(), IS_SHOW);
    EXPECT_EQ(sideBarLayoutProperty->GetShowControlButton(), SHOW_CONTROL_BUTTON);
    EXPECT_EQ(sideBarLayoutProperty->GetSideBarWidth(), SIDE_BAR_WIDTH);
    EXPECT_EQ(sideBarLayoutProperty->GetMinSideBarWidth(), MIN_SIDE_BAR_WIDTH);
    EXPECT_EQ(sideBarLayoutProperty->GetMaxSideBarWidth(), MAX_SIDE_BAR_WIDTH);
    EXPECT_EQ(sideBarLayoutProperty->GetMinContentWidth(), MIN_CONTENT_WIDTH);
    EXPECT_EQ(sideBarLayoutProperty->GetAutoHide(), AUTO_HIDE);
    EXPECT_EQ(sideBarLayoutProperty->GetSideBarPosition(), SIDE_BAR_POSITION);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonWidth(), WIDTH);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonHeight(), HEIGHT);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonLeft(), LEFT);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonTop(), TOP);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonShowIconInfo()->GetSrc(), SHOW_ICON_STR);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonHiddenIconInfo()->GetSrc(), HIDDEN_ICON_STR);
    EXPECT_EQ(sideBarLayoutProperty->GetControlButtonSwitchingIconInfo()->GetSrc(), SWITCHING_ICON_STR);
}

/**
 * @tc.name: SideBarPatternTestNg003
 * @tc.desc: Test SideBar OnModifyDone
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg003, TestSize.Level1)
{
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->OnModifyDone();
    SideBarContainerModelInstance.SetShowSideBar(true);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::SHOW);
    EXPECT_NE(pattern->dragEvent_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg004
 * @tc.desc: Test SideBar CreateAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg004, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->CreateAnimation();
    EXPECT_NE(pattern->controller_, nullptr);
    EXPECT_NE(pattern->rightToLeftAnimation_, nullptr);
    EXPECT_NE(pattern->leftToRightAnimation_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg005
 * @tc.desc: Test SideBar CreateAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg005, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->CreateAnimation();
    EXPECT_NE(pattern->controller_, nullptr);
    EXPECT_NE(pattern->rightToLeftAnimation_, nullptr);
    EXPECT_NE(pattern->leftToRightAnimation_, nullptr);
    pattern->CreateAnimation();
    EXPECT_NE(pattern->controller_, nullptr);
    EXPECT_NE(pattern->rightToLeftAnimation_, nullptr);
    EXPECT_NE(pattern->leftToRightAnimation_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg006
 * @tc.desc: Test SideBar DoSideBarAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg006, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->CreateAnimation();
    pattern->DoSideBarAnimation();
    EXPECT_EQ(pattern->showSideBar_, true);
}

/**
 * @tc.name: SideBarPatternTestNg007
 * @tc.desc: Test SideBar UpdateSideBarPosition
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg007, TestSize.Level1)
{
    float value = 0.0f;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    pattern->UpdateSideBarPosition(value);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    EXPECT_EQ(pattern->currentOffset_, 0);
}

/**
 * @tc.name: SideBarPatternTestNg008
 * @tc.desc: Test SideBar UpdateSideBarPosition
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg008, TestSize.Level1)
{
    float value = UPDATE_SIDE_BAR_POSITION_VALUE;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    pattern->realSideBarWidth_ = REAL_SIDE_BAR_WIDTH_VALUE;
    pattern->UpdateSideBarPosition(value);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    EXPECT_EQ(pattern->currentOffset_, CURRENT_OFFSET_VALUE);
}

/**
 * @tc.name: SideBarPatternTestNg009
 * @tc.desc: Test SideBar UpdateSideBarPosition
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg009, TestSize.Level1)
{
    float value = UPDATE_SIDE_BAR_POSITION_VALUE;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->realSideBarWidth_ = REAL_SIDE_BAR_WIDTH_VALUE;
    pattern->UpdateSideBarPosition(value);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    EXPECT_EQ(pattern->currentOffset_, CURRENT_OFFSET_VALUE);
}
/**
 * @tc.name: SideBarPatternTestNg010
 * @tc.desc: Test SideBar UpdateSideBarPosition
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg010, TestSize.Level1)
{
    float value = UPDATE_SIDE_BAR_POSITION_VALUE;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    EXPECT_FALSE(pattern == nullptr);
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::AUTO;
    pattern->realSideBarWidth_ = REAL_SIDE_BAR_WIDTH_VALUE;
    pattern->UpdateSideBarPosition(value);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    EXPECT_EQ(pattern->currentOffset_, CURRENT_OFFSET_VALUE);
}

/**
 * @tc.name: SideBarPatternTestNg011
 * @tc.desc: Test SideBar UpdateControlButtonIcon
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg011, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBarContainer,controlButton node,then mount controlButton node to its parent
     * frameNode.
     * @tc.expected: check whether the sideBarContainer node's children is not empty.
     */
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    pattern->AttachToFrameNode(frameNode);

    auto themeManagerOne = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineBase::GetCurrent()->SetThemeManager(themeManagerOne);
    EXPECT_CALL(*themeManagerOne, GetTheme(_)).WillRepeatedly(Return(AceType::MakeRefPtr<SideBarTheme>()));

    SideBarContainerModelInstance.CreateAndMountControlButton(frameNode);

    auto children = frameNode->GetChildren();
    ASSERT_FALSE(children.empty());

    /**
     * @tc.steps: step2. get the default color of controlButton from sidebar theme, then execute UpdateControlButtonIcon
     * method.
     * @tc.expected: check whether the controlButton's fillColor is same as the default color.
     */
    auto sideBarTheme = AceType::MakeRefPtr<SideBarTheme>();
    ASSERT_NE(sideBarTheme, nullptr);
    Color controlButtonColor = sideBarTheme->GetControlImageColor();

    auto imgFrameNode = AceType::DynamicCast<FrameNode>(children.front());
    auto imageLayoutProperty = imgFrameNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(imageLayoutProperty, nullptr);

    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    pattern->UpdateControlButtonIcon();
    auto imgSourceInfo = imageLayoutProperty->GetImageSourceInfoValue();
    EXPECT_EQ(imgSourceInfo.GetFillColor()->GetValue(), controlButtonColor.GetValue());

    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->UpdateControlButtonIcon();
    imgSourceInfo = imageLayoutProperty->GetImageSourceInfoValue();
    EXPECT_EQ(imgSourceInfo.GetFillColor()->GetValue(), controlButtonColor.GetValue());

    pattern->sideBarStatus_ = SideBarStatus::CHANGING;
    pattern->UpdateControlButtonIcon();
    imgSourceInfo = imageLayoutProperty->GetImageSourceInfoValue();
    EXPECT_EQ(imgSourceInfo.GetFillColor()->GetValue(), controlButtonColor.GetValue());
}

/**
 * @tc.name: SideBarPatternTestNg012
 * @tc.desc: Test SideBar InitControlButtonTouchEvent
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg012, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    int32_t imgNodeId = ElementRegister::GetInstance()->MakeUniqueId();
    auto imgNode = FrameNode::GetOrCreateFrameNode(
        OHOS::Ace::V2::IMAGE_ETS_TAG, imgNodeId, []() { return AceType::MakeRefPtr<ImagePattern>(); });
    EXPECT_FALSE(imgNode == nullptr);
    auto imgHub = imgNode->GetEventHub<EventHub>();
    EXPECT_FALSE(imgHub == nullptr);
    auto gestureHub = imgHub->GetOrCreateGestureEventHub();
    EXPECT_FALSE(gestureHub == nullptr);
    pattern->SetHasControlButton(true);
    EXPECT_EQ(pattern->controlButtonClickEvent_, nullptr);
    pattern->InitControlButtonTouchEvent(gestureHub);
    EXPECT_NE(pattern->controlButtonClickEvent_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg013
 * @tc.desc: Test SideBar DoAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg013, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->DoAnimation();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::RTL);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    pattern->animDir_ = SideBarAnimationDirection::LTR;
    EXPECT_EQ(pattern->currentOffset_, 0.0f);
}

/**
 * @tc.name: SideBarPatternTestNg014
 * @tc.desc: Test SideBar DoAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg014, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->DoAnimation();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::RTL);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    pattern->animDir_ = SideBarAnimationDirection::RTL;
    EXPECT_EQ(pattern->currentOffset_, 0.0f);
}

/**
 * @tc.name: SideBarPatternTestNg015
 * @tc.desc: Test SideBar DoAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg015, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->DoAnimation();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::RTL);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    pattern->realSideBarWidth_ = REAL_SIDE_BAR_WIDTH_VALUE;
    pattern->animDir_ = SideBarAnimationDirection::RTL;
    EXPECT_EQ(pattern->currentOffset_, 0.0f);
}

/**
 * @tc.name: SideBarPatternTestNg016
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg016, TestSize.Level1)
{
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    SideBarContainerModelInstance.SetSideBarPosition(SideBarPosition::START);
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::RTL);
}

/**
 * @tc.name: SideBarPatternTestNg017
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg017, TestSize.Level1)
{
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    SideBarContainerModelInstance.SetSideBarPosition(SideBarPosition::END);
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::RTL);
}

/**
 * @tc.name: SideBarPatternTestNg018
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg018, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::CHANGING;
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::LTR);
}

/**
 * @tc.name: SideBarPatternTestNg019
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg019, TestSize.Level1)
{
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::AUTO;
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::LTR);
}

/**
 * @tc.name: SideBarPatternTestNg020
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg020, TestSize.Level1)
{
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    SideBarContainerModelInstance.SetSideBarPosition(SideBarPosition::END);
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::LTR);
}

/**
 * @tc.name: SideBarPatternTestNg021
 * @tc.desc: Test SideBar UpdateAnimDir
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg021, TestSize.Level1)
{
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    EXPECT_FALSE(pattern == nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    EXPECT_FALSE(frameNode == nullptr);
    pattern->AttachToFrameNode(frameNode);
    stack->Push(frameNode);
    SideBarContainerModelInstance.SetSideBarPosition(SideBarPosition::START);
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->UpdateAnimDir();
    EXPECT_EQ(pattern->animDir_, SideBarAnimationDirection::LTR);
}

/**
 * @tc.name: SideBarSetOnChangeEvent001
 * @tc.desc: Test SideBar Set Func
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarSetOnChangeEvent001, TestSize.Level1)
{
    SideBarContainerModelNG sideBarContainerModelInstance;
    sideBarContainerModelInstance.Create();
    ChangeEvent eventOnChange = [](const bool isShow) { EXPECT_TRUE(isShow); };
    sideBarContainerModelInstance.SetOnChangeEvent(std::move(eventOnChange));
    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);

    auto sideBarContainerEventHub = frameNode->GetEventHub<NG::SideBarContainerEventHub>();
    ASSERT_NE(sideBarContainerEventHub, nullptr);
    sideBarContainerEventHub->SetOnChangeEvent(std::move(eventOnChange));
    sideBarContainerEventHub->FireChangeEvent(true);
    sideBarContainerEventHub->SetOnChangeEvent(nullptr);
    ASSERT_EQ(sideBarContainerEventHub->isShowChangeEvent_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg022
 * @tc.desc: Test SideBar OnDirtyLayoutWrapperSwap
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg022, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern, frameNode, sideBarLayoutProperty
        layoutWrapper .
     * @tc.expected: check whether the pattern->needInitRealSideBarWidth_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);

    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);
    auto sideBarLayoutProperty = frameNode->GetLayoutProperty<SideBarContainerLayoutProperty>();
    ASSERT_NE(sideBarLayoutProperty, nullptr);
    
    WeakPtr<FrameNode> hostNode = frameNode;
    auto geometryNode = AceType::MakeRefPtr<GeometryNode>();
    ASSERT_NE(geometryNode, nullptr);
    auto layoutProperty = AceType::MakeRefPtr<LayoutProperty>();
    ASSERT_NE(layoutProperty, nullptr);
    auto layoutWrapper = AceType::MakeRefPtr<LayoutWrapper>(hostNode, geometryNode, layoutProperty);
    ASSERT_NE(layoutWrapper, nullptr);
    auto layoutAlgorithmWrapper = AceType::MakeRefPtr<LayoutAlgorithmWrapper>(
        AceType::MakeRefPtr<SideBarContainerLayoutAlgorithm>());
    ASSERT_NE(layoutAlgorithmWrapper, nullptr);
    layoutWrapper->SetLayoutAlgorithm(layoutAlgorithmWrapper);

    auto algorithmWrapper = AceType::DynamicCast<LayoutAlgorithmWrapper>(layoutWrapper->GetLayoutAlgorithm());
    ASSERT_NE(algorithmWrapper, nullptr);
    auto algorithm = AceType::DynamicCast<SideBarContainerLayoutAlgorithm>(algorithmWrapper->GetLayoutAlgorithm());
    ASSERT_NE(algorithm, nullptr);

    DirtySwapConfig config;
    EXPECT_TRUE(pattern->needInitRealSideBarWidth_);
    pattern->OnDirtyLayoutWrapperSwap(layoutWrapper, config);
    EXPECT_FALSE(pattern->needInitRealSideBarWidth_);
}

/**
 * @tc.name: SideBarPatternTestNg023
 * @tc.desc: Test SideBar HandleDragStart
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg023, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern.
     * @tc.expected: check whether the pattern->sideBarStatus_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->HandleDragStart();
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::SHOW);
}

/**
 * @tc.name: SideBarPatternTestNg024
 * @tc.desc: Test SideBar HandleDragStart
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg024, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern.
     * @tc.expected: check whether the pattern->sideBarStatus_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::SHOW);
    /**
     * @tc.steps: step2. change pattern->sideBarStatus_.
     * @tc.expected: step2. check whether the pattern->sideBarStatus_ is correct.
     */
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->HandleDragStart();
    EXPECT_NE(pattern->sideBarStatus_, SideBarStatus::SHOW);
}

/**
 * @tc.name: SideBarPatternTestNg025
 * @tc.desc: Test SideBar HandleDragUpdate
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg025, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern and frameNode.
     * @tc.expected: check whether the pattern->sideBarStatus_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);

    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);
    pattern->frameNode_ = frameNode;
    
    pattern->sideBarStatus_ = SideBarStatus::SHOW;
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::SHOW);
    pattern->HandleDragUpdate(FLOAT_240);
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::CHANGING);
    /**
     * @tc.steps: step2. change pattern->sideBarStatus_.
     * @tc.expected: check whether the pattern->sideBarStatus_ is correct.
     */
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->HandleDragUpdate(FLOAT_240);
    EXPECT_NE(pattern->sideBarStatus_, SideBarStatus::SHOW);
}

/**
 * @tc.name: SideBarPatternTestNg026
 * @tc.desc: Test SideBar HandleDragEnd
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg026, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern.
     * @tc.expected: check whether the pattern->sideBarStatus_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    
    pattern->HandleDragEnd();
    EXPECT_EQ(pattern->sideBarStatus_, SideBarStatus::SHOW);
    /**
     * @tc.steps: step2. change pattern->sideBarStatus_.
     * @tc.expected: check whether the sideBarContainerEventHub is correct.
     */
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->HandleDragEnd();
    EXPECT_NE(pattern->sideBarStatus_, SideBarStatus::SHOW);
}

/**
 * @tc.name: SideBarPatternTestNg027
 * @tc.desc: Test SideBar FireChangeEvent
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg027, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern frameNode.
     * @tc.expected: check whether the sideBarContainerEventHub is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);
    pattern->frameNode_ = frameNode;
    auto sideBarContainerEventHub = pattern->GetEventHub<SideBarContainerEventHub>();
    pattern->FireChangeEvent(false);
    EXPECT_NE(sideBarContainerEventHub, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg028
 * @tc.desc: Test SideBar HandleDragUpdate
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg028, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern frameNode layoutProperty.
     * @tc.expected: check whether the showSideBar_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);

    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);
    pattern->frameNode_ = frameNode;

    auto layoutProperty = pattern->GetLayoutProperty<SideBarContainerLayoutProperty>();
    ASSERT_NE(layoutProperty, nullptr);
    /**
     * @tc.steps: step2. get minSideBarWidth, host, geometryNode .
     * @tc.expected: check whether objects is correct.
     */
    auto minSideBarWidth = layoutProperty->GetMinSideBarWidth().value_or(DEFAULT_MIN_SIDE_BAR_WIDTH);
    auto maxSideBarWidth = layoutProperty->GetMaxSideBarWidth().value_or(DEFAULT_MAX_SIDE_BAR_WIDTH);
    EXPECT_EQ(DEFAULT_MIN_SIDE_BAR_WIDTH.unit_, DimensionUnit::VP);

    auto host = pattern->GetHost();
    ASSERT_NE(host, nullptr);
    auto geometryNode = host->GetGeometryNode();
    ASSERT_NE(geometryNode, nullptr);

    auto frameSize = geometryNode->GetFrameSize();
    auto parentWidth = frameSize.Width();
    auto constraint = layoutProperty->GetLayoutConstraint();
    auto scaleProperty = constraint->scaleProperty;
    scaleProperty.vpScale = 1.0;
    auto minSideBarWidthPx = ConvertToPx(minSideBarWidth, scaleProperty, parentWidth).value_or(0);
    auto maxSideBarWidthPx = ConvertToPx(maxSideBarWidth, scaleProperty, parentWidth).value_or(0);

    auto sideBarPosition = layoutProperty->GetSideBarPosition().value_or(SideBarPosition::START);
    bool isSideBarStart = sideBarPosition == SideBarPosition::START;
    EXPECT_TRUE(isSideBarStart);

    auto sideBarLine = pattern->preSidebarWidth_ + (isSideBarStart ? FLOAT_240 : -FLOAT_240);
    EXPECT_EQ(sideBarLine, FLOAT_240);
    EXPECT_EQ(200, minSideBarWidthPx);
    EXPECT_EQ(280, maxSideBarWidthPx);

    pattern->HandleDragUpdate(FLOAT_240);
    /**
     * @tc.steps: step3. change pattern->dragRect_.width_ and param.
     * @tc.expected: check whether halfDragRegionWidth is correct.
     */
    pattern->dragRect_.width_ = 100.0f;
    pattern->HandleDragUpdate(-20.0f);
    auto halfDragRegionWidth = pattern->dragRect_.Width() / 2;
    EXPECT_EQ(halfDragRegionWidth, 50.0);
    /**
     * @tc.steps: step4. change param.
     * @tc.expected: check whether autoHide_ is correct.
     */
    auto autoHide_ = layoutProperty->GetAutoHide().value_or(true);
    pattern->HandleDragUpdate(-60.0f);
    EXPECT_TRUE(autoHide_);
}

/**
 * @tc.name: SideBarPatternTestNg029
 * @tc.desc: Test SideBar OnUpdateShowSideBar
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg029, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar ,then get pattern frameNode layoutProperty and newShowSideBar,
     *  modify showSideBar_.
     * @tc.expected: check whether the showSideBar_ is correct.
     */
    SideBarContainerModelNG sideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);

    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    ASSERT_NE(frameNode, nullptr);
    pattern->frameNode_ = frameNode;

    auto layoutProperty = pattern->GetLayoutProperty<SideBarContainerLayoutProperty>();
    ASSERT_NE(layoutProperty, nullptr);
    auto newShowSideBar = layoutProperty->GetShowSideBar().value_or(true);
    EXPECT_TRUE(newShowSideBar);

    EXPECT_TRUE(pattern->showSideBar_);
    pattern->showSideBar_ = false;
    EXPECT_FALSE(pattern->showSideBar_);
    pattern->OnUpdateShowSideBar(layoutProperty);
    EXPECT_TRUE(pattern->showSideBar_);
}

/**
 * @tc.name: SideBarPatternTestNg030
 * @tc.desc: Test SideBar DoSideBarAnimation
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg030, TestSize.Level1)
{
    /**
     * @tc.steps: step1. get pattern and frameNode, modify sideBarStatus_.
     * @tc.expected: check whether the showSideBar_ is correct.
     */
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    pattern->AttachToFrameNode(frameNode);
    pattern->CreateAnimation();
    pattern->sideBarStatus_ = SideBarStatus::HIDDEN;
    pattern->DoSideBarAnimation();
    EXPECT_EQ(pattern->showSideBar_, true);
}

/**
 * @tc.name: SideBarPatternTestNg031
 * @tc.desc: Test SideBar CreateAndMountControlButton
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg031, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBarContainer node, and set the state of ShowSideBar to false.
     * Then execute CreateAndMountControlButton method.
     * @tc.expected: check whether the controlButton's fillColor is same as the default color.
     */
    SideBarContainerModelNG SideBarContainerModelInstance;
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    stack->Push(frameNode);
    pattern->AttachToFrameNode(frameNode);

    auto themeManagerOne = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineBase::GetCurrent()->SetThemeManager(themeManagerOne);
    EXPECT_CALL(*themeManagerOne, GetTheme(_)).WillRepeatedly(Return(AceType::MakeRefPtr<SideBarTheme>()));

    SideBarContainerModelInstance.SetShowSideBar(false);
    SideBarContainerModelInstance.CreateAndMountControlButton(frameNode);

    auto children = frameNode->GetChildren();
    ASSERT_FALSE(children.empty());

    auto sideBarTheme = AceType::MakeRefPtr<SideBarTheme>();
    ASSERT_NE(sideBarTheme, nullptr);
    Color controlButtonColor = sideBarTheme->GetControlImageColor();

    auto imgFrameNode = AceType::DynamicCast<FrameNode>(children.front());
    auto imageLayoutProperty = imgFrameNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(imageLayoutProperty, nullptr);
    auto imgSourceInfo = imageLayoutProperty->GetImageSourceInfoValue();
    EXPECT_EQ(imgSourceInfo.GetFillColor()->GetValue(), controlButtonColor.GetValue());
}

/**
 * @tc.name: SideBarPatternTestNg032
 * @tc.desc: Test SideBar InitControlButtonMouseEvent
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg032, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBar and controlbutton node,then register inputEvent to inputeEventHub.
     * @tc.expected: check whether the controlButtonHoverEvent_ is not nullptr.
     */
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    pattern->AttachToFrameNode(frameNode);
    int32_t imgNodeId = ElementRegister::GetInstance()->MakeUniqueId();
    auto imgNode = FrameNode::GetOrCreateFrameNode(
        OHOS::Ace::V2::IMAGE_ETS_TAG, imgNodeId, []() { return AceType::MakeRefPtr<ImagePattern>(); });
    ASSERT_NE(imgNode, nullptr);
    auto imgHub = imgNode->GetEventHub<EventHub>();
    ASSERT_NE(imgHub, nullptr);
    auto inputHub = imgHub->GetOrCreateInputEventHub();
    ASSERT_NE(inputHub, nullptr);
    pattern->SetHasControlButton(true);
    EXPECT_EQ(pattern->controlButtonHoverEvent_, nullptr);
    pattern->InitControlButtonMouseEvent(inputHub);
    EXPECT_NE(pattern->controlButtonHoverEvent_, nullptr);
}

/**
 * @tc.name: SideBarPatternTestNg033
 * @tc.desc: Test SideBar OnControlButtonHover
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg033, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBarContainer frameNode, then execute OnControlButtonHover method(hover is true).
     * @tc.expected: check whether the isControlButtonHover_ is false.
     */
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    pattern->AttachToFrameNode(frameNode);

    pattern->OnControlButtonHover(true);
    EXPECT_FALSE(pattern->isControlButtonHover_);

    /**
     * @tc.steps: step2. create controlButton node,and mount controlButton node to its parent frameNode.
     * Then execute OnControlButtonHover method(hover is true).
     * @tc.expected: check whether the isControlButtonHover_ is true.
     */
    int32_t imgNodeId = ElementRegister::GetInstance()->MakeUniqueId();
    auto imgNode = FrameNode::GetOrCreateFrameNode(
        OHOS::Ace::V2::IMAGE_ETS_TAG, imgNodeId, []() { return AceType::MakeRefPtr<ImagePattern>(); });
    ASSERT_NE(imgNode, nullptr);
    imgNode->MountToParent(frameNode);
    pattern->OnControlButtonHover(true);
    EXPECT_TRUE(pattern->isControlButtonHover_);

    /**
     * @tc.steps: step3. remove hover from the controlButton.
     * @tc.expected: check whether the isControlButtonHover_ is false.
     */
    pattern->OnControlButtonHover(false);
    EXPECT_FALSE(pattern->isControlButtonHover_);
}

/**
 * @tc.name: SideBarPatternTestNg034
 * @tc.desc: Test SideBar HandleMouseEvent
 * @tc.type: FUNC
 */
HWTEST_F(SideBarPatternTestNg, SideBarPatternTestNg034, TestSize.Level1)
{
    /**
     * @tc.steps: step1. create sideBarContainer frameNode,then execute HandleMouseEvent method.
     * @tc.expected: check whether the isControlButtonHover_ is false.
     */
    auto pattern = AceType::MakeRefPtr<SideBarContainerPattern>();
    ASSERT_NE(pattern, nullptr);
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto frameNode = FrameNode::CreateFrameNode("Test", nodeId, pattern);
    ASSERT_NE(frameNode, nullptr);
    pattern->AttachToFrameNode(frameNode);

    MouseInfo info;
    info.SetAction(MouseAction::PRESS);
    info.SetButton(MouseButton::LEFT_BUTTON);
    pattern->HandleMouseEvent(info);
    EXPECT_FALSE(pattern->isControlButtonHover_);

    /**
     * @tc.steps: step2. create controlButton frameNode, and mount it to its parent frameNode.
     * Then execute HandleMouseEvent method.
     * @tc.expected: check whether the isControlButtonHover_ is false.
     */
    int32_t imgNodeId = ElementRegister::GetInstance()->MakeUniqueId();
    auto imgNode = FrameNode::GetOrCreateFrameNode(
        OHOS::Ace::V2::IMAGE_ETS_TAG, imgNodeId, []() { return AceType::MakeRefPtr<ImagePattern>(); });
    ASSERT_NE(imgNode, nullptr);
    imgNode->MountToParent(frameNode);

    pattern->HandleMouseEvent(info);
    EXPECT_FALSE(pattern->isControlButtonHover_);

    /**
     * @tc.steps: step3. execute OnControlButtonHover method(hover is true).
     * @tc.expected: check whether the isControlButtonHover_ is true.
     */
    pattern->OnControlButtonHover(true);
    pattern->HandleMouseEvent(info);
    info.SetAction(MouseAction::RELEASE);
    pattern->HandleMouseEvent(info);
    info.SetAction(MouseAction::HOVER_ENTER);
    pattern->HandleMouseEvent(info);
    info.SetButton(MouseButton::RIGHT_BUTTON);
    pattern->HandleMouseEvent(info);
    EXPECT_TRUE(pattern->isControlButtonHover_);
}
} // namespace OHOS::Ace::NG
