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

#ifndef FOUNDATION_ACE_TEST_UNITTEST_CORE_COMMON_PLUGIN_MOCK_PLUGIN_TUILS_H
#define FOUNDATION_ACE_TEST_UNITTEST_CORE_COMMON_PLUGIN_MOCK_PLUGIN_TUILS_H

#include <string>

#include "core/common/plugin_utils.h"

namespace OHOS::Ace {
class MockPluginUtils final : public PluginUtils {
public:
    MockPluginUtils() = default;
    ~MockPluginUtils() = default;

    int32_t StartAbility(
        const std::string& bundleName, const std::string& abilityName, const std::string& params) override
    {
        return 1;
    }
};
} // namespace OHOS::Ace
#endif // FOUNDATION_ACE_TEST_UNITTEST_CORE_COMMON_PLUGIN_MOCK_PLUGIN_TUILS_H