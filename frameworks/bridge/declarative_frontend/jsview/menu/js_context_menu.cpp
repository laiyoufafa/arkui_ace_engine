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

#include "frameworks/bridge/declarative_frontend/jsview/menu/js_context_menu.h"

#include "base/subwindow/subwindow.h"
#include "base/subwindow/subwindow_manager.h"
#include "bridge/common/utils/engine_helper.h"
#include "core/common/container.h"

namespace OHOS::Ace::Framework {

void JSContextMenu::Close(const JSCallbackInfo& args)
{
    auto scopedDelegate = EngineHelper::GetCurrentDelegate();
    if (!scopedDelegate) {
        // this case usually means there is no foreground container, need to figure out the reason.
        LOGE("scopedDelegate is null, please check");
        return;
    }
#if defined(MULTIPLE_WINDOW_SUPPORTED)
    if (Container::IsCurrentUseNewPipeline()) {
        SubwindowManager::GetInstance()->HideMenuNG();
    } else {
        SubwindowManager::GetInstance()->CloseMenu();
    }
#else
    // Close context menu.
    auto container = Container::Current();
    if (container) {
        auto context = AceType::DynamicCast<PipelineContext>(container->GetPipelineContext());
        auto executor = Container::CurrentTaskExecutor();
        if (executor) {
            executor->PostTask(
                [context]() {
                    if (context) {
                        context->CloseContextMenu();
                    }
                },
                TaskExecutor::TaskType::UI);
        }
    }
#endif
    args.SetReturnValue(args.This());
}

void JSContextMenu::JSBind(BindingTarget globalObj)
{
    JSClass<JSContextMenu>::Declare("ContextMenu");
    JSClass<JSContextMenu>::StaticMethod("close", &JSContextMenu::Close);

    JSClass<JSContextMenu>::Inherit<JSViewAbstract>();
    JSClass<JSContextMenu>::Bind<>(globalObj);
}

} // namespace OHOS::Ace::Framework
