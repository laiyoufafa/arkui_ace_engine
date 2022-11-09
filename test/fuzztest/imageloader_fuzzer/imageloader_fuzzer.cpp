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

#include "imageloader_fuzzer.h"

#include "core/image/image_cache.h"
#include "core/image/image_loader.h"
#include "core/image/image_object.h"

namespace OHOS::Ace {
RefPtr<ImageObject> CreateAnimatedImageObject(
    ImageSourceInfo source, const Size& imageSize, int32_t frameCount, const sk_sp<SkData>& data)
{
    return Referenced::MakeRefPtr<StaticImageObject>(source, imageSize, frameCount, data);
}

RefPtr<ImageObject> GetImageSvgDomObj(ImageSourceInfo source, const std::unique_ptr<SkMemoryStream > &svgStream,
                                      const RefPtr<PipelineBase>& context, std::optional<Color>& color)
{
    return nullptr;
}

const Ace::ImageSourceInfo CreatImageSourceInfo(const uint8_t* data, size_t size)
{
    std::string randomString = reinterpret_cast<const char*>(data);
    Ace::InternalResource::ResourceId resourceId = Ace::InternalResource::ResourceId::NO_ID;
    Ace::RefPtr<Ace::PixelMap> pixmap = nullptr;
    Ace::Dimension dimension(static_cast<double>(U32_AT(data)));
    Ace::ImageSourceInfo info(randomString, dimension, dimension, resourceId, pixmap);
    return info;
}

bool FileImageLoaderTest(const uint8_t* data, size_t size)
{
    auto info = CreatImageSourceInfo(data, size);
    Ace::WeakPtr<Ace::PipelineBase> context = nullptr;
    Ace::DataProviderImageLoader dataProviderImageLoader;
    return dataProviderImageLoader.LoadImageData(info, context) != nullptr;
}

bool AssetImageLoad(const uint8_t* data, size_t size)
{
    auto info = CreatImageSourceInfo(data, size);
    WeakPtr<Ace::PipelineBase> context = nullptr;
    AssetImageLoader assetImageLoader;
    return assetImageLoader.LoadImageData(info, context) != nullptr;
}

bool NetworkImageLoad(const uint8_t* data, size_t size)
{
    auto info = CreatImageSourceInfo(data, size);
    WeakPtr<Ace::PipelineBase> context = nullptr;
    NetworkImageLoader networkImageLoader;
    return networkImageLoader.LoadImageData(info, context) != nullptr;
}

bool InternalImageLoad(const uint8_t* data, size_t size)
{
    auto info = CreatImageSourceInfo(data, size);
    Ace::WeakPtr<Ace::PipelineBase> context = nullptr;
    Ace::InternalImageLoader internalImageLoader;
    return internalImageLoader.LoadImageData(info, context) != nullptr;
}
} // namespace OHOS::Ace

using namespace OHOS;
using namespace OHOS::Ace;
/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::Ace::ImageSourceInfo i;
    auto c = ImageCache::Create();
    c->SetCapacity(1);
    /* Run your code on data */
    FileImageLoaderTest(data, size);
    AssetImageLoad(data, size);
    NetworkImageLoad(data, size);
    InternalImageLoad(data, size);
    return i.IsSvg();
}
