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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVIGATION_BAR_ITEM_NODE_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVIGATION_BAR_ITEM_NODE_H

#include <cstdint>
#include <optional>

#include "core/components_ng/base/frame_node.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT BarItemNode : public FrameNode {
    DECLARE_ACE_TYPE(BarItemNode, FrameNode)
public:
    BarItemNode(const std::string& tag, int32_t nodeId);
    ~BarItemNode() override = default;

    bool IsAtomicNode() const override
    {
        return false;
    }

    void SetTextNode(const RefPtr<UINode>& text)
    {
        text_ = text;
    }

    const RefPtr<UINode>& GetTextNode() const
    {
        return text_;
    }

    void SetIconNode(const RefPtr<UINode>& icon)
    {
        icon_ = icon;
    }

    const RefPtr<UINode>& GetIconNode() const
    {
        return icon_;
    }

    ACE_DEFINE_PROPERTY_ITEM_FUNC_WITHOUT_GROUP(IconIndex, int32_t);
    void OnIconIndexUpdate(int32_t value) {}
    ACE_DEFINE_PROPERTY_ITEM_FUNC_WITHOUT_GROUP(TextIndex, int32_t);
    void OnTextIndexUpdate(int32_t value) {}
    ACE_DEFINE_PROPERTY_ITEM_FUNC_WITHOUT_GROUP(IconSrc, std::string);
    void OnIconSrcUpdate(const std::string& value) {}
    ACE_DEFINE_PROPERTY_ITEM_FUNC_WITHOUT_GROUP(Text, std::string);
    void OnTextUpdate(const std::string& value) {}

private:
    RefPtr<UINode> text_;
    RefPtr<UINode> icon_;
};

} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_NAVIGATION_BAR_ITEM_NODE_H