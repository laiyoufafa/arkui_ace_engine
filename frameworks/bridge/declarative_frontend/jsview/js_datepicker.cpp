/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bridge/declarative_frontend/jsview/js_datepicker.h"

#include <utility>

#include "base/log/ace_scoring_log.h"
#include "base/utils/utils.h"
#include "bridge/common/utils/engine_helper.h"
#include "bridge/declarative_frontend/engine/functions/js_function.h"
#include "bridge/declarative_frontend/jsview/js_interactable_view.h"
#include "bridge/declarative_frontend/jsview/js_view_common_def.h"
#include "bridge/declarative_frontend/jsview/models/picker_model_impl.h"
#include "bridge/declarative_frontend/jsview/models/timepicker_model_impl.h"
#include "bridge/declarative_frontend/view_stack_processor.h"
#include "core/components/picker/picker_data.h"
#include "core/components/picker/picker_date_component.h"
#include "core/components/picker/picker_theme.h"
#include "core/components/picker/picker_time_component.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/picker/datepicker_model_ng.h"
#include "core/components_ng/pattern/time_picker/timepicker_model.h"
#include "core/components_ng/pattern/time_picker/timepicker_model_ng.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/event/ace_event_helper.h"
#include "core/pipeline_ng/pipeline_context.h"

namespace OHOS::Ace {
std::unique_ptr<DatePickerModel> DatePickerModel::datePickerInstance_ = nullptr;
std::unique_ptr<TimePickerModel> TimePickerModel::timePickerInstance_ = nullptr;
DatePickerModel* DatePickerModel::GetInstance()
{
    if (!datePickerInstance_) {
#ifdef NG_BUILD
        datePickerInstance_.reset(new NG::DatePickerModelNG());
#else
        if (Container::IsCurrentUseNewPipeline()) {
            datePickerInstance_.reset(new NG::DatePickerModelNG());
        } else {
            datePickerInstance_.reset(new Framework::DatePickerModelImpl());
        }
#endif
    }
    return datePickerInstance_.get();
}

TimePickerModel* TimePickerModel::GetInstance()
{
    if (!timePickerInstance_) {
#ifdef NG_BUILD
        timePickerInstance_.reset(new NG::TimePickerModelNG());
#else
        if (Container::IsCurrentUseNewPipeline()) {
            timePickerInstance_.reset(new NG::TimePickerModelNG());
        } else {
            timePickerInstance_.reset(new Framework::TimePickerModelImpl());
        }
#endif
    }
    return timePickerInstance_.get();
}

} // namespace OHOS::Ace

namespace OHOS::Ace::Framework {
namespace {
void AddEvent(RefPtr<PickerBaseComponent>& picker, const JSCallbackInfo& info, DatePickerType pickerType)
{
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("DatePicker AddEvent error, info is non-valid");
        return;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onAccept = paramObject->GetProperty("onAccept");
    if (!onAccept->IsUndefined() && onAccept->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onAccept));
        auto acceptId =
            EventMarker([execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
                JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
                std::vector<std::string> keys = { "year", "month", "day", "hour", "minute", "second" };
                ACE_SCORING_EVENT("DatePickerDialog.onAccept");
                func->Execute(keys, info);
            });
        picker->SetDialogAcceptEvent(acceptId);
    }
    auto onCancel = paramObject->GetProperty("onCancel");
    if (!onCancel->IsUndefined() && onCancel->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onCancel));
        auto cancelId = EventMarker([execCtx = info.GetExecutionContext(), func = std::move(jsFunc)]() {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("DatePickerDialog.onCancel");
            func->Execute();
        });
        picker->SetDialogCancelEvent(cancelId);
    }
    auto onChange = paramObject->GetProperty("onChange");
    if (!onChange->IsUndefined() && onChange->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onChange));
        auto changeId = EventMarker([execCtx = info.GetExecutionContext(), type = pickerType, func = std::move(jsFunc)](
                                        const std::string& info) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            std::vector<std::string> keys;
            if (type == DatePickerType::DATE) {
                keys = { "year", "month", "day" };
            } else {
                keys = { "hour", "minute" };
            }
            ACE_SCORING_EVENT("DatePickerDialog.onChange");
            func->Execute(keys, info);
        });
        picker->SetDialogChangeEvent(changeId);
    }
}

std::map<std::string, NG::DialogEvent> ChangeDialogEvent(const JSCallbackInfo& info, DatePickerType pickerType)
{
    std::map<std::string, NG::DialogEvent> dialogEvent;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("DatePicker AddEvent error, info is non-valid");
        return dialogEvent;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onChange = paramObject->GetProperty("onChange");
    if (!onChange->IsUndefined() && onChange->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onChange));
        auto changeId = [execCtx = info.GetExecutionContext(), type = pickerType, func = std::move(jsFunc)](
                            const std::string& info) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            std::vector<std::string> keys;
            if (type == DatePickerType::DATE) {
                keys = { "year", "month", "day" };
            } else {
                keys = { "hour", "minute" };
            }
            ACE_SCORING_EVENT("DatePickerDialog.onChange");
            func->Execute(keys, info);
        };
        dialogEvent["changeId"] = changeId;
    }
    auto onAccept = paramObject->GetProperty("onAccept");
    if (!onAccept->IsUndefined() && onAccept->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onAccept));
        auto acceptId = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const std::string& info) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            std::vector<std::string> keys = { "year", "month", "day", "hour", "minute", "second" };
            ACE_SCORING_EVENT("DatePickerDialog.onAccept");
            func->Execute(keys, info);
        };
        dialogEvent["acceptId"] = acceptId;
    }
    return dialogEvent;
}

std::map<std::string, NG::DialogGestureEvent> DialogCancelEvent(const JSCallbackInfo& info)
{
    std::map<std::string, NG::DialogGestureEvent> dialogCancelEvent;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("DatePicker AddEvent error, info is non-valid");
        return dialogCancelEvent;
    }
    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    auto onCancel = paramObject->GetProperty("onCancel");
    if (!onCancel->IsUndefined() && onCancel->IsFunction()) {
        auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(onCancel));
        auto cancelId = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const GestureEvent& /*info*/) {
            JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
            ACE_SCORING_EVENT("DatePickerDialog.onCancel");
            func->Execute();
        };
        dialogCancelEvent["cancelId"] = cancelId;
    }
    return dialogCancelEvent;
}

JSRef<JSVal> DatePickerChangeEventToJSValue(const DatePickerChangeEvent& eventInfo)
{
    JSRef<JSObject> obj = JSRef<JSObject>::New();
    std::unique_ptr<JsonValue> argsPtr = JsonUtil::ParseJsonString(eventInfo.GetSelectedStr());
    if (!argsPtr) {
        LOGW("selectedStr is not exist.");
        return JSRef<JSVal>::Cast(obj);
    }
    std::vector<std::string> keys = { "year", "month", "day", "hour", "minute", "second" };
    for (auto iter = keys.begin(); iter != keys.end(); iter++) {
        const std::string key = *iter;
        const auto value = argsPtr->GetValue(key);
        if (!value || value->ToString().empty()) {
            LOGI("key[%{public}s] is not exist.", key.c_str());
            continue;
        }
        obj->SetProperty<int32_t>(key.c_str(), value->GetInt());
    }
    return JSRef<JSVal>::Cast(obj);
}
} // namespace

void JSDatePicker::JSBind(BindingTarget globalObj)
{
    JSClass<JSDatePicker>::Declare("DatePicker");
    MethodOptions opt = MethodOptions::NONE;
    JSClass<JSDatePicker>::StaticMethod("create", &JSDatePicker::Create, opt);
    JSClass<JSDatePicker>::StaticMethod("lunar", &JSDatePicker::SetLunar);
    JSClass<JSDatePicker>::StaticMethod("onChange", &JSDatePicker::OnChange);
    JSClass<JSDatePicker>::StaticMethod("backgroundColor", &JSDatePicker::PickerBackgroundColor);
    // keep compatible, need remove after
    JSClass<JSDatePicker>::StaticMethod("useMilitaryTime", &JSDatePicker::UseMilitaryTime);
    JSClass<JSDatePicker>::StaticMethod("onClick", &JSInteractableView::JsOnClick);
    JSClass<JSDatePicker>::StaticMethod("onTouch", &JSInteractableView::JsOnTouch);
    JSClass<JSDatePicker>::StaticMethod("onKeyEvent", &JSInteractableView::JsOnKey);
    JSClass<JSDatePicker>::StaticMethod("onDeleteEvent", &JSInteractableView::JsOnDelete);
    JSClass<JSDatePicker>::StaticMethod("onAppear", &JSInteractableView::JsOnAppear);
    JSClass<JSDatePicker>::StaticMethod("onDisAppear", &JSInteractableView::JsOnDisAppear);
    JSClass<JSDatePicker>::StaticMethod("disappearTextStyle", &JSDatePicker::SetDisappearTextStyle);
    JSClass<JSDatePicker>::StaticMethod("textStyle", &JSDatePicker::SetTextStyle);
    JSClass<JSDatePicker>::StaticMethod("selectedTextStyle", &JSDatePicker::SetSelectedTextStyle);
    JSClass<JSDatePicker>::Inherit<JSViewAbstract>();
    JSClass<JSDatePicker>::Bind(globalObj);
}

void JSDatePicker::Create(const JSCallbackInfo& info)
{
    DatePickerType pickerType = DatePickerType::DATE;
    JSRef<JSObject> paramObject;
    if (info.Length() >= 1 && info[0]->IsObject()) {
        paramObject = JSRef<JSObject>::Cast(info[0]);
        auto type = paramObject->GetProperty("type");
        if (type->IsNumber()) {
            pickerType = static_cast<DatePickerType>(type->ToNumber<int32_t>());
        }
    }
    switch (pickerType) {
        case DatePickerType::TIME: {
            CreateTimePicker(info, paramObject);
            break;
        }
        case DatePickerType::DATE: {
            CreateDatePicker(info, paramObject);
            break;
        }
        default: {
            LOGE("Undefined date picker type.");
            break;
        }
    }
}

void JSDatePicker::SetLunar(bool isLunar)
{
    DatePickerModel::GetInstance()->SetShowLunar(isLunar);
}

void JSDatePicker::UseMilitaryTime(bool isUseMilitaryTime)
{
    DatePickerModel::GetInstance()->SetHour24(isUseMilitaryTime);
}

void JSDatePicker::ParseTextProperties(const JSRef<JSObject>& paramObj, NG::PickerTextProperties& result)
{
    auto disappearProperty = paramObj->GetProperty("disappearTextStyle");
    auto normalProperty = paramObj->GetProperty("textStyle");
    auto selectedProperty = paramObj->GetProperty("selectedTextStyle");

    if (!disappearProperty->IsNull() && disappearProperty->IsObject()) {
        JSRef<JSObject> disappearObj = JSRef<JSObject>::Cast(disappearProperty);
        JSDatePicker::ParseTextStyle(disappearObj, result.disappearTextStyle_);
    }

    if (!normalProperty->IsNull() && normalProperty->IsObject()) {
        JSRef<JSObject> noramlObj = JSRef<JSObject>::Cast(normalProperty);
        JSDatePicker::ParseTextStyle(noramlObj, result.normalTextStyle_);
    }

    if (!selectedProperty->IsNull() && selectedProperty->IsObject()) {
        JSRef<JSObject> selectedObj = JSRef<JSObject>::Cast(selectedProperty);
        JSDatePicker::ParseTextStyle(selectedObj, result.selectedTextStyle_);
    }
}

void JSDatePicker::ParseTextStyle(const JSRef<JSObject>& paramObj, NG::PickerTextStyle& textStyle)
{
    auto fontColor = paramObj->GetProperty("color");
    auto fontStyle = paramObj->GetProperty("font");

    Color textColor;
    if (JSViewAbstract::ParseJsColor(fontColor, textColor)) {
        textStyle.textColor = textColor;
    }

    if (!fontStyle->IsObject()) {
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

void JSDatePicker::SetDisappearTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("datePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    DatePickerModel::GetInstance()->SetDisappearTextStyle(theme, textStyle);
}

void JSDatePicker::SetTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("datePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    DatePickerModel::GetInstance()->SetNormalTextStyle(theme, textStyle);
}

void JSDatePicker::SetSelectedTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("datePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    DatePickerModel::GetInstance()->SetSelectedTextStyle(theme, textStyle);
}

void JSDatePicker::OnChange(const JSCallbackInfo& info)
{
    if (info.Length() < 1 || !info[0]->IsFunction()) {
        LOGI("info not function");
        return;
    }

    auto jsFunc = AceType::MakeRefPtr<JsEventFunction<DatePickerChangeEvent, 1>>(
        JSRef<JSFunc>::Cast(info[0]), DatePickerChangeEventToJSValue);
    auto onChange = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const BaseEventInfo* info) {
        JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
        ACE_SCORING_EVENT("datePicker.onChange");
        const auto* eventInfo = TypeInfoHelper::DynamicCast<DatePickerChangeEvent>(info);
        func->Execute(*eventInfo);
    };
    DatePickerModel::GetInstance()->SetOnChange(std::move(onChange));
}

void JSTimePicker::OnChange(const JSCallbackInfo& info)
{
    if (info.Length() < 1 || !info[0]->IsFunction()) {
        LOGI("info not function");
        return;
    }

    auto jsFunc = AceType::MakeRefPtr<JsEventFunction<DatePickerChangeEvent, 1>>(
        JSRef<JSFunc>::Cast(info[0]), DatePickerChangeEventToJSValue);
    auto onChange = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const BaseEventInfo* index) {
        JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
        ACE_SCORING_EVENT("datePicker.onChange");
        const auto* eventInfo = TypeInfoHelper::DynamicCast<DatePickerChangeEvent>(index);
        func->Execute(*eventInfo);
    };
    TimePickerModel::GetInstance()->SetOnChange(std::move(onChange));
}

void JSDatePicker::PickerBackgroundColor(const JSCallbackInfo& info)
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
        DatePickerModel::GetInstance()->SetBackgroundColor(backgroundColor);
    }

    auto pickerBase = AceType::DynamicCast<PickerBaseComponent>(ViewStackProcessor::GetInstance()->GetMainComponent());
    if (!pickerBase) {
        LOGE("PickerBaseComponent is null");
        return;
    }

    pickerBase->SetHasBackgroundColor(true);
}

PickerDate JSDatePicker::ParseDate(const JSRef<JSVal>& dateVal)
{
    auto pickerDate = PickerDate();
    if (!dateVal->IsObject()) {
        return pickerDate;
    }
    auto dateObj = JSRef<JSObject>::Cast(dateVal);
    auto yearFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getFullYear"));
    auto monthFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getMonth"));
    auto dateFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getDate"));
    JSRef<JSVal> year = yearFunc->Call(dateObj);
    JSRef<JSVal> month = monthFunc->Call(dateObj);
    JSRef<JSVal> date = dateFunc->Call(dateObj);

    if (year->IsNumber() && month->IsNumber() && date->IsNumber()) {
        pickerDate.SetYear(year->ToNumber<int32_t>());
        pickerDate.SetMonth(month->ToNumber<int32_t>() + 1); // 0-11 means 1 to 12 months
        pickerDate.SetDay(date->ToNumber<int32_t>());
    }
    return pickerDate;
}

PickerTime JSDatePicker::ParseTime(const JSRef<JSVal>& timeVal)
{
    auto pickerTime = PickerTime();
    if (!timeVal->IsObject()) {
        return pickerTime;
    }
    auto timeObj = JSRef<JSObject>::Cast(timeVal);
    auto hourFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getHours"));
    auto minuteFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getMinutes"));
    auto secondFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getSeconds"));
    JSRef<JSVal> hour = hourFunc->Call(timeObj);
    JSRef<JSVal> minute = minuteFunc->Call(timeObj);
    JSRef<JSVal> second = secondFunc->Call(timeObj);

    if (hour->IsNumber() && minute->IsNumber() && second->IsNumber()) {
        pickerTime.SetHour(hour->ToNumber<int32_t>());
        pickerTime.SetMinute(minute->ToNumber<int32_t>());
        pickerTime.SetSecond(second->ToNumber<int32_t>());
    }
    return pickerTime;
}

void ParseSelectedDateTimeObject(const JSCallbackInfo& info, const JSRef<JSObject>& selectedObject, bool isDatePicker)
{
    JSRef<JSVal> changeEventVal = selectedObject->GetProperty("changeEvent");
    auto jsFunc = AceType::MakeRefPtr<JsFunction>(JSRef<JSObject>(), JSRef<JSFunc>::Cast(changeEventVal));
    auto changeEvent = [execCtx = info.GetExecutionContext(), func = std::move(jsFunc)](const BaseEventInfo* info) {
        JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(execCtx);
        ACE_SCORING_EVENT("DatePicker.SelectedDateTimeChangeEvent");
        const auto* eventInfo = TypeInfoHelper::DynamicCast<DatePickerChangeEvent>(info);
        CHECK_NULL_VOID(eventInfo);
        auto selectedStr = eventInfo->GetSelectedStr();
        auto sourceJson = JsonUtil::ParseJsonString(selectedStr);
        if (!sourceJson || sourceJson->IsNull()) {
            LOGE("invalid selected date value.");
            return;
        }

        std::tm dateTime = { 0 };
        auto year = sourceJson->GetValue("year");
        if (year && year->IsNumber()) {
            dateTime.tm_year = year->GetInt() - 1900; // local date start from 1900
        }
        auto month = sourceJson->GetValue("month");
        if (month && month->IsNumber()) {
            dateTime.tm_mon = month->GetInt();
        }
        auto day = sourceJson->GetValue("day");
        if (day && day->IsNumber()) {
            dateTime.tm_mday = day->GetInt();
        }
        auto hour = sourceJson->GetValue("hour");
        if (hour && hour->IsNumber()) {
            dateTime.tm_hour = hour->GetInt();
        }
        auto minute = sourceJson->GetValue("minute");
        if (minute && minute->IsNumber()) {
            dateTime.tm_min = minute->GetInt();
        }
        auto milliseconds = Date::GetMilliSecondsByDateTime(dateTime);
        auto dateObj = JSDate::New(milliseconds);
        func->ExecuteJS(1, &dateObj);
    };
    if (isDatePicker) {
        DatePickerModel::GetInstance()->SetChangeEvent(std::move(changeEvent));
    } else {
        TimePickerModel::GetInstance()->SetChangeEvent(std::move(changeEvent));
    }
}

void JSDatePicker::CreateDatePicker(const JSCallbackInfo& info, const JSRef<JSObject>& paramObj)
{
    JSRef<JSVal> startDate;
    JSRef<JSVal> endDate;
    JSRef<JSVal> selectedDate;
    if (!paramObj->IsUndefined()) {
        startDate = paramObj->GetProperty("start");
        endDate = paramObj->GetProperty("end");
        selectedDate = paramObj->GetProperty("selected");
    }
    auto parseStartDate = ParseDate(startDate);
    auto parseEndDate = ParseDate(endDate);
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("datePicker Theme is null");
        return;
    }
    DatePickerModel::GetInstance()->CreateDatePicker(theme);
    if (startDate->IsObject()) {
        DatePickerModel::GetInstance()->SetStartDate(parseStartDate);
    }
    if (endDate->IsObject()) {
        DatePickerModel::GetInstance()->SetEndDate(parseEndDate);
    }
    if (selectedDate->IsObject()) {
        JSRef<JSObject> selectedDateObj = JSRef<JSObject>::Cast(selectedDate);
        JSRef<JSVal> changeEventVal = selectedDateObj->GetProperty("changeEvent");
        PickerDate parseSelectedDate;
        if (!changeEventVal->IsUndefined() && changeEventVal->IsFunction()) {
            ParseSelectedDateTimeObject(info, selectedDateObj, true);
            parseSelectedDate = ParseDate(selectedDateObj->GetProperty("value"));
        } else {
            parseSelectedDate = ParseDate(selectedDate);
        }
        auto startDays = parseStartDate.ToDays();
        auto endDays = parseEndDate.ToDays();
        auto selectedDays = parseSelectedDate.ToDays();
        if (startDays > endDays || selectedDays < startDays || selectedDays > endDays) {
            LOGE("date error");
        }
        DatePickerModel::GetInstance()->SetSelectedDate(parseSelectedDate);
    }
    SetDefaultAttributes();
}

void JSDatePicker::SetDefaultAttributes()
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("datePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    auto selectedStyle = theme->GetOptionStyle(true, false);
    textStyle.textColor = selectedStyle.GetTextColor();
    textStyle.fontSize = selectedStyle.GetFontSize();
    textStyle.fontWeight = selectedStyle.GetFontWeight();
    DatePickerModel::GetInstance()->SetSelectedTextStyle(theme, textStyle);

    auto disappearStyle = theme->GetDisappearOptionStyle();
    textStyle.textColor = disappearStyle.GetTextColor();
    textStyle.fontSize = disappearStyle.GetFontSize();
    textStyle.fontWeight = disappearStyle.GetFontWeight();
    DatePickerModel::GetInstance()->SetDisappearTextStyle(theme, textStyle);

    auto normalStyle = theme->GetOptionStyle(false, false);
    textStyle.textColor = normalStyle.GetTextColor();
    textStyle.fontSize = normalStyle.GetFontSize();
    textStyle.fontWeight = normalStyle.GetFontWeight();
    DatePickerModel::GetInstance()->SetNormalTextStyle(theme, textStyle);
}

void JSDatePicker::CreateTimePicker(const JSCallbackInfo& info, const JSRef<JSObject>& paramObj)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    DatePickerModel::GetInstance()->CreateTimePicker(theme);
    auto selectedTime = paramObj->GetProperty("selected");
    if (selectedTime->IsObject()) {
        JSRef<JSObject> selectedTimeObj = JSRef<JSObject>::Cast(selectedTime);
        JSRef<JSVal> changeEventVal = selectedTimeObj->GetProperty("changeEvent");
        if (!changeEventVal->IsUndefined() && changeEventVal->IsFunction()) {
            ParseSelectedDateTimeObject(info, selectedTimeObj, true);
            auto parseSelectedTime = ParseTime(selectedTimeObj->GetProperty("value"));
            DatePickerModel::GetInstance()->SetSelectedTime(parseSelectedTime);
        } else {
            DatePickerModel::GetInstance()->SetSelectedTime(ParseTime(selectedTime));
        }
    }
}

void JSDatePickerDialog::JSBind(BindingTarget globalObj)
{
    JSClass<JSDatePickerDialog>::Declare("DatePickerDialog");
    JSClass<JSDatePickerDialog>::StaticMethod("show", &JSDatePickerDialog::Show);

    JSClass<JSDatePickerDialog>::Bind<>(globalObj);
}

void JSDatePickerDialog::Show(const JSCallbackInfo& info)
{
    auto scopedDelegate = EngineHelper::GetCurrentDelegate();
    if (!scopedDelegate) {
        // this case usually means there is no foreground container, need to figure out the reason.
        LOGE("scopedDelegate is null, please check");
        return;
    }
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("DatePicker Show dialog error, info is non-valid");
        return;
    }

    auto paramObject = JSRef<JSObject>::Cast(info[0]);
    DatePickerType pickerType = DatePickerType::DATE;
    auto type = paramObject->GetProperty("type");
    if (type->IsNumber()) {
        pickerType = static_cast<DatePickerType>(type->ToNumber<int32_t>());
    }
    if (Container::IsCurrentUseNewPipeline()) {
        auto dialogEvent = ChangeDialogEvent(info, DatePickerType::DATE);
        auto dialogCancelEvent = DialogCancelEvent(info);
        DatePickerDialogShow(paramObject, dialogEvent, dialogCancelEvent);
        return;
    }

    std::string name;
    RefPtr<Component> component;
    switch (pickerType) {
        case DatePickerType::TIME: {
            CreateTimePicker(component, paramObject);
            name = "TimePickerDialog";
            break;
        }
        case DatePickerType::DATE: {
            CreateDatePicker(component, paramObject);
            name = "DatePickerDialog";
            break;
        }
        default: {
            LOGE("Undefined date picker type.");
            return;
        }
    }

    auto datePicker = AceType::DynamicCast<PickerBaseComponent>(component);
    DialogProperties properties {};
    properties.alignment = DialogAlignment::CENTER;
    properties.customComponent = datePicker;
    properties.customStyle = true;
    if (pickerType == DatePickerType::DATE) {
        AddEvent(datePicker, info, DatePickerType::DATE);
    } else {
        AddEvent(datePicker, info, DatePickerType::TIME);
    }
    datePicker->SetDialogName(name);
    datePicker->OpenDialog(properties);
}

void JSDatePickerDialog::DatePickerDialogShow(const JSRef<JSObject>& paramObj,
    const std::map<std::string, NG::DialogEvent>& dialogEvent,
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

    NG::DatePickerSettingData settingData;
    auto startDate = paramObj->GetProperty("start");
    auto endDate = paramObj->GetProperty("end");
    auto selectedDate = paramObj->GetProperty("selected");
    auto lunar = paramObj->GetProperty("lunar");
    auto sTime = paramObj->GetProperty("showTime");
    auto useMilitary = paramObj->GetProperty("useMilitaryTime");
    settingData.isLunar = lunar->ToBoolean();
    settingData.showTime = sTime->ToBoolean();
    settingData.useMilitary = useMilitary->ToBoolean();
    auto parseStartDate = ParseDate(startDate);
    auto parseEndDate = ParseDate(endDate);
    auto parseSelectedDate = ParseDate(selectedDate);
    auto startDays = parseStartDate.ToDays();
    auto endDays = parseEndDate.ToDays();
    auto selectedDays = parseSelectedDate.ToDays();
    if (startDays > endDays || selectedDays < startDays || selectedDays > endDays) {
        LOGE("date error");
    }

    auto theme = GetTheme<DialogTheme>();
    if (!theme) {
        LOGE("DialogTheme is null");
        return;
    }

    ButtonInfo buttonInfo;
    DialogProperties properties;
    if (SystemProperties::GetDeviceType() == DeviceType::PHONE) {
        properties.alignment = DialogAlignment::BOTTOM;
    } else {
        properties.alignment = DialogAlignment::CENTER;
    }
    properties.customStyle = false;
    properties.offset = DimensionOffset(Offset(0, -theme->GetMarginBottom().ConvertToPx()));

    std::map<std::string, PickerDate> datePickerProperty;
    std::map<std::string, PickerTime> timePickerProperty;
    if (startDate->IsObject()) {
        settingData.datePickerProperty["start"] = parseStartDate;
    }
    if (endDate->IsObject()) {
        settingData.datePickerProperty["end"] = parseEndDate;
    }
    if (selectedDate->IsObject()) {
        settingData.datePickerProperty["selected"] = parseSelectedDate;
        settingData.timePickerProperty["selected"] = ParseTime(selectedDate);
    }

    JSDatePicker::ParseTextProperties(paramObj, settingData.properties);
    auto context = AccessibilityManager::DynamicCast<NG::PipelineContext>(pipelineContext);
    auto overlayManager = context ? context->GetOverlayManager() : nullptr;
    executor->PostTask(
        [properties, settingData, dialogEvent, dialogCancelEvent,
            weak = WeakPtr<NG::OverlayManager>(overlayManager)] {
            auto overlayManager = weak.Upgrade();
            CHECK_NULL_VOID(overlayManager);
            overlayManager->ShowDateDialog(properties, settingData, dialogEvent, dialogCancelEvent);
        },
        TaskExecutor::TaskType::UI);
}

void JSDatePickerDialog::CreateDatePicker(RefPtr<Component>& component, const JSRef<JSObject>& paramObj)
{
    auto datePicker = AceType::MakeRefPtr<PickerDateComponent>();
    auto startDate = paramObj->GetProperty("start");
    auto endDate = paramObj->GetProperty("end");
    auto selectedDate = paramObj->GetProperty("selected");
    auto lunar = paramObj->GetProperty("lunar");
    bool isLunar = lunar->ToBoolean();
    auto parseStartDate = ParseDate(startDate);
    auto parseEndDate = ParseDate(endDate);
    auto parseSelectedDate = ParseDate(selectedDate);
    auto startDays = parseStartDate.ToDays();
    auto endDays = parseEndDate.ToDays();
    auto selectedDays = parseSelectedDate.ToDays();
    if (startDays > endDays || selectedDays < startDays || selectedDays > endDays) {
        LOGE("date error");
    }
    if (startDate->IsObject()) {
        datePicker->SetStartDate(parseStartDate);
    }
    if (endDate->IsObject()) {
        datePicker->SetEndDate(parseEndDate);
    }
    if (selectedDate->IsObject()) {
        datePicker->SetSelectedDate(parseSelectedDate);
    }
    datePicker->SetIsDialog(true);
    datePicker->SetIsCreateDialogComponent(true);
    datePicker->SetShowLunar(isLunar);

    component = datePicker;
}

void JSDatePickerDialog::CreateTimePicker(RefPtr<Component>& component, const JSRef<JSObject>& paramObj)
{
    auto timePicker = AceType::MakeRefPtr<PickerTimeComponent>();
    auto selectedTime = paramObj->GetProperty("selected");
    auto useMilitaryTime = paramObj->GetProperty("useMilitaryTime");
    bool isUseMilitaryTime = useMilitaryTime->ToBoolean();
    if (selectedTime->IsObject()) {
        timePicker->SetSelectedTime(ParseTime(selectedTime));
    }
    timePicker->SetIsDialog(true);
    timePicker->SetIsCreateDialogComponent(true);
    timePicker->SetHour24(isUseMilitaryTime);
    component = timePicker;
}

PickerDate JSDatePickerDialog::ParseDate(const JSRef<JSVal>& dateVal)
{
    auto pickerDate = PickerDate();
    if (!dateVal->IsObject()) {
        return pickerDate;
    }
    auto dateObj = JSRef<JSObject>::Cast(dateVal);
    auto yearFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getFullYear"));
    auto monthFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getMonth"));
    auto dateFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getDate"));
    JSRef<JSVal> year = yearFunc->Call(dateObj);
    JSRef<JSVal> month = monthFunc->Call(dateObj);
    JSRef<JSVal> date = dateFunc->Call(dateObj);

    if (year->IsNumber() && month->IsNumber() && date->IsNumber()) {
        pickerDate.SetYear(year->ToNumber<int32_t>());
        pickerDate.SetMonth(month->ToNumber<int32_t>() + 1); // 0-11 means 1 to 12 months
        pickerDate.SetDay(date->ToNumber<int32_t>());
    }
    return pickerDate;
}

PickerTime JSDatePickerDialog::ParseTime(const JSRef<JSVal>& timeVal)
{
    auto pickerTime = PickerTime();
    if (!timeVal->IsObject()) {
        return pickerTime;
    }
    auto timeObj = JSRef<JSObject>::Cast(timeVal);
    auto hourFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getHours"));
    auto minuteFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getMinutes"));
    auto secondFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getSeconds"));
    JSRef<JSVal> hour = hourFunc->Call(timeObj);
    JSRef<JSVal> minute = minuteFunc->Call(timeObj);
    JSRef<JSVal> second = secondFunc->Call(timeObj);

    if (hour->IsNumber() && minute->IsNumber() && second->IsNumber()) {
        pickerTime.SetHour(hour->ToNumber<int32_t>());
        pickerTime.SetMinute(minute->ToNumber<int32_t>());
        pickerTime.SetSecond(second->ToNumber<int32_t>());
    }
    return pickerTime;
}

void JSTimePicker::JSBind(BindingTarget globalObj)
{
    JSClass<JSTimePicker>::Declare("TimePicker");
    MethodOptions opt = MethodOptions::NONE;
    JSClass<JSTimePicker>::StaticMethod("create", &JSTimePicker::Create, opt);
    JSClass<JSTimePicker>::StaticMethod("onChange", &JSTimePicker::OnChange);
    JSClass<JSTimePicker>::StaticMethod("backgroundColor", &JSTimePicker::PickerBackgroundColor);
    JSClass<JSTimePicker>::StaticMethod("useMilitaryTime", &JSTimePicker::UseMilitaryTime);
    JSClass<JSTimePicker>::StaticMethod("onClick", &JSInteractableView::JsOnClick);
    JSClass<JSTimePicker>::StaticMethod("onTouch", &JSInteractableView::JsOnTouch);
    JSClass<JSTimePicker>::StaticMethod("onKeyEvent", &JSInteractableView::JsOnKey);
    JSClass<JSTimePicker>::StaticMethod("onDeleteEvent", &JSInteractableView::JsOnDelete);
    JSClass<JSTimePicker>::StaticMethod("onAppear", &JSInteractableView::JsOnAppear);
    JSClass<JSTimePicker>::StaticMethod("onDisAppear", &JSInteractableView::JsOnDisAppear);
    JSClass<JSTimePicker>::StaticMethod("disappearTextStyle", &JSTimePicker::SetDisappearTextStyle);
    JSClass<JSTimePicker>::StaticMethod("textStyle", &JSTimePicker::SetTextStyle);
    JSClass<JSTimePicker>::StaticMethod("selectedTextStyle", &JSTimePicker::SetSelectedTextStyle);
    JSClass<JSTimePicker>::Inherit<JSViewAbstract>();
    JSClass<JSTimePicker>::Bind(globalObj);
}

void JSTimePicker::Create(const JSCallbackInfo& info)
{
    JSRef<JSObject> paramObject = JSRef<JSObject>::New();
    if (info.Length() >= 1 && info[0]->IsObject()) {
        paramObject = JSRef<JSObject>::Cast(info[0]);
    }
    CreateTimePicker(info, paramObject);
}

void JSTimePicker::UseMilitaryTime(bool isUseMilitaryTime)
{
    TimePickerModel::GetInstance()->SetHour24(isUseMilitaryTime);
}

void JSTimePicker::PickerBackgroundColor(const JSCallbackInfo& info)
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
        TimePickerModel::GetInstance()->SetBackgroundColor(backgroundColor);
    }

    auto pickerBase = AceType::DynamicCast<PickerBaseComponent>(ViewStackProcessor::GetInstance()->GetMainComponent());
    if (!pickerBase) {
        LOGE("PickerBaseComponent is null");
        return;
    }

    pickerBase->SetHasBackgroundColor(true);
}

void JSTimePicker::SetDisappearTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    TimePickerModel::GetInstance()->SetDisappearTextStyle(theme, textStyle);
}

void JSTimePicker::SetTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    TimePickerModel::GetInstance()->SetNormalTextStyle(theme, textStyle);
}

void JSTimePicker::SetSelectedTextStyle(const JSCallbackInfo& info)
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("The arg is wrong, it is supposed to have at least 1 argument");
    } else {
        JSDatePicker::ParseTextStyle(info[0], textStyle);
    }
    TimePickerModel::GetInstance()->SetSelectedTextStyle(theme, textStyle);
}

void JSTimePicker::CreateTimePicker(const JSCallbackInfo& info, const JSRef<JSObject>& paramObj)
{
    auto selectedTime = paramObj->GetProperty("selected");
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    TimePickerModel::GetInstance()->CreateTimePicker(theme);
    if (selectedTime->IsObject()) {
        JSRef<JSObject> selectedTimeObj = JSRef<JSObject>::Cast(selectedTime);
        JSRef<JSVal> changeEventVal = selectedTimeObj->GetProperty("changeEvent");
        if (!changeEventVal->IsUndefined() && changeEventVal->IsFunction()) {
            ParseSelectedDateTimeObject(info, selectedTimeObj, false);
            auto parseSelectedTime = ParseTime(selectedTimeObj->GetProperty("value"));
            TimePickerModel::GetInstance()->SetSelectedTime(parseSelectedTime);
        } else {
            TimePickerModel::GetInstance()->SetSelectedTime(ParseTime(selectedTime));
        }
    }
    SetDefaultAttributes();
}

void JSTimePicker::SetDefaultAttributes()
{
    auto theme = GetTheme<PickerTheme>();
    if (!theme) {
        LOGE("timePicker Theme is null");
        return;
    }
    NG::PickerTextStyle textStyle;
    auto selectedStyle = theme->GetOptionStyle(true, false);
    textStyle.textColor = selectedStyle.GetTextColor();
    textStyle.fontSize = selectedStyle.GetFontSize();
    textStyle.fontWeight = selectedStyle.GetFontWeight();
    TimePickerModel::GetInstance()->SetSelectedTextStyle(theme, textStyle);

    auto disappearStyle = theme->GetDisappearOptionStyle();
    textStyle.textColor = disappearStyle.GetTextColor();
    textStyle.fontSize = disappearStyle.GetFontSize();
    textStyle.fontWeight = disappearStyle.GetFontWeight();
    TimePickerModel::GetInstance()->SetDisappearTextStyle(theme, textStyle);

    auto normalStyle = theme->GetOptionStyle(false, false);
    textStyle.textColor = normalStyle.GetTextColor();
    textStyle.fontSize = normalStyle.GetFontSize();
    textStyle.fontWeight = normalStyle.GetFontWeight();
    TimePickerModel::GetInstance()->SetNormalTextStyle(theme, textStyle);
}

PickerTime JSTimePicker::ParseTime(const JSRef<JSVal>& timeVal)
{
    auto pickerTime = PickerTime();
    if (!timeVal->IsObject()) {
        return pickerTime;
    }
    auto timeObj = JSRef<JSObject>::Cast(timeVal);
    auto hourFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getHours"));
    auto minuteFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getMinutes"));
    auto secondFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getSeconds"));
    JSRef<JSVal> hour = hourFunc->Call(timeObj);
    JSRef<JSVal> minute = minuteFunc->Call(timeObj);
    JSRef<JSVal> second = secondFunc->Call(timeObj);

    if (hour->IsNumber() && minute->IsNumber() && second->IsNumber()) {
        pickerTime.SetHour(hour->ToNumber<int32_t>());
        pickerTime.SetMinute(minute->ToNumber<int32_t>());
        pickerTime.SetSecond(second->ToNumber<int32_t>());
    }
    return pickerTime;
}

void JSTimePickerDialog::JSBind(BindingTarget globalObj)
{
    JSClass<JSTimePickerDialog>::Declare("TimePickerDialog");
    JSClass<JSTimePickerDialog>::StaticMethod("show", &JSTimePickerDialog::Show);

    JSClass<JSTimePickerDialog>::Bind<>(globalObj);
}

void JSTimePickerDialog::Show(const JSCallbackInfo& info)
{
    auto scopedDelegate = EngineHelper::GetCurrentDelegate();
    if (!scopedDelegate) {
        // this case usually means there is no foreground container, need to figure out the reason.
        LOGE("scopedDelegate is null, please check");
        return;
    }
    if (info.Length() < 1 || !info[0]->IsObject()) {
        LOGE("DatePicker Show dialog error, info is non-valid");
        return;
    }

    auto paramObject = JSRef<JSObject>::Cast(info[0]);

    if (Container::IsCurrentUseNewPipeline()) {
        auto dialogEvent = ChangeDialogEvent(info, DatePickerType::TIME);
        auto dialogCancelEvent = DialogCancelEvent(info);
        TimePickerDialogShow(paramObject, dialogEvent, dialogCancelEvent);
        return;
    }

    RefPtr<Component> component;
    CreateTimePicker(component, paramObject);

    auto datePicker = AceType::DynamicCast<PickerBaseComponent>(component);
    DialogProperties properties {};
    properties.alignment = DialogAlignment::CENTER;
    properties.customComponent = datePicker;
    properties.customStyle = true;
    AddEvent(datePicker, info, DatePickerType::TIME);
    datePicker->SetDialogName("TimePickerDialog");
    datePicker->OpenDialog(properties);
}

void JSTimePickerDialog::TimePickerDialogShow(const JSRef<JSObject>& paramObj,
    const std::map<std::string, NG::DialogEvent>& dialogEvent,
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

    auto theme = JSAlertDialog::GetTheme<DialogTheme>();
    if (!theme) {
        LOGE("DialogTheme is null");
        return;
    }

    auto selectedTime = paramObj->GetProperty("selected");
    auto useMilitaryTime = paramObj->GetProperty("useMilitaryTime");
    NG::TimePickerSettingData settingData;
    settingData.isUseMilitaryTime = useMilitaryTime->ToBoolean();

    ButtonInfo buttonInfo;
    DialogProperties properties;
    if (SystemProperties::GetDeviceType() == DeviceType::PHONE) {
        properties.alignment = DialogAlignment::BOTTOM;
    } else {
        properties.alignment = DialogAlignment::CENTER;
    }
    properties.customStyle = false;
    properties.offset = DimensionOffset(Offset(0, -theme->GetMarginBottom().ConvertToPx()));

    std::map<std::string, PickerTime> timePickerProperty;
    if (selectedTime->IsObject()) {
        settingData.dialogTitleDate = ParseDate(selectedTime);
        timePickerProperty["selected"] = ParseTime(selectedTime);
    }
    JSDatePicker::ParseTextProperties(paramObj, settingData.properties);

    auto context = AccessibilityManager::DynamicCast<NG::PipelineContext>(pipelineContext);
    auto overlayManager = context ? context->GetOverlayManager() : nullptr;
    executor->PostTask(
        [properties, settingData, timePickerProperty, dialogEvent, dialogCancelEvent,
            weak = WeakPtr<NG::OverlayManager>(overlayManager)] {
            auto overlayManager = weak.Upgrade();
            CHECK_NULL_VOID(overlayManager);
            overlayManager->ShowTimeDialog(
                properties, settingData, timePickerProperty, dialogEvent, dialogCancelEvent);
        },
        TaskExecutor::TaskType::UI);
}

void JSTimePickerDialog::CreateTimePicker(RefPtr<Component>& component, const JSRef<JSObject>& paramObj)
{
    auto timePicker = AceType::MakeRefPtr<PickerTimeComponent>();
    auto selectedTime = paramObj->GetProperty("selected");
    auto useMilitaryTime = paramObj->GetProperty("useMilitaryTime");
    bool isUseMilitaryTime = useMilitaryTime->ToBoolean();
    if (selectedTime->IsObject()) {
        timePicker->SetSelectedTime(ParseTime(selectedTime));
    }
    timePicker->SetIsDialog(true);
    timePicker->SetIsCreateDialogComponent(true);
    timePicker->SetHour24(isUseMilitaryTime);
    component = timePicker;
}

PickerTime JSTimePickerDialog::ParseTime(const JSRef<JSVal>& timeVal)
{
    auto pickerTime = PickerTime();
    if (!timeVal->IsObject()) {
        return pickerTime;
    }
    auto timeObj = JSRef<JSObject>::Cast(timeVal);
    auto hourFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getHours"));
    auto minuteFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getMinutes"));
    auto secondFunc = JSRef<JSFunc>::Cast(timeObj->GetProperty("getSeconds"));
    JSRef<JSVal> hour = hourFunc->Call(timeObj);
    JSRef<JSVal> minute = minuteFunc->Call(timeObj);
    JSRef<JSVal> second = secondFunc->Call(timeObj);

    if (hour->IsNumber() && minute->IsNumber() && second->IsNumber()) {
        pickerTime.SetHour(hour->ToNumber<int32_t>());
        pickerTime.SetMinute(minute->ToNumber<int32_t>());
        pickerTime.SetSecond(second->ToNumber<int32_t>());
    }
    return pickerTime;
}

PickerDate JSTimePickerDialog::ParseDate(const JSRef<JSVal>& dateVal)
{
    auto pickerDate = PickerDate();
    if (!dateVal->IsObject()) {
        return pickerDate;
    }
    auto dateObj = JSRef<JSObject>::Cast(dateVal);
    auto yearFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getFullYear"));
    auto monthFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getMonth"));
    auto dateFunc = JSRef<JSFunc>::Cast(dateObj->GetProperty("getDate"));
    JSRef<JSVal> year = yearFunc->Call(dateObj);
    JSRef<JSVal> month = monthFunc->Call(dateObj);
    JSRef<JSVal> date = dateFunc->Call(dateObj);

    if (year->IsNumber() && month->IsNumber() && date->IsNumber()) {
        pickerDate.SetYear(year->ToNumber<int32_t>());
        pickerDate.SetMonth(month->ToNumber<int32_t>() + 1); // 0-11 means 1 to 12 months
        pickerDate.SetDay(date->ToNumber<int32_t>());
    }
    return pickerDate;
}
} // namespace OHOS::Ace::Framework
