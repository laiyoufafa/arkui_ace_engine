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

#ifndef FOUNDATION_ACE_INTERFACE_INNERKITS_FORM_RENDERER_DISPATCHER_STUB_H
#define FOUNDATION_ACE_INTERFACE_INNERKITS_FORM_RENDERER_DISPATCHER_STUB_H

#include <map>

#include "form_renderer_delegate_interface.h"
#include "iremote_object.h"
#include "iremote_stub.h"

namespace OHOS {
namespace Ace {
/**
 * @class FormRendererDelegateStub
 * Form renderer delegate stub.
 */
class FormRendererDelegateStub : public IRemoteStub<IFormRendererDelegate> {
public:
    FormRendererDelegateStub();
    virtual ~FormRendererDelegateStub();
    /**
     * @brief handle remote request.
     * @param data input param.
     * @param reply output param.
     * @param option message option.
     * @return Returns ERR_OK on success, others on failure.
     */
    virtual int32_t OnRemoteRequest(
        uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option);

private:
    int32_t HandleOnSurfaceCreate(MessageParcel &data, MessageParcel &reply);
    int32_t HandleOnActionEvent(MessageParcel &data, MessageParcel &reply);

    using FormRendererDelegateFunc =
        int32_t (FormRendererDelegateStub::*)(MessageParcel &data, MessageParcel &reply);
    std::map<uint32_t, FormRendererDelegateFunc> memberFuncMap_;

    DISALLOW_COPY_AND_MOVE(FormRendererDelegateStub);
};
} // namespace Ace
} // namespace OHOS
#endif // FOUNDATION_ACE_INTERFACE_INNERKITS_FORM_RENDERER_DISPATCHER_STUB_H
