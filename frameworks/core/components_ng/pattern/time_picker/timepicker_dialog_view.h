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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TIME_PICKER_TIME_PICKER_DIALOG_VIEW_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TIME_PICKER_TIME_PICKER_DIALOG_VIEW_H

#include "base/utils/macros.h"
#include "core/components/common/layout/constants.h"
#include "core/components/picker/picker_base_component.h"
#include "core/components_ng/pattern/time_picker/timepicker_row_pattern.h"

namespace OHOS::Ace::NG {

class ACE_EXPORT TimePickerDialogView {
public:
    static RefPtr<FrameNode> Show(const DialogProperties& dialogProperties,
        std::map<std::string, PickerTime> timePickerProperty, bool isUseMilitaryTime,
        std::map<std::string, NG::DailogEvent> dialogEvent,
        std::map<std::string, NG::DailogGestureEvent> dialogCancalEvent);
    static void SetSelectedTime(const RefPtr<TimePickerRowPattern>& timePickerRowPattern, const PickerTime& value);
    static void SetHour24(const RefPtr<TimePickerRowPattern>& timePickerRowPattern, bool isUseMilitaryTime = false);
    static void SetDailogChange(const RefPtr<FrameNode>& frameNode, DailogEvent&& onChange);
    static RefPtr<FrameNode> CreateButtonNode(const RefPtr<FrameNode>& dateNode,
        std::map<std::string, NG::DailogEvent> dialogEvent,
        std::map<std::string, NG::DailogGestureEvent> dialogCancalEvent);
    static void SetDailogAcceptEvent(const RefPtr<FrameNode>& frameNode, DailogEvent onChange);
    static RefPtr<FrameNode> CreateTitleButtonNode(const RefPtr<FrameNode>& dateNode);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_PATTERNS_TIME_PICKER_TIME_PICKER_DIALOG_VIEW_H