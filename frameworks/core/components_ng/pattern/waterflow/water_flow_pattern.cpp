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

#include "core/components_ng/pattern/waterflow/water_flow_pattern.h"

namespace OHOS::Ace::NG {
bool WaterFlowPattern::UpdateCurrentOffset(float /* delta */, int32_t /* source */)
{
    return true;
};
bool WaterFlowPattern::IsAtTop() const
{
    return true;
};
bool WaterFlowPattern::IsAtBottom() const
{
    return false;
};
void WaterFlowPattern::UpdateScrollBarOffset() {};
} // namespace OHOS::Ace::NG
