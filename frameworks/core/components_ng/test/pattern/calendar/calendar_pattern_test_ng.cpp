/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <optional>
#include <string>

#include "gtest/gtest.h"

#define private public
#define protected public

#include "base/geometry/dimension.h"
#include "base/memory/ace_type.h"
#include "core/components/calendar/calendar_data_adapter.h"
#include "core/components/common/layout/constants.h"
#include "core/components/common/properties/color.h"
#include "core/components_ng/base/frame_node.h"
#include "core/components_ng/base/view_stack_processor.h"
#include "core/components_ng/pattern/calendar/calendar_paint_method.h"
#include "core/components_ng/pattern/calendar/calendar_paint_property.h"
#include "core/components_ng/pattern/calendar/calendar_pattern.h"
#include "core/components_ng/pattern/calendar/calendar_view.h"
#include "core/components_ng/pattern/swiper/swiper_layout_property.h"
#include "core/components_ng/pattern/swiper/swiper_paint_property.h"
#include "core/components_ng/test/mock/theme/mock_theme_manager.h"
#include "core/components_v2/inspector/inspector_constants.h"
#include "core/pipeline/base/element_register.h"
#include "core/pipeline_ng/test/mock/mock_pipeline_base.h"
#include "core/pipeline_ng/ui_task_scheduler.h"

using namespace testing;
using namespace testing::ext;
namespace OHOS::Ace::NG {
struct TestProperty {};
namespace {
const Color COLOR_VALUE = Color(0xffbbffff);
const Dimension SIZE_VALUE = 1.2_px;

const int32_t DAY_VALUE = 1;
const int32_t MONTH_VALUE = 1;
const int32_t YEAR_VALUE = 1;
const int32_t INDEX_VALUE = 1;
const int32_t JUMP_YEAR = 2023;
const int32_t JUMP_MONTH = 3;
const int32_t JUMP_DAY_FIRST = 3;
const int32_t JUMP_DAY_SECOND = 13;
const int32_t JUMP_DAY_THIRD = 23;
const int32_t DEFAULT_FOCUS_RADIUS = 15;
const int32_t WEEKS_COUNT_FIVE = 5;
const int32_t WEEKS_COUNT_SIX = 6;
const int32_t FIRST_DAY_INDEX_VALUE = 1;
const std::string LUNAR_MONTH_VALUE = "五月";
const std::string LUNAR_DAY_VALUE = "初五";
const std::string DAY_MARK = "MARK";
const std::string DAY_MARK_VALUE = "MARK_VALUE";
const std::string OFF_DAYS_VALUE = "OFF_DAYS";

// Day width and Height.
const Dimension DAY_HEIGHT = 48.0_vp;
const Dimension DAY_WIDTH = 48.0_vp;

// GregorianDay YAxis Offset and Height.
const Dimension GREGORIAN_DAY_HEIGHT = 20.0_vp;
const Dimension GREGORIAN_DAY_OFFSET = 4.0_vp;

// Lunar YAxis Offset and Height.
const Dimension LUNAR_DAY_HEIGHT = 10.0_vp;
const Dimension LUNAR_DAY_OFFSET = 2.0_vp;
} // namespace

class CalendarPatternTestNg : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();

protected:
    static RefPtr<FrameNode> CreateCalendarNode(TestProperty& testProperty);
};

void CalendarPatternTestNg::SetUpTestCase()
{
    MockPipelineBase::SetUp();
}

void CalendarPatternTestNg::TearDownTestCase()
{
    MockPipelineBase::TearDown();
}

RefPtr<FrameNode> CalendarPatternTestNg::CreateCalendarNode(TestProperty& testProperty)
{
    return nullptr;
}

/**
 * @tc.name: CalendarViewTest001
 * @tc.desc: Create Calendar.
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarViewTest001, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     *            case: property are all unset
     * @tc.expected: step1. Create Calendar successfully
     */
    CalendarData calendarData;
    CalendarView::Create(calendarData);
    CurrentDayStyle dayStyle;
    CalendarView::SetCurrentDayStyle(dayStyle);
    NonCurrentDayStyle nonCurrentDayStyle;
    CalendarView::SetNonCurrentDayStyle(nonCurrentDayStyle);
    TodayStyle todayStyle;
    CalendarView::SetTodayStyle(todayStyle);
    WeekStyle weekStyle;
    CalendarView::SetWeekStyle(weekStyle);
    WorkStateStyle workStateStyle;
    CalendarView::SetWorkStateStyle(workStateStyle);

    RefPtr<UINode> element = ViewStackProcessor::GetInstance()->Finish();
    EXPECT_EQ(element->GetTag(), V2::CALENDAR_ETS_TAG);
    auto frameNode1 = AceType::DynamicCast<FrameNode>(element);
    auto calendarPattern = frameNode1->GetPattern<CalendarPattern>();
    EXPECT_TRUE(calendarPattern);
    EXPECT_FALSE(calendarPattern->calendarControllerNg_);

    /**
     * @tc.steps: step2. Create Calendar
     *            case: properties are all set
     * @tc.expected: step2. Create Calendar successfully & properties are set successfully
     */
    // properties related with color are all set to COLOR_VALUE, related with size are all set to SIZE_VALUE
    dayStyle.UpdateDayColor(COLOR_VALUE);
    dayStyle.UpdateLunarColor(COLOR_VALUE);
    dayStyle.UpdateMarkLunarColor(COLOR_VALUE);
    dayStyle.UpdateDayFontSize(SIZE_VALUE);
    dayStyle.UpdateLunarDayFontSize(SIZE_VALUE);
    dayStyle.UpdateDayHeight(SIZE_VALUE);
    dayStyle.UpdateDayWidth(SIZE_VALUE);
    dayStyle.UpdateGregorianCalendarHeight(SIZE_VALUE);
    dayStyle.UpdateDayYAxisOffset(SIZE_VALUE);
    dayStyle.UpdateLunarDayYAxisOffset(SIZE_VALUE);
    dayStyle.UpdateUnderscoreXAxisOffset(SIZE_VALUE);
    dayStyle.UpdateUnderscoreYAxisOffset(SIZE_VALUE);
    dayStyle.UpdateScheduleMarkerXAxisOffset(SIZE_VALUE);
    dayStyle.UpdateScheduleMarkerYAxisOffset(SIZE_VALUE);
    dayStyle.UpdateColSpace(SIZE_VALUE);
    dayStyle.UpdateDailyFiveRowSpace(SIZE_VALUE);
    dayStyle.UpdateDailySixRowSpace(SIZE_VALUE);
    dayStyle.UpdateLunarHeight(SIZE_VALUE);
    dayStyle.UpdateUnderscoreWidth(SIZE_VALUE);
    dayStyle.UpdateUnderscoreLength(SIZE_VALUE);
    dayStyle.UpdateScheduleMarkerRadius(SIZE_VALUE);
    dayStyle.UpdateBoundaryRowOffset(SIZE_VALUE);
    dayStyle.UpdateBoundaryColOffset(SIZE_VALUE);

    nonCurrentDayStyle.UpdateNonCurrentMonthDayColor(COLOR_VALUE);
    nonCurrentDayStyle.UpdateNonCurrentMonthLunarColor(COLOR_VALUE);
    nonCurrentDayStyle.UpdateNonCurrentMonthWorkDayMarkColor(COLOR_VALUE);
    nonCurrentDayStyle.UpdateNonCurrentMonthOffDayMarkColor(COLOR_VALUE);

    todayStyle.UpdateFocusedDayColor(COLOR_VALUE);
    todayStyle.UpdateFocusedLunarColor(COLOR_VALUE);
    todayStyle.UpdateFocusedAreaBackgroundColor(COLOR_VALUE);
    todayStyle.UpdateFocusedAreaRadius(SIZE_VALUE);

    weekStyle.UpdateWeekColor(COLOR_VALUE);
    weekStyle.UpdateWeekendDayColor(COLOR_VALUE);
    weekStyle.UpdateWeekendLunarColor(COLOR_VALUE);
    weekStyle.UpdateWeekFontSize(SIZE_VALUE);
    weekStyle.UpdateWeekHeight(SIZE_VALUE);
    weekStyle.UpdateWeekWidth(SIZE_VALUE);
    weekStyle.UpdateWeekAndDayRowSpace(SIZE_VALUE);

    workStateStyle.UpdateWorkDayMarkColor(COLOR_VALUE);
    workStateStyle.UpdateOffDayMarkColor(COLOR_VALUE);
    workStateStyle.UpdateWorkDayMarkSize(SIZE_VALUE);
    workStateStyle.UpdateOffDayMarkSize(SIZE_VALUE);
    workStateStyle.UpdateWorkStateWidth(SIZE_VALUE);
    workStateStyle.UpdateWorkStateHorizontalMovingDistance(SIZE_VALUE);
    workStateStyle.UpdateWorkStateVerticalMovingDistance(SIZE_VALUE);

    // case: controller is not null
    auto calendarControllerNg = AceType::MakeRefPtr<CalendarControllerNg>();
    calendarData.controller = calendarControllerNg;
    CalendarView::Create(calendarData);
    CalendarView::SetCurrentDayStyle(dayStyle);
    CalendarView::SetNonCurrentDayStyle(nonCurrentDayStyle);
    CalendarView::SetTodayStyle(todayStyle);
    CalendarView::SetWeekStyle(weekStyle);
    CalendarView::SetWorkStateStyle(workStateStyle);
    element = ViewStackProcessor::GetInstance()->Finish();

    EXPECT_EQ(element->GetTag(), V2::CALENDAR_ETS_TAG);
    auto frameNode = AceType::DynamicCast<FrameNode>(element);
    ASSERT_TRUE(frameNode);
    calendarPattern = frameNode->GetPattern<CalendarPattern>();
    EXPECT_TRUE(calendarPattern);
    EXPECT_TRUE(calendarPattern->calendarControllerNg_);
    auto swiperNode = frameNode->GetChildren().front();
    ASSERT_TRUE(swiperNode);
    auto calendarFrameNode = AceType::DynamicCast<FrameNode>(swiperNode->GetChildren().front());
    ASSERT_TRUE(calendarFrameNode);
    auto calendarPaintProperty = calendarFrameNode->GetPaintProperty<CalendarPaintProperty>();

    // CurrentDayStyle
    EXPECT_EQ(calendarPaintProperty->GetDayColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetLunarColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetMarkLunarColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDayFontSizeValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetLunarDayFontSizeValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDayHeightValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDayWidthValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetGregorianCalendarHeightValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDayYAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetLunarDayYAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetUnderscoreXAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetUnderscoreYAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetScheduleMarkerXAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetScheduleMarkerYAxisOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetColSpaceValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDailyFiveRowSpaceValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetDailySixRowSpaceValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetLunarHeightValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetUnderscoreWidthValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetUnderscoreLengthValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetScheduleMarkerRadiusValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetBoundaryRowOffsetValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetBoundaryColOffsetValue(1.0_px), SIZE_VALUE);

    // NonCurrentDayStyle
    EXPECT_EQ(calendarPaintProperty->GetNonCurrentMonthDayColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetNonCurrentMonthLunarColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetNonCurrentMonthWorkDayMarkColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetNonCurrentMonthOffDayMarkColorValue(Color::RED), COLOR_VALUE);

    // TodayStyle
    EXPECT_EQ(calendarPaintProperty->GetFocusedDayColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetFocusedLunarColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetFocusedAreaBackgroundColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetFocusedAreaRadiusValue(1.0_px), SIZE_VALUE);

    // WeekStyle
    EXPECT_EQ(calendarPaintProperty->GetWeekColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekendDayColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekendLunarColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekFontSizeValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekHeightValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekWidthValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWeekAndDayRowSpaceValue(1.0_px), SIZE_VALUE);

    // WorkStateStyle
    EXPECT_EQ(calendarPaintProperty->GetWorkDayMarkColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetOffDayMarkColorValue(Color::RED), COLOR_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWorkDayMarkSizeValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetOffDayMarkSizeValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWorkStateWidthValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWorkStateHorizontalMovingDistanceValue(1.0_px), SIZE_VALUE);
    EXPECT_EQ(calendarPaintProperty->GetWorkStateVerticalMovingDistanceValue(1.0_px), SIZE_VALUE);
}

HWTEST_F(CalendarPatternTestNg, CalendarViewTest002, TestSize.Level1)
{
    auto* stack = ViewStackProcessor::GetInstance();
    RefPtr<CalendarPattern> initPattern = AceType::MakeRefPtr<CalendarPattern>();
    RefPtr<FrameNode> frameNode = FrameNode::CreateFrameNode("testNode", 1, initPattern);
    stack->Push(frameNode);
    ObtainedMonth obtainedMonth;
    obtainedMonth.year = YEAR_VALUE;
    obtainedMonth.month = MONTH_VALUE;
    obtainedMonth.firstDayIndex = FIRST_DAY_INDEX_VALUE;
    CalendarView::SetCurrentData(obtainedMonth);
    CalendarView::SetPreData(obtainedMonth);
    CalendarView::SetNextData(obtainedMonth);

    CalendarDay calendarDay;
    calendarDay.index = INDEX_VALUE;
    calendarDay.day = DAY_VALUE;
    calendarDay.weekend = true;
    calendarDay.today = false;
    calendarDay.focused = false;
    calendarDay.touched = true;
    calendarDay.isFirstOfLunar = false;
    calendarDay.hasSchedule = true;
    calendarDay.markLunarDay = false;
    calendarDay.lunarMonth = LUNAR_MONTH_VALUE;
    calendarDay.lunarDay = LUNAR_DAY_VALUE;
    calendarDay.dayMark = DAY_MARK;
    calendarDay.dayMarkValue = DAY_MARK_VALUE;
    CalendarMonth calendarMonth;
    calendarMonth.year = YEAR_VALUE;
    calendarMonth.month = MONTH_VALUE;
    calendarDay.month = calendarMonth;
    CalendarView::SetCalendarDay(calendarDay);
    auto pattern = ViewStackProcessor::GetInstance()->GetMainFrameNode()->GetPattern<CalendarPattern>();

    EXPECT_EQ(pattern->GetCurrentMonthData().year, YEAR_VALUE);
    EXPECT_EQ(pattern->GetCurrentMonthData().month, MONTH_VALUE);
    EXPECT_EQ(pattern->GetCurrentMonthData().firstDayIndex, FIRST_DAY_INDEX_VALUE);
    EXPECT_EQ(pattern->GetPreMonthData().year, YEAR_VALUE);
    EXPECT_EQ(pattern->GetPreMonthData().month, MONTH_VALUE);
    EXPECT_EQ(pattern->GetPreMonthData().firstDayIndex, FIRST_DAY_INDEX_VALUE);
    EXPECT_EQ(pattern->GetNextMonthData().year, YEAR_VALUE);
    EXPECT_EQ(pattern->GetNextMonthData().month, MONTH_VALUE);
    EXPECT_EQ(pattern->GetNextMonthData().firstDayIndex, FIRST_DAY_INDEX_VALUE);

    EXPECT_EQ(pattern->GetCalendarDay().index, INDEX_VALUE);
    EXPECT_EQ(pattern->GetCalendarDay().day, DAY_VALUE);
    EXPECT_TRUE(pattern->GetCalendarDay().weekend);
    EXPECT_FALSE(pattern->GetCalendarDay().today);
    EXPECT_FALSE(pattern->GetCalendarDay().focused);
    EXPECT_TRUE(pattern->GetCalendarDay().touched);
    EXPECT_FALSE(pattern->GetCalendarDay().isFirstOfLunar);
    EXPECT_TRUE(pattern->GetCalendarDay().hasSchedule);
    EXPECT_FALSE(pattern->GetCalendarDay().markLunarDay);
    EXPECT_EQ(pattern->GetCalendarDay().lunarMonth, LUNAR_MONTH_VALUE);
    EXPECT_EQ(pattern->GetCalendarDay().lunarDay, LUNAR_DAY_VALUE);
    EXPECT_EQ(pattern->GetCalendarDay().dayMark, DAY_MARK);
    EXPECT_EQ(pattern->GetCalendarDay().dayMarkValue, DAY_MARK_VALUE);
    EXPECT_EQ(pattern->GetCalendarDay().month.year, YEAR_VALUE);
    EXPECT_EQ(pattern->GetCalendarDay().month.month, MONTH_VALUE);
}

HWTEST_F(CalendarPatternTestNg, CalendarViewTest003, TestSize.Level1)
{
    CalendarData calendarData;
    CalendarView::Create(calendarData);
    CalendarView::SetShowLunar(false);
    CalendarView::SetShowHoliday(false);
    CalendarView::SetNeedSlide(true);
    CalendarView::SetStartOfWeek(Week::Sun);
    CalendarView::SetOffDays(OFF_DAYS_VALUE);
    CalendarView::SetDirection(Axis::HORIZONTAL);

    auto* stack = ViewStackProcessor::GetInstance();
    auto swiperNode = stack->GetMainFrameNode()->GetFirstChild();
    auto swiperFrameNode = AceType::DynamicCast<FrameNode>(swiperNode);
    auto swiperPaintProperty = swiperFrameNode->GetPaintProperty<SwiperPaintProperty>();
    auto swiperLayoutProperty = swiperFrameNode->GetLayoutProperty<SwiperLayoutProperty>();
    auto calendarFrameNode = AceType::DynamicCast<FrameNode>(swiperNode->GetFirstChild());
    auto calendarPaintProperty = calendarFrameNode->GetPaintProperty<CalendarPaintProperty>();

    EXPECT_FALSE(calendarPaintProperty->GetShowLunarValue());
    EXPECT_FALSE(calendarPaintProperty->GetShowHolidayValue());
    EXPECT_FALSE(swiperPaintProperty->GetDisableSwipeValue());
    EXPECT_EQ(calendarPaintProperty->GetStartOfWeekValue(), Week::Sun);
    EXPECT_EQ(calendarPaintProperty->GetOffDaysValue(), OFF_DAYS_VALUE);
    EXPECT_EQ(swiperLayoutProperty->GetDirectionValue(), Axis::HORIZONTAL);
}

/**
 * @tc.name: CalendarTest004
 * @tc.desc: Create calendar, and invoke its JumpTo function to calculate the date.
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarTest004, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     * @tc.expected: step1. Create Calendar successfully.
     */
    auto* stack = ViewStackProcessor::GetInstance();
    RefPtr<CalendarPattern> calendar = AceType::MakeRefPtr<CalendarPattern>();
    RefPtr<FrameNode> frameNode = FrameNode::CreateFrameNode("testNode", 1, calendar);
    stack->Push(frameNode);
    ObtainedMonth obtainedMonth;
    obtainedMonth.year = JUMP_YEAR;
    obtainedMonth.month = JUMP_MONTH;
    obtainedMonth.firstDayIndex = FIRST_DAY_INDEX_VALUE;

    // Add 31 days.
    std::vector<CalendarDay> days;
    for (int32_t i = 0; i < 31; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH;
        day.day = i + 1;
        if (i == 1) {
            day.focused = true;
        }
        days.emplace_back(std::move(day));
    }
    obtainedMonth.days = days;
    CalendarView::SetCurrentData(obtainedMonth);
    CalendarView::SetPreData(obtainedMonth);
    CalendarView::SetNextData(obtainedMonth);

    CalendarDay calendarDay;
    calendarDay.index = INDEX_VALUE;
    calendarDay.day = DAY_VALUE;
    calendarDay.today = false;
    calendarDay.focused = true;
    calendarDay.touched = true;

    /**
     * @tc.steps: step2. Jump to destination from 1 to 3.
     * @tc.expected: step2. Jumped successfully, the focused changed from 1 to 3.
     */
    CalendarMonth calendarMonth;
    calendarMonth.year = JUMP_YEAR;
    calendarMonth.month = JUMP_MONTH;
    calendarDay.month = calendarMonth;
    CalendarView::SetCalendarDay(calendarDay);

    auto pattern = ViewStackProcessor::GetInstance()->GetMainFrameNode()->GetPattern<CalendarPattern>();
    pattern->FireGoToRequestData(JUMP_YEAR, JUMP_MONTH, JUMP_DAY_FIRST);
    pattern->JumpTo(pattern->currentMonth_);
    EXPECT_EQ(pattern->currentMonth_.days[JUMP_DAY_FIRST - 1].focused, true);

    /**
     * @tc.steps: step3. Jump to destination from 3 to 13.
     * @tc.expected: step3. Jumped successfully, the focused changed from 3 to 13.
     */
    pattern->FireGoToRequestData(JUMP_YEAR, JUMP_MONTH, JUMP_DAY_SECOND);
    pattern->JumpTo(pattern->currentMonth_);
    EXPECT_EQ(pattern->currentMonth_.days[JUMP_DAY_SECOND - 1].focused, true);

    /**
     * @tc.steps: step4. Jump to destination from 13 to 23.
     * @tc.expected: step4. Jumped successfully, the focused changed from 13 to 23.
     */
    pattern->FireGoToRequestData(JUMP_YEAR, JUMP_MONTH, JUMP_DAY_THIRD);
    pattern->JumpTo(pattern->currentMonth_);
    EXPECT_EQ(pattern->currentMonth_.days[JUMP_DAY_THIRD - 1].focused, true);

    /**
     * @tc.steps: step5. Jump to destination from 23 to 3.
     * @tc.expected: step5. Jumped successfully, the focused changed from 23 to 3.
     */
    pattern->FireGoToRequestData(JUMP_YEAR, JUMP_MONTH, JUMP_DAY_FIRST);
    pattern->JumpTo(pattern->currentMonth_);
    EXPECT_EQ(pattern->currentMonth_.days[JUMP_DAY_FIRST - 1].focused, true);
}

/**
 * @tc.name: CalendarTest005
 * @tc.desc: Create calendar, and check the 6 rows (42 days).
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarTest005, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     * @tc.expected: step1. Create Calendar successfully.
     */
    auto* stack = ViewStackProcessor::GetInstance();
    RefPtr<CalendarPattern> calendar = AceType::MakeRefPtr<CalendarPattern>();
    RefPtr<FrameNode> frameNode = FrameNode::CreateFrameNode("testNode", 1, calendar);
    stack->Push(frameNode);
    ObtainedMonth obtainedMonth;
    obtainedMonth.year = JUMP_YEAR;
    obtainedMonth.month = JUMP_MONTH;
    obtainedMonth.firstDayIndex = FIRST_DAY_INDEX_VALUE;

    // Add 42 days totally.
    // Add 31 days in March.
    std::vector<CalendarDay> days;
    for (int32_t i = 0; i < 31; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH;
        day.day = i + 1;
        if (i == 1) {
            day.focused = true;
        }
        days.emplace_back(std::move(day));
    }

    // Add 11 days in April.
    for (int32_t i = 31; i < 42; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH + 1;
        day.day = i - 30;
        days.emplace_back(std::move(day));
    }

    obtainedMonth.days = days;
    CalendarView::SetCurrentData(obtainedMonth);
    CalendarView::SetPreData(obtainedMonth);
    CalendarView::SetNextData(obtainedMonth);

    CalendarDay calendarDay;
    calendarDay.index = INDEX_VALUE;
    calendarDay.day = DAY_VALUE;
    calendarDay.today = false;
    calendarDay.focused = true;
    calendarDay.touched = true;

    /**
     * @tc.steps: step2. Set 42 days in a month.
     * @tc.expected: step2. The rows of month is 6.
     */
    CalendarMonth calendarMonth;
    calendarMonth.year = JUMP_YEAR;
    calendarMonth.month = JUMP_MONTH;
    calendarDay.month = calendarMonth;
    CalendarView::SetCalendarDay(calendarDay);

    auto paintMethod = AceType::MakeRefPtr<CalendarPaintMethod>(obtainedMonth, calendarDay);
    RSCanvas rsCanvas;
    paintMethod->DrawWeekAndDates(rsCanvas, Offset(0, 0));
    EXPECT_EQ(paintMethod->rowCount_, WEEKS_COUNT_SIX);
}

/**
 * @tc.name: CalendarTest006
 * @tc.desc: Create calendar, and check the 5 rows (35 days).
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarTest006, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     * @tc.expected: step1. Create Calendar successfully.
     */
    auto* stack = ViewStackProcessor::GetInstance();
    RefPtr<CalendarPattern> calendar = AceType::MakeRefPtr<CalendarPattern>();
    RefPtr<FrameNode> frameNode = FrameNode::CreateFrameNode("testNode", 1, calendar);
    stack->Push(frameNode);
    ObtainedMonth obtainedMonth;
    obtainedMonth.year = JUMP_YEAR;
    obtainedMonth.month = JUMP_MONTH;
    obtainedMonth.firstDayIndex = FIRST_DAY_INDEX_VALUE;

    // Add 35 days totally.
    // Add 31 days in March.
    std::vector<CalendarDay> days;
    for (int32_t i = 0; i < 31; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH;
        day.day = i + 1;
        if (i == 1) {
            day.focused = true;
        }
        days.emplace_back(std::move(day));
    }

    // Add 4 days in April.
    for (int32_t i = 31; i < 35; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH + 1;
        day.day = i - 30;
        days.emplace_back(std::move(day));
    }

    obtainedMonth.days = days;
    CalendarView::SetCurrentData(obtainedMonth);
    CalendarView::SetPreData(obtainedMonth);
    CalendarView::SetNextData(obtainedMonth);

    CalendarDay calendarDay;
    calendarDay.index = INDEX_VALUE;
    calendarDay.day = DAY_VALUE;

    /**
     * @tc.steps: step2. Set 35days in a month.
     * @tc.expected: step2. The rows of month is 5.
     */
    CalendarMonth calendarMonth;
    calendarMonth.year = JUMP_YEAR;
    calendarMonth.month = JUMP_MONTH;
    calendarDay.month = calendarMonth;
    CalendarView::SetCalendarDay(calendarDay);

    auto paintMethod = AceType::MakeRefPtr<CalendarPaintMethod>(obtainedMonth, calendarDay);
    RSCanvas rsCanvas;
    paintMethod->DrawWeekAndDates(rsCanvas, Offset(0, 0));
    EXPECT_EQ(paintMethod->rowCount_, WEEKS_COUNT_FIVE);
}

/**
 * @tc.name: CalendarTest007
 * @tc.desc: Create calendar, and check the todayStyle focus status.
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarTest007, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     * @tc.expected: step1. Create Calendar successfully.
     */
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineBase::GetCurrent()->SetThemeManager(themeManager);
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly(Return(AceType::MakeRefPtr<CalendarTheme>()));

    // Today style.
    TodayStyle todayStyle;
    Color focusedDayColor = Color::WHITE;
    todayStyle.UpdateFocusedDayColor(focusedDayColor);
    Color focusedLunarColor = Color::WHITE;
    todayStyle.UpdateFocusedLunarColor(focusedLunarColor);
    Color focusedAreaBackgroundColor = Color::BLUE;
    todayStyle.UpdateFocusedAreaBackgroundColor(focusedAreaBackgroundColor);
    Dimension focusedAreaRadius = Dimension(DEFAULT_FOCUS_RADIUS, DimensionUnit::VP);
    todayStyle.UpdateFocusedAreaRadius(focusedAreaRadius);

    // Day style of current month.
    CurrentDayStyle dayStyle;
    dayStyle.UpdateDayColor(Color::BLACK);

    CalendarData calendarData;
    auto calendarControllerNg = AceType::MakeRefPtr<CalendarControllerNg>();
    calendarData.controller = calendarControllerNg;
    CalendarView::Create(calendarData);
    CalendarView::SetTodayStyle(todayStyle);
    CalendarView::SetCurrentDayStyle(dayStyle);
    RefPtr<UINode> element = ViewStackProcessor::GetInstance()->Finish();

    EXPECT_EQ(element->GetTag(), V2::CALENDAR_ETS_TAG);
    auto frameNode = AceType::DynamicCast<FrameNode>(element);
    auto calendarPattern = frameNode->GetPattern<CalendarPattern>();
    auto swiperNode = frameNode->GetChildren().front();
    auto calendarFrameNode = AceType::DynamicCast<FrameNode>(swiperNode->GetChildren().front());
    auto calendarPaintProperty = calendarFrameNode->GetPaintProperty<CalendarPaintProperty>();

    ObtainedMonth obtainedMonth;
    obtainedMonth.year = JUMP_YEAR;
    obtainedMonth.month = JUMP_MONTH;
    obtainedMonth.firstDayIndex = FIRST_DAY_INDEX_VALUE;

    // Add 31 days.
    std::vector<CalendarDay> days;
    for (int32_t i = 0; i < 31; i++) {
        CalendarDay day;
        day.index = i;
        day.month.year = JUMP_YEAR;
        day.month.month = JUMP_MONTH;
        day.day = i + 1;
        if (i == 0) {
            day.focused = true;
        }
        days.emplace_back(std::move(day));
    }
    obtainedMonth.days = days;

    CalendarView::SetCurrentData(obtainedMonth);
    CalendarView::SetPreData(obtainedMonth);
    CalendarView::SetNextData(obtainedMonth);

    CalendarDay calendarDay;
    calendarDay.index = INDEX_VALUE;
    calendarDay.day = DAY_VALUE;
    calendarDay.today = false;
    calendarDay.focused = true;
    calendarDay.touched = true;

    /**
     * @tc.steps: step2. Set the first day focused, check the first day text style.
     * @tc.expected: step2. The text color is 0xffffffff.
     */
    CalendarMonth calendarMonth;
    calendarMonth.year = JUMP_YEAR;
    calendarMonth.month = JUMP_MONTH;
    calendarDay.month = calendarMonth;
    CalendarView::SetCalendarDay(calendarDay);

    auto paintMethod = AceType::MakeRefPtr<CalendarPaintMethod>(obtainedMonth, calendarDay);
    RSCanvas rsCanvas;
    paintMethod->SetCalendarTheme(calendarPaintProperty);
    RSTextStyle dateTextStyle;
    RSTextStyle lunarTextStyle;
    paintMethod->InitTextStyle(dateTextStyle, lunarTextStyle);

    paintMethod->SetDayTextStyle(dateTextStyle, lunarTextStyle, obtainedMonth.days[0]);

    EXPECT_EQ(dateTextStyle.color_, RSColor(0xffffffff));
    EXPECT_EQ(lunarTextStyle.color_, RSColor(0xffffffff));

    /**
     * @tc.steps: step3. Set the first day focused, check the second day text style.
     * @tc.expected: step3. The text color is 0xff000000.
     */
    paintMethod->SetDayTextStyle(dateTextStyle, lunarTextStyle, obtainedMonth.days[1]);
    EXPECT_EQ(dateTextStyle.color_, RSColor(0xff000000));
    EXPECT_EQ(lunarTextStyle.color_, RSColor(0xff000000));
}

/**
 * @tc.name: CalendarTest008
 * @tc.desc: Create calendar, and check the gregorianDayYAxisOffset.
 * @tc.type: FUNC
 */
HWTEST_F(CalendarPatternTestNg, CalendarTest008, TestSize.Level1)
{
    /**
     * @tc.steps: step1. Create Calendar
     * @tc.expected: step1. Create Calendar successfully.
     */
    auto themeManager = AceType::MakeRefPtr<MockThemeManager>();
    MockPipelineBase::GetCurrent()->SetThemeManager(themeManager);
    EXPECT_CALL(*themeManager, GetTheme(_)).WillRepeatedly(Return(AceType::MakeRefPtr<CalendarTheme>()));

    // Today style.
    TodayStyle todayStyle;
    Color focusedDayColor = Color::WHITE;
    todayStyle.UpdateFocusedDayColor(focusedDayColor);
    Color focusedLunarColor = Color::WHITE;
    todayStyle.UpdateFocusedLunarColor(focusedLunarColor);
    Color focusedAreaBackgroundColor = Color::BLUE;
    todayStyle.UpdateFocusedAreaBackgroundColor(focusedAreaBackgroundColor);
    Dimension focusedAreaRadius = Dimension(DEFAULT_FOCUS_RADIUS, DimensionUnit::VP);
    todayStyle.UpdateFocusedAreaRadius(focusedAreaRadius);

    // Day style of current month.
    CurrentDayStyle dayStyle;
    dayStyle.UpdateDayColor(Color::BLACK);
    dayStyle.UpdateLunarColor(COLOR_VALUE);
    dayStyle.UpdateDayHeight(DAY_HEIGHT);
    dayStyle.UpdateDayWidth(DAY_WIDTH);
    dayStyle.UpdateGregorianCalendarHeight(GREGORIAN_DAY_HEIGHT);
    dayStyle.UpdateDayYAxisOffset(GREGORIAN_DAY_OFFSET);
    dayStyle.UpdateLunarDayYAxisOffset(LUNAR_DAY_OFFSET);
    dayStyle.UpdateLunarHeight(LUNAR_DAY_HEIGHT);

    CalendarData calendarData;
    auto calendarControllerNg = AceType::MakeRefPtr<CalendarControllerNg>();
    calendarData.controller = calendarControllerNg;
    CalendarView::Create(calendarData);
    CalendarView::SetTodayStyle(todayStyle);
    CalendarView::SetCurrentDayStyle(dayStyle);
    RefPtr<UINode> element = ViewStackProcessor::GetInstance()->Finish();

    EXPECT_EQ(element->GetTag(), V2::CALENDAR_ETS_TAG);
    auto frameNode = AceType::DynamicCast<FrameNode>(element);
    auto calendarPattern = frameNode->GetPattern<CalendarPattern>();
    auto swiperNode = frameNode->GetChildren().front();
    auto calendarFrameNode = AceType::DynamicCast<FrameNode>(swiperNode->GetChildren().front());
    auto calendarPaintProperty = calendarFrameNode->GetPaintProperty<CalendarPaintProperty>();

    /**
     * @tc.steps: step1. Check the offset and height.
     * @tc.expected: step1. Get the offset and height successfully.
     */
    EXPECT_EQ(calendarPaintProperty->GetDayHeightValue(SIZE_VALUE), DAY_HEIGHT);
    EXPECT_EQ(calendarPaintProperty->GetDayWidthValue(SIZE_VALUE), DAY_WIDTH);
    EXPECT_EQ(calendarPaintProperty->GetGregorianCalendarHeightValue(SIZE_VALUE), GREGORIAN_DAY_HEIGHT);
    EXPECT_EQ(calendarPaintProperty->GetDayYAxisOffsetValue(SIZE_VALUE), GREGORIAN_DAY_OFFSET);
    EXPECT_EQ(calendarPaintProperty->GetLunarDayYAxisOffsetValue(SIZE_VALUE), LUNAR_DAY_OFFSET);
    EXPECT_EQ(calendarPaintProperty->GetLunarHeightValue(SIZE_VALUE), LUNAR_DAY_HEIGHT);
}
} // namespace OHOS::Ace::NG
