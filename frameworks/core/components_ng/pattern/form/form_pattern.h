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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FORM_FORM_PATTERN_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FORM_FORM_PATTERN_H

#include "core/components/form/resource/form_request_data.h"
#include "core/components_ng/event/event_hub.h"
#include "core/components_ng/pattern/form/form_event_hub.h"
#include "core/components_ng/pattern/form/form_layout_property.h"
#include "core/components_ng/pattern/pattern.h"

namespace OHOS {
namespace MMI {
class PointerEvent;
} // namespace MMI

namespace Ace {
class SubContainer;
class FormManagerDelegate;

namespace NG {

class FormPattern : public Pattern {
    DECLARE_ACE_TYPE(FormPattern, Pattern);

public:
    FormPattern();
    ~FormPattern() override;

    void OnActionEvent(const std::string& action) const;

    void OnLoadEvent();

    RefPtr<LayoutProperty> CreateLayoutProperty() override
    {
        return MakeRefPtr<FormLayoutProperty>();
    }

    RefPtr<EventHub> CreateEventHub() override
    {
        return MakeRefPtr<FormEventHub>();
    }

    std::unique_ptr<DrawDelegate> GetDrawDelegate();

    const RefPtr<SubContainer>& GetSubContainer() const;

    void DispatchPointerEvent(
        const std::shared_ptr<MMI::PointerEvent>& pointerEvent) const;

    RefPtr<RenderContext> GetExternalRenderContext()
    {
        return externalRenderContext_;
    }

    bool GetIsLoaded() const
    {
        return isLoaded_;
    }
private:
    void OnAttachToFrameNode() override;
    bool OnDirtyLayoutWrapperSwap(FrameNode* frameNode, const DirtySwapConfig& config) override;
    void OnRebuildFrame() override;

    void InitFormManagerDelegate();
    void CreateCardContainer();
    void RemoveSubContainer();

    void FireOnAcquiredEvent(int64_t id) const;
    void FireOnRouterEvent(const std::unique_ptr<JsonValue>& action) const;
    void FireOnLoadEvent() const;
    void FireOnErrorEvent(const std::string& code, const std::string& msg) const;
    void FireOnUninstallEvent(int64_t id) const;

    bool ISAllowUpdate() const;
    void EnableDrag();
    // used by ArkTS Card, for RSSurfaceNode from FRS,
    RefPtr<RenderContext> externalRenderContext_;

    RefPtr<SubContainer> subContainer_;
    RefPtr<FormManagerDelegate> formManagerBridge_;

    RequestFormInfo cardInfo_;
    bool isLoaded_ = false;
};

} // namespace NG
} // namespace Ace
} // namespace OHOS

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_FORM_FORM_PATTERN_H
