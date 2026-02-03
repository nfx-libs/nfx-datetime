# Changelog

## [Unreleased]

### Added

- NIL

### Changed

- NIL

### Deprecated

- NIL

### Removed

- NIL

### Fixed

- Fixed warnings for unused local variables in `DateTime::toString()` and `DateTimeOffset` formatting functions

### Security

- NIL

## [0.4.0] - 2026-02-03

### Added

- nfx-stringbuilder dependency (v0.5.0) for high-performance string building
- Fast-path parsing optimizations for common ISO 8601 formats

### Changed

- `TimeSpan::toString()` now uses `nfx::StringBuilder` instead of `std::ostringstream`
- `DateTimeOffset::toString()` now uses `nfx::StringBuilder` instead of `std::ostringstream`
- `DateTime::toString()` now uses `nfx::StringBuilder` instead of `std::ostringstream`
- Updated Google Benchmark dependency from 1.9.4 to 1.9.5

## [0.3.0] - 2026-01-11

### Added

- C-string constructors for `DateTime` and `DateTimeOffset` for convenient initialization from string literals
- `std::initializer_list<const char*>` constructors for `DateTime` and `DateTimeOffset` for single-string initialization syntax
- `DateTime::Format::Iso8601PreciseTrimmed` format option that removes trailing zeros from fractional seconds (e.g., `.1230000Z` → `.123Z`, `.1000000Z` → `.1Z`) while maintaining full ISO 8601:2019 §5.3.4.2 compliance
- `DateTime::Format::Iso8601Millis` format option for fixed 3-digit millisecond precision (e.g., `.123Z`, `.000Z`)
- `DateTime::Format::Iso8601Micros` format option for fixed 6-digit microsecond precision (e.g., `.123456Z`, `.000000Z`)

### Changed

- **BREAKING**: DateTime::Format enum values renamed (`Iso8601WithOffset` → `Iso8601Extended`, `Iso8601Compact` → `Iso8601Basic`) to align with ISO 8601 terminology

## [0.2.0] - 2026-01-05

### Changed

- **BREAKING**: DateTime::Format enum renamed (`Iso8601Basic` → `Iso8601`, `Iso8601Extended` → `Iso8601Precise`, `DateOnly` → `Iso8601Date`, `TimeOnly` → `Iso8601Time`)
- **BREAKING**: Fractional seconds always display exactly 7 digits (tick precision) in `Iso8601Precise` format
- **BREAKING**: DateTimeOffset offset always explicit (`±HH:MM`), removed `Z` notation for UTC
- **BREAKING**: toString() consolidated to single method with default parameter
- 
### Removed

- **BREAKING**: Standalone `toIso8601Extended()` method (use `toString(Format::Iso8601Precise)`)

## [0.1.1] - 2025-11-27

### Changed

- Consolidated packaging tool detection in CMake configuration

### Fixed

- Removed incorrect runtime dependencies from DEB/RPM packages

## [0.1.0] - 2025-11-17 - Initial Release

### Added

- **DateTime**: UTC-based date and time representation

  - 100-nanosecond precision (10 million ticks per second)
  - Range: January 1, 0001 to December 31, 9999
  - Complete arithmetic operators (+, -, comparisons)
  - Factory methods (`now()`, `today()`, `min()`, `max()`, `epoch()`, `fromEpochSeconds()`, `fromEpochMilliseconds()`)
  - Component accessors (`year()`, `month()`, `day()`, `hour()`, `minute()`, `second()`, `millisecond()`, `microsecond()`, `nanosecond()`, `dayOfWeek()`, `dayOfYear()`)
  - ISO 8601 parsing and formatting (`fromString()`, `toString()`, `toIso8601Extended()`)
  - Epoch timestamp conversion (`toEpochSeconds()`, `toEpochMilliseconds()`)
  - Date/time manipulation (`date()`, `timeOfDay()`)
  - std::chrono interoperability (`toChrono()`, `fromChrono()`)

- **DateTimeOffset**: Timezone-aware date and time representation

  - All DateTime features plus timezone offset support
  - Offset range: -14:00 to +14:00
  - Component accessors (`year()`, `month()`, `day()`, `hour()`, `minute()`, `second()`, `millisecond()`, `microsecond()`, `nanosecond()`, `dayOfWeek()`, `dayOfYear()`)
  - Timezone conversion (`toOffset()`, `toUniversalTime()`, `toLocalTime()`)
  - ISO 8601 parsing and formatting with timezone (`fromString()`, `toString()`, `toIso8601Extended()`)
  - UTC and local time accessors (`utcDateTime()`, `localDateTime()`, `utcTicks()`)
  - Factory methods (`now()`, `utcNow()`, `today()`, `fromEpochSeconds()`, `fromEpochMilliseconds()`, `fromFILETIME()`)
  - Date/time arithmetic (`add()`, `addDays()`, `addHours()`, `addMinutes()`, `addSeconds()`, `addMonths()`, `addYears()`)
  - Windows FILETIME conversion support

- **TimeSpan**: Duration and time interval representation

  - 100-nanosecond precision matching DateTime
  - Range: -10,675,199 days to +10,675,199 days
  - Complete arithmetic operators (+, -, +=, -=, unary -, \*, /, comparisons)
  - User-defined literals for natural C++ syntax (`_d`, `_h`, `_min`, `_s`, `_ms`, `_us`, `_ns`)
  - Compile-time evaluation with `constexpr` literals
  - Factory methods (`fromDays()`, `fromHours()`, `fromMinutes()`, `fromSeconds()`, `fromMilliseconds()`, `fromMicroseconds()`, `fromTicks()`)
  - Total value accessors (`days()`, `hours()`, `minutes()`, `seconds()`, `milliseconds()`, `microseconds()`, `nanoseconds()`)
  - Tick count accessor (`ticks()`)
  - ISO 8601 duration parsing and formatting (`fromString()`, `toString()`)
  - Duration calculations and comparisons
  - std::chrono::duration interoperability (`toChrono()`, `fromChrono()`)
  - Proper rounding to 100ns tick precision for sub-tick values

- **Documentation**

  - README with feature overview
  - Detailed API documentation with Doxygen comments
  - Sample application demonstrating library usage
  - Build and installation instructions

- **Testing & Benchmarking**
  - Unit test suite
  - Performance benchmarks for all operations
  - Cross-compiler performance validation
