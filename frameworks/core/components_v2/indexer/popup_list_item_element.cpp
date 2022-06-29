/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "core/components_v2/indexer/popup_list_item_element.h"
#include "core/components_v2/indexer/popup_list_item_component.h"
#include "core/pipeline/base/composed_component.h"

namespace OHOS::Ace::V2 {
void PopupListItemElement::PerformBuild()
{
    RefPtr<PopupListItemComponent> popupListItemComponent = AceType::DynamicCast<PopupListItemComponent>(component_);
    if (popupListItemComponent) {
        ComponentGroupElement::PerformBuild();
    }
}
} // namespace OHOS::Ace::V2
