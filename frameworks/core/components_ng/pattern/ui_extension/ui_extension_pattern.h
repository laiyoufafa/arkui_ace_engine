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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_UI_EXTENSION_UI_EXTENSION_PATTERN_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_UI_EXTENSION_UI_EXTENSION_PATTERN_H

#include "base/memory/referenced.h"
#include "base/want/want_wrap.h"
#include "core/components_ng/event/gesture_event_hub.h"
#include "core/components_ng/pattern/window_scene/scene/window_pattern.h"
#include "core/event/mouse_event.h"
#include "core/event/touch_event.h"

namespace OHOS::AAFwk {
class Want;
class WantParams;
}

namespace OHOS::Ace::NG {
class UIExtensionProxy;
class UIExtensionPattern : public WindowPattern {
    DECLARE_ACE_TYPE(UIExtensionPattern, WindowPattern);

public:
    explicit UIExtensionPattern(const RefPtr<OHOS::Ace::WantWrap>& wantWrap);
    ~UIExtensionPattern() override;

    void OnWindowShow() override;
    void OnWindowHide() override;

    bool HasStartingPage() override
    {
        return false;
    }

    void SetOnRemoteReadyCallback(std::function<void(const RefPtr<UIExtensionProxy>&)>&& callback);
    void SetOnReleaseCallback(std::function<void(int32_t)>&& callback);
    void SetOnResultCallback(std::function<void(int32_t, const AAFwk::Want&)>&& callback);
    void SetOnReceiveCallback(std::function<void(const AAFwk::WantParams&)>&& callback);
    void SetOnErrorCallback(
        std::function<void(int32_t code, const std::string& name, const std::string& message)>&& callback);

    void OnConnect() override;
    void OnDisconnect() override;

    void RequestExtensionSessionActivation();
    void RequestExtensionSessionBackground();
    void RequestExtensionSessionDestruction();

    RefPtr<LayoutAlgorithm> CreateLayoutAlgorithm() override;

    FocusPattern GetFocusPattern() const override;

private:
    enum ReleaseCode {
        DESTROY_NORMAL = 0,
        CONNECT_BROKEN,
    };

    struct ErrorMsg {
        int32_t code = 0;
        std::string name;
        std::string message;
    };

    void OnModifyDone() override;
    void OnDetachFromFrameNode(FrameNode* frameNode) override;

    void InitOnKeyEvent(const RefPtr<FocusHub>& focusHub);
    bool OnKeyEvent(const KeyEvent& event);
    void HandleFocusEvent();
    void HandleBlurEvent();
    bool KeyEventConsumed(const KeyEvent& event);

    void InitTouchEvent(const RefPtr<GestureEventHub>& gestureHub);
    void InitMouseEvent(const RefPtr<InputEventHub>& inputHub);
    void HandleTouchEvent(const TouchEventInfo& info);
    void HandleMouseEvent(const MouseInfo& info);
    void UnregisterAbilityResultListener();
    void OnConnectInner();

    void OnRemoteReady();
    void OnResult(int32_t code, const AAFwk::Want& want);
    void OnReceive(const AAFwk::WantParams& wantParams);

    RefPtr<TouchEventImpl> touchEvent_;
    RefPtr<InputEvent> mouseEvent_;

    std::function<void(const RefPtr<UIExtensionProxy>&)> onRemoteReadyCallback_;
    std::function<void(int32_t)> onReleaseCallback_;
    std::function<void(int32_t, const AAFwk::Want&)> onResultCallback_;
    std::function<void(const AAFwk::WantParams&)> onReceiveCallback_;
    std::function<void(int32_t code, const std::string& name, const std::string& message)> onErrorCallback_;

    ErrorMsg lastError_;
    ACE_DISALLOW_COPY_AND_MOVE(UIExtensionPattern);
};
} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_UI_EXTENSION_UI_EXTENSION_PATTERN_H
