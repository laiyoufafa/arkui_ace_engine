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

#include <unistd.h>
#include "frame_trace_adapter_impl.h"

namespace OHOS::Ace {
#ifdef __aarch64__
const char* FRAME_TRACE_SO_PATH = "/system/lib64/libframe_trace_intf.z.so";
#else
const char* FRAME_TRACE_SO_PATH = "/system/lib/libframe_trace_intf.z.so";
#endif

void FrameTraceAdapterImpl::QuickExecute(std::function<void()> && func)
{
    FRAME_TRACE::TraceAndExecute(std::move(func), FRAME_TRACE::TraceType::QUICK_TRACE);
}

void FrameTraceAdapterImpl::SlowExecute(std::function<void()> && func)
{
    FRAME_TRACE::TraceAndExecute(std::move(func), FRAME_TRACE::TraceType::SLOW_TRACE);
}

bool FrameTraceAdapterImpl::EnableFrameTrace(const std::string &traceTag)
{
    return FRAME_TRACE::FrameAwareTraceEnable(traceTag);
}

FrameTraceAdapter* FrameTraceAdapter::GetInstance()
{
    static FrameTraceAdapterImpl instance;
    return &instance;
}

bool FrameTraceAdapterImpl::IsEnabled()
{
    if (access(FRAME_TRACE_SO_PATH, F_OK) != 0) {
        return false;
    }
    return FRAME_TRACE::IsEnabled();
}
}
