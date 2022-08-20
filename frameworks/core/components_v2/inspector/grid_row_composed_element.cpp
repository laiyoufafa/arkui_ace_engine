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

#include "core/components_v2/inspector/grid_row_composed_element.h"

#include "base/log/dump_log.h"
#include "core/components_v2/grid_layout/grid_container_utils.h"
#include "core/components_v2/grid_layout/grid_row_element.h"
#include "core/components_v2/grid_layout/render_grid_row.h"
#include "core/components_v2/inspector/utils.h"

namespace OHOS::Ace::V2 {
namespace {

const std::unordered_map<std::string,
    std::function<std::unique_ptr<JsonValue>(const GridRowComposedElement&)>> CREATE_JSON_MAP {
    { "constructor", [](const GridRowComposedElement& inspector) { return inspector.GetConstructor(); } }
};

} // namespace

void GridRowComposedElement::Dump()
{
    InspectorComposedElement::Dump();
    DumpLog::GetInstance().AddDesc(
        std::string("gutter: ").append(GetGridRowGutter()));
    DumpLog::GetInstance().AddDesc(
        std::string("columns: ").append(GetGridRowColumns()));
    DumpLog::GetInstance().AddDesc(
        std::string("breakpoints: ").append(GetGridRowBreakpoints()));
    DumpLog::GetInstance().AddDesc(
        std::string("direction: ").append(GetGridRowDirection()));
}

std::unique_ptr<JsonValue> GridRowComposedElement::ToJsonObject() const
{
    auto resultJson = InspectorComposedElement::ToJsonObject();
    for (const auto& value : CREATE_JSON_MAP) {
        resultJson->Put(value.first.c_str(), value.second(*this));
    }
    return resultJson;
}

std::unique_ptr<JsonValue> GridRowComposedElement::GetConstructor() const
{
    auto jsonValue = JsonUtil::Create(true);
    jsonValue->Put("gutter", GetGridRowGutter().c_str());
    jsonValue->Put("columns", GetGridRowColumns().c_str());
    jsonValue->Put("breakpoints", GetGridRowBreakpoints().c_str());
    jsonValue->Put("direction", GetGridRowDirection().c_str());
    return jsonValue;
}


std::string GridRowComposedElement::GetGridRowSizeType() const
{
    auto renderNode = GetInspectorNode(GridRowElement::TypeId());
    if (!renderNode) {
        return "-";
    }
    auto renderGridRow = AceType::DynamicCast<RenderGridRow>(renderNode);
    if (!renderGridRow) {
        return "-";
    }
    auto sizeType = renderGridRow->GetCurrentSizeType();
    switch (sizeType) {
        case GridSizeType::UNDEFINED:
            return std::string("SizeType.Auto");
        case GridSizeType::XS:
            return std::string("SizeType.XS");
        case GridSizeType::SM:
            return std::string("SizeType.SM");
        case GridSizeType::MD:
            return std::string("SizeType.MD");
        case GridSizeType::LG:
            return std::string("SizeType.LG");
        default:
            break;
    }
    
    return std::string("SizeType.Auto");
}

std::string GridRowComposedElement::GetGridRowDirection() const
{
    auto renderNode = GetInspectorNode(GridRowElement::TypeId());
    if (!renderNode) {
        return "-";
    }
    auto renderGridRow = AceType::DynamicCast<RenderGridRow>(renderNode);
    if (!renderGridRow) {
        return "-";
    }
    auto direction = renderGridRow->GetDirection();
    switch (direction) {
        case GridRowDirection::Row:
            return "Row";
        case GridRowDirection::RowReverse:
            return "RowReverse";
        default:
            break;
    }
    return "Unknown";
}

std::string GridRowComposedElement::GetGridRowBreakpoints() const
{
    auto renderNode = GetInspectorNode(GridRowElement::TypeId());
    if (!renderNode) {
        return "[]";
    }
    auto renderGridRow = AceType::DynamicCast<RenderGridRow>(renderNode);
    if (!renderGridRow) {
        return "[]";
    }
    auto breakpointPtr = renderGridRow->GetBreakPoints();
    if (!breakpointPtr) {
        return "[";
    }
    std::string result = "[";
    for (auto & breakpoint : breakpointPtr->breakpoints) {
        result.append(breakpoint);
        result.append(", ");
    }
    if (result.size() > 1) {
        result = result.substr(0, result.size() - 1).append("]");
    } else {
        result = result.append("]");
    }
    return result;
}

std::string GridRowComposedElement::GetGridRowColumns() const
{
    auto renderNode = GetInspectorNode(GridRowElement::TypeId());
    if (!renderNode) {
        return "-";
    }
    auto renderGridRow = AceType::DynamicCast<RenderGridRow>(renderNode);
    if (!renderGridRow) {
        return "-";
    }
    auto totalColumn = renderGridRow->GetTotalCol();
    return std::to_string(totalColumn);
}

std::string GridRowComposedElement::GetGridRowGutter() const
{
    auto renderNode = GetInspectorNode(GridRowElement::TypeId());
    if (!renderNode) {
        return "-";
    }
    auto renderGridRow = AceType::DynamicCast<RenderGridRow>(renderNode);
    if (!renderGridRow) {
        return "-";
    }
    auto gutter = renderGridRow->GetGutter();
    std::string result = "<";
    result.append(std::to_string(gutter.first));
    result.append(", ");
    result.append(std::to_string(gutter.second));
    result.append(">");
    return result;
}

void GridRowComposedElement::AddChildWithSlot(int32_t slot, const RefPtr<Component>& newComponent)
{
    auto gridRowElement = GetContentElement<GridRowElement>(GridRowElement::TypeId());
    if (!gridRowElement) {
        LOGE("get GetStackElement failed");
        return;
    }
    gridRowElement->UpdateChildWithSlot(nullptr, newComponent, slot, slot);
    gridRowElement->MarkDirty();
}

void GridRowComposedElement::UpdateChildWithSlot(int32_t slot, const RefPtr<Component>& newComponent)
{
    auto gridRowElement = GetContentElement<GridRowElement>(GridRowElement::TypeId());
    if (!gridRowElement) {
        LOGE("get GetStackElement failed");
        return;
    }
    auto child = gridRowElement->GetChildBySlot(slot);
    gridRowElement->UpdateChildWithSlot(child, newComponent, slot, slot);
    gridRowElement->MarkDirty();
}

void GridRowComposedElement::DeleteChildWithSlot(int32_t slot)
{
    auto gridRowElement = GetContentElement<GridRowElement>(GridRowElement::TypeId());
    if (!gridRowElement) {
        LOGE("get GetStackElement failed");
        return;
    }
    auto child = gridRowElement->GetChildBySlot(slot);
    gridRowElement->UpdateChildWithSlot(child, nullptr, slot, slot);
    gridRowElement->MarkDirty();
}

} // namespace OHOS::Ace::V2