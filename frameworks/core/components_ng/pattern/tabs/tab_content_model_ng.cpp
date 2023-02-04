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

#include "core/components_ng/pattern/tabs/tab_content_model_ng.h"

#include "base/memory/ace_type.h"
#include "base/memory/referenced.h"
#include "base/utils/utils.h"
#include "core/components/tab_bar/tab_theme.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/image/image_layout_property.h"
#include "core/components_ng/pattern/image/image_pattern.h"
#include "core/components_ng/pattern/linear_layout/linear_layout_pattern.h"
#include "core/components_ng/pattern/swiper/swiper_pattern.h"
#include "core/components_ng/pattern/tabs/tab_bar_pattern.h"
#include "core/components_ng/pattern/tabs/tab_content_node.h"
#include "core/components_ng/pattern/tabs/tab_content_pattern.h"
#include "core/components_ng/pattern/tabs/tabs_node.h"
#include "core/components_ng/pattern/text/text_pattern.h"
#include "core/components_v2/inspector/inspector_constants.h"

namespace OHOS::Ace::NG {
namespace {

// TODO use theme
constexpr char DEFAULT_TAB_BAR_NAME[] = "TabBar";
constexpr Dimension DEFAULT_SINGLE_TEXT_FONT_SIZE = 16.0_fp;
constexpr Dimension DEFAULT_SMALL_TEXT_FONT_SIZE = 10.0_fp;
constexpr Dimension DEFAULT_IMAGE_SIZE = 24.0_vp;
constexpr Dimension TAB_BAR_SPACE = 2.0_vp;
constexpr Dimension TAB_BAR_ITEM_SMALL_PADDING(8, DimensionUnit::VP);
constexpr Dimension TAB_BAR_ITEM_BIG_PADDING(12, DimensionUnit::VP);

} // namespace

void TabContentModelNG::Create(std::function<void()>&& deepRenderFunc)
{
    auto* stack = ViewStackProcessor::GetInstance();
    auto nodeId = stack->ClaimNodeId();
    auto deepRender = [nodeId, deepRenderFunc = std::move(deepRenderFunc)]() -> RefPtr<UINode> {
        CHECK_NULL_RETURN(deepRenderFunc, nullptr);
        ScopedViewStackProcessor scopedViewStackProcessor;
        deepRenderFunc();
        auto deepChild = ViewStackProcessor::GetInstance()->Finish();
        auto parent = FrameNode::GetFrameNode(V2::TAB_CONTENT_ITEM_ETS_TAG, nodeId);
        if (deepChild && parent) {
            deepChild->MountToParent(parent);
        }
        return deepChild;
    };
    auto frameNode = TabContentNode::GetOrCreateTabContentNode(V2::TAB_CONTENT_ITEM_ETS_TAG, nodeId,
        [shallowBuilder = AceType::MakeRefPtr<ShallowBuilder>(std::move(deepRender))]() {
            return AceType::MakeRefPtr<TabContentPattern>(shallowBuilder);
        });
    stack->Push(frameNode);
    SetTabBar(DEFAULT_TAB_BAR_NAME, "", nullptr, true); // Set default tab bar.
    ACE_UPDATE_LAYOUT_PROPERTY(TabContentLayoutProperty, Text, DEFAULT_TAB_BAR_NAME);
}

void TabContentModelNG::Create()
{
    auto* stack = ViewStackProcessor::GetInstance();
    int32_t nodeId = stack->ClaimNodeId();
    auto frameNode = TabContentNode::GetOrCreateTabContentNode(
        V2::TAB_CONTENT_ITEM_ETS_TAG, nodeId, []() { return AceType::MakeRefPtr<TabContentPattern>(nullptr); });
    stack->Push(frameNode);
    SetTabBar(DEFAULT_TAB_BAR_NAME, "", nullptr, true); // Set default tab bar.
    ACE_UPDATE_LAYOUT_PROPERTY(TabContentLayoutProperty, Text, DEFAULT_TAB_BAR_NAME);
}

void TabContentModelNG::Pop()
{
    auto tabContent = NG::ViewStackProcessor::GetInstance()->GetMainFrameNode();
    AddTabBarItem(tabContent, DEFAULT_NODE_SLOT, true);
    NG::ViewStackProcessor::GetInstance()->PopContainer();
}

RefPtr<TabsNode> TabContentModelNG::FindTabsNode(const RefPtr<UINode>& tabContent)
{
    CHECK_NULL_RETURN(tabContent, nullptr);
    RefPtr<UINode> parent = tabContent->GetParent();

    while (parent) {
        if (AceType::InstanceOf<TabsNode>(parent)) {
            return AceType::DynamicCast<TabsNode>(parent);
        }
        parent = parent->GetParent();
    }
    return nullptr;
}

void TabContentModelNG::AddTabBarItem(const RefPtr<UINode>& tabContent, int32_t position, bool update)
{
    LOGD("position %{public}d", position);
    CHECK_NULL_VOID(tabContent);
    auto tabContentId = tabContent->GetId();

    auto tabContentNode = AceType::DynamicCast<TabContentNode>(tabContent);
    CHECK_NULL_VOID(tabContentNode);

    if (update && !tabContentNode->HasTabBarItemId()) {
        LOGD("Update only, return");
        return;
    }

    auto tabsNode = FindTabsNode(tabContent);
    CHECK_NULL_VOID(tabsNode);

    auto tabBarNode = tabsNode->GetTabBar();
    CHECK_NULL_VOID(tabBarNode);
    auto& tabBarParam = tabContentNode->GetPattern<TabContentPattern>()->GetTabBarParam();

    // Create column node to contain image and text or builder.
    auto columnNode = FrameNode::GetOrCreateFrameNode(V2::COLUMN_ETS_TAG, tabContentNode->GetTabBarItemId(),
        []() { return AceType::MakeRefPtr<LinearLayoutPattern>(true); });

    auto linearLayoutProperty = columnNode->GetLayoutProperty<LinearLayoutProperty>();
    CHECK_NULL_VOID(linearLayoutProperty);
    linearLayoutProperty->UpdateMainAxisAlign(FlexAlign::CENTER);
    linearLayoutProperty->UpdateCrossAxisAlign(FlexAlign::CENTER);
    linearLayoutProperty->UpdateSpace(TAB_BAR_SPACE);

    auto tabBarFrameNode = AceType::DynamicCast<FrameNode>(tabBarNode);
    CHECK_NULL_VOID(tabBarFrameNode);
    auto tabBarPattern = tabBarFrameNode->GetPattern<TabBarPattern>();
    CHECK_NULL_VOID(tabBarPattern);

    // Create tab bar with builder.
    if (tabBarParam.HasBuilder()) {
        ScopedViewStackProcessor builderViewStackProcessor;
        tabBarParam.ExecuteBuilder();
        auto builderNode = ViewStackProcessor::GetInstance()->Finish();
        if (static_cast<int32_t>(columnNode->GetChildren().size()) != 0) {
            columnNode->Clean();
        }
        builderNode->MountToParent(columnNode);
        tabBarNode->ReplaceChild(tabsNode->GetBuilderByContentId(tabContentId, columnNode), columnNode);
        tabBarPattern->AddTabBarItemType(tabContentId, true);
        return;
    }

    if (tabBarParam.GetText().empty()) {
        LOGW("Text is empty.");
        return;
    }

    LOGD("Text %{public}s", tabBarParam.GetText().c_str());

    // Create text node and image node.
    RefPtr<FrameNode> textNode;
    RefPtr<FrameNode> imageNode;
    auto layoutProperty = columnNode->GetLayoutProperty();
    CHECK_NULL_VOID(layoutProperty);
    auto deviceType = SystemProperties::GetDeviceType();
    auto tabBarItemPadding = deviceType == DeviceType::PHONE ? TAB_BAR_ITEM_SMALL_PADDING : TAB_BAR_ITEM_BIG_PADDING;
    layoutProperty->UpdatePadding({ CalcLength(tabBarItemPadding), CalcLength(tabBarItemPadding),
        CalcLength(tabBarItemPadding), CalcLength(tabBarItemPadding) });
    if (static_cast<int32_t>(columnNode->GetChildren().size()) == 0) {
        ImageSourceInfo imageSourceInfo(tabBarParam.GetIcon());
        imageNode = FrameNode::GetOrCreateFrameNode(V2::IMAGE_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(),
            []() { return AceType::MakeRefPtr<ImagePattern>(); });
        textNode = FrameNode::GetOrCreateFrameNode(V2::TEXT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(),
            []() { return AceType::MakeRefPtr<TextPattern>(); });
        CHECK_NULL_VOID(textNode);
        CHECK_NULL_VOID(imageNode);
        columnNode->MountToParent(tabBarNode, position);
        imageNode->MountToParent(columnNode);
        textNode->MountToParent(columnNode);
    } else {
        imageNode = AceType::DynamicCast<FrameNode>(columnNode->GetChildren().front());
        textNode = AceType::DynamicCast<FrameNode>(columnNode->GetChildren().back());
    }
    CHECK_NULL_VOID(textNode);
    CHECK_NULL_VOID(imageNode);

    auto swiperNode = AceType::DynamicCast<FrameNode>(tabContentNode->GetParent());
    CHECK_NULL_VOID(swiperNode);
    auto swiperPattern = swiperNode->GetPattern<SwiperPattern>();
    CHECK_NULL_VOID(swiperPattern);
    int32_t indicator = swiperPattern->GetCurrentIndex();
    int32_t totalCount = swiperPattern->TotalCount();
    if (indicator > totalCount - 1 || indicator < 0) {
        indicator = 0;
    }

    // Update property of text.
    auto pipelineContext = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipelineContext);
    auto tabTheme = pipelineContext->GetTheme<TabTheme>();
    CHECK_NULL_VOID(tabTheme);
    auto textLayoutProperty = textNode->GetLayoutProperty<TextLayoutProperty>();
    CHECK_NULL_VOID(textLayoutProperty);
    if ((static_cast<int32_t>(tabBarNode->GetChildren().size()) - 1) == indicator) {
        textLayoutProperty->UpdateTextColor(tabTheme->GetActiveIndicatorColor());
    } else {
        textLayoutProperty->UpdateTextColor(tabTheme->GetSubTabTextOffColor());
    }
    textLayoutProperty->UpdateContent(tabBarParam.GetText());
    textLayoutProperty->UpdateFontSize(DEFAULT_SINGLE_TEXT_FONT_SIZE);
    textLayoutProperty->UpdateTextAlign(TextAlign::CENTER);
    textLayoutProperty->UpdateMaxLines(1);
    textLayoutProperty->UpdateTextOverflow(TextOverflow::ELLIPSIS);

    // Update property of image.
    auto imageProperty = imageNode->GetLayoutProperty<ImageLayoutProperty>();
    CHECK_NULL_VOID(imageProperty);
    if (!tabBarParam.GetIcon().empty()) {
        textLayoutProperty->UpdateFontSize(DEFAULT_SMALL_TEXT_FONT_SIZE);
        imageProperty->UpdateUserDefinedIdealSize(
            CalcSize(NG::CalcLength(DEFAULT_IMAGE_SIZE), NG::CalcLength(DEFAULT_IMAGE_SIZE)));
    } else {
        imageProperty->UpdateUserDefinedIdealSize(CalcSize());
    }
    ImageSourceInfo imageSourceInfo(tabBarParam.GetIcon());
    imageProperty->UpdateImageSourceInfo(imageSourceInfo);

    columnNode->MarkModifyDone();
    textNode->MarkModifyDone();
    imageNode->MarkModifyDone();
    tabBarPattern->AddTabBarItemType(tabContentId, false);
}

void TabContentModelNG::RemoveTabBarItem(const RefPtr<TabContentNode>& tabContentNode)
{
    CHECK_NULL_VOID(tabContentNode);
    if (!tabContentNode->HasTabBarItemId()) {
        return;
    }

    auto tabBarItemId = tabContentNode->GetTabBarItemId();
    LOGD("Tab ID: %{public}d, Bar item ID: %{public}d", tabContentNode->GetId(), tabBarItemId);
    auto tabBarItemNode = ElementRegister::GetInstance()->GetUINodeById(tabBarItemId);
    CHECK_NULL_VOID(tabBarItemNode);
    auto tabBarNode = tabBarItemNode->GetParent();
    tabBarNode->RemoveChild(tabBarItemNode);
    CHECK_NULL_VOID(tabBarNode);
    tabContentNode->ResetTabBarItemId();
}

void TabContentModelNG::SetTabBar(const std::optional<std::string>& text, const std::optional<std::string>& icon,
    TabBarBuilderFunc&& builder, bool /*useContentOnly*/)
{
    ACE_UPDATE_LAYOUT_PROPERTY(TabContentLayoutProperty, Icon, icon.value_or(""));
    ACE_UPDATE_LAYOUT_PROPERTY(TabContentLayoutProperty, Text, text.value_or(""));
    auto frameNodePattern = ViewStackProcessor::GetInstance()->GetMainFrameNodePattern<TabContentPattern>();
    CHECK_NULL_VOID(frameNodePattern);
    frameNodePattern->SetTabBar(text.value_or(""), icon.value_or(""), std::move(builder));
}

} // namespace OHOS::Ace::NG
