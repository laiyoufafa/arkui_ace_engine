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

#include "core/components_ng/pattern/window_scene/scene/container/window_pattern.h"

#include "include/vsync_station.h"
#include "render_service_client/core/ui/rs_ui_director.h"
#include "session/container/include/session_stage.h"

#include "base/utils/time_util.h"
#include "core/common/container.h"
#include "core/common/container_scope.h"
#include "core/common/thread_checker.h"
#include "core/components_ng/render/adapter/rosen_render_context.h"

namespace {
constexpr int32_t IDLE_TASK_DELAY_MILLISECOND = 51;
constexpr float ONE_SECOND_IN_NANO = 1000000000.0f;

float GetDisplayRefreshRate()
{
#ifdef PREVIEW
    return 30.0f;
#else
    return 60.0f;
#endif
}
} // namespace

namespace OHOS::Ace::NG {

class SizeChangeListener : public Rosen::ISizeChangeListener {
public:
    explicit SizeChangeListener(int32_t instanceId) : instanceId_(instanceId) {}
    virtual ~SizeChangeListener() = default;

    void OnSizeChange(const Rosen::WSRect& rect, Rosen::SizeChangeReason reason) override
    {
        ContainerScope scope(instanceId_);
        auto container = Container::Current();
        CHECK_NULL_VOID(container);
        auto window = static_cast<WindowPattern*>(container->GetWindow());
        CHECK_NULL_VOID(window);
        window->SetWindowRect(Rect(rect.posX_, rect.posY_, rect.width_, rect.height_));

        ViewportConfig config;
        config.SetPosition(rect.posX_, rect.posY_);
        config.SetSize(rect.width_, rect.height_);
        // TODO: get display density
        config.SetDensity(1.5);
        LOGI("OnSizeChange window rect: [%{public}d, %{public}d, %{public}u, %{public}u]", rect.posX_, rect.posY_,
            rect.width_, rect.height_);
        window->UpdateViewportConfig(config, SESSION_TO_WINDOW_MAP.at(reason));
    }

private:
    const std::map<Rosen::SizeChangeReason, Rosen::WindowSizeChangeReason> SESSION_TO_WINDOW_MAP {
        { Rosen::SizeChangeReason::SHOW, Rosen::WindowSizeChangeReason::UNDEFINED },
        { Rosen::SizeChangeReason::HIDE, Rosen::WindowSizeChangeReason::HIDE },
        { Rosen::SizeChangeReason::MAXIMIZE, Rosen::WindowSizeChangeReason::MAXIMIZE },
        { Rosen::SizeChangeReason::RECOVER, Rosen::WindowSizeChangeReason::RECOVER },
        { Rosen::SizeChangeReason::ROTATION, Rosen::WindowSizeChangeReason::ROTATION },
    };
    int32_t instanceId_ = -1;
};

class InputEventListener : public Rosen::IInputEventListener {
public:
    explicit InputEventListener(int32_t instanceId) : instanceId_(instanceId) {}
    virtual ~InputEventListener() = default;

    void OnPointerEvent(const std::shared_ptr<OHOS::MMI::PointerEvent>& pointerEvent) override
    {
        ContainerScope scope(instanceId_);
        auto container = Container::Current();
        CHECK_NULL_VOID(container);
        auto window = static_cast<WindowPattern*>(container->GetWindow());
        CHECK_NULL_VOID(window);
        window->ProcessPointerEvent(pointerEvent);
    }

    void OnKeyEvent(const std::shared_ptr<OHOS::MMI::KeyEvent>& keyEvent) override
    {
        ContainerScope scope(instanceId_);
        auto container = Container::Current();
        CHECK_NULL_VOID(container);
        auto window = static_cast<WindowPattern*>(container->GetWindow());
        CHECK_NULL_VOID(window);
        window->ProcessKeyEvent(keyEvent);
    }

    void OnAxisEvent(const std::shared_ptr<OHOS::MMI::AxisEvent>& axisEvent) override
    {
        ContainerScope scope(instanceId_);
        auto container = Container::Current();
        CHECK_NULL_VOID(container);
        auto window = static_cast<WindowPattern*>(container->GetWindow());
        CHECK_NULL_VOID(window);
        window->ProcessAxisEvent(axisEvent);
    }

private:
    int32_t instanceId_ = -1;
};

WindowPattern::WindowPattern(
    const std::shared_ptr<AbilityRuntime::Context>& context, const std::shared_ptr<Rosen::RSSurfaceNode>& surfaceNode)
    : surfaceNode_(surfaceNode), context_(context)
{}

void WindowPattern::Init()
{
    int64_t refreshPeriod = static_cast<int64_t>(ONE_SECOND_IN_NANO / GetDisplayRefreshRate());
    vsyncCallback_ = std::make_shared<Rosen::VsyncCallback>();
    vsyncCallback_->onCallback = [weakTask = taskExecutor_, instanceId = instanceId_, refreshPeriod](
                                     int64_t nanoTimestamp) {
        auto onVsync = [instanceId, nanoTimestamp, refreshPeriod] {
            ContainerScope scope(instanceId);
            // use container to get window can make sure the window is valid
            auto container = Container::Current();
            CHECK_NULL_VOID(container);
            auto window = container->GetWindow();
            CHECK_NULL_VOID(window);
            window->OnVsync(static_cast<uint64_t>(nanoTimestamp), 0);
            auto pipeline = container->GetPipelineContext();
            if (pipeline) {
                pipeline->OnIdle(nanoTimestamp + refreshPeriod);
            }
        };
        auto taskExecutor = weakTask.Upgrade();
        CHECK_NULL_VOID(taskExecutor);
        auto uiTaskRunner = SingleTaskExecutor::Make(taskExecutor, TaskExecutor::TaskType::UI);
        if (uiTaskRunner.IsRunOnCurrentThread()) {
            onVsync();
            return;
        }
        uiTaskRunner.PostTask([callback = std::move(onVsync)]() { callback(); });
    };
    rsUIDirector_ = Rosen::RSUIDirector::Create();
    rsUIDirector_->SetRSSurfaceNode(GetSurfaceNode());
    rsUIDirector_->SetCacheDir(AceApplicationInfo::GetInstance().GetDataFileDirPath());
    rsUIDirector_->Init();
    rsUIDirector_->SetUITaskRunner(
        [taskExecutor = taskExecutor_.Upgrade(), instanceId = instanceId_](const std::function<void()>& task) {
            ContainerScope scope(instanceId);
            if (taskExecutor) {
                taskExecutor->PostTask(task, TaskExecutor::TaskType::UI);
            }
        });
}

void WindowPattern::Destroy()
{
    vsyncCallback_.reset();
    rsUIDirector_->Destroy();
    rsUIDirector_.reset();
    callbacks_.clear();
}

void WindowPattern::LoadContent(
    const std::string& contentUrl, NativeEngine* engine, NativeValue* storage, AbilityRuntime::Context* context)
{
    uiContent_ = UIContent::Create(context_.get(), engine);
    CHECK_NULL_VOID(uiContent_);
    uiContent_->Initialize(std::shared_ptr<Window>(this), contentUrl, storage);

    auto inputListener = std::make_shared<InputEventListener>(instanceId_);
    RegisterInputEventListener(inputListener);
}

void WindowPattern::UpdateViewportConfig(const ViewportConfig& config, Rosen::WindowSizeChangeReason reason)
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->UpdateViewportConfig(config, reason);
}

void WindowPattern::SetRootFrameNode(const RefPtr<NG::FrameNode>& root)
{
    LOGI("WindowPattern set root frame node");
    CHECK_NULL_VOID(root);
    auto rosenRenderContext = AceType::DynamicCast<RosenRenderContext>(root->GetRenderContext());
    CHECK_NULL_VOID(rosenRenderContext);
    if (rosenRenderContext->GetRSNode()) {
        rsUIDirector_->SetRoot(rosenRenderContext->GetRSNode()->GetId());
    }
}

void WindowPattern::RequestFrame()
{
    CHECK_NULL_VOID_NOLOG(onShow_);
    CHECK_RUN_ON(UI);
    CHECK_NULL_VOID_NOLOG(!isRequestVsync_);

    LOGD("request next vsync");
    RequestVsync(vsyncCallback_);
    lastRequestVsyncTime_ = GetSysTimestamp();
    isRequestVsync_ = true;

    auto taskExecutor = taskExecutor_.Upgrade();
    CHECK_NULL_VOID(taskExecutor);
    taskExecutor->PostDelayedTask(
        [id = instanceId_]() {
            ContainerScope scope(id);
            auto container = Container::Current();
            CHECK_NULL_VOID(container);
            auto pipeline = container->GetPipelineContext();
            CHECK_NULL_VOID(pipeline);
            pipeline->OnIdle(0);
        },
        TaskExecutor::TaskType::UI, IDLE_TASK_DELAY_MILLISECOND);
}

void WindowPattern::RecordFrameTime(uint64_t timeStamp, const std::string& name)
{
    LOGD("WindowPattern RecordFrameTime");
    rsUIDirector_->SetTimeStamp(timeStamp, name);
}

bool WindowPattern::FlushCustomAnimation(uint64_t timeStamp)
{
    return rsUIDirector_->RunningCustomAnimation(timeStamp);
}

void WindowPattern::FlushTasks()
{
    CHECK_RUN_ON(UI);
    LOGD("WindowPattern flush tasks");
    rsUIDirector_->SendMessages();
}

void WindowPattern::RequestVsync(const std::shared_ptr<Rosen::VsyncCallback>& vsyncCallback)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    Rosen::VsyncStation::GetInstance().RequestVsync(vsyncCallback);
}

void WindowPattern::OnShow()
{
    onShow_ = true;
    rsUIDirector_->GoForeground();
    rsUIDirector_->SendMessages();
}

void WindowPattern::OnHide()
{
    onShow_ = false;
    rsUIDirector_->GoBackground();
    rsUIDirector_->SendMessages();
}

void WindowPattern::SetDrawTextAsBitmap(bool useBitmap)
{
    Rosen::RSSystemProperties::SetDrawTextAsBitmap(useBitmap);
}

float WindowPattern::GetRefreshRate() const
{
    return GetDisplayRefreshRate();
}

void WindowPattern::RegisterSessionStageStateListener(
    const std::shared_ptr<Rosen::ISessionStageStateListener>& listener)
{
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->RegisterSessionStageStateListener(listener);
}

void WindowPattern::RegisterSizeChangeListener(const std::shared_ptr<Rosen::ISizeChangeListener>& listener)
{
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->RegisterSizeChangeListener(listener);
}

void WindowPattern::RegisterInputEventListener(const std::shared_ptr<Rosen::IInputEventListener>& listener)
{
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->RegisterInputEventListener(listener);
}

void WindowPattern::ProcessPointerEvent(const std::shared_ptr<OHOS::MMI::PointerEvent>& pointerEvent)
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->ProcessPointerEvent(pointerEvent);
}

void WindowPattern::ProcessKeyEvent(const std::shared_ptr<OHOS::MMI::KeyEvent>& keyEvent)
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->ProcessKeyEvent(keyEvent);
}

void WindowPattern::ProcessAxisEvent(const std::shared_ptr<OHOS::MMI::AxisEvent>& axisEvent)
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->ProcessAxisEvent(axisEvent);
}

void WindowPattern::Connect()
{
    auto sizeChangeListener = std::make_shared<SizeChangeListener>(instanceId_);
    RegisterSizeChangeListener(sizeChangeListener);
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->Connect();
}

void WindowPattern::Foreground()
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->Foreground();
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->Foreground();
}

void WindowPattern::Background()
{
    CHECK_NULL_VOID(uiContent_);
    uiContent_->Background();
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->Background();
}

void WindowPattern::Disconnect()
{
    CHECK_NULL_VOID(sessionStage_);
    sessionStage_->Disconnect();
}

} // namespace OHOS::Ace::NG
