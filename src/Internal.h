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
 * @file Internal.h
 * @brief Internal helper functions shared across nfx-datetime implementation
 * @details Provides cross-platform timezone offset calculation, thread-safe time conversion
 *          wrappers, and other internal utilities used by DateTime and DateTimeOffset classes.
 *          Not part of the public API.
 */

#pragma once

#include <atomic>
#include <chrono>
#include <ctime>

#include "nfx/datetime/DateTime.h"
#include "nfx/datetime/TimeSpan.h"
#include "nfx/detail/datetime/Constants.h"

//----------------------------------------------
// Cross-platform time functions
//----------------------------------------------

/** @brief Cross-platform thread-safe time conversion functions */
#if defined( __MINGW32__ ) || defined( __MINGW64__ )
// MinGW uses Windows CRT gmtime/localtime (thread-safe on modern MinGW)
/** @brief Thread-safe UTC time conversion wrapper for MinGW */
#    define NFX_DATETIME_GMTIME_R( timer, result ) ( ( *( result ) = *gmtime( timer ) ), ( result )->tm_year >= 0 )
/** @brief Thread-safe local time conversion wrapper for MinGW */
#    define NFX_DATETIME_LOCALTIME_R( timer, result )                                                                  \
        ( ( *( result ) = *localtime( timer ) ), ( result )->tm_year >= 0 )
#elif defined( _WIN32 )
// MSVC Windows uses gmtime_s and localtime_s (different parameter order than POSIX)
/** @brief Thread-safe UTC time conversion wrapper for Windows gmtime_s */
#    define NFX_DATETIME_GMTIME_R( timer, result ) ( gmtime_s( ( result ), ( timer ) ) == 0 )
/** @brief Thread-safe local time conversion wrapper for Windows localtime_s */
#    define NFX_DATETIME_LOCALTIME_R( timer, result ) ( localtime_s( ( result ), ( timer ) ) == 0 )
#elif defined( __GNUC__ ) || defined( __clang__ )
// POSIX systems use gmtime_r and localtime_r
/** @brief Thread-safe UTC time conversion wrapper for POSIX gmtime_r */
#    define NFX_DATETIME_GMTIME_R( timer, result ) ( gmtime_r( ( timer ), ( result ) ) != nullptr )
/** @brief Thread-safe local time conversion wrapper for POSIX localtime_r */
#    define NFX_DATETIME_LOCALTIME_R( timer, result ) ( localtime_r( ( timer ), ( result ) ) != nullptr )
#else
// Fallback to non-thread-safe versions
/** @brief Fallback UTC time conversion using non-thread-safe gmtime */
#    define NFX_DATETIME_GMTIME_R( timer, result ) ( ( *( result ) = *gmtime( timer ) ), true )
/** @brief Fallback local time conversion using non-thread-safe localtime */
#    define NFX_DATETIME_LOCALTIME_R( timer, result ) ( ( *( result ) = *localtime( timer ) ), true )
#endif

namespace nfx::time::internal
{
    /**
     * @brief Thread-safe timezone offset cache with hour-level granularity
     * @details Caches timezone offset per hour to handle DST transitions correctly.
     *          Uses atomic operations for lock-free reads in the common case.
     */
    class TimeZoneOffsetCache
    {
    public:
        TimeZoneOffsetCache() noexcept
            : m_cachedHour{ 0 },
              m_offsetSeconds{ 0 }
        {
        }

        /**
         * @brief Get timezone offset for given DateTime, using cache when valid
         * @param dateTime The DateTime to get timezone offset for
         * @return TimeSpan representing the timezone offset
         * @note Cache invalidates hourly to handle DST transitions correctly
         */
        TimeSpan offset( const DateTime& dateTime ) noexcept
        {
            // Cache key: year * 1000000 + month * 10000 + day * 100 + hour
            // This ensures cache invalidation on DST transitions (which occur at specific hours)
            const auto year{ dateTime.year() };
            const auto month{ dateTime.month() };
            const auto day{ dateTime.day() };
            const auto hour{ dateTime.hour() };

            const std::int64_t currentHourKey{ year * 1000000LL + month * 10000 + day * 100 + hour };

            // Fast path: check if cache is valid (lock-free)
            if( m_cachedHour.load( std::memory_order_acquire ) == currentHourKey )
            {
                return TimeSpan::fromSeconds(
                    static_cast<double>( m_offsetSeconds.load( std::memory_order_acquire ) ) );
            }

            // Slow path: recompute offset
            const auto newOffset{ computeOffset( dateTime ) };

            // Update cache atomically
            m_offsetSeconds.store( newOffset, std::memory_order_release );
            m_cachedHour.store( currentHourKey, std::memory_order_release );

            return TimeSpan::fromSeconds( static_cast<double>( newOffset ) );
        }

    private:
        std::atomic<std::int64_t> m_cachedHour;    ///< Cached hour key (yyyymmddhh)
        std::atomic<std::int64_t> m_offsetSeconds; ///< Cached offset in seconds

        /**
         * @brief Compute timezone offset for given DateTime
         * @param dateTime The DateTime to compute offset for
         * @return Offset in seconds
         */
        std::int64_t computeOffset( const DateTime& dateTime ) noexcept
        {
            // Convert DateTime to Unix timestamp (seconds since 1970-01-01)
            const auto unixSeconds{ dateTime.toEpochSeconds() };
            const auto systemTimeT{ static_cast<std::time_t>( unixSeconds ) };

            std::tm utcTm{};
            std::tm localTm{};

            const bool utcSuccess = NFX_DATETIME_GMTIME_R( &systemTimeT, &utcTm );
            const bool localSuccess = NFX_DATETIME_LOCALTIME_R( &systemTimeT, &localTm );

            if( !utcSuccess || !localSuccess )
            {
                return 0; // Fallback to UTC
            }

            const auto utcSeconds{ utcTm.tm_hour * constants::SECONDS_PER_HOUR +
                                   utcTm.tm_min * constants::SECONDS_PER_MINUTE + utcTm.tm_sec };
            const auto localSeconds{ localTm.tm_hour * constants::SECONDS_PER_HOUR +
                                     localTm.tm_min * constants::SECONDS_PER_MINUTE + localTm.tm_sec };

            auto offsetSeconds{ localSeconds - utcSeconds };

            // Handle day boundary crossing
            if( localTm.tm_mday != utcTm.tm_mday )
            {
                if( localTm.tm_mday > utcTm.tm_mday )
                {
                    offsetSeconds += constants::SECONDS_PER_DAY;
                }
                else
                {
                    offsetSeconds -= constants::SECONDS_PER_DAY;
                }
            }

            return offsetSeconds;
        }
    };

    /**
     * @brief Get system timezone offset with caching
     * @param dateTime The DateTime to get timezone offset for
     * @return TimeSpan representing the timezone offset
     */
    inline TimeSpan systemTimezoneOffset( const DateTime& dateTime ) noexcept
    {
        static TimeZoneOffsetCache cache;
        return cache.offset( dateTime );
    }

} // namespace nfx::time::internal
