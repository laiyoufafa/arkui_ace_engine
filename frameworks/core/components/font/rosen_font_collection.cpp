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

#include "core/components/font/rosen_font_collection.h"

#include "include/core/SkTypeface.h"
#include "base/i18n/localization.h"
#include "base/log/ace_trace.h"
#include "base/log/log.h"
#include "base/utils/system_properties.h"
#include "base/utils/utils.h"
#include "core/common/ace_engine.h"

#include "rosen_text/font_collection.h"

namespace OHOS::Ace {

RosenFontCollection RosenFontCollection::instance;

std::shared_ptr<Rosen::FontCollection> RosenFontCollection::GetFontCollection()
{
    std::call_once(fontFlag_, [this]() {
        fontCollection_ = Rosen::FontCollection::Create();
    });
    return fontCollection_;
}

void RosenFontCollection::LoadFontFromList(const uint8_t* fontData, size_t length, std::string familyName)
{
    std::call_once(fontFlag_, [this]() {
        fontCollection_ = Rosen::FontCollection::Create();
    });
    
    auto it = std::find(families_.begin(), families_.end(), familyName);
    if (it != families_.end()) {
        return;
    }

    families_.emplace_back(familyName);

    if (fontCollection_) {
        fontCollection_->LoadFont(familyName, fontData, length);
    }
}

RosenFontCollection& RosenFontCollection::GetInstance()
{
    return instance;
}

void RosenFontCollection::VaryFontCollectionWithFontWeightScale(float fontWeightScale)
{
    if (LessOrEqual(fontWeightScale, 0.0)) {
        return;
    }

}

void RosenFontCollection::LoadSystemFont()
{
    ACE_FUNCTION_TRACE();

}

void RosenFontCollection::SetIsZawgyiMyanmar(bool isZawgyiMyanmar)
{
    ACE_FUNCTION_TRACE();

    if (isZawgyiMyanmar_ == isZawgyiMyanmar) {
        return;
    }
    isZawgyiMyanmar_ = isZawgyiMyanmar;

    AceEngine::Get().NotifyContainers([](const RefPtr<Container>& container) {
        if (container) {
            container->NotifyFontNodes();
        }
    });
}

} // namespace OHOS::Ace
