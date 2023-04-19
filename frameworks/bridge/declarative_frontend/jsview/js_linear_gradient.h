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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_LINEAR_GRADIENT_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_LINEAR_GRADIENT_H

#include "base/memory/referenced.h"
#include "bridge/declarative_frontend/engine/bindings.h"

namespace OHOS::Ace::Framework {
class JSLinearGradient final : public Referenced {
public:
    JSLinearGradient() = default;
    ~JSLinearGradient() override = default;

    static void JSBind(BindingTarget globalObj);
    static void Constructor(const JSCallbackInfo& args);
    static void Destructor(JSLinearGradient* jsLinearGradientPtr);
    std::vector<std::pair<Color, Dimension>> GetGradient()
    {
        return gradient_;
    }

private:
    std::vector<std::pair<Color, Dimension>> gradient_;
    ACE_DISALLOW_COPY_AND_MOVE(JSLinearGradient);
};
} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_LINEAR_GRADIENT_H