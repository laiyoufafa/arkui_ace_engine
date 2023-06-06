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

#ifndef FOUNDATION_ACE_INTERFACE_INNERKITS_ACE_DECLARATIVE_MODULE_PRELOADER_H
#define FOUNDATION_ACE_INTERFACE_INNERKITS_ACE_DECLARATIVE_MODULE_PRELOADER_H

#include <memory>
#include <string>

#include "macros.h"

class NativeEngine;

namespace OHOS::Ace {

class ACE_FORCE_EXPORT DeclarativeModulePreloader {
public:
    static void Preload(NativeEngine& runtime);

    // ArkTsCard start
    static void PreloadCard(NativeEngine& runtime, const std::string &bundleName);
    static void ReloadCard(NativeEngine& runtime, const std::string &bundleName); // For card upgrade condition
    // ArkTsCard end
};

} // namespace OHOS::Ace

#endif // FOUNDATION_ACE_INTERFACE_INNERKITS_ACE_DECLARATIVE_MODULE_PRELOADER_H
