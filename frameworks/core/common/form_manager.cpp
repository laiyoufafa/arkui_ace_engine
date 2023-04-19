/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "core/common/form_manager.h"

#include "unistd.h"

#include "base/log/log.h"
#include "base/utils/utils.h"

namespace OHOS::Ace {
FormManager::FormManager() {}

FormManager::~FormManager() {}

void FormManager::AddSubContainer(int64_t formId, const RefPtr<SubContainer>& subContainer)
{
    std::lock_guard<std::mutex> lock(mutex_);
    subContainerMap_.insert_or_assign(formId, subContainer);
}

void FormManager::RemoveSubContainer(int64_t formId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    subContainerMap_.erase(formId);
}

RefPtr<SubContainer> FormManager::GetSubContainer(int64_t formId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto subContainer = subContainerMap_.find(formId);
    if (subContainer != subContainerMap_.end()) {
        return subContainer->second;
    } else {
        return nullptr;
    }
}

void FormManager::SetFormUtils(const std::shared_ptr<FormUtils>& formUtils)
{
    std::lock_guard<std::mutex> lock(formUtilsMutex_);
    CHECK_NULL_VOID_NOLOG(formUtils);
    formUtils_ = formUtils;
}

std::shared_ptr<FormUtils> FormManager::GetFormUtils()
{
    std::lock_guard<std::mutex> lock(formUtilsMutex_);
    CHECK_NULL_RETURN_NOLOG(formUtils_, nullptr);
    return formUtils_;
}

} // namespace OHOS::Ace
