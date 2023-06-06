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

#include "image_source_ohos.h"

#include "image_source.h"
#include "image_type.h"
#include "media_errors.h"

#include "base/image/pixel_map.h"

namespace OHOS::Ace {
RefPtr<ImageSource> ImageSource::Create(int32_t fd)
{
    uint32_t errorCode;
    Media::SourceOptions options;
    auto src = Media::ImageSource::CreateImageSource(fd, options, errorCode);
    if (errorCode != Media::SUCCESS) {
        LOGE("create image source failed, errorCode = %{public}u", errorCode);
        return nullptr;
    }
    return MakeRefPtr<ImageSourceOhos>(std::move(src));
}

RefPtr<ImageSource> ImageSource::Create(const uint8_t* data, uint32_t size)
{
    uint32_t errorCode;
    Media::SourceOptions options;
    auto src = Media::ImageSource::CreateImageSource(data, size, options, errorCode);
    if (errorCode != Media::SUCCESS) {
        LOGE("create image source failed, errorCode = %{public}u", errorCode);
        return nullptr;
    }
    return MakeRefPtr<ImageSourceOhos>(std::move(src));
}

std::string ImageSourceOhos::GetProperty(const std::string& key)
{
    std::string value;
    uint32_t res = imageSource_->GetImagePropertyString(0, key, value);
    if (res != Media::SUCCESS) {
        LOGE("Get ImageSource property %{public}s failed, errorCode = %{public}u", key.c_str(), res);
    }
    return value;
}

RefPtr<PixelMap> ImageSourceOhos::CreatePixelMap(int32_t width, int32_t height)
{
    return CreatePixelMap(0, width, height);
}

RefPtr<PixelMap> ImageSourceOhos::CreatePixelMap(uint32_t index, int32_t width, int32_t height)
{
    Media::DecodeOptions options {
        .desiredSize = { width, height },
    };
    uint32_t errorCode;
    auto pixmap = imageSource_->CreatePixelMapEx(index, options, errorCode);
    if (errorCode != Media::SUCCESS) {
        LOGE("create PixelMap from ImageSource failed, index = %{public}u, errorCode = %{public}u", index, errorCode);
        return nullptr;
    }
    return PixelMap::Create(std::move(pixmap));
}
} // namespace OHOS::Ace
