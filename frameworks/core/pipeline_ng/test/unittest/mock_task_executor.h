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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_PIPELINE_NG_TEST_UNITTEST_MOCK_TASK_EXECUTOR_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_PIPELINE_NG_TEST_UNITTEST_MOCK_TASK_EXECUTOR_H

#include "gmock/gmock-function-mocker.h"
#include "gmock/gmock.h"
#include "base/thread/task_executor.h"
#include "base/utils/utils.h"

namespace OHOS::Ace::NG {
class MockTaskExecutor : public TaskExecutor {
public:
    MOCK_METHOD(void, AddTaskObserver, (Task&& callback), (override));
    MOCK_METHOD(void, RemoveTaskObserver, (), (override));

    bool WillRunOnCurrentThread(TaskType type) const override
    {
        return true;
    }
private:
    MOCK_METHOD(Task, WrapTaskWithTraceId, (Task&& task, int32_t id), (const override));

    bool OnPostTask(Task&& task, TaskType type, uint32_t delayTime) const override
    {
        CHECK_NULL_RETURN(task, false);
        task();
        return true;
    }
};
} // namespace OHOS::Ace::NG
#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_PIPELINE_NG_TEST_UNITTEST_MOCK_TASK_EXECUTOR_H
