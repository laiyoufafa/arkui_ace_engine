/*
 * Copyright (c) 2023 iSoftStone Information Technology (Group) Co.,Ltd.
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

#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/pattern/checkboxgroup/checkboxgroup_pattern.h"
#include "core/components_ng/pattern/radio/radio_pattern.h"
#include "core/components_ng/pattern/stage/page_event_hub.h"
#include "core/components_ng/pattern/stage/page_pattern.h"
#include "core/components_ng/pattern/stage/stage_pattern.h"
#include "core/pipeline/base/element_register.h"
#include "core/pipeline_ng/test/mock/mock_pipeline_base.h"
#include "core/common/container.h"
#include "core/common/ace_engine.h"
#include "core/common/container_scope.h"
#include "test/mock/core/common/mock_container.h"
#include "core/components_ng/pattern/custom/custom_measure_layout_node.h"
#undef private
#undef protected

using namespace testing;
using namespace testing::ext;

namespace OHOS::Ace::NG {
namespace {
const std::string CHECK_STRING = "check";
const std::string TEST_GROUP_NAME = "testGroup";
const std::string ERROR_GROUP_NAME = "errorGroup";
const std::string FRAME_NODE_TAG = "testFrameNode";
const std::string TEST_ANIMATOR_ID = "testAnimatorId";
constexpr int32_t RADIO_ID_FIRST = 1;
constexpr int32_t RADIO_ID_SECOND = 2;
constexpr int32_t RADIO_ID_THIRD = 3;
constexpr int32_t CHECK_BOX_ID_FIRST = 4;
constexpr int32_t CHECK_BOX_ID_SECOND = 5;
constexpr int32_t CHECK_BOX_ID_THIRD = 6;
constexpr int32_t CHECK_BOX_ID_FOURTH = 6;
constexpr int32_t TEST_CONTAINER_ID = 100;
using OnPageShow = void (*)();
using OnPageHide = void (*)();
} // namespace

class StageTestNg : public testing::Test {
public:
    static void SetUpTestSuite()
    {
        MockPipelineBase::SetUp();
        MockContainer::SetUp();
        ContainerScope::UpdateCurrent(TEST_CONTAINER_ID);
        AceEngine::Get().AddContainer(TEST_CONTAINER_ID, MockContainer::Current());
    }
    static void TearDownTestSuite()
    {
        MockPipelineBase::TearDown();
    }
};

/**
 * @tc.name: PageEventHubTest001
 * @tc.desc: Testing Radio Correlation Functions of PageEventHub work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PageEventHubTest001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Build a PageEventHub.
     */
    PageEventHub pageEventHub;
    pageEventHub.UpdateRadioGroupValue(TEST_GROUP_NAME, RADIO_ID_FIRST);

    /**
     * @tc.steps: step2. Add radio to group.
     * @tc.expected: The HasRadioId function of PageEventHub meets expectations .
     */
    pageEventHub.AddRadioToGroup(TEST_GROUP_NAME, RADIO_ID_FIRST);
    EXPECT_TRUE(pageEventHub.HasRadioId(TEST_GROUP_NAME, RADIO_ID_FIRST));
    EXPECT_FALSE(pageEventHub.HasRadioId(ERROR_GROUP_NAME, RADIO_ID_FIRST));

    /**
     * @tc.steps: step3. Add another two radio to group.
     * @tc.expected: The HasRadioId function of PageEventHub meets expectations .
     */
    pageEventHub.AddRadioToGroup(TEST_GROUP_NAME, RADIO_ID_SECOND);
    pageEventHub.AddRadioToGroup(TEST_GROUP_NAME, RADIO_ID_THIRD);

    /**
     * @tc.steps: step4. Create real node and fake node to ElementRegister.
     */
    auto radioPattern = AceType::MakeRefPtr<RadioPattern>();
    auto radioNode = FrameNode::CreateFrameNode(FRAME_NODE_TAG, RADIO_ID_FIRST, radioPattern);
    ElementRegister::GetInstance()->AddReferenced(RADIO_ID_FIRST, radioNode);
    auto checkBoxGroup = AceType::MakeRefPtr<CheckBoxGroupPattern>();
    auto checkBoxGroupNode = FrameNode::CreateFrameNode(FRAME_NODE_TAG, RADIO_ID_SECOND, checkBoxGroup);
    ElementRegister::GetInstance()->AddReferenced(RADIO_ID_SECOND, checkBoxGroupNode);

    /**
     * @tc.steps: step5. UpdateRadioGroupValue.
     */
    pageEventHub.UpdateRadioGroupValue(TEST_GROUP_NAME, RADIO_ID_FIRST);
    pageEventHub.UpdateRadioGroupValue(TEST_GROUP_NAME, RADIO_ID_SECOND);

    /**
     * @tc.steps: step6. RemoveRadioFromGroup.
     * @tc.expected: The radio remove successful .
     */
    pageEventHub.RemoveRadioFromGroup(TEST_GROUP_NAME, RADIO_ID_FIRST);
    EXPECT_FALSE(pageEventHub.HasRadioId(TEST_GROUP_NAME, RADIO_ID_FIRST));
}

/**
 * @tc.name: PageEventHubTest002
 * @tc.desc: Testing CheckBox Correlation Functions of PageEventHub work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PageEventHubTest002, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Build a PageEventHub.
     * @tc.expected: The CheckBoxGroupMap size meets expectations .
     */
    PageEventHub pageEventHub;
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap().size(), 0);

    /**
     * @tc.steps: step2. build error pattern and add to the group.
     * @tc.expected: The CheckBoxGroupMap[TEST_GROUP_NAME] has the error pattern.
     */
    auto errorPattern = AceType::MakeRefPtr<RadioPattern>();
    auto errorNode = FrameNode::CreateFrameNode(V2::CHECKBOXGROUP_ETS_TAG, CHECK_BOX_ID_FIRST, errorPattern);
    ElementRegister::GetInstance()->AddReferenced(CHECK_BOX_ID_FIRST, errorNode);
    pageEventHub.RemoveCheckBoxFromGroup(TEST_GROUP_NAME, CHECK_BOX_ID_FIRST);
    pageEventHub.AddCheckBoxGroupToGroup(TEST_GROUP_NAME, CHECK_BOX_ID_FIRST);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 1);

    /**
     * @tc.steps: step3. remove error pattern.
     * @tc.expected: The CheckBoxGroupMap[TEST_GROUP_NAME] has the error pattern.
     */
    ElementRegister::GetInstance()->RemoveItem(CHECK_BOX_ID_FIRST);
    pageEventHub.RemoveCheckBoxFromGroup(TEST_GROUP_NAME, CHECK_BOX_ID_FIRST);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 0);

    /**
     * @tc.steps: step4. add real checkBoxGroup and add to the group.
     * @tc.expected: add success.
     */
    auto checkBoxGroup = AceType::MakeRefPtr<CheckBoxGroupPattern>();
    auto checkBoxGroupNode = FrameNode::CreateFrameNode(V2::CHECKBOXGROUP_ETS_TAG, CHECK_BOX_ID_THIRD, checkBoxGroup);
    ElementRegister::GetInstance()->AddReferenced(CHECK_BOX_ID_THIRD, checkBoxGroupNode);
    pageEventHub.AddCheckBoxGroupToGroup(TEST_GROUP_NAME, CHECK_BOX_ID_THIRD);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 1);

    /**
     * @tc.steps: step5. add checkBox to group
     * @tc.expected: add success.
     */
    pageEventHub.AddCheckBoxToGroup(TEST_GROUP_NAME, CHECK_BOX_ID_SECOND);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 2);

    /**
     * @tc.steps: step6. remove the checkBoxGroup from group.
     * @tc.expected: remove success.
     */
    ElementRegister::GetInstance()->RemoveItem(CHECK_BOX_ID_THIRD);
    pageEventHub.RemoveCheckBoxFromGroup(TEST_GROUP_NAME, CHECK_BOX_ID_THIRD);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 1);

    /**
     * @tc.steps: step7. add checkBoxGroup to group again.
     * @tc.expected: add success.
     */
    ElementRegister::GetInstance()->AddReferenced(CHECK_BOX_ID_THIRD, checkBoxGroupNode);
    pageEventHub.AddCheckBoxGroupToGroup(TEST_GROUP_NAME, CHECK_BOX_ID_THIRD);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 2);

    /**
     * @tc.steps: step8. build second checkBoxGroup to group.
     * @tc.expected: add fail.
     */
    auto checkBoxGroup2 = AceType::MakeRefPtr<CheckBoxGroupPattern>();
    auto checkBoxGroupNode2 =
        FrameNode::CreateFrameNode(V2::CHECKBOXGROUP_ETS_TAG, CHECK_BOX_ID_FOURTH, checkBoxGroup2);
    ElementRegister::GetInstance()->AddReferenced(CHECK_BOX_ID_FOURTH, checkBoxGroupNode2);
    pageEventHub.RemoveCheckBoxFromGroup(TEST_GROUP_NAME, CHECK_BOX_ID_FIRST);
    EXPECT_EQ(pageEventHub.GetCheckBoxGroupMap()[TEST_GROUP_NAME].size(), 2);
}

/**
 * @tc.name: StageManagerTest001
 * @tc.desc: Testing Functions of StageManager work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, StageManagerTest001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create StagePattern and some PagePattern.
     */
    auto stageNode = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 0, AceType::MakeRefPtr<StagePattern>());
    auto firstNode =
        FrameNode::CreateFrameNode("1", 1, AceType::MakeRefPtr<PagePattern>(AceType::MakeRefPtr<PageInfo>()));
    auto secondNode =
        FrameNode::CreateFrameNode("2", 2, AceType::MakeRefPtr<PagePattern>(AceType::MakeRefPtr<PageInfo>()));
    auto thirdNode =
        FrameNode::CreateFrameNode("3", 3, AceType::MakeRefPtr<PagePattern>(AceType::MakeRefPtr<PageInfo>()));
    auto fourthNode =
        FrameNode::CreateFrameNode("4", 4, AceType::MakeRefPtr<PagePattern>(AceType::MakeRefPtr<PageInfo>()));

    /**
     * @tc.steps: step2. Create a StageManager based on stageNode.
     * @tc.expected: stagePattern_ successfully assigned a value.
     */
    StageManager stageManager(stageNode);
    EXPECT_NE(stageManager.stagePattern_, nullptr);

    /**
     * @tc.steps: step3. PopPage.
     * @tc.expected: Expected no child failed.
     */
    EXPECT_FALSE(stageManager.PopPage());

    /**
     * @tc.steps: step4. Push a Page into StageManager.
     * @tc.expected: Push successfully.
     */
    EXPECT_TRUE(stageManager.PushPage(firstNode));

    /**
     * @tc.steps: step5. Push another three Page with different parameters into StageManager.
     * @tc.expected: Push successfully.
     */
    stageManager.PushPage(secondNode, false, false);
    stageManager.PushPage(thirdNode, true, false);
    stageManager.PushPage(fourthNode, false, true);
    EXPECT_EQ(stageNode->GetChildren().size(), 4);

    /**
     * @tc.steps: step6. Push an exist page.
     * @tc.expected: StageNode size not changed.
     */
    stageManager.PushPage(secondNode);
    EXPECT_EQ(stageNode->GetChildren().size(), 4);

    /**
     * @tc.steps: step7. MovePageToFront first Page and GetLastPage.
     * @tc.expected: always return success , last node is firstNode.
     */
    EXPECT_TRUE(stageManager.MovePageToFront(firstNode));
    EXPECT_TRUE(stageManager.MovePageToFront(firstNode));
    EXPECT_EQ(stageManager.GetLastPage(), firstNode);
    EXPECT_EQ(stageNode->GetChildren().size(), 4);
    /**
     * @tc.steps: step8. PopPage to index 0.
     * @tc.expected: always return success.
     */
    stageManager.PopPageToIndex(1);
    EXPECT_EQ(stageNode->GetChildren().size(), 3);

    /**
     * @tc.steps: step9. PopPage with different parameters.
     * @tc.expected: removeChild meets expectations .
     */
    stageManager.PopPage(true, false);
    stageManager.PopPage(false, true);
    EXPECT_EQ(stageNode->GetChildren().size(), 2);

    /**
     * @tc.steps: step10. Try clean with different parameters.
     * @tc.expected: stageManager clear success ,size meets expectations.
     */
    stageManager.StopPageTransition();
    stageManager.ReloadStage();
    stageManager.CleanPageStack();
    EXPECT_EQ(stageNode->GetChildren().size(), 1);
}

/**
 * @tc.name: PagePatternTest001
 * @tc.desc: Testing OnDirtyLayoutWrapperSwap of PagePattern work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create a PagePattern.
     */
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());

    /**
     * @tc.steps: step2. Calling the SetFirstBuildCallback function.
     * @tc.expected: The callback firstBuildCallback_ in the pagePattern not nullptr.
     */
    pattern.SetFirstBuildCallback([]() {});
    EXPECT_NE(pattern.firstBuildCallback_, nullptr);

    /**
     * @tc.steps: step3. Build a DirtySwapConfig and call the OnDirtyLayoutWrapperSwap function.
     * @tc.expected: he callback firstBuildCallback_ in the pagePattern cleared.
     */
    DirtySwapConfig config;
    pattern.OnDirtyLayoutWrapperSwap(nullptr, config);
    EXPECT_EQ(pattern.firstBuildCallback_, nullptr);

    /**
     * @tc.steps: step4. Call SetFirstBuildCallback again.
     * @tc.expected: The callback firstBuildCallback_ in the pagePattern is nullptr .
     */
    pattern.SetFirstBuildCallback([]() {});
    EXPECT_EQ(pattern.firstBuildCallback_, nullptr);
}

/**
 * @tc.name: PagePatternTest002
 * @tc.desc: Testing OnDirtyLayoutWrapperSwap of PagePattern work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest002, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create some node and PagePattern.
     */
    auto node = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 0, AceType::MakeRefPtr<StagePattern>());
    auto child = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 1, AceType::MakeRefPtr<StagePattern>());
    node->AddChild(child);
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());
    pattern.AttachToFrameNode(node);

    /**
     * @tc.steps: step2. get child node renderContext and set shareId.
     */
    child->GetRenderContext()->SetShareId("shareId");

    /**
     * @tc.steps: step3. Calling the BuildSharedTransitionMap function.
     * @tc.expected: The property sharedTransitionMap_ in the pagePattern size meets expectations.
     */
    pattern.BuildSharedTransitionMap();
    EXPECT_EQ(pattern.sharedTransitionMap_.size(), 1);
}

/**
 * @tc.name: PagePatternTest003
 * @tc.desc: Testing ProcessHideState And ProcessShowState of PagePattern work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest003, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create some node and PagePattern.
     */
    auto parent = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 0, AceType::MakeRefPtr<StagePattern>());
    auto node = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 1, AceType::MakeRefPtr<StagePattern>());
    auto child = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 2, AceType::MakeRefPtr<StagePattern>());
    parent->AddChild(node);
    node->AddChild(child);
    child->GetRenderContext()->SetShareId("shareId");
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());
    pattern.AttachToFrameNode(node);

    /**
     * @tc.steps: step2. Calling the ProcessShowState function.
     * @tc.expected: Call node isActive function return true.
     */
    pattern.ProcessShowState();
    EXPECT_TRUE(node->IsActive());

    /**
     * @tc.steps: step3. Calling the ProcessHideState function.
     * @tc.expected: Call node isActive function return false.
     */
    pattern.ProcessHideState();
    EXPECT_FALSE(node->IsActive());
}

/**
 * @tc.name: PagePatternTest004
 * @tc.desc: Testing the Show and Hide Related Methods of PagePattern.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest004, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create some node and PagePattern.
     */
    auto parent = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 0, AceType::MakeRefPtr<StagePattern>());
    auto node = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 1, AceType::MakeRefPtr<StagePattern>());
    auto child = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 2, AceType::MakeRefPtr<StagePattern>());
    parent->AddChild(node);
    node->AddChild(child);
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());
    pattern.AttachToFrameNode(node);

    /**
     * @tc.steps: step2. Calling the MarkRenderDone function.
     * @tc.expected: The property isRenderDone_ in the pagePattern is true
     */
    pattern.MarkRenderDone();
    EXPECT_TRUE(pattern.isRenderDone_);

    /**
     * @tc.steps: step3. Calling the SetOnPageShow function.
     * @tc.expected: The property onPageShow_ in the pagePattern not nullptr
     */
    pattern.SetOnPageShow([]() {});
    EXPECT_NE(pattern.onPageShow_, nullptr);

    /**
     * @tc.steps: step4. Calling the SetOnPageHide function.
     * @tc.expected: The property onPageHide_ in the pagePattern not nullptr
     */
    pattern.SetOnPageHide([]() {});
    EXPECT_NE(pattern.onPageHide_, nullptr);

    /**
     * @tc.steps: step5. Get container and set state_ Frontend::State::ON_SHOW.
     */
    Container::Current()->state_ = Frontend::State::ON_SHOW;

    /**
     * @tc.steps: step6. Calling the OnShow function.
     * @tc.expected: The property isOnShow_ in the pagePattern is true.
     */
    pattern.OnShow();
    EXPECT_TRUE(pattern.isOnShow_);

    /**
     * @tc.steps: step7. Calling the OnHide function.
     * @tc.expected: The property isOnShow_ in the pagePattern is false.
     */
    pattern.OnHide();
    EXPECT_FALSE(pattern.isOnShow_);
}

/**
 * @tc.name: PagePatternTest005
 * @tc.desc: Test the PageTransition related functions in the PagePattern work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest005, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create some node and PagePattern.
     */
    auto parent = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 0, AceType::MakeRefPtr<StagePattern>());
    auto node = FrameNode::CreateFrameNode(FRAME_NODE_TAG, 1, AceType::MakeRefPtr<StagePattern>());
    auto child = CustomMeasureLayoutNode::CreateCustomMeasureLayoutNode(2, "child");
    parent->AddChild(node);
    node->AddChild(child);
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());
    pattern.AttachToFrameNode(node);
    pattern.ReloadPage();

    /**
     * @tc.steps: step2. Create PageTransitionEffect with option.
     */
    PageTransitionOption option;
    option.routeType = RouteType::POP;
    auto effect = AceType::MakeRefPtr<PageTransitionEffect>(PageTransitionType::ENTER, option);

    /**
     * @tc.steps: step3. Calling the AddPageTransition function.
     * @tc.expected: Calling the GetTopTransition function returned not nullptr.
     */
    pattern.AddPageTransition(effect);
    EXPECT_NE(pattern.GetTopTransition(), nullptr);

    /**
     * @tc.steps: step4. Calling the TriggerPageTransition function.
     * @tc.expected: Attribute pageTransitionFinish_ not nullptr.
     */
    pattern.TriggerPageTransition(PageTransitionType::ENTER_POP, []() {});
    EXPECT_NE(pattern.pageTransitionFinish_, nullptr);

    /**
     * @tc.steps: step5. Calling the StopPageTransition function.
     * @tc.expected: Attribute pageTransitionFinish_ is nullptr.
     */
    pattern.StopPageTransition();
    EXPECT_EQ(pattern.pageTransitionFinish_, nullptr);

    /**
     * @tc.steps: step6.The PageTransitionEffect SetUserCallback and PagePattern SetPageTransitionFunc.
     */
    effect->SetUserCallback([](RouteType routeType, const float& value) {});
    pattern.SetPageTransitionFunc([]() {});

    /**
     * @tc.steps: step7.Calling the TriggerPageTransition function and stop it.
     * @tc.expected: Attribute pageTransitionFinish_ not nullptr.
     */
    pattern.TriggerPageTransition(PageTransitionType::ENTER_POP, []() {});
    pattern.StopPageTransition();
    EXPECT_NE(pattern.pageTransitionFinish_, nullptr);

    /**
     * @tc.steps: step8.Calling the ClearPageTransitionEffect function.
     * @tc.expected: The GetTopTransition function returns a nullptr.
     */
    pattern.ClearPageTransitionEffect();
    EXPECT_EQ(pattern.GetTopTransition(), nullptr);
}

/**
 * @tc.name: PagePatternTest006
 * @tc.desc: Testing AddJsAnimator And GetJsAnimator of PagePattern work correctly.
 * @tc.type: FUNC
 */
HWTEST_F(StageTestNg, PagePatternTest006, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create PagePattern.
     */
    PagePattern pattern(AceType::MakeRefPtr<PageInfo>());

    /**
     * @tc.steps: step2. Create AnimatorInfo.
     */
    auto animatorInfo = AceType::MakeRefPtr<Framework::AnimatorInfo>();

    /**
     * @tc.steps: step3. Calling the GetJsAnimator function in the PagePattern.
     * @tc.expected: The GetJsAnimator function returned nullptr;
     */
    EXPECT_EQ(pattern.GetJsAnimator(TEST_ANIMATOR_ID), nullptr);

    /**
     * @tc.steps: step4. Calling the SetAnimator function in the PagePattern.
     * @tc.expected: The GetJsAnimator function returned not nullptr;
     */
    animatorInfo->SetAnimator(AceType::MakeRefPtr<Animator>(TEST_ANIMATOR_ID.c_str()));
    pattern.AddJsAnimator(TEST_ANIMATOR_ID, animatorInfo);
    EXPECT_NE(pattern.GetJsAnimator(TEST_ANIMATOR_ID), nullptr);
}
} // namespace OHOS::Ace::NG