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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_CIRCLE_PAINT_METHOD_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_CIRCLE_PAINT_METHOD_H

#include "base/geometry/ng/size_t.h"
#include "core/components_ng/base/geometry_node.h"
#include "core/components_ng/pattern/select_overlay/select_overlay_property.h"
#include "core/components_ng/pattern/shape/shape_paint_property.h"
#include "core/components_ng/render/circle_painter.h"
#include "core/components_ng/render/node_paint_method.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT SelectOverlayPaintMethod : public NodePaintMethod {
    DECLARE_ACE_TYPE(SelectOverlayPaintMethod, NodePaintMethod)
public:
    explicit SelectOverlayPaintMethod(std::shared_ptr<SelectOverlayInfo> info) : info_(std::move(info)) {}
    ~SelectOverlayPaintMethod() override = default;

    CanvasDrawFunction GetContentDrawFunction(PaintWrapper* paintWrapper) override;

private:
    static void DrawHandles(const std::shared_ptr<SelectOverlayInfo>& info, RSCanvas& canvas);

    static void PaintHandle(RSCanvas& canvas, const RectF& handleRect, bool handleOnTop);

    std::shared_ptr<SelectOverlayInfo> info_;

    ACE_DISALLOW_COPY_AND_MOVE(SelectOverlayPaintMethod);
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_CIRCLE_PAINT_METHOD_H