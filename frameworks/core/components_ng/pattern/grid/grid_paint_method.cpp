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

#include "core/components_ng/pattern/grid/grid_paint_method.h"

#include "core/components_ng/pattern/scroll/inner/scroll_bar_painter.h"

namespace OHOS::Ace::NG {
CanvasDrawFunction GridPaintMethod::GetForegroundDrawFunction(PaintWrapper* /* paintWrapper */)
{
    return [weak = scrollBar_](RSCanvas& canvas) {
        auto scrollBar = weak.Upgrade();
        CHECK_NULL_VOID(scrollBar);
        ScrollBarPainter::PaintRectBar(canvas, scrollBar, UINT8_MAX);
    };
}
} // namespace OHOS::Ace::NG