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

#include "core/components_ng/test/pattern/relative_container/relative_container_common_ng.h"

namespace OHOS::Ace {

void RelativeContainerTestUtilsNG::AddAlignRule(const std::string& id, const AlignDirection& direction,
    const HorizontalAlign& horizontalRule, std::map<AlignDirection, AlignRule>& alignRules)
{
    AlignRule alignRule;
    alignRule.anchor = id;
    alignRule.horizontal = horizontalRule;
    alignRules[direction] = alignRule;
}

void RelativeContainerTestUtilsNG::AddAlignRule(const std::string& id, const AlignDirection& direction,
    const VerticalAlign& verticalRule, std::map<AlignDirection, AlignRule>& alignRules)
{
    AlignRule alignRule;
    alignRule.anchor = id;
    alignRule.vertical = verticalRule;
    alignRules[direction] = alignRule;
}
} // namespace OHOS::Ace