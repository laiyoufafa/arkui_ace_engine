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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_RENDER_ADAPTER_ANIMATED_IMAGE_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_RENDER_ADAPTER_ANIMATED_IMAGE_H

#include <memory>
#include <utility>

#include "include/codec/SkCodec.h"
#include "include/core/SkImage.h"

#include "base/image/image_source.h"
#include "core/components_ng/image_provider/adapter/skia_image_data.h"
#include "core/components_ng/render/adapter/pixelmap_image.h"
#include "core/components_ng/render/adapter/skia_image.h"

namespace OHOS::Ace::NG {
class AnimatedImage : public virtual CanvasImage {
    DECLARE_ACE_TYPE(AnimatedImage, CanvasImage)
public:
    // initialize animator
    AnimatedImage(const std::unique_ptr<SkCodec>& codec, std::string url);
    ~AnimatedImage() override = default;
    static RefPtr<CanvasImage> Create(const RefPtr<SkiaImageData>& data, const SizeF& size, const std::string& url);
    void ControlAnimation(bool play) override;
    void SetRedrawCallback(std::function<void()>&& callback) override
    {
        redraw_ = std::move(callback);
    }

    bool IsStatic() override
    {
        return false;
    }

protected:
    // ensure frames decode serially
    std::mutex decodeMtx_;
    // protect currentFrame_
    mutable std::mutex frameMtx_;

private:
    void RenderFrame(uint32_t idx);

    // runs on Background thread
    void DecodeFrame(uint32_t idx);
    bool GetCachedFrame(uint32_t idx);

    virtual void DecodeImpl(uint32_t idx) = 0;
    virtual RefPtr<CanvasImage> GetCachedFrameImpl(const std::string& key) = 0;
    virtual void UseCachedFrame(RefPtr<CanvasImage>&& image) = 0;
    virtual void CacheFrame(const std::string& key) = 0;

    RefPtr<Animator> animator_;
    std::function<void()> redraw_;
    const std::string cacheKey_;

    ACE_DISALLOW_COPY_AND_MOVE(AnimatedImage);
};

class AnimatedSkImage : public AnimatedImage, public SkiaImage {
    DECLARE_ACE_TYPE(AnimatedSkImage, AnimatedImage, SkiaImage)
public:
    AnimatedSkImage(std::unique_ptr<SkCodec> codec, std::string url)
        : AnimatedImage(codec, std::move(url)), codec_(std::move(codec))
    {}
    ~AnimatedSkImage() override = default;

    sk_sp<SkImage> GetImage() const override;

    RefPtr<CanvasImage> Clone() override
    {
        return Claim(this);
    }

private:
    void DecodeImpl(uint32_t idx) override;

    void CacheFrame(const std::string& key) override;
    RefPtr<CanvasImage> GetCachedFrameImpl(const std::string& key) override;
    void UseCachedFrame(RefPtr<CanvasImage>&& image) override;

    SkBitmap requiredFrame_;
    std::unique_ptr<SkCodec> codec_;
    sk_sp<SkImage> currentFrame_;

    ACE_DISALLOW_COPY_AND_MOVE(AnimatedSkImage);
};

class AnimatedPixmap : public AnimatedImage, public PixelMapImage {
    DECLARE_ACE_TYPE(AnimatedPixmap, AnimatedImage, PixelMapImage)
public:
    AnimatedPixmap(
        const std::unique_ptr<SkCodec>& codec, const RefPtr<ImageSource>& src, const SizeF& size, std::string url)
        : AnimatedImage(codec, std::move(url)), width_(size.Width()), height_(size.Height()), src_(src)
    {}
    ~AnimatedPixmap() override = default;
    RefPtr<PixelMap> GetPixelMap() const override;

private:
    void DecodeImpl(uint32_t idx) override;

    void CacheFrame(const std::string& key) override;
    RefPtr<CanvasImage> GetCachedFrameImpl(const std::string& key) override;
    void UseCachedFrame(RefPtr<CanvasImage>&& image) override;

    RefPtr<PixelMap> currentFrame_;

    int32_t width_;
    int32_t height_;
    const RefPtr<ImageSource> src_;

    ACE_DISALLOW_COPY_AND_MOVE(AnimatedPixmap);
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_RENDER_ADAPTER_ANIMATED_IMAGE_H
