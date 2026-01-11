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
 * @file DateTimeOffset.inl
 * @brief Inline implementations for DateTimeOffset class performance-critical methods
 * @details Contains inline implementations of constructors, operators, and accessor methods
 *          for timezone-aware datetime operations. These are separated from the main header
 *          to improve compilation times while maintaining zero-cost abstractions and optimal
 *          performance for UTC conversion and comparison operations.
 */

#include <stdexcept>

#include "Constants.h"

namespace nfx::time
{
    //=====================================================================
    // DateTimeOffset class
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    inline constexpr DateTimeOffset::DateTimeOffset() noexcept
        : m_dateTime{ DateTime::min() },
          m_offset{ 0 }
    {
    }

    inline constexpr DateTimeOffset::DateTimeOffset( const DateTime& dateTime, const TimeSpan& offset ) noexcept
        : m_dateTime{ dateTime },
          m_offset{ offset }
    {
    }

    inline constexpr DateTimeOffset::DateTimeOffset( std::int64_t ticks, const TimeSpan& offset ) noexcept
        : m_dateTime{ ticks },
          m_offset{ offset }
    {
    }

    inline DateTimeOffset::DateTimeOffset( std::int32_t year, std::int32_t month, std::int32_t day, const TimeSpan& offset ) noexcept
        : m_dateTime{ year, month, day },
          m_offset{ offset }
    {
    }

    inline DateTimeOffset::DateTimeOffset( std::int32_t year, std::int32_t month, std::int32_t day,
        std::int32_t hour, std::int32_t minute, std::int32_t second, const TimeSpan& offset ) noexcept
        : m_dateTime{ year, month, day, hour, minute, second },
          m_offset{ offset }
    {
    }

    inline DateTimeOffset::DateTimeOffset( std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, std::int32_t second,
        std::int32_t millisecond, const TimeSpan& offset ) noexcept
        : m_dateTime{ year, month, day, hour, minute, second, millisecond },
          m_offset{ offset }
    {
    }

    inline DateTimeOffset::DateTimeOffset( std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, std::int32_t second,
        std::int32_t millisecond, std::int32_t microsecond, const TimeSpan& offset ) noexcept
        : m_dateTime{ year, month, day, hour, minute, second, millisecond },
          m_offset{ offset }
    {
        // Add microseconds to the DateTime (1 microsecond = 10 ticks)
        m_dateTime += TimeSpan{ microsecond * 10 };
    }

    inline DateTimeOffset::DateTimeOffset( std::string_view iso8601String )
        : m_dateTime{ DateTime::min() },
          m_offset{ 0 }
    {
        if ( !fromString( iso8601String, *this ) )
        {
            throw std::invalid_argument{ "Invalid ISO 8601 DateTimeOffset string format" };
        }
    }

    inline DateTimeOffset::DateTimeOffset( const char* iso8601String )
        : DateTimeOffset{ std::string_view{ iso8601String } }
    {
    }

    inline DateTimeOffset::DateTimeOffset( std::initializer_list<const char*> list )
        : DateTimeOffset{ list.size() > 0 ? std::string_view{ *list.begin() } : std::string_view{} }
    {
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    inline constexpr std::strong_ordering DateTimeOffset::operator<=>( const DateTimeOffset& other ) const noexcept
    {
        // Compare UTC times
        const auto thisUtc = utcTicks();
        const auto otherUtc = other.utcTicks();

        if ( thisUtc < otherUtc )
        {
            return std::strong_ordering::less;
        }
        if ( thisUtc > otherUtc )
        {
            return std::strong_ordering::greater;
        }

        return std::strong_ordering::equal;
    }

    inline constexpr bool DateTimeOffset::operator==( const DateTimeOffset& other ) const noexcept
    {
        // Compare UTC times
        return utcTicks() == other.utcTicks();
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    inline constexpr DateTimeOffset DateTimeOffset::operator+( const TimeSpan& duration ) const noexcept
    {
        return DateTimeOffset{ m_dateTime + duration, m_offset };
    }

    inline constexpr DateTimeOffset DateTimeOffset::operator-( const TimeSpan& duration ) const noexcept
    {
        return DateTimeOffset{ m_dateTime - duration, m_offset };
    }

    inline constexpr TimeSpan DateTimeOffset::operator-( const DateTimeOffset& other ) const noexcept
    {
        return TimeSpan{ utcTicks() - other.utcTicks() };
    }

    inline constexpr DateTimeOffset& DateTimeOffset::operator+=( const TimeSpan& duration ) noexcept
    {
        m_dateTime += duration;

        return *this;
    }

    inline constexpr DateTimeOffset& DateTimeOffset::operator-=( const TimeSpan& duration ) noexcept
    {
        m_dateTime -= duration;

        return *this;
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    inline constexpr const DateTime& DateTimeOffset::dateTime() const noexcept
    {
        return m_dateTime;
    }

    inline constexpr const TimeSpan& DateTimeOffset::offset() const noexcept
    {
        return m_offset;
    }

    inline constexpr std::int64_t DateTimeOffset::ticks() const noexcept
    {
        return m_dateTime.ticks();
    }

    inline constexpr std::int64_t DateTimeOffset::utcTicks() const noexcept
    {
        return m_dateTime.ticks() - m_offset.ticks();
    }

    inline std::int32_t DateTimeOffset::year() const noexcept
    {
        return m_dateTime.year();
    }

    inline std::int32_t DateTimeOffset::month() const noexcept
    {
        return m_dateTime.month();
    }

    inline std::int32_t DateTimeOffset::day() const noexcept
    {
        return m_dateTime.day();
    }

    inline std::int32_t DateTimeOffset::hour() const noexcept
    {
        return m_dateTime.hour();
    }

    inline std::int32_t DateTimeOffset::minute() const noexcept
    {
        return m_dateTime.minute();
    }

    inline std::int32_t DateTimeOffset::second() const noexcept
    {
        return m_dateTime.second();
    }

    inline std::int32_t DateTimeOffset::millisecond() const noexcept
    {
        return m_dateTime.millisecond();
    }

    inline std::int32_t DateTimeOffset::microsecond() const noexcept
    {
        // Extract microseconds from ticks (1 microsecond = 10 ticks)
        const auto remainderTicks{ m_dateTime.ticks() % 10000 };

        return static_cast<std::int32_t>( remainderTicks / 10 );
    }

    inline std::int32_t DateTimeOffset::nanosecond() const noexcept
    {
        // Extract nanosecond component (in hundreds of nanoseconds since ticks are 100ns units)
        const auto remainderTicks{ m_dateTime.ticks() % 10 };

        return static_cast<std::int32_t>( remainderTicks * 100 );
    }

    inline std::int32_t DateTimeOffset::dayOfWeek() const noexcept
    {
        return m_dateTime.dayOfWeek();
    }

    inline std::int32_t DateTimeOffset::dayOfYear() const noexcept
    {
        return m_dateTime.dayOfYear();
    }

    inline std::int32_t DateTimeOffset::totalOffsetMinutes() const noexcept
    {
        return static_cast<std::int32_t>( m_offset.minutes() );
    }

    //----------------------------------------------
    // Conversion methods
    //----------------------------------------------

    inline std::int64_t DateTimeOffset::toEpochSeconds() const noexcept
    {
        return utcDateTime().toEpochSeconds();
    }

    inline std::int64_t DateTimeOffset::toEpochMilliseconds() const noexcept
    {
        return utcDateTime().toEpochMilliseconds();
    }

    inline TimeSpan DateTimeOffset::timeOfDay() const noexcept
    {
        return m_dateTime.timeOfDay();
    }

    //----------------------------------------------
    // Arithmetic methods
    //----------------------------------------------

    inline DateTimeOffset DateTimeOffset::add( const TimeSpan& value ) const noexcept
    {
        return *this + value;
    }

    inline TimeSpan DateTimeOffset::subtract( const DateTimeOffset& value ) const noexcept
    {
        return *this - value;
    }

    inline DateTimeOffset DateTimeOffset::subtract( const TimeSpan& value ) const noexcept
    {
        return *this - value;
    }

    inline DateTimeOffset DateTimeOffset::addTicks( std::int64_t ticks ) const noexcept
    {
        return *this + TimeSpan{ ticks };
    }

    inline bool DateTimeOffset::equals( const DateTimeOffset& other ) const noexcept
    {
        return *this == other;
    }

    inline bool DateTimeOffset::equalsExact( const DateTimeOffset& other ) const noexcept
    {
        return m_dateTime == other.m_dateTime && m_offset == other.m_offset;
    }

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const DateTimeOffset& dateTimeOffset );

    std::istream& operator>>( std::istream& is, DateTimeOffset& dateTimeOffset );
} // namespace nfx::time

//=====================================================================
// std::formatter specialization
//=====================================================================

namespace std
{
    template <>
    struct formatter<nfx::time::DateTimeOffset>
    {
        constexpr auto parse( std::format_parse_context& ctx )
        {
            return ctx.begin();
        }

        auto format( const nfx::time::DateTimeOffset& dto, std::format_context& ctx ) const
        {
            return format_to( ctx.out(), "{}", dto.toString() );
        }
    };
} // namespace std
