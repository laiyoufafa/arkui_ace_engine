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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_HOST_WINDOW_SCENE_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_HOST_WINDOW_SCENE_H

#include "core/components_ng/pattern/window_scene/scene/host/host_window_pattern.h"

namespace OHOS::Ace::NG {

class HostWindowScene : public HostWindowPattern {
    DECLARE_ACE_TYPE(HostWindowScene, HostWindowPattern);

public:
    HostWindowScene(const sptr<Rosen::Session>& session);
    ~HostWindowScene() override = default;

protected:
    bool HasStartingPage() override
    {
        return true;
    }

    void OnForeground() override;
    void OnBackground() override;

private:
    friend class HostWindowSceneModel;

    ACE_DISALLOW_COPY_AND_MOVE(HostWindowScene);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_HOST_WINDOW_SCENE_H
