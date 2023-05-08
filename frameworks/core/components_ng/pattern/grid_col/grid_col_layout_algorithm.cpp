
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

#include "grid_col_layout_algorithm.h"

#include "core/components_ng/property/measure_utils.h"
#include "core/components_v2/grid_layout/grid_container_utils.h"

namespace OHOS::Ace::NG {

void GridColLayoutAlgorithm::Layout(FrameNode* frameNode)
{
    auto children = frameNode->GetAllFrameNodeChildren();
    for (auto&& child : children) {
        child->Layout();
    }
}

} // namespace OHOS::Ace::NG
