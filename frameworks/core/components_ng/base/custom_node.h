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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_BASE_COSTOM_NODE_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_BASE_COSTOM_NODE_H

#include <string>

#include "base/utils/macros.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/pattern/custom/custom_node_pattern.h"

namespace OHOS::Ace::NG {
// CustomNode is the frame node of @Component struct.
class ACE_EXPORT CustomNode : public FrameNode {
    DECLARE_ACE_TYPE(CustomNode, FrameNode);

public:
    using ApplyFunction = std::function<void(const RefPtr<FrameNode>&)>;
    using AppearFunction = std::function<void()>;
    using UpdateJsViewFunction = std::function<void(WeakPtr<CustomNode>)>;

    static RefPtr<CustomNode> CreateCustomNode(const std::string& id);

    CustomNode(const std::string& id);
    ~CustomNode() override;

    void SetAppearFunction(AppearFunction&& func)
    {
        appearFunction_ = std::move(func);
    }

    void SetUpdateJsViewFunction(UpdateJsViewFunction&& func)
    {
        UpdateJsViewFunction_ = std::move(func);
    }

    void SetRenderFunction(const RenderFunction& renderFunction)
    {
        auto pattern = DynamicCast<CustomNodePattern>(GetPattern());
        if (pattern) {
            pattern->SetRenderFunction(renderFunction);
        }
    }

    // called by view in js thread
    void MarkNeedRebuild();

    // called by pipeline in js thread of update.
    void Rebuild();

protected:
    void OnContextAttached() override;

private:
    void BuildChildren(const RefPtr<FrameNode>& child);

    AppearFunction appearFunction_;
    UpdateJsViewFunction UpdateJsViewFunction_;

    bool needRebuild_ = false;
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_BASE_COSTOM_NODE_H
