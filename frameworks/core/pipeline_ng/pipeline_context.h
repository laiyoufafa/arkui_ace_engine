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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_PIPELINE_NG_CONTEXT_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_PIPELINE_NG_CONTEXT_H

#include <cstdint>
#include <functional>
#include <list>
#include <utility>

#include "base/memory/referenced.h"
#include "core/common/frontend.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/manager/drag_drop/drag_drop_manager.h"
#include "core/components_ng/manager/full_screen/full_screen_manager.h"
#include "core/components_ng/manager/select_overlay/select_overlay_manager.h"
#include "core/components_ng/manager/shared_overlay/shared_overlay_manager.h"
#include "core/components_ng/pattern/custom/custom_node.h"
#include "core/components_ng/pattern/overlay/overlay_manager.h"
#include "core/components_ng/pattern/stage/stage_manager.h"
#include "core/event/touch_event.h"
#include "core/pipeline/pipeline_base.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT PipelineContext final : public PipelineBase {
    DECLARE_ACE_TYPE(NG::PipelineContext, PipelineBase);

public:
    using PredictTask = std::function<void(int64_t)>;
    PipelineContext(std::unique_ptr<Window> window, RefPtr<TaskExecutor> taskExecutor,
        RefPtr<AssetManager> assetManager, RefPtr<PlatformResRegister> platformResRegister,
        const RefPtr<Frontend>& frontend, int32_t instanceId);
    PipelineContext(std::unique_ptr<Window> window, RefPtr<TaskExecutor> taskExecutor,
        RefPtr<AssetManager> assetManager, const RefPtr<Frontend>& frontend, int32_t instanceId);
    PipelineContext() = default;

    ~PipelineContext() override = default;

    static RefPtr<PipelineContext> GetCurrentContext();

    static float GetCurrentRootWidth();

    static float GetCurrentRootHeight();

    void SetupRootElement() override;

    void SetupSubRootElement();

    const RefPtr<FrameNode>& GetRootElement() const
    {
        return rootNode_;
    }

    void AddKeyFrame(float fraction, const RefPtr<Curve>& curve, const std::function<void()>& propertyCallback) override
    {}

    void AddKeyFrame(float fraction, const std::function<void()>& propertyCallback) override {}

    // add schedule task and return the unique mark id.
    uint32_t AddScheduleTask(const RefPtr<ScheduleTask>& task) override;

    // remove schedule task by id.
    void RemoveScheduleTask(uint32_t id) override {}

    // Called by view when touch event received.
    void OnTouchEvent(const TouchEvent& point, bool isSubPipe = false) override;

    // Called by container when key event received.
    // if return false, then this event needs platform to handle it.
    bool OnKeyEvent(const KeyEvent& event) override;

    // Called by view when mouse event received.
    void OnMouseEvent(const MouseEvent& event) override;

    // Called by view when axis event received.
    void OnAxisEvent(const AxisEvent& event) override;

    // Called by container when rotation event received.
    // if return false, then this event needs platform to handle it.
    bool OnRotationEvent(const RotationEvent& event) const override
    {
        return false;
    }

    void OnDragEvent(int32_t x, int32_t y, DragEventAction action) override;

    // Called by view when idle event.
    void OnIdle(int64_t deadline) override;

    void SetBuildAfterCallback(const std::function<void()>& callback) override {}

    void SaveExplicitAnimationOption(const AnimationOption& option) override {}

    void CreateExplicitAnimator(const std::function<void()>& onFinishEvent) override {}

    void ClearExplicitAnimationOption() override {}

    AnimationOption GetExplicitAnimationOption() const override
    {
        return {};
    }

    void AddVisibleAreaChangeNode(const RefPtr<FrameNode>& node, double ratio, const VisibleRatioCallback& callback);

    void HandleVisibleAreaChangeEvent();

    void Destroy() override;

    void OnShow() override;

    void OnHide() override;

    void WindowFocus(bool isFocus) override;

    void ShowContainerTitle(bool isShow) override;

    void SetAppBgColor(const Color& color) override;

    void SetAppTitle(const std::string& title) override;

    void SetAppIcon(const RefPtr<PixelMap>& icon) override;

    void OnSurfaceChanged(
        int32_t width, int32_t height, WindowSizeChangeReason type = WindowSizeChangeReason::UNDEFINED) override;

    void OnSurfacePositionChanged(int32_t posX, int32_t posY) override {}

    void OnSurfaceDensityChanged(double density) override;

    void OnSystemBarHeightChanged(double statusBar, double navigationBar) override {}

    void OnSurfaceDestroyed() override {}

    void NotifyOnPreDraw() override {}

    bool CallRouterBackToPopPage() override
    {
        return OnBackPressed();
    }

    bool OnBackPressed();

    void AddDirtyCustomNode(const RefPtr<UINode>& dirtyNode);

    void AddDirtyLayoutNode(const RefPtr<FrameNode>& dirty);

    void AddDirtyRenderNode(const RefPtr<FrameNode>& dirty);

    void AddPredictTask(PredictTask&& task);

    void FlushDirtyNodeUpdate();

    void SetRootRect(double width, double height, double offset) override;

    const RefPtr<FullScreenManager>& GetFullScreenManager();

    const RefPtr<StageManager>& GetStageManager();

    const RefPtr<OverlayManager>& GetOverlayManager();

    const RefPtr<SelectOverlayManager>& GetSelectOverlayManager();

    const RefPtr<SharedOverlayManager>& GetSharedOverlayManager()
    {
        return sharedTransitionManager_;
    }

    const RefPtr<DragDropManager>& GetDragDropManager();

    void FlushBuild() override;

    void FlushPipelineImmediately();

    void AddBuildFinishCallBack(std::function<void()>&& callback);

    void AddWindowStateChangedCallback(int32_t nodeId);

    void RemoveWindowStateChangedCallback(int32_t nodeId);

    void AddWindowFocusChangedCallback(int32_t nodeId);

    void RemoveWindowFocusChangedCallback(int32_t nodeId);

    bool GetIsFocusingByTab() const
    {
        return isFocusingByTab_;
    }

    void SetIsFocusingByTab(bool isFocusingByTab)
    {
        isFocusingByTab_ = isFocusingByTab;
    }

    bool GetIsNeedShowFocus() const
    {
        return isNeedShowFocus_;
    }

    void SetIsNeedShowFocus(bool isNeedShowFocus)
    {
        isNeedShowFocus_ = isNeedShowFocus;
    }

    bool GetOnShow() const
    {
        return onShow_;
    }

    bool RequestDefaultFocus();
    bool RequestFocus(const std::string& targetNodeId) override;
    void AddDirtyFocus(const RefPtr<FrameNode>& node);
    void RootLostFocus(BlurReason reason = BlurReason::FOCUS_SWITCH) const;

    void AddNodesToNotifyMemoryLevel(int32_t nodeId);
    void RemoveNodesToNotifyMemoryLevel(int32_t nodeId);
    void NotifyMemoryLevel(int32_t level) override;
    void FlushMessages() override;

    void FlushUITasks() override
    {
        taskScheduler_.FlushTask();
    }
    // end pipeline, exit app
    void Finish(bool autoFinish) const override;

protected:
    void StartWindowSizeChangeAnimate(int32_t width, int32_t height, WindowSizeChangeReason type);

    void FlushVsync(uint64_t nanoTimestamp, uint32_t frameCount) override;
    void FlushPipelineWithoutAnimation() override;
    void FlushFocus();
    void FlushAnimation(uint64_t nanoTimestamp) override;
    bool OnDumpInfo(const std::vector<std::string>& params) const override;

    void OnVirtualKeyboardHeightChange(float keyboardHeight) override;

private:
    void FlushWindowStateChangedCallback(bool isShow);

    void FlushWindowFocusChangedCallback(bool isFocus);

    void FlushTouchEvents();

    void FlushBuildFinishCallbacks();

    template<typename T>
    struct NodeCompare {
        bool operator()(const T& nodeLeft, const T& nodeRight) const
        {
            if (!nodeLeft || !nodeRight) {
                return false;
            }
            if (nodeLeft->GetDepth() < nodeRight->GetDepth()) {
                return true;
            }
            if (nodeLeft->GetDepth() == nodeRight->GetDepth()) {
                return nodeLeft < nodeRight;
            }
            return false;
        }
    };

    UITaskScheduler taskScheduler_;

    std::unordered_map<uint32_t, WeakPtr<ScheduleTask>> scheduleTasks_;
    std::set<RefPtr<UINode>, NodeCompare<RefPtr<UINode>>> dirtyNodes_;
    std::list<std::function<void()>> buildFinishCallbacks_;

    // window on show or on hide
    std::list<int32_t> onWindowStateChangedCallbacks_;
    // window on focused or on unfocused
    std::list<int32_t> onWindowFocusChangedCallbacks_;

    std::list<int32_t> nodesToNotifyMemoryLevel_;

    std::list<TouchEvent> touchEvents_;

    RefPtr<FrameNode> rootNode_;

    std::unordered_map<int32_t, std::list<VisibleCallbackInfo>> visibleAreaChangeNodes_;

    RefPtr<StageManager> stageManager_;
    RefPtr<OverlayManager> overlayManager_;
    RefPtr<FullScreenManager> fullScreenManager_;
    RefPtr<SelectOverlayManager> selectOverlayManager_;
    RefPtr<DragDropManager> dragDropManager_;
    RefPtr<SharedOverlayManager> sharedTransitionManager_;
    WeakPtr<FrameNode> dirtyFocusNode_;
    WeakPtr<FrameNode> dirtyFocusScope_;
    uint32_t nextScheduleTaskId_ = 0;
    bool hasIdleTasks_ = false;
    bool isFocusingByTab_ = false;
    bool isNeedShowFocus_ = false;
    bool onShow_ = true;
    bool onFocus_ = true;

    ACE_DISALLOW_COPY_AND_MOVE(PipelineContext);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMMON_PIPELINE_NG_CONTEXT_H
