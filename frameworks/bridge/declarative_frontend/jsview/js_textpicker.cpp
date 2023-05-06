/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "bridge/declarative_frontend/jsview/js_textpicker.h"

#include <securec.h>

#include "base/log/ace_scoring_log.h"
#include "bridge/common/utils/engine_helper.h"
#include "bridge/declarative_frontend/engine/functions/js_function.h"
#include "bridge/declarative_frontend/jsview/js_datepicker.h"
#include "bridge/declarative_frontend/jsview/js_interactable_view.h"
#include "bridge/declarative_frontend/jsview/js_view_abstract.h"
#include "bridge/declarative_frontend/jsview/js_view_common_def.h"
#include "bridge/declarative_frontend/jsview/models/textpicker_model_impl.h"
#include "bridge/declarative_frontend/view_stack_processor.h"
#include "core/components/picker/picker_base_component.h"
#include "core/components/picker/picker_theme.h"
#include "core/components_ng/pattern/text_picker/textpicker_model.h"
#include "core/components_ng/pattern/text_picker/textpicker_model_ng.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace {

std::unique_ptr<TextPickerModel> TextPickerModel::textPickerInstance_ = nullptr;

TextPickerModel* TextPickerModel::GetInstance()
{
    if (!textPickerInstance_) {
#ifdef NG_BUILD
        textPickerInstance_.reset(new NG::TextPickerModelNG());
#else
        if (Container::IsCurrentUseNewPipeline()) {
            textPickerInstance_.reset(new NG::TextPickerModelNG());
        } else {
            textPickerInstance_.reset(new Framework::TextPickerModelImpl());
        }
#endif
    }
    return textPickerInstance_.get();
}
} // namespace OHOS::Ace

namespace OHOS::Ace::Framework {
void JSTextPicker::JSBind(BindingTarget globalObj)
{
    JSClass<JSTextPicker>::Declare("TextPicker");
    MethodOptions opt = MethodOptions::NONE;
    JSClass<JSTextPicker>::StaticMethod("create", &JSTextPicker::Create, opt);
    JSClass<JSTextPicker>::StaticMethod("defaultPickerItemHeight", &JSTextPicker::SetDefaultPickerItemHeight);
    JSClass<JSTextPicker>::StaticMethod("disappearTextStyle", &JSTextPicker::SetDisappearTextStyle);
    JSClass<JSTextPicker>::StaticMethod("textStyle", &JSTextPicker::SetTextStyle);
    JSClass<JSTextPicker>::StaticMethod("selectedTextStyle", &JSTextPicker::SetSelectedTextStyle);
    JSClass<JSTextPicker>::StaticMethod("selectedIndex", &JSTextPicker::SetSelectedIndex);
    JSClass<JSTextPicker>::StaticMethod("onAccept", &JSTextPicker::OnAccept);
    JSClass<JSTextPicker>::StaticMethod("onCancel", &JSTextPicker::OnCancel);
    JSClass<JSTextPicker>::StaticMethod("onChange", &JSTextPicker::OnChange);
    JSClass<JSTextPicker>::StaticMethod("backgroundColor", &JSTextPicker::PickerBackgroundColor);
    JSClass<JSTextPicker>::StaticMethod("onClick", &JSInteractableView::JsOnClick);
    JSClass<JSTextPicker>::StaticMethod("onTouch", &JSInteractableView::JsOnTouch);
    JSClass<JSTextPicker>::StaticMethod("onKeyEvent", &JSInteractableView::JsOnKey);
    JSClass<JSTextPicker>::StaticMethod("onDeleteEvent", &JSInteractableView::JsOnDelete);
    JSClass<JSTextPicker>::StaticMethod("onAppear", &JSInteractableView::JsOnAppear);
    JSClass<JSTextPicker>::StaticMethod("onDisAppear", &JSInteractableView::JsOnDisAppear);
    JSClass<JSTextPicker>::Inherit<JSViewAbstract>();
    JSClass<JSTextPicker>::Bind(globalObj);
}

void JSTextPicker::PickerBackgroundColor(const JSCallbackInfo& info)
{
    JSViewAbstract::JsBackgroundColor(info);

    if (Container::IsCurrentUseNewPipeline()) {
        if (info.Length() < 1) {
            LOGI("The arg(PickerBackgroundColor) is wrong, it is supposed to have at least 1 argument");
            return;
        }
        Color backgroundColor;
        if (!ParseJsColor(info[0], backgroundColor)) {
            LOGI("the info[0] is null");
            return;
        }
        TextPickerModel::GetInstance()->SetBackgroundColor(backgroundColor);
    }

    auto pickerBase = AceType::DynamicCast<PickerBaseComponent>(ViewStackProcessor::GetInstance()->GetMainComponent());
    if (!pickerBase) {
        LOGE("PickerBaseComponent is null");
        return;
    }

    pickerBase->SetHasBackgroundColor(true);
}

size_t JSTextPicker::ProcessCascadeOptionDepth(const NG::TextCascadePickerOptions& option)
{
    size_t depth = 1;
    if (option.children.size() == 0) {
        return depth;
    }

    for (auto&& pos : option.children) {
        size_t tmpDep = 1;
        tmpDep += ProcessCascadeOptionDepth(pos);
        if (tmpDep > depth) {
            depth = tmpDep;
        }
    }
    return depth;
}

void JSTextPicker::CreateMulti(const RefPtr<PickerTheme>& theme, const std::vector<std::string>& values,
    const std::vector<uint32_t>& selectedValues, bool isCascade,
    const std::vector<NG::TextCascadePickerOptions>& options)
{
    TextPickerModel::GetInstance()->MultiInit(theme);
    TextPickerModel::GetInstance()->SetValues(values);
    TextPickerModel::GetInstance()->SetSelecteds(selectedValues);
    TextPickerModel::GetInstance()->SetIsCascade(isCascade);
    TextPickerModel::GetInstance()->SetColumns(options);
}

void JSTextPicker::Create(const JSCallbackInfo& info)
{
    if (info.Length() >= 1 && info[0]->IsObject()) {
        auto paramObject = JSRef<JSObject>::Cast(info[0]);
        uint32_t selectedValue = 0;
        uint32_t kind = 0;
        std::string value = "";
        std::vector<uint32_t> selectedValues;
        std::vector<std::string> values;
        std::vector<NG::RangeContent> rangeResult;
        std::vector<NG::TextCascadePickerOptions> options;
        bool isCascade = false;
        bool isSingleRange = false;
        bool optionsMultiContentCheckErr = false;
        bool optionsCascadeContentCheckErr = false;
        isSingleRange = ProcessSingleRangeValue(paramObject, rangeResult, kind, selectedValue, value);
        if (!isSingleRange) {
            if (!JSTextPickerParser::ParseMultiTextArray(paramObject, options, selectedValues, values)) {
                options.clear();
                optionsMultiContentCheckErr = true;
            }
            if (optionsMultiContentCheckErr) {
                optionsCascadeContentCheckErr = !ProcessCascadeOptions(paramObject,
                    options, selectedValues, values, isCascade);
            }
        }
        if (!isSingleRange && optionsMultiContentCheckErr && optionsCascadeContentCheckErr) {
            LOGE("parse range all type error.");
            rangeResult.clear();
            options.clear();
            return;
        }
        auto theme = GetTheme<PickerTheme>();
        if (!theme) {
            LOGE("PickerText Theme is null");
            return;
        }
        if (rangeResult.size() > 0) {
            TextPickerModel::GetInstance()->Create(theme, kind);
            TextPickerModel::GetInstance()->SetRange(rangeResult);
            TextPickerModel::GetInstance()->SetSelected(selectedValue);
            TextPickerModel::GetInstance()->SetValue(value);
        } else {
            CreateMulti(theme, values, selectedValues, isCascade, options);
        }
        TextPickerModel::GetInstance()->SetDefaultAttributes(theme);
        JSInteractableView::SetFocusable(false);
        JSInteractableView::SetFocusNode(true);
    }
}

bool JSTextPicker::ProcessSingleRangeValue(const JSRef<JSObject>& paramObjec,
    std::vector<NG::RangeContent>& rangeResult, uint32_t& kind, uint32_t& selectedValue, std::string& value)
{
    bool ret = true;
    auto getRange = paramObjec->GetProperty("range");
    if (getRange->IsNull() || getRange->IsUndefined()) {
        return false;
    }
    if (!JSTextPickerParser::ParseTextArray(paramObjec, rangeResult, kind, selectedValue, value)) {
        if (!JSTextPickerParser::ParseIconTextArray(paramObjec, rangeResult, kind, selectedValue)) {
            LOGI("parse range error.");
            rangeResult.clear();
            ret = false;
        }
    }
    return ret;
}

bool JSTextPicker::ProcessCascadeOptions(const JSRef<JSObject>& paramObject,
    std::vector<NG::TextCascadePickerOptions>& options,
    std::vector<uint32_t>& selectedValues, std::vector<std::string>& values, bool& isCascade)
{
    auto getRange = paramObject->GetProperty("range");
    if (getRange->IsNull() || getRange->IsUndefined()) {
        options.clear();
        LOGE("parse cascade Options error.");
        return false;
    }
    if (!JSTextPickerParser::ParseCascadeTextArray(paramObject, selectedValues, values)) {
        LOGE("parse cascade text error.");
        options.clear();
        return false;
    } else {
        JSRef<JSArray> getRange = paramObject->GetProperty("range");
        JSTextPickerParser::GenerateCascadeOptions(getRange, options);
        uint32_t maxCount = options.empty()? 0 : 1;
        for (size_t i = 0; i < options.size(); i++) {
            size_t tmp  = ProcessCascadeOptionDepth(options[i]);
            if (tmp > maxCount) {
                maxCount = tmp;
            }
        }
        if (selectedValues.size() < maxCount) {
            auto differ = maxCount - selectedValues.size();
            for (uint32_t i = 0; i < differ; i++) {
                selectedValues.emplace_back(0);
            }
        }
        if (values.size() < maxCount) {
            auto differ = maxCount - values.size();
            for (uint32_t i = 0; i < differ; i++) {
                values.emplace_back("");
            }
        }
        isCascade = true;
        TextPickerModel::GetInstance()->SetMaxCount(maxCount);
    }
    return true;
}

bool JSTextPickerParser::GenerateCascadeOptionsInternal(const JSRef<JSObject>& jsObj,
    std::vector<NG::TextCascadePickerOptions>& options)
{
    NG::TextCascadePickerOptions option;
    auto text = jsObj->GetProperty("text");
    option.rangeResult.emplace_back(text->ToString());
    auto children = jsObj->GetProperty("children");
    if (children->IsArray()) {
        JSRef<JSArray> arrayChildren = JSRef<JSArray>::Cast(children);
        if (arrayChildren->Length() > 0) {
            if (!GenerateCascadeOptions(arrayChildren, option.children)) {
                options.clear();
                return false;
            }
        }
    }
    options.emplace_back(option);
    return true;
}

bool JSTextPickerParser::GenerateCascadeOptions(const JSRef<JSArray>& array,
    std::vector<NG::TextCascadePickerOptions>& options)
{
    for (size_t i = 0; i < array->Length(); i++) {
        if (array->GetValueAt(i)->IsObject()) {
            auto jsObj = JSRef<JSObject>::Cast(array->GetValueAt(i));
            if (!GenerateCascadeOptionsInternal(jsObj, options)) {
                return false;
            }
        } else {
            options.clear();
            return false;
        }
    }
    return true;
}

bool JSTextPickerParser::ParseMultiTextArrayRangeInternal(const JSRef<JSVal>& value,
    std::vector<NG::TextCascadePickerOptions>& options)
{
    if (value->IsArray()) {
        std::vector<std::string> rangeResult;
        NG::TextCascadePickerOptions option;
        if (!ParseJsStrArray(value, option.rangeResult)) {
            LOGI("parse str array error.");
            return false;
        } else {
            options.emplace_back(option);
        }
    } else {
        return false;
    }
    return true;
}

bool JSTextPickerParser::ParseMultiTextArrayRange(const JSRef<JSArray>& jsRangeValue,
    std::vector<NG::TextCascadePickerOptions>& options)
{
    options.clear();
    if (jsRangeValue->Length() > 0) {
        for (size_t i = 0; i < jsRangeValue->Length(); i++) {
            JSRef<JSVal> value = jsRangeValue->GetValueAt(i);
            if (!ParseMultiTextArrayRangeInternal(value, options)) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

void JSTextPickerParser::ParseMultiTextArraySelectInternal(const std::vector<NG::TextCascadePickerOptions>& options,
    const std::vector<std::string>& values, std::vector<uint32_t>& selectedValues)
{
    uint32_t selectedValue = 0;
    for (uint32_t i = 0; i < options.size(); i++) {
        if (i > values.size() - 1 || values[i].empty()) {
            selectedValues.emplace_back(0);
            continue;
        }

        auto valueIterator = std::find(options[i].rangeResult.begin(),
            options[i].rangeResult.end(), values[i]);
        if (valueIterator != options[i].rangeResult.end()) {
            selectedValue = std::distance(options[i].rangeResult.begin(), valueIterator);
            selectedValues.emplace_back(selectedValue);
        } else {
            selectedValues.emplace_back(0);
        }
    }
}

void JSTextPickerParser::ParseMultiTextArraySelectArrayInternal(
    const std::vector<NG::TextCascadePickerOptions>& options,
    std::vector<uint32_t>& selectedValues)
{
    for (uint32_t i = 0; i < options.size(); i++) {
        if (i > selectedValues.size() - 1) {
            selectedValues.emplace_back(0);
        } else {
            if (selectedValues[i] < 0 || selectedValues[i] >= options[i].rangeResult.size()) {
                LOGW("selectedValue is out of range");
                selectedValues[i] = 0;
            }
        }
    }
}

bool JSTextPickerParser::ParseMultiTextArraySelect(const JsiRef<JsiValue>& jsSelectedValue,
    std::vector<NG::TextCascadePickerOptions>& options, std::vector<uint32_t>& selectedValues,
    std::vector<std::string>& values)
{
    if (jsSelectedValue->IsArray()) {
        if (!ParseJsIntegerArray(jsSelectedValue, selectedValues)) {
            LOGE("parse selectedValues array error.");
            return false;
        }
        ParseMultiTextArraySelectArrayInternal(options, selectedValues);
    } else {
        uint32_t selectedValue = 0;
        if (ParseJsInteger(jsSelectedValue, selectedValue)) {
            if (options.size() < 1 ||
                selectedValue < 0 || selectedValue >= options[0].rangeResult.size()) {
                LOGW("selectedValue is out of range");
                selectedValue = 0;
            }
            selectedValues.emplace_back(selectedValue);
            for (uint32_t i = 1; i < options.size(); i++) {
                selectedValues.emplace_back(0);
            }
        } else {
            ParseMultiTextArraySelectInternal(options, values, selectedValues);
        }
    }
    return true;
}

void JSTextPickerParser::ParseMultiTextArrayValueInternal(const std::vector<NG::TextCascadePickerOptions>& options,
    std::vector<std::string>& values)
{
    for (uint32_t i = 0; i < options.size(); i++) {
        if (i > values.size() - 1) {
            values.emplace_back("");
        } else {
            auto valueIterator = std::find(options[i].rangeResult.begin(),
                options[i].rangeResult.end(), values[i]);
            if (valueIterator == options[i].rangeResult.end()) {
                values[i] = "";
            }
        }
    }
}

void JSTextPickerParser::ParseMultiTextArrayValueSingleInternal(
    const std::vector<NG::TextCascadePickerOptions>& options, const std::string& value,
    std::vector<std::string>& values)
{
    if (options.size() > 0) {
        auto valueIterator = std::find(options[0].rangeResult.begin(), options[0].rangeResult.end(), value);
        if (valueIterator != options[0].rangeResult.end()) {
            values.emplace_back(value);
        } else {
            values.emplace_back("");
        }
        for (uint32_t i = 1; i < options.size(); i++) {
            values.emplace_back("");
        }
    } else {
        for (uint32_t i = 0; i < options.size(); i++) {
            values.emplace_back("");
        }
    }
}

bool JSTextPickerParser::ParseMultiTextArrayValue(const JsiRef<JsiValue>& jsValue,
    std::vector<NG::TextCascadePickerOptions>& options, std::vector<std::string>& values)
{
    if (jsValue->IsArray()) {
        if (!ParseJsStrArray(jsValue, values)) {
            LOGE("parse value array error.");
            return false;
        }
        ParseMultiTextArrayValueInternal(options, values);
    } else {
        std::string value;
        if (ParseJsString(jsValue, value)) {
            ParseMultiTextArrayValueSingleInternal(options, value, values);
        } else {
            for (uint32_t i = 0; i < options.size(); i++) {
                values.emplace_back("");
            }
        }
    }
    return true;
}

bool JSTextPickerParser::ParseMultiTextArray(const JSRef<JSObject>& paramObject,
    std::vector<NG::TextCascadePickerOptions>& options, std::vector<uint32_t>& selectedValues,
    std::vector<std::string>& values)
{
    auto getSelected = paramObject->GetProperty("selected");
    auto getValue = paramObject->GetProperty("value");
    auto getRange = paramObject->GetProperty("range");
    if (getRange->IsNull() || getRange->IsUndefined()) {
        return false;
    }
    JSRef<JSArray> array = JSRef<JSArray>::Cast(getRange);
    std::vector<NG::TextCascadePickerOptions> getOptionsVector;
    if (!ParseMultiTextArrayRange(array, options)) {
        return false;
    }
    if (!ParseMultiTextArrayValue(getValue, options, values)) {
        return false;
    }
    if (!ParseMultiTextArraySelect(getSelected, options, selectedValues, values)) {
        return false;
    }
    return true;
}

bool JSTextPickerParser::ParseInternalArray(const JSRef<JSArray>& jsRangeValue, std::vector<uint32_t>& selectedValues,
    std::vector<std::string>& values, uint32_t index)
{
    std::vector<std::string> resultStr;
    for (size_t i = 0; i < jsRangeValue->Length(); i++) {
        if (jsRangeValue->GetValueAt(i)->IsObject()) {
            auto jsObj = JSRef<JSObject>::Cast(jsRangeValue->GetValueAt(i));
            auto getText = jsObj->GetProperty("text");
            std::string textStr = "";
            if (ParseJsString(getText, textStr)) {
                resultStr.emplace_back(textStr);
            } else {
                return false;
            }
        }
    }
    if (index > values.size() - 1) {
        values.emplace_back("");
    } else {
        auto valueIterator = std::find(resultStr.begin(), resultStr.end(), values[index]);
        if (valueIterator == resultStr.end()) {
            values[index] = "";
        }
    }

    if (index > selectedValues.size() - 1) {
        selectedValues.emplace_back(0);
    } else {
        if (selectedValues[index] < 0 || selectedValues[index] >= resultStr.size()) {
            LOGW("selectedValue is out of range");
            selectedValues[index] = 0;
        }
    }

    if (selectedValues[index] == 0 && !values[index].empty()) {
        auto valueIterator = std::find(resultStr.begin(), resultStr.end(), values[index]);
        if (valueIterator != resultStr.end()) {
            selectedValues[index] = std::distance(resultStr.begin(), valueIterator);
        }
    }
    auto jsObj = JSRef<JSObject>::Cast(jsRangeValue->GetValueAt(selectedValues[index]));
    auto getChildren = jsObj->GetProperty("children");
    if (getChildren->IsArray()) {
        ParseInternalArray(getChildren, selectedValues, values, index + 1);
    }
    return true;
}

bool JSTextPickerParser::ParseCascadeTextArray(const JSRef<JSObject>& paramObject,
    std::vector<uint32_t>& selectedValues, std::vector<std::string>& values)
{
    JSRef<JSArray> getRange = paramObject->GetProperty("range");
    auto getSelected = paramObject->GetProperty("selected");
    auto getValue = paramObject->GetProperty("value");
    std::vector<NG::TextCascadePickerOptions> getOptionsVector;
    std::vector<std::string> resultStr;

    std::string value = "";
    if (getValue->IsArray()) {
        if (!ParseJsStrArray(getValue, values)) {
            LOGE("parse value array error.");
            return false;
        }
    } else {
        if (!ParseJsString(getValue, value)) {
            value = "";
        }
        values.emplace_back(value);
    }
    uint32_t selectValue = 0;
    if (getSelected->IsArray()) {
        if (!ParseJsIntegerArray(getSelected, selectedValues)) {
            LOGE("parse selectedValues array error.");
            return false;
        }
    } else {
        if (!ParseJsInteger(getSelected, selectValue)) {
            selectValue = 0;
        }
        selectedValues.emplace_back(selectValue);
    }
    return ParseInternalArray(getRange, selectedValues, values, 0);
}

bool JSTextPickerParser::ParseTextArray(const JSRef<JSObject>& paramObject,
    std::vector<NG::RangeContent>& result, uint32_t& kind, uint32_t& selectedValue, std::string& value)
{
    auto getSelected = paramObject->GetProperty("selected");
    auto getValue = paramObject->GetProperty("value");
    JSRef<JSArray> getRange = paramObject->GetProperty("range");
    std::vector<std::string> getRangeVector;
    if (getRange->Length() > 0) {
        if (!ParseJsStrArray(getRange, getRangeVector)) {
            LOGE("parse str array error.");
            return false;
        }

        result.clear();
        for (const auto& text : getRangeVector) {
            NG::RangeContent content;
            content.icon_ = "";
            content.text_ = text;
            result.emplace_back(content);
        }
        kind = NG::TEXT;
        if (!ParseJsString(getValue, value)) {
            value = getRangeVector.front();
        }
        if (!ParseJsInteger(getSelected, selectedValue) && !value.empty()) {
            auto valueIterator = std::find(getRangeVector.begin(), getRangeVector.end(), value);
            if (valueIterator != getRangeVector.end()) {
                selectedValue = std::distance(getRangeVector.begin(), valueIterator);
            }
        }
        if (selectedValue < 0 || selectedValue >= getRangeVector.size()) {
            LOGW("selectedValue is out of range");
            selectedValue = 0;
        }
    }
    
    return true;
}

bool JSTextPickerParser::ParseIconTextArray(const JSRef<JSObject>& paramObject,
    std::vector<NG::RangeContent>& result, uint32_t& kind, uint32_t& selectedValue)
{
    auto getSelected = paramObject->GetProperty("selected");
    auto getRange = paramObject->GetProperty("range");
    if (!getRange->IsArray()) {
        LOGE("arg is not array.");
        return false;
    }
    JSRef<JSArray> array = JSRef<JSArray>::Cast(getRange);
    result.clear();
    kind = 0;
    for (size_t i = 0; i < array->Length(); i++) {
        auto jsObj = JSRef<JSObject>::Cast(array->GetValueAt(i));
        auto rangeIcon = jsObj->GetProperty("icon");
        auto rangeText = jsObj->GetProperty("text");
        NG::RangeContent content;
        std::string icon;
        std::string text;
        if (ParseJsMedia(rangeIcon, icon)) {
            content.icon_ = icon;
            kind |= NG::ICON;
        }

        if (ParseJsString(rangeText, text)) {
            content.text_ = text;
            kind |= NG::TEXT;
        }
        result.emplace_back(content);
    }

    if (kind != NG::ICON && kind != (NG::ICON | NG::TEXT)) {
        LOGE("kind is wrong.");
        return false;
    }

    if (!ParseJsInteger(getSelected, selectedValue)) {
        selectedValue = 0;
    }
    return true;
}

void JSTextPickerParser::ParseTextStyle(const JSRef<JSObject>& paramObj, NG::PickerTextStyle& textStyle)
{
    auto fontColor = paramObj->GetProperty("color");
    auto fontStyle = paramObj->GetProperty("font");

    Color textColor;
    if (ParseJsColor(fontColor, textColor)) {
        textStyle.textColor = textColor;
    }

    if (!fontStyle->IsObject()) {
        LOGE("fontStyle is not obj.");
        return;
    }
    JSRef<JSObject> fontObj = JSRef<JSObject>::Cast(fontStyle);
    auto fontSize = fontObj->GetProperty("size");
    auto fontWeight = fontObj->GetProperty("weight");
    if (fontSize->IsNull() || fontSize->IsUndefined()) {
        textStyle.fontSize = Dimension(-1);
    } else {
        CalcDimension size;
        if (!ParseJsDimensionFp(fontSize, size) || size.Unit() == DimensionUnit::PERCENT) {
            textStyle.fontSize = Dimension(-1);
            LOGW("Parse to dimension FP failed.");
        } else {
            textStyle.fontSize = size;
        }
    }

    if (!fontWeight->IsNull() && !fontWeight->IsUndefined()) {
        std::string weight;
        if (fontWeight->IsNumber()) {
            weight = std::to_string(fontWeight->ToNumber<int32_t>());
        } else {
            ParseJsString(fontWeight, weight);
        }
        textStyle.fontWeight = ConvertStrToFontWeight(weight);
    }
}

void JSTextPicker::SetDefaultPickerItemHeight(const JSCallbackInfo& info)
{
    if (info.Length() < 1) {
        LOGE("The arg is wrong, it is supposed to have atleast 1 argument.");
        return;
    }
    CalcDimension height;
    if (info[0]->IsNumber() || info[0]->IsString()) {
        if (!ParseJsDimensionFp(info[0], height)) {
            return;
        }
    }
    TextPickerModel::GetInstance()->SetDefaultPickerItemHeight(height);
}

void JSTextPicker::SetDisappearTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("PickerText Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSTextPickerParser::ParseTextStyle(info[0], textStyle);
    }
    TextPickerModel::GetInstance()->SetDisappearTextStyle(theme, textStyle);
}

void JSTextPicker::SetTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("PickerText Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSTextPickerParser::ParseTextStyle(info[0], textStyle);
    }
    TextPickerModel::GetInstance()->SetNormalTextStyle(theme, textStyle);
}

void JSTextPicker::SetSelectedTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("PickerText Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSTextPickerParser::ParseTextStyle(info[0], textStyle);
    }
    TextPickerModel::GetInstance()->SetSelectedTextStyle(theme, textStyle);
}

void JSTextPicker::ProcessCascadeSelected(const std::vector<NG::TextCascadePickerOptions>& options,
    uint32_t index, std::vector<uint32_t>& selectedValues)
{
    std::vector<std::string> rangeResultValue;
    NG::TextCascadePickerOptions option;
    for (size_t i = 0; i < options.size(); i++) {
        rangeResultValue.emplace_back(options[i].rangeResult[0]);
    }

    if (index > selectedValues.size() - 1) {
        selectedValues.emplace_back(0);
    }

    if (selectedValues[index] < 0 || selectedValues[index] >= rangeResultValue.size()) {
        LOGW("selectedValue is out of range");
        selectedValues[index] = 0;
    }
    if (selectedValues[index] <= options.size() - 1 && options[selectedValues[index]].children.size() > 0) {
        ProcessCascadeSelected(options[selectedValues[index]].children, index + 1, selectedValues);
    }
}

void JSTextPicker::SetSelectedInternal(uint32_t count, std::vector<uint32_t>& selectedValues)
{
    std::vector<NG::TextCascadePickerOptions> options;
    TextPickerModel::GetInstance()->GetMultiOptions(options);
    for (uint32_t i = 0; i < count; i++) {
        if (i > selectedValues.size() - 1) {
            selectedValues.emplace_back(0);
        } else {
            if (selectedValues[i] < 0 || selectedValues[i] >= options[i].rangeResult.size()) {
                LOGW("selectedValue is out of range");
                selectedValues[i] = 0;
            }
        }
    }
}

void JSTextPicker::SetSelectedIndexMultiInternal(uint32_t count, std::vector<uint32_t>& selectedValues)
{
    if (!TextPickerModel::GetInstance()->IsCascade()) {
        SetSelectedInternal(count, selectedValues);
    } else {
        std::vector<NG::TextCascadePickerOptions> options;
        TextPickerModel::GetInstance()->GetMultiOptions(options);
        ProcessCascadeSelected(options, 0, selectedValues);
        uint32_t maxCount = TextPickerModel::GetInstance()->GetMaxCount();
        if (selectedValues.size() < maxCount) {
            auto differ = maxCount - selectedValues.size();
            for (uint32_t i = 0; i < differ; i++) {
                selectedValues.emplace_back(0);
            }
        }
    }
}

void JSTextPicker::SetSelectedIndexSingleInternal(const std::vector<NG::TextCascadePickerOptions>& options,
    uint32_t count, uint32_t& selectedValue, std::vector<uint32_t>& selectedValues)
{
    if (options.size() > 0) {
        if (selectedValue < 0 || selectedValue >= options[0].rangeResult.size()) {
            LOGW("selectedValue is out of range");
            selectedValue = 0;
        }
        selectedValues.emplace_back(selectedValue);
        for (uint32_t i = 1; i < count; i++) {
            selectedValues.emplace_back(0);
        }
    } else {
        for (uint32_t i = 0; i < count; i++) {
            selectedValues.emplace_back(0);
        }
    }
}

void JSTextPicker::SetSelectedIndexMulti(const JsiRef<JsiValue>& jsSelectedValue)
{
    std::vector<uint32_t> selectedValues;
    std::vector<NG::TextCascadePickerOptions> options;
    TextPickerModel::GetInstance()->GetMultiOptions(options);
    auto count = TextPickerModel::GetInstance()->IsCascade() ?
        TextPickerModel::GetInstance()->GetMaxCount() : options.size();
    if (jsSelectedValue->IsArray()) {
        if (!ParseJsIntegerArray(jsSelectedValue, selectedValues)) {
            LOGE("parse selectedValues array error.");
            for (uint32_t i = 0; i < count; i++) {
                selectedValues.emplace_back(0);
            }
            TextPickerModel::GetInstance()->SetSelecteds(selectedValues);
            return;
        }
        SetSelectedIndexMultiInternal(count, selectedValues);
    } else {
        uint32_t selectedValue = 0;
        if (ParseJsInteger(jsSelectedValue, selectedValue)) {
            SetSelectedIndexSingleInternal(options, count, selectedValue, selectedValues);
        } else {
            for (uint32_t i = 0; i < count; i++) {
                selectedValues.emplace_back(0);
            }
        }
    }
    TextPickerModel::GetInstance()->SetSelecteds(selectedValues);
}

void JSTextPicker::SetSelectedIndexSingle(const JsiRef<JsiValue>& jsSelectedValue)
{
    // Single
    std::vector<uint32_t> selectedValues;
    std::vector<NG::RangeContent> rangeResult;
    TextPickerModel::GetInstance()->GetSingleRange(rangeResult);
    if (jsSelectedValue->IsArray()) {
        if (!ParseJsIntegerArray(jsSelectedValue, selectedValues)) {
            LOGE("SetselectedIndex parse selectedValues array error.");
            uint32_t selectedValue = 0;
            TextPickerModel::GetInstance()->SetSelected(selectedValue);
            return;
        }
        if (selectedValues.size() > 0) {
            if (selectedValues[0] < 0 || selectedValues[0] >= rangeResult.size()) {
                LOGW("selectedValue is out of range");
                selectedValues[0] = 0;
            }
        } else {
            selectedValues.emplace_back(0);
        }

        TextPickerModel::GetInstance()->SetSelected(selectedValues[0]);
    } else {
        uint32_t selectedValue = 0;
        if (ParseJsInteger(jsSelectedValue, selectedValue)) {
            if (selectedValue < 0 || selectedValue >= rangeResult.size()) {
                LOGW("selectedValue is out of range");
                selectedValue = 0;
            }
        }
        TextPickerModel::GetInstance()->SetSelected(selectedValue);
    }
}

void JSTextPicker::SetSelectedIndex(const JSCallbackInfo& info)
{
    if (info.Length() >= 1) {
        auto jsSelectedValue = info[0];
        if (jsSelectedValue->IsNull() || jsSelectedValue->IsUndefined()) {
            return;
        }
        if (TextPickerModel::GetInstance()->IsSingle()) {
            SetSelectedIndexSingle(jsSelectedValue);
        } else {
            SetSelectedIndexMulti(jsSelectedValue);
        }
    }
}

void JSTextPicker::OnAccept(const JSCallbackInfo& info) {}

void JSTextPicker::OnCancel(const JSCallbackInfo& info) {}

void JSTextPicker::OnChange(const JSCallbackInfo& info)
{
    if (Container::IsCurrentUseNewPipeline()) {
        if (!info[0]->IsFunction()) {
            return;
        }
        auto jsFunc = JSRef<JSFunc>::Cast(info[0]);
        auto onChange = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](
                            const std::vector<std::string>& value, const std::vector<double>& index) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("TextPicker.onChange");
            if (value.size() == 1 && index.size() == 1) {
                auto params = ConvertToJSValues(value[0], index[0]);
                func->Call(JSRef<JSObject>(), static_cast<int>(params.size()), params.data());
            } else {
                std::vector<JSRef<JSVal>> result;
                JSRef<JSArray> valueArray = JSRef<JSArray>::New();
                for (uint32_t i = 0; i < value.size(); i++) {
                    valueArray->SetValueAt(i, JSRef<JSVal>::Make(ToJSValue(value[i])));
                }
                JSRef<JSVal> valueJs = JSRef<JSVal>::Cast(valueArray);
                result.emplace_back(valueJs);
                JSRef<JSArray> selectedArray = JSRef<JSArray>::New();
                for (uint32_t i = 0; i < index.size(); i++) {
                    selectedArray->SetValueAt(i, JSRef<JSVal>::Make(ToJSValue(index[i])));
                }
                JSRef<JSVal> selectedJs = JSRef<JSVal>::Cast(selectedArray);
                result.emplace_back(selectedJs);
                func->Call(JSRef<JSObject>(), static_cast<int>(result.size()), result.data());
            }
        };
        TextPickerModel::GetInstance()->SetOnCascadeChange(std::move(onChange));
        info.ReturnSelf();
    } else {
        if (!info[0]->IsFunction()) {
            return;
        }
        auto jsFunc = JSRef<JSFunc>::Cast(info[0]);
        auto onChange = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](
                            const std::string& value, double index) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("TextPicker.onChange");
            auto params = ConvertToJSValues(value, index);
            func->Call(JSRef<JSObject>(), static_cast<int>(params.size()), params.data());
        };
        TextPickerModel::GetInstance()->SetOnChange(std::move(onChange));
        info.ReturnSelf();
    }
}

void JSTextPickerDialog::JSBind(BindingTarget globalObj)
{
    JSClass<JSTextPickerDialog>::Declare("TextPickerDialog");
    JSClass<JSTextPickerDialog>::StaticMethod("show", &JSTextPickerDialog::Show);

    JSClass<JSTextPickerDialog>::Bind<>(globalObj);
}

void JSTextPickerDialog::Show(const JSCallbackInfo& info)
{
    auto scopedDelegate = EngineHelper::GetCurrentDelegate();
    if (!scopedDelegate) {
        // this case usually means there is no foreground container, need to figure out the reason.
        LOGE("scopedDelegate is null, please check");
        return;
    }
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("TextPicker create error, info is non-valid");
        return;
    }

    auto paramObject = JSRef<JSObject>::Cast(info[0]);

    if (Container::IsCurrentUseNewPipeline()) {
        auto dialogEvent = DialogEvent(info);
        auto dialogCancelEvent = DialogCancelEvent(info);
        TextPickerDialogShow(paramObject, dialogEvent, dialogCancelEvent);
        return;
    }

    auto PickerText = AceType::MakeRefPtr<PickerTextComponent>();
    if (!PickerText) {
        LOGE("PickerText Component is null");
        return;
    }
    ParseText(PickerText, paramObject);
    DialogProperties properties {};
    properties.alignment = DialogAlignment::CENTER;
    properties.customComponent = PickerText;
    properties.customStyle = true;
    AddEvent(PickerText, info);
    PickerText->SetDialogName("pickerTextDialog");
    PickerText->OpenDialog(properties);
}

void JSTextPickerDialog::TextPickerDialogShow(const JSRef<JSObject>& paramObj,
    const std::map<std::string, NG::DialogTextEvent>& dialogEvent,
    const std::map<std::string, NG::DialogGestureEvent>& dialogCancelEvent)
{
    auto container = Container::Current();
    if (!container) {
        return;
    }
    auto pipelineContext = AccessibilityManager::DynamicCast<NG::PipelineContext>(container->GetPipelineContext());
    if (!pipelineContext) {
        return;
    }

    auto executor = pipelineContext->GetTaskExecutor();
    if (!executor) {
        return;
    }

    auto theme = JSTextPicker::GetTheme<DialogTheme>();
    if (!theme) {
        LOGE("DialogTheme is null");
        return;
    }

    NG::TextPickerSettingData settingData;
    if (!ParseShowData(paramObj, settingData)) {
        return;
    }

    DialogProperties properties;
    ButtonInfo buttonInfo;
    if (SystemProperties::GetDeviceType() == DeviceType::PHONE) {
        properties.alignment = DialogAlignment::BOTTOM;
    } else {
        properties.alignment = DialogAlignment::CENTER;
    }
    properties.customStyle = false;
    properties.offset = DimensionOffset(Offset(0, -theme->GetMarginBottom().ConvertToPx()));

    auto context = AccessibilityManager::DynamicCast<NG::PipelineContext>(pipelineContext);
    auto overlayManager = context ? context->GetOverlayManager() : nullptr;
    executor->PostTask(
        [properties, settingData, dialogEvent, dialogCancelEvent,
            weak = WeakPtr<NG::OverlayManager>(overlayManager)] {
            auto overlayManager = weak.Upgrade();
            CHECK_NULL_VOID(overlayManager);
            overlayManager->ShowTextDialog(properties, settingData, dialogEvent, dialogCancelEvent);
        },
        TaskExecutor::TaskType::UI);
}

bool JSTextPickerDialog::ParseShowDataOptions(const JSRef<JSObject>& paramObject,
    std::vector<NG::TextCascadePickerOptions>& options, std::vector<uint32_t>& selectedValues,
    std::vector<std::string>& values, bool& isCascade)
{
    bool optionsMultiContentCheckErr = false;
    bool optionsCascadeContentCheckErr = false;
    if (!JSTextPickerParser::ParseMultiTextArray(paramObject, options, selectedValues, values)) {
        LOGI("parse multi text error.");
        options.clear();
        optionsMultiContentCheckErr = true;
    }

    if (optionsMultiContentCheckErr) {
        if (!JSTextPickerParser::ParseCascadeTextArray(paramObject, selectedValues, values)) {
            LOGI("parse cascade text error.");
            options.clear();
            optionsCascadeContentCheckErr = true;
        } else {
            JSRef<JSArray> getRange = paramObject->GetProperty("range");
            JSTextPickerParser::GenerateCascadeOptions(getRange, options);
            isCascade = true;
        }
    }
    if (optionsMultiContentCheckErr && optionsCascadeContentCheckErr) {
        LOGI("parse option error.");
        options.clear();
        return false;
    }
    return true;
}

bool JSTextPickerDialog::ParseShowDataAttribute(const JSRef<JSObject>& paramObject,
    NG::TextPickerSettingData& settingData)
{
    CalcDimension height;
    NG::PickerTextProperties textProperties;
    auto defaultHeight = paramObject->GetProperty("defaultPickerItemHeight");
    if (defaultHeight->IsNumber() || defaultHeight->IsString()) {
        if (!JSViewAbstract::ParseJsDimensionFp(defaultHeight, height)) {
            return false;
        }
    }
    settingData.height = height;
    ParseTextProperties(paramObject, textProperties);
    if (memcpy_s(&settingData.properties, sizeof(NG::PickerTextProperties),
        &textProperties, sizeof(NG::PickerTextProperties)) != EOK) {
        LOGE("memcpy properties error.");
        return false;
    }
    return true;
}

void JSTextPickerDialog::ParseShowDataMultiContent(const std::vector<NG::TextCascadePickerOptions>& options,
    const std::vector<uint32_t>& selectedValues, const std::vector<std::string>& values,
    bool& isCascade, NG::TextPickerSettingData& settingData)
{
    settingData.columnKind = NG::TEXT;
    for (auto& item : selectedValues) {
        settingData.selectedValues.emplace_back(item);
    }
    for (auto& item : values) {
        settingData.values.emplace_back(item);
    }
    for (auto& item : options) {
        settingData.options.emplace_back(item);
    }
    settingData.isCascade = isCascade;
}

bool JSTextPickerDialog::ParseShowData(const JSRef<JSObject>& paramObject, NG::TextPickerSettingData& settingData)
{
    std::vector<NG::RangeContent> rangeResult;
    uint32_t selectedValue = 0;
    uint32_t kind = 0;
    std::string value = "";
    bool rangeContentCheckErr = false;
    bool optionsCascadeContentCheckErr = false;
    std::vector<uint32_t> selectedValues;
    std::vector<std::string> values;
    bool isCascade = false;
    std::vector<NG::TextCascadePickerOptions> options;
    auto getRange = paramObject->GetProperty("range");
    if (getRange->IsNull() || getRange->IsUndefined()) {
        return false;
    }
    if (!JSTextPickerParser::ParseTextArray(paramObject, rangeResult, kind, selectedValue, value)) {
        if (!JSTextPickerParser::ParseIconTextArray(paramObject, rangeResult, kind, selectedValue)) {
            LOGI("parse range error.");
            rangeContentCheckErr = true;
            rangeResult.clear();
        }
    }
    if (rangeContentCheckErr) {
        optionsCascadeContentCheckErr = !ParseShowDataOptions(paramObject, options, selectedValues, values, isCascade);
    }
    if (rangeContentCheckErr && optionsCascadeContentCheckErr) {
        LOGE("parse option all type error.");
        return false;
    }
    if (memset_s(&settingData, sizeof(NG::TextPickerSettingData), 0, sizeof(NG::TextPickerSettingData)) != EOK) {
        LOGE("memset settingData error.");
        return false;
    }
    if (!ParseShowDataAttribute(paramObject, settingData)) {
        return false;
    }
    if (rangeResult.size() > 0) {
        settingData.selected = selectedValue;
        settingData.columnKind = kind;
        for (const auto& item : rangeResult) {
            settingData.rangeVector.emplace_back(item);
        }
    } else {
        ParseShowDataMultiContent(options, selectedValues, values, isCascade, settingData);
    }
    return true;
}

void JSTextPickerDialog::ParseTextProperties(const JSRef<JSObject>& paramObj, NG::PickerTextProperties& result)
{
    auto disappearProperty = paramObj->GetProperty("disappearTextStyle");
    auto normalProperty = paramObj->GetProperty("textStyle");
    auto selectedProperty = paramObj->GetProperty("selectedTextStyle");

    if (!disappearProperty->IsNull() && disappearProperty->IsObject()) {
        JSRef<JSObject> disappearObj = JSRef<JSObject>::Cast(disappearProperty);
        JSTextPickerParser::ParseTextStyle(disappearObj, result.disappearTextStyle_);
    }

    if (!normalProperty->IsNull() && normalProperty->IsObject()) {
        JSRef<JSObject> noramlObj = JSRef<JSObject>::Cast(normalProperty);
        JSTextPickerParser::ParseTextStyle(noramlObj, result.normalTextStyle_);
    }

    if (!selectedProperty->IsNull() && selectedProperty->IsObject()) {
        JSRef<JSObject> selectedObj = JSRef<JSObject>::Cast(selectedProperty);
        JSTextPickerParser::ParseTextStyle(selectedObj, result.selectedTextStyle_);
    }
}

std::map<std::string, NG::DialogTextEvent> JSTextPickerDialog::DialogEvent(const JSCallbackInfo& info)
{
    std::map<std::string, NG::DialogTextEvent> dialogEvent;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("TextPicker AddEvent error, info is non-valid");
        return dialogEvent;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onAccept = paramObject->GetProperty("onAccept");
    if (!onAccept->IsUndefined() && onAccept->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onAccept));
        auto acceptId = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            std::vector<std::string> keys = { "value", "index" };
            ACE_SCORING_EVENT("TextPickerDialog.onAccept");
            func->Execute(keys, info);
        };
        dialogEvent["acceptId"] = acceptId;
    }
    auto onChange = paramObject->GetProperty("onChange");
    if (!onChange->IsUndefined() && onChange->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onChange));
        auto changeId = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            std::vector<std::string> keys = { "value", "index" };
            ACE_SCORING_EVENT("TextPickerDialog.onChange");
            func->Execute(keys, info);
        };
        dialogEvent["changeId"] = changeId;
    }
    return dialogEvent;
}

std::map<std::string, NG::DialogGestureEvent> JSTextPickerDialog::DialogCancelEvent(const JSCallbackInfo& info)
{
    std::map<std::string, NG::DialogGestureEvent> dialogCancelEvent;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("TextPicker AddEvent error, info is non-valid");
        return dialogCancelEvent;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onCancel = paramObject->GetProperty("onCancel");
    if (!onCancel->IsUndefined() && onCancel->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onCancel));
        auto cancelId = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const GestureEvent& /*info*/) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("TextPickerDialog.onCancel");
            func->Execute();
        };
        dialogCancelEvent["cancelId"] = cancelId;
    }
    return dialogCancelEvent;
}

void JSTextPickerDialog::AddEvent(RefPtr<PickerTextComponent>& picker, const JSCallbackInfo& info)
{
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("TextPicker AddEvent error, info is non-valid");
        return;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onAccept = paramObject->GetProperty("onAccept");
    if (!onAccept->IsUndefined() && onAccept->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onAccept));
        auto acceptId =
            EventMarker([execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
                JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
                std::vector<std::string> keys = { "value", "index" };
                ACE_SCORING_EVENT("TextPickerDialog.onAccept");
                func->Execute(keys, info);
            });
        picker->SetDialogAcceptEvent(acceptId);
    }
    auto onCancel = paramObject->GetProperty("onCancel");
    if (!onCancel->IsUndefined() && onCancel->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onCancel));
        auto cancelId = EventMarker([execCtx = info.GetExecutionContext(), func = std::move(jsFunc)]() {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("TextPickerDialog.onCancel");
            func->Execute();
        });
        picker->SetDialogCancelEvent(cancelId);
    }
    auto onChange = paramObject->GetProperty("onChange");
    if (!onChange->IsUndefined() && onChange->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onChange));
        auto changeId =
            EventMarker([execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
                JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
                std::vector<std::string> keys = { "value", "index" };
                ACE_SCORING_EVENT("TextPickerDialog.onChange");
                func->Execute(keys, info);
            });
        picker->SetDialogChangeEvent(changeId);
    }
}

void JSTextPickerDialog::ParseText(RefPtr<PickerTextComponent>& component, const JSRef<JSObject>& paramObj)
{
    auto getSelected = paramObj->GetProperty("selected");
    auto defaultHeight = paramObj->GetProperty("defaultPickerItemHeight");
    JSRef<JSArray> getRange = paramObj->GetProperty("range");
    std::vector<std::string> getRangeVector;
    if (!JSViewAbstract::ParseJsStrArray(getRange, getRangeVector)) {
        LOGE("parse range failed");
        return;
    }

    std::string value = "";
    uint32_t selectedValue = 0;
    auto getValue = paramObj->GetProperty("value");
    if (!JSViewAbstract::ParseJsInteger(getSelected, selectedValue) && JSViewAbstract::ParseJsString(getValue, value)) {
        auto valueIterator = std::find(getRangeVector.begin(), getRangeVector.end(), value);
        if (valueIterator != getRangeVector.end()) {
            selectedValue = std::distance(getRangeVector.begin(), valueIterator);
        }
    }

    if (selectedValue < 0 || selectedValue >= getRangeVector.size()) {
        LOGW("selectedValue is out of range");
        selectedValue = 0;
    }

    CalcDimension height;
    if (defaultHeight->IsNumber() || defaultHeight->IsString()) {
        if (!JSViewAbstract::ParseJsDimensionFp(defaultHeight, height)) {
            return;
        }
    }

    component->SetIsDialog(true);
    component->SetIsCreateDialogComponent(true);
    if (!defaultHeight->IsEmpty()) {
        component->SetColumnHeight(height);
        component->SetDefaultHeight(true);
    }
    component->SetSelected(selectedValue);
    component->SetRange(getRangeVector);
}
} // namespace OHOS::Ace::Framework
