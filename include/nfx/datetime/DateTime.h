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
 * @file DateTime.h
 * @brief Cross-platform DateTime class with ISO 8601 support
 * @details Provides datetime operations with 100-nanosecond precision,
 *          ISO 8601 parsing and formatting, and cross-platform compatibility.
 *          Supports date range from January 1, 0001 to December 31, 9999.
 *
 * @note DateTime can represent both UTC and local times. For timezone-aware operations
 *       with explicit offset information, use the DateTimeOffset class instead.
 *
 * @section iso8601_datetime_format ISO 8601 DateTime Format
 *
 * @par DateTime uses ISO 8601 format with UTC timezone for string representation:
 * @code
 * ┌─────────────────────────────────────────────────────────────────────┐
 * │                     ISO 8601 DateTime Format                        │
 * ├─────────────────────────────────────────────────────────────────────┤
 * │  YYYY-MM-DD  T  HH:MM:SS  [.fffffff]  Z                             │
 * │   │  │  │   │   │  │  │      │        │                             │
 * │   │  │  │   │   │  │  │      │        └─ Zulu (UTC indicator)       │
 * │   │  │  │   │   │  │  │      └────────── Fraction (optional)        │
 * │   │  │  │   │   │  │  └───────────────── Seconds (00-59)            │
 * │   │  │  │   │   │  └──────────────────── Minutes (00-59)            │
 * │   │  │  │   │   └─────────────────────── Hours (00-23)              │
 * │   │  │  │   └─────────────────────────── Time separator             │
 * │   │  │  └─────────────────────────────── Day (01-31)                │
 * │   │  └────────────────────────────────── Month (01-12)              │
 * │   └───────────────────────────────────── Year (0001-9999)           │
 * │                                                                     │
 * │  Note: DateTime values can represent UTC or local time. The 'Z'     │
 * │        suffix is used for UTC serialization, but DateTime itself    │
 * │        doesn't store timezone information.                          │
 * └─────────────────────────────────────────────────────────────────────┘
 * @endcode
 *
 * @par Format Examples:
 * @code
 * ┌────────────────────────────────┬───────────────────────────────────┐
 * │          Input String          │              Meaning              │
 * ├────────────────────────────────┼───────────────────────────────────┤
 * │  2024-01-15T12:30:45Z          │  Jan 15, 2024, 12:30:45 UTC       │
 * │  2024-06-20T00:00:00Z          │  Jun 20, 2024, midnight UTC       │
 * │  2024-12-31T23:59:59Z          │  Dec 31, 2024, 23:59:59 UTC       │
 * │  2024-01-15T12:30:45.1230000Z  │  With 7-digit fractions (123 ms)  │
 * │  2024-01-15T12:30:45.123Z      │  Trimmed fractions (123 ms)       │
 * │  2024-01-15T12:30:45.1234560Z  │  With microseconds (123.456 ms)   │
 * │  2024-01-15T12:30:45.123456Z   │  Trimmed microseconds (123.456 ms)│
 * │  2024-01-15T12:30:45.1234567Z  │  Full precision (100 ns ticks)    │
 * │  1970-01-01T00:00:00Z          │  Unix epoch (start of Unix time)  │
 * │  0001-01-01T00:00:00Z          │  Minimum DateTime value           │
 * │  9999-12-31T23:59:59.9999999Z  │  Maximum DateTime value           │
 * └────────────────────────────────┴───────────────────────────────────┘
 * @endcode
 *
 * @par Date-Only Format:
 * @code
 * ┌──────────────┬────────────────────────────────────────────────┐
 * │  YYYY-MM-DD  │  Date without time (time implicitly 00:00:00)  │
 * ├──────────────┼────────────────────────────────────────────────┤
 * │  2024-01-15  │  January 15, 2024 at midnight UTC              │
 * │  2024-06-20  │  June 20, 2024 at midnight UTC                 │
 * └──────────────┴────────────────────────────────────────────────┘
 * @endcode
 *
 * @par Internal Representation:
 * @code
 * ┌──────────────────────────────────────────────────────────────────────┐
 * │        DateTime stores absolute time as 100-nanosecond ticks         │
 * ├──────────────────────────────────────────────────────────────────────┤
 * │                                                                      │
 * │  Tick = 100 nanoseconds (0.0000001 seconds)                          │
 * │                                                                      │
 * │   ┌─────────────────────────────────────────────────────────────┐    │
 * │   │                  Tick Conversion Reference                  │    │
 * │   ├─────────────────────────────────────────────────────────────┤    │
 * │   │  1 tick                = 100 nanoseconds                    │    │
 * │   │  10 ticks              = 1 microsecond                      │    │
 * │   │  10,000 ticks          = 1 millisecond                      │    │
 * │   │  10,000,000 ticks      = 1 second                           │    │
 * │   │  600,000,000 ticks     = 1 minute                           │    │
 * │   │  36,000,000,000 ticks  = 1 hour                             │    │
 * │   │  864,000,000,000 ticks = 1 day                              │    │
 * │   └─────────────────────────────────────────────────────────────┘    │
 * │                                                                      │
 * │  Epoch: Ticks are counted from January 1, 0001 00:00:00.0000000 UTC  │
 * │                                                                      │
 * │  Range: 0001-01-01T00:00:00.0000000Z to 9999-12-31T23:59:59.9999999Z │
 * │         (0 ticks to 3,155,378,975,999,999,999 ticks)                 │
 * │                                                                      │
 * │  Storage: Single int64_t (8 bytes) - compact and efficient           │
 * └──────────────────────────────────────────────────────────────────────┘
 * @endcode
 *
 * @par Key Characteristics:
 * @code
 * ┌──────────────────┬───────────────────────────────────────────────┐
 * │     Property     │                  Description                  │
 * ├──────────────────┼───────────────────────────────────────────────┤
 * │  Timezone        │  Can represent UTC or local time              │
 * │  Precision       │  100 nanoseconds (10 million ticks/second)    │
 * │  Range           │  Year 1 to Year 9999 (Gregorian calendar)     │
 * │  Storage size    │  8 bytes (single int64_t)                     │
 * │  String format   │  ISO 8601 with 'Z' suffix (UTC)               │
 * │  Leap years      │  Fully supported (Feb 29 in leap years)       │
 * │  Leap seconds    │  Not supported (follows standard time)        │
 * │  Calendar        │  Proleptic Gregorian (extended before 1582)   │
 * │  Use case        │  System logs, timestamps, general datetime    │
 * └──────────────────┴───────────────────────────────────────────────┘
 * @endcode
 *
 * @par DateTime vs DateTimeOffset:
 * @code
 * ┌───────────────────┬─────────────────────┬─────────────────────────────┐
 * │      Feature      │      DateTime       │       DateTimeOffset        │
 * ├───────────────────┼─────────────────────┼─────────────────────────────┤
 * │  Timezone info    │  None (implicit)    │  Explicit offset from UTC   │
 * │  Storage          │  8 bytes            │  16 bytes (DateTime+offset) │
 * │  String format    │  ...Z               │  ...±HH:MM or Z             │
 * │  Local time       │  Supported via now()│  Always explicit with offset│
 * │  Comparisons      │  Direct             │  Via UTC conversion         │
 * │  Use for          │  General datetime   │  Timezone-aware operations  │
 * │  Example          │  Log timestamps     │  Appointment scheduling     │
 * └───────────────────┴─────────────────────┴─────────────────────────────┘
 * @endcode
 *
 * @par Common DateTime Operations:
 * @code
 * ┌──────────────────────────────────────────────────────────────────────┐
 * │                          Operation Examples                          │
 * ├──────────────────────────────────────────────────────────────────────┤
 * │  // Current local time                                               │
 * │  DateTime now{ DateTime::now() };                                    │
 * │                                                                      │
 * │  // Current UTC time                                                 │
 * │  DateTime utcNow{ DateTime::utcNow() };                              │
 * │                                                                      │
 * │  // Create specific date/time                                        │
 * │  DateTime dt{ 2024, 6, 15, 14, 30, 0 };  // Jun 15, 2024 14:30       │
 * │                                                                      │
 * │  // Parse from string (constructor - throws on error)                │
 * │  DateTime parsed1{ "2024-06-15T14:30:00Z" };                         │
 * │                                                                      │
 * │  // Parse from string (safe - returns std::optional)                 │
 * │  auto parsed2 = DateTime::fromString("2024-06-15T14:30:00Z");        │
 * │  if (parsed2)                                                        │
 * │  {                                                                   │
 * │      // use parsed2                                                  │
 * │  }                                                                   │
 * │                                                                      │
 * │  // Arithmetic                                                       │
 * │  DateTime later{ dt + TimeSpan::fromHours(2.5) };                    │
 * │  TimeSpan duration = later - dt;                                     │
 * │                                                                      │
 * │  // Epoch timestamp conversion                                       │
 * │  int64_t epochSecs = dt.toEpochSeconds();                            │
 * │  DateTime fromEpoch{ DateTime::fromEpochSeconds(epochSecs) };        │
 * │                                                                      │
 * │  // Format output                                                    │
 * │  std::string iso{ dt.toString(Format::Iso8601) };                    │
 * │  std::string precise{ dt.toString(Format::Iso8601Precise) };         │
 * └──────────────────────────────────────────────────────────────────────┘
 * @endcode
 *
 * @par Important Notes:
 * - **UTC vs Local**: Use `now()` for local time, `utcNow()` for UTC
 * - **No Timezone Info**: DateTime doesn't store timezone offset (use DateTimeOffset for that)
 * - **No Daylight Saving**: DateTime doesn't track DST changes automatically
 * - **Leap Years**: Automatically handled (Feb 29 exists in 2024, 2028, etc.)
 * - **No Leap Seconds**: Follows standard civil time (ignores leap seconds)
 * - **std::chrono Integration**: Convertible to/from std::chrono::system_clock::time_point
 * - **Thread-Safe**: All operations are thread-safe (immutable value type)
 */

#pragma once

#include <chrono>
#include <compare>
#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <string_view>

#include "TimeSpan.h"

namespace nfx::time
{
    /**
     * @brief Cross-platform DateTime type with 100-nanosecond precision
     * @details Implements datetime operations with:
     *          - 100-nanosecond tick precision (10 million ticks per second)
     *          - Range: January 1, 0001 to December 31, 9999
     *          - ISO 8601 string parsing and formatting
     *          - System clock interoperability
     *          - Arithmetic operations with time intervals
     *          - Support for both UTC and local time representations
     *
     *          Note: DateTime doesn't store timezone information. For explicit timezone
     *          offset tracking, use DateTimeOffset instead.
     */
    class DateTime final
    {
    public:
        //----------------------------------------------
        // Enumerations
        //----------------------------------------------

        /**
         * @brief DateTime string format options
         * @details Provides type-safe format selection with self-documenting format names
         */
        enum class Format : std::uint8_t
        {
            /** @brief ISO 8601 with seconds precision: "2024-01-01T12:00:00Z" */
            Iso8601,

            /** @brief ISO 8601 with full tick precision (7 decimal digits): "2024-01-01T12:00:00.1234567Z" */
            Iso8601Precise,

            /** @brief ISO 8601 with trimmed trailing zeros in fractions: "2024-01-01T12:00:00.1234Z" */
            Iso8601PreciseTrimmed,

            /** @brief ISO 8601 with numeric offset (always +00:00 for DateTime): "2024-01-01T12:00:00+00:00" */
            Iso8601Extended,

            /** @brief ISO 8601 compact form without separators: "20240101T120000Z" */
            Iso8601Basic,

            /** @brief ISO 8601 date only: "2024-01-01" */
            Iso8601Date,

            /** @brief ISO 8601 time only: "12:00:00" */
            Iso8601Time,

            /** @brief Unix epoch seconds (integer): "1704110400" */
            UnixSeconds,

            /** @brief Unix epoch milliseconds (integer): "1704110400000" */
            UnixMilliseconds,
        };

        //----------------------------------------------
        // Construction
        //----------------------------------------------

        /** @brief Default constructor (minimum DateTime value) */
        constexpr DateTime() noexcept;

        /**
         * @brief Construct from tick count (100-nanosecond units since year 1)
         * @param ticks Number of 100-nanosecond intervals since January 1, 0001 UTC
         */
        explicit constexpr inline DateTime( std::int64_t ticks ) noexcept;

        /**
         * @brief Construct from system clock time point
         * @param timePoint System clock time point to convert from
         */
        inline explicit DateTime( std::chrono::system_clock::time_point timePoint ) noexcept;

        /**
         * @brief Construct from date components
         * @param year Year component (1-9999)
         * @param month Month component (1-12)
         * @param day Day component (1-31)
         */
        DateTime( std::int32_t year, std::int32_t month, std::int32_t day ) noexcept;

        /**
         * @brief Construct from date and time components
         * @param year Year component (1-9999)
         * @param month Month component (1-12)
         * @param day Day component (1-31)
         * @param hour Hour component (0-23)
         * @param minute Minute component (0-59)
         * @param second Second component (0-59)
         */
        DateTime( std::int32_t year, std::int32_t month, std::int32_t day,
            std::int32_t hour, std::int32_t minute, std::int32_t second ) noexcept;

        /**
         * @brief Construct from date and time components with milliseconds
         * @param year Year component (1-9999)
         * @param month Month component (1-12)
         * @param day Day component (1-31)
         * @param hour Hour component (0-23)
         * @param minute Minute component (0-59)
         * @param second Second component (0-59)
         * @param millisecond Millisecond component (0-999)
         */
        DateTime( std::int32_t year, std::int32_t month, std::int32_t day,
            std::int32_t hour, std::int32_t minute, std::int32_t second,
            std::int32_t millisecond ) noexcept;

        /**
         * @brief Parse from ISO 8601 string
         * @param iso8601String ISO 8601 formatted string to parse
         */
        inline explicit DateTime( std::string_view iso8601String );

        /**
         * @brief Parse from C-string (convenience for string literals)
         * @param iso8601String ISO 8601 formatted C-string to parse
         */
        inline explicit DateTime( const char* iso8601String );

        /**
         * @brief Initialize from initializer list (convenience for single string)
         * @param list Initializer list containing ISO 8601 string
         */
        inline explicit DateTime( std::initializer_list<const char*> list );

        /** @brief Copy constructor */
        DateTime( const DateTime& ) = default;

        /** @brief Move constructor */
        DateTime( DateTime&& ) noexcept = default;

        //----------------------------------------------
        // Destruction
        //----------------------------------------------

        /** @brief Destructor */
        ~DateTime() = default;

        //----------------------------------------------
        // Assignment
        //----------------------------------------------

        /**
         * @brief Copy assignment operator
         * @return Reference to this DateTime after assignment
         */
        DateTime& operator=( const DateTime& ) = default;

        /**
         * @brief Move assignment operator
         * @return Reference to this DateTime after assignment
         */
        DateTime& operator=( DateTime&& ) noexcept = default;

        //----------------------------------------------
        // Comparison operators
        //----------------------------------------------

        /**
         * @brief Three-way comparison operator
         * @param other The DateTime to compare with
         * @return std::strong_ordering indicating the relative order of the two DateTimes
         */
        inline constexpr std::strong_ordering operator<=>( const DateTime& other ) const noexcept;

        /**
         * @brief Equality comparison
         * @param other The DateTime to compare with
         * @return true if both DateTimes represent the same instant, false otherwise
         */
        inline constexpr bool operator==( const DateTime& other ) const noexcept;

        //----------------------------------------------
        // Arithmetic operators
        //----------------------------------------------

        /**
         * @brief Add time duration
         * @param duration The TimeSpan to add to this DateTime
         * @return New DateTime representing this DateTime plus the duration
         */
        inline constexpr DateTime operator+( const TimeSpan& duration ) const noexcept;

        /**
         * @brief Subtract time duration
         * @param duration The TimeSpan to subtract from this DateTime
         * @return New DateTime representing this DateTime minus the duration
         */
        inline constexpr DateTime operator-( const TimeSpan& duration ) const noexcept;

        /**
         * @brief Get time difference between DateTimes
         * @param other The DateTime to subtract from this DateTime
         * @return TimeSpan representing the difference (this - other)
         */
        inline constexpr TimeSpan operator-( const DateTime& other ) const noexcept;

        /**
         * @brief Add time duration (in-place)
         * @param duration The TimeSpan to add to this DateTime
         * @return Reference to this DateTime after adding the duration
         */
        inline constexpr DateTime& operator+=( const TimeSpan& duration ) noexcept;

        /**
         * @brief Subtract time duration (in-place)
         * @param duration The TimeSpan to subtract from this DateTime
         * @return Reference to this DateTime after subtracting the duration
         */
        inline constexpr DateTime& operator-=( const TimeSpan& duration ) noexcept;

        //----------------------------------------------
        // Property accessors
        //----------------------------------------------

        /**
         * @brief Get year component (1-9999)
         * @return The year component of this DateTime (1-9999)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t year() const noexcept;

        /**
         * @brief Get month component (1-12)
         * @return The month component of this DateTime (1-12)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t month() const noexcept;

        /**
         * @brief Get day component (1-31)
         * @return The day component of this DateTime (1-31)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t day() const noexcept;

        /**
         * @brief Get hour component (0-23)
         * @return The hour component of this DateTime (0-23)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t hour() const noexcept;

        /**
         * @brief Get minute component (0-59)
         * @return The minute component of this DateTime (0-59)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t minute() const noexcept;

        /**
         * @brief Get second component (0-59)
         * @return The second component of this DateTime (0-59)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t second() const noexcept;

        /**
         * @brief Get millisecond component (0-999)
         * @return The millisecond component of this DateTime (0-999)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t millisecond() const noexcept;

        /**
         * @brief Get microsecond component (0-999)
         * @return The microsecond component of this DateTime (0-999)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t microsecond() const noexcept;

        /**
         * @brief Get nanosecond component (0-900, in 100ns increments)
         * @return The nanosecond component of this DateTime (0, 100, 200, ..., 900)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         * @note Due to 100-nanosecond tick precision, only values 0-900 in 100ns increments are possible
         */
        [[nodiscard]] std::int32_t nanosecond() const noexcept;

        /**
         * @brief Get tick count (100-nanosecond units since year 1)
         * @return The number of 100-nanosecond intervals since January 1, 0001 UTC
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] constexpr std::int64_t ticks() const noexcept;

        /**
         * @brief Get day of week (0=Sunday, 6=Saturday)
         * @return The day of week as an integer (0=Sunday, 1=Monday, ..., 6=Saturday)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t dayOfWeek() const noexcept;

        /**
         * @brief Get day of year (1-366)
         * @return The day of year as an integer (1-366, where 366 occurs in leap years)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::int32_t dayOfYear() const noexcept;

        //----------------------------------------------
        // Conversion methods
        //----------------------------------------------

        /**
         * @brief Convert to Epoch timestamp (seconds since epoch)
         * @return Number of seconds since Unix epoch (January 1, 1970 00:00:00 UTC)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline constexpr std::int64_t toEpochSeconds() const noexcept;

        /**
         * @brief Convert to Epoch timestamp (milliseconds since epoch)
         * @return Number of milliseconds since Unix epoch (January 1, 1970 00:00:00 UTC)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline constexpr std::int64_t toEpochMilliseconds() const noexcept;

        /**
         * @brief Get date component (time set to 00:00:00)
         * @return New DateTime with the same date but time set to 00:00:00
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] DateTime date() const noexcept;

        /**
         * @brief Get time of day as duration since midnight
         * @return TimeSpan representing the elapsed time since midnight (00:00:00)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] TimeSpan timeOfDay() const noexcept;

        //----------------------------------------------
        // Validation methods
        //----------------------------------------------

        /**
         * @brief Check if this DateTime is valid
         * @return true if this DateTime represents a valid date and time, false otherwise
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] bool isValid() const noexcept;

        /**
         * @brief Check if given year is a leap year
         * @param year The year to check for leap year status
         * @return true if the year is a leap year, false otherwise
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr bool isLeapYear( std::int32_t year ) noexcept;

        /**
         * @brief Get days in month for given year and month
         * @param year The year to check (1-9999)
         * @param month The month to check (1-12)
         * @return Number of days in the specified month (28-31)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr std::int32_t daysInMonth( std::int32_t year, std::int32_t month ) noexcept;

        //----------------------------------------------
        // Static factory methods
        //----------------------------------------------

        /**
         * @brief Get current local time
         * @return DateTime representing the current local date and time (system timezone)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static DateTime now() noexcept;

        /**
         * @brief Get current UTC time
         * @return DateTime representing the current UTC date and time
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static DateTime utcNow() noexcept;

        /**
         * @brief Get current local date (time set to 00:00:00)
         * @return DateTime representing the current local date with time set to 00:00:00
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static DateTime today() noexcept;

        /**
         * @brief Get minimum DateTime value
         * @return DateTime representing the minimum representable date (January 1, 0001 00:00:00.0000000 UTC)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr DateTime min() noexcept;

        /**
         * @brief Get maximum DateTime value
         * @return DateTime representing the maximum representable date (December 31, 9999 23:59:59.9999999 UTC)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr DateTime max() noexcept;

        /**
         * @brief Get Unix epoch DateTime (January 1, 1970 00:00:00 UTC)
         * @return DateTime representing the Unix epoch (January 1, 1970 00:00:00 UTC)
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr DateTime epoch() noexcept;

        /**
         * @brief Parse ISO 8601 string safely without throwing exceptions
         * @param iso8601String The ISO 8601 formatted string to parse
         * @param result Reference to store the parsed DateTime if successful
         * @return true if parsing succeeded, false otherwise
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static bool fromString( std::string_view iso8601String, DateTime& result ) noexcept;

        /**
         * @brief Parse ISO 8601 string and return optional DateTime
         * @param iso8601String The ISO 8601 formatted string to parse
         * @return std::optional<DateTime> containing the parsed value if successful, std::nullopt otherwise
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static std::optional<DateTime> fromString( std::string_view iso8601String ) noexcept;

        /**
         * @brief Create from Epoch timestamp (seconds since epoch)
         * @param seconds The number of seconds since Unix epoch (January 1, 1970 00:00:00 UTC)
         * @return DateTime representing the specified Epoch timestamp
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr DateTime fromEpochSeconds( std::int64_t seconds ) noexcept;

        /**
         * @brief Create from Epoch timestamp (milliseconds since epoch)
         * @param milliseconds The number of milliseconds since Unix epoch (January 1, 1970 00:00:00 UTC)
         * @return DateTime representing the specified Epoch timestamp in milliseconds
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline static constexpr DateTime fromEpochMilliseconds( std::int64_t milliseconds ) noexcept;

        //----------------------------------------------
        // String formatting
        //----------------------------------------------

        /**
         * @brief Convert to string using specified format
         * @param format The format to use for string conversion
         * @return String representation using the specified format
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::string toString( Format format = Format::Iso8601 ) const;

        //----------------------------------------------
        // std::chrono interoperability
        //----------------------------------------------

        /**
         * @brief Convert to std::chrono::system_clock::time_point
         * @details Values outside the representable range of std::chrono::system_clock
         *          (approximately years 1677-2262 on 64-bit Linux systems) will be
         *          clamped to the nearest representable value. This means extreme dates
         *          (near year 1 or year 9999) cannot round-trip through chrono.
         * @return A system_clock::time_point representing this DateTime
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] std::chrono::system_clock::time_point toChrono() const noexcept;

        /**
         * @brief Create DateTime from std::chrono::system_clock::time_point
         * @details Values outside the representable range of DateTime
         *          will be clamped to the nearest valid DateTime value.
         * @param timePoint The system_clock time point to convert
         * @return A DateTime representing the same instant in time
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] static DateTime fromChrono( const std::chrono::system_clock::time_point& timePoint ) noexcept;

    private:
        /** @brief 100-nanosecond ticks since January 1, 0001 UTC */
        std::int64_t m_ticks;
    };
} // namespace nfx::time

#include "nfx/detail/datetime/DateTime.inl"
