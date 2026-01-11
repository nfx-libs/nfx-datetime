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
 * @file DateTime.inl
 * @brief Inline implementations for DateTime class performance-critical methods
 * @details Contains inline implementations of constructors, operators, and accessor methods
 *          that benefit from inlining for optimal performance. These are separated from the
 *          main header to improve compilation times while maintaining zero-cost abstractions.
 */

#include <stdexcept>

#include "Constants.h"

namespace nfx::time
{
    //=====================================================================
    // DateTime class
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    inline constexpr DateTime::DateTime() noexcept
        : m_ticks{ constants::MIN_DATETIME_TICKS }
    {
    }

    inline constexpr DateTime::DateTime( std::int64_t ticks ) noexcept
        : m_ticks{ ticks }
    {
    }

    inline DateTime::DateTime( std::chrono::system_clock::time_point timePoint ) noexcept
    {
        auto duration{ timePoint.time_since_epoch() };

        using ticks_duration = std::chrono::duration<std::int64_t, std::ratio<1, 10000000>>;
        auto ticks{ std::chrono::duration_cast<ticks_duration>( duration ) };

        m_ticks = constants::UNIX_EPOCH_TICKS + ticks.count();

        // Clamp to valid range
        if ( m_ticks < constants::MIN_DATETIME_TICKS )
        {
            m_ticks = constants::MIN_DATETIME_TICKS;
        }
        else if ( m_ticks > constants::MAX_DATETIME_TICKS )
        {
            m_ticks = constants::MAX_DATETIME_TICKS;
        }
    }

    inline DateTime::DateTime( std::string_view iso8601String )
    {
        DateTime result;
        if ( !fromString( iso8601String, result ) )
        {
            throw std::invalid_argument{ "Invalid ISO 8601 DateTime string" };
        }
        m_ticks = result.m_ticks;
    }

    inline DateTime::DateTime( const char* iso8601String )
        : DateTime{ std::string_view{ iso8601String } }
    {
    }

    inline DateTime::DateTime( std::initializer_list<const char*> list )
        : DateTime{ list.size() > 0 ? std::string_view{ *list.begin() } : std::string_view{} }
    {
    }

    //----------------------------------------------
    // Comparison operators
    //----------------------------------------------

    inline constexpr std::strong_ordering DateTime::operator<=>( const DateTime& other ) const noexcept
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

    inline constexpr bool DateTime::operator==( const DateTime& other ) const noexcept
    {
        return m_ticks == other.m_ticks;
    }

    //----------------------------------------------
    // Arithmetic operators
    //----------------------------------------------

    inline constexpr DateTime DateTime::operator+( const TimeSpan& duration ) const noexcept
    {
        return DateTime{ m_ticks + duration.ticks() };
    }

    inline constexpr DateTime DateTime::operator-( const TimeSpan& duration ) const noexcept
    {
        return DateTime{ m_ticks - duration.ticks() };
    }

    inline constexpr TimeSpan DateTime::operator-( const DateTime& other ) const noexcept
    {
        return TimeSpan{ m_ticks - other.m_ticks };
    }

    inline constexpr DateTime& DateTime::operator+=( const TimeSpan& duration ) noexcept
    {
        m_ticks += duration.ticks();

        return *this;
    }

    inline constexpr DateTime& DateTime::operator-=( const TimeSpan& duration ) noexcept
    {
        m_ticks -= duration.ticks();

        return *this;
    }

    //----------------------------------------------
    // Property accessors
    //----------------------------------------------

    inline constexpr std::int64_t DateTime::ticks() const noexcept
    {
        return m_ticks;
    }

    //----------------------------------------------
    // Conversion methods
    //----------------------------------------------

    inline constexpr std::int64_t DateTime::toEpochSeconds() const noexcept
    {
        return ( m_ticks - constants::UNIX_EPOCH_TICKS ) / constants::TICKS_PER_SECOND;
    }

    inline constexpr std::int64_t DateTime::toEpochMilliseconds() const noexcept
    {
        return ( m_ticks - constants::UNIX_EPOCH_TICKS ) / constants::TICKS_PER_MILLISECOND;
    }

    //----------------------------------------------
    // Validation methods
    //----------------------------------------------

    inline constexpr bool DateTime::isLeapYear( std::int32_t year ) noexcept
    {
        return ( year % 4 == 0 && year % 100 != 0 ) || ( year % 400 == 0 );
    }

    inline constexpr std::int32_t DateTime::daysInMonth( std::int32_t year, std::int32_t month ) noexcept
    {
        if ( month < 1 || month > 12 )
        {
            return 0;
        }

        if ( month == 2 && isLeapYear( year ) )
        {
            return 29;
        }

        switch ( month )
        {
            case 1:
            {
                return 31;
            }
            case 2:
            {
                return 28; // Non-leap year February
            }
            case 3:
            {
                return 31;
            }
            case 4:
            {
                return 30;
            }
            case 5:
            {
                return 31;
            }
            case 6:
            {
                return 30;
            }
            case 7:
            {
                return 31;
            }
            case 8:
            {
                return 31;
            }
            case 9:
            {
                return 30;
            }
            case 10:
            {
                return 31;
            }
            case 11:
            {
                return 30;
            }
            case 12:
            {
                return 31;
            }
            default:
            {
                return 0;
            }
        }
    }

    //----------------------------------------------
    // Static factory methods
    //----------------------------------------------

    inline constexpr DateTime DateTime::min() noexcept
    {
        return DateTime{ constants::MIN_DATETIME_TICKS };
    }

    inline constexpr DateTime DateTime::max() noexcept
    {
        return DateTime{ constants::MAX_DATETIME_TICKS };
    }

    inline constexpr DateTime DateTime::epoch() noexcept
    {
        return DateTime{ constants::UNIX_EPOCH_TICKS };
    }

    inline constexpr DateTime DateTime::fromEpochSeconds( std::int64_t seconds ) noexcept
    {
        std::int64_t ticks{ constants::UNIX_EPOCH_TICKS + ( seconds * constants::TICKS_PER_SECOND ) };

        return DateTime{ ticks };
    }

    inline constexpr DateTime DateTime::fromEpochMilliseconds( std::int64_t milliseconds ) noexcept
    {
        std::int64_t ticks{ constants::UNIX_EPOCH_TICKS + ( milliseconds * constants::TICKS_PER_MILLISECOND ) };

        return DateTime{ ticks };
    }

    //=====================================================================
    // Stream operators
    //=====================================================================

    std::ostream& operator<<( std::ostream& os, const DateTime& dateTime );

    std::istream& operator>>( std::istream& is, DateTime& dateTime );
} // namespace nfx::time

//=====================================================================
// std::formatter specialization
//=====================================================================

namespace std
{
    template <>
    struct formatter<nfx::time::DateTime>
    {
        constexpr auto parse( std::format_parse_context& ctx )
        {
            return ctx.begin();
        }

        auto format( const nfx::time::DateTime& dt, std::format_context& ctx ) const
        {
            return format_to( ctx.out(), "{}", dt.toString( nfx::time::DateTime::Format::Iso8601 ) );
        }
    };
} // namespace std
