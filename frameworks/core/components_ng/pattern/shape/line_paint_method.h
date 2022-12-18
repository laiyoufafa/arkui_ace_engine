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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_LINE_PAINT_METHOD_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_LINE_PAINT_METHOD_H

#include <utility>

#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/shape/line_paint_property.h"
#include "core/components_ng/render/line_painter.h"
#include "core/components_ng/render/node_paint_method.h"

namespace OHOS::Ace::NG {
class ACE_EXPORT LinePaintMethod : public NodePaintMethod {
    DECLARE_ACE_TYPE(LinePaintMethod, NodePaintMethod)
public:
    LinePaintMethod() = default;
    explicit LinePaintMethod(const RefPtr<ShapePaintProperty>& shapePaintProperty)
        : propertiesFromAncestor_(shapePaintProperty)
    {}
    ~LinePaintMethod() override = default;
    CanvasDrawFunction GetContentDrawFunction(PaintWrapper* paintWrapper) override
    {
        auto linePaintProperty = DynamicCast<LinePaintProperty>(paintWrapper->GetPaintProperty()->Clone());
        if (!linePaintProperty) {
            return nullptr;
        }
        if (propertiesFromAncestor_) {
            linePaintProperty->UpdateShapeProperty(propertiesFromAncestor_);
        }
        if (!linePaintProperty->GetStartPoint() || !linePaintProperty->GetEndPoint()) {
            return nullptr;
        }
        auto offset = paintWrapper->GetContentOffset();
        return [linePaintProperty, offset](RSCanvas& canvas) {
            LinePainter::DrawLine(canvas, *linePaintProperty, offset); };
    }

private:
    RefPtr<ShapePaintProperty> propertiesFromAncestor_;
    ACE_DISALLOW_COPY_AND_MOVE(LinePaintMethod);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERN_LINE_PAINT_METHOD_H
