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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_TEST_PATTERN_CUSTOM_PAINT_MOCK_PARAGRAPH_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_TEST_PATTERN_CUSTOM_PAINT_MOCK_PARAGRAPH_H

#include <cstddef>

#include "gmock/gmock.h"
#include "rosen_text/typography.h"
#include "txt/paragraph.h"
#include "include/core/SkCanvas.h"

namespace OHOS::Ace::NG {

class MockParagraph : public Rosen::Typography {
public:
    MOCK_CONST_METHOD0(GetMaxWidth, double());
    MOCK_CONST_METHOD0(GetHeight, double());
    MOCK_CONST_METHOD0(GetActualWidth, double());
    MOCK_METHOD0(GetMinIntrinsicWidth, double());
    MOCK_METHOD0(GetMaxIntrinsicWidth, double());
    MOCK_METHOD0(GetAlphabeticBaseline, double());
    MOCK_METHOD0(GetIdeographicBaseline, double());
    MOCK_CONST_METHOD0(DidExceedMaxLines, bool());
    MOCK_METHOD0(GetTextRectsOfPlaceholders, std::vector<Rosen::TextRect>());
    MOCK_CONST_METHOD0(GetLineCount, int());

    MOCK_METHOD1(SetIndents, void(const std::vector<float> &indents));
    MOCK_METHOD1(Layout, void(double width));
    MOCK_METHOD1(GetWordBoundaryByIndex, Rosen::Boundary(size_t offset));

    MOCK_METHOD2(GetGlyphIndexByCoordinate, Rosen::IndexAndAffinity(double x, double y));

    MOCK_METHOD3(Paint, void(SkCanvas* canvas, double x, double y));
    MOCK_METHOD3(Paint, void(Rosen::Drawing::Canvas* canvas, double x, double y));

    MOCK_METHOD4(GetTextRectsByBoundary, std::vector<Rosen::TextRect>(size_t start,
      size_t end,
      Rosen::TextRectHeightStyle rect_height_style,
      Rosen::TextRectWidthStyle rect_width_style));
#ifdef NEW_SKIA
    MOCK_METHOD0(GetLineMetrics, std::vector<txt::LineMetrics>&());
#endif
};

} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_TEST_PATTERN_CUSTOM_PAINT_MOCK_PARAGRAPH_H
