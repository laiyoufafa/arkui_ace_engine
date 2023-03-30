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

#include "core/image/image_object.h"

#include "base/thread/background_task_executor.h"
#include "base/utils/utils.h"
#include "core/common/container.h"
#include "core/common/container_scope.h"
#include "core/components/image/render_image.h"
#include "core/components_ng/render/canvas_image.h"
#include "core/image/flutter_image_cache.h"
#include "core/image/image_compressor.h"

#ifdef APNG_IMAGE_SUPPORT
#include "core/image/apng/apng_image_decoder.h"
#include "core/image/apng/apng_image_object.h"
#endif

namespace OHOS::Ace {

std::string ImageObject::GenerateCacheKey(const ImageSourceInfo& srcInfo, Size targetImageSize)
{
    return srcInfo.GetKey() + std::to_string(static_cast<int32_t>(targetImageSize.Width())) +
           std::to_string(static_cast<int32_t>(targetImageSize.Height()));
}

RefPtr<ImageObject> ImageObject::BuildImageObject(
    ImageSourceInfo source, const RefPtr<PipelineBase> context, const sk_sp<SkData>& skData, bool useSkiaSvg)
{
    // build svg image object.
    if (source.IsSvg()) {
#ifdef NG_BUILD
        return nullptr;
#else
        const auto svgStream = std::make_unique<SkMemoryStream>(skData);
        if (!svgStream) {
            return nullptr;
        }
        auto color = source.GetFillColor();
        if (!useSkiaSvg) {
            return Ace::GetImageSvgDomObj(source, svgStream, context, color);
        } else {
            uint64_t colorValue = 0;
            if (color.has_value()) {
                colorValue = color.value().GetValue();
                // skia svg relies on the 32th bit to determine whether or not to use the color we set.
                colorValue = colorValue | (static_cast<int64_t>(0b1) << 32);
            }
            auto skiaDom = SkSVGDOM::MakeFromStream(*svgStream, colorValue);
            return skiaDom ? MakeRefPtr<SvgSkiaImageObject>(source, Size(), 1, skiaDom) : nullptr;
        }
#endif
    }

    // if is png or apng check
#ifdef APNG_IMAGE_SUPPORT
    if (source.isPng()) {
        auto apngDecoder = AceType::MakeRefPtr<PNGImageDecoder>(skData);
        if (apngDecoder && apngDecoder->isApng()) {
            if (!apngDecoder->DecodeImage()) {
                return nullptr;
            }

            Size imageSize = apngDecoder->GetImageSize();
            uint32_t frameCount = apngDecoder->GetFrameCount();
            return MakeRefPtr<ApngImageObject>(source, imageSize, frameCount, skData, apngDecoder);
        }
    }
#endif

    // build normal pixel image object.
    auto codec = SkCodec::MakeFromData(skData);
    int32_t totalFrames = 1;
    Size imageSize;
    if (codec) {
        totalFrames = codec->getFrameCount();
        switch (codec->getOrigin()) {
            case SkEncodedOrigin::kLeftTop_SkEncodedOrigin:
            case SkEncodedOrigin::kRightTop_SkEncodedOrigin:
            case SkEncodedOrigin::kRightBottom_SkEncodedOrigin:
            case SkEncodedOrigin::kLeftBottom_SkEncodedOrigin:
                imageSize.SetSize(Size(codec->dimensions().fHeight, codec->dimensions().fWidth));
                break;
            default:
                imageSize.SetSize(Size(codec->dimensions().fWidth, codec->dimensions().fHeight));
        }
    }
    if (totalFrames == 1) {
        return MakeRefPtr<StaticImageObject>(source, imageSize, totalFrames, skData);
    } else {
        return CreateAnimatedImageObject(source, imageSize, totalFrames, skData);
    }
}

Size ImageObject::MeasureForImage(RefPtr<RenderImage> image)
{
    return image->MeasureForNormalImage();
}

void SvgImageObject::PerformLayoutImageObject(RefPtr<RenderImage> image)
{
    image->PerformLayoutSvgImage();
}

Size SvgImageObject::MeasureForImage(RefPtr<RenderImage> image)
{
    return image->MeasureForSvgImage();
}

void SvgSkiaImageObject::PerformLayoutImageObject(RefPtr<RenderImage> image) {}

Size SvgSkiaImageObject::MeasureForImage(RefPtr<RenderImage> image)
{
    return image->MeasureForSvgImage();
}

void StaticImageObject::UploadToGpuForRender(const WeakPtr<PipelineBase>& context,
    const UploadSuccessCallback& successCallback,
    const FailedCallback& failedCallback, const Size& imageSize, bool forceResize, bool syncMode)
{
    auto task = [context, successCallback, failedCallback, imageSize, forceResize, skData = skData_,
                    imageSource = imageSource_, id = Container::CurrentId()]() mutable {
        ContainerScope scope(id);
        auto pipelineContext = context.Upgrade();
        if (!pipelineContext) {
            LOGE("pipeline context has been released.");
            return;
        }
        auto taskExecutor = pipelineContext->GetTaskExecutor();
        if (!taskExecutor) {
            LOGE("task executor is null.");
            return;
        }

        auto key = GenerateCacheKey(imageSource, imageSize);
        // is already uploaded
        if (!ImageProvider::TryUploadingImage(key, successCallback, failedCallback)) {
            LOGI("other thread is uploading same image to gpu : %{public}s", imageSource.ToString().c_str());
            return;
        }

        RefPtr<NG::CanvasImage> cachedFlutterImage;
        auto imageCache = pipelineContext->GetImageCache();
        if (imageCache) {
            auto cachedImage = imageCache->GetCacheImage(key);
            if (cachedImage) {
                auto skImage = cachedImage->imagePtr;
                cachedFlutterImage = NG::CanvasImage::Create(&skImage);
            }
        }

        // found cached image obj (can be rendered)
        if (cachedFlutterImage) {
            LOGD("get cached image success: %{public}s", key.c_str());
            ImageProvider::ProccessUploadResult(taskExecutor, imageSource, imageSize, cachedFlutterImage);
            return;
        }

        auto callback = [successCallback, imageSource, taskExecutor, imageCache, imageSize, key,
                            id = Container::CurrentId()](
                            sk_sp<SkImage> image, sk_sp<SkData> compressData) {

            if (!image && !compressData.get()) {
                ImageProvider::ProccessUploadResult(taskExecutor, imageSource, imageSize, nullptr,
                    "Image data may be broken or absent in upload callback.");
            }
            ContainerScope scope(id);
            sk_sp<SkImage> skImage = image;
            auto canvasImage = NG::CanvasImage::Create(&skImage);
            int32_t width = static_cast<int32_t>(imageSize.Width() + 0.5);
            int32_t height = static_cast<int32_t>(imageSize.Height() + 0.5);
            canvasImage->SetRawCompressData(&compressData, width, height);

            if (imageCache) {
                LOGD("cache image key: %{public}s", key.c_str());
                imageCache->CacheImage(key, std::make_shared<CachedImage>(skImage));
            }
            ImageProvider::ProccessUploadResult(taskExecutor, imageSource, imageSize, canvasImage);
        };
        // here skdata is origin pic, also have 'target size'
        // if have skdata, means origin pic is rendered first time
        // if no skdata, means origin pic has shown, and has been cleared
        // we try to use small image or compressed image instead of origin pic.
        sk_sp<SkData> stripped;
        if (ImageCompressor::GetInstance()->CanCompress()) {
            // load compressed
            auto compressedData = ImageProvider::LoadImageRawDataFromFileCache(pipelineContext, key, ".astc");
            stripped = ImageCompressor::StripFileHeader(compressedData);
        }
        auto smallData = ImageProvider::LoadImageRawDataFromFileCache(pipelineContext, key);
        if (smallData) {
            skData = smallData;
        }

        if (!skData) {
            LOGD("reload sk data");
            skData = ImageProvider::LoadImageRawData(imageSource, pipelineContext);
            if (!skData) {
                LOGE("reload image data failed. imageSource: %{private}s", imageSource.ToString().c_str());
                ImageProvider::ProccessUploadResult(taskExecutor, imageSource, imageSize, nullptr,
                    "Image data may be broken or absent, please check if image file or image data is valid.");
                return;
            }
        }

        // make lazy image from file
        auto rawImage = SkImage::MakeFromEncoded(skData);
        if (!rawImage) {
            LOGE("static image MakeFromEncoded fail! imageSource: %{private}s", imageSource.ToString().c_str());
            ImageProvider::ProccessUploadResult(taskExecutor, imageSource, imageSize, nullptr,
                "Image data may be broken, please check if image file or image data is broken.");
            return;
        }
        sk_sp<SkImage> image;
        if (smallData) {
            image = rawImage;
        } else {
            image = ImageProvider::ResizeSkImage(rawImage, imageSource.GetSrc(), imageSize, forceResize);
        }
        ImageProvider::UploadImageToGPUForRender(pipelineContext, image, stripped, callback, key);
        skData = nullptr;
    };
    if (syncMode) {
        task();
        return;
    }
    uploadForPaintTask_ = CancelableTask(std::move(task));
    BackgroundTaskExecutor::GetInstance().PostTask(uploadForPaintTask_);
}

bool StaticImageObject::CancelBackgroundTasks()
{
    return uploadForPaintTask_ ? uploadForPaintTask_.Cancel(false) : false;
}

void PixelMapImageObject::PerformLayoutImageObject(RefPtr<RenderImage> image)
{
    image->PerformLayoutPixmap();
}

Size PixelMapImageObject::MeasureForImage(RefPtr<RenderImage> image)
{
    return image->MeasureForPixmap();
}

} // namespace OHOS::Ace
