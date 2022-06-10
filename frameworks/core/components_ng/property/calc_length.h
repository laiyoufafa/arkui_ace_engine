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

#ifndef FOUNDATION_ACE_FRAMEWORKS_COMPONENTS_NG_PROPERTIES_CALC_LENGTH_H
#define FOUNDATION_ACE_FRAMEWORKS_COMPONENTS_NG_PROPERTIES_CALC_LENGTH_H

#include "base/geometry/dimension.h"
#include "base/utils/utils.h"

namespace OHOS::Ace::NG {
struct ScaleProperty {
    double vpScale { 0.0 };
    double fpScale { 0.0 };
    double lpxScale { 0.0 };

    void Reset()
    {
        vpScale = 0.0;
        fpScale = 0.0;
        lpxScale = 0.0;
    }

    bool operator==(const ScaleProperty& scaleProperty) const
    {
        return (vpScale == scaleProperty.vpScale) && (fpScale == scaleProperty.fpScale) &&
               (lpxScale == scaleProperty.lpxScale);
    }

    bool operator!=(const ScaleProperty& scaleProperty) const
    {
        return !(*this == scaleProperty);
    }
};

class CalcLength {
public:
    CalcLength() = default;
    explicit CalcLength(const std::string& value)
    {
        calcValue_ = value;
    }
    ~CalcLength() = default;

    explicit CalcLength(double value, DimensionUnit unit = DimensionUnit::PX) : dimension_(value, unit) {};
    explicit CalcLength(const Dimension& dimension) : dimension_(dimension) {};

    void Reset()
    {
        calcValue_ = "";
        dimension_.Reset();
    }

    const std::string& CalcValue() const
    {
        return calcValue_;
    }

    void SetCalcValue(const std::string& value)
    {
        calcValue_ = value;
    }

    bool NormalizeToPx(double vpScale, double fpScale, double lpxScale, double parentLength, double& result) const;

    bool IsValid() const
    {
        if (calcValue_.empty()) {
            return dimension_.IsNonNegative();
        }
        return true;
    }

    bool operator==(const CalcLength& length) const
    {
        if (calcValue_.empty() && length.calcValue_.empty()) {
            return dimension_ == length.dimension_;
        }
        return calcValue_ == length.calcValue_;
    }

    bool operator!=(const CalcLength& length) const
    {
        return !(*this == length);
    }

    std::string ToString() const
    {
        if (calcValue_.empty()) {
            return dimension_.ToString();
        }
        return calcValue_;
    }

private:
    std::string calcValue_;
    Dimension dimension_;
};
} // namespace OHOS::Ace::NG

#endif // FOUNDATION_ACE_FRAMEWORKS_COMPONENTS_NG_PROPERTIES_CALC_LENGTH_H
