/*
 * MIT License
 *
 * Copyright (c) 2025 nfx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file Sample_DateTime.cpp
 * @brief Demonstrates DateTime, TimeSpan, and DateTimeOffset usage
 * @details This sample shows how to use the nfx-datetime library for high-precision
 *          temporal operations, ISO 8601 parsing/formatting, and timezone handling
 */

#include <iostream>
#include <string>

#include <nfx/datetime/DateTime.h>
#include <nfx/datetime/DateTimeOffset.h>
#include <nfx/datetime/TimeSpan.h>

int main()
{
	using namespace nfx::time;

	std::cout << "=== nfx-datetime Library ===\n\n";

	//=====================================================================
	// 1. DateTime: Basic construction
	//=====================================================================
	{
		std::cout << "1. DateTime: Basic construction\n";
		std::cout << "--------------------------------\n";

		// DateTime can represent both local and UTC time
		DateTime localNow{ DateTime::now() };  // Current LOCAL time (system timezone)
		DateTime utcNow{ DateTime::utcNow() }; // Current UTC time
		DateTime today{ DateTime::today() };   // Today's date at midnight (LOCAL)
		DateTime specific{ 2024, 6, 15, 14, 30, 45 };
		DateTime epoch{ DateTime::epoch() };

		std::cout << "Current LOCAL time:    " << localNow.toString() << "\n";
		std::cout << "Current UTC time:      " << utcNow.toString() << "\n";
		std::cout << "Today at midnight:     " << today.toString() << "\n";
		std::cout << "Specific date/time:    " << specific.toString() << "\n";
		std::cout << "Unix epoch:            " << epoch.toString() << "\n";
		std::cout << "Note: DateTime is always UTC (no timezone offset)\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 2. DateTime: Parsing ISO 8601 strings
	//=====================================================================
	{
		std::cout << "2. DateTime: Parsing ISO 8601 strings\n";
		std::cout << "--------------------------------------\n";

		DateTime dt1{ DateTime{ "2024-06-15T14:30:45Z" } };
		DateTime dt2{ DateTime{ "2024-12-25" } };

		std::cout << "Parsed \"2024-06-15T14:30:45Z\":  " << dt1.toString() << "\n";
		std::cout << "Parsed \"2024-12-25\":            " << dt2.toString() << "\n";

		// Safe parsing with fromString
		DateTime result;
		bool success{ DateTime::fromString( "2024-01-01T00:00:00Z", result ) };
		std::cout << "fromString success:             " << ( success ? "true" : "false" ) << "\n";
		if ( success )
		{
			std::cout << "  Parsed value:                 " << result.toString() << "\n";
		}

		std::cout << "\n";
	}

	//=====================================================================
	// 3. DateTime: Accessing components
	//=====================================================================
	{
		std::cout << "3. DateTime: Accessing components\n";
		std::cout << "----------------------------------\n";

		DateTime dt{ 2024, 6, 15, 14, 30, 45, 123 };
		std::cout << "DateTime: " << dt.toString( DateTime::Format::Iso8601Precise ) << "\n";
		std::cout << "  Year:         " << dt.year() << "\n";
		std::cout << "  Month:        " << dt.month() << "\n";
		std::cout << "  Day:          " << dt.day() << "\n";
		std::cout << "  Hour:         " << dt.hour() << "\n";
		std::cout << "  Minute:       " << dt.minute() << "\n";
		std::cout << "  Second:       " << dt.second() << "\n";
		std::cout << "  Millisecond:  " << dt.millisecond() << "\n";
		std::cout << "  Day of week:  " << dt.dayOfWeek() << " (0=Sunday)\n";
		std::cout << "  Day of year:  " << dt.dayOfYear() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 4. DateTime: Arithmetic operations
	//=====================================================================
	{
		std::cout << "4. DateTime: Arithmetic operations\n";
		std::cout << "-----------------------------------\n";

		DateTime start{ 2024, 6, 15, 10, 0, 0 };
		std::cout << "Start time:              " << start.toString() << "\n";

		DateTime later{ start + TimeSpan::fromHours( 2.5 ) };
		std::cout << "After +2.5 hours:        " << later.toString() << "\n";

		DateTime earlier{ start - TimeSpan::fromMinutes( 30.0 ) };
		std::cout << "After -30 minutes:       " << earlier.toString() << "\n";

		TimeSpan difference{ later - start };
		std::cout << "Difference:              " << difference.toString() << " (" << difference.hours() << " hours)\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 5. DateTime: Comparisons
	//=====================================================================
	{
		std::cout << "5. DateTime: Comparisons\n";
		std::cout << "------------------------\n";

		DateTime dt1{ 2024, 6, 15, 10, 0, 0 };
		DateTime dt2{ 2024, 6, 15, 14, 0, 0 };
		DateTime dt3{ 2024, 6, 15, 10, 0, 0 };

		std::cout << std::boolalpha;
		std::cout << "dt1 == dt3:  " << ( dt1 == dt3 ) << "\n";
		std::cout << "dt1 != dt2:  " << ( dt1 != dt2 ) << "\n";
		std::cout << "dt1 < dt2:   " << ( dt1 < dt2 ) << "\n";
		std::cout << "dt2 > dt1:   " << ( dt2 > dt1 ) << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 6. DateTime: Epoch timestamp conversions
	//=====================================================================
	{
		std::cout << "6. DateTime: Epoch timestamp conversions\n";
		std::cout << "----------------------------------------\n";

		DateTime dt{ 2024, 1, 1, 0, 0, 0 };
		std::cout << "DateTime:              " << dt.toString() << "\n";
		std::cout << "Unix seconds:          " << dt.toEpochSeconds() << "\n";
		std::cout << "Unix milliseconds:     " << dt.toEpochMilliseconds() << "\n";

		DateTime fromEpoch{ DateTime::fromEpochSeconds( 1704067200 ) };
		std::cout << "From epoch 1704067200: " << fromEpoch.toString() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 7. DateTime: Leap year handling
	//=====================================================================
	{
		std::cout << "7. DateTime: Leap year handling\n";
		std::cout << "--------------------------------\n";

		std::cout << std::boolalpha;
		std::cout << "2024 is leap year:    " << DateTime::isLeapYear( 2024 ) << "\n";
		std::cout << "2023 is leap year:    " << DateTime::isLeapYear( 2023 ) << "\n";
		std::cout << "Days in Feb 2024:     " << DateTime::daysInMonth( 2024, 2 ) << "\n";
		std::cout << "Days in Feb 2023:     " << DateTime::daysInMonth( 2023, 2 ) << "\n";

		DateTime leapDay{ 2024, 2, 29, 12, 0, 0 };
		std::cout << "Feb 29, 2024:         " << leapDay.toString() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 8. TimeSpan: Creating durations
	//=====================================================================
	{
		std::cout << "8. TimeSpan: Creating durations\n";
		std::cout << "--------------------------------\n";

		TimeSpan days{ TimeSpan::fromDays( 1.5 ) };
		TimeSpan hours{ TimeSpan::fromHours( 2.5 ) };
		TimeSpan minutes{ TimeSpan::fromMinutes( 90.0 ) };
		TimeSpan seconds{ TimeSpan::fromSeconds( 3600.0 ) };

		std::cout << "1.5 days:       " << days.toString() << "\n";
		std::cout << "2.5 hours:      " << hours.toString() << "\n";
		std::cout << "90 minutes:     " << minutes.toString() << "\n";
		std::cout << "3600 seconds:   " << seconds.toString() << "\n";
		std::cout << "Note: ISO 8601 duration format (PT = Period Time)\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 9. TimeSpan: Parsing ISO 8601 durations
	//=====================================================================
	{
		std::cout << "9. TimeSpan: Parsing ISO 8601 durations\n";
		std::cout << "----------------------------------------\n";

		TimeSpan ts1{ TimeSpan{ "PT1H" } };
		TimeSpan ts2{ TimeSpan{ "PT1H30M45S" } };
		TimeSpan ts3{ TimeSpan{ "P1DT12H" } };
		TimeSpan ts4{ TimeSpan{ "-PT2H30M" } };

		std::cout << "PT1H:           " << ts1.hours() << " hours\n";
		std::cout << "PT1H30M45S:     " << ts2.seconds() << " seconds\n";
		std::cout << "P1DT12H:        " << ts3.hours() << " hours\n";
		std::cout << "-PT2H30M:       " << ts4.minutes() << " minutes\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 10. TimeSpan: Unit conversions
	//=====================================================================
	{
		std::cout << "10. TimeSpan: Unit conversions\n";
		std::cout << "-------------------------------\n";

		TimeSpan duration{ TimeSpan::fromHours( 2.5 ) };
		std::cout << "Duration: 2.5 hours\n";
		std::cout << "  Total days:          " << duration.days() << "\n";
		std::cout << "  Total hours:         " << duration.hours() << "\n";
		std::cout << "  Total minutes:       " << duration.minutes() << "\n";
		std::cout << "  Total seconds:       " << duration.seconds() << "\n";
		std::cout << "  Total milliseconds:  " << duration.milliseconds() << "\n";
		std::cout << "  Ticks (100ns):       " << duration.ticks() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 11. TimeSpan: Arithmetic operations
	//=====================================================================
	{
		std::cout << "11. TimeSpan: Arithmetic operations\n";
		std::cout << "------------------------------------\n";

		TimeSpan ts1{ TimeSpan::fromHours( 2.0 ) };
		TimeSpan ts2{ TimeSpan::fromMinutes( 30.0 ) };

		std::cout << "ts1 (2 hours):    " << ts1.toString() << "\n";
		std::cout << "ts2 (30 minutes): " << ts2.toString() << "\n";
		std::cout << "ts1 + ts2:        " << ( ts1 + ts2 ).toString() << "\n";
		std::cout << "ts1 - ts2:        " << ( ts1 - ts2 ).toString() << "\n";
		std::cout << "-ts1:             " << ( -ts1 ).toString() << "\n";

		TimeSpan complex{ TimeSpan::fromDays( 1.0 ) + TimeSpan::fromHours( 3.0 ) + TimeSpan::fromMinutes( 30.0 ) };
		std::cout << "1d + 3h + 30m:    " << complex.toString() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 12. TimeSpan: Comparisons
	//=====================================================================
	{
		std::cout << "12. TimeSpan: Comparisons\n";
		std::cout << "-------------------------\n";

		TimeSpan short_duration{ TimeSpan::fromMinutes( 30.0 ) };
		TimeSpan long_duration{ TimeSpan::fromHours( 2.0 ) };

		std::cout << std::boolalpha;
		std::cout << "30 min == 30 min:  " << ( short_duration == TimeSpan::fromMinutes( 30.0 ) ) << "\n";
		std::cout << "30 min < 2 hours:  " << ( short_duration < long_duration ) << "\n";
		std::cout << "2 hours > 30 min:  " << ( long_duration > short_duration ) << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 13. DateTimeOffset: Basic construction with timezone
	//=====================================================================
	{
		std::cout << "13. DateTimeOffset: Basic construction with timezone\n";
		std::cout << "-----------------------------------------------------\n";

		DateTimeOffset now{ DateTimeOffset::now() };
		DateTimeOffset utcNow{ DateTimeOffset::utcNow() };
		DateTimeOffset tokyo{ 2024, 6, 15, 14, 30, 0, TimeSpan::fromHours( 9.0 ) };
		DateTimeOffset newYork{ 2024, 6, 15, 14, 30, 0, TimeSpan::fromHours( -5.0 ) };

		std::cout << "Local time:       " << now.toString() << "\n";
		std::cout << "UTC time:         " << utcNow.toString() << "\n";
		std::cout << "Tokyo (UTC+9):    " << tokyo.toString() << "\n";
		std::cout << "New York (UTC-5): " << newYork.toString() << "\n";
		std::cout << "Note: DateTimeOffset includes timezone offset\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 14. DateTimeOffset: Parsing with timezone offsets
	//=====================================================================
	{
		std::cout << "14. DateTimeOffset: Parsing with timezone offsets\n";
		std::cout << "--------------------------------------------------\n";

		DateTimeOffset dto1{ DateTimeOffset{ "2024-06-15T14:30:00+09:00" } };
		DateTimeOffset dto2{ DateTimeOffset{ "2024-06-15T14:30:00-05:00" } };
		DateTimeOffset dto3{ DateTimeOffset{ "2024-06-15T14:30:00Z" } };

		std::cout << "Parsed +09:00:  " << dto1.toString() << "\n";
		std::cout << "Parsed -05:00:  " << dto2.toString() << "\n";
		std::cout << "Parsed Z (UTC): " << dto3.toString() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 15. DateTimeOffset: Timezone conversions
	//=====================================================================
	{
		std::cout << "15. DateTimeOffset: Timezone conversions\n";
		std::cout << "-----------------------------------------\n";

		DateTimeOffset tokyo{ 2024, 6, 15, 18, 0, 0, TimeSpan::fromHours( 9.0 ) };
		std::cout << "Tokyo (UTC+9):         " << tokyo.toString() << "\n";

		DateTimeOffset newYork{ tokyo.toOffset( TimeSpan::fromHours( -5.0 ) ) };
		std::cout << "Same moment in NY:     " << newYork.toString() << "\n";

		DateTimeOffset utc{ tokyo.toUniversalTime() };
		std::cout << "Same moment in UTC:    " << utc.toString() << "\n";

		std::cout << "\nAll represent same instant: " << std::boolalpha
				  << ( tokyo.utcTicks() == newYork.utcTicks() ) << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 16. DateTimeOffset: Accessing components
	//=====================================================================
	{
		std::cout << "16. DateTimeOffset: Accessing components\n";
		std::cout << "-----------------------------------------\n";

		DateTimeOffset dto{ 2024, 6, 15, 14, 30, 45, TimeSpan::fromHours( 2.0 ) };
		std::cout << "DateTimeOffset:     " << dto.toString() << "\n";
		std::cout << "  Local time:       " << dto.dateTime().toString() << "\n";
		std::cout << "  UTC time:         " << dto.utcDateTime().toString() << "\n";
		std::cout << "  Offset:           " << dto.offset().toString() << "\n";
		std::cout << "  Offset minutes:   " << dto.totalOffsetMinutes() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 17. DateTimeOffset: Arithmetic operations
	//=====================================================================
	{
		std::cout << "17. DateTimeOffset: Arithmetic operations\n";
		std::cout << "------------------------------------------\n";

		DateTimeOffset start{ 2024, 6, 15, 10, 0, 0, TimeSpan::fromHours( 2.0 ) };
		std::cout << "Start:                " << start.toString() << "\n";

		DateTimeOffset later{ start + TimeSpan::fromHours( 3.0 ) };
		std::cout << "After +3 hours:       " << later.toString() << "\n";

		// Difference between different timezones (same UTC moment)
		DateTimeOffset tokyo{ 2024, 6, 15, 18, 0, 0, TimeSpan::fromHours( 9.0 ) };
		DateTimeOffset ny{ 2024, 6, 15, 4, 0, 0, TimeSpan::fromHours( -5.0 ) };
		TimeSpan diff{ tokyo - ny };
		std::cout << "\nTokyo time:           " << tokyo.toString() << "\n";
		std::cout << "NY time:              " << ny.toString() << "\n";
		std::cout << "Difference (UTC):     " << diff.toString() << " (same UTC moment)\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 18. DateTimeOffset: Comparisons (UTC-based)
	//=====================================================================
	{
		std::cout << "18. DateTimeOffset: Comparisons (UTC-based)\n";
		std::cout << "--------------------------------------------\n";

		// Same moment in different timezones
		DateTimeOffset tokyo{ 2024, 6, 15, 18, 0, 0, TimeSpan::fromHours( 9.0 ) };
		DateTimeOffset newYork{ 2024, 6, 15, 4, 0, 0, TimeSpan::fromHours( -5.0 ) };

		std::cout << "Tokyo:                     " << tokyo.toString() << "\n";
		std::cout << "New York:                  " << newYork.toString() << "\n";
		std::cout << std::boolalpha;
		std::cout << "Equal (same UTC moment):   " << ( tokyo == newYork ) << "\n";
		std::cout << "Exact (incl offset):       " << tokyo.equalsExact( newYork ) << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 19. Integration: Task scheduling across timezones
	//=====================================================================
	{
		std::cout << "19. Integration: Task scheduling across timezones\n";
		std::cout << "--------------------------------------------------\n";

		DateTime taskStart{ 2024, 6, 15, 9, 0, 0 };
		TimeSpan duration{ TimeSpan::fromHours( 2.0 ) + TimeSpan::fromMinutes( 30.0 ) };
		DateTime taskEnd{ taskStart + duration };

		std::cout << "Task starts (UTC):    " << taskStart.toString() << "\n";
		std::cout << "Duration:             " << duration.toString() << "\n";
		std::cout << "Task ends (UTC):      " << taskEnd.toString() << "\n";

		DateTimeOffset endTokyo{ taskEnd, TimeSpan::fromHours( 9.0 ) };
		DateTimeOffset endNY{ taskEnd, TimeSpan::fromHours( -4.0 ) };
		std::cout << "End in Tokyo:         " << endTokyo.toString() << "\n";
		std::cout << "End in New York:      " << endNY.toString() << "\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 20. Integration: Meeting across timezones
	//=====================================================================
	{
		std::cout << "20. Integration: Meeting across timezones\n";
		std::cout << "------------------------------------------\n";

		DateTimeOffset meetingTokyo{ 2024, 6, 15, 14, 0, 0, TimeSpan::fromHours( 9.0 ) };
		std::cout << "Meeting in Tokyo (UTC+9):    " << meetingTokyo.toString() << "\n";

		DateTimeOffset meetingNY{ meetingTokyo.toOffset( TimeSpan::fromHours( -4.0 ) ) };
		DateTimeOffset meetingLondon{ meetingTokyo.toOffset( TimeSpan::fromHours( 1.0 ) ) };
		DateTimeOffset meetingSydney{ meetingTokyo.toOffset( TimeSpan::fromHours( 10.0 ) ) };

		std::cout << "Same meeting in NY:          " << meetingNY.toString() << " (" << meetingNY.hour() << ":00)\n";
		std::cout << "Same meeting in London:      " << meetingLondon.toString() << " (" << meetingLondon.hour() << ":00)\n";
		std::cout << "Same meeting in Sydney:      " << meetingSydney.toString() << " (" << meetingSydney.hour() << ":00)\n";

		std::cout << "\n";
	}

	//=====================================================================
	// 21. TimeSpan: Using modern C++ user-defined literals
	//=====================================================================
	{
		using namespace nfx::time::literals;

		std::cout << "21. TimeSpan: Using modern C++ user-defined literals\n";
		std::cout << "-----------------------------------------------------\n";

		// Creating durations with natural syntax
		auto duration1 = 2_h;
		auto duration2 = 30_min;
		auto duration3 = 45_s;
		auto duration4 = 500_ms;

		std::cout << "2 hours:        " << duration1.toString() << " (" << duration1.hours() << " hours)\n";
		std::cout << "30 minutes:     " << duration2.toString() << " (" << duration2.minutes() << " minutes)\n";
		std::cout << "45 seconds:     " << duration3.toString() << " (" << duration3.seconds() << " seconds)\n";
		std::cout << "500 millisec:   " << duration4.toString() << " (" << duration4.milliseconds() << " ms)\n";

		// Combining literals with operators
		auto workDay = 8_h + 30_min;
		auto breakTime = 1_h;
		auto actualWork = workDay - breakTime;

		std::cout << "\nWork day example:\n";
		std::cout << "  Work day:     " << workDay.toString() << " (" << workDay.hours() << " hours)\n";
		std::cout << "  Break time:   " << breakTime.toString() << " (" << breakTime.hours() << " hour)\n";
		std::cout << "  Actual work:  " << actualWork.toString() << " (" << actualWork.hours() << " hours)\n";

		// Complex durations
		auto videoLength = 1_h + 23_min + 45_s;
		auto sleepTime = 8_h + 15_min;
		auto cookingTime = 45_min + 30_s;

		std::cout << "\nReal-world examples:\n";
		std::cout << "  Video length: " << videoLength.toString() << "\n";
		std::cout << "  Sleep time:   " << sleepTime.toString() << "\n";
		std::cout << "  Cooking time: " << cookingTime.toString() << "\n";

		// Fractional values
		auto duration5 = 2.5_h;
		auto duration6 = 30.5_min;
		auto duration7 = 1.5_s;

		std::cout << "\nFractional values:\n";
		std::cout << "  2.5 hours:    " << duration5.toString() << " (" << duration5.minutes() << " minutes)\n";
		std::cout << "  30.5 minutes: " << duration6.toString() << " (" << duration6.seconds() << " seconds)\n";
		std::cout << "  1.5 seconds:  " << duration7.toString() << " (" << duration7.milliseconds() << " ms)\n";

		// Compile-time evaluation (constexpr)
		constexpr auto compileTime = 2_h + 30_min;
		std::cout << "\nCompile-time literal: " << compileTime.toString() << " (evaluated at compile-time!)\n";

		std::cout << "\n";
	}

	return 0;
}
