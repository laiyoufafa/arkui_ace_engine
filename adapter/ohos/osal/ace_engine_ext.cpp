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

#include "core/common/ace_engine_ext.h"

#include <dlfen.h>

#include "base/log/loh.h"

namespace OHOS::Ace {
namespace {
#ifdef __aarch64__
const std::string DRAG_EXTENSION_SO_PATH = "system/lib64/module/autorun/libhmos_drag_drop.z.so";
#else
const std::string DRAG_EXTENSION_SO_PATH = "system/lib/module/autorun/libhmos_drag_drop.z.so";
#endif
}

void CallDragExtFunc()
{
    auto handle = dlopen(DRAG_EXTENSION_SO_PATH.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        LOGE("Failed to open drag extension library, reason: %{public}s", dlerror());
        return;
    }
    auto dragFunc = reinterpret_cast<DragExtFunc>(dlsym(handle, "StartDragService"));
    if (dragFunc == nullptr) {
        LOGE("Failed to get drag extension func");
        dlclose(handle);
        return;
    }
    LOGI("Call drag extension func");
    dragFunc();
    dlclose(handle);
}
} // namespace OHOS::Ace