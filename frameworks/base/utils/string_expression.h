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

#ifndef FOUNDATION_ACE_FRAMEWORKS_BASE_UTILS_STRING_EXPRESSION_H
#define FOUNDATION_ACE_FRAMEWORKS_BASE_UTILS_STRING_EXPRESSION_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "base/geometry/dimension.h"

namespace OHOS::Ace::StringExpression {

void InitMapping(std::map<std::string, int>& mapping);

std::vector<std::string> ConvertDal2Rpn(std::string formula);

double CalculateExp(const std::string& expression, const std::function<double(const Dimension&)>& calcFunc);

} // namespace OHOS::Ace::StringExpression

#endif // FOUNDATION_ACE_FRAMEWORKS_BASE_UTILS_STRING_EXPRESSION_H