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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVROUTER_NAVDESTINATION_VIEW_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVROUTER_NAVDESTINATION_VIEW_H

#include "base/memory/referenced.h"
#include "bridge/declarative_frontend/engine/js_ref_ptr.h"
#include "frameworks/bridge/declarative_frontend/engine/js_types.h"
#include "core/components_ng/base/ui_node.h"

namespace OHOS::Ace::NG {

using namespace Framework;

class ACE_EXPORT NavDestinationView {
public:
    static void Create();
    static void Create(std::function<void()>&& deepRenderFunc);
    static void SetHideTitleBar(bool hideTitleBar);
    static void SetTitle(const std::string& title, bool hasSubTitle);
    static void SetSubtitle(const std::string& subtitle);
    static void SetCustomTitle(const RefPtr<UINode>& customTitle);
    static void SetTitleHeight(const Dimension& height);
    static void SetOnShown(std::function<void()>&& onShown);
    static void SetOnHidden(std::function<void()>&& onHidden);
    static void SetOnBackPressed(std::function<bool()>&& onBackPressed);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVROUTER_NAVDESTINATION_VIEW_H
