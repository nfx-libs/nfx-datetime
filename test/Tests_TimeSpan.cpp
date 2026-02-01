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
 * @file Tests_TimeSpan.cpp
 * @brief Unit tests for TimeSpan class
 * @details Tests high-precision time duration operations with ISO 8601 duration format support,
 *          arithmetic operations, and conversions between various time units
 */

#include <gtest/gtest.h>

#include <sstream>

#include <nfx/datetime/TimeSpan.h>

namespace nfx::time::test
{
    //=====================================================================
    // TimeSpan type tests
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    TEST( TimeSpanConstruction, FromTicks )
    {
        // Default constructor (zero duration)
        TimeSpan ts1;
        EXPECT_EQ( ts1.ticks(), 0 );

        // Constructor from ticks
        TimeSpan ts2{ 10000000 }; // 1 second in ticks
        EXPECT_EQ( ts2.ticks(), 10000000 );
        EXPECT_DOUBLE_EQ( ts2.seconds(), 1.0 );

        // Copy constructor
        TimeSpan ts3{ ts2 };
        EXPECT_EQ( ts3.ticks(), ts2.ticks() );

        // Move constructor
        TimeSpan ts4{ std::move( ts3 ) };
        EXPECT_EQ( ts4.ticks(), 10000000 );
    }

    TEST( TimeSpanConstruction, FromChrono )
    {
        // Construct from std::chrono::seconds
        auto chronoSeconds{ std::chrono::seconds( 60 ) };
        TimeSpan ts1{ chronoSeconds };
        EXPECT_DOUBLE_EQ( ts1.seconds(), 60.0 );

        // Construct from std::chrono::milliseconds
        auto chronoMillis{ std::chrono::milliseconds( 1500 ) };
        TimeSpan ts2{ chronoMillis };
        EXPECT_DOUBLE_EQ( ts2.milliseconds(), 1500.0 );
        EXPECT_DOUBLE_EQ( ts2.seconds(), 1.5 );

        // Construct from std::chrono::hours
        auto chronoHours{ std::chrono::hours( 2 ) };
        TimeSpan ts3{ chronoHours };
        EXPECT_DOUBLE_EQ( ts3.hours(), 2.0 );

        // Construct from std::chrono::minutes
        auto chronoMinutes{ std::chrono::minutes( 45 ) };
        TimeSpan ts4{ chronoMinutes };
        EXPECT_DOUBLE_EQ( ts4.minutes(), 45.0 );
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    TEST( TimeSpanAssignment, Assignment )
    {
        TimeSpan ts1{ 10000000 };
        TimeSpan ts2;

        // Copy assignment
        ts2 = ts1;
        EXPECT_EQ( ts2.ticks(), ts1.ticks() );

        // Move assignment
        TimeSpan ts3;
        ts3 = std::move( ts2 );
        EXPECT_EQ( ts3.ticks(), 10000000 );
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    TEST( TimeSpanOperations, ComparisonOperators )
    {
        TimeSpan ts1{ TimeSpan::fromSeconds( 10 ) };
        TimeSpan ts2{ TimeSpan::fromSeconds( 20 ) };
        TimeSpan ts3{ TimeSpan::fromSeconds( 10 ) };

        // Equality
        EXPECT_TRUE( ts1 == ts3 );
        EXPECT_FALSE( ts1 == ts2 );

        // Inequality
        EXPECT_TRUE( ts1 != ts2 );
        EXPECT_FALSE( ts1 != ts3 );

        // Less than
        EXPECT_TRUE( ts1 < ts2 );
        EXPECT_FALSE( ts2 < ts1 );
        EXPECT_FALSE( ts1 < ts3 );

        // Less than or equal
        EXPECT_TRUE( ts1 <= ts2 );
        EXPECT_TRUE( ts1 <= ts3 );
        EXPECT_FALSE( ts2 <= ts1 );

        // Greater than
        EXPECT_TRUE( ts2 > ts1 );
        EXPECT_FALSE( ts1 > ts2 );
        EXPECT_FALSE( ts1 > ts3 );

        // Greater than or equal
        EXPECT_TRUE( ts2 >= ts1 );
        EXPECT_TRUE( ts1 >= ts3 );
        EXPECT_FALSE( ts1 >= ts2 );
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    TEST( TimeSpanArithmetic, Addition )
    {
        TimeSpan ts1{ TimeSpan::fromHours( 2 ) };
        TimeSpan ts2{ TimeSpan::fromMinutes( 30 ) };
        TimeSpan result{ ts1 + ts2 };

        EXPECT_DOUBLE_EQ( result.hours(), 2.5 );
        EXPECT_DOUBLE_EQ( result.minutes(), 150.0 );
    }

    TEST( TimeSpanArithmetic, Subtraction )
    {
        TimeSpan ts1{ TimeSpan::fromHours( 2 ) };
        TimeSpan ts2{ TimeSpan::fromMinutes( 30 ) };
        TimeSpan result{ ts1 - ts2 };

        EXPECT_DOUBLE_EQ( result.hours(), 1.5 );
        EXPECT_DOUBLE_EQ( result.minutes(), 90.0 );
    }

    TEST( TimeSpanArithmetic, Negation )
    {
        TimeSpan ts{ TimeSpan::fromHours( 2.5 ) };
        TimeSpan neg{ -ts };

        EXPECT_DOUBLE_EQ( neg.hours(), -2.5 );
        EXPECT_EQ( neg.ticks(), -ts.ticks() );
    }

    TEST( TimeSpanArithmetic, AdditionAssignment )
    {
        TimeSpan ts1{ TimeSpan::fromHours( 2 ) };
        TimeSpan ts2{ TimeSpan::fromMinutes( 30 ) };
        ts1 += ts2;

        EXPECT_DOUBLE_EQ( ts1.hours(), 2.5 );
    }

    TEST( TimeSpanArithmetic, SubtractionAssignment )
    {
        TimeSpan ts1{ TimeSpan::fromHours( 2 ) };
        TimeSpan ts2{ TimeSpan::fromMinutes( 30 ) };
        ts1 -= ts2;

        EXPECT_DOUBLE_EQ( ts1.hours(), 1.5 );
    }

    TEST( TimeSpanArithmetic, MultiplicationByInteger )
    {
        TimeSpan ts{ TimeSpan::fromHours( 2 ) };
        TimeSpan result{ ts * 3 };

        EXPECT_DOUBLE_EQ( result.hours(), 6.0 );

        // Test reverse order (scalar * TimeSpan)
        TimeSpan result2{ 3 * ts };
        EXPECT_DOUBLE_EQ( result2.hours(), 6.0 );
    }

    TEST( TimeSpanArithmetic, MultiplicationByDouble )
    {
        TimeSpan ts{ TimeSpan::fromHours( 2 ) };
        TimeSpan result{ ts * 2.5 };

        EXPECT_DOUBLE_EQ( result.hours(), 5.0 );

        // Test reverse order
        TimeSpan result2{ 2.5 * ts };
        EXPECT_DOUBLE_EQ( result2.hours(), 5.0 );

        // Test fractional multiplication
        TimeSpan half{ ts * 0.5 };
        EXPECT_DOUBLE_EQ( half.hours(), 1.0 );
    }

    TEST( TimeSpanArithmetic, DivisionByInteger )
    {
        TimeSpan ts{ TimeSpan::fromHours( 6 ) };
        TimeSpan result{ ts / 3 };

        EXPECT_DOUBLE_EQ( result.hours(), 2.0 );
    }

    TEST( TimeSpanArithmetic, DivisionByDouble )
    {
        TimeSpan ts{ TimeSpan::fromHours( 5 ) };
        TimeSpan result{ ts / 2.0 };

        EXPECT_DOUBLE_EQ( result.hours(), 2.5 );

        // Test fractional division
        TimeSpan result2{ ts / 2.5 };
        EXPECT_DOUBLE_EQ( result2.hours(), 2.0 );
    }

    TEST( TimeSpanArithmetic, DivisionByTimeSpan )
    {
        TimeSpan ts1{ TimeSpan::fromHours( 6 ) };
        TimeSpan ts2{ TimeSpan::fromHours( 2 ) };
        double ratio{ ts1 / ts2 };

        EXPECT_DOUBLE_EQ( ratio, 3.0 );

        // Test with different units
        TimeSpan ts3{ TimeSpan::fromMinutes( 90 ) };
        TimeSpan ts4{ TimeSpan::fromMinutes( 30 ) };
        double ratio2{ ts3 / ts4 };
        EXPECT_DOUBLE_EQ( ratio2, 3.0 );

        // Test fractional ratio
        TimeSpan ts5{ TimeSpan::fromHours( 5 ) };
        TimeSpan ts6{ TimeSpan::fromHours( 2 ) };
        double ratio3{ ts5 / ts6 };
        EXPECT_DOUBLE_EQ( ratio3, 2.5 );
    }

    TEST( TimeSpanArithmetic, MultiplicationEdgeCases )
    {
        // Zero multiplication
        TimeSpan ts{ TimeSpan::fromHours( 2 ) };
        TimeSpan zero{ ts * 0 };
        EXPECT_EQ( zero.ticks(), 0 );

        // Negative multiplication
        TimeSpan neg{ ts * -1 };
        EXPECT_DOUBLE_EQ( neg.hours(), -2.0 );

        // Large multiplication
        TimeSpan large{ TimeSpan::fromSeconds( 1 ) * 86400 };
        EXPECT_DOUBLE_EQ( large.days(), 1.0 );
    }

    TEST( TimeSpanArithmetic, DivisionEdgeCases )
    {
        TimeSpan ts{ TimeSpan::fromHours( 6 ) };

        // Negative division
        TimeSpan neg{ ts / -2 };
        EXPECT_DOUBLE_EQ( neg.hours(), -3.0 );

        // Division by one
        TimeSpan same{ ts / 1 };
        EXPECT_DOUBLE_EQ( same.hours(), 6.0 );

        // Division ratio edge cases
        TimeSpan ts1{ TimeSpan::fromSeconds( 1 ) };
        TimeSpan ts2{ TimeSpan::fromSeconds( 1 ) };
        EXPECT_DOUBLE_EQ( ts1 / ts2, 1.0 );
    }

    TEST( TimeSpanArithmetic, ComplexChaining )
    {
        TimeSpan ts{ TimeSpan::fromHours( 2 ) };
        TimeSpan result{ ( ts * 2 + TimeSpan::fromMinutes( 30 ) ) / 2 };

        // (2h * 2 + 30m) / 2 = (4h + 0.5h) / 2 = 4.5h / 2 = 2.25h
        EXPECT_DOUBLE_EQ( result.hours(), 2.25 );
    }

    //----------------------------------------------
    // Accessors
    //----------------------------------------------

    TEST( TimeSpanAccessors, PropertyAccessors )
    {
        // Test various time units
        TimeSpan ts{ TimeSpan::fromHours( 2.5 ) };

        EXPECT_EQ( ts.ticks(), 2.5 * 60 * 60 * 10000000 );
        EXPECT_DOUBLE_EQ( ts.days(), 2.5 / 24.0 );
        EXPECT_DOUBLE_EQ( ts.hours(), 2.5 );
        EXPECT_DOUBLE_EQ( ts.minutes(), 150.0 );
        EXPECT_DOUBLE_EQ( ts.seconds(), 9000.0 );
        EXPECT_DOUBLE_EQ( ts.milliseconds(), 9000000.0 );

        // Test negative duration
        TimeSpan negTs{ TimeSpan::fromMinutes( -45 ) };
        EXPECT_DOUBLE_EQ( negTs.minutes(), -45.0 );
        EXPECT_DOUBLE_EQ( negTs.hours(), -0.75 );
    }

    //----------------------------------------------
    // String formatting
    //----------------------------------------------

    TEST( TimeSpanStringFormatting, ToStringCompactFormat )
    {
        // Test compact ISO 8601 format (omits zero components)
        EXPECT_EQ( TimeSpan::fromHours( 1 ).toString(), "PT1H" );
        EXPECT_EQ( TimeSpan::fromMinutes( 30 ).toString(), "PT30M" );
        EXPECT_EQ( TimeSpan::fromSeconds( 45 ).toString(), "PT45S" );

        // Combined durations (compact format)
        TimeSpan ts1{ TimeSpan::fromHours( 1 ) + TimeSpan::fromMinutes( 30 ) };
        EXPECT_EQ( ts1.toString(), "PT1H30M" );

        TimeSpan ts2{ TimeSpan::fromHours( 2 ) + TimeSpan::fromSeconds( 15 ) };
        EXPECT_EQ( ts2.toString(), "PT2H15S" );

        TimeSpan ts3{ TimeSpan::fromMinutes( 45 ) + TimeSpan::fromSeconds( 30 ) };
        EXPECT_EQ( ts3.toString(), "PT45M30S" );

        // Full combination
        TimeSpan ts4{ TimeSpan::fromHours( 1 ) + TimeSpan::fromMinutes( 30 ) + TimeSpan::fromSeconds( 45 ) };
        EXPECT_EQ( ts4.toString(), "PT1H30M45S" );

        // Zero duration (minimum required format)
        EXPECT_EQ( TimeSpan().toString(), "PT0S" );

        // Milliseconds (fractional seconds) in compact format
        TimeSpan ts5{ TimeSpan::fromSeconds( 1.5 ) };
        std::string result{ ts5.toString() };
        EXPECT_TRUE( result.find( "PT1." ) != std::string::npos || result.find( "PT1.5" ) != std::string::npos );
        EXPECT_TRUE( result.back() == 'S' );

        TimeSpan ts6{ TimeSpan::fromSeconds( 0.123 ) };
        std::string result2{ ts6.toString() };
        EXPECT_TRUE( result2.find( "PT0." ) != std::string::npos );
        EXPECT_TRUE( result2.back() == 'S' );
    }

    TEST( TimeSpanStringFormatting, ToStringNegativeDurations )
    {
        // Negative durations (sign before P)
        TimeSpan negHour{ TimeSpan::fromHours( -1 ) };
        EXPECT_EQ( negHour.toString(), "-PT1H" );

        TimeSpan negMinute{ TimeSpan::fromMinutes( -30 ) };
        EXPECT_EQ( negMinute.toString(), "-PT30M" );

        TimeSpan negCombined{ TimeSpan::fromHours( -2 ) + TimeSpan::fromMinutes( -15 ) };
        EXPECT_EQ( negCombined.toString(), "-PT2H15M" );
    }

    TEST( TimeSpanStringFormatting, ToStringWithDays )
    {
        // Durations with days component
        TimeSpan days{ TimeSpan::fromDays( 1 ) };
        std::string result{ days.toString() };
        EXPECT_TRUE( result.find( "P1D" ) != std::string::npos || result.find( "PT24H" ) != std::string::npos );

        TimeSpan daysAndTime{ TimeSpan::fromDays( 2 ) + TimeSpan::fromHours( 3 ) + TimeSpan::fromMinutes( 30 ) };
        result = daysAndTime.toString();
        EXPECT_TRUE(
            result.find( "P2DT3H30M" ) != std::string::npos || result.find( "PT51H30M" ) != std::string::npos );
    }

    //----------------------------------------------
    // String parsing
    //----------------------------------------------

    TEST( TimeSpanStringParsing, ParseCompactFormat )
    {
        // Parse compact ISO 8601 format (omits zero components)
        TimeSpan ts1{ TimeSpan{ "PT1H" } };
        EXPECT_DOUBLE_EQ( ts1.hours(), 1.0 );

        TimeSpan ts2{ TimeSpan{ "PT30M" } };
        EXPECT_DOUBLE_EQ( ts2.minutes(), 30.0 );

        TimeSpan ts3{ TimeSpan{ "PT45S" } };
        EXPECT_DOUBLE_EQ( ts3.seconds(), 45.0 );

        // Combined compact format
        TimeSpan ts4{ TimeSpan{ "PT1H30M" } };
        EXPECT_DOUBLE_EQ( ts4.minutes(), 90.0 );

        TimeSpan ts5{ TimeSpan{ "PT2H15S" } };
        EXPECT_DOUBLE_EQ( ts5.seconds(), 2 * 3600 + 15 );

        // Zero duration
        TimeSpan ts6{ TimeSpan{ "PT0S" } };
        EXPECT_DOUBLE_EQ( ts6.seconds(), 0.0 );
    }

    TEST( TimeSpanStringParsing, ParseVerboseFormat )
    {
        // Parse verbose ISO 8601 format (includes zero components)
        TimeSpan ts1{ TimeSpan{ "PT1H0M0S" } };
        EXPECT_DOUBLE_EQ( ts1.hours(), 1.0 );

        TimeSpan ts2{ TimeSpan{ "PT0H30M0S" } };
        EXPECT_DOUBLE_EQ( ts2.minutes(), 30.0 );

        TimeSpan ts3{ TimeSpan{ "PT0H0M45S" } };
        EXPECT_DOUBLE_EQ( ts3.seconds(), 45.0 );

        // Combined verbose format
        TimeSpan ts4{ TimeSpan{ "PT1H30M0S" } };
        EXPECT_DOUBLE_EQ( ts4.minutes(), 90.0 );

        TimeSpan ts5{ TimeSpan{ "PT1H0M30S" } };
        EXPECT_DOUBLE_EQ( ts5.seconds(), 3600 + 30 );

        TimeSpan ts6{ TimeSpan{ "PT0H30M45S" } };
        EXPECT_DOUBLE_EQ( ts6.seconds(), 30 * 60 + 45 );

        // Full verbose format
        TimeSpan ts7{ TimeSpan{ "PT1H30M45S" } };
        EXPECT_DOUBLE_EQ( ts7.seconds(), 3600 + 30 * 60 + 45 );

        // Zero duration verbose
        TimeSpan ts8{ TimeSpan{ "PT0H0M0S" } };
        EXPECT_DOUBLE_EQ( ts8.seconds(), 0.0 );
    }

    TEST( TimeSpanStringParsing, ParseNegativeFormats )
    {
        // Negative durations (sign before P)
        TimeSpan ts1{ TimeSpan{ "-PT1H" } };
        EXPECT_DOUBLE_EQ( ts1.hours(), -1.0 );

        TimeSpan ts2{ TimeSpan{ "-PT30M" } };
        EXPECT_DOUBLE_EQ( ts2.minutes(), -30.0 );

        TimeSpan ts3{ TimeSpan{ "-PT1H30M45S" } };
        EXPECT_DOUBLE_EQ( ts3.seconds(), -( 3600 + 30 * 60 + 45 ) );

        // Verbose negative format
        TimeSpan ts4{ TimeSpan{ "-PT1H0M0S" } };
        EXPECT_DOUBLE_EQ( ts4.hours(), -1.0 );
    }

    TEST( TimeSpanStringParsing, ParseFractionalSeconds )
    {
        // Fractional seconds in various formats
        TimeSpan ts1{ TimeSpan{ "PT1.5S" } };
        EXPECT_DOUBLE_EQ( ts1.seconds(), 1.5 );

        TimeSpan ts2{ TimeSpan{ "PT0.123S" } };
        EXPECT_NEAR( ts2.seconds(), 0.123, 0.001 );

        TimeSpan ts3{ TimeSpan{ "PT1H30M45.500S" } };
        EXPECT_NEAR( ts3.seconds(), 3600 + 30 * 60 + 45.5, 0.001 );
    }

    TEST( TimeSpanStringParsing, ParseWithDays )
    {
        // ISO 8601 with days component
        TimeSpan ts1{ TimeSpan{ "P1D" } };
        EXPECT_DOUBLE_EQ( ts1.days(), 1.0 );

        TimeSpan ts2{ TimeSpan{ "P1DT12H" } };
        EXPECT_DOUBLE_EQ( ts2.hours(), 36.0 );

        TimeSpan ts3{ TimeSpan{ "P2DT3H30M45S" } };
        EXPECT_DOUBLE_EQ( ts3.seconds(), 2 * 86400 + 3 * 3600 + 30 * 60 + 45 );

        // Days only (no time component)
        TimeSpan ts4{ TimeSpan{ "P5D" } };
        EXPECT_DOUBLE_EQ( ts4.days(), 5.0 );
    }

    TEST( TimeSpanStringParsing, FromStringCompactAndVerbose )
    {
        TimeSpan result;

        // Compact format
        EXPECT_TRUE( TimeSpan::fromString( "PT1H", result ) );
        EXPECT_DOUBLE_EQ( result.hours(), 1.0 );

        EXPECT_TRUE( TimeSpan::fromString( "PT30M", result ) );
        EXPECT_DOUBLE_EQ( result.minutes(), 30.0 );

        EXPECT_TRUE( TimeSpan::fromString( "PT45S", result ) );
        EXPECT_DOUBLE_EQ( result.seconds(), 45.0 );

        // Verbose format
        EXPECT_TRUE( TimeSpan::fromString( "PT1H0M0S", result ) );
        EXPECT_DOUBLE_EQ( result.hours(), 1.0 );

        EXPECT_TRUE( TimeSpan::fromString( "PT0H30M0S", result ) );
        EXPECT_DOUBLE_EQ( result.minutes(), 30.0 );

        EXPECT_TRUE( TimeSpan::fromString( "PT0H0M45S", result ) );
        EXPECT_DOUBLE_EQ( result.seconds(), 45.0 );

        // Mixed formats
        EXPECT_TRUE( TimeSpan::fromString( "PT1H30M", result ) );
        EXPECT_DOUBLE_EQ( result.minutes(), 90.0 );

        EXPECT_TRUE( TimeSpan::fromString( "PT1H0M30S", result ) );
        EXPECT_DOUBLE_EQ( result.seconds(), 3630.0 );

        // Negative formats
        EXPECT_TRUE( TimeSpan::fromString( "-PT1H", result ) );
        EXPECT_DOUBLE_EQ( result.hours(), -1.0 );

        EXPECT_TRUE( TimeSpan::fromString( "-PT1H0M0S", result ) );
        EXPECT_DOUBLE_EQ( result.hours(), -1.0 );
    }

    TEST( TimeSpanStringParsing, RoundTripCompactAndVerbose )
    {
        // Test that parsing both formats produces identical results
        TimeSpan compact1{ TimeSpan{ "PT1H" } };
        TimeSpan verbose1{ TimeSpan{ "PT1H0M0S" } };
        EXPECT_EQ( compact1.ticks(), verbose1.ticks() );

        TimeSpan compact2{ TimeSpan{ "PT30M" } };
        TimeSpan verbose2{ TimeSpan{ "PT0H30M0S" } };
        EXPECT_EQ( compact2.ticks(), verbose2.ticks() );

        TimeSpan compact3{ TimeSpan{ "PT45S" } };
        TimeSpan verbose3{ TimeSpan{ "PT0H0M45S" } };
        EXPECT_EQ( compact3.ticks(), verbose3.ticks() );

        TimeSpan compact4{ TimeSpan{ "PT1H30M45S" } };
        TimeSpan verbose4{ TimeSpan{ "PT1H30M45S" } };
        EXPECT_EQ( compact4.ticks(), verbose4.ticks() );

        // Round-trip: parse verbose, output compact (or vice versa)
        TimeSpan parsed{ TimeSpan{ "PT1H0M0S" } };
        std::string output{ parsed.toString() };
        TimeSpan reparsed{ TimeSpan{ output } };
        EXPECT_EQ( parsed.ticks(), reparsed.ticks() );
    }

    TEST( TimeSpanStringParsing, RejectInvalidComponentOrder )
    {
        // Should reject components in wrong order (H before M before S)
        EXPECT_FALSE( TimeSpan::fromString( "PT30M2H" ).has_value() );    // Minutes before hours
        EXPECT_FALSE( TimeSpan::fromString( "PT45S30M" ).has_value() );   // Seconds before minutes
        EXPECT_FALSE( TimeSpan::fromString( "PT45S2H" ).has_value() );    // Seconds before hours
        EXPECT_FALSE( TimeSpan::fromString( "PT30M2H15S" ).has_value() ); // Minutes before hours
        EXPECT_FALSE( TimeSpan::fromString( "PT15S30M2H" ).has_value() ); // All reversed

        // Constructor should throw for invalid order
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "PT30M2H" }; }, std::invalid_argument );
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "PT45S30M" }; }, std::invalid_argument );

        // fromString with bool should return false
        TimeSpan result;
        EXPECT_FALSE( TimeSpan::fromString( "PT30M2H", result ) );
        EXPECT_FALSE( TimeSpan::fromString( "PT45S30M", result ) );
    }

    TEST( TimeSpanStringParsing, RejectDuplicateComponents )
    {
        // Should reject duplicate time components
        EXPECT_FALSE( TimeSpan::fromString( "PT1H2H" ).has_value() );        // Duplicate hours
        EXPECT_FALSE( TimeSpan::fromString( "PT30M15M" ).has_value() );      // Duplicate minutes
        EXPECT_FALSE( TimeSpan::fromString( "PT45S30S" ).has_value() );      // Duplicate seconds
        EXPECT_FALSE( TimeSpan::fromString( "PT1H30M2H" ).has_value() );     // Hours appears twice
        EXPECT_FALSE( TimeSpan::fromString( "PT1H30M45S30M" ).has_value() ); // Minutes appears twice

        // Constructor should throw for duplicates
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "PT1H2H" }; }, std::invalid_argument );
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "PT30M15M" }; }, std::invalid_argument );

        // fromString with bool should return false
        TimeSpan result;
        EXPECT_FALSE( TimeSpan::fromString( "PT1H2H", result ) );
        EXPECT_FALSE( TimeSpan::fromString( "PT30M15M", result ) );
    }

    TEST( TimeSpanStringParsing, AcceptValidOrders )
    {
        // Should accept valid component orders (H before M before S)
        EXPECT_TRUE( TimeSpan::fromString( "PT2H30M" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT2H45S" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT30M45S" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT2H30M45S" ).has_value() );

        // Verbose formats with zeros should also work
        EXPECT_TRUE( TimeSpan::fromString( "PT0H30M0S" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT2H0M45S" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT0H0M45S" ).has_value() );

        // Partial components (omitting some) should work
        EXPECT_TRUE( TimeSpan::fromString( "PT2H" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT30M" ).has_value() );
        EXPECT_TRUE( TimeSpan::fromString( "PT45S" ).has_value() );
    }

    //----------------------------------------------
    // Factory
    //----------------------------------------------

    TEST( TimeSpanFactory, StaticFactoryMethods )
    {
        // fromDays
        TimeSpan days{ TimeSpan::fromDays( 1.5 ) };
        EXPECT_DOUBLE_EQ( days.days(), 1.5 );
        EXPECT_DOUBLE_EQ( days.hours(), 36.0 );

        // fromHours
        TimeSpan hours{ TimeSpan::fromHours( 2.5 ) };
        EXPECT_DOUBLE_EQ( hours.hours(), 2.5 );
        EXPECT_DOUBLE_EQ( hours.minutes(), 150.0 );

        // fromMinutes
        TimeSpan minutes{ TimeSpan::fromMinutes( 90 ) };
        EXPECT_DOUBLE_EQ( minutes.minutes(), 90.0 );
        EXPECT_DOUBLE_EQ( minutes.hours(), 1.5 );

        // fromSeconds
        TimeSpan seconds{ TimeSpan::fromSeconds( 3600 ) };
        EXPECT_DOUBLE_EQ( seconds.seconds(), 3600.0 );
        EXPECT_DOUBLE_EQ( seconds.hours(), 1.0 );

        // fromMilliseconds
        TimeSpan milliseconds{ TimeSpan::fromMilliseconds( 5000 ) };
        EXPECT_DOUBLE_EQ( milliseconds.milliseconds(), 5000.0 );
        EXPECT_DOUBLE_EQ( milliseconds.seconds(), 5.0 );

        // fromMicroseconds
        TimeSpan microseconds{ TimeSpan::fromMicroseconds( 5000 ) };
        EXPECT_DOUBLE_EQ( microseconds.microseconds(), 5000.0 );
        EXPECT_DOUBLE_EQ( microseconds.milliseconds(), 5.0 );

        // fromTicks
        TimeSpan ticks{ TimeSpan::fromTicks( 10000000 ) }; // 1 second = 10,000,000 ticks
        EXPECT_EQ( ticks.ticks(), 10000000 );
        EXPECT_DOUBLE_EQ( ticks.seconds(), 1.0 );

        // Test precision
        TimeSpan precise{ TimeSpan::fromSeconds( 1.5 ) };
        EXPECT_DOUBLE_EQ( precise.milliseconds(), 1500.0 );
    }

    TEST( TimeSpanFactory, Microseconds )
    {
        // Test microseconds conversions
        TimeSpan us1{ TimeSpan::fromMicroseconds( 1000 ) };
        EXPECT_DOUBLE_EQ( us1.milliseconds(), 1.0 );
        EXPECT_DOUBLE_EQ( us1.microseconds(), 1000.0 );

        TimeSpan us2{ TimeSpan::fromMicroseconds( 1000000 ) };
        EXPECT_DOUBLE_EQ( us2.seconds(), 1.0 );
    }

    //----------------------------------------------
    // Safe parsing with fromString()
    //----------------------------------------------

    TEST( TimeSpanStringParsing, FromStringMethod )
    {
        TimeSpan result;

        // Valid ISO 8601 duration
        EXPECT_TRUE( TimeSpan::fromString( "PT1H30M45S", result ) );
        EXPECT_DOUBLE_EQ( result.minutes(), 90.75 );

        // Valid numeric seconds
        EXPECT_TRUE( TimeSpan::fromString( "60.5", result ) );
        EXPECT_DOUBLE_EQ( result.seconds(), 60.5 );

        // Invalid formats should return false
        EXPECT_FALSE( TimeSpan::fromString( "invalid", result ) );
        EXPECT_FALSE( TimeSpan::fromString( "", result ) );
        EXPECT_FALSE( TimeSpan::fromString( "PT", result ) );
    }

    TEST( TimeSpanStringParsing, FromStringOptionalValid )
    {
        // Valid ISO 8601 duration
        auto ts1{ TimeSpan::fromString( "PT1H30M45S" ) };
        EXPECT_TRUE( ts1.has_value() );
        EXPECT_DOUBLE_EQ( ts1->minutes(), 90.75 );

        // Valid numeric seconds
        auto ts2{ TimeSpan::fromString( "60.5" ) };
        EXPECT_TRUE( ts2.has_value() );
        EXPECT_DOUBLE_EQ( ts2->seconds(), 60.5 );
    }

    TEST( TimeSpanStringParsing, FromStringOptionalInvalid )
    {
        // Invalid formats should return nullopt
        EXPECT_FALSE( TimeSpan::fromString( "invalid" ).has_value() );
        EXPECT_FALSE( TimeSpan::fromString( "" ).has_value() );
        EXPECT_FALSE( TimeSpan::fromString( "PT" ).has_value() );
    }

    TEST( TimeSpanStringParsing, ConstructorThrowsOnInvalid )
    {
        // Invalid format should throw
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "invalid" }; }, std::invalid_argument );
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "" }; }, std::invalid_argument );
        EXPECT_THROW( { [[maybe_unused]] auto _ = TimeSpan{ "PT" }; }, std::invalid_argument );
    }

    //----------------------------------------------
    // std::chrono interoperability
    //----------------------------------------------

    TEST( TimeSpanChrono, ToChrono )
    {
        // Convert TimeSpan to chrono duration
        TimeSpan ts1{ TimeSpan::fromSeconds( 90 ) };
        auto chronoDuration{ ts1.toChrono() };
        auto chronoSeconds{ std::chrono::duration_cast<std::chrono::seconds>( chronoDuration ) };
        EXPECT_EQ( chronoSeconds.count(), 90 );

        // Test with fractional seconds
        TimeSpan ts2{ TimeSpan::fromSeconds( 1.5 ) };
        auto chronoDuration2{ ts2.toChrono() };
        auto chronoMillis{ std::chrono::duration_cast<std::chrono::milliseconds>( chronoDuration2 ) };
        EXPECT_EQ( chronoMillis.count(), 1500 );

        // Test with hours
        TimeSpan ts3{ TimeSpan::fromHours( 2 ) };
        auto chronoDuration3{ ts3.toChrono() };
        auto chronoHours{ std::chrono::duration_cast<std::chrono::hours>( chronoDuration3 ) };
        EXPECT_EQ( chronoHours.count(), 2 );
    }

    TEST( TimeSpanChrono, FromChrono )
    {
        // Create TimeSpan from chrono seconds
        auto chronoSeconds{ std::chrono::seconds( 120 ) };
        TimeSpan ts1{ TimeSpan::fromChrono( chronoSeconds ) };
        EXPECT_DOUBLE_EQ( ts1.seconds(), 120.0 );

        // Create TimeSpan from chrono milliseconds
        auto chronoMillis{ std::chrono::milliseconds( 2500 ) };
        TimeSpan ts2{ TimeSpan::fromChrono( chronoMillis ) };
        EXPECT_DOUBLE_EQ( ts2.milliseconds(), 2500.0 );
        EXPECT_DOUBLE_EQ( ts2.seconds(), 2.5 );

        // Create TimeSpan from chrono hours
        auto chronoHours{ std::chrono::hours( 3 ) };
        TimeSpan ts3{ TimeSpan::fromChrono( chronoHours ) };
        EXPECT_DOUBLE_EQ( ts3.hours(), 3.0 );

        // Create TimeSpan from chrono minutes
        auto chronoMinutes{ std::chrono::minutes( 90 ) };
        TimeSpan ts4{ TimeSpan::fromChrono( chronoMinutes ) };
        EXPECT_DOUBLE_EQ( ts4.minutes(), 90.0 );

        // Round-trip conversion
        TimeSpan original{ TimeSpan::fromSeconds( 45.5 ) };
        auto chronoDuration{ original.toChrono() };
        TimeSpan roundTrip{ TimeSpan::fromChrono( chronoDuration ) };
        EXPECT_NEAR( roundTrip.seconds(), original.seconds(), 0.001 );
    }

    //----------------------------------------------
    // Stream operators
    //----------------------------------------------

    TEST( TimeSpanStreamOperators, OutputOperator )
    {
        TimeSpan ts{ TimeSpan::fromHours( 1.5 ) };
        std::ostringstream oss;
        oss << ts;

        std::string output{ oss.str() };
        EXPECT_FALSE( output.empty() );
        EXPECT_TRUE( output.find( "PT" ) != std::string::npos );
    }

    TEST( TimeSpanStreamOperators, InputOperator )
    {
        std::istringstream iss{ "PT1H30M45S" };
        TimeSpan ts;
        iss >> ts;

        EXPECT_FALSE( iss.fail() );
        EXPECT_DOUBLE_EQ( ts.minutes(), 90.75 );

        // Invalid input should set failbit
        std::istringstream iss2{ "invalid" };
        TimeSpan ts2;
        iss2 >> ts2;
        EXPECT_TRUE( iss2.fail() );
    }

    //----------------------------------------------
    // std::formatter support
    //----------------------------------------------

    TEST( TimeSpanFormatter, BasicFormatting )
    {
        TimeSpan duration{ TimeSpan::fromHours( 2.5 ) };
        std::string formatted{ std::format( "{}", duration ) };

        // Should be in ISO 8601 duration format
        EXPECT_NE( formatted.find( "PT" ), std::string::npos );
        EXPECT_NE( formatted.find( "2H30M" ), std::string::npos );
    }

    TEST( TimeSpanFormatter, FormatNegativeDuration )
    {
        TimeSpan duration{ TimeSpan::fromMinutes( -45.0 ) };
        std::string formatted{ std::format( "{}", duration ) };

        EXPECT_NE( formatted.find( "-PT45M" ), std::string::npos );
    }

    TEST( TimeSpanFormatter, FormatInString )
    {
        TimeSpan duration{ TimeSpan::fromDays( 1.5 ) };
        std::string message{ std::format( "Task duration: {}", duration ) };

        EXPECT_NE( message.find( "Task duration:" ), std::string::npos );
        EXPECT_NE( message.find( "P1DT12H" ), std::string::npos );
    }

    TEST( TimeSpanFormatter, FormatMultipleDurations )
    {
        TimeSpan short_duration{ TimeSpan::fromMinutes( 30.0 ) };
        TimeSpan long_duration{ TimeSpan::fromHours( 8.0 ) };
        std::string range{ std::format( "Break: {}, Work: {}", short_duration, long_duration ) };

        EXPECT_NE( range.find( "Break:" ), std::string::npos );
        EXPECT_NE( range.find( "PT30M" ), std::string::npos );
        EXPECT_NE( range.find( "Work:" ), std::string::npos );
        EXPECT_NE( range.find( "PT8H" ), std::string::npos );
    }

    TEST( TimeSpanFormatter, FormatWithOtherTypes )
    {
        TimeSpan duration{ TimeSpan::fromSeconds( 3661.0 ) }; // 1h 1m 1s
        int count{ 5 };
        std::string mixed{ std::format( "Processed {} items in {}", count, duration ) };

        EXPECT_NE( mixed.find( "Processed 5 items in" ), std::string::npos );
        EXPECT_NE( mixed.find( "PT1H1M1S" ), std::string::npos );
    }

    TEST( TimeSpanFormatter, FormatZeroDuration )
    {
        TimeSpan zero;
        std::string formatted{ std::format( "{}", zero ) };

        EXPECT_NE( formatted.find( "PT0S" ), std::string::npos );
    }

    //----------------------------------------------
    // Literals
    //----------------------------------------------

    TEST( TimeSpanLiterals, DaysLiteral )
    {
        using namespace nfx::time::literals;

        // Integer days
        auto days1 = 1_d;
        EXPECT_DOUBLE_EQ( days1.days(), 1.0 );
        EXPECT_DOUBLE_EQ( days1.hours(), 24.0 );

        auto days2 = 7_d;
        EXPECT_DOUBLE_EQ( days2.days(), 7.0 );

        // Floating-point days
        auto days3 = 1.5_d;
        EXPECT_DOUBLE_EQ( days3.days(), 1.5 );
        EXPECT_DOUBLE_EQ( days3.hours(), 36.0 );

        auto days4 = 0.5_d;
        EXPECT_DOUBLE_EQ( days4.hours(), 12.0 );
    }

    TEST( TimeSpanLiterals, HoursLiteral )
    {
        using namespace nfx::time::literals;

        // Integer hours
        auto hours1 = 1_h;
        EXPECT_DOUBLE_EQ( hours1.hours(), 1.0 );
        EXPECT_DOUBLE_EQ( hours1.minutes(), 60.0 );

        auto hours2 = 24_h;
        EXPECT_DOUBLE_EQ( hours2.days(), 1.0 );

        // Floating-point hours
        auto hours3 = 2.5_h;
        EXPECT_DOUBLE_EQ( hours3.hours(), 2.5 );
        EXPECT_DOUBLE_EQ( hours3.minutes(), 150.0 );

        auto hours4 = 0.5_h;
        EXPECT_DOUBLE_EQ( hours4.minutes(), 30.0 );
    }

    TEST( TimeSpanLiterals, MinutesLiteral )
    {
        using namespace nfx::time::literals;

        // Integer minutes
        auto minutes1 = 1_min;
        EXPECT_DOUBLE_EQ( minutes1.minutes(), 1.0 );
        EXPECT_DOUBLE_EQ( minutes1.seconds(), 60.0 );

        auto minutes2 = 60_min;
        EXPECT_DOUBLE_EQ( minutes2.hours(), 1.0 );

        // Floating-point minutes
        auto minutes3 = 30.5_min;
        EXPECT_DOUBLE_EQ( minutes3.minutes(), 30.5 );
        EXPECT_DOUBLE_EQ( minutes3.seconds(), 1830.0 );

        auto minutes4 = 0.5_min;
        EXPECT_DOUBLE_EQ( minutes4.seconds(), 30.0 );
    }

    TEST( TimeSpanLiterals, SecondsLiteral )
    {
        using namespace nfx::time::literals;

        // Integer seconds
        auto seconds1 = 1_s;
        EXPECT_DOUBLE_EQ( seconds1.seconds(), 1.0 );
        EXPECT_DOUBLE_EQ( seconds1.milliseconds(), 1000.0 );

        auto seconds2 = 60_s;
        EXPECT_DOUBLE_EQ( seconds2.minutes(), 1.0 );

        // Floating-point seconds
        auto seconds3 = 1.5_s;
        EXPECT_DOUBLE_EQ( seconds3.seconds(), 1.5 );
        EXPECT_DOUBLE_EQ( seconds3.milliseconds(), 1500.0 );

        auto seconds4 = 0.5_s;
        EXPECT_DOUBLE_EQ( seconds4.milliseconds(), 500.0 );
    }

    TEST( TimeSpanLiterals, MillisecondsLiteral )
    {
        using namespace nfx::time::literals;

        // Integer milliseconds
        auto ms1 = 1_ms;
        EXPECT_DOUBLE_EQ( ms1.milliseconds(), 1.0 );

        auto ms2 = 1000_ms;
        EXPECT_DOUBLE_EQ( ms2.seconds(), 1.0 );

        // Floating-point milliseconds
        auto ms3 = 1500.5_ms;
        EXPECT_DOUBLE_EQ( ms3.milliseconds(), 1500.5 );
        EXPECT_NEAR( ms3.seconds(), 1.5005, 0.0001 );

        auto ms4 = 0.5_ms;
        EXPECT_NEAR( ms4.milliseconds(), 0.5, 0.0001 );
    }

    TEST( TimeSpanLiterals, MicrosecondsLiteral )
    {
        using namespace nfx::time::literals;

        // Integer microseconds
        auto us1 = 1_us;
        EXPECT_DOUBLE_EQ( us1.microseconds(), 1.0 );

        auto us2 = 1000_us;
        EXPECT_DOUBLE_EQ( us2.milliseconds(), 1.0 );

        auto us3 = 1000000_us;
        EXPECT_DOUBLE_EQ( us3.seconds(), 1.0 );

        // Floating-point microseconds
        auto us4 = 1500.5_us;
        EXPECT_DOUBLE_EQ( us4.microseconds(), 1500.5 );
        EXPECT_NEAR( us4.milliseconds(), 1.5005, 0.0001 );

        auto us5 = 0.5_us;
        EXPECT_NEAR( us5.microseconds(), 0.5, 0.0001 );
    }

    TEST( TimeSpanLiterals, NanosecondsLiteral )
    {
        using namespace nfx::time::literals;

        // Integer nanoseconds
        auto ns1 = 100_ns;
        EXPECT_DOUBLE_EQ( ns1.nanoseconds(), 100.0 );
        EXPECT_DOUBLE_EQ( ns1.ticks(), 1 ); // 100 ns = 1 tick

        auto ns2 = 1000_ns;
        EXPECT_DOUBLE_EQ( ns2.microseconds(), 1.0 );

        auto ns3 = 1000000_ns;
        EXPECT_DOUBLE_EQ( ns3.milliseconds(), 1.0 );

        // Floating-point nanoseconds (rounds to nearest 100ns tick)
        auto ns4 = 150.5_ns;
        EXPECT_DOUBLE_EQ( ns4.nanoseconds(), 200.0 ); // Rounds to 2 ticks

        // Note: Due to 100ns tick precision, values may be rounded
        auto ns5 = 50_ns; // Rounds to 1 tick (100 ns)
        EXPECT_DOUBLE_EQ( ns5.nanoseconds(), 100.0 );
    }

    TEST( TimeSpanLiterals, CombinedLiterals )
    {
        using namespace nfx::time::literals;

        // Simple combinations
        auto duration1 = 1_h + 30_min;
        EXPECT_DOUBLE_EQ( duration1.minutes(), 90.0 );

        auto duration2 = 2_h + 15_min + 30_s;
        EXPECT_DOUBLE_EQ( duration2.seconds(), 2 * 3600 + 15 * 60 + 30 );

        // Complex combination
        auto duration3 = 1_d + 2_h + 30_min + 45_s + 500_ms;
        EXPECT_DOUBLE_EQ(
            duration3.days(), 1.0 + ( 2.0 / 24.0 ) + ( 30.0 / ( 24.0 * 60.0 ) ) + ( 45.5 / ( 24.0 * 3600.0 ) ) );

        // Subtraction
        auto duration4 = 2_h - 30_min;
        EXPECT_DOUBLE_EQ( duration4.minutes(), 90.0 );

        // Mixed operations
        auto duration5 = ( 3_h + 30_min ) - ( 1_h + 15_min );
        EXPECT_DOUBLE_EQ( duration5.minutes(), 135.0 );
    }

    TEST( TimeSpanLiterals, CompileTimeEvaluation )
    {
        using namespace nfx::time::literals;

        // Test constexpr evaluation
        constexpr auto ct1 = 2_h;
        EXPECT_DOUBLE_EQ( ct1.hours(), 2.0 );

        constexpr auto ct2 = 30_min;
        EXPECT_DOUBLE_EQ( ct2.minutes(), 30.0 );

        constexpr auto ct3 = 45_s;
        EXPECT_DOUBLE_EQ( ct3.seconds(), 45.0 );

        // Constexpr arithmetic
        constexpr auto ct4 = 2_h + 30_min;
        EXPECT_DOUBLE_EQ( ct4.minutes(), 150.0 );

        constexpr auto ct5 = 1_d + 12_h;
        EXPECT_DOUBLE_EQ( ct5.hours(), 36.0 );
    }

    TEST( TimeSpanLiterals, NegativeLiterals )
    {
        using namespace nfx::time::literals;

        // Negation operator
        auto neg1 = -2_h;
        EXPECT_DOUBLE_EQ( neg1.hours(), -2.0 );

        auto neg2 = -30_min;
        EXPECT_DOUBLE_EQ( neg2.minutes(), -30.0 );

        // Combining positive and negative
        auto combined = 2_h + ( -30_min );
        EXPECT_DOUBLE_EQ( combined.minutes(), 90.0 );

        auto combined2 = 1_h - 30_min;
        EXPECT_DOUBLE_EQ( combined2.minutes(), 30.0 );
    }

    TEST( TimeSpanLiterals, ComparisonWithLiterals )
    {
        using namespace nfx::time::literals;

        // Direct comparisons
        EXPECT_TRUE( 1_h < 2_h );
        EXPECT_TRUE( 30_min == 0.5_h );
        EXPECT_TRUE( 60_s == 1_min );
        EXPECT_TRUE( 1000_ms == 1_s );

        // Complex comparisons
        EXPECT_TRUE( ( 1_h + 30_min ) > 1_h );
        EXPECT_TRUE( ( 2_h - 30_min ) == 90_min );
        EXPECT_FALSE( 1_d == 23_h );
    }

    TEST( TimeSpanLiterals, WithFactoryMethods )
    {
        using namespace nfx::time::literals;

        // Literals should produce same result as factory methods
        auto lit1 = 2.5_h;
        auto fact1 = TimeSpan::fromHours( 2.5 );
        EXPECT_EQ( lit1.ticks(), fact1.ticks() );

        auto lit2 = 30_min;
        auto fact2 = TimeSpan::fromMinutes( 30.0 );
        EXPECT_EQ( lit2.ticks(), fact2.ticks() );

        auto lit3 = 45_s;
        auto fact3 = TimeSpan::fromSeconds( 45.0 );
        EXPECT_EQ( lit3.ticks(), fact3.ticks() );

        auto lit4 = 1500_ms;
        auto fact4 = TimeSpan::fromMilliseconds( 1500.0 );
        EXPECT_EQ( lit4.ticks(), fact4.ticks() );
    }

    TEST( TimeSpanLiterals, StringFormatting )
    {
        using namespace nfx::time::literals;

        // Test that literals format correctly
        auto duration1 = 2_h + 30_min;
        EXPECT_EQ( duration1.toString(), "PT2H30M" );

        auto duration2 = 1_d + 12_h;
        std::string result = duration2.toString();
        EXPECT_TRUE( result.find( "P1DT12H" ) != std::string::npos || result.find( "PT36H" ) != std::string::npos );

        auto duration3 = 45_s + 500_ms;
        result = duration3.toString();
        EXPECT_TRUE( result.find( "PT45.5" ) != std::string::npos || result.find( "PT45.500S" ) != std::string::npos );
    }

    TEST( TimeSpanLiterals, RealWorldExamples )
    {
        using namespace nfx::time::literals;

        // Work day calculation
        auto workDay = 8_h + 30_min;
        auto breakTime = 1_h;
        auto actualWork = workDay - breakTime;
        EXPECT_DOUBLE_EQ( actualWork.hours(), 7.5 );

        // Video length
        auto videoLength = 1_h + 23_min + 45_s;
        EXPECT_DOUBLE_EQ( videoLength.seconds(), 3600 + 23 * 60 + 45 );

        // Cooking timer
        auto prepTime = 15_min;
        auto cookTime = 45_min;
        auto totalTime = prepTime + cookTime;
        EXPECT_DOUBLE_EQ( totalTime.hours(), 1.0 );

        // Sleep duration
        auto sleepDuration = 8_h + 15_min;
        EXPECT_DOUBLE_EQ( sleepDuration.minutes(), 495.0 );
    }

    //----------------------------------------------
    // Edge cases
    //----------------------------------------------

    TEST( TimeSpanEdgeCases, ZeroDuration )
    {
        TimeSpan zero;
        EXPECT_EQ( zero.ticks(), 0 );
        EXPECT_DOUBLE_EQ( zero.seconds(), 0.0 );
        EXPECT_EQ( zero.toString(), "PT0S" ); // Compact format preferred

        // Verify verbose format is also parseable
        TimeSpan parsed;
        EXPECT_TRUE( TimeSpan::fromString( "PT0H0M0S", parsed ) );
        EXPECT_EQ( parsed.ticks(), 0 );
    }

    TEST( TimeSpanEdgeCases, NegativeDurations )
    {
        TimeSpan neg{ TimeSpan::fromHours( -2.5 ) };
        EXPECT_DOUBLE_EQ( neg.hours(), -2.5 );
        EXPECT_LT( neg.ticks(), 0 );

        // Arithmetic with negative durations
        TimeSpan pos{ TimeSpan::fromHours( 3 ) };
        TimeSpan result{ pos + neg };
        EXPECT_DOUBLE_EQ( result.hours(), 0.5 );
    }

    TEST( TimeSpanEdgeCases, VeryLargeDurations )
    {
        // Test near the limits (±10,675,199 days ≈ ±29,247 years)
        TimeSpan large{ TimeSpan::fromDays( 10000000 ) };
        EXPECT_DOUBLE_EQ( large.days(), 10000000.0 );
        EXPECT_GT( large.ticks(), 0 );
    }

    //----------------------------------------------
    // Integration
    //----------------------------------------------

    TEST( TimeSpanIntegration, RoundTripSerialization )
    {
        TimeSpan original{ TimeSpan::fromHours( 2 ) + TimeSpan::fromMinutes( 30 ) + TimeSpan::fromSeconds( 45.5 ) };

        // Convert to string and back
        std::string serialized{ original.toString() };
        TimeSpan deserialized{ TimeSpan{ serialized } };

        // Should be approximately equal (may have rounding in milliseconds)
        EXPECT_NEAR( deserialized.seconds(), original.seconds(), 0.001 );
    }

    TEST( TimeSpanIntegration, ArithmeticChaining )
    {
        // Test complex arithmetic operations
        TimeSpan result{ TimeSpan::fromHours( 5 ) + TimeSpan::fromMinutes( 30 ) - TimeSpan::fromSeconds( 45 ) +
                         TimeSpan::fromMilliseconds( 500 ) };

        double expectedSeconds{ 5 * 3600 + 30 * 60 - 45 + 0.5 };
        EXPECT_NEAR( result.seconds(), expectedSeconds, 0.001 );
    }
} // namespace nfx::time::test
