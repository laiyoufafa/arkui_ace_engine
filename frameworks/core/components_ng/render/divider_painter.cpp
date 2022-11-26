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

#include "core/components_ng/render/divider_painter.h"

#include "core/components_ng/render/drawing_prop_convertor.h"

namespace OHOS::Ace::NG {
void DividerPainter::DrawLine(RSCanvas& canvas, const OffsetF& offset) const
{
    RSPen pen;
    pen.SetAntiAlias(true);
    pen.SetWidth(constrainStrokeWidth_);
    pen.SetCapStyle(ToRSCapStyle(lineCap_.value_or(LineCap::SQUARE)));
    pen.SetColor(ToRSColor(dividerColor_.value_or(Color::BLACK)));
    canvas.AttachPen(pen);

    auto dividerWidth = constrainStrokeWidth_ / 2;
    auto startPointX = offset.GetX() + dividerWidth;
    auto startPointY = offset.GetY() + dividerWidth;
    PointF start = PointF(startPointX, startPointY);
    PointF end = vertical_ ? PointF(startPointX, startPointY + dividerLength_ - constrainStrokeWidth_)
                           : PointF(startPointX + dividerLength_ - constrainStrokeWidth_, startPointY);
    canvas.DrawLine(ToRSPonit(start), ToRSPonit(end));
}
} // namespace OHOS::Ace::NG