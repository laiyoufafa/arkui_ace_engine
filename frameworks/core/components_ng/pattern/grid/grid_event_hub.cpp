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

#include "core/components_ng/pattern/grid/grid_event_hub.h"

#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/pattern/grid/grid_item_pattern.h"
#include "core/components_ng/pattern/grid/grid_pattern.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace::NG {

void GridEventHub::InitItemDragEvent(const RefPtr<GestureEventHub>& gestureHub)
{
    auto actionStartTask = [weak = WeakClaim(this)](const GestureEvent& info) {
        auto eventHub = weak.Upgrade();
        if (eventHub) {
            eventHub->HandleOnItemDragStart(info);
        }
    };

    auto actionUpdateTask = [weak = WeakClaim(this)](const GestureEvent& info) {
        auto eventHub = weak.Upgrade();
        if (eventHub) {
            eventHub->HandleOnItemDragUpdate(info);
        }
    };

    auto actionEndTask = [weak = WeakClaim(this)](const GestureEvent& info) {
        auto eventHub = weak.Upgrade();
        if (eventHub) {
            eventHub->HandleOnItemDragEnd(info);
        }
    };

    auto actionCancelTask = [weak = WeakClaim(this)]() {
        auto eventHub = weak.Upgrade();
        if (eventHub) {
            eventHub->HandleOnItemDragCancel();
        }
    };

    auto dragEvent = MakeRefPtr<DragEvent>(
        std::move(actionStartTask), std::move(actionUpdateTask), std::move(actionEndTask), std::move(actionCancelTask));
    gestureHub->SetDragEvent(dragEvent, { PanDirection::ALL }, DEFAULT_PAN_FINGER, DEFAULT_PAN_DISTANCE);
}

RefPtr<FrameNode> GridEventHub::FindGridItemByPosition(float x, float y)
{
    auto host = GetFrameNode();
    CHECK_NULL_RETURN(host, nullptr);

    std::map<int32_t, RefPtr<FrameNode>> hitFrameNodes;
    std::list<RefPtr<FrameNode>> children;
    host->GenerateOneDepthVisibleFrame(children);
    for (const auto& child : children) {
        auto geometryNode = child->GetGeometryNode();
        if (!geometryNode) {
            continue;
        }

        auto globalFrameRect = geometryNode->GetFrameRect();
        globalFrameRect.SetOffset(child->GetOffsetRelativeToWindow());

        if (globalFrameRect.IsInRegion(PointF(x, y))) {
            hitFrameNodes.insert(std::make_pair(child->GetDepth(), child));
        }
    }

    if (hitFrameNodes.empty()) {
        return nullptr;
    }

    return hitFrameNodes.rbegin()->second;
}

int32_t GridEventHub::GetGridItemIndex(const RefPtr<FrameNode>& frameNode)
{
    CHECK_NULL_RETURN(frameNode, 0);
    auto gridFrameNode = GetFrameNode();
    CHECK_NULL_RETURN(gridFrameNode, 0);
    auto gridPattern = gridFrameNode->GetPattern<GridPattern>();
    CHECK_NULL_RETURN(gridPattern, 0);
    RefPtr<GridItemPattern> itemPattern = frameNode->GetPattern<GridItemPattern>();
    CHECK_NULL_RETURN(itemPattern, 0);

    auto gridLayoutInfo = gridPattern->GetGridLayoutInfo();
    auto mainIndex = itemPattern->GetMainIndex();
    auto crossIndex = itemPattern->GetCrossIndex();
    auto crossIndexIterator = gridLayoutInfo.gridMatrix_.find(mainIndex);
    if (crossIndexIterator != gridLayoutInfo.gridMatrix_.end()) {
        auto crossIndexMap = crossIndexIterator->second;

        auto indexIterator = crossIndexMap.find(crossIndex);
        if (indexIterator != crossIndexMap.end()) {
            return indexIterator->second;
        }
    }

    return 0;
}

bool GridEventHub::GetEditable() const
{
    auto host = GetFrameNode();
    CHECK_NULL_RETURN(host, false);
    auto layoutProperty = host->GetLayoutProperty<GridLayoutProperty>();
    CHECK_NULL_RETURN(layoutProperty, false);
    return layoutProperty->GetEditable().value_or(false);
}

void GridEventHub::HandleOnItemDragStart(const GestureEvent& info)
{
    if (!GetEditable()) {
        return;
    }

    auto pipeline = PipelineContext::GetCurrentContext();
    CHECK_NULL_VOID(pipeline);

    auto globalX = static_cast<float>(info.GetGlobalPoint().GetX());
    auto globalY = static_cast<float>(info.GetGlobalPoint().GetY());

    auto itemFrameNode = FindGridItemByPosition(globalX, globalY);
    CHECK_NULL_VOID(itemFrameNode);

    draggedIndex_ = GetGridItemIndex(itemFrameNode);
    OHOS::Ace::ItemDragInfo itemDragInfo;
    itemDragInfo.SetX(pipeline->ConvertPxToVp(Dimension(globalX, DimensionUnit::PX)));
    itemDragInfo.SetY(pipeline->ConvertPxToVp(Dimension(globalY, DimensionUnit::PX)));
    auto customNode = FireOnItemDragStart(itemDragInfo, draggedIndex_);
    CHECK_NULL_VOID(customNode);
    auto manager = pipeline->GetDragDropManager();
    CHECK_NULL_VOID(manager);

    dragDropProxy_ = manager->CreateAndShowDragWindow(customNode, info);
    CHECK_NULL_VOID(dragDropProxy_);
    dragDropProxy_->OnItemDragStart(info, GetFrameNode());
}

void GridEventHub::HandleOnItemDragUpdate(const GestureEvent& info)
{
    if (!GetEditable()) {
        return;
    }

    CHECK_NULL_VOID(dragDropProxy_);
    dragDropProxy_->OnItemDragMove(info, draggedIndex_);
}

void GridEventHub::HandleOnItemDragEnd(const GestureEvent& info)
{
    if (!GetEditable()) {
        return;
    }

    CHECK_NULL_VOID(dragDropProxy_);
    dragDropProxy_->OnItemDragEnd(info, draggedIndex_);
    dragDropProxy_->DestroyDragWindow();
    dragDropProxy_ = nullptr;
    draggedIndex_ = 0;
}

void GridEventHub::HandleOnItemDragCancel()
{
    if (!GetEditable()) {
        return;
    }

    CHECK_NULL_VOID(dragDropProxy_);
    dragDropProxy_->onItemDragCancel();
    dragDropProxy_->DestroyDragWindow();
    dragDropProxy_ = nullptr;
    draggedIndex_ = 0;
}

} // namespace OHOS::Ace::NG