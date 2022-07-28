/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_EVENT_MANAGER_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_EVENT_MANAGER_H

#include <unordered_map>

#include "base/memory/ace_type.h"
#include "base/memory/referenced.h"
#include "core/event/axis_event.h"
#include "core/event/key_event.h"
#include "core/event/mouse_event.h"
#include "core/event/rotation_event.h"
#include "core/event/touch_event.h"
#include "core/focus/focus_node.h"

namespace OHOS::Ace {

class RenderNode;
class Element;
class TextOverlayManager;
using MouseHoverTestList = std::list<WeakPtr<RenderNode>>;

class EventManager : public virtual AceType {
public:
    // After the touch down event is triggered, the touch test is performed to collect the corresponding
    // touch event target list.
    void TouchTest(const TouchEvent& touchPoint, const RefPtr<RenderNode>& renderNode,
        const TouchRestrict& touchRestrict, const Offset& offset = Offset(),
            float viewScale = 1.0f, bool needAppend = false);
    void TouchTest(const AxisEvent& event, const RefPtr<RenderNode>& renderNode, const TouchRestrict& touchRestrict);

    bool DispatchTouchEvent(const TouchEvent& point);
    bool DispatchTouchEvent(const AxisEvent& event);

    // Distribute the key event to the corresponding root node. If the root node is not processed, return false and the
    // platform will handle it.
    bool DispatchKeyEvent(const KeyEvent& event, const RefPtr<FocusNode>& focusNode);

    // Distribute the rotation event to the corresponding render tree or requested render node. If the render is not
    // processed, return false and the platform will handle it.
    static bool DispatchRotationEvent(
        const RotationEvent& event, const RefPtr<RenderNode>& renderNode, const RefPtr<RenderNode>& requestFocusNode);

    // mouse event target list.
    void MouseTest(const MouseEvent& touchPoint, const RefPtr<RenderNode>& renderNode);

    bool DispatchMouseEvent(const MouseEvent& event);
    bool DispatchMouseHoverEvent(const MouseEvent& event);

    void AxisTest(const AxisEvent& event, const RefPtr<RenderNode>& renderNode);
    bool DispatchAxisEvent(const AxisEvent& event);

    void ClearResults();
    void SetInstanceId(int32_t instanceId)
    {
        instanceId_ = instanceId;
    }
    int32_t GetInstanceId()
    {
        return instanceId_;
    }
    void HandleGlobalEvent(const TouchEvent& touchPoint, const RefPtr<TextOverlayManager>& textOverlayManager);

    std::list<std::pair<int32_t, WeakPtr<FocusNode>>>& GetTabIndexNodes()
    {
        return tabIndexNodes_;
    }

    void SetTabIndexNodes(std::list<std::pair<int32_t, WeakPtr<FocusNode>>>& tabIndexNodes)
    {
        tabIndexNodes_ = std::move(tabIndexNodes);
    }

    void SetIsTabNodesCollected(bool isTabNodesCollected)
    {
        isTabNodesCollected_ = isTabNodesCollected;
    }

    void CollectTabIndexNodes(const RefPtr<FocusNode>& rootNode);

private:
    std::unordered_map<size_t, TouchTestResult> touchTestResults_;
    std::unordered_map<size_t, MouseTestResult> mouseTestResults_;
    TouchTestResult axisTouchTestResult_;
    MouseHoverTestList mouseHoverTestResults_;
    MouseHoverTestList mouseHoverTestResultsPre_;
    WeakPtr<RenderNode> mouseHoverNodePre_;
    WeakPtr<RenderNode> mouseHoverNode_;
    WeakPtr<RenderNode> axisNode_;
    bool isTabNodesCollected_ = false;
    bool isLastInTabNodes_ = false;
    int32_t tabPressedIndex_ = -1;
    std::list<std::pair<int32_t, WeakPtr<FocusNode>>> tabIndexNodes_;
    WeakPtr<FocusNode> firstZeroNode_;
    int32_t instanceId_ = 0;
    bool inSelectedRect_ = false;
};

} // namespace OHOS::Ace

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_EVENT_MANAGER_H
