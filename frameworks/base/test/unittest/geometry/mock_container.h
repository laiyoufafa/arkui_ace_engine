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

#ifndef FOUNDATION_ACE_FRAMEWORKS_BASE_TEST_UNITTEST_GEOMETRY_MOCK_CONTAINER_H
#define FOUNDATION_ACE_FRAMEWORKS_BASE_TEST_UNITTEST_GEOMETRY_MOCK_CONTAINER_H

#include "gmock/gmock.h"
#include "core/common/container.h"

namespace OHOS::Ace {
class MockContainer final : public Container {
    DECLARE_ACE_TYPE(MockContainer, Container);
public:
    explicit MockContainer(RefPtr<PipelineBase> pipelineContext = nullptr) : pipelineContext_(pipelineContext) {}
    RefPtr<PipelineBase> GetPipelineContext() const override
    {
        return pipelineContext_;
    }
    MOCK_METHOD(void, Initialize, (), (override));
    MOCK_METHOD(void, Destroy, (), (override));
    MOCK_METHOD(int32_t, GetInstanceId, (), (const, override));
    MOCK_METHOD(std::string, GetHostClassName, (), (const, override));

    MOCK_METHOD(RefPtr<Frontend>, GetFrontend, (), (const, override));
    MOCK_METHOD(RefPtr<TaskExecutor>, GetTaskExecutor, (), (const, override));
    MOCK_METHOD(RefPtr<AssetManager>, GetAssetManager, (), (const, override));
    MOCK_METHOD(RefPtr<PlatformResRegister>, GetPlatformResRegister, (), (const, override));
    MOCK_METHOD(bool, Dump, (const std::vector<std::string>& params), (override));
    MOCK_METHOD(int32_t, GetViewWidth, (), (const, override));
    MOCK_METHOD(int32_t, GetViewHeight, (), (const, override));
    MOCK_METHOD(int32_t, GetViewPosX, (), (const, override));
    MOCK_METHOD(int32_t, GetViewPosY, (), (const, override));
    MOCK_METHOD(uint32_t, GetWindowId, (), (const, override));
    MOCK_METHOD(void*, GetView, (), (const, override));
private:
    RefPtr<PipelineBase> pipelineContext_;
};
} // namespace OHOS::Ace
#endif // FOUNDATION_ACE_FRAMEWORKS_BASE_TEST_UNITTEST_GEOMETRY_MOCK_CONTAINER_H
