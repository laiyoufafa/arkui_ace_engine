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

#include "flutter_render_data_panel.h"
#include "rosen_render_data_panel.h"

namespace OHOS::Ace {
RefPtr<RenderNode> RenderProgressDataPanel::Create()
{
    if (SystemProperties::GetRosenBackendEnabled()) {
        return AceType::MakeRefPtr<RosenRenderProgressDataPanel>();
    } else {
        return AceType::MakeRefPtr<FlutterRenderProgressDataPanel>();
    }
}

RefPtr<RenderNode> RenderPercentageDataPanel::Create()
{
    if (SystemProperties::GetRosenBackendEnabled()) {
        return AceType::MakeRefPtr<RosenRenderPercentageDataPanel>();
    } else {
        return AceType::MakeRefPtr<FlutterRenderPercentageDataPanel>();
    }
}
} // namespace OHOS::Ace
