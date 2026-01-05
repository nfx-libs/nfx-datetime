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
 * @file TimeSpan.inl
 * @brief Inline implementations for TimeSpan class performance-critical methods
 * @details Contains inline implementations of constructors, operators, and accessor methods
 *          for time duration operations. These are separated from the main header to improve
 *          compilation times while maintaining zero-cost abstractions and optimal performance
 *          for arithmetic operations and time unit conversions.
 */

#include <cmath>
#include <stdexcept>

#include "Constants.h"

namespace nfx::time
{
    //=====================================================================
    // TimeSpan class
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    inline constexpr TimeSpan::TimeSpan( std::int64_t ticks ) noexcept
        : m_ticks{ ticks }
    {
    }

    inline constexpr TimeSpan::TimeSpan( std::chrono::system_clock::duration duration ) noexcept
        : m_ticks{ std::chrono::duration_cast<std::chrono::nanoseconds>( duration ).count() / 100 }
    {
    }

    inline TimeSpan::TimeSpan( std::string_view iso8601String )
    {
        TimeSpan result;
        if ( !fromString( iso8601String, result ) )
        {
            throw std::invalid_argument{ "Invalid ISO 8601 duration string format" };
        }
        m_ticks = result.m_ticks;
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    inline constexpr std::strong_ordering TimeSpan::operator<=>( const TimeSpan& other ) const noexcept
    {
        if ( m_ticks < other.m_ticks )
        {
            return std::strong_ordering::less;
        }
        if ( m_ticks > other.m_ticks )
        {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::equal;
    }

    inline constexpr bool TimeSpan::operator==( const TimeSpan& other ) const noexcept
    {
        return m_ticks == other.m_ticks;
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    inline constexpr TimeSpan TimeSpan::operator+( const TimeSpan& other ) const noexcept
    {
        return TimeSpan{ m_ticks + other.m_ticks };
    }

    inline constexpr TimeSpan TimeSpan::operator-( const TimeSpan& other ) const noexcept
    {
        return TimeSpan{ m_ticks - other.m_ticks };
    }

    inline constexpr TimeSpan TimeSpan::operator-() const noexcept
    {
        return TimeSpan{ -m_ticks };
    }

    inline constexpr TimeSpan& TimeSpan::operator+=( const TimeSpan& other ) noexcept
    {
        m_ticks += other.m_ticks;

        return *this;
    }

    inline constexpr TimeSpan& TimeSpan::operator-=( const TimeSpan& other ) noexcept
    {
        m_ticks -= other.m_ticks;

        return *this;
    }

    inline TimeSpan TimeSpan::operator*( double multiplier ) const noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( std::round( static_cast<double>( m_ticks ) * multiplier ) ) };
    }

    inline TimeSpan TimeSpan::operator/( double divisor ) const noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( std::round( static_cast<double>( m_ticks ) / divisor ) ) };
    }

    inline double TimeSpan::operator/( const TimeSpan& other ) const noexcept
    {
        return static_cast<double>( m_ticks ) / static_cast<double>( other.m_ticks );
    }

    inline TimeSpan operator*( double multiplier, const TimeSpan& ts ) noexcept
    {
        return ts * multiplier;
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    inline double TimeSpan::days() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_DAY;
    }

    inline double TimeSpan::hours() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_HOUR;
    }

    inline double TimeSpan::minutes() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_MINUTE;
    }

    inline double TimeSpan::seconds() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_SECOND;
    }

    inline double TimeSpan::milliseconds() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_MILLISECOND;
    }

    inline double TimeSpan::microseconds() const noexcept
    {
        return static_cast<double>( m_ticks ) / constants::TICKS_PER_MICROSECOND;
    }

    inline double TimeSpan::nanoseconds() const noexcept
    {
        return static_cast<double>( m_ticks ) * constants::NANOSECONDS_PER_TICK;
    }

    inline constexpr std::int64_t TimeSpan::ticks() const noexcept
    {
        return m_ticks;
    }

    //----------------------------------------------
    // Static factory methods
    //----------------------------------------------

    inline constexpr TimeSpan TimeSpan::fromDays( double days ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( days * constants::TICKS_PER_DAY ) };
    }

    inline constexpr TimeSpan TimeSpan::fromHours( double hours ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( hours * constants::TICKS_PER_HOUR ) };
    }

    inline constexpr TimeSpan TimeSpan::fromMinutes( double minutes ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( minutes * constants::TICKS_PER_MINUTE ) };
    }

    inline constexpr TimeSpan TimeSpan::fromSeconds( double seconds ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( seconds * constants::TICKS_PER_SECOND ) };
    }

    inline constexpr TimeSpan TimeSpan::fromMilliseconds( double milliseconds ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( milliseconds * constants::TICKS_PER_MILLISECOND ) };
    }

    inline constexpr TimeSpan TimeSpan::fromMicroseconds( double microseconds ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( std::round( microseconds * constants::TICKS_PER_MICROSECOND ) ) };
    }

    inline constexpr TimeSpan TimeSpan::fromTicks( double ticks ) noexcept
    {
        return TimeSpan{ static_cast<std::int64_t>( ticks ) };
    }

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const TimeSpan& timeSpan );

    std::istream& operator>>( std::istream& is, TimeSpan& timeSpan );

    //=====================================================================
    // User-defined literals
    //=====================================================================

    namespace literals
    {
        inline constexpr TimeSpan operator""_d( long double days ) noexcept
        {
            return TimeSpan::fromDays( static_cast<double>( days ) );
        }

        inline constexpr TimeSpan operator""_d( unsigned long long days ) noexcept
        {
            return TimeSpan::fromDays( static_cast<double>( days ) );
        }

        inline constexpr TimeSpan operator""_h( long double hours ) noexcept
        {
            return TimeSpan::fromHours( static_cast<double>( hours ) );
        }

        inline constexpr TimeSpan operator""_h( unsigned long long hours ) noexcept
        {
            return TimeSpan::fromHours( static_cast<double>( hours ) );
        }

        inline constexpr TimeSpan operator""_min( long double minutes ) noexcept
        {
            return TimeSpan::fromMinutes( static_cast<double>( minutes ) );
        }

        inline constexpr TimeSpan operator""_min( unsigned long long minutes ) noexcept
        {
            return TimeSpan::fromMinutes( static_cast<double>( minutes ) );
        }

        inline constexpr TimeSpan operator""_s( long double seconds ) noexcept
        {
            return TimeSpan::fromSeconds( static_cast<double>( seconds ) );
        }

        inline constexpr TimeSpan operator""_s( unsigned long long seconds ) noexcept
        {
            return TimeSpan::fromSeconds( static_cast<double>( seconds ) );
        }

        inline constexpr TimeSpan operator""_ms( long double milliseconds ) noexcept
        {
            return TimeSpan::fromMilliseconds( static_cast<double>( milliseconds ) );
        }

        inline constexpr TimeSpan operator""_ms( unsigned long long milliseconds ) noexcept
        {
            return TimeSpan::fromMilliseconds( static_cast<double>( milliseconds ) );
        }

        inline constexpr TimeSpan operator""_us( long double microseconds ) noexcept
        {
            return TimeSpan::fromMicroseconds( static_cast<double>( microseconds ) );
        }

        inline constexpr TimeSpan operator""_us( unsigned long long microseconds ) noexcept
        {
            return TimeSpan::fromMicroseconds( static_cast<double>( microseconds ) );
        }

        inline constexpr TimeSpan operator""_ns( long double nanoseconds ) noexcept
        {
            return TimeSpan::fromTicks( std::round( static_cast<double>( nanoseconds ) / 100.0 ) );
        }

        inline constexpr TimeSpan operator""_ns( unsigned long long nanoseconds ) noexcept
        {
            return TimeSpan::fromTicks( std::round( static_cast<double>( nanoseconds ) / 100.0 ) );
        }
    } // namespace literals
} // namespace nfx::time

//=====================================================================
// std::formatter specialization
//=====================================================================

namespace std
{
    template <>
    struct formatter<nfx::time::TimeSpan>
    {
        constexpr auto parse( std::format_parse_context& ctx )
        {
            return ctx.begin();
        }

        auto format( const nfx::time::TimeSpan& ts, std::format_context& ctx ) const
        {
            return format_to( ctx.out(), "{}", ts.toString() );
        }
    };
} // namespace std
