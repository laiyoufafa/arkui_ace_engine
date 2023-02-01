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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_BRUSH_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_BRUSH_H

#include <memory>

#include "testing_color.h"
#include "testing_filter.h"
#include "testing_shader_effect.h"

namespace OHOS::Ace::Testing {
enum class BlendMode {
    CLEAR,
    SRC,
    DST,
    SRC_OVER,
    DST_OVER,
    SRC_IN,
    DST_IN,
    SRC_OUT,
    DST_OUT,
    SRC_ATOP,
    DST_ATOP,
    XOR,
    PLUS,
    MODULATE,
    SCREEN,
    OVERLAY,
    DARKEN,
    LIGHTEN,
    COLOR_DODGE,
    COLOR_BURN,
    HARD_LIGHT,
    SOFT_LIGHT,
    DIFFERENCE,
    EXCLUSION,
    MULTIPLY,
    HUE,
    STATURATION,
    COLOR,
    LUMINOSITY,
};
class TestingBrush {
public:
    TestingBrush() = default;
    ~TestingBrush() = default;
    explicit TestingBrush(int rgba) noexcept {};

    virtual void SetAntiAlias(bool alias) {}
    virtual void SetWidth(float width) {}
    virtual void SetColor(const TestingColor& color) {}
    virtual void SetColor(int color) {}
    virtual void SetShaderEffect(std::shared_ptr<TestingShaderEffect> effect) {}
    virtual void SetFilter(const TestingFilter filter) {}
    virtual void SetBlendMode(BlendMode mode) {}
    virtual void SetAlpha(uint32_t alpha) {}
    virtual void SetAlphaF(float a) {}
    virtual void SetARGB(int r, int g, int b, int a) {}
};
} // namespace OHOS::Ace::Testing
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_MOCK_ROSEN_TEST_TESTING_BRUSH_H
