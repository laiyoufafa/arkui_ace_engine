/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <string>

#include "gtest/gtest.h"

#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/marquee/marquee_accessibility_property.h"
#include "core/components_ng/pattern/marquee/marquee_model_ng.h"
#include "core/components_ng/pattern/marquee/marquee_pattern.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS::Ace::NG {
namespace {
const std::string MARQUEE_SRC = "marquee";
} // namespace

class MarqueeAccessibilityPropertyTestNg : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp() override;
    void TearDown() override;
};

void MarqueeAccessibilityPropertyTestNg::SetUpTestCase() {}
void MarqueeAccessibilityPropertyTestNg::TearDownTestCase() {}
void MarqueeAccessibilityPropertyTestNg::SetUp() {}
void MarqueeAccessibilityPropertyTestNg::TearDown() {}

/**
 * @tc.name: MarqueeTest001
 * @tc.desc: Test Text property of marquee.
 * @tc.type: FUNC
 */
HWTEST_F(MarqueeAccessibilityPropertyTestNg, MarqueeAccessibilityPropertyTestNg001, TestSize.Level1)
{
    MarqueeModelNG marqueeModel;
    marqueeModel.Create();
    marqueeModel.SetValue(MARQUEE_SRC);

    auto frameNode = AceType::DynamicCast<FrameNode>(ViewStackProcessor::GetInstance()->Finish());
    EXPECT_NE(frameNode, nullptr);
    auto accessibility = frameNode->GetAccessibilityProperty<MarqueeAccessibilityProperty>();
    EXPECT_EQ(accessibility->GetText(), MARQUEE_SRC);
}
} // namespace OHOS::Ace::NG
