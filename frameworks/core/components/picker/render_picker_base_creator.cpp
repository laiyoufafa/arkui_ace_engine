/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#ifndef NEW_SKIA
#include "flutter_render_picker_base.h"
#endif
#include "rosen_render_picker_base.h"

namespace OHOS::Ace {
RefPtr<RenderNode> RenderPickerBase::Create()
{
    if (SystemProperties::GetRosenBackendEnabled()) {
#ifdef ENABLE_ROSEN_BACKEND
        return AceType::MakeRefPtr<RosenRenderPickerBase>();
#else
        return nullptr;
#endif
    } else {
#ifndef NEW_SKIA
        return AceType::MakeRefPtr<FlutterRenderPickerBase>();
#else
        return nullptr;
#endif
    }
}
} // namespace OHOS::Ace
