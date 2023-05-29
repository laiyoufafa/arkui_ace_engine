/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_ACE_FRAMEWORK_JAVASCRIPT_BRIDGE_JS_VIEW_JS_TABS_H
#define FOUNDATION_ACE_FRAMEWORK_JAVASCRIPT_BRIDGE_JS_VIEW_JS_TABS_H

#include "frameworks/bridge/declarative_frontend/jsview/js_container_base.h"

namespace OHOS::Ace::Framework {

class JSTabs : public JSContainerBase {
public:
    static void JSBind(BindingTarget globalObj);
    static void Create(const JSCallbackInfo& info);
    static void Pop();

    static void CreateForNG(const JSCallbackInfo& info);
    static void SetIndex(int32_t index);
    static void SetVertical(const std::string& value);
    static void SetBarPosition(const JSCallbackInfo& info);
    static void SetScrollable(const std::string& value);
    static void SetBarMode(const std::string& value);
    static void SetBarWidth(const JSCallbackInfo& info);
    static void SetBarHeight(const JSCallbackInfo& info);
    static void SetOnChange(const JSCallbackInfo& info);
    static void SetAnimationDuration(float value);
    static void SetFadingEdge(const JSCallbackInfo& info);
    static void SetDivider(const JSCallbackInfo& info);
    static void SetBarOverlap(const JSCallbackInfo& info);
    static void SetBarBackgroundColor(const JSCallbackInfo& info);
    static void SetClip(const JSCallbackInfo& info);
};

} // namespace OHOS::Ace::Framework

#endif // FOUNDATION_ACE_FRAMEWORK_JAVASCRIPT_BRIDGE_JS_VIEW_JS_TABS_H
