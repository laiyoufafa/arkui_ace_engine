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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TAB_CONTENT_VIEW_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TAB_CONTENT_VIEW_H

#include "base/geometry/axis.h"
#include "base/geometry/dimension.h"
#include "base/memory/referenced.h"
#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components_ng/pattern/tabs/tab_bar_layout_property.h"
#include "core/components_ng/pattern/tabs/tab_bar_pattern.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT TabContentView {
public:
    static void Create(std::function<void()>&& deepRenderFunc);
    static void Create();
    static void Pop(const RefPtr<FrameNode>& tabContent);
    static void SetTabBar(const std::string& text, const std::string& icon, TabBarBuilderFunc&& builder);
};

} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TABS_TAB_CONTENT_VIEW_H
