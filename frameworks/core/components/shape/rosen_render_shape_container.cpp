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

#include "frameworks/core/components/shape/rosen_render_shape_container.h"

#include "render_service_client/core/ui/rs_node.h"
#include "securec.h"

#include "frameworks/core/components/transform/rosen_render_transform.h"

namespace OHOS::Ace {
namespace {

struct ZIndexComparator {
    bool operator()(const RefPtr<RenderNode>& left, const RefPtr<RenderNode>& right) const
    {
        return (left->GetZIndex() < right->GetZIndex());
    }
};

inline std::multiset<RefPtr<RenderNode>, ZIndexComparator> SortChildrenByZIndex(
    const std::list<RefPtr<RenderNode>>& children)
{
    return std::multiset<RefPtr<RenderNode>, ZIndexComparator>(children.begin(), children.end());
}

} // namespace

void RosenRenderShapeContainer::PerformLayout()
{
    RenderShapeContainer::PerformLayout();

    auto node = GetRSNode();
    if (node == nullptr) {
        return;
    }
    node->SetClipToFrame(true);

    double viewBoxWidth = NormalizePercentToPx(viewBox_.Width(), false);
    double viewBoxHeight = NormalizePercentToPx(viewBox_.Height(), true);
    double viewBoxLeft = NormalizePercentToPx(viewBox_.Left(), false);
    double viewBoxTop = NormalizePercentToPx(viewBox_.Top(), true);
    if (!GetLayoutSize().IsInfinite() && GreatNotEqual(viewBoxWidth, 0.0) && GreatNotEqual(viewBoxHeight, 0.0)) {
        double scale = std::min(GetLayoutSize().Width() / viewBoxWidth, GetLayoutSize().Height() / viewBoxHeight);
        double tx = GetLayoutSize().Width() * 0.5 - (viewBoxWidth * 0.5 + viewBoxLeft) * scale;
        double ty = GetLayoutSize().Height() * 0.5 - (viewBoxHeight * 0.5 + viewBoxTop) * scale;
        for (const auto& child : GetChildren()) {
            auto rsNode = child->GetRSNode();
            if (!rsNode) {
                continue;
            }
            rsNode->SetPivot(0.0f, 0.0f);
            rsNode->SetScale(scale);
            rsNode->SetTranslate({ tx, ty });
        }
    }
}

void RosenRenderShapeContainer::Paint(RenderContext& context, const Offset& offset)
{
    const auto renderContext = static_cast<RosenRenderContext*>(&context);
#ifndef USE_ROSEN_DRAWING
    skCanvas_ = renderContext->GetCanvas();
    if (!skCanvas_) {
        return;
    }
    if (mesh_.size() == 0) {
        auto imageInfo = SkImageInfo::Make(GetLayoutSize().Width(), GetLayoutSize().Height(),
            SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kUnpremul_SkAlphaType);
        skOffBitmap_.allocPixels(imageInfo);
        skOffBitmap_.eraseColor(SK_ColorTRANSPARENT);
        skOffCanvas_ = std::make_unique<SkCanvas>(skOffBitmap_);

        double viewBoxWidth = NormalizePercentToPx(viewBox_.Width(), false);
        double viewBoxHeight = NormalizePercentToPx(viewBox_.Height(), true);
        double viewBoxLeft = NormalizePercentToPx(viewBox_.Left(), false);
        double viewBoxTop = NormalizePercentToPx(viewBox_.Top(), true);
        if (!GetLayoutSize().IsInfinite() && GreatNotEqual(viewBoxWidth, 0.0) && GreatNotEqual(viewBoxHeight, 0.0)) {
            double scale = std::min(GetLayoutSize().Width() / viewBoxWidth, GetLayoutSize().Height() / viewBoxHeight);
            double tx = GetLayoutSize().Width() * 0.5 - (viewBoxWidth * 0.5 + viewBoxLeft) * scale;
            double ty = GetLayoutSize().Height() * 0.5 - (viewBoxHeight * 0.5 + viewBoxTop) * scale;
            skOffCanvas_->scale(scale, scale);
            skOffCanvas_->translate(tx, ty);
        }
        const auto& children = GetChildren();
        for (const auto& item : SortChildrenByZIndex(children)) {
            Offset childOffset = offset;
            RefPtr<RosenRenderShape> renderShape = GetShapeChild(item, childOffset);
            if (renderShape) {
                renderShape->PaintOnCanvas(skOffCanvas_.get(), childOffset);
                continue;
            }
            RenderNode::Paint(context, offset);
        }
        if (column_ == 0 && row_ == 0) {
#ifndef NEW_SKIA
            skCanvas_->drawBitmap(skOffBitmap_, 0, 0);
#else
            skCanvas_->drawImage(skOffBitmap_.asImage(), 0, 0, SkSamplingOptions());
#endif
        }
    } else {
        BitmapMesh(context, offset);
    }
#else
    canvas_ = renderContext->GetCanvas();
    if (!canvas_) {
        return;
    }
    if (mesh_.size() == 0) {
        RSBitmapFormat format {
            RSColorType::COLORTYPE_RGBA_8888,
            RSAlphaType::ALPHATYPE_UNPREMUL,
        };
        offBitmap_.Build(GetLayoutSize().Width(), GetLayoutSize().Height(), format);
        offBitmap_.ClearWithColor(RSColor::COLOR_TRANSPARENT);
        offCanvas_ = std::make_unique<RSCanvas>();
        offCanvas_->Bind(offBitmap_);

        double viewBoxWidth = NormalizePercentToPx(viewBox_.Width(), false);
        double viewBoxHeight = NormalizePercentToPx(viewBox_.Height(), true);
        double viewBoxLeft = NormalizePercentToPx(viewBox_.Left(), false);
        double viewBoxTop = NormalizePercentToPx(viewBox_.Top(), true);
        if (!GetLayoutSize().IsInfinite() && GreatNotEqual(viewBoxWidth, 0.0) && GreatNotEqual(viewBoxHeight, 0.0)) {
            double scale = std::min(GetLayoutSize().Width() / viewBoxWidth, GetLayoutSize().Height() / viewBoxHeight);
            double tx = GetLayoutSize().Width() * 0.5 - (viewBoxWidth * 0.5 + viewBoxLeft) * scale;
            double ty = GetLayoutSize().Height() * 0.5 - (viewBoxHeight * 0.5 + viewBoxTop) * scale;
            offCanvas_->Scale(scale, scale);
            offCanvas_->Translate(tx, ty);
        }
        const auto& children = GetChildren();
        for (const auto& item : SortChildrenByZIndex(children)) {
            Offset childOffset = offset;
            RefPtr<RosenRenderShape> renderShape = GetShapeChild(item, childOffset);
            if (renderShape) {
                renderShape->PaintOnCanvas(offCanvas_.get(), childOffset);
                continue;
            }
            RenderNode::Paint(context, offset);
        }
        if (column_ == 0 && row_ == 0) {
            canvas_->DrawBitmap(offBitmap_, 0, 0);
        }
    } else {
        BitmapMesh(context, offset);
    }
#endif
}

RefPtr<RosenRenderShape> RosenRenderShapeContainer::GetShapeChild(const RefPtr<RenderNode>& node, Offset& offset)
{
    offset += node->GetPosition();
    if (auto renderShape = AceType::DynamicCast<RosenRenderShape>(node)) {
        return renderShape;
    }
    const auto& children = node->GetChildren();
    for (const auto& item : SortChildrenByZIndex(children)) {
        if (RefPtr<RosenRenderShape> temp = GetShapeChild(item, offset)) {
            return temp;
        }
    }
    offset = offset - node->GetPosition();
    return nullptr;
}

void RosenRenderShapeContainer::BitmapMesh(RenderContext& context, const Offset& offset)
{
#ifndef USE_ROSEN_DRAWING
    auto imageInfo = SkImageInfo::Make(GetLayoutSize().Width(), GetLayoutSize().Height(),
        SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kUnpremul_SkAlphaType);
    skOffBitmap_.allocPixels(imageInfo);
    skOffBitmap_.eraseColor(SK_ColorTRANSPARENT);
    skOffCanvas_ = std::make_unique<SkCanvas>(skOffBitmap_);

    const auto renderContext = static_cast<RosenRenderContext*>(&context);
    skCanvas_ = renderContext->GetCanvas();
    if (!skCanvas_) {
        return;
    }
#else
    RSBitmapFormat format {
        RSColorType::COLORTYPE_RGBA_8888,
        RSAlphaType::ALPHATYPE_UNPREMUL,
    };
    offBitmap_.Build(GetLayoutSize().Width(), GetLayoutSize().Height(), format);
    offBitmap_.ClearWithColor(RSColor::COLOR_TRANSPARENT);
    offCanvas_ = std::make_unique<RSCanvas>();
    offCanvas_->Bind(offBitmap_);

    const auto renderContext = static_cast<RosenRenderContext*>(&context);
    canvas_ = renderContext->GetCanvas();
    if (!canvas_) {
        return;
    }
#endif

    // for the child
    double viewBoxWidth = NormalizePercentToPx(viewBox_.Width(), false);
    double viewBoxHeight = NormalizePercentToPx(viewBox_.Height(), true);
    double viewBoxLeft = NormalizePercentToPx(viewBox_.Left(), false);
    double viewBoxTop = NormalizePercentToPx(viewBox_.Top(), true);
    if (!GetLayoutSize().IsInfinite() && GreatNotEqual(viewBoxWidth, 0.0) && GreatNotEqual(viewBoxHeight, 0.0)) {
        double scale = std::min(GetLayoutSize().Width() / viewBoxWidth, GetLayoutSize().Height() / viewBoxHeight);
        double tx = GetLayoutSize().Width() * 0.5 - (viewBoxWidth * 0.5 + viewBoxLeft) * scale;
        double ty = GetLayoutSize().Height() * 0.5 - (viewBoxHeight * 0.5 + viewBoxTop) * scale;
#ifndef USE_ROSEN_DRAWING
        skOffCanvas_->scale(scale, scale);
        skOffCanvas_->translate(tx, ty);
#else
        offCanvas_->Scale(scale, scale);
        offCanvas_->Translate(tx, ty);
#endif
    }

    const auto& children = GetChildren();
    for (const auto& item : SortChildrenByZIndex(children)) {
        Offset childOffset = offset;
        RefPtr<RosenRenderShape> renderShape = GetShapeChild(item, childOffset);
        if (renderShape) {
#ifndef USE_ROSEN_DRAWING
            renderShape->PaintOnCanvas(skOffCanvas_.get(), childOffset);
#else
            renderShape->PaintOnCanvas(offCanvas_.get(), childOffset);
#endif
        }
    }
    if (column_ == 0 && row_ == 0) {
#ifndef USE_ROSEN_DRAWING
#ifndef NEW_SKIA
        skCanvas_->drawBitmap(skOffBitmap_, 0, 0);
#else
        skCanvas_->drawImage(skOffBitmap_.asImage(), 0, 0, SkSamplingOptions());
#endif
#else
        canvas_->DrawBitmap(offBitmap_, 0, 0);
#endif
        return;
    }
    uint32_t size = mesh_.size();
    float verts[size];
    for (uint32_t i = 0; i < size; i++) {
        verts[i] = (float)mesh_[i];
    }
#ifndef USE_ROSEN_DRAWING
    DrawBitmapMesh(skOffBitmap_, column_, row_, verts, 0, nullptr);
#else
    DrawBitmapMesh(offBitmap_, column_, row_, verts, 0, nullptr);
#endif
}

#ifndef USE_ROSEN_DRAWING
void RosenRenderShapeContainer::DrawBitmapMesh(SkBitmap& bitmap, int column, int row,
    const float* vertices, const int* colors, const SkPaint* paint)
#else
void RosenRenderShapeContainer::DrawBitmapMesh(RSBitmap& bitmap, int column, int row,
    const float* vertices, const int* colors, const RSBrush* brush)
#endif
{
    const int vertCounts = (column + 1) * (row + 1);
    int32_t size = 6;
    const int indexCount = column * row * size;
#ifndef USE_ROSEN_DRAWING
    uint32_t flags = SkVertices::kHasTexCoords_BuilderFlag;
    if (colors) {
        flags |= SkVertices::kHasColors_BuilderFlag;
    }
    SkVertices::Builder builder(SkVertices::kTriangles_VertexMode, vertCounts, indexCount, flags);
    if (memcpy_s(builder.positions(), vertCounts * sizeof(SkPoint), vertices, vertCounts * sizeof(SkPoint)) != 0) {
        return;
    }
    if (colors) {
        if (memcpy_s(builder.colors(), vertCounts * sizeof(SkColor), colors, vertCounts * sizeof(SkColor)) != 0) {
            return;
        }
    }
    SkPoint* texsPoint = builder.texCoords();
    uint16_t* indices = builder.indices();
    const SkScalar height = SkIntToScalar(bitmap.height());
    const SkScalar width = SkIntToScalar(bitmap.width());
#else
    int flags = static_cast<int>(RSVertices::BuilderFlags::BUILDERFLAGS_TEXCOORDS);
    if (colors) {
        flags |= static_cast<int>(RSVertices::BuilderFlags::BUILDERFLAGS_COLORS);
    }
    RSVertices::Builder builder(RSVertexMode::VERTEXMODE_TRIANGLES, vertCounts, indexCount, flags);
    if (memcpy_s(builder.Positions(), vertCounts * sizeof(RSPoint),
        vertices, vertCounts * sizeof(RSPoint)) != 0) {
        return;
    }
    if (colors) {
        if (memcpy_s(
            builder.Colors(), vertCounts * sizeof(RSColor), colors, vertCounts * sizeof(RSColor)) != 0) {
            return;
        }
    }
    RSPoint* texsPoint = builder.TexCoords();
    uint16_t* indices = builder.Indices();
    const RSScalar height = IntToScalar(bitmap.GetHeight());
    const RSScalar width = IntToScalar(bitmap.GetWidth());
#endif

    if (row == 0) {
        LOGE("row is zero");
        return;
    }
    if (column == 0) {
        LOGE("column is zero");
        return;
    }
#ifndef USE_ROSEN_DRAWING
    const SkScalar dy = height / row;
    const SkScalar dx = width / column;

    SkPoint* texsPit = texsPoint;
    SkScalar y = 0;
#else
    const RSScalar dy = height / row;
    const RSScalar dx = width / column;

    RSPoint* texsPit = texsPoint;
    RSScalar y = 0;
#endif
    for (int i = 0; i <= row; i++) {
        if (i == row) {
            y = height;  // to ensure numerically we hit h exactly
        }
#ifndef USE_ROSEN_DRAWING
        SkScalar x = 0;
        for (int j = 0; j < column; j++) {
            texsPit->set(x, y);
            texsPit += 1;
            x += dx;
        }
        texsPit->set(width, y);
#else
        RSScalar x = 0;
        for (int j = 0; j < column; j++) {
            texsPit->Set(x, y);
            texsPit += 1;
            x += dx;
        }
        texsPit->Set(width, y);
#endif
        texsPit += 1;
        y += dy;
    }

    uint16_t* dex = indices;
    int index = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            *dex++ = index;
            *dex++ = index + column + 1;
            *dex++ = index + column + 2;

            *dex++ = index;
            *dex++ = index + column + 2;
            *dex++ = index + 1;

            index += 1;
        }
        index += 1;
    }
#ifndef USE_ROSEN_DRAWING
    SkPaint tempPaint;
    if (paint) {
        tempPaint = *paint;
    }
    sk_sp<SkShader> shader;
    sk_sp<SkImage> image = SkImage::MakeFromBitmap(bitmap);
#ifdef USE_SYSTEM_SKIA
    shader = image->makeShader(SkShader::kClamp_TileMode, SkShader::kClamp_TileMode);
#elif defined(NEW_SKIA)
    shader = image->makeShader(SkTileMode::kClamp, SkTileMode::kClamp, SkSamplingOptions());
#else
    shader = image->makeShader(SkTileMode::kClamp, SkTileMode::kClamp);
#endif
    tempPaint.setShader(shader);
    skCanvas_->drawVertices(builder.detach(), SkBlendMode::kModulate, tempPaint);
#else
    RSBrush tempBrush;
    if (brush) {
        tempBrush = *brush;
    }
    RSImage image;
    image.BuildFromBitmap(bitmap);
    auto sampling = RSSamplingOptions(RSFilterMode::NEAREST, RSMipmapMode::NEAREST);
    RSMatrix matrix;
    auto shader = RSShaderEffect::CreateImageShader(
        image, RSTileMode::CLAMP, RSTileMode::CLAMP, sampling, matrix);
    tempBrush.SetShaderEffect(shader);
    canvas_->AttachBrush(tempBrush);
    canvas_->DrawVertices(builder, RSBlendMode::MODULATE);
    canvas_->DetachBrush();
#endif // USE_ROSEN_DRAWING
}

} // namespace OHOS::Ace
