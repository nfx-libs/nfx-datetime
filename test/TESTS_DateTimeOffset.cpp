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
 * @file TESTS_DateTimeOffset.cpp
 * @brief Unit tests for DateTimeOffset class
 * @details Tests timezone-aware datetime operations including construction, comparison,
 *          arithmetic operations, ISO 8601 parsing/formatting with timezone offsets,
 *          UTC/local time conversions, and timezone offset handling.
 */

#include <gtest/gtest.h>

#include <sstream>

#include <nfx/datetime/DateTimeOffset.h>

namespace nfx::time::test
{
    //=====================================================================
    // DateTimeOffset type tests
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    TEST( DateTimeOffsetConstruction, DefaultConstructor )
    {
        DateTimeOffset dto;
        EXPECT_EQ( dto.ticks(), DateTime::min().ticks() );
        EXPECT_EQ( dto.offset().ticks(), 0 );
    }

    TEST( DateTimeOffsetConstruction, FromDateTimeAndOffset )
    {
        DateTime dt{ 2024, 1, 15, 12, 30, 45 };
        TimeSpan offset{ TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto{ dt, offset };

        EXPECT_EQ( dto.dateTime(), dt );
        EXPECT_EQ( dto.offset().ticks(), offset.ticks() );
        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 1 );
        EXPECT_EQ( dto.day(), 15 );
        EXPECT_EQ( dto.hour(), 12 );
        EXPECT_EQ( dto.minute(), 30 );
        EXPECT_EQ( dto.second(), 45 );
    }

    TEST( DateTimeOffsetConstruction, FromTicksAndOffset )
    {
        std::int64_t ticks{ 638400000000000000LL };  // Some arbitrary tick value
        TimeSpan offset{ TimeSpan::fromHours( 2 ) }; // +02:00
        DateTimeOffset dto{ ticks, offset };

        EXPECT_EQ( dto.ticks(), ticks );
        EXPECT_EQ( dto.offset().ticks(), offset.ticks() );
    }

    TEST( DateTimeOffsetConstruction, FromDateComponentsAndOffset )
    {
        TimeSpan offset{ TimeSpan::fromHours( 3.0 ) };
        DateTimeOffset dto{ 2024, 6, 15, offset };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 6 );
        EXPECT_EQ( dto.day(), 15 );
        EXPECT_EQ( dto.hour(), 0 );
        EXPECT_EQ( dto.minute(), 0 );
        EXPECT_EQ( dto.second(), 0 );
        EXPECT_EQ( dto.offset().ticks(), offset.ticks() );
    }

    TEST( DateTimeOffsetConstruction, FromDateTimeComponentsAndOffset )
    {
        TimeSpan offset{ TimeSpan::fromHours( -8.0 ) };
        DateTimeOffset dto{ 2024, 12, 25, 18, 45, 30, offset };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 12 );
        EXPECT_EQ( dto.day(), 25 );
        EXPECT_EQ( dto.hour(), 18 );
        EXPECT_EQ( dto.minute(), 45 );
        EXPECT_EQ( dto.second(), 30 );
        EXPECT_EQ( dto.offset().ticks(), offset.ticks() );
    }

    TEST( DateTimeOffsetConstruction, FromDateTimeWithMillisecondsAndOffset )
    {
        TimeSpan offset{ TimeSpan::fromHours( 5.5 ) };
        DateTimeOffset dto{ 2024, 3, 10, 9, 15, 22, 500, offset };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 3 );
        EXPECT_EQ( dto.day(), 10 );
        EXPECT_EQ( dto.hour(), 9 );
        EXPECT_EQ( dto.minute(), 15 );
        EXPECT_EQ( dto.second(), 22 );
        EXPECT_EQ( dto.millisecond(), 500 );
    }

    TEST( DateTimeOffsetConstruction, FromDateTimeWithMicrosecondsAndOffset )
    {
        TimeSpan offset{ TimeSpan::fromHours( 1.0 ) };
        DateTimeOffset dto{ 2024, 7, 20, 14, 30, 45, 123, 456, offset };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.millisecond(), 123 );
        EXPECT_EQ( dto.microsecond(), 456 );
    }

    TEST( DateTimeOffsetConstruction, CopyConstructor )
    {
        DateTimeOffset dto1{ 2024, 5, 15, 10, 20, 30, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ dto1 };

        EXPECT_EQ( dto1.ticks(), dto2.ticks() );
        EXPECT_EQ( dto1.offset().ticks(), dto2.offset().ticks() );
    }

    TEST( DateTimeOffsetConstruction, MoveConstructor )
    {
        DateTimeOffset dto1{ 2024, 5, 15, 10, 20, 30, TimeSpan::fromHours( 2.0 ) };
        std::int64_t originalTicks{ dto1.ticks() };
        DateTimeOffset dto2{ std::move( dto1 ) };
        EXPECT_EQ( dto2.ticks(), originalTicks );
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    TEST( DateTimeOffsetAssignment, CopyAssignment )
    {
        DateTimeOffset dto1{ 2024, 5, 15, 10, 20, 30, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2;
        dto2 = dto1;

        EXPECT_EQ( dto1.ticks(), dto2.ticks() );
        EXPECT_EQ( dto1.offset().ticks(), dto2.offset().ticks() );
    }

    TEST( DateTimeOffsetAssignment, MoveAssignment )
    {
        DateTimeOffset dto1{ 2024, 5, 15, 10, 20, 30, TimeSpan::fromHours( 2.0 ) };
        std::int64_t originalTicks{ dto1.ticks() };
        DateTimeOffset dto2;
        dto2 = std::move( dto1 );

        EXPECT_EQ( dto2.ticks(), originalTicks );
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    TEST( DateTimeOffsetComparison, EqualityOperator )
    {
        // Same UTC time, different offsets should be equal
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 10, 0, 0, TimeSpan::fromHours( 0.0 ) }; // Same UTC time

        EXPECT_TRUE( dto1 == dto2 );

        DateTimeOffset dto3{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 2.0 ) };
        EXPECT_FALSE( dto1 == dto3 );
    }

    TEST( DateTimeOffsetComparison, InequalityOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 2.0 ) };

        EXPECT_TRUE( dto1 != dto2 );
        EXPECT_FALSE( dto1 != dto1 );
    }

    TEST( DateTimeOffsetComparison, LessThanOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( dto1 < dto2 );
        EXPECT_FALSE( dto2 < dto1 );
        EXPECT_FALSE( dto1 < dto1 );
    }

    TEST( DateTimeOffsetComparison, LessThanOrEqualOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( dto1 <= dto2 );
        EXPECT_TRUE( dto1 <= dto1 );
        EXPECT_FALSE( dto2 <= dto1 );
    }

    TEST( DateTimeOffsetComparison, GreaterThanOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( dto2 > dto1 );
        EXPECT_FALSE( dto1 > dto2 );
        EXPECT_FALSE( dto1 > dto1 );
    }

    TEST( DateTimeOffsetComparison, GreaterThanOrEqualOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( dto2 >= dto1 );
        EXPECT_TRUE( dto1 >= dto1 );
        EXPECT_FALSE( dto1 >= dto2 );
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    TEST( DateTimeOffsetArithmetic, AddTimeSpan )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        TimeSpan duration{ TimeSpan::fromHours( 3.0 ) };
        DateTimeOffset result{ dto + duration };

        EXPECT_EQ( result.hour(), 15 );
        EXPECT_EQ( result.offset().ticks(), dto.offset().ticks() ); // Offset unchanged
    }

    TEST( DateTimeOffsetArithmetic, SubtractTimeSpan )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        TimeSpan duration{ TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset result{ dto - duration };

        EXPECT_EQ( result.hour(), 10 );
        EXPECT_EQ( result.offset().ticks(), dto.offset().ticks() );
    }

    TEST( DateTimeOffsetArithmetic, SubtractDateTimeOffset )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 15, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        TimeSpan difference{ dto1 - dto2 };

        EXPECT_EQ( difference.hours(), 3.0 );
    }

    TEST( DateTimeOffsetArithmetic, AddAssignTimeSpan )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        TimeSpan duration{ TimeSpan::fromMinutes( 45.0 ) };
        dto += duration;

        EXPECT_EQ( dto.hour(), 12 );
        EXPECT_EQ( dto.minute(), 45 );
    }

    TEST( DateTimeOffsetArithmetic, SubtractAssignTimeSpan )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 30, 0, TimeSpan::fromHours( 2.0 ) };
        TimeSpan duration{ TimeSpan::fromMinutes( 30.0 ) };
        dto -= duration;

        EXPECT_EQ( dto.hour(), 12 );
        EXPECT_EQ( dto.minute(), 0 );
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    TEST( DateTimeOffsetAccessors, ComponentAccessors )
    {
        DateTimeOffset dto{ 2024, 3, 15, 14, 30, 45, 123, TimeSpan::fromHours( -5.0 ) };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 3 );
        EXPECT_EQ( dto.day(), 15 );
        EXPECT_EQ( dto.hour(), 14 );
        EXPECT_EQ( dto.minute(), 30 );
        EXPECT_EQ( dto.second(), 45 );
        EXPECT_EQ( dto.millisecond(), 123 );
    }

    TEST( DateTimeOffsetAccessors, OffsetAccessors )
    {
        TimeSpan offset{ TimeSpan::fromHours( 5.5 ) };
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, offset };

        EXPECT_EQ( dto.offset().ticks(), offset.ticks() );
        EXPECT_EQ( dto.totalOffsetMinutes(), 330 ); // 5.5 hours = 330 minutes
    }

    TEST( DateTimeOffsetAccessors, UtcDateTime )
    {
        // Local: 2024-01-15 12:00:00 +02:00
        // UTC:   2024-01-15 10:00:00
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTime utc{ dto.utcDateTime() };

        EXPECT_EQ( utc.hour(), 10 );
        EXPECT_EQ( utc.day(), 15 );
    }

    TEST( DateTimeOffsetAccessors, DayOfWeekAndDayOfYear )
    {
        // January 15, 2024 is a Monday (day 1)
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_EQ( dto.dayOfWeek(), 1 );  // Monday
        EXPECT_EQ( dto.dayOfYear(), 15 ); // 15th day of year
    }

    //----------------------------------------------
    // Conversion methods
    //----------------------------------------------

    TEST( DateTimeOffsetConversion, ToEpochSeconds )
    {
        // Unix epoch: 1970-01-01 00:00:00 UTC
        DateTimeOffset epoch{ DateTimeOffset::epoch() };
        EXPECT_EQ( epoch.toEpochSeconds(), 0 );

        DateTimeOffset dto{ 1970, 1, 1, 1, 0, 0, TimeSpan::fromHours( 0.0 ) };
        EXPECT_EQ( dto.toEpochSeconds(), 3600 ); // 1 hour after epoch
    }

    TEST( DateTimeOffsetConversion, ToEpochMilliseconds )
    {
        DateTimeOffset epoch{ DateTimeOffset::epoch() };
        EXPECT_EQ( epoch.toEpochMilliseconds(), 0 );

        DateTimeOffset dto{ 1970, 1, 1, 0, 0, 1, 500, TimeSpan::fromHours( 0.0 ) };
        EXPECT_EQ( dto.toEpochMilliseconds(), 1500 ); // 1.5 seconds = 1500 ms
    }

    TEST( DateTimeOffsetConversion, ToOffset )
    {
        // 12:00 +02:00 = 10:00 UTC = 05:00 -05:00
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset converted{ dto.toOffset( TimeSpan::fromHours( -5.0 ) ) };

        EXPECT_EQ( converted.hour(), 5 );
        EXPECT_EQ( converted.offset().hours(), -5.0 );
        EXPECT_EQ( dto.utcTicks(), converted.utcTicks() ); // Same UTC time
    }

    TEST( DateTimeOffsetConversion, ToUniversalTime )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 3.0 ) };
        DateTimeOffset utc{ dto.toUniversalTime() };

        EXPECT_EQ( utc.hour(), 9 );
        EXPECT_EQ( utc.offset().ticks(), 0 );
    }

    TEST( DateTimeOffsetConversion, DateAndTimeOfDay )
    {
        DateTimeOffset dto{ 2024, 3, 15, 14, 30, 45, TimeSpan::fromHours( 1.0 ) };

        DateTimeOffset dateOnly{ dto.date() };
        EXPECT_EQ( dateOnly.hour(), 0 );
        EXPECT_EQ( dateOnly.minute(), 0 );
        EXPECT_EQ( dateOnly.second(), 0 );
        EXPECT_EQ( dateOnly.day(), 15 );

        TimeSpan timeOfDay{ dto.timeOfDay() };
        EXPECT_EQ( timeOfDay.hours(), 14.0 + 30.0 / 60.0 + 45.0 / 3600.0 );
    }

    //----------------------------------------------
    // Arithmetic methods
    //----------------------------------------------

    TEST( DateTimeOffsetArithmeticMethods, AddDays )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset result{ dto.addDays( 5.0 ) };

        EXPECT_EQ( result.day(), 20 );
        EXPECT_EQ( result.hour(), 12 );
    }

    TEST( DateTimeOffsetArithmeticMethods, AddHours )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset result{ dto.addHours( 25.0 ) };

        EXPECT_EQ( result.day(), 16 );
        EXPECT_EQ( result.hour(), 13 );
    }

    TEST( DateTimeOffsetArithmeticMethods, AddMonths )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset result{ dto.addMonths( 2 ) };

        EXPECT_EQ( result.month(), 3 );
        EXPECT_EQ( result.day(), 15 );
    }

    TEST( DateTimeOffsetArithmeticMethods, AddYears )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset result{ dto.addYears( 1 ) };

        EXPECT_EQ( result.year(), 2025 );
        EXPECT_EQ( result.month(), 1 );
        EXPECT_EQ( result.day(), 15 );
    }

    //----------------------------------------------
    // String formatting
    //----------------------------------------------

    TEST( DateTimeOffsetStringFormatting, ToStringIso8601 )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 30, 45, TimeSpan::fromHours( 2.0 ) };
        std::string str{ dto.toString() };

        // Should be in format: 2024-01-15T12:30:45+02:00
        EXPECT_NE( str.find( "2024-01-15" ), std::string::npos );
        EXPECT_NE( str.find( "12:30:45" ), std::string::npos );
        EXPECT_NE( str.find( "+02:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetStringFormatting, ToStringWithNegativeOffset )
    {
        DateTimeOffset dto{ 2024, 6, 20, 18, 45, 30, TimeSpan::fromHours( -5.0 ) };
        std::string str{ dto.toString() };

        EXPECT_NE( str.find( "-05:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetStringFormatting, toIso8601Extended )
    {
        DateTimeOffset dto{ 2024, 3, 10, 9, 15, 22, 123, TimeSpan::fromHours( 1.0 ) };
        std::string str{ dto.toString( DateTime::Format::Iso8601Precise ) };

        // Should include fractional seconds
        EXPECT_NE( str.find( "22.123" ), std::string::npos );
        EXPECT_NE( str.find( "+01:00" ), std::string::npos );
    }

    //----------------------------------------------
    // Comparison methods
    //----------------------------------------------

    TEST( DateTimeOffsetComparisonMethods, Equals )
    {
        // Same UTC time, different offsets
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 10, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( dto1.equals( dto2 ) );
    }

    TEST( DateTimeOffsetComparisonMethods, EqualsExact )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 10, 0, 0, TimeSpan::fromHours( 0.0 ) }; // Same UTC but different local
        DateTimeOffset dto3{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 2.0 ) }; // Exact match

        EXPECT_FALSE( dto1.equalsExact( dto2 ) ); // Same UTC, different local/offset
        EXPECT_TRUE( dto1.equalsExact( dto3 ) );  // Exact match
    }

    TEST( DateTimeOffsetComparisonMethods, SpaceshipOperator )
    {
        DateTimeOffset dto1{ 2024, 1, 15, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset dto2{ 2024, 1, 15, 13, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_TRUE( ( dto1 <=> dto2 ) < 0 );
        EXPECT_TRUE( ( dto2 <=> dto1 ) > 0 );
        EXPECT_TRUE( ( dto1 <=> dto1 ) == 0 );
    }

    //----------------------------------------------
    // Static factory methods
    //----------------------------------------------

    TEST( DateTimeOffsetFactory, MinMaxValues )
    {
        DateTimeOffset minVal{ DateTimeOffset::min() };
        DateTimeOffset maxVal{ DateTimeOffset::max() };

        EXPECT_EQ( minVal.year(), 1 );
        EXPECT_EQ( minVal.month(), 1 );
        EXPECT_EQ( minVal.day(), 1 );

        EXPECT_EQ( maxVal.year(), 9999 );
        EXPECT_EQ( maxVal.month(), 12 );
        EXPECT_EQ( maxVal.day(), 31 );
    }

    TEST( DateTimeOffsetFactory, epoch )
    {
        DateTimeOffset epoch{ DateTimeOffset::epoch() };

        EXPECT_EQ( epoch.year(), 1970 );
        EXPECT_EQ( epoch.month(), 1 );
        EXPECT_EQ( epoch.day(), 1 );
        EXPECT_EQ( epoch.hour(), 0 );
        EXPECT_EQ( epoch.offset().ticks(), 0 );
    }

    TEST( DateTimeOffsetFactory, FromEpochSeconds )
    {
        DateTimeOffset dto{ DateTimeOffset::fromEpochSeconds( 3600 ) }; // 1 hour after epoch

        EXPECT_EQ( dto.year(), 1970 );
        EXPECT_EQ( dto.month(), 1 );
        EXPECT_EQ( dto.day(), 1 );
        EXPECT_EQ( dto.hour(), 1 );
        EXPECT_EQ( dto.offset().ticks(), 0 ); // UTC
    }

    TEST( DateTimeOffsetFactory, FromEpochMilliseconds )
    {
        DateTimeOffset dto{ DateTimeOffset::fromEpochMilliseconds( 1500 ) }; // 1.5 seconds

        EXPECT_EQ( dto.second(), 1 );
        EXPECT_EQ( dto.millisecond(), 500 );
    }

    //----------------------------------------------
    // String parsing
    //----------------------------------------------

    TEST( DateTimeOffsetStringParsing, ConstructorParsingWithPositiveOffset )
    {
        DateTimeOffset dto{ DateTimeOffset{ "2024-01-15T12:30:45+02:00" } };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 1 );
        EXPECT_EQ( dto.day(), 15 );
        EXPECT_EQ( dto.hour(), 12 );
        EXPECT_EQ( dto.minute(), 30 );
        EXPECT_EQ( dto.second(), 45 );
        EXPECT_EQ( dto.offset().hours(), 2.0 );
    }

    TEST( DateTimeOffsetStringParsing, ConstructorParsingWithNegativeOffset )
    {
        DateTimeOffset dto{ DateTimeOffset{ "2024-06-20T18:45:30-05:00" } };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.hour(), 18 );
        EXPECT_EQ( dto.offset().hours(), -5.0 );
    }

    TEST( DateTimeOffsetStringParsing, ConstructorParsingWithZuluOffset )
    {
        DateTimeOffset dto{ DateTimeOffset{ "2024-03-10T09:15:22Z" } };

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.month(), 3 );
        EXPECT_EQ( dto.hour(), 9 );
        EXPECT_EQ( dto.offset().ticks(), 0 ); // Z means UTC
    }

    TEST( DateTimeOffsetStringParsing, ConstructorParsingWithFractionalSeconds )
    {
        DateTimeOffset dto{ DateTimeOffset{ "2024-01-15T12:30:45.123+01:00" } };

        EXPECT_EQ( dto.second(), 45 );
        EXPECT_EQ( dto.millisecond(), 123 );
    }

    TEST( DateTimeOffsetStringParsing, FromStringValid )
    {
        DateTimeOffset dto;
        bool success{ DateTimeOffset::fromString( "2024-01-15T12:30:45+02:00", dto ) };

        EXPECT_TRUE( success );
        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.hour(), 12 );
    }

    TEST( DateTimeOffsetStringParsing, FromStringInvalid )
    {
        DateTimeOffset dto;
        bool success{ DateTimeOffset::fromString( "invalid-date-string", dto ) };

        EXPECT_FALSE( success );
    }

    TEST( DateTimeOffsetStringParsing, FromStringOptionalValid )
    {
        auto dto{ DateTimeOffset::fromString( "2024-01-15T12:30:45+02:00" ) };

        EXPECT_TRUE( dto.has_value() );
        EXPECT_EQ( dto->year(), 2024 );
        EXPECT_EQ( dto->hour(), 12 );
        EXPECT_EQ( dto->minute(), 30 );
        EXPECT_EQ( dto->offset().hours(), 2.0 );
    }

    TEST( DateTimeOffsetStringParsing, FromStringOptionalInvalid )
    {
        auto dto{ DateTimeOffset::fromString( "invalid-date-string" ) };

        EXPECT_FALSE( dto.has_value() );
    }

    TEST( DateTimeOffsetStringParsing, ConstructorThrowsOnInvalid )
    {
        EXPECT_THROW( [[maybe_unused]] auto _ = DateTimeOffset{ "not-a-date" }, std::invalid_argument );
    }

    TEST( DateTimeOffsetStringParsing, RejectInvalidOffsets )
    {
        // Should reject offsets outside ±14:00 range
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+15:00" ).has_value() ); // +15 hours > +14
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-15:00" ).has_value() ); // -15 hours < -14
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+99:00" ).has_value() ); // Way out of range
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-99:00" ).has_value() );

        // Should reject +14:01 or greater (max is exactly ±14:00)
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+14:01" ).has_value() );
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+14:30" ).has_value() );
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-14:01" ).has_value() );
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-14:30" ).has_value() );

        // Should reject invalid minute values
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+01:60" ).has_value() ); // 60 minutes invalid
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+01:99" ).has_value() ); // 99 minutes invalid

        // Should reject negative component values
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+-01:00" ).has_value() ); // Double sign
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-+01:00" ).has_value() ); // Double sign
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00++01:00" ).has_value() ); // Double sign
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00-+01:00" ).has_value() ); // Double sign

        // Constructor should throw for invalid offsets
        EXPECT_THROW( { [[maybe_unused]] auto _ = DateTimeOffset{ "2024-01-15T12:00:00+15:00" }; }, std::invalid_argument );
        EXPECT_THROW( { [[maybe_unused]] auto _ = DateTimeOffset{ "2024-01-15T12:00:00+14:30" }; }, std::invalid_argument );

        // fromString with bool should return false
        DateTimeOffset result;
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+15:00", result ) );
        EXPECT_FALSE( DateTimeOffset::fromString( "2024-01-15T12:00:00+14:01", result ) );
    }

    TEST( DateTimeOffsetStringParsing, AcceptValidOffsetEdgeCases )
    {
        // Should accept exactly ±14:00 (maximum valid offset)
        auto dto1{ DateTimeOffset::fromString( "2024-01-15T12:00:00+14:00" ) };
        EXPECT_TRUE( dto1.has_value() );
        EXPECT_EQ( dto1->offset().hours(), 14.0 );

        auto dto2{ DateTimeOffset::fromString( "2024-01-15T12:00:00-14:00" ) };
        EXPECT_TRUE( dto2.has_value() );
        EXPECT_EQ( dto2->offset().hours(), -14.0 );

        // Should accept valid ranges
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00+13:59" ).has_value() );
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00-13:59" ).has_value() );
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00+00:00" ).has_value() );
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00Z" ).has_value() );

        // Should accept compact formats
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00+1400" ).has_value() );
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00-1400" ).has_value() );
        EXPECT_TRUE( DateTimeOffset::fromString( "2024-01-15T12:00:00+14" ).has_value() );
    }

    //----------------------------------------------
    // Stream operators
    //----------------------------------------------

    TEST( DateTimeOffsetStreamOperators, OutputOperator )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 30, 45, TimeSpan::fromHours( 2.0 ) };
        std::ostringstream oss;
        oss << dto;

        std::string output{ oss.str() };
        EXPECT_NE( output.find( "2024" ), std::string::npos );
        EXPECT_NE( output.find( "12:30:45" ), std::string::npos );
    }

    TEST( DateTimeOffsetStreamOperators, InputOperator )
    {
        std::istringstream iss( "2024-01-15T12:30:45+02:00" );
        DateTimeOffset dto;
        iss >> dto;

        EXPECT_EQ( dto.year(), 2024 );
        EXPECT_EQ( dto.hour(), 12 );
        EXPECT_EQ( dto.offset().hours(), 2.0 );
    }

    //----------------------------------------------
    // std::formatter support
    //----------------------------------------------

    TEST( DateTimeOffsetFormatter, BasicFormatting )
    {
        DateTimeOffset dto{ 2024, 1, 15, 12, 30, 45, TimeSpan::fromHours( 2.0 ) };
        std::string formatted{ std::format( "{}", dto ) };

        EXPECT_NE( formatted.find( "2024-01-15" ), std::string::npos );
        EXPECT_NE( formatted.find( "12:30:45" ), std::string::npos );
        EXPECT_NE( formatted.find( "+02:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetFormatter, FormattingWithNegativeOffset )
    {
        DateTimeOffset dto{ 2024, 6, 20, 18, 45, 30, TimeSpan::fromHours( -5.0 ) };
        std::string formatted{ std::format( "{}", dto ) };

        EXPECT_NE( formatted.find( "2024-06-20" ), std::string::npos );
        EXPECT_NE( formatted.find( "18:45:30" ), std::string::npos );
        EXPECT_NE( formatted.find( "-05:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetFormatter, FormatInString )
    {
        DateTimeOffset dto{ 2024, 11, 16, 14, 30, 0, TimeSpan::fromHours( 1.0 ) };
        std::string message{ std::format( "Meeting scheduled for: {}", dto ) };

        EXPECT_NE( message.find( "Meeting scheduled for:" ), std::string::npos );
        EXPECT_NE( message.find( "2024-11-16" ), std::string::npos );
        EXPECT_NE( message.find( "14:30:00" ), std::string::npos );
        EXPECT_NE( message.find( "+01:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetFormatter, MultipleFormatting )
    {
        DateTimeOffset start{ 2024, 1, 1, 9, 0, 0, TimeSpan::fromHours( -8.0 ) };
        DateTimeOffset end{ 2024, 1, 1, 17, 0, 0, TimeSpan::fromHours( -8.0 ) };
        std::string range{ std::format( "Office hours (PST): {} to {}", start, end ) };

        EXPECT_NE( range.find( "Office hours (PST):" ), std::string::npos );
        EXPECT_NE( range.find( "09:00:00" ), std::string::npos );
        EXPECT_NE( range.find( "17:00:00" ), std::string::npos );
        EXPECT_NE( range.find( "-08:00" ), std::string::npos );
    }

    TEST( DateTimeOffsetFormatter, FormatWithOtherTypes )
    {
        DateTimeOffset dto{ 2024, 12, 25, 15, 30, 0, TimeSpan::fromHours( 5.5 ) };
        std::string event{ "Holiday Event" };
        std::string mixed{ std::format( "{} at {}", event, dto ) };

        EXPECT_NE( mixed.find( "Holiday Event at" ), std::string::npos );
        EXPECT_NE( mixed.find( "2024-12-25" ), std::string::npos );
        EXPECT_NE( mixed.find( "+05:30" ), std::string::npos );
    }

    TEST( DateTimeOffsetFormatter, FormatZuluTime )
    {
        DateTimeOffset dto{ 2024, 3, 10, 10, 0, 0, TimeSpan::fromHours( 0.0 ) };
        std::string formatted{ std::format( "{}", dto ) };

        EXPECT_NE( formatted.find( "2024-03-10" ), std::string::npos );
        EXPECT_NE( formatted.find( "10:00:00" ), std::string::npos );
        // Should show +00:00 or Z for UTC
    }

    //----------------------------------------------
    // Edge cases and validation
    //----------------------------------------------

    TEST( DateTimeOffsetEdgeCases, DateTimeCrossingDayBoundary )
    {
        // 23:00 + 2 hours = next day 01:00
        DateTimeOffset dto{ 2024, 1, 15, 23, 0, 0, TimeSpan::fromHours( 0.0 ) };
        DateTimeOffset result{ dto.addHours( 2.0 ) };

        EXPECT_EQ( result.day(), 16 );
        EXPECT_EQ( result.hour(), 1 );
    }

    TEST( DateTimeOffsetEdgeCases, OffsetConversionAcrossDays )
    {
        // 01:00 +10:00 = 15:00 -09:00 (previous day)
        DateTimeOffset dto{ 2024, 1, 16, 1, 0, 0, TimeSpan::fromHours( 10.0 ) };
        DateTimeOffset converted{ dto.toOffset( TimeSpan::fromHours( -9.0 ) ) };

        EXPECT_EQ( converted.day(), 15 );
        EXPECT_EQ( converted.hour(), 6 ); // UTC is 15:00 previous day
    }

    TEST( DateTimeOffsetEdgeCases, LeapYear )
    {
        // February 29, 2024 (leap year)
        DateTimeOffset dto{ 2024, 2, 29, 12, 0, 0, TimeSpan::fromHours( 0.0 ) };

        EXPECT_EQ( dto.month(), 2 );
        EXPECT_EQ( dto.day(), 29 );
    }
} // namespace nfx::time::test
