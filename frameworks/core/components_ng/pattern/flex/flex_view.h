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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_LINEAR_LAYOUT_FLEX_VIEW_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_LINEAR_LAYOUT_FLEX_VIEW_H

#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"

namespace OHOS::Ace::NG {
class ACE_EXPORT FlexView {
public:
    static void Create(FlexDirection direction, FlexAlign mainAxisAlign, FlexAlign crossAxisAlign);
    static void CreateWrap(WrapDirection direction, WrapAlignment mainAxisAlign, WrapAlignment crossAxisAlign,
        WrapAlignment alignment);
    static void Direction(const FlexDirection& value);
    static void MainAxisAlign(const FlexAlign& value);
    static void CrossAxisAlign(const FlexAlign& value);

    static void SetWrapDirection(WrapDirection value);
    static void SetWrapAlignment(WrapAlignment value);
    static void SetWrapMainAlignment(WrapAlignment value);
    static void SetWrapCrossAlignment(WrapAlignment value);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_LINEAR_LAYOUT_FLEX_VIEW_H
