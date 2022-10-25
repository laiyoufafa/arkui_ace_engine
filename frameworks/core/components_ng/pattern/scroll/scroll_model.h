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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SCROLL_SCROLL_MODEL_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SCROLL_SCROLL_MODEL_H

#include <memory>

#include "core/components/common/layout/constants.h"
#include "core/components/scroll_bar/scroll_bar_proxy.h"
#include "core/components_ng/pattern/scroll/scroll_event_hub.h"

namespace OHOS::Ace {

class ScrollBarTheme;
class ScrollControllerBase;

class ScrollModel {
public:
    static ScrollModel* GetInstance();
    virtual ~ScrollModel() = default;

    virtual void Create() = 0;
    virtual RefPtr<ScrollControllerBase> GetOrCreateController() = 0;
    virtual void SetAxis(Axis axis) = 0;
    virtual void SetOnScrollBegin(NG::ScrollBeginEvent&& event) = 0;
    virtual void SetOnScroll(NG::OnScrollEvent&& event) = 0;
    virtual void SetOnScrollEdge(NG::ScrollEdgeEvent&& event) = 0;
    virtual void SetOnScrollEnd(NG::ScrollEndEvent&& event) = 0;
    virtual void SetScrollBarProxy(const RefPtr<ScrollBarProxy>& proxy) = 0;
    virtual void InitScrollBar(const RefPtr<ScrollBarTheme>& theme, const std::pair<bool, Color>& color,
        const std::pair<bool, Dimension>& width, EdgeEffect effect) = 0;
    virtual void SetDisplayMode(int displayMode) = 0;
    virtual void SetScrollBarWidth(const Dimension& dimension) = 0;
    virtual void SetScrollBarColor(const Color& color) = 0;
    virtual void SetEdgeEffect(EdgeEffect edgeEffect) = 0;

private:
    static std::unique_ptr<ScrollModel> instance_;
};

} // namespace OHOS::Ace
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_SCROLL_SCROLL_MODEL_H
