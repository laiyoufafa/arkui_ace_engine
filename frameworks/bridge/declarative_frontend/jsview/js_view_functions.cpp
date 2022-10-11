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

#include "frameworks/bridge/declarative_frontend/jsview/js_view_functions.h"

#include "base/log/ace_trace.h"
#include "bridge/declarative_frontend/engine/js_ref_ptr.h"
#include "core/components_ng/layout/layout_wrapper.h"
#include "core/pipeline/base/composed_element.h"
#include "frameworks/bridge/declarative_frontend/engine/js_execution_scope_defines.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_view.h"
#include "frameworks/bridge/declarative_frontend/jsview/js_view_measure_layout.h"
#include "frameworks/bridge/declarative_frontend/view_stack_processor.h"

namespace OHOS::Ace::Framework {

#ifdef USE_ARK_ENGINE

namespace {

JSRef<JSObject> GenConstraint(const std::optional<NG::LayoutConstraintF>& parentConstraint)
{
    auto minSize = parentConstraint->minSize;
    auto maxSize = parentConstraint->maxSize;
    JSRef<JSObject> constraint = JSRef<JSObject>::New();
    constraint->SetProperty<double>("minWidth", minSize.Width());
    constraint->SetProperty<double>("minHeight", minSize.Height());
    constraint->SetProperty<double>("maxWidth", maxSize.Width());
    constraint->SetProperty<double>("maxHeight", maxSize.Height());
    return constraint;
}

void CopyPadding(NG::PaddingProperty& padding, const std::unique_ptr<NG::PaddingProperty>& target)
{
    padding.left = target->left;
    padding.right = target->right;
    padding.top = target->top;
    padding.bottom = target->bottom;
}

JSRef<JSObject> GenPadding(const NG::PaddingProperty& padding_)
{
    JSRef<JSObject> padding = JSRef<JSObject>::New();
    padding->SetProperty("top", padding_.top->ToString());
    padding->SetProperty("right", padding_.right->ToString());
    padding->SetProperty("bottom", padding_.bottom->ToString());
    padding->SetProperty("left", padding_.left->ToString());
    return padding;
}

JSRef<JSObject> GenBorderInfo(const RefPtr<NG::LayoutWrapper>& layoutWrapper)
{
    auto borderWidth = layoutWrapper->GetLayoutProperty()->GetBorderWidthProperty()->leftDimen;
    NG::PaddingProperty margin;
    NG::PaddingProperty padding;
    CopyPadding(margin, layoutWrapper->GetLayoutProperty()->GetMarginProperty());
    CopyPadding(padding, layoutWrapper->GetLayoutProperty()->GetPaddingProperty());

    JSRef<JSObject> borderInfo = JSRef<JSObject>::New();
    borderInfo->SetProperty("borderWidth", borderWidth->ToString());
    borderInfo->SetPropertyObject("margin", GenPadding(margin));
    borderInfo->SetPropertyObject("padding", GenPadding(padding));
    return borderInfo;
}

JSRef<JSObject> GenPositionInfo(const RefPtr<NG::LayoutWrapper>& layoutWrapper)
{
    auto offset = layoutWrapper->GetGeometryNode()->GetFrameOffset();
    JSRef<JSObject> position = JSRef<JSObject>::New();
    position->SetProperty("x", offset.GetX());
    position->SetProperty("y", offset.GetY());
    return position;
}

void FillSubComponetProperty(JSRef<JSObject>& info, const RefPtr<NG::LayoutWrapper>& layoutWrapper, const size_t& index)
{
    info->SetProperty<std::string>("name", "");
    info->SetProperty<std::string>("id", std::to_string(index));
    info->SetPropertyObject("constraint", GenConstraint(layoutWrapper->GetGeometryNode()->GetParentLayoutConstraint()));
    info->SetPropertyObject("borderInfo", GenBorderInfo(layoutWrapper));
    info->SetPropertyObject("position", GenPositionInfo(layoutWrapper));
}

JSRef<JSArray> GenLayoutChildArray(std::list<RefPtr<NG::LayoutWrapper>> children)
{
    JSRef<JSArray> childInfo = JSRef<JSArray>::New();
    JSRef<JSFunc> layoutFunc = JSRef<JSFunc>::New<FunctionCallback>(ViewMeasureLayout::JSLayout);
    size_t index = 0;

    for (const auto& iter : children) {
        JSRef<JSObject> info = JSRef<JSObject>::New();
        FillSubComponetProperty(info, iter, index);
        info->SetPropertyObject("layout", layoutFunc);
        childInfo->SetValueAt(index++, info);
    }

    return childInfo;
}

JSRef<JSArray> GenMeasureChildArray(std::list<RefPtr<NG::LayoutWrapper>> children)
{
    JSRef<JSArray> childInfo = JSRef<JSArray>::New();
    JSRef<JSFunc> measureFunc = JSRef<JSFunc>::New<FunctionCallback>(ViewMeasureLayout::JSMeasure);
    size_t index = 0;

    for (const auto& iter : children) {
        JSRef<JSObject> info = JSRef<JSObject>::New();
        FillSubComponetProperty(info, iter, index);
        info->SetPropertyObject("measure", measureFunc);
        childInfo->SetValueAt(index++, info);
    }

    return childInfo;
}

} // namespace

void ViewFunctions::ExecuteLayout(NG::LayoutWrapper* layoutWrapper)
{
    auto children = layoutWrapper->GetAllChildrenWithBuild();
    auto parentConstraint = layoutWrapper->GetGeometryNode()->GetParentLayoutConstraint();
    auto constraint = GenConstraint(parentConstraint);
    auto childArray = GenLayoutChildArray(children);
    JSRef<JSVal> params[2] = { childArray, constraint };

    ViewMeasureLayout::SetLayoutChildren(layoutWrapper->GetAllChildrenWithBuild());
    ViewMeasureLayout::SetDefaultMeasureConstraint(parentConstraint.value());
    jsLayoutFunc_.Lock()->Call(jsObject_.Lock(), 2, params);
}

void ViewFunctions::ExecuteMeasure(NG::LayoutWrapper* layoutWrapper)
{
    auto children = layoutWrapper->GetAllChildrenWithBuild();
    auto parentConstraint = layoutWrapper->GetGeometryNode()->GetParentLayoutConstraint();
    auto constraint = GenConstraint(parentConstraint);
    auto childArray = GenMeasureChildArray(children);
    JSRef<JSVal> params[2];

    params[0] = childArray;
    params[1] = constraint;

    ViewMeasureLayout::SetMeasureChildren(children);
    ViewMeasureLayout::SetDefaultMeasureConstraint(parentConstraint.value());
    jsMeasureFunc_.Lock()->Call(jsObject_.Lock(), 2, params);
}

#else

void ViewFunctions::ExecuteLayout(NG::LayoutWrapper* layoutWrapper)
{
    return;
}

void ViewFunctions::ExecuteMeasure(NG::LayoutWrapper* layoutWrapper)
{
    return;
}

#endif

void ViewFunctions::InitViewFunctions(
    const JSRef<JSObject>& jsObject, const JSRef<JSFunc>& jsRenderFunction, bool partialUpdate)
{
    jsObject_ = jsObject;

    if (partialUpdate) {
        if (jsObject->GetProperty("initialRender")->IsFunction()) {
            JSRef<JSVal> jsRenderFunc = jsObject->GetProperty("initialRenderView");
            if (jsRenderFunc->IsFunction()) {
                jsRenderFunc_ = JSRef<JSFunc>::Cast(jsRenderFunc);
            } else {
                LOGE("View lacks mandatory 'initialRenderView()' function, fatal internal error.");
            }
        } else {
            LOGE("View lacks mandatory 'initialRender()' function, fatal internal error.");
        }

        JSRef<JSVal> jsRerenderFunc = jsObject->GetProperty("rerender");
        if (jsRerenderFunc->IsFunction()) {
            jsRerenderFunc_ = JSRef<JSFunc>::Cast(jsRerenderFunc);
        } else {
            LOGE("View lacks mandatory 'rerender()' function, fatal internal error.");
        }
    }

    JSRef<JSVal> jsAppearFunc = jsObject->GetProperty("aboutToAppear");
    if (jsAppearFunc->IsFunction()) {
        jsAppearFunc_ = JSRef<JSFunc>::Cast(jsAppearFunc);
    }

    JSRef<JSVal> jsDisappearFunc = jsObject->GetProperty("aboutToDisappear");
    if (jsDisappearFunc->IsFunction()) {
        jsDisappearFunc_ = JSRef<JSFunc>::Cast(jsDisappearFunc);
    } else {
        LOGD("aboutToDisappear is not a function");
    }

    JSRef<JSVal> jsLayoutFunc = jsObject->GetProperty("onLayout");
    if (jsLayoutFunc->IsFunction()) {
        jsLayoutFunc_ = JSRef<JSFunc>::Cast(jsLayoutFunc);
    }

    JSRef<JSVal> jsMeasureFunc = jsObject->GetProperty("onMeasure");
    if (jsMeasureFunc->IsFunction()) {
        jsMeasureFunc_ = JSRef<JSFunc>::Cast(jsMeasureFunc);
    }

    JSRef<JSVal> jsAboutToBeDeletedFunc = jsObject->GetProperty("aboutToBeDeleted");
    if (jsAboutToBeDeletedFunc->IsFunction()) {
        jsAboutToBeDeletedFunc_ = JSRef<JSFunc>::Cast(jsAboutToBeDeletedFunc);
    } else {
        LOGD("aboutToBeDeleted is not a function");
    }

    JSRef<JSVal> jsAboutToRenderFunc = jsObject->GetProperty("aboutToRender");
    if (jsAboutToRenderFunc->IsFunction()) {
        jsAboutToRenderFunc_ = JSRef<JSFunc>::Cast(jsAboutToRenderFunc);
    } else {
        LOGD("aboutToRender is not a function");
    }

    JSRef<JSVal> jsRenderDoneFunc = jsObject->GetProperty("onRenderDone");
    if (jsRenderDoneFunc->IsFunction()) {
        jsRenderDoneFunc_ = JSRef<JSFunc>::Cast(jsRenderDoneFunc);
    } else {
        LOGD("onRenderDone is not a function");
    }

    JSRef<JSVal> jsAboutToBuildFunc = jsObject->GetProperty("aboutToBuild");
    if (jsAboutToBuildFunc->IsFunction()) {
        jsAboutToBuildFunc_ = JSRef<JSFunc>::Cast(jsAboutToBuildFunc);
    } else {
        LOGD("aboutToBuild is not a function");
    }

    JSRef<JSVal> jsBuildDoneFunc = jsObject->GetProperty("onBuildDone");
    if (jsBuildDoneFunc->IsFunction()) {
        jsBuildDoneFunc_ = JSRef<JSFunc>::Cast(jsBuildDoneFunc);
    } else {
        LOGD("onBuildDone is not a function");
    }

    JSRef<JSVal> jsTransitionFunc = jsObject->GetProperty("pageTransition");
    if (jsTransitionFunc->IsFunction()) {
        jsTransitionFunc_ = JSRef<JSFunc>::Cast(jsTransitionFunc);
    } else {
        LOGD("transition is not a function");
    }

    JSRef<JSVal> jsOnHideFunc = jsObject->GetProperty("onPageHide");
    if (jsOnHideFunc->IsFunction()) {
        jsOnHideFunc_ = JSRef<JSFunc>::Cast(jsOnHideFunc);
    } else {
        LOGD("onHide is not a function");
    }

    JSRef<JSVal> jsOnShowFunc = jsObject->GetProperty("onPageShow");
    if (jsOnShowFunc->IsFunction()) {
        jsOnShowFunc_ = JSRef<JSFunc>::Cast(jsOnShowFunc);
    } else {
        LOGD("onShow is not a function");
    }

    JSRef<JSVal> jsBackPressFunc = jsObject->GetProperty("onBackPress");
    if (jsBackPressFunc->IsFunction()) {
        jsBackPressFunc_ = JSRef<JSFunc>::Cast(jsBackPressFunc);
    } else {
        LOGD("onBackPress is not a function");
    }

    if (!partialUpdate) {
        JSRef<JSVal> jsUpdateWithValueParamsFunc = jsObject->GetProperty("updateWithValueParams");
        if (jsUpdateWithValueParamsFunc->IsFunction()) {
            LOGD("updateWithValueParams is a function");
            jsUpdateWithValueParamsFunc_ = JSRef<JSFunc>::Cast(jsUpdateWithValueParamsFunc);
        } else {
            LOGD("updateWithValueParams is not a function");
        }
        jsRenderFunc_ = jsRenderFunction;
    }
}

ViewFunctions::ViewFunctions(const JSRef<JSObject>& jsObject, const JSRef<JSFunc>& jsRenderFunction)
{
    ACE_DCHECK(jsObject);
    InitViewFunctions(jsObject, jsRenderFunction, false);
}

void ViewFunctions::ExecuteRender()
{
    if (jsRenderFunc_.IsEmpty()) {
        LOGE("no render function in View!");
        return;
    }

    auto func = jsRenderFunc_.Lock();
    JSRef<JSVal> jsThis = jsObject_.Lock();
    jsRenderResult_ = func->Call(jsThis);
}

void ViewFunctions::ExecuteAppear()
{
    ExecuteFunction(jsAppearFunc_, "aboutToAppear");
}

void ViewFunctions::ExecuteDisappear()
{
    ExecuteFunction(jsDisappearFunc_, "aboutToDisappear");
}

bool ViewFunctions::HasLayout() const
{
    return !jsLayoutFunc_.IsEmpty();
}

bool ViewFunctions::HasMeasure() const
{
    return !jsMeasureFunc_.IsEmpty();
}

void ViewFunctions::ExecuteAboutToBeDeleted()
{
    ExecuteFunction(jsAboutToBeDeletedFunc_, "aboutToDisappear");
}

void ViewFunctions::ExecuteAboutToRender()
{
    // for developer callback.
    ExecuteFunction(jsAboutToBuildFunc_, "aboutToBuild");
    // for state manager mark rendering progress.
    ExecuteFunction(jsAboutToRenderFunc_, "aboutToRender");
}

void ViewFunctions::ExecuteOnRenderDone()
{
    // for state manager reset rendering progress.
    ExecuteFunction(jsRenderDoneFunc_, "onRenderDone");
    // for developer callback.
    ExecuteFunction(jsBuildDoneFunc_, "onBuildDone");
}

void ViewFunctions::ExecuteTransition()
{
    ExecuteFunction(jsTransitionFunc_, "pageTransition");
}

bool ViewFunctions::HasPageTransition() const
{
    return !jsTransitionFunc_.IsEmpty();
}

void ViewFunctions::ExecuteShow()
{
    ExecuteFunction(jsOnShowFunc_, "onPageShow");
}

void ViewFunctions::ExecuteHide()
{
    ExecuteFunction(jsOnHideFunc_, "onPageHide");
}

// Method not needed for Partial Update code path
void ViewFunctions::ExecuteUpdateWithValueParams(const std::string& jsonData)
{
    ExecuteFunctionWithParams(jsUpdateWithValueParamsFunc_, "updateWithValueParams", jsonData);
}

bool ViewFunctions::ExecuteOnBackPress()
{
    auto ret = ExecuteFunctionWithReturn(jsBackPressFunc_, "onBackPress");
    if (!ret->IsEmpty() && ret->IsBoolean()) {
        return ret->ToBoolean();
    }
    return false;
}

void ViewFunctions::ExecuteFunction(JSWeak<JSFunc>& func, const char* debugInfo)
{
    JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(context_)
    if (func.IsEmpty()) {
        LOGD("View doesn't have %{public}s() method!", debugInfo);
        return;
    }
    ACE_SCOPED_TRACE("%s", debugInfo);
    JSRef<JSVal> jsObject = jsObject_.Lock();
    func.Lock()->Call(jsObject);
}

JSRef<JSVal> ViewFunctions::ExecuteFunctionWithReturn(JSWeak<JSFunc>& func, const char* debugInfo)
{
    JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(context_, JSRef<JSVal>::Make())
    if (func.IsEmpty()) {
        LOGD("View doesn't have %{public}s() method!", debugInfo);
        return JSRef<JSVal>::Make();
    }
    ACE_SCOPED_TRACE("%s", debugInfo);
    JSRef<JSVal> jsObject = jsObject_.Lock();
    JSRef<JSVal> result = func.Lock()->Call(jsObject);
    if (result.IsEmpty()) {
        LOGE("Error calling %{public}s", debugInfo);
    }
    return result;
}

void ViewFunctions::ExecuteFunctionWithParams(JSWeak<JSFunc>& func, const char* debugInfo, const std::string& jsonData)
{
    JAVASCRIPT_EXECUTION_SCOPE_WITH_CHECK(context_)
    if (func.IsEmpty()) {
        LOGD("View doesn't have %{public}s() method!", debugInfo);
        return;
    }

    JSRef<JSObject> obj = JSRef<JSObject>::New();
    JSRef<JSVal> param = obj->ToJsonObject(jsonData.c_str());

    JSRef<JSVal> jsObject = jsObject_.Lock();
    JSRef<JSVal> result = func.Lock()->Call(jsObject, 1, &param);
    if (result.IsEmpty()) {
        LOGE("Error calling %{public}s", debugInfo);
    }
}

// Baseline version of Destroy
void ViewFunctions::Destroy(JSView* parentCustomView)
{
    LOGD("Destroy");
    // Might be called from parent view, before any result has been produced??
    if (jsRenderResult_.IsEmpty()) {
        LOGD("ViewFunctions::Destroy() -> no previous render result to delete");
        return;
    }

    auto renderRes = jsRenderResult_.Lock();
    if (renderRes.IsEmpty() || !renderRes->IsObject()) {
        LOGD("ViewFunctions::Destroy() -> result not an object");
        return;
    }

    JSRef<JSObject> obj = JSRef<JSObject>::Cast(renderRes);
    if (!obj.IsEmpty()) {
        // jsRenderResult_ maybe an js exception, not a JSView
        JSView* view = obj->Unwrap<JSView>();
        if (view != nullptr) {
            view->Destroy(parentCustomView);
        }
    }
    jsRenderResult_.Reset();
    LOGD("ViewFunctions::Destroy() end");
}

// PartialUpdate version of Destroy
void ViewFunctions::Destroy()
{
    LOGD("Destroy");

    // Might be called from parent view, before any result has been produced??
    if (jsRenderResult_.IsEmpty()) {
        LOGD("ViewFunctions::Destroy() -> no previous render result to delete");
        return;
    }

    auto renderRes = jsRenderResult_.Lock();
    if (renderRes.IsEmpty() || !renderRes->IsObject()) {
        LOGD("ViewFunctions::Destroy() -> result not an object");
        return;
    }

    // merging: when would a render function return a JSView ?
    JSRef<JSObject> obj = JSRef<JSObject>::Cast(renderRes);
    if (!obj.IsEmpty()) {
        // jsRenderResult_ maybe an js exception, not a JSView
        JSView* view = obj->Unwrap<JSView>();
        if (view != nullptr) {
            LOGE("NOTE NOTE NOTE render returned a JSView object that's dangling!");
        }
    }
    jsObject_.Reset();
    jsRenderResult_.Reset();

    LOGD("ViewFunctions::Destroy() end");
}

// Partial update method
void ViewFunctions::ExecuteRerender()
{
    if (jsRerenderFunc_.IsEmpty()) {
        LOGE("no rerender function in View!");
        return;
    }

    auto func = jsRerenderFunc_.Lock();
    JSRef<JSVal> jsThis = jsObject_.Lock();
    jsRenderResult_ = func->Call(jsThis);
}

// Partial update method
ViewFunctions::ViewFunctions(const JSRef<JSObject>& jsObject)
{
    InitViewFunctions(jsObject, JSRef<JSFunc>(), true);
}

} // namespace OHOS::Ace::Framework
