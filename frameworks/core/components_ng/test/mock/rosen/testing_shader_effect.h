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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_SHADER_EFFECT_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_SHADER_EFFECT_H

#include <memory>

#include "testing_color.h"
#include "testing_point.h"

namespace OHOS::Ace::Testing {
enum class TileMode {
    CLAMP,
    REPEAT,
    MIRROR,
};
class TestingShaderEffect {
public:
    TestingShaderEffect() = default;
    ~TestingShaderEffect() = default;

    static std::shared_ptr<TestingShaderEffect> CreateSweepGradient(const TestingPoint& /* centerPt */,
        const std::vector<uint32_t>& /* colors */, const std::vector<float>& /* pos */, TileMode /* mode */,
        float /* startAngle */, float /* endAngle */)
    {
        return std::make_shared<TestingShaderEffect>();
    }

    static std::shared_ptr<TestingShaderEffect> CreateLinearGradient(const TestingPoint& /* startPt */,
        const TestingPoint& /* endPt */, const std::vector<uint32_t>& /* colors */, const std::vector<float>& /* pos */,
        TileMode /* mode */)
    {
        return std::make_shared<TestingShaderEffect>();
    }
};
} // namespace OHOS::Ace::Testing
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_SHADER_EFFECT_H
