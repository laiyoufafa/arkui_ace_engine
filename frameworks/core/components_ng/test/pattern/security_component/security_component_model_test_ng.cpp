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

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>

#include "gtest/gtest.h"

#define protected public
#define private public
#include "base/geometry/dimension.h"
#include "base/memory/ace_type.h"
#include "base/memory/referenced.h"
#include "base/utils/system_properties.h"
#include "core/components/common/layout/constants.h"
#include "core/components/common/properties/color.h"
#include "core/components/common/properties/text_style.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/button/button_layout_property.h"
#include "core/components_ng/pattern/button/button_pattern.h"
#include "core/components_ng/pattern/image/image_pattern.h"
#include "core/components_ng/pattern/security_component/location_button/location_button_common.h"
#include "core/components_ng/pattern/security_component/location_button/location_button_model_ng.h"
#include "core/components_ng/pattern/security_component/paste_button/paste_button_common.h"
#include "core/components_ng/pattern/security_component/paste_button/paste_button_model_ng.h"
#include "core/components_ng/pattern/security_component/save_button/save_button_common.h"
#include "core/components_ng/pattern/security_component/save_button/save_button_model_ng.h"
#include "core/components_ng/pattern/security_component/security_component_handler.h"
#include "core/components_ng/pattern/security_component/security_component_pattern.h"
#include "core/components_ng/pattern/security_component/security_component_theme.h"
#include "core/components_ng/pattern/text/text_pattern.h"
#include "core/components_ng/test/mock/theme/mock_theme_manager.h"
#include "core/pipeline_ng/test/mock/mock_pipeline_base.h"
#include "core/pipeline_ng/ui_task_scheduler.h"
#undef protected
#undef private

using namespace testing;
using namespace testing::ext;

namespace OHOS::Ace::NG {
namespace {
    constexpr float DEFAULT_ICON_SIZE = 16.0f;
    constexpr float DEFAULT_FONT_SIZE = 16.0f;
    constexpr float DEFAULT_ICON_MIN_SIZE = 12.0f;
    constexpr float DEFAULT_FONT_MIN_SIZE = 12.0f;
    constexpr float DEFAULT_PADDING = 10.0f;
    constexpr float DEFAULT_BORDER_RADIUS = 1.0f;
    constexpr float DEFAULT_BORDER_WIDTH = 1.0f;
    constexpr float DEFAULT_PADDING_WITHOUT_BG = 4.0f;
    constexpr float DEFAULT_BUTTON_SIZE = 20.0f;
    const std::string DEFAULT_TEXT = "Add Security Component Buttom";
    constexpr int INDEX_ZERO = 0;
    constexpr int INDEX_ONE = 1;
    constexpr int INDEX_TWO = 2;
    constexpr int INDEX_SIZE = 3;
    constexpr int ICON_RESOURCE_TABLE = 2;
}

#ifdef SECURITY_COMPONENT_ENABLE
namespace {
    constexpr float MAX_ROTATE = 360.0f;
}
#endif

class SecurityComponentModelTestNg : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
protected:
    static RefPtr<FrameNode> CreateSecurityComponent(int32_t text, int32_t icon,
        int32_t backgroundType, std::string type);
    static void CreateSecurityComponentNotFinish(int32_t text, int32_t icon,
        int32_t backgroundType, std::string type);
    static RefPtr<LayoutWrapper> CreateLayoutWrapper(RefPtr<FrameNode>& node);
    static RefPtr<LayoutWrapper> CreateSecurityComponentLayoutWrapper(RefPtr<FrameNode>& node);
    static void CheckSecurityComponentDefaultProp(RefPtr<FrameNode>& node);
    static void InitDefaultTheme(RefPtr<SecurityComponentTheme>& theme);
    static void SetLocationUserDefinedPropty();
    static void SetPasteUserDefinedPropty();
    static void SetSaveUserDefinedPropty();
};

class MockSecurityComponentButtonLayoutAlgorithm : public BoxLayoutAlgorithm {
public:
    MockSecurityComponentButtonLayoutAlgorithm() = default;

    ~MockSecurityComponentButtonLayoutAlgorithm() override = default;

    void Measure(LayoutWrapper* layoutWrapper) override
    {
        SizeF frameSize(DEFAULT_BUTTON_SIZE, DEFAULT_BUTTON_SIZE);
        layoutWrapper->GetGeometryNode()->SetFrameSize(frameSize);
    }
};

void SecurityComponentModelTestNg::InitDefaultTheme(RefPtr<SecurityComponentTheme>& theme)
{
    theme->iconSize_ = Dimension(DEFAULT_ICON_SIZE);
    theme->fontSize_ = Dimension(DEFAULT_FONT_SIZE);
    theme->minIconSize_ = Dimension(DEFAULT_ICON_MIN_SIZE);
    theme->minFontSize_ = Dimension(DEFAULT_FONT_MIN_SIZE);
    theme->backgroundTopPadding_ = Dimension(DEFAULT_PADDING);
    theme->backgroundRightPadding_ = Dimension(DEFAULT_PADDING);
    theme->backgroundBottomPadding_ = Dimension(DEFAULT_PADDING);
    theme->backgroundLeftPadding_ = Dimension(DEFAULT_PADDING);
    theme->textIconSpace_ = Dimension(DEFAULT_PADDING);
    theme->borderRadius_ = Dimension(DEFAULT_BORDER_RADIUS);
    theme->borderWidth_ = Dimension(DEFAULT_BORDER_WIDTH);
    theme->paddingWithoutBg_ = Dimension(DEFAULT_PADDING_WITHOUT_BG);
    theme->iconColor_ = Color::WHITE;
    theme->fontColor_ = Color::WHITE;
    theme->backgroundColor_ = Color::BLUE;
    theme->iconColorNoBg_ = Color::BLUE;
    theme->fontColorNoBg_ = Color::BLACK;
    theme->borderColor_ = Color::RED;
}

void SecurityComponentModelTestNg::SetUpTestCase()
{
    MockPipelineBase::SetUp();
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineBase::GetCurrent()->SetThemeManager(themeManager);
    auto scTheme = AceType::MakeRefPtr<SecurityComponentTheme>();
    InitDefaultTheme(scTheme);
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly(Return(scTheme));
}

void SecurityComponentModelTestNg::TearDownTestCase()
{
    MockPipelineBase::TearDown();
}

RefPtr<FrameNode> SecurityComponentModelTestNg::CreateSecurityComponent(int32_t text, int32_t icon,
    int32_t backgroundType, std::string type)
{
    if (type == V2::SEC_LOCATION_BUTTON_ETS_TAG) {
        LocationButtonModelNG locationSc;
        locationSc.Create(text, icon, backgroundType);
    } else if (type == V2::SEC_PASTE_BUTTON_ETS_TAG) {
        PasteButtonModelNG pasteSc;
        pasteSc.Create(text, icon, backgroundType);
    } else if (type == V2::SEC_SAVE_BUTTON_ETS_TAG) {
        SaveButtonModelNG saveSc;
        saveSc.Create(text, icon, backgroundType);
    }
    RefPtr<UINode> element = ViewStackProcessor::GetInstance()->Finish();
    return AceType::DynamicCast<FrameNode>(element);
}

void SecurityComponentModelTestNg::CreateSecurityComponentNotFinish(int32_t text, int32_t icon,
    int32_t backgroundType, std::string type)
{
    if (type == V2::SEC_LOCATION_BUTTON_ETS_TAG) {
        LocationButtonModelNG locationSc;
        locationSc.Create(text, icon, backgroundType);
    } else if (type == V2::SEC_PASTE_BUTTON_ETS_TAG) {
        PasteButtonModelNG pasteSc;
        pasteSc.Create(text, icon, backgroundType);
    } else if (type == V2::SEC_SAVE_BUTTON_ETS_TAG) {
        SaveButtonModelNG saveSc;
        saveSc.Create(text, icon, backgroundType);
    }
}

void SecurityComponentModelTestNg::SetLocationUserDefinedPropty()
{
    LocationButtonModelNG sc;
    sc.SetFontSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconColor(Color::RED);
    sc.SetFontStyle(Ace::FontStyle::ITALIC);
    sc.SetFontWeight(FontWeight::BOLDER);
    std::vector<std::string> fontFamilies = { "testFont" };
    sc.SetFontFamily(fontFamilies);
    sc.SetFontColor(Color::GREEN);
    sc.SetBackgroundColor(Color::GRAY);
    sc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundBorderColor(Color::GREEN);
    sc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    sc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconSpace(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    sc.SetlayoutOrder(SecSecurityComponentLayoutOrder::TEXT_FIRST);
}

void SecurityComponentModelTestNg::SetPasteUserDefinedPropty()
{
    PasteButtonModelNG sc;
    sc.SetFontSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconColor(Color::RED);
    sc.SetFontStyle(Ace::FontStyle::ITALIC);
    sc.SetFontWeight(FontWeight::BOLDER);
    std::vector<std::string> fontFamilies = { "testFont" };
    sc.SetFontFamily(fontFamilies);
    sc.SetFontColor(Color::GREEN);
    sc.SetBackgroundColor(Color::GRAY);
    sc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundBorderColor(Color::GREEN);
    sc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    sc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconSpace(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    sc.SetlayoutOrder(SecSecurityComponentLayoutOrder::TEXT_FIRST);
}

void SecurityComponentModelTestNg::SetSaveUserDefinedPropty()
{
    SaveButtonModelNG sc;
    sc.SetFontSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconSize(Dimension(15.0)); // 15.0 vp
    sc.SetIconColor(Color::RED);
    sc.SetFontStyle(Ace::FontStyle::ITALIC);
    sc.SetFontWeight(FontWeight::BOLDER);
    std::vector<std::string> fontFamilies = { "testFont" };
    sc.SetFontFamily(fontFamilies);
    sc.SetFontColor(Color::GREEN);
    sc.SetBackgroundColor(Color::GRAY);
    sc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundBorderColor(Color::GREEN);
    sc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    sc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    sc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconSpace(Dimension(25.0)); // 25.0 vp
    sc.SetTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    sc.SetlayoutOrder(SecSecurityComponentLayoutOrder::TEXT_FIRST);
}

RefPtr<LayoutWrapper> SecurityComponentModelTestNg::CreateLayoutWrapper(RefPtr<FrameNode>& node)
{
    RefPtr<GeometryNode> geometryNode = AceType::MakeRefPtr<GeometryNode>();
    if (geometryNode == nullptr) {
        return nullptr;
    }
    return AceType::MakeRefPtr<LayoutWrapper>(node, geometryNode, node->GetLayoutProperty());
}

RefPtr<LayoutWrapper> SecurityComponentModelTestNg::CreateSecurityComponentLayoutWrapper(RefPtr<FrameNode>& node)
{
    auto layoutWrapper = CreateLayoutWrapper(node);
    auto buttonNode = GetSecCompChildNode(node, V2::BUTTON_ETS_TAG);
    CHECK_NULL_RETURN(buttonNode, nullptr);
    auto buttonWrapper = CreateLayoutWrapper(buttonNode);
    auto buttonAlgorithm = AceType::MakeRefPtr<MockSecurityComponentButtonLayoutAlgorithm>();
    CHECK_NULL_RETURN(buttonAlgorithm, nullptr);
    buttonWrapper->SetLayoutAlgorithm(AceType::MakeRefPtr<LayoutAlgorithmWrapper>(buttonAlgorithm));

    auto iconNode = GetSecCompChildNode(node, V2::IMAGE_ETS_TAG);
    CHECK_NULL_RETURN(iconNode, nullptr);
    auto iconWrapper = CreateLayoutWrapper(iconNode);
    auto iconPattern = iconNode->GetPattern<ImagePattern>();
    CHECK_NULL_RETURN(iconPattern, nullptr);
    auto iconAlgorithm = iconPattern->CreateLayoutAlgorithm();
    CHECK_NULL_RETURN(iconAlgorithm, nullptr);
    iconWrapper->SetLayoutAlgorithm(AceType::MakeRefPtr<LayoutAlgorithmWrapper>(iconAlgorithm));

    auto textNode = GetSecCompChildNode(node, V2::TEXT_ETS_TAG);
    CHECK_NULL_RETURN(textNode, nullptr);
    auto textWrapper = CreateLayoutWrapper(textNode);
    auto textPattern = textNode->GetPattern<TextPattern>();
    CHECK_NULL_RETURN(textPattern, nullptr);
    auto textAlgorithm = textPattern->CreateLayoutAlgorithm();
    CHECK_NULL_RETURN(textAlgorithm, nullptr);
    textWrapper->SetLayoutAlgorithm(AceType::MakeRefPtr<LayoutAlgorithmWrapper>(textAlgorithm));

    layoutWrapper->childrenMap_[INDEX_ZERO] = buttonWrapper;
    layoutWrapper->childrenMap_[INDEX_ONE] = iconWrapper;
    layoutWrapper->childrenMap_[INDEX_TWO] = textWrapper;
    layoutWrapper->currentChildCount_ = INDEX_SIZE;
    return layoutWrapper;
}

void SecurityComponentModelTestNg::CheckSecurityComponentDefaultProp(RefPtr<FrameNode>& node)
{
    auto property = node->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetSecurityComponentDescription().value_or(1), 0);
    EXPECT_EQ(property->GetIconStyle().value_or(1), 0);
    EXPECT_EQ(property->GetBackgroundType().value_or(static_cast<int32_t>(ButtonType::NORMAL)),
        static_cast<int32_t>(ButtonType::CAPSULE));
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING);
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING);
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::VERTICAL),
        SecurityComponentLayoutDirection::HORIZONTAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::TEXT_FIRST),
        SecSecurityComponentLayoutOrder::ICON_FIRST);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest001
 * @tc.desc: Test default properties of security component.
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);

    CheckSecurityComponentDefaultProp(frameNode);

    // text node property
    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetMaxLines().value_or(0), 1);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_FONT_SIZE);
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::ITALIC), Ace::FontStyle::NORMAL);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::NORMAL), FontWeight::MEDIUM);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    EXPECT_FALSE(fontFamily.empty());
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::WHITE);

    // icon node property
    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::WHITE);

    // button node property
    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::RED);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_WIDTH);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::NONE);

    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_RADIUS);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::BLUE);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::NORMAL), ButtonType::CAPSULE);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest002
 * @tc.desc: Test default properties of security component without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        BUTTON_TYPE_NULL, V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetBackgroundType().value_or(static_cast<int32_t>(ButtonType::NORMAL)),
        BUTTON_TYPE_NULL);
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);

    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::BLACK);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::BLUE);

    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::TRANSPARENT);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::CAPSULE), ButtonType::NORMAL);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest003
 * @tc.desc: Test default properties of security component without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest003, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(1, -1,
        BUTTON_TYPE_NULL, V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetIconStyle().value_or(1), -1);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_EQ(iconNode, nullptr);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest004
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest004, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    SetLocationUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest005
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest005, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    SetLocationUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest006
 * @tc.desc: Test set security component property without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest006, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, BUTTON_TYPE_NULL, V2::SEC_LOCATION_BUTTON_ETS_TAG);
    LocationButtonModelNG locationSc;
    locationSc.SetBackgroundColor(Color::GRAY);
    locationSc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    locationSc.SetBackgroundBorderColor(Color::GREEN);
    locationSc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    locationSc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    locationSc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_NE(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_NE(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_EQ(widthProp, nullptr);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_NE(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
}

/**
 * @tc.name: SecurityComponentLocationPropertyTest007
 * @tc.desc: Test set security component property without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPropertyTest007, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(1, -1, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_LOCATION_BUTTON_ETS_TAG);
    LocationButtonModelNG locationSc;
    locationSc.SetTextIconSpace(Dimension(15.0)); // 15.0vp

    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);
}

/**
 * @tc.name: SecurityComponentLocationPatternTest001
 * @tc.desc: Test default security component modifyDone
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPatternTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    EXPECT_EQ(secCompGesture->GetHitTestMode(), HitTestMode::HTMDEFAULT);

    // modifydone again
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);
}

/**
 * @tc.name: SecurityComponentLocationPatternTest002
 * @tc.desc: Test event callback
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationPatternTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    auto clickListener = secCompGesture->clickEventActuator_->clickEvents_.back();
    ASSERT_NE(clickListener, nullptr);
    ASSERT_NE(clickListener->callback_, nullptr);
    GestureEvent info;
    clickListener->callback_(info);

    auto eventHub = frameNode->GetEventHub<EventHub>();
    ASSERT_NE(eventHub, nullptr);
    ASSERT_NE(eventHub->onAppear_, nullptr);
    ASSERT_NE(eventHub->onDisappear_, nullptr);
    eventHub->onAppear_();
    eventHub->onDisappear_();
    ASSERT_EQ(pattern->scId_, -1);
}

/**
 * @tc.name: SecurityComponentLocationLayoutAlgoTest001
 * @tc.desc: Test default layout algo
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationLayoutAlgoTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto layoutAlgo = pattern->CreateLayoutAlgorithm();
    ASSERT_NE(layoutAlgo, nullptr);

    auto layoutWrapper = CreateSecurityComponentLayoutWrapper(frameNode);
    ASSERT_NE(layoutWrapper, nullptr);
    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    property->UpdateBackgroundType(static_cast<int32_t>(ButtonType::CAPSULE));
    property->UpdateTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    property->UpdateLayoutOrder(SecSecurityComponentLayoutOrder::ICON_FIRST);
    pattern->OnModifyDone();

    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);

    property->UpdateBackgroundType(static_cast<int32_t>(ButtonType::CIRCLE));
    property->UpdateTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    property->UpdateLayoutOrder(SecSecurityComponentLayoutOrder::ICON_FIRST);
    pattern->OnModifyDone();

    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);
}

/**
 * @tc.name: SecurityComponentLocationAccessibilityPropertyTest001
 * @tc.desc: Test security component accessibility property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentLocationAccessibilityPropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto accessibility = pattern->CreateAccessibilityProperty();
    ASSERT_NE(accessibility, nullptr);
    accessibility->SetHost(frameNode);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest001
 * @tc.desc: Test default properties of security component.
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);

    CheckSecurityComponentDefaultProp(frameNode);

    // text node property
    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetMaxLines().value_or(0), 1);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_FONT_SIZE);
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::ITALIC), Ace::FontStyle::NORMAL);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::NORMAL), FontWeight::MEDIUM);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    EXPECT_FALSE(fontFamily.empty());
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::WHITE);

    // icon node property
    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::WHITE);

    // button node property
    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::RED);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_WIDTH);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::NONE);

    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_RADIUS);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::BLUE);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::NORMAL), ButtonType::CAPSULE);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest002
 * @tc.desc: Test default properties of security component without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        BUTTON_TYPE_NULL, V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetBackgroundType().value_or(static_cast<int32_t>(ButtonType::NORMAL)),
        BUTTON_TYPE_NULL);
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);

    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::BLACK);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::BLUE);

    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::TRANSPARENT);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::CAPSULE), ButtonType::NORMAL);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest003
 * @tc.desc: Test default properties of security component without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest003, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(1, -1,
        BUTTON_TYPE_NULL, V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetIconStyle().value_or(1), -1);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_EQ(iconNode, nullptr);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest004
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest004, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    SetSaveUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest005
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest005, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    SetSaveUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest006
 * @tc.desc: Test set security component property without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest006, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, BUTTON_TYPE_NULL, V2::SEC_SAVE_BUTTON_ETS_TAG);
    SaveButtonModelNG saveSc;
    saveSc.SetBackgroundColor(Color::GRAY);
    saveSc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    saveSc.SetBackgroundBorderColor(Color::GREEN);
    saveSc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    saveSc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    saveSc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp

    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_NE(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_NE(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_EQ(widthProp, nullptr);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_NE(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
}

/**
 * @tc.name: SecurityComponentSavePropertyTest007
 * @tc.desc: Test set security component property without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePropertyTest007, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(1, -1, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_SAVE_BUTTON_ETS_TAG);
    SaveButtonModelNG saveSc;
    saveSc.SetTextIconSpace(Dimension(15.0)); // 15.0vp

    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);
}

/**
 * @tc.name: SecurityComponentSavePatternTest001
 * @tc.desc: Test default security component modifyDone
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePatternTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    EXPECT_EQ(secCompGesture->GetHitTestMode(), HitTestMode::HTMDEFAULT);

    // modifydone again
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);
}

/**
 * @tc.name: SecurityComponentSavePatternTest002
 * @tc.desc: Test event callback
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSavePatternTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    auto clickListener = secCompGesture->clickEventActuator_->clickEvents_.back();
    ASSERT_NE(clickListener, nullptr);
    ASSERT_NE(clickListener->callback_, nullptr);
    GestureEvent info;
    clickListener->callback_(info);

    auto eventHub = frameNode->GetEventHub<EventHub>();
    ASSERT_NE(eventHub, nullptr);
    ASSERT_NE(eventHub->onAppear_, nullptr);
    ASSERT_NE(eventHub->onDisappear_, nullptr);
    eventHub->onAppear_();
    eventHub->onDisappear_();
    ASSERT_EQ(pattern->scId_, -1);
}

/**
 * @tc.name: SecurityComponentSaveLayoutAlgoTest001
 * @tc.desc: Test default layout algo
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSaveLayoutAlgoTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto layoutAlgo = pattern->CreateLayoutAlgorithm();
    ASSERT_NE(layoutAlgo, nullptr);

    auto layoutWrapper = CreateSecurityComponentLayoutWrapper(frameNode);
    ASSERT_NE(layoutWrapper, nullptr);
    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    property->UpdateBackgroundType(static_cast<int32_t>(ButtonType::CAPSULE));
    property->UpdateTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    property->UpdateLayoutOrder(SecSecurityComponentLayoutOrder::ICON_FIRST);
    pattern->OnModifyDone();

    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);
}

/**
 * @tc.name: SecurityComponentSaveAccessibilityPropertyTest001
 * @tc.desc: Test security component accessibility property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentSaveAccessibilityPropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_SAVE_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto accessibility = pattern->CreateAccessibilityProperty();
    ASSERT_NE(accessibility, nullptr);
    accessibility->SetHost(frameNode);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest001
 * @tc.desc: Test default properties of security component.
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);

    CheckSecurityComponentDefaultProp(frameNode);

    // text node property
    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetMaxLines().value_or(0), 1);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_FONT_SIZE);
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::ITALIC), Ace::FontStyle::NORMAL);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::NORMAL), FontWeight::MEDIUM);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    EXPECT_FALSE(fontFamily.empty());
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::WHITE);

    // icon node property
    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::WHITE);

    // button node property
    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::RED);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_WIDTH);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::NONE);

    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_BORDER_RADIUS);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::BLUE);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::NORMAL), ButtonType::CAPSULE);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest002
 * @tc.desc: Test default properties of security component without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        BUTTON_TYPE_NULL, V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetBackgroundType().value_or(static_cast<int32_t>(ButtonType::NORMAL)),
        BUTTON_TYPE_NULL);
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), DEFAULT_PADDING_WITHOUT_BG);
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(),
        DEFAULT_PADDING_WITHOUT_BG);

    auto textNode = GetSecCompChildNode(frameNode, V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::BLACK);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::BLUE);

    auto buttonNode = GetSecCompChildNode(frameNode, V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::TRANSPARENT);
    EXPECT_EQ(buttonProp->GetType().value_or(ButtonType::CAPSULE), ButtonType::NORMAL);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest003
 * @tc.desc: Test default properties of security component without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest003, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(1, -1,
        BUTTON_TYPE_NULL, V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetIconStyle().value_or(1), -1);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);

    auto iconNode = GetSecCompChildNode(frameNode, V2::IMAGE_ETS_TAG);
    ASSERT_EQ(iconNode, nullptr);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest004
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest004, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    SetPasteUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest005
 * @tc.desc: Test set security component property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest005, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    SetPasteUserDefinedPropty();
    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_EQ(property->GetTextIconLayoutDirection().value_or(SecurityComponentLayoutDirection::HORIZONTAL),
        SecurityComponentLayoutDirection::VERTICAL);
    EXPECT_EQ(property->GetLayoutOrder().value_or(SecSecurityComponentLayoutOrder::ICON_FIRST),
        SecSecurityComponentLayoutOrder::TEXT_FIRST);

    auto iconNode = GetCurSecCompChildNode(V2::IMAGE_ETS_TAG);
    ASSERT_NE(iconNode, nullptr);
    auto iconProp = iconNode->GetLayoutProperty<ImageLayoutProperty>();
    ASSERT_NE(iconProp, nullptr);
    ImageSourceInfo iconSrc = iconProp->GetImageSourceInfo().value_or(ImageSourceInfo());
    EXPECT_EQ(iconSrc.GetFillColor().value_or(Color()), Color::RED);
    auto textNode = GetCurSecCompChildNode(V2::TEXT_ETS_TAG);
    ASSERT_NE(textNode, nullptr);
    auto textProp = textNode->GetLayoutProperty<TextLayoutProperty>();
    ASSERT_NE(textProp, nullptr);
    EXPECT_EQ(textProp->GetFontSize().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(textProp->GetItalicFontStyle().value_or(Ace::FontStyle::NORMAL), Ace::FontStyle::ITALIC);
    EXPECT_EQ(textProp->GetFontWeight().value_or(FontWeight::MEDIUM), FontWeight::BOLDER);
    std::vector<std::string> emptyVec;
    auto fontFamily = textProp->GetFontFamily().value_or(emptyVec);
    ASSERT_FALSE(fontFamily.empty());
    EXPECT_EQ(fontFamily[0], "testFont");
    EXPECT_EQ(textProp->GetTextColor().value_or(Color()), Color::GREEN);

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_EQ(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_NE(widthProp, nullptr);
    EXPECT_EQ(widthProp->leftDimen.value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_EQ(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
    EXPECT_EQ(buttonProp->GetBorderRadius().value_or(Dimension(0.0)).ConvertToVp(), 15.0); // 15.0 vp
    EXPECT_EQ(buttonRender->GetBackgroundColor().value_or(Color()), Color::GRAY);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest006
 * @tc.desc: Test set security component property without background
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest006, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(0, 0, BUTTON_TYPE_NULL, V2::SEC_PASTE_BUTTON_ETS_TAG);
    PasteButtonModelNG pasteSc;
    pasteSc.SetBackgroundColor(Color::GRAY);
    pasteSc.SetBackgroundBorderWidth(Dimension(15.0)); // 15.0 vp
    pasteSc.SetBackgroundBorderColor(Color::GREEN);
    pasteSc.SetBackgroundBorderStyle(BorderStyle::DOTTED);
    pasteSc.SetBackgroundBorderRadius(Dimension(15.0)); // 15.0 vp
    pasteSc.SetBackgroundPadding(Dimension(25.0)); // 25.0 vp

    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_NE(property->GetBackgroundLeftPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundRightPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundTopPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp
    EXPECT_NE(property->GetBackgroundBottomPadding().value_or(Dimension(0.0)).ConvertToVp(), 25.0); // 25.0 vp

    auto buttonNode = GetCurSecCompChildNode(V2::BUTTON_ETS_TAG);
    ASSERT_NE(buttonNode, nullptr);
    auto buttonProp = buttonNode->GetLayoutProperty<ButtonLayoutProperty>();
    ASSERT_NE(buttonProp, nullptr);
    auto buttonRender = buttonNode->GetRenderContext();
    ASSERT_NE(buttonRender, nullptr);
    auto borderColor = buttonRender->GetBorderColor().value_or(BorderColorProperty());
    EXPECT_NE(borderColor.leftColor.value_or(Color()), Color::GREEN);
    auto& widthProp = buttonProp->GetBorderWidthProperty();
    ASSERT_EQ(widthProp, nullptr);
    auto borderStyle = buttonRender->GetBorderStyle().value_or(BorderStyleProperty());
    EXPECT_NE(borderStyle.styleLeft.value_or(BorderStyle::SOLID), BorderStyle::DOTTED);
}

/**
 * @tc.name: SecurityComponentPastePropertyTest007
 * @tc.desc: Test set security component property without icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePropertyTest007, TestSize.Level1)
{
    CreateSecurityComponentNotFinish(1, -1, static_cast<int32_t>(ButtonType::CAPSULE),
        V2::SEC_PASTE_BUTTON_ETS_TAG);
    PasteButtonModelNG pasteSc;
    pasteSc.SetTextIconSpace(Dimension(15.0)); // 15.0vp

    auto frameNode = ViewStackProcessor::GetInstance()->GetMainFrameNode();
    ASSERT_NE(frameNode, nullptr);
    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    EXPECT_EQ(property->GetTextIconSpace().value_or(Dimension(0.0)).ConvertToVp(), 0.0);
}

/**
 * @tc.name: SecurityComponentPastePatternTest001
 * @tc.desc: Test default security component modifyDone
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePatternTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    EXPECT_EQ(secCompGesture->GetHitTestMode(), HitTestMode::HTMDEFAULT);

    // modifydone again
    pattern->OnModifyDone();
    EXPECT_TRUE(pattern->isAppearCallback_);
    EXPECT_NE(pattern->clickListener_, nullptr);
}

/**
 * @tc.name: SecurityComponentPastePatternTest002
 * @tc.desc: Test event callback
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPastePatternTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);

    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto secCompGesture = frameNode->GetOrCreateGestureEventHub();
    ASSERT_NE(secCompGesture, nullptr);
    auto clickListener = secCompGesture->clickEventActuator_->clickEvents_.back();
    ASSERT_NE(clickListener, nullptr);
    ASSERT_NE(clickListener->callback_, nullptr);
    GestureEvent info;
    clickListener->callback_(info);

    auto eventHub = frameNode->GetEventHub<EventHub>();
    ASSERT_NE(eventHub, nullptr);
    ASSERT_NE(eventHub->onAppear_, nullptr);
    ASSERT_NE(eventHub->onDisappear_, nullptr);
    eventHub->onAppear_();
    eventHub->onDisappear_();
    ASSERT_EQ(pattern->scId_, -1);
}

/**
 * @tc.name: SecurityComponentPasteLayoutAlgoTest001
 * @tc.desc: Test default layout algo
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPasteLayoutAlgoTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto layoutAlgo = pattern->CreateLayoutAlgorithm();
    ASSERT_NE(layoutAlgo, nullptr);

    auto layoutWrapper = CreateSecurityComponentLayoutWrapper(frameNode);
    ASSERT_NE(layoutWrapper, nullptr);
    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);

    auto property = frameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    property->UpdateBackgroundType(static_cast<int32_t>(ButtonType::CAPSULE));
    property->UpdateTextIconLayoutDirection(SecurityComponentLayoutDirection::VERTICAL);
    property->UpdateLayoutOrder(SecSecurityComponentLayoutOrder::ICON_FIRST);
    pattern->OnModifyDone();

    layoutAlgo->Measure(layoutWrapper.rawPtr_);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Height(), DEFAULT_BUTTON_SIZE);
    EXPECT_EQ(layoutWrapper->geometryNode_->GetFrameSize().Width(), DEFAULT_BUTTON_SIZE);
}

/**
 * @tc.name: SecurityComponentPasteAccessibilityPropertyTest001
 * @tc.desc: Test security component accessibility property
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentPasteAccessibilityPropertyTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(frameNode, nullptr);
    ASSERT_EQ(frameNode->GetTag(), V2::SEC_PASTE_BUTTON_ETS_TAG);
    auto pattern = frameNode->GetPattern<SecurityComponentPattern>();
    ASSERT_NE(pattern, nullptr);
    pattern->OnModifyDone();

    auto accessibility = pattern->CreateAccessibilityProperty();
    ASSERT_NE(accessibility, nullptr);
    accessibility->SetHost(frameNode);
}

/**
 * @tc.name: SecurityComponentGetIconResourceTest001
 * @tc.desc: Test set security component property with invalid icon
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentGetIconResourceTest001, TestSize.Level1)
{
    InternalResource::ResourceId id;
    EXPECT_FALSE(LocationButtonModelNG::GetInstance()->GetIconResource(-1, id));
    EXPECT_FALSE(LocationButtonModelNG::GetInstance()->GetIconResource(ICON_RESOURCE_TABLE + 1, id));
    EXPECT_FALSE(PasteButtonModelNG::GetInstance()->GetIconResource(-1, id));
    EXPECT_FALSE(PasteButtonModelNG::GetInstance()->GetIconResource(ICON_RESOURCE_TABLE + 1, id));
    EXPECT_FALSE(SaveButtonModelNG::GetInstance()->GetIconResource(-1, id));
    EXPECT_FALSE(SaveButtonModelNG::GetInstance()->GetIconResource(ICON_RESOURCE_TABLE + 1, id));
}

#ifdef SECURITY_COMPONENT_ENABLE
/**
 * @tc.name: SecurityComponentHandlerTest001
 * @tc.desc: Test security component handler
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentHandlerTest001, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    RefPtr<FrameNode> invalidFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), "test");
    int32_t scId;
    GestureEvent info;

    ASSERT_EQ(SecurityComponentHandler::RegisterSecurityComponent(invalidFrameNode, scId), -1);
    ASSERT_EQ(SecurityComponentHandler::UpdateSecurityComponent(invalidFrameNode, 0), -1);
    ASSERT_EQ(SecurityComponentHandler::UnregisterSecurityComponent(-1), -1);
    ASSERT_EQ(SecurityComponentHandler::ReportSecurityComponentClickEvent(-1, frameNode, info), -1);
    ASSERT_EQ(SecurityComponentHandler::ReportSecurityComponentClickEvent(0, invalidFrameNode, info), -1);
}

/**
 * @tc.name: SecurityComponentHandlerTest002
 * @tc.desc: Test security component handler
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentHandlerTest002, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_SAVE_BUTTON_ETS_TAG);
    int32_t scId;
    GestureEvent info;

    ASSERT_EQ(SecurityComponentHandler::RegisterSecurityComponent(frameNode, scId), -1);
    ASSERT_EQ(SecurityComponentHandler::UpdateSecurityComponent(frameNode, 0), -1);
    ASSERT_EQ(SecurityComponentHandler::UnregisterSecurityComponent(0),
        OHOS::Security::SecurityComponent::SCErrCode::SC_SERVICE_ERROR_CALLER_INVALID);
    ASSERT_EQ(SecurityComponentHandler::ReportSecurityComponentClickEvent(0, frameNode, info), -1);
}

/**
 * @tc.name: SecurityComponentHandlerTest003
 * @tc.desc: Test security component handler
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentHandlerTest003, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_LOCATION_BUTTON_ETS_TAG);
    int32_t scId;
    GestureEvent info;

    ASSERT_EQ(SecurityComponentHandler::RegisterSecurityComponent(frameNode, scId), -1);
    ASSERT_EQ(SecurityComponentHandler::UpdateSecurityComponent(frameNode, 0), -1);
    ASSERT_EQ(SecurityComponentHandler::UnregisterSecurityComponent(0),
        OHOS::Security::SecurityComponent::SCErrCode::SC_SERVICE_ERROR_CALLER_INVALID);
    ASSERT_EQ(SecurityComponentHandler::ReportSecurityComponentClickEvent(0, frameNode, info), -1);
}

/**
 * @tc.name: SecurityComponentHandlerTest004
 * @tc.desc: Test security component handler
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentHandlerTest004, TestSize.Level1)
{
    RefPtr<FrameNode> frameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    int32_t scId;
    GestureEvent info;

    ASSERT_EQ(SecurityComponentHandler::RegisterSecurityComponent(frameNode, scId), -1);
    ASSERT_EQ(SecurityComponentHandler::UpdateSecurityComponent(frameNode, 0), -1);
    ASSERT_EQ(SecurityComponentHandler::UnregisterSecurityComponent(0),
        OHOS::Security::SecurityComponent::SCErrCode::SC_SERVICE_ERROR_CALLER_INVALID);
    ASSERT_EQ(SecurityComponentHandler::ReportSecurityComponentClickEvent(0, frameNode, info), -1);
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest001
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest001, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    PixStretchEffectOption pixelStretchEffectOption;
    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdatePixelStretchEffect(pixelStretchEffectOption);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest002
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest002, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateLightUpEffect(1);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest003
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest003, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateSphericalEffect(0);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateSphericalEffect(1);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest004
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest004, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateForegroundColor(Color::TRANSPARENT);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateForegroundColor(Color::GRAY);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest005
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest005, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    RefPtr<BasicShape> basicShape;
    renderContext->UpdateClipMask(basicShape);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest006
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest006, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateClipEdge(true);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest007
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest007, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontColorBlend(Color::TRANSPARENT);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontColorBlend(Color::GRAY);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest008
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest008, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontHueRotate(0);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontHueRotate(MAX_ROTATE);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontHueRotate(1);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest009
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest009, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontSepia(0.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontSepia(1.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest010
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest010, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontInvert(0.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontInvert(1.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest011
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest011, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontContrast(1.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontContrast(2.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest012
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest012, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontSaturate(1.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontSaturate(2.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest013
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest013, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontGrayScale(0.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontGrayScale(1.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest014
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest014, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontBlurRadius(0.0_px);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest015
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest015, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto property = parentFrameNode->GetLayoutProperty<SecurityComponentLayoutProperty>();
    ASSERT_NE(property, nullptr);
    property->UpdateVisibility(VisibleType::VISIBLE);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    property->UpdateVisibility(VisibleType::INVISIBLE);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest016
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest016, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateFrontBrightness(1.0_vp);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateFrontBrightness(2.0_vp);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}

/**
 * @tc.name: SecurityComponentCheckParentNodesEffectTest017
 * @tc.desc: Test security component CheckParentNodesEffect
 * @tc.type: FUNC
 * @tc.author:
 */
HWTEST_F(SecurityComponentModelTestNg, SecurityComponentCheckParentNodesEffectTest017, TestSize.Level1)
{
    RefPtr<FrameNode> parentFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(parentFrameNode, nullptr);
    RefPtr<FrameNode> childFrameNode = CreateSecurityComponent(0, 0,
        static_cast<int32_t>(ButtonType::CAPSULE), V2::SEC_PASTE_BUTTON_ETS_TAG);
    ASSERT_NE(childFrameNode, nullptr);
    parentFrameNode->AddChild(childFrameNode);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));

    auto renderContext = parentFrameNode->GetRenderContext();
    ASSERT_NE(renderContext, nullptr);
    renderContext->UpdateOpacity(1);
    ASSERT_FALSE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
    renderContext->UpdateOpacity(2);
    ASSERT_TRUE(SecurityComponentHandler::CheckParentNodesEffect(childFrameNode));
}
#endif
} // namespace OHOS::Ace::NG
