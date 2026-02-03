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
 * @file DateTime.cpp
 * @brief Implementation of DateTime class methods for datetime operations
 * @details Provides parsing logic for ISO 8601 format, string conversion and formatting,
 *          date/time component extraction, validation logic, and factory methods for
 *          creating DateTime instances. Implements cross-platform time conversions and
 *          arithmetic operations with 100-nanosecond precision. Supports both UTC and
 *          local time representations.
 */

#include "nfx/datetime/DateTime.h"
#include "Internal.h"

#include <nfx/string/StringBuilder.h>

#include <charconv>
#include <istream>
#include <limits>

namespace nfx::time
{
    namespace internal
    {
        //=====================================================================
        // std::chrono interoperability limits
        //=====================================================================

        /**
         * @brief Minimum DateTime value that can safely round-trip through std::chrono::system_clock
         * @details This is platform-dependent; on systems with 64-bit nanosecond precision,
         *          this is approximately year 1678.
         */
        static constexpr std::int64_t MIN_CHRONO_SAFE_TICKS = std::max(
            constants::MIN_DATETIME_TICKS,
            constants::UNIX_EPOCH_TICKS + ( std::numeric_limits<std::int64_t>::min() / 100 ) );

        /**
         * @brief Maximum DateTime value that can safely round-trip through std::chrono::system_clock
         * @details This is platform-dependent; on systems with 64-bit nanosecond precision,
         *          this is approximately year 2262.
         */
        static constexpr std::int64_t MAX_CHRONO_SAFE_TICKS = std::min(
            constants::MAX_DATETIME_TICKS,
            constants::UNIX_EPOCH_TICKS + ( std::numeric_limits<std::int64_t>::max() / 100 ) );

        //=====================================================================
        //  Internal helper methods
        //=====================================================================

        /** @brief Convert ticks to date components */
        static constexpr void dateComponentsFromTicks(
            std::int64_t ticks, std::int32_t& year, std::int32_t& month, std::int32_t& day ) noexcept
        {
            // Extract date components using Gregorian 400-year cycle algorithm (O(1) complexity)
            std::int64_t totalDays{ ticks / constants::TICKS_PER_DAY };

            // Use 400-year cycles for O(1) year calculation
            std::int64_t num400Years{ totalDays / constants::DAYS_PER_400_YEARS };
            totalDays %= constants::DAYS_PER_400_YEARS;

            // Extract 100-year periods (handle leap year edge case at 400-year boundary)
            std::int64_t num100Years{ totalDays / constants::DAYS_PER_100_YEARS };
            if( num100Years > 3 )
            {
                num100Years = 3; // Year divisible by 400 is a leap year
            }
            totalDays -= num100Years * constants::DAYS_PER_100_YEARS;

            // Extract 4-year cycles
            std::int64_t num4Years{ totalDays / constants::DAYS_PER_4_YEARS };
            totalDays %= constants::DAYS_PER_4_YEARS;

            // Extract remaining years (handle leap year edge case at 4-year boundary)
            std::int64_t numYears{ totalDays / constants::DAYS_PER_YEAR };
            if( numYears > 3 )
            {
                numYears = 3; // 4th year in cycle is a leap year
            }
            totalDays -= numYears * constants::DAYS_PER_YEAR;

            // Calculate final year (add 1 because year 1 is the base)
            year = static_cast<std::int32_t>( 1 + num400Years * 400 + num100Years * 100 + num4Years * 4 + numYears );

            // Find the month by iterating through months (already O(1) - max 12 iterations)
            month = 1;
            while( month <= 12 )
            {
                std::int32_t daysInCurrentMonth{ DateTime::daysInMonth( year, month ) };
                if( totalDays < daysInCurrentMonth )
                {
                    break;
                }

                totalDays -= daysInCurrentMonth;
                ++month;
            }

            // Remaining days is the day of month (add 1 because day is 1-based)
            day = static_cast<std::int32_t>( totalDays ) + 1;
        }

        /** @brief Convert ticks to time components */
        static constexpr void timeComponentsFromTicks(
            std::int64_t ticks,
            std::int32_t& hour,
            std::int32_t& minute,
            std::int32_t& second,
            std::int32_t& millisecond ) noexcept
        {
            std::int64_t timeTicks{ ticks % constants::TICKS_PER_DAY };

            hour = static_cast<std::int32_t>( timeTicks / constants::TICKS_PER_HOUR );
            timeTicks %= constants::TICKS_PER_HOUR;

            minute = static_cast<std::int32_t>( timeTicks / constants::TICKS_PER_MINUTE );
            timeTicks %= constants::TICKS_PER_MINUTE;

            second = static_cast<std::int32_t>( timeTicks / constants::TICKS_PER_SECOND );
            timeTicks %= constants::TICKS_PER_SECOND;

            millisecond = static_cast<std::int32_t>( timeTicks / constants::TICKS_PER_MILLISECOND );
        }

        /** @brief Convert date components to ticks */
        static constexpr std::int64_t dateToTicks( std::int32_t year, std::int32_t month, std::int32_t day ) noexcept
        {
            // Calculate total days since January 1, year 1 using Gregorian 400-year cycle algorithm (O(1) complexity)
            std::int64_t totalDays{ 0 };

            // Adjust to 0-based year for calculation
            std::int32_t y{ year - 1 };

            // Add days for complete 400-year cycles
            totalDays += ( y / 400 ) * constants::DAYS_PER_400_YEARS;
            y %= 400;

            // Add days for complete 100-year periods
            totalDays += ( y / 100 ) * constants::DAYS_PER_100_YEARS;
            y %= 100;

            // Add days for complete 4-year cycles
            totalDays += ( y / 4 ) * constants::DAYS_PER_4_YEARS;
            y %= 4;

            // Add days for remaining years
            totalDays += y * constants::DAYS_PER_YEAR;

            // Add days for complete months in the given year
            for( std::int32_t m{ 1 }; m < month; ++m )
            {
                totalDays += DateTime::daysInMonth( year, m );
            }

            // Add days in the current month (subtract 1 because day is 1-based)
            totalDays += day - 1;

            return totalDays * constants::TICKS_PER_DAY;
        }

        /** @brief Convert time components to ticks */
        static constexpr std::int64_t timeToTicks(
            std::int32_t hour, std::int32_t minute, std::int32_t second, std::int32_t millisecond ) noexcept
        {
            return ( static_cast<std::int64_t>( hour ) * constants::TICKS_PER_HOUR ) +
                   ( static_cast<std::int64_t>( minute ) * constants::TICKS_PER_MINUTE ) +
                   ( static_cast<std::int64_t>( second ) * constants::TICKS_PER_SECOND ) +
                   ( static_cast<std::int64_t>( millisecond ) * constants::TICKS_PER_MILLISECOND );
        }

        /** @brief Validate date components */
        static constexpr bool isValidDate( std::int32_t year, std::int32_t month, std::int32_t day ) noexcept
        {
            if( year < constants::MIN_YEAR || year > constants::MAX_YEAR )
            {
                return false;
            }
            if( month < 1 || month > 12 )
            {
                return false;
            }
            if( day < 1 || day > DateTime::daysInMonth( year, month ) )
            {
                return false;
            }

            return true;
        }

        /** @brief Validate time components */
        static constexpr bool isValidTime(
            std::int32_t hour, std::int32_t minute, std::int32_t second, std::int32_t millisecond ) noexcept
        {
            return hour >= 0 && hour <= constants::HOURS_PER_DAY - 1 && minute >= 0 &&
                   minute <= constants::MINUTES_PER_HOUR - 1 && second >= 0 &&
                   second <= constants::SECONDS_PER_MINUTE - 1 && millisecond >= 0 &&
                   millisecond <= constants::MILLISECONDS_PER_SECOND - 1;
        }
    } // namespace internal

    //=====================================================================
    // Fast parsing helpers
    //=====================================================================

    namespace
    {
        /** @brief Fast parse 2 digits without validation (inline-optimized) */
        [[nodiscard]] constexpr std::int32_t parse2Digits( const char* p ) noexcept
        {
            return ( p[0] - '0' ) * 10 + ( p[1] - '0' );
        }

        /** @brief Fast parse 4 digits without validation (inline-optimized) */
        [[nodiscard]] constexpr std::int32_t parse4Digits( const char* p ) noexcept
        {
            return ( p[0] - '0' ) * 1000 + ( p[1] - '0' ) * 100 + ( p[2] - '0' ) * 10 + ( p[3] - '0' );
        }

        /** @brief Check if character is a digit */
        [[nodiscard]] constexpr bool isDigit( char c ) noexcept
        {
            return c >= '0' && c <= '9';
        }

        /** @brief Validate that all positions contain digits */
        [[nodiscard]] constexpr bool areDigits( const char* p, std::size_t count ) noexcept
        {
            for( std::size_t i = 0; i < count; ++i )
            {
                if( !isDigit( p[i] ) )
                {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Fast-path parser for standard ISO 8601 formats
         * @details Handles the most common formats with fixed positions:
         *          - "YYYY-MM-DD" (10 chars)
         *          - "YYYY-MM-DDTHH:mm:ss" (19 chars)
         *          - "YYYY-MM-DDTHH:mm:ssZ" (20 chars)
         *          - "YYYY-MM-DDTHH:mm:ss.f" (21-27 chars)
         *          - "YYYY-MM-DDTHH:mm:ss.fZ" (22-28 chars)
         * @return true if parsed successfully via fast path, false if fallback needed
         */
        [[nodiscard]] bool tryParseFastPath( std::string_view str, DateTime& result ) noexcept
        {
            const std::size_t len = str.length();

            // Fast path requirements: minimum 10 chars (YYYY-MM-DD)
            if( len < 10 )
            {
                return false;
            }

            const char* data = str.data();

            // Validate fixed separators and digit positions for date part
            if( data[4] != '-' || data[7] != '-' || !areDigits( data, 4 ) || !areDigits( data + 5, 2 ) ||
                !areDigits( data + 8, 2 ) )
            {
                return false;
            }

            // Parse date components using fast digit parsing
            const std::int32_t year = parse4Digits( data );
            const std::int32_t month = parse2Digits( data + 5 );
            const std::int32_t day = parse2Digits( data + 8 );

            // Validate date components
            if( !internal::isValidDate( year, month, day ) )
            {
                return false;
            }

            // Date-only format: "YYYY-MM-DD"
            if( len == 10 )
            {
                const std::int64_t ticks = internal::dateToTicks( year, month, day );
                result = DateTime{ ticks };
                return true;
            }

            // Time part must start with 'T'
            if( data[10] != 'T' )
            {
                return false;
            }

            // Need at least "YYYY-MM-DDTHH:mm:ss" (19 chars)
            if( len < 19 )
            {
                return false;
            }

            // Validate time separators and digits
            if( data[13] != ':' || data[16] != ':' || !areDigits( data + 11, 2 ) || !areDigits( data + 14, 2 ) ||
                !areDigits( data + 17, 2 ) )
            {
                return false;
            }

            // Parse time components
            const std::int32_t hour = parse2Digits( data + 11 );
            const std::int32_t minute = parse2Digits( data + 14 );
            const std::int32_t second = parse2Digits( data + 17 );

            // Validate time components
            if( !internal::isValidTime( hour, minute, second, 0 ) )
            {
                return false;
            }

            std::int32_t fractionalTicks = 0;
            std::size_t pos = 19;

            // Handle optional 'Z' at position 19
            if( len == 20 && data[19] == 'Z' )
            {
                // "YYYY-MM-DDTHH:mm:ssZ" - no fractional seconds
                pos = 20;
            }
            // Handle fractional seconds
            else if( len > 19 && data[19] == '.' )
            {
                pos = 20; // Start after '.'

                // Parse up to 7 fractional digits (100ns precision)
                std::int32_t fractionValue = 0;
                std::int32_t fractionDigits = 0;

                while( pos < len && isDigit( data[pos] ) && fractionDigits < 7 )
                {
                    fractionValue = fractionValue * 10 + ( data[pos] - '0' );
                    ++fractionDigits;
                    ++pos;
                }

                if( fractionDigits == 0 )
                {
                    return false; // '.' must be followed by at least one digit
                }

                // Pad to 7 digits (convert to 100ns ticks)
                while( fractionDigits < 7 )
                {
                    fractionValue *= 10;
                    ++fractionDigits;
                }

                fractionalTicks = fractionValue;

                // Skip remaining fractional digits beyond our precision
                while( pos < len && isDigit( data[pos] ) )
                {
                    ++pos;
                }

                // Optional 'Z' after fractional seconds
                if( pos < len && data[pos] == 'Z' )
                {
                    ++pos;
                }
            }
            else if( len != 19 )
            {
                // Unexpected format - not a standard ISO 8601 we can fast-path
                return false;
            }

            // Should have consumed the entire string
            if( pos != len )
            {
                return false;
            }

            // Calculate total ticks
            const std::int64_t ticks = internal::dateToTicks( year, month, day ) +
                                       internal::timeToTicks( hour, minute, second, 0 ) + fractionalTicks;

            result = DateTime{ ticks };
            return true;
        }
    } // namespace

    //=====================================================================
    // DateTime class
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    DateTime::DateTime( std::int32_t year, std::int32_t month, std::int32_t day ) noexcept
    {
        if( !internal::isValidDate( year, month, day ) )
        {
            m_ticks = constants::MIN_DATETIME_TICKS;
            return;
        }
        m_ticks = internal::dateToTicks( year, month, day );
    }

    DateTime::DateTime(
        std::int32_t year,
        std::int32_t month,
        std::int32_t day,
        std::int32_t hour,
        std::int32_t minute,
        std::int32_t second ) noexcept
    {
        if( !internal::isValidDate( year, month, day ) || !internal::isValidTime( hour, minute, second, 0 ) )
        {
            m_ticks = constants::MIN_DATETIME_TICKS;
            return;
        }
        m_ticks = internal::dateToTicks( year, month, day ) + internal::timeToTicks( hour, minute, second, 0 );
    }

    DateTime::DateTime(
        std::int32_t year,
        std::int32_t month,
        std::int32_t day,
        std::int32_t hour,
        std::int32_t minute,
        std::int32_t second,
        std::int32_t millisecond ) noexcept
    {
        if( !internal::isValidDate( year, month, day ) || !internal::isValidTime( hour, minute, second, millisecond ) )
        {
            m_ticks = constants::MIN_DATETIME_TICKS;
            return;
        }
        m_ticks =
            internal::dateToTicks( year, month, day ) + internal::timeToTicks( hour, minute, second, millisecond );
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    std::int32_t DateTime::year() const noexcept
    {
        std::int32_t year, month, day;
        internal::dateComponentsFromTicks( m_ticks, year, month, day );

        return year;
    }

    std::int32_t DateTime::month() const noexcept
    {
        std::int32_t year, month, day;
        internal::dateComponentsFromTicks( m_ticks, year, month, day );

        return month;
    }

    std::int32_t DateTime::day() const noexcept
    {
        std::int32_t year, month, day;
        internal::dateComponentsFromTicks( m_ticks, year, month, day );

        return day;
    }

    std::int32_t DateTime::hour() const noexcept
    {
        std::int32_t hour, minute, second, millisecond;
        internal::timeComponentsFromTicks( m_ticks, hour, minute, second, millisecond );

        return hour;
    }

    std::int32_t DateTime::minute() const noexcept
    {
        std::int32_t hour, minute, second, millisecond;
        internal::timeComponentsFromTicks( m_ticks, hour, minute, second, millisecond );

        return minute;
    }

    std::int32_t DateTime::second() const noexcept
    {
        std::int32_t hour, minute, second, millisecond;
        internal::timeComponentsFromTicks( m_ticks, hour, minute, second, millisecond );

        return second;
    }

    std::int32_t DateTime::millisecond() const noexcept
    {
        std::int32_t hour, minute, second, millisecond;
        internal::timeComponentsFromTicks( m_ticks, hour, minute, second, millisecond );

        return millisecond;
    }

    std::int32_t DateTime::microsecond() const noexcept
    {
        const auto remainderTicks{ m_ticks % 10000 };

        return static_cast<std::int32_t>( remainderTicks / 10 );
    }

    std::int32_t DateTime::nanosecond() const noexcept
    {
        const auto remainderTicks{ m_ticks % 10 };

        return static_cast<std::int32_t>( remainderTicks * 100 );
    }

    std::int32_t DateTime::dayOfWeek() const noexcept
    {
        // January 1, 0001 was a Monday (day 1), so we need to adjust
        std::int64_t days{ m_ticks / constants::TICKS_PER_DAY };

        // 0=Sunday, 6=Saturday
        return static_cast<std::int32_t>( ( days + 1 ) % 7 );
    }

    std::int32_t DateTime::dayOfYear() const noexcept
    {
        std::int32_t year, month, day;
        internal::dateComponentsFromTicks( m_ticks, year, month, day );

        std::int32_t dayCount{ 0 };
        for( std::int32_t m{ 1 }; m < month; ++m )
        {
            dayCount += daysInMonth( year, m );
        }

        return dayCount + day;
    }

    //----------------------------------------------
    // Conversion methods
    //----------------------------------------------

    DateTime DateTime::date() const noexcept
    {
        std::int64_t dayTicks{ ( m_ticks / constants::TICKS_PER_DAY ) * constants::TICKS_PER_DAY };

        return DateTime{ dayTicks };
    }

    TimeSpan DateTime::timeOfDay() const noexcept
    {
        std::int64_t timeTicks{ m_ticks % constants::TICKS_PER_DAY };

        return TimeSpan{ timeTicks };
    }

    //----------------------------------------------
    // String formatting
    //----------------------------------------------

    namespace
    {
        /** @brief Append two-digit zero-padded integer */
        inline void appendTwoDigits( nfx::string::StringBuilder& sb, std::int32_t value ) noexcept
        {
            sb.append( static_cast<char>( '0' + ( value / 10 ) ) );
            sb.append( static_cast<char>( '0' + ( value % 10 ) ) );
        }

        /** @brief Append four-digit zero-padded integer */
        inline void appendFourDigits( nfx::string::StringBuilder& sb, std::int32_t value ) noexcept
        {
            sb.append( static_cast<char>( '0' + ( value / 1000 ) ) );
            sb.append( static_cast<char>( '0' + ( ( value / 100 ) % 10 ) ) );
            sb.append( static_cast<char>( '0' + ( ( value / 10 ) % 10 ) ) );
            sb.append( static_cast<char>( '0' + ( value % 10 ) ) );
        }

        /** @brief Append ISO 8601 date part: YYYY-MM-DD */
        inline void appendIso8601Date(
            nfx::string::StringBuilder& sb, std::int32_t year, std::int32_t month, std::int32_t day ) noexcept
        {
            appendFourDigits( sb, year );
            sb.append( '-' );
            appendTwoDigits( sb, month );
            sb.append( '-' );
            appendTwoDigits( sb, day );
        }

        /** @brief Append ISO 8601 time part: HH:mm:ss */
        inline void appendIso8601Time(
            nfx::string::StringBuilder& sb, std::int32_t hour, std::int32_t minute, std::int32_t second ) noexcept
        {
            appendTwoDigits( sb, hour );
            sb.append( ':' );
            appendTwoDigits( sb, minute );
            sb.append( ':' );
            appendTwoDigits( sb, second );
        }

        /** @brief Append ISO 8601 datetime part: YYYY-MM-DDTHH:mm:ss */
        inline void appendIso8601DateTime( nfx::string::StringBuilder& sb,
                                           std::int32_t year,
                                           std::int32_t month,
                                           std::int32_t day,
                                           std::int32_t hour,
                                           std::int32_t minute,
                                           std::int32_t second ) noexcept
        {
            appendIso8601Date( sb, year, month, day );
            sb.append( 'T' );
            appendIso8601Time( sb, hour, minute, second );
        }

        /** @brief Append zero-padded fractional seconds with specific precision */
        inline void appendFractionalSeconds( nfx::string::StringBuilder& sb,
                                             std::int32_t fractionalValue,
                                             std::size_t bufferSize ) noexcept
        {
            char fracBuffer[8]; // Max size for 7 digits + decimal point
            fracBuffer[0] = '.';

            const auto ptr = std::to_chars( fracBuffer + 1, fracBuffer + bufferSize, fractionalValue ).ptr;
            const auto fracLen = ptr - fracBuffer;
            const auto paddingNeeded = bufferSize - fracLen;

            // Shift digits right and add leading zeros if needed
            if( paddingNeeded > 0 )
            {
                std::memmove( fracBuffer + 1 + paddingNeeded, fracBuffer + 1, fracLen - 1 );
                std::memset( fracBuffer + 1, '0', paddingNeeded );
            }

            sb.append( std::string_view{ fracBuffer, bufferSize } );
        }

        /** @brief Append fractional seconds with trimmed trailing zeros */
        inline void appendFractionalSecondsTrimmed( nfx::string::StringBuilder& sb, std::int32_t fractionalTicks ) noexcept
        {
            if( fractionalTicks > 0 )
            {
                char fracBuffer[8];
                fracBuffer[0] = '.';
                const auto ptr = std::to_chars( fracBuffer + 1, fracBuffer + 8, fractionalTicks ).ptr;
                auto fracLen = ptr - fracBuffer;
                const auto paddingNeeded = 8 - fracLen;

                if( paddingNeeded > 0 )
                {
                    std::memmove( fracBuffer + 1 + paddingNeeded, fracBuffer + 1, fracLen - 1 );
                    std::memset( fracBuffer + 1, '0', paddingNeeded );
                    fracLen = 8;
                }

                // Trim trailing zeros
                while( fracLen > 2 && fracBuffer[fracLen - 1] == '0' )
                {
                    --fracLen;
                }

                sb.append( std::string_view{ fracBuffer, static_cast<std::size_t>( fracLen ) } );
            }
            else
            {
                sb.append( ".0" );
            }
        }

        /** @brief Format ISO 8601 with UTC indicator: YYYY-MM-DDTHH:mm:ssZ */
        inline void formatIso8601(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s ) noexcept
        {
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            sb.append( 'Z' );
        }

        /** @brief Format ISO 8601 with precise fractional seconds: YYYY-MM-DDTHH:mm:ss.1234567Z */
        inline void formatIso8601Precise(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s, std::int64_t ticks ) noexcept
        {
            const std::int32_t fractionalTicks{ static_cast<std::int32_t>( ticks % constants::TICKS_PER_SECOND ) };
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            appendFractionalSeconds( sb, fractionalTicks, 8 );
            sb.append( 'Z' );
        }

        /** @brief Format ISO 8601 with trimmed fractional seconds: YYYY-MM-DDTHH:mm:ss.f+Z */
        inline void formatIso8601PreciseTrimmed(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s, std::int64_t ticks ) noexcept
        {
            const std::int32_t fractionalTicks{ static_cast<std::int32_t>( ticks % constants::TICKS_PER_SECOND ) };
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            appendFractionalSecondsTrimmed( sb, fractionalTicks );
            sb.append( 'Z' );
        }

        /** @brief Format ISO 8601 with milliseconds: YYYY-MM-DDTHH:mm:ss.123Z */
        inline void formatIso8601Millis(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s, std::int64_t ticks ) noexcept
        {
            const std::int32_t fractionalTicks{ static_cast<std::int32_t>( ticks % constants::TICKS_PER_SECOND ) };
            const std::int32_t milliseconds{ static_cast<std::int32_t>( fractionalTicks / constants::TICKS_PER_MILLISECOND ) };
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            appendFractionalSeconds( sb, milliseconds, 4 );
            sb.append( 'Z' );
        }

        /** @brief Format ISO 8601 with microseconds: YYYY-MM-DDTHH:mm:ss.123456Z */
        inline void formatIso8601Micros(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s, std::int64_t ticks ) noexcept
        {
            const std::int32_t fractionalTicks{ static_cast<std::int32_t>( ticks % constants::TICKS_PER_SECOND ) };
            const std::int32_t microseconds{ static_cast<std::int32_t>( fractionalTicks / constants::TICKS_PER_MICROSECOND ) };
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            appendFractionalSeconds( sb, microseconds, 7 );
            sb.append( 'Z' );
        }

        /** @brief Format ISO 8601 extended with UTC offset: YYYY-MM-DDTHH:mm:ss+00:00 */
        inline void formatIso8601Extended(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s ) noexcept
        {
            appendIso8601DateTime( sb, y, mon, d, h, min, s );
            sb.append( "+00:00" );
        }

        /** @brief Format ISO 8601 basic (compact): YYYYMMDDTHHMMSSZ */
        inline void formatIso8601Basic(
            nfx::string::StringBuilder& sb, std::int32_t y, std::int32_t mon, std::int32_t d, std::int32_t h, std::int32_t min, std::int32_t s ) noexcept
        {
            appendFourDigits( sb, y );
            appendTwoDigits( sb, mon );
            appendTwoDigits( sb, d );
            sb.append( 'T' );
            appendTwoDigits( sb, h );
            appendTwoDigits( sb, min );
            appendTwoDigits( sb, s );
            sb.append( 'Z' );
        }
    } // namespace

    std::string DateTime::toString( Format format ) const
    {
        std::int32_t y, mon, d, h, min, s, ms;
        internal::dateComponentsFromTicks( m_ticks, y, mon, d );
        internal::timeComponentsFromTicks( m_ticks, h, min, s, ms );

        nfx::string::StringBuilder sb;

        switch( format )
        {
            case Format::Iso8601:
                formatIso8601( sb, y, mon, d, h, min, s );
                break;

            case Format::Iso8601Precise:
                formatIso8601Precise( sb, y, mon, d, h, min, s, m_ticks );
                break;

            case Format::Iso8601PreciseTrimmed:
                formatIso8601PreciseTrimmed( sb, y, mon, d, h, min, s, m_ticks );
                break;

            case Format::Iso8601Millis:
                formatIso8601Millis( sb, y, mon, d, h, min, s, m_ticks );
                break;

            case Format::Iso8601Micros:
                formatIso8601Micros( sb, y, mon, d, h, min, s, m_ticks );
                break;

            case Format::Iso8601Extended:
                formatIso8601Extended( sb, y, mon, d, h, min, s );
                break;

            case Format::Iso8601Basic:
                formatIso8601Basic( sb, y, mon, d, h, min, s );
                break;

            case Format::Iso8601Date:
                appendIso8601Date( sb, y, mon, d );
                break;

            case Format::Iso8601Time:
                appendIso8601Time( sb, h, min, s );
                break;

            case Format::UnixSeconds:
            {
                char buffer[32];
                const auto epochSecs = toEpochSeconds();
                const auto ptr = std::to_chars( buffer, buffer + 32, epochSecs ).ptr;
                sb.append( std::string_view{ buffer, static_cast<std::size_t>( ptr - buffer ) } );
                break;
            }

            case Format::UnixMilliseconds:
            {
                char buffer[32];
                const auto epochMs = toEpochMilliseconds();
                const auto ptr = std::to_chars( buffer, buffer + 32, epochMs ).ptr;
                sb.append( std::string_view{ buffer, static_cast<std::size_t>( ptr - buffer ) } );
                break;
            }

            default:
                return toString( Format::Iso8601 );
        }

        return sb.toString();
    }

    //----------------------------------------------
    // Validation methods
    //----------------------------------------------

    bool DateTime::isValid() const noexcept
    {
        return m_ticks >= constants::MIN_DATETIME_TICKS && m_ticks <= constants::MAX_DATETIME_TICKS;
    }

    //----------------------------------------------
    // Static factory methods
    //----------------------------------------------

    DateTime DateTime::now() noexcept
    {
        const auto utcNow{ DateTime::utcNow() };
        const auto localOffset{ internal::systemTimezoneOffset( utcNow ) };

        return utcNow + localOffset;
    }

    DateTime DateTime::utcNow() noexcept
    {
        return DateTime{ std::chrono::system_clock::now() };
    }

    DateTime DateTime::today() noexcept
    {
        return now().date();
    }

    bool DateTime::fromString( std::string_view iso8601String, DateTime& result ) noexcept
    {
        // Fast empty/length check
        if( iso8601String.empty() || iso8601String.length() < 10 )
        {
            return false;
        }

        // Try fast-path parser first (handles 95% of real-world cases)
        // Supports: YYYY-MM-DD, YYYY-MM-DDTHH:mm:ss, YYYY-MM-DDTHH:mm:ssZ,
        //           YYYY-MM-DDTHH:mm:ss.f, YYYY-MM-DDTHH:mm:ss.fffffffZ
        if( tryParseFastPath( iso8601String, result ) )
        {
            return true;
        }

        // Fallback to flexible parser for non-standard formats
        // (handles timezone offsets, variable digit counts, etc.)

        // Remove trailing 'Z' if present (for flexible parser compatibility)
        if( iso8601String.back() == 'Z' )
        {
            iso8601String.remove_suffix( 1 );
        }

        // Remove timezone offset for DateTime parsing
        auto tzPos{ iso8601String.find_last_of( "+-" ) };

        // Ensure it's not in date part (after position 10 = "YYYY-MM-DD")
        if( tzPos != std::string_view::npos && tzPos > 10 )
        {
            iso8601String = iso8601String.substr( 0, tzPos );
        }

        const char* data = iso8601String.data();
        const char* end = data + iso8601String.size();

        // Parse year (YYYY)
        if( iso8601String.size() < 4 )
        {
            return false;
        }

        std::int32_t year{ 0 };
        auto [ptr1, ec1] = std::from_chars( data, data + 4, year );
        if( ec1 != std::errc{} || ptr1 != data + 4 )
        {
            return false;
        }

        // Expect '-'
        if( ptr1 >= end || *ptr1 != '-' )
        {
            return false;
        }
        ++ptr1; // Skip '-'

        // Parse month (MM or M)
        std::int32_t month{ 0 };
        auto dashPos = iso8601String.find( '-', 5 ); // Find second dash after "YYYY-"
        if( dashPos == std::string_view::npos )
        {
            return false;
        }

        auto [ptr2, ec2] = std::from_chars( ptr1, data + dashPos, month );
        if( ec2 != std::errc{} )
        {
            return false;
        }

        // Expect '-'
        if( ptr2 >= end || *ptr2 != '-' )
        {
            return false;
        }
        ++ptr2; // Skip '-'

        // Parse day (DD or D)
        std::int32_t day{ 0 };
        const char* dayEnd = ptr2;
        while( dayEnd < end && *dayEnd >= '0' && *dayEnd <= '9' )
        {
            ++dayEnd;
        }

        auto [ptr3, ec3] = std::from_chars( ptr2, dayEnd, day );
        if( ec3 != std::errc{} )
        {
            return false;
        }

        // Time part is optional
        std::int32_t hour{ 0 }, minute{ 0 }, second{ 0 };
        std::int32_t fractionalTicks{ 0 };

        if( ptr3 < end && *ptr3 == 'T' )
        {
            ++ptr3; // Skip 'T'

            // Parse hour (HH or H)
            const char* hourEnd = ptr3;
            while( hourEnd < end && *hourEnd >= '0' && *hourEnd <= '9' )
            {
                ++hourEnd;
            }

            auto [ptr4, ec4] = std::from_chars( ptr3, hourEnd, hour );
            if( ec4 != std::errc{} )
            {
                return false;
            }

            // Expect ':'
            if( ptr4 >= end || *ptr4 != ':' )
            {
                return false;
            }
            ++ptr4; // Skip ':'

            // Parse minute (MM or M)
            const char* minEnd = ptr4;
            while( minEnd < end && *minEnd >= '0' && *minEnd <= '9' )
            {
                ++minEnd;
            }

            auto [ptr5, ec5] = std::from_chars( ptr4, minEnd, minute );
            if( ec5 != std::errc{} )
            {
                return false;
            }

            // Expect ':'
            if( ptr5 >= end || *ptr5 != ':' )
            {
                return false;
            }
            ++ptr5; // Skip ':'

            // Parse second (SS or S)
            const char* secEnd = ptr5;
            while( secEnd < end && *secEnd >= '0' && *secEnd <= '9' )
            {
                ++secEnd;
            }

            auto [ptr6, ec6] = std::from_chars( ptr5, secEnd, second );
            if( ec6 != std::errc{} )
            {
                return false;
            }

            // Parse fractional seconds if present
            if( ptr6 < end && *ptr6 == '.' )
            {
                ++ptr6; // Skip '.'

                // Count fractional digits (max 7 for 100ns precision)
                const char* fracStart = ptr6;
                const char* fracEnd = fracStart;
                std::int32_t fractionDigits{ 0 };

                while( fracEnd < end && *fracEnd >= '0' && *fracEnd <= '9' && fractionDigits < 7 )
                {
                    ++fracEnd;
                    ++fractionDigits;
                }

                if( fractionDigits > 0 )
                {
                    std::int32_t fractionValue{ 0 };
                    auto [ptrF, ecF] = std::from_chars( fracStart, fracEnd, fractionValue );
                    if( ecF != std::errc{} || ptrF != fracEnd )
                    {
                        return false;
                    }

                    // Pad to 7 digits (convert to 100ns ticks)
                    while( fractionDigits < 7 )
                    {
                        fractionValue *= 10;
                        ++fractionDigits;
                    }

                    fractionalTicks = fractionValue;
                }
            }
        }

        // Validate components
        if( !internal::isValidDate( year, month, day ) || !internal::isValidTime( hour, minute, second, 0 ) )
        {
            return false;
        }

        // Calculate ticks
        std::int64_t ticks{ internal::dateToTicks( year, month, day ) +
                            internal::timeToTicks( hour, minute, second, 0 ) + fractionalTicks };

        result = DateTime{ ticks };

        return true;
    }

    std::optional<DateTime> DateTime::fromString( std::string_view iso8601String ) noexcept
    {
        DateTime result;
        if( fromString( iso8601String, result ) )
        {
            return result;
        }
        return std::nullopt;
    }

    //----------------------------------------------
    // std::chrono interoperability
    //----------------------------------------------

    std::chrono::system_clock::time_point DateTime::toChrono() const noexcept
    {
        // Clamp to chrono-safe range
        std::int64_t safeTicks{ m_ticks };
        if( safeTicks < internal::MIN_CHRONO_SAFE_TICKS )
        {
            safeTicks = internal::MIN_CHRONO_SAFE_TICKS;
        }
        else if( safeTicks > internal::MAX_CHRONO_SAFE_TICKS )
        {
            safeTicks = internal::MAX_CHRONO_SAFE_TICKS;
        }

        // Calculate duration since Unix epoch in 100-nanosecond ticks
        std::int64_t ticksSinceEpoch{ safeTicks - constants::UNIX_EPOCH_TICKS };

        // Convert to std::chrono duration (100ns precision)
        using ticks_duration = std::chrono::duration<std::int64_t, std::ratio<1, 10000000>>;
        auto duration{ ticks_duration{ ticksSinceEpoch } };

        return std::chrono::system_clock::time_point{ duration };
    }

    DateTime DateTime::fromChrono( const std::chrono::system_clock::time_point& timePoint ) noexcept
    {
        return DateTime{ timePoint };
    }

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const DateTime& dateTime )
    {
        os << dateTime.toString( nfx::time::DateTime::Format::Iso8601 );

        return os;
    }

    std::istream& operator>>( std::istream& is, DateTime& dateTime )
    {
        std::string str;
        is >> str;
        if( !DateTime::fromString( str, dateTime ) )
        {
            is.setstate( std::ios::failbit );
        }

        return is;
    }
} // namespace nfx::time
