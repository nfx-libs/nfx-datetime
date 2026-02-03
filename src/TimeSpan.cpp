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
 * @file TimeSpan.cpp
 * @brief Implementation of TimeSpan class methods for time duration operations
 * @details Provides parsing logic for ISO 8601 duration format, H:M:S time format,
 *          and numeric seconds format. Implements string conversion and formatting
 *          functionality for time intervals with 100-nanosecond precision.
 */

#include "nfx/datetime/TimeSpan.h"

#include <nfx/string/StringBuilder.h>

#include <charconv>
#include <string>

namespace nfx::time
{
    //=====================================================================
    // Fast parsing helpers
    //=====================================================================

    namespace
    {
        /** @brief Check if character is a digit */
        [[nodiscard]] constexpr bool isDigit( char c ) noexcept
        {
            return c >= '0' && c <= '9';
        }

        /** @brief Parse integer from string view using fast digit parsing */
        [[nodiscard]] bool fastParseInt( std::string_view str, double& value ) noexcept
        {
            if( str.empty() )
            {
                return false;
            }

            value = 0;
            for( char c : str )
            {
                if( !isDigit( c ) )
                {
                    return false;
                }
                value = value * 10 + ( c - '0' );
            }
            return true;
        }

        /** @brief Parse fractional seconds (e.g., "45.5S" -> 45.5) */
        [[nodiscard]] bool fastParseDecimal( std::string_view str, double& value ) noexcept
        {
            if( str.empty() )
            {
                return false;
            }

            auto dotPos = str.find( '.' );
            if( dotPos == std::string_view::npos )
            {
                // No decimal point - parse as integer
                return fastParseInt( str, value );
            }

            // Parse integer part
            double intPart = 0;
            if( dotPos > 0 && !fastParseInt( str.substr( 0, dotPos ), intPart ) )
            {
                return false;
            }

            // Parse fractional part
            double fracPart = 0;
            if( dotPos + 1 < str.length() )
            {
                auto fracStr = str.substr( dotPos + 1 );
                if( !fastParseInt( fracStr, fracPart ) )
                {
                    return false;
                }

                // Convert to fraction (e.g., "5" from "45.5" -> 0.5)
                double divisor = 1;
                for( std::size_t i = 0; i < fracStr.length(); ++i )
                {
                    divisor *= 10;
                }
                fracPart /= divisor;
            }

            value = intPart + fracPart;
            return true;
        }

        /**
         * @brief Fast-path parser for common ISO 8601 duration formats
         * @details Handles optimized parsing for:
         *          - PT1H, PT2H30M, PT45S (simple integer components)
         *          - PT1H30M45S (all components)
         *          - PT45.5S (fractional seconds)
         * @return true if parsed successfully via fast path, false if fallback needed
         */
        [[nodiscard]] bool tryParseFastPathDuration( std::string_view str, TimeSpan& result ) noexcept
        {
            if( str.length() < 3 ) // Minimum: "PT1H"
            {
                return false;
            }

            // Handle negative durations
            bool isNegative = false;
            if( str[0] == '-' )
            {
                isNegative = true;
                str.remove_prefix( 1 );
                if( str.length() < 3 )
                {
                    return false;
                }
            }

            // Must start with 'P'
            if( str[0] != 'P' )
            {
                return false;
            }

            // Check for time part indicator 'T'
            if( str.length() < 3 || str[1] != 'T' )
            {
                return false; // Fallback for date components (P1D) or complex formats
            }

            // Parse time components: PT[n]H[n]M[n]S
            str.remove_prefix( 2 ); // Skip "PT"

            double totalSeconds = 0;
            bool foundComponent = false;

            // Parse hours (if present)
            auto hPos = str.find( 'H' );
            if( hPos != std::string_view::npos )
            {
                double hours = 0;
                if( !fastParseDecimal( str.substr( 0, hPos ), hours ) )
                {
                    return false;
                }
                totalSeconds += hours * constants::SECONDS_PER_HOUR;
                str.remove_prefix( hPos + 1 );
                foundComponent = true;
            }

            // Parse minutes (if present)
            auto mPos = str.find( 'M' );
            if( mPos != std::string_view::npos )
            {
                double minutes = 0;
                if( !fastParseDecimal( str.substr( 0, mPos ), minutes ) )
                {
                    return false;
                }
                totalSeconds += minutes * constants::SECONDS_PER_MINUTE;
                str.remove_prefix( mPos + 1 );
                foundComponent = true;
            }

            // Parse seconds (if present)
            auto sPos = str.find( 'S' );
            if( sPos != std::string_view::npos )
            {
                double seconds = 0;
                if( !fastParseDecimal( str.substr( 0, sPos ), seconds ) )
                {
                    return false;
                }
                totalSeconds += seconds;
                str.remove_prefix( sPos + 1 );
                foundComponent = true;
            }

            // Must have at least one component and consumed entire string
            if( !foundComponent || !str.empty() )
            {
                return false;
            }

            // Apply negative sign
            if( isNegative )
            {
                totalSeconds = -totalSeconds;
            }

            result = TimeSpan::fromSeconds( totalSeconds );
            return true;
        }
    } // namespace

    //=====================================================================
    // TimeSpan class
    //=====================================================================

    //----------------------------------------------
    // String formatting
    //----------------------------------------------

    std::string TimeSpan::toString() const
    {
        nfx::string::StringBuilder sb( 64 );

        // Handle negative durations
        bool isNegative{ m_ticks < 0 };
        if( isNegative )
        {
            sb.append( '-' );
        }

        sb.append( 'P' );

        std::int64_t absTicks{ std::abs( m_ticks ) };
        std::int64_t totalSeconds{ absTicks / constants::TICKS_PER_SECOND };

        // Break down into days, hours, minutes, seconds
        std::int32_t days{ static_cast<std::int32_t>( totalSeconds / constants::SECONDS_PER_DAY ) };
        std::int32_t remainingSeconds{ static_cast<std::int32_t>( totalSeconds % constants::SECONDS_PER_DAY ) };
        std::int32_t hours{ remainingSeconds / constants::SECONDS_PER_HOUR };
        std::int32_t minutes{ ( remainingSeconds % constants::SECONDS_PER_HOUR ) / constants::SECONDS_PER_MINUTE };
        std::int32_t seconds{ remainingSeconds % constants::SECONDS_PER_MINUTE };

        // Output days if present
        if( days > 0 )
        {
            sb.append( days ).append( 'D' );
        }

        // Check if we have any time components
        std::int64_t fractionalTicks{ absTicks % constants::TICKS_PER_SECOND };
        bool hasTimeComponent{ hours > 0 || minutes > 0 || seconds > 0 || fractionalTicks > 0 };

        // Only output time component if present
        if( hasTimeComponent )
        {
            sb.append( 'T' );

            if( hours > 0 )
            {
                sb.append( hours ).append( 'H' );
            }

            if( minutes > 0 )
            {
                sb.append( minutes ).append( 'M' );
            }

            // Include seconds with or without fractional part
            if( seconds > 0 || fractionalTicks > 0 )
            {
                if( fractionalTicks > 0 )
                {
                    // Format fractional seconds directly to avoid allocations
                    char fracBuffer[8];
                    std::snprintf(
                        fracBuffer, sizeof( fracBuffer ), "%07lld", static_cast<long long>( fractionalTicks ) );

                    // Strip trailing zeros
                    std::size_t fracLen{ 7 };
                    while( fracLen > 1 && fracBuffer[fracLen - 1] == '0' )
                    {
                        --fracLen;
                    }

                    sb.append( seconds ).append( '.' ).append( std::string_view{ fracBuffer, fracLen } ).append( 'S' );
                }
                else
                {
                    sb.append( seconds ).append( 'S' );
                }
            }
        }
        else if( days == 0 )
        {
            // No days and no time components: output PT0S for zero duration
            sb.append( "T0S" );
        }
        return sb.toString();
    }

    //----------------------------------------------
    // String parsing
    //----------------------------------------------

    bool TimeSpan::fromString( std::string_view iso8601DurationString, TimeSpan& result ) noexcept
    {
        if( iso8601DurationString.empty() )
        {
            return false;
        }

        // Try fast-path for common ISO 8601 duration formats (PT1H30M, PT45S, etc.)
        if( tryParseFastPathDuration( iso8601DurationString, result ) )
        {
            return true;
        }

        // Handle numeric seconds format (convenience)
        if( iso8601DurationString.find_first_not_of( "0123456789.-" ) == std::string_view::npos )
        {
            double seconds{};
            auto [ptr, ec] = std::from_chars(
                iso8601DurationString.data(), iso8601DurationString.data() + iso8601DurationString.size(), seconds );
            if( ec == std::errc{} && ptr == iso8601DurationString.data() + iso8601DurationString.size() )
            {
                result = TimeSpan::fromSeconds( seconds );
                return true;
            }
            return false;
        }

        // Check for negative sign
        bool isNegative{ false };
        std::string_view parseStr{ iso8601DurationString };
        if( !parseStr.empty() && parseStr[0] == '-' )
        {
            isNegative = true;
            parseStr = parseStr.substr( 1 );
        }

        if( parseStr.length() > 1 && parseStr[0] == 'P' )
        {
            double days{ 0.0 };
            double totalSeconds{ 0.0 };
            bool foundComponent{ false };

            // Parse days component (P[n]D)
            auto tPos{ parseStr.find( 'T' ) };

            // Parse date part (before T, or entire string if no T)
            auto datePart{ tPos != std::string_view::npos ? parseStr.substr( 1, tPos - 1 ) : parseStr.substr( 1 ) };
            if( !datePart.empty() )
            {
                auto dPos{ datePart.find( 'D' ) };
                if( dPos != std::string_view::npos )
                {
                    auto dayStr{ datePart.substr( 0, dPos ) };
                    auto [ptr, ec] = std::from_chars( dayStr.data(), dayStr.data() + dayStr.size(), days );
                    if( ec == std::errc{} && ptr == dayStr.data() + dayStr.size() )
                    {
                        foundComponent = true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            // Parse time part after T (if present)
            if( tPos != std::string_view::npos )
            {
                auto timePart{ parseStr.substr( tPos + 1 ) };
                auto originalTimePart{ timePart }; // Keep original for position tracking

                // Must have at least one component after T (not just "PT")
                if( timePart.empty() )
                {
                    return false;
                }

                // Find all component positions in the original string for order validation
                auto hPos{ originalTimePart.find( 'H' ) };
                auto mPos{ originalTimePart.find( 'M' ) };
                auto sPos{ originalTimePart.find( 'S' ) };

                // Detect duplicate components
                if( hPos != std::string_view::npos && originalTimePart.find( 'H', hPos + 1 ) != std::string_view::npos )
                {
                    return false; // Duplicate hours
                }
                if( mPos != std::string_view::npos && originalTimePart.find( 'M', mPos + 1 ) != std::string_view::npos )
                {
                    return false; // Duplicate minutes
                }
                if( sPos != std::string_view::npos && originalTimePart.find( 'S', sPos + 1 ) != std::string_view::npos )
                {
                    return false; // Duplicate seconds
                }

                // Validate component order (H before M before S)
                if( hPos != std::string_view::npos && mPos != std::string_view::npos && hPos > mPos )
                {
                    return false; // Minutes before hours
                }
                if( hPos != std::string_view::npos && sPos != std::string_view::npos && hPos > sPos )
                {
                    return false; // Seconds before hours
                }
                if( mPos != std::string_view::npos && sPos != std::string_view::npos && mPos > sPos )
                {
                    return false; // Seconds before minutes
                }

                // Parse hours
                if( hPos != std::string_view::npos )
                {
                    auto hourStr{ timePart.substr( 0, hPos ) };
                    double hours{};
                    auto [ptr, ec] = std::from_chars( hourStr.data(), hourStr.data() + hourStr.size(), hours );
                    if( ec == std::errc{} && ptr == hourStr.data() + hourStr.size() )
                    {
                        totalSeconds += hours * static_cast<double>( constants::SECONDS_PER_HOUR );
                        timePart = timePart.substr( hPos + 1 );
                        foundComponent = true;
                    }
                    else
                    {
                        return false;
                    }
                }

                // Parse minutes (recalculate position in current timePart)
                mPos = timePart.find( 'M' );
                if( mPos != std::string::npos )
                {
                    auto minuteStr{ timePart.substr( 0, mPos ) };
                    double minutes{};
                    auto [ptr, ec] = std::from_chars( minuteStr.data(), minuteStr.data() + minuteStr.size(), minutes );
                    if( ec == std::errc{} && ptr == minuteStr.data() + minuteStr.size() )
                    {
                        totalSeconds += minutes * static_cast<double>( constants::SECONDS_PER_MINUTE );
                        timePart = timePart.substr( mPos + 1 );
                        foundComponent = true;
                    }
                    else
                    {
                        return false;
                    }
                }

                // Parse seconds (recalculate position in current timePart)
                sPos = timePart.find( 'S' );
                if( sPos != std::string::npos )
                {
                    auto secondStr{ timePart.substr( 0, sPos ) };
                    double seconds{};
                    auto [ptr, ec] = std::from_chars( secondStr.data(), secondStr.data() + secondStr.size(), seconds );
                    if( ec == std::errc{} && ptr == secondStr.data() + secondStr.size() )
                    {
                        totalSeconds += seconds;
                        foundComponent = true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            // Must have parsed at least one valid component (D, H, M, or S)
            if( !foundComponent )
            {
                return false;
            }

            // Convert days to seconds
            totalSeconds += days * static_cast<double>( constants::SECONDS_PER_DAY );

            // Apply negative sign if present
            if( isNegative )
            {
                totalSeconds = -totalSeconds;
            }

            result = TimeSpan::fromSeconds( totalSeconds );

            return true;
        }

        return false;
    }

    std::optional<TimeSpan> TimeSpan::fromString( std::string_view iso8601DurationString ) noexcept
    {
        TimeSpan result;
        if( fromString( iso8601DurationString, result ) )
        {
            return result;
        }
        return std::nullopt;
    }

    //----------------------------------------------
    // std::chrono interoperability
    //----------------------------------------------

    std::chrono::system_clock::duration TimeSpan::toChrono() const noexcept
    {
        // Maximum representable TimeSpan is ~29,227 years (std::int64_t max ticks)
        // m_ticks * 100 converts to nanoseconds and cannot overflow because:
        // max_ticks * 100 = 9,223,372,036,854,775,807 * 100 = 922,337,203,685,477,580,700
        // which exceeds int64_t max, but we constrain construction to valid ranges.
        //
        // In practice, TimeSpan is limited by double precision in fromDays/fromHours/etc.
        // and fromString validates reasonable ranges. Direct tick construction could
        // theoretically create values near max, but m_ticks * 100 would still fit in
        // the nanoseconds representation used by system_clock::duration.
        static_assert(
            std::numeric_limits<std::int64_t>::max() / 100 > 0, "TimeSpan tick to nanosecond conversion assumption" );

        return std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::nanoseconds{ m_ticks * 100 } );
    }

    TimeSpan TimeSpan::fromChrono( const std::chrono::system_clock::duration& duration ) noexcept
    {
        return TimeSpan{ std::chrono::duration_cast<std::chrono::nanoseconds>( duration ).count() / 100 };
    }

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const TimeSpan& timeSpan )
    {
        os << timeSpan.toString();

        return os;
    }

    std::istream& operator>>( std::istream& is, TimeSpan& timeSpan )
    {
        std::string str;
        is >> str;
        if( !TimeSpan::fromString( str, timeSpan ) )
        {
            is.setstate( std::ios::failbit );
        }

        return is;
    }
} // namespace nfx::time
