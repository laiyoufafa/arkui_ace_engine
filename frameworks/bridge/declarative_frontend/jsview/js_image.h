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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_IMAGE_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_IMAGE_H

#include "frameworks/bridge/declarative_frontend/jsview/js_interactable_view.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_utils.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_view_abstract.h"

namespace OHOS::Ace::Framework {
class JSImage : public JSViewAbstract, public JSInteractableView {
public:
    static void Create(const JSCallbackInfo& info);
    static void HandleLoadImageSuccess(const BaseEventInfo& param);
    static void HandleLoadImageFail(const BaseEventInfo& param);
    static void SetAlt(const JSCallbackInfo& args);
    static void SetMatchTextDirection(bool value);
    static void SetFitOriginalSize(bool value);
    static void SetSourceSize(const JSCallbackInfo& info);
    static void SetObjectFit(int32_t value);
    static void SetImageFill(const JSCallbackInfo& info);
    static void SetImageInterpolation(int32_t imageInterpolation);
    static void SetImageRenderMode(int32_t imageRenderMode);
    static void SetImageRepeat(int32_t imageRepeat);
    static void JSBind(BindingTarget globalObj);
    static void OnComplete(const JSCallbackInfo& args);
    static void OnError(const JSCallbackInfo& args);
    static void OnFinish(const JSCallbackInfo& info);
    static void SetSyncLoad(const JSCallbackInfo& info);
    static void SetColorFilter(const JSCallbackInfo& info);

    static void JsBorder(const JSCallbackInfo& info);
    static void JsBorderRadius(const JSCallbackInfo& info);
    static void SetLeftBorderWidth(const CalcDimension& value);
    static void SetTopBorderWidth(const CalcDimension& value);
    static void SetRightBorderWidth(const CalcDimension& value);
    static void SetBottomBorderWidth(const CalcDimension& value);
    static void SetBorderRadius(const CalcDimension& value);
    static void JsOpacity(const JSCallbackInfo& info);
    static void JsBlur(const JSCallbackInfo& info);
    static void JsTransition(const JSCallbackInfo& info);
    static void JsSetDraggable(bool draggable);
    static void JsOnDragStart(const JSCallbackInfo& info);
    static void SetCopyOption(const JSCallbackInfo& info);
    static void GetJsMediaBundleInfo(const JSRef<JSVal>& jsValue, std::string& bundleName, std::string& moduleName);

protected:
    static void SetBorder(const Border& border);
    static void SetAutoResize(bool autoResize);

    static bool IsDrawable(const JSRef<JSVal>& jsValue);
};

class JSColorFilter : public AceType {
    DECLARE_ACE_TYPE(JSColorFilter, AceType);

public:
    JSColorFilter() = default;
    ~JSColorFilter() override = default;
    static void ConstructorCallback(const JSCallbackInfo& info);
    static void DestructorCallback(JSColorFilter* obj);

    void SetColorFilterMatrix(const std::vector<float>&& matrix)
    {
        colorfiltermatrix_ = std::move(matrix);
    }

    const std::vector<float>& GetColorFilterMatrix() const
    {
        return colorfiltermatrix_;
    }

private:
    std::vector<float> colorfiltermatrix_;
};

} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_IMAGE_H
