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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_STAGE_PAGE_PATTERN_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_STAGE_PAGE_PATTERN_H

#include <functional>

#include "base/memory/referenced.h"
#include "base/utils/noncopyable.h"
#include "core/animation/page_transition_common.h"
#include "core/components_ng/pattern/pattern.h"
#include "core/components_ng/pattern/stage/page_event_hub.h"
#include "core/components_ng/pattern/stage/page_info.h"
#include "core/components_ng/pattern/stage/page_transition_effect.h"

namespace OHOS::Ace::NG {

using SharedTransitionMap = std::unordered_map<ShareId, WeakPtr<FrameNode>>;
// PagePattern is the base class for page root render node.
class ACE_EXPORT PagePattern : public Pattern {
    DECLARE_ACE_TYPE(PagePattern, Pattern);

public:
    explicit PagePattern(const RefPtr<PageInfo>& pageInfo) : pageInfo_(pageInfo) {}
    ~PagePattern() override = default;

    bool IsMeasureBoundary() const override
    {
        return true;
    }

    bool IsAtomicNode() const override
    {
        return false;
    }

    const RefPtr<PageInfo>& GetPageInfo() const
    {
        return pageInfo_;
    }

    void OnShow();

    void OnHide();

    bool OnBackPressed() const
    {
        if (OnBackPressed_) {
            return OnBackPressed_();
        }
        return false;
    }

    void SetOnPageShow(std::function<void()>&& onPageShow)
    {
        onPageShow_ = std::move(onPageShow);
    }

    void SetOnPageHide(std::function<void()>&& onPageHide)
    {
        onPageHide_ = std::move(onPageHide);
    }

    void SetOnBackPressed(std::function<bool()>&& OnBackPressed)
    {
        OnBackPressed_ = std::move(OnBackPressed);
    }

    void SetPageTransitionFunc(std::function<void()>&& pageTransitionFunc)
    {
        pageTransitionFunc_ = std::move(pageTransitionFunc);
    }

    // find pageTransition effect according to transition type, it will clear the transition effect stack
    RefPtr<PageTransitionEffect> FindPageTransitionEffect(PageTransitionType type);

    void ClearPageTransitionEffect();

    RefPtr<PageTransitionEffect> GetTopTransition() const;

    void AddPageTransition(const RefPtr<PageTransitionEffect>& effect);

    RefPtr<EventHub> CreateEventHub() override
    {
        return MakeRefPtr<PageEventHub>();
    }

    bool TriggerPageTransition(PageTransitionType type, const std::function<void()>& onFinish);

    FocusPattern GetFocusPattern() const override
    {
        return { FocusType::SCOPE, true };
    }

    const SharedTransitionMap& GetSharedTransitionMap() const
    {
        return sharedTransitionMap_;
    }

    void BuildSharedTransitionMap();

private:
    void OnAttachToFrameNode() override;
    bool OnDirtyLayoutWrapperSwap(const RefPtr<LayoutWrapper>& wrapper, const DirtySwapConfig& config) override;

    // Mark current page node inactive state to show in render tree.
    void ProcessHideState();
    // Mark current page node active state to show in render tree.
    void ProcessShowState();

    RefPtr<PageInfo> pageInfo_;

    std::function<void()> onPageShow_;
    std::function<void()> onPageHide_;
    std::function<bool()> OnBackPressed_;
    std::function<void()> pageTransitionFunc_;
    std::stack<RefPtr<PageTransitionEffect>> pageTransitionEffects_;

    bool isOnShow_ = false;

    SharedTransitionMap sharedTransitionMap_;

    ACE_DISALLOW_COPY_AND_MOVE(PagePattern);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_STAGE_PAGE_PATTERN_H
