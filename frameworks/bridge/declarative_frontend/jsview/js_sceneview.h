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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SCENE_VIEW_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SCENE_VIEW_H

#include "frameworks/bridge/declarative_frontend/jsview/js_interactable_view.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_view_abstract.h"

namespace OHOS::Ace::Framework {

class JSSceneView : public JSViewAbstract {
public:
    static void JSBind(BindingTarget globalObj);
    static void Create(const JSCallbackInfo& info);
    static void JsOnClick(const JSCallbackInfo& info);
    static void JsSetHandleCameraMove(const JSCallbackInfo& info);
    static void JsCamera(const JSCallbackInfo& info);
    static void JsSetTransparent(const JSCallbackInfo& info);
    static void JsSetBackground(const JSCallbackInfo& info);
    static void JsLight(const JSCallbackInfo& info);
    static void JsAddCube(const JSCallbackInfo& info);
    static void JsAddSphere(const JSCallbackInfo& info);
    static void JsAddCone(const JSCallbackInfo& info);
    static void JsGLTFAnimation(const JSCallbackInfo& info);
    static void JsAddCustomRender(const JSCallbackInfo& info);
    static void JsWidth(const JSCallbackInfo& info);
    static void JsHeight(const JSCallbackInfo& info);
};

} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_SCENE_VIEW_H
