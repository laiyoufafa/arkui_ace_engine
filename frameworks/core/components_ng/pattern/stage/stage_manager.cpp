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

#include "core/components_ng/pattern/stage/stage_manager.h"

#include "base/geometry/ng/size_t.h"
#include "base/memory/referenced.h"
#include "base/utils/utils.h"
#include "core/animation/page_transition_common.h"
#include "core/common/container.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/base/ui_node.h"
#include "core/components_ng/manager/shared_overlay/shared_overlay_manager.h"
#include "core/components_ng/pattern/overlay/overlay_manager.h"
#include "core/components_ng/pattern/stage/page_pattern.h"
#include "core/components_ng/pattern/stage/stage_pattern.h"
#include "core/components_ng/property/property.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/pipeline_ng/pipeline_context.h"
#include "core/pipeline_ng/ui_task_scheduler.h"

namespace OHOS::Ace::NG {

namespace {
void FirePageTransition(const RefPtr<FrameNode>& page, PageTransitionType transitionType)
{
    CHECK_NULL_VOID(page);
    auto pagePattern = page->GetPattern<PagePattern>();
    CHECK_NULL_VOID(pagePattern);
    if (transitionType == PageTransitionType::EXIT_PUSH || transitionType == PageTransitionType::EXIT_POP) {
        pagePattern->TriggerPageTransition(transitionType, [page, instanceId = Container::CurrentId()]() {
            ContainerScope scope(instanceId);
            LOGI("pageTransition exit finish");
            CHECK_NULL_VOID(page);
            auto pattern = page->GetPattern<PagePattern>();
            CHECK_NULL_VOID(pattern);
            pattern->OnHide();
        });
        return;
    }
    pagePattern->TriggerPageTransition(transitionType, nullptr);
}

void StartTransition(const RefPtr<FrameNode>& srcPage, const RefPtr<FrameNode>& destPage, RouteType type)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);
    auto sharedManager = pipeline->GetSharedOverlayManager();
    CHECK_NULL_VOID(sharedManager);
    sharedManager->StartSharedTransition(srcPage, destPage);
    if (type == RouteType::PUSH) {
        FirePageTransition(srcPage, PageTransitionType::EXIT_PUSH);
        FirePageTransition(destPage, PageTransitionType::ENTER_PUSH);
    } else if (type == RouteType::POP) {
        FirePageTransition(srcPage, PageTransitionType::EXIT_POP);
        FirePageTransition(destPage, PageTransitionType::ENTER_POP);
    }
}
} // namespace
StageManager::StageManager(const RefPtr<FrameNode>& stage) : stageNode_(stage)
{
    stagePattern_ = DynamicCast<StagePattern>(stageNode_->GetPattern());
}

bool StageManager::PushPage(const RefPtr<FrameNode>& node, bool needHideLast, bool needTransition)
{
    CHECK_NULL_RETURN(stageNode_, false);
    CHECK_NULL_RETURN(node, false);

    const auto& children = stageNode_->GetChildren();
    RefPtr<FrameNode> outPageNode;
    needTransition &= !children.empty();
    if (!children.empty() && needHideLast) {
        FirePageHide(children.back(), needTransition ? PageTransitionType::EXIT_PUSH : PageTransitionType::NONE);
        outPageNode = AceType::DynamicCast<FrameNode>(children.back());
    }
    auto rect = stageNode_->GetGeometryNode()->GetFrameRect();
    rect.SetOffset({});
    node->GetRenderContext()->SyncGeometryProperties(rect);
    // mount to parent and mark build render tree.
    node->MountToParent(stageNode_);
    // then build the total child.
    node->Build();
    stageNode_->RebuildRenderContextTree();
    FirePageShow(node, needTransition ? PageTransitionType::ENTER_PUSH : PageTransitionType::NONE);

    auto pagePattern = node->GetPattern<PagePattern>();
    CHECK_NULL_RETURN(pagePattern, false);
    stagePattern_->currentPageIndex_ = pagePattern->GetPageInfo()->GetPageId();
    if (needTransition) {
        pagePattern->SetFirstBuildCallback([outPageNode, weakIn = WeakPtr<FrameNode>(node)]() {
            auto inPageNode = weakIn.Upgrade();
            // outPageNode need to perform the onHide function so we keep its RefPtr
            StartTransition(outPageNode, inPageNode, RouteType::PUSH);
        });
    }

    // flush layout task.
    if (!stageNode_->GetGeometryNode()->GetMarginFrameSize().IsPositive()) {
        // in first load case, wait for window size.
        LOGI("waiting for window size");
        return true;
    }
    stageNode_->MarkDirtyNode(PROPERTY_UPDATE_MEASURE);
    return true;
}

bool StageManager::PopPage(bool needShowNext, bool needTransition)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_RETURN(pipeline, false);
    CHECK_NULL_RETURN(stageNode_, false);
    const auto& children = stageNode_->GetChildren();
    if (children.empty()) {
        LOGE("fail to pop page due to children is null");
        return false;
    }
    auto pageNode = children.back();
    const size_t transitionPageSize = 2;
    needTransition &= (children.size() >= transitionPageSize);
    FirePageHide(pageNode, needTransition ? PageTransitionType::EXIT_POP : PageTransitionType::NONE);

    RefPtr<FrameNode> inPageNode;
    if (needShowNext && children.size() >= transitionPageSize) {
        auto newPageNode = *(++children.rbegin());
        FirePageShow(newPageNode, needTransition ? PageTransitionType::ENTER_POP : PageTransitionType::NONE);
        inPageNode = AceType::DynamicCast<FrameNode>(newPageNode);
    }

    auto outPageNode = AceType::DynamicCast<FrameNode>(pageNode);
    if (needTransition) {
        StartTransition(outPageNode, inPageNode, RouteType::POP);
    }
    stageNode_->RemoveChild(pageNode);
    stageNode_->RebuildRenderContextTree();
    pipeline->RequestFrame();
    return true;
}

bool StageManager::PopPageToIndex(int32_t index, bool needShowNext, bool needTransition)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_RETURN(pipeline, false);
    CHECK_NULL_RETURN(stageNode_, false);
    const auto& children = stageNode_->GetChildren();
    if (children.empty()) {
        LOGE("fail to pop page due to children is null");
        return false;
    }
    int32_t popSize = static_cast<int32_t>(children.size()) - index - 1;
    if (popSize < 0) {
        LOGE("fail to pop page due to index is out of range");
        return false;
    }
    if (popSize == 0) {
        LOGD("already here");
        return true;
    }

    bool firstPageTransition = true;
    auto outPageNode = AceType::DynamicCast<FrameNode>(children.back());
    auto iter = children.rbegin();
    for (int32_t current = 0; current < popSize; ++current) {
        auto pageNode = *iter;
        FirePageHide(
            pageNode, firstPageTransition && needTransition ? PageTransitionType::EXIT_POP : PageTransitionType::NONE);
        firstPageTransition = false;
        ++iter;
    }

    RefPtr<FrameNode> inPageNode;
    if (needShowNext) {
        const auto& newPageNode = *iter;
        FirePageShow(newPageNode, needTransition ? PageTransitionType::ENTER_POP : PageTransitionType::NONE);
        inPageNode = AceType::DynamicCast<FrameNode>(newPageNode);
    }

    if (needTransition) {
        StartTransition(outPageNode, inPageNode, RouteType::POP);
    }
    for (int32_t current = 0; current < popSize; ++current) {
        auto pageNode = children.back();
        stageNode_->RemoveChild(pageNode);
    }
    stageNode_->RebuildRenderContextTree();
    pipeline->RequestFrame();
    return true;
}

bool StageManager::CleanPageStack()
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_RETURN(pipeline, false);
    CHECK_NULL_RETURN(stageNode_, false);
    const auto& children = stageNode_->GetChildren();
    if (children.size() <= 1) {
        LOGE("fail to clean page stack due to children size is illegal");
        return false;
    }
    auto popSize = static_cast<int32_t>(children.size() - 1);
    for (int32_t count = 1; count <= popSize; ++count) {
        auto pageNode = children.front();
        stageNode_->RemoveChild(pageNode);
    }
    stageNode_->RebuildRenderContextTree();
    pipeline->RequestFrame();
    return true;
}

bool StageManager::MovePageToFront(const RefPtr<FrameNode>& node, bool needHideLast, bool needTransition)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_RETURN(pipeline, false);
    CHECK_NULL_RETURN(stageNode_, false);
    const auto& children = stageNode_->GetChildren();
    if (children.empty()) {
        LOGE("child is empty");
        return false;
    }
    const auto& lastPage = children.back();
    if (lastPage == node) {
        LOGD("page already on the top");
        return true;
    }
    if (needHideLast) {
        FirePageHide(lastPage, needTransition ? PageTransitionType::EXIT_PUSH : PageTransitionType::NONE);
    }
    node->MovePosition(static_cast<int32_t>(stageNode_->GetChildren().size() - 1));
    FirePageShow(node, needTransition ? PageTransitionType::ENTER_PUSH : PageTransitionType::NONE);

    stageNode_->RebuildRenderContextTree();
    if (needTransition) {
        auto outPageNode = AceType::DynamicCast<FrameNode>(lastPage);
        StartTransition(outPageNode, node, RouteType::PUSH);
    }
    pipeline->RequestFrame();
    return true;
}

void StageManager::FirePageHide(const RefPtr<UINode>& node, PageTransitionType transitionType)
{
    auto pageNode = DynamicCast<FrameNode>(node);
    CHECK_NULL_VOID(pageNode);
    auto pagePattern = pageNode->GetPattern<PagePattern>();
    CHECK_NULL_VOID(pagePattern);
    if (transitionType == PageTransitionType::NONE) {
        pagePattern->OnHide();
    }

    auto pageFocusHub = pageNode->GetFocusHub();
    CHECK_NULL_VOID(pageFocusHub);
    pageFocusHub->SetParentFocusable(false);

    auto context = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID_NOLOG(context);
    context->SetIsNeedShowFocus(false);
}

void StageManager::FirePageShow(const RefPtr<UINode>& node, PageTransitionType /* transitionType */)
{
    auto pageNode = DynamicCast<FrameNode>(node);
    CHECK_NULL_VOID(pageNode);
    auto pagePattern = pageNode->GetPattern<PagePattern>();
    CHECK_NULL_VOID(pagePattern);
    pageNode->GetLayoutProperty()->UpdateVisibility(VisibleType::VISIBLE);
    pagePattern->OnShow();

    auto pageFocusHub = pageNode->GetFocusHub();
    CHECK_NULL_VOID(pageFocusHub);
    pageFocusHub->SetParentFocusable(true);
    pageFocusHub->RequestFocus();

    auto context = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID_NOLOG(context);
    context->SetIsNeedShowFocus(false);
}

RefPtr<FrameNode> StageManager::GetLastPage()
{
    CHECK_NULL_RETURN(stageNode_, nullptr);
    const auto& children = stageNode_->GetChildren();
    if (children.empty()) {
        LOGE("fail to return page due to children is null");
        return nullptr;
    }
    return DynamicCast<FrameNode>(children.back());
}

void StageManager::ReloadStage()
{
    const auto& children = stageNode_->GetChildren();
    for (const auto& child : children) {
        auto frameNode = DynamicCast<FrameNode>(child);
        if (!frameNode) {
            continue;
        }
        auto pagePattern = frameNode->GetPattern<PagePattern>();
        if (!pagePattern) {
            continue;
        }
        pagePattern->ReloadPage();
    }
}

} // namespace OHOS::Ace::NG
