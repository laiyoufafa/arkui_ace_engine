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

#include "gtest/gtest.h"
#include "interfaces/inner_api/ace/ui_content.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS::Ace::NG {
class ViewportConfigTestNG : public testing::Test {};

HWTEST_F(ViewportConfigTestNG, ParseTest001, TestSize.Level1)
{
    auto infos = std::vector<std::string>();
    UIContent::ShowDumpHelp(infos);
    EXPECT_GT(infos.size(), 0);
    for (auto info : infos) {
        GTEST_LOG_(INFO) << info;
    }
}
} // namespace OHOS::Ace::NG