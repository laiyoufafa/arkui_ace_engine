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

#include "core/components_ng/manager/drag_drop/drag_drop_manager.h"

#include "core/components_ng/pattern/grid/grid_event_hub.h"
#include "core/components_ng/pattern/root/root_pattern.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace::NG {
RefPtr<DragDropProxy> DragDropManager::CreateAndShowDragWindow(
    const RefPtr<PixelMap>& pixelMap, const GestureEvent& info)
{
#if !defined(PREVIEW)
    if (dragWindow_) {
        LOGW("CreateAndShowDragWindow: There is a drag window, create drag window failed.");
        return nullptr;
    }

    CreateDragWindow(info, pixelMap->GetWidth(), pixelMap->GetHeight());
    dragWindow_->DrawPixelMap(pixelMap);
#endif
    auto proxy = MakeRefPtr<DragDropProxy>();
    return proxy;
}

RefPtr<DragDropProxy> DragDropManager::CreateAndShowDragWindow(
    const RefPtr<FrameNode>& frameNode, const GestureEvent& info)
{
#if !defined(PREVIEW)
    if (dragWindow_) {
        LOGW("CreateAndShowDragWindow: There is a drag window, create drag window failed.");
        return nullptr;
    }

    auto geometryNode = frameNode->GetGeometryNode();
    CHECK_NULL_RETURN(geometryNode, nullptr);

    auto frameRect = geometryNode->GetFrameSize();
    CreateDragWindow(info, static_cast<uint32_t>(frameRect.Width()), static_cast<uint32_t>(frameRect.Height()));
    dragWindow_->DrawFrameNode(frameNode);
#endif

    auto proxy = MakeRefPtr<DragDropProxy>();
    return proxy;
}

void DragDropManager::CreateDragWindow(const GestureEvent& info, uint32_t width, uint32_t height)
{
#if !defined(PREVIEW)
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    pipeline->SetIsDragged(true);

    auto rect = pipeline->GetCurrentWindowRect();
    dragWindow_ = DragWindow::CreateDragWindow("APP_DRAG_WINDOW",
        static_cast<int32_t>(info.GetGlobalPoint().GetX()) + rect.Left(),
        static_cast<int32_t>(info.GetGlobalPoint().GetY()) + rect.Top(), width, height);
    dragWindow_->SetOffset(rect.Left(), rect.Top());
#endif
}

RefPtr<FrameNode> DragDropManager::CreateDragRootNode(const RefPtr<UINode>& customNode)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_RETURN(pipeline, nullptr);

    auto rootNode = FrameNode::CreateFrameNodeWithTree(
        V2::ROOT_ETS_TAG, ElementRegister::GetInstance()->MakeUniqueId(), MakeRefPtr<RootPattern>());
    rootNode->SetHostRootId(pipeline->GetInstanceId());
    rootNode->SetHostPageId(-1);
    rootNode->AddChild(customNode);
    rootNode->AttachToMainTree();
    rootNode->MarkDirtyNode(PROPERTY_UPDATE_MEASURE);
    pipeline->FlushPipelineImmediately();

    return rootNode;
}

void DragDropManager::UpdateDragWindowPosition(int32_t globalX, int32_t globalY)
{
#if !defined(PREVIEW)
    CHECK_NULL_VOID(dragWindow_);
    dragWindow_->MoveTo(globalX, globalY);
#endif
}

RefPtr<FrameNode> DragDropManager::FindDragFrameNodeByPosition(float globalX, float globalY, DragType dragType)
{
    std::set<WeakPtr<FrameNode>> frameNodes;
    switch (dragType) {
        case DragType::COMMON:
            frameNodes = dragFrameNodes_;
            break;
        case DragType::GRID:
            frameNodes = gridDragFrameNodes_;
            break;
        default:
            break;
    }

    if (frameNodes.empty()) {
        return nullptr;
    }

    std::map<int32_t, RefPtr<FrameNode>> hitFrameNodes;
    for (const auto& weakNode : frameNodes) {
        auto frameNode = weakNode.Upgrade();
        if (!frameNode) {
            continue;
        }

        auto geometryNode = frameNode->GetGeometryNode();
        if (!geometryNode) {
            continue;
        }

        auto globalFrameRect = geometryNode->GetFrameRect();
        globalFrameRect.SetOffset(frameNode->GetOffsetRelativeToWindow());

        if (globalFrameRect.IsInRegion(PointF(globalX, globalY))) {
            hitFrameNodes.insert(std::make_pair(frameNode->GetDepth(), frameNode));
        }
    }

    if (hitFrameNodes.empty()) {
        return nullptr;
    }
    return hitFrameNodes.rbegin()->second;
}

void DragDropManager::OnDragStart(float globalX, float globalY)
{
    preTargetFrameNode_ = FindDragFrameNodeByPosition(globalX, globalY, DragType::COMMON);
}

void DragDropManager::OnDragMove(float globalX, float globalY, const std::string& extraInfo)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    UpdateDragWindowPosition(static_cast<int32_t>(globalX), static_cast<int32_t>(globalY));

    RefPtr<OHOS::Ace::DragEvent> event = AceType::MakeRefPtr<OHOS::Ace::DragEvent>();
    event->SetX(pipeline->ConvertPxToVp(Dimension(globalX, DimensionUnit::PX)));
    event->SetY(pipeline->ConvertPxToVp(Dimension(globalY, DimensionUnit::PX)));

    auto dragFrameNode = FindDragFrameNodeByPosition(globalX, globalY, DragType::COMMON);
    if (!dragFrameNode) {
        if (preTargetFrameNode_) {
            FireOnDragEvent(
                preTargetFrameNode_, event, DragEventType::LEAVE, extraInfo);
            preTargetFrameNode_ = nullptr;
        }

        return;
    }

    if (dragFrameNode == preTargetFrameNode_) {
        FireOnDragEvent(dragFrameNode, event, DragEventType::MOVE, extraInfo);
        return;
    }

    if (preTargetFrameNode_) {
        FireOnDragEvent(preTargetFrameNode_, event, DragEventType::LEAVE, extraInfo);
    }

    FireOnDragEvent(dragFrameNode, event, DragEventType::ENTER, extraInfo);
    preTargetFrameNode_ = dragFrameNode;
}

void DragDropManager::OnDragEnd(float globalX, float globalY, const std::string& extraInfo)
{
    preTargetFrameNode_ = nullptr;

    auto dragFrameNode = FindDragFrameNodeByPosition(globalX, globalY, DragType::COMMON);
    if (!dragFrameNode) {
        return;
    }

    auto eventHub = dragFrameNode->GetEventHub<EventHub>();
    CHECK_NULL_VOID(eventHub);

    if (!eventHub->HasOnDrop()) {
        return;
    }

    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    RefPtr<OHOS::Ace::DragEvent> event = AceType::MakeRefPtr<OHOS::Ace::DragEvent>();
    event->SetX(pipeline->ConvertPxToVp(Dimension(globalX, DimensionUnit::PX)));
    event->SetY(pipeline->ConvertPxToVp(Dimension(globalY, DimensionUnit::PX)));

    eventHub->FireOnDrop(event, extraInfo);
}

void DragDropManager::onDragCancel()
{
    preTargetFrameNode_ = nullptr;
}

void DragDropManager::FireOnDragEvent(const RefPtr<FrameNode>& frameNode, const RefPtr<OHOS::Ace::DragEvent>& event,
    DragEventType type, const std::string& extraInfo)
{
    auto eventHub = frameNode->GetEventHub<EventHub>();
    CHECK_NULL_VOID(eventHub);

    switch (type) {
        case DragEventType::ENTER:
            eventHub->FireOnDragEnter(event, extraInfo);
            break;
        case DragEventType::MOVE:
            eventHub->FireOnDragMove(event, extraInfo);
            break;
        case DragEventType::LEAVE:
            eventHub->FireOnDragLeave(event, extraInfo);
            break;
        case DragEventType::DROP:
            eventHub->FireOnDrop(event, extraInfo);
            break;
        default:
            break;
    }
}

void DragDropManager::OnItemDragStart(float globalX, float globalY)
{
    preGridTargetFrameNode_ = FindDragFrameNodeByPosition(globalX, globalY, DragType::GRID);
}

void DragDropManager::OnItemDragMove(float globalX, float globalY, int32_t draggedIndex)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    UpdateDragWindowPosition(static_cast<int32_t>(globalX), static_cast<int32_t>(globalY));

    OHOS::Ace::ItemDragInfo itemDragInfo;
    itemDragInfo.SetX(pipeline->ConvertPxToVp(Dimension(globalX, DimensionUnit::PX)));
    itemDragInfo.SetY(pipeline->ConvertPxToVp(Dimension(globalY, DimensionUnit::PX)));

    auto dragFrameNode = FindDragFrameNodeByPosition(globalX, globalY, DragType::GRID);
    if (!dragFrameNode) {
        if (preGridTargetFrameNode_) {
            FireOnItemDragEvent(preGridTargetFrameNode_, itemDragInfo, DragEventType::LEAVE, draggedIndex);
            preGridTargetFrameNode_ = nullptr;
        }

        return;
    }

    if (dragFrameNode == preGridTargetFrameNode_) {
        auto eventHub = dragFrameNode->GetEventHub<GridEventHub>();
        CHECK_NULL_VOID(eventHub);

        auto itemFrameNode = eventHub->FindGridItemByPosition(globalX, globalY);
        int32_t insertIndex = eventHub->GetGridItemIndex(itemFrameNode);
        FireOnItemDragEvent(dragFrameNode, itemDragInfo, DragEventType::MOVE, draggedIndex, insertIndex);
        return;
    }

    if (preGridTargetFrameNode_) {
        FireOnItemDragEvent(preGridTargetFrameNode_, itemDragInfo, DragEventType::LEAVE, draggedIndex);
    }

    FireOnItemDragEvent(dragFrameNode, itemDragInfo, DragEventType::ENTER, draggedIndex);
    preGridTargetFrameNode_ = dragFrameNode;
}

void DragDropManager::OnItemDragEnd(float globalX, float globalY, int32_t draggedIndex)
{
    preGridTargetFrameNode_ = nullptr;

    auto dragFrameNode = FindDragFrameNodeByPosition(globalX, globalY, DragType::GRID);
    if (!dragFrameNode) {
        return;
    }

    auto eventHub = dragFrameNode->GetEventHub<GridEventHub>();
    CHECK_NULL_VOID(eventHub);

    if (!eventHub->HasOnItemDrop()) {
        return;
    }

    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    OHOS::Ace::ItemDragInfo itemDragInfo;
    itemDragInfo.SetX(pipeline->ConvertPxToVp(Dimension(globalX, DimensionUnit::PX)));
    itemDragInfo.SetY(pipeline->ConvertPxToVp(Dimension(globalY, DimensionUnit::PX)));

    auto itemFrameNode = eventHub->FindGridItemByPosition(globalX, globalY);
    int32_t insertIndex = eventHub->GetGridItemIndex(itemFrameNode);
    eventHub->FireOnItemDrop(itemDragInfo, draggedIndex, insertIndex, true);
}

void DragDropManager::onItemDragCancel()
{
    preGridTargetFrameNode_ = nullptr;
}

void DragDropManager::FireOnItemDragEvent(const RefPtr<FrameNode>& frameNode,
    const OHOS::Ace::ItemDragInfo& itemDragInfo, DragEventType type, int32_t draggedIndex, int32_t insertIndex)
{
    auto eventHub = frameNode->GetEventHub<GridEventHub>();
    CHECK_NULL_VOID(eventHub);

    switch (type) {
        case DragEventType::ENTER:
            eventHub->FireOnItemDragEnter(itemDragInfo);
            break;
        case DragEventType::MOVE:
            eventHub->FireOnItemDragMove(itemDragInfo, draggedIndex, insertIndex);
            break;
        case DragEventType::LEAVE:
            eventHub->FireOnItemDragLeave(itemDragInfo, draggedIndex);
            break;
        case DragEventType::DROP:
            eventHub->FireOnItemDrop(itemDragInfo, draggedIndex, insertIndex, true);
            break;
        default:
            break;
    }
}

void DragDropManager::AddDataToClipboard(const std::string& extraInfo)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    auto newData = JsonUtil::Create(true);
    newData->Put("customDragInfo", extraInfo.c_str());

    if (!clipboard_) {
        clipboard_ = ClipboardProxy::GetInstance()->GetClipboard(pipeline->GetTaskExecutor());
    }
    if (!clipboardCallback_) {
        auto callback = [weakManager = WeakClaim(this), addData = newData->ToString()](const std::string& data) {
            auto dragDropManager = weakManager.Upgrade();
            if (dragDropManager) {
                auto clipboardAllData = JsonUtil::Create(true);
                clipboardAllData->Put("preData", data.c_str());
                clipboardAllData->Put("newData", addData.c_str());
                dragDropManager->clipboard_->SetData(clipboardAllData->ToString(), CopyOptions::Local, true);
            }
        };
        clipboardCallback_ = callback;
    }
    clipboard_->GetData(clipboardCallback_);
}

void DragDropManager::GetExtraInfoFromClipboard(std::string& extraInfo)
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    if (!clipboard_) {
        clipboard_ = ClipboardProxy::GetInstance()->GetClipboard(pipeline->GetTaskExecutor());
    }

    std::string* extraInfoPtr = &extraInfo;
    if (!clipboardCallback_) {
        auto callback = [extraInfoPtr](const std::string& data) {
            auto json = JsonUtil::ParseJsonString(data);
            auto newData = JsonUtil::ParseJsonString(json->GetString("newData"));
            *extraInfoPtr = newData->GetString("customDragInfo");
        };
        clipboardCallback_ = callback;
    }

    if (clipboardCallback_) {
        clipboard_->GetData(clipboardCallback_, true);
    }
}

void DragDropManager::RestoreClipboardData()
{
    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    if (!clipboard_) {
        clipboard_ = ClipboardProxy::GetInstance()->GetClipboard(pipeline->GetTaskExecutor());
    }

    if (!deleteDataCallback_) {
        auto callback = [weakManager = WeakClaim(this)](const std::string& data) {
            auto dragDropManager = weakManager.Upgrade();
            if (dragDropManager) {
                auto json = JsonUtil::ParseJsonString(data);
                dragDropManager->clipboard_->SetData(json->GetString("preData"));
            }
        };
        deleteDataCallback_ = callback;
    }
    clipboard_->GetData(deleteDataCallback_);
}

void DragDropManager::DestroyDragWindow()
{
#if !defined(PREVIEW)
    CHECK_NULL_VOID(dragWindow_);
    dragWindow_->Destroy();
    dragWindow_ = nullptr;
#endif
}

} // namespace OHOS::Ace::NG