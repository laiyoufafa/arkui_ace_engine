/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_WEB_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_WEB_H

#include <string>

#include "bridge/declarative_frontend/jsview/js_view_common_def.h"
#include "core/components/web/web_component.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_container_base.h"

namespace OHOS::Ace::Framework {
class JSWeb : public JSContainerBase {
public:
    static void JSBind(BindingTarget globalObj);
    static void Create(const JSCallbackInfo& info);
    static void CreateInNewPipeline(JSRef<JSObject> controller, JSRef<JSVal> setWebIdFunction,
        std::optional<std::string> dstSrc);
    static void CreateInOldPipeline(JSRef<JSObject> controller, JSRef<JSVal> setWebIdFunction,
        std::optional<std::string> dstSrc);
    static void CreateWithWebController(JSRef<JSObject> controller,
        RefPtr<WebComponent>& webComponent);
    static void CreateWithWebviewController(JSRef<JSObject> controller, JSRef<JSVal> setWebIdFunction,
        RefPtr<WebComponent>& webComponent);
    static void OnAlert(const JSCallbackInfo& args);
    static void OnBeforeUnload(const JSCallbackInfo& args);
    static void OnConfirm(const JSCallbackInfo& args);
    static void OnFullScreenEnter(const JSCallbackInfo& args);
    static void OnFullScreenExit(const JSCallbackInfo& args);
    static void OnPrompt(const JSCallbackInfo& args);
    static void OnConsoleLog(const JSCallbackInfo& args);
    static void OnPageStart(const JSCallbackInfo& args);
    static void OnPageFinish(const JSCallbackInfo& args);
    static void OnProgressChange(const JSCallbackInfo& args);
    static void OnTitleReceive(const JSCallbackInfo& args);
    static void OnGeolocationHide(const JSCallbackInfo& args);
    static void OnGeolocationShow(const JSCallbackInfo& args);
    static void OnRequestFocus(const JSCallbackInfo& args);
    static void OnError(const JSCallbackInfo& args);
    static void OnMessage(const JSCallbackInfo& args);
    static void OnDownloadStart(const JSCallbackInfo& args);
    static void OnErrorReceive(const JSCallbackInfo& args);
    static void OnHttpErrorReceive(const JSCallbackInfo& args);
    static void OnFileSelectorShow(const JSCallbackInfo& args);
    static void CustomSchemes(const JSCallbackInfo& args);
    static void OnInterceptRequest(const JSCallbackInfo& args);
    static void OnUrlLoadIntercept(const JSCallbackInfo& args);
    static void JsEnabled(bool isJsEnabled);
    static void ContentAccessEnabled(bool isContentAccessEnabled);
    static void FileAccessEnabled(bool isFileAccessEnabled);
    static void OnFocus(const JSCallbackInfo& args);
    static void OnLineImageAccessEnabled(bool isOnLineImageAccessEnabled);
    static void DomStorageAccessEnabled(bool isDomStorageAccessEnabled);
    static void ImageAccessEnabled(bool isImageAccessEnabled);
    static void MixedMode(int32_t MixedModeNum);
    static void ZoomAccessEnabled(bool isZoomAccessEnabled);
    static void GeolocationAccessEnabled(bool isGeolocationAccessEnabled);
    static void JavaScriptProxy(const JSCallbackInfo& args);
    static void UserAgent(const std::string& userAgent);
    static void OnRenderExited(const JSCallbackInfo& args);
    static void OnRefreshAccessedHistory(const JSCallbackInfo& args);
    static void CacheMode(int32_t cacheMode);
    static void OverviewModeAccess(bool isOverviewModeAccessEnabled);
    static void FileFromUrlAccess(bool isFileFromUrlAccessEnabled);
    static void DatabaseAccess(bool isDatabaseAccessEnabled);
    static void TextZoomRatio(int32_t textZoomRatioNum);
    static void WebDebuggingAccessEnabled(bool isWebDebuggingAccessEnabled);
    static void OnMouse(const JSCallbackInfo& args);
    static void OnResourceLoad(const JSCallbackInfo& args);
    static void OnScaleChange(const JSCallbackInfo& args);
    static void OnScroll(const JSCallbackInfo& args);
    static void BackgroundColor(const JSCallbackInfo& info);
    static void InitialScale(float scale);
    static void Password(bool password);
    static void TableData(bool tableData);
    static void OnFileSelectorShowAbandoned(const JSCallbackInfo& args);
    static void OnHttpAuthRequest(const JSCallbackInfo& args);
    static void OnSslErrorRequest(const JSCallbackInfo& args);
    static void OnSslSelectCertRequest(const JSCallbackInfo& args);
    static void OnPermissionRequest(const JSCallbackInfo& args);
    static void OnContextMenuShow(const JSCallbackInfo& args);
    static void OnSearchResultReceive(const JSCallbackInfo& args);
    static void MediaPlayGestureAccess(bool isNeedGestureAccess);
    static void OnKeyEvent(const JSCallbackInfo& args);
    static void JsOnDragStart(const JSCallbackInfo& info);
    static void JsOnDragEnter(const JSCallbackInfo& info);
    static void JsOnDragMove(const JSCallbackInfo& info);
    static void JsOnDragLeave(const JSCallbackInfo& info);
    static void JsOnDrop(const JSCallbackInfo& info);
    static void PinchSmoothModeEnabled(bool isPinchSmoothModeEnabled);
    static void OnWindowNew(const JSCallbackInfo& args);
    static void OnWindowExit(const JSCallbackInfo& args);
    static void MultiWindowAccessEnabled(bool isMultiWindowAccessEnable);
    static void WebCursiveFont(const std::string& cursiveFontFamily);
    static void WebFantasyFont(const std::string& fantasyFontFamily);
    static void WebFixedFont(const std::string& fixedFontFamily);
    static void WebSansSerifFont(const std::string& sansSerifFontFamily);
    static void WebSerifFont(const std::string& serifFontFamily);
    static void WebStandardFont(const std::string& standardFontFamily);
    static void DefaultFixedFontSize(int32_t defaultFixedFontSize);
    static void DefaultFontSize(int32_t defaultFontSize);
    static void MinFontSize(int32_t minFontSize);
    static void MinLogicalFontSize(int32_t minLogicalFontSize);
    static void BlockNetwork(bool isNetworkBlocked);
    static void OnPageVisible(const JSCallbackInfo& args);
    static void OnInterceptKeyEvent(const JSCallbackInfo& args);
    static void OnDataResubmitted(const JSCallbackInfo& args);
    static void OnFaviconReceived(const JSCallbackInfo& args);
    static void OnTouchIconUrlReceived(const JSCallbackInfo& args);
    static void DarkMode(int32_t darkMode);
    static void ForceDarkAccess(bool access);

protected:
    static void OnCommonDialog(const JSCallbackInfo& args, int dialogEventType);
};
} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_JS_WEB_H
