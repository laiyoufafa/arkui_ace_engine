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

#ifndef FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_TEXTPICKER_MODEL_IMPL_H
#define FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_TEXTPICKER_MODEL_IMPL_H

#include "core/components_ng/pattern/text_picker/textpicker_model.h"

namespace OHOS::Ace::Framework {
class TextPickerModelImpl : public TextPickerModel {
public:
    TextPickerModelImpl() = default;
    ~TextPickerModelImpl() override = default;

    void Create(RefPtr<PickerTheme> pickerTheme, uint32_t columnKind) override;
    void SetSelected(uint32_t value) override;
    void SetRange(const std::vector<NG::RangeContent>& value) override;
    void SetValue(const std::string& value) override {}
    void SetOnChange(TextChangeEvent&& onChange) override;
    void SetDefaultPickerItemHeight(const Dimension& value) override;
    void SetDefaultAttributes(RefPtr<PickerTheme> pickerTheme) override {}
    void SetDisappearTextStyle(const NG::PickerTextStyle& value) override {};
    void SetNormalTextStyle(const NG::PickerTextStyle& value) override {};
    void SetSelectedTextStyle(const NG::PickerTextStyle& value) override {};
};
} // namespace OHOS::Ace::Framework
#endif // FRAMEWORKS_BRIDGE_DECLARATIVE_FRONTEND_JS_VIEW_MODELS_TEXTPICKER_MODEL_IMPL_H
