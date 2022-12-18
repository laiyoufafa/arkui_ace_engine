/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "core/components_ng/gestures/recognizers/exclusive_recognizer.h"

namespace OHOS::Ace::NG {
void ExclusiveRecognizer::OnAccepted() {}

void ExclusiveRecognizer::OnRejected() {}

void ExclusiveRecognizer::OnPending() {}

void ExclusiveRecognizer::OnBlocked() {}

bool ExclusiveRecognizer::HandleEvent(const TouchEvent& /* point */)
{
    return true;
}

bool ExclusiveRecognizer::HandleEvent(const AxisEvent& /* event */)
{
    return true;
}

void ExclusiveRecognizer::BatchAdjudicate(const RefPtr<NGGestureRecognizer>& recognizer, GestureDisposal disposal) {}

bool ExclusiveRecognizer::ReconcileFrom(const RefPtr<NGGestureRecognizer>& /* recognizer */)
{
    return true;
}
void ExclusiveRecognizer::OnResetStatus() {}
} // namespace OHOS::Ace::NG
