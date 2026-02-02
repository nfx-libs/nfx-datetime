# nfx-datetime

<!-- Project Info -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://github.com/nfx-libs/nfx-datetime/blob/main/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/nfx-libs/nfx-datetime?style=flat-square)](https://github.com/nfx-libs/nfx-datetime/releases) [![GitHub tag (latest by date)](https://img.shields.io/github/tag/nfx-libs/nfx-datetime?style=flat-square)](https://github.com/nfx-libs/nfx-datetime/tags)<br/>

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square) ![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg?style=flat-square) ![Cross Platform](https://img.shields.io/badge/Platform-Linux_Windows-lightgrey?style=flat-square)

<!-- CI/CD Status -->

[![Linux GCC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datetime/build-linux-gcc.yml?branch=main&label=Linux%20GCC&style=flat-square)](https://github.com/nfx-libs/nfx-datetime/actions/workflows/build-linux-gcc.yml) [![Linux Clang](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datetime/build-linux-clang.yml?branch=main&label=Linux%20Clang&style=flat-square)](https://github.com/nfx-libs/nfx-datetime/actions/workflows/build-linux-clang.yml) [![Windows MinGW](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datetime/build-windows-mingw.yml?branch=main&label=Windows%20MinGW&style=flat-square)](https://github.com/nfx-libs/nfx-datetime/actions/workflows/build-windows-mingw.yml) [![Windows MSVC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datetime/build-windows-msvc.yml?branch=main&label=Windows%20MSVC&style=flat-square)](https://github.com/nfx-libs/nfx-datetime/actions/workflows/build-windows-msvc.yml) [![CodeQL](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-datetime/codeql.yml?branch=main&label=CodeQL&style=flat-square)](https://github.com/nfx-libs/nfx-datetime/actions/workflows/codeql.yml)

> A high-performance C++20 datetime library with 100-nanosecond precision, full ISO 8601 support, and cross-platform timezone handling

## Overview

nfx-datetime is a modern C++20 library providing high-precision temporal operations with 100-nanosecond resolution. It offers comprehensive ISO 8601 parsing and formatting, timezone-aware datetime handling, and arithmetic operations optimized for performance across multiple platforms and compilers.

## Key Features

### 🕒 High-Precision Temporal Types

- **DateTime**: UTC-only datetime operations with 100-nanosecond precision (ticks)
- **DateTimeOffset**: Timezone-aware datetime with UTC offset handling
- **TimeSpan**: Duration and interval arithmetic with high precision

### 📅 ISO 8601 Compliance

- Full ISO 8601 parsing and formatting support
- Basic format: `20250124T054200Z`
- Extended format: `2025-01-24T05:42:00Z`
- Precision formats:
  - Full precision: `2025-01-24T05:42:00.1234567Z` (100-nanosecond ticks)
  - Trimmed: `2025-01-24T05:42:00.123Z` (trailing zeros removed)
  - Milliseconds: `2025-01-24T05:42:00.123Z` (3-digit fixed)
  - Microseconds: `2025-01-24T05:42:00.123456Z` (6-digit fixed)
- Timezone offsets: `2025-01-24T05:42:00+02:00`
- Duration format: `P1DT2H30M` (1 day, 2 hours, 30 minutes)

### 📊 Real-World Applications

- **Logging & Audit Systems**: High-precision timestamp recording for system logs and audit trails
- **Financial Systems**: Accurate datetime handling for transactions, settlements, and market data
- **IoT & Telemetry**: Precise time-series data collection and analysis
- **Calendar Applications**: Timezone-aware scheduling and appointment management
- **Data Synchronization**: Cross-platform timestamp coordination
- **API Services**: ISO 8601 compliant request/response timestamps
- **Database Operations**: Timestamp conversion and timezone handling
- **Event Processing**: Duration calculations and time-based event correlation

### ⚡ Performance Optimized

- High-precision arithmetic operations (100-nanosecond resolution)
- Highly optimized parsing
- Efficient string formatting
- Zero-cost abstractions with constexpr support
- Compiler-optimized inline implementations

### 🌍 Cross-Platform Support

- **Platforms**: Linux, Windows
- **Architecture**: x86-64
- **Compilers**: GCC 14+, Clang 18+, MSVC 2022+
- Thread-safe operations
- Consistent behavior across platforms
- CI/CD testing on multiple compilers

## Quick Start

### Requirements

- C++20 compatible compiler:
  - **GCC 14+** (14.2.0 tested)
  - **Clang 18+** (19.1.7 tested)
  - **MSVC 2022+** (19.44+ tested)
- CMake 3.20 or higher

### CMake Integration

```cmake
# Build options
option(NFX_DATETIME_BUILD_STATIC         "Build static library"               OFF )
option(NFX_DATETIME_BUILD_SHARED         "Build shared library"               OFF )

# Development options
option(NFX_DATETIME_BUILD_TESTS          "Build tests"                        OFF )
option(NFX_DATETIME_BUILD_SAMPLES        "Build samples"                      OFF )
option(NFX_DATETIME_BUILD_BENCHMARKS     "Build benchmarks"                   OFF )
option(NFX_DATETIME_BUILD_DOCUMENTATION  "Build Doxygen documentation"        OFF )

# Installation
option(NFX_DATETIME_INSTALL_PROJECT      "Install project"                    OFF )

# Packaging
option(NFX_DATETIME_PACKAGE_SOURCE       "Enable source package generation"   OFF )
option(NFX_DATETIME_PACKAGE_ARCHIVE      "Enable TGZ/ZIP package generation"  OFF )
option(NFX_DATETIME_PACKAGE_DEB          "Enable DEB package generation"      OFF )
option(NFX_DATETIME_PACKAGE_RPM          "Enable RPM package generation"      OFF )
option(NFX_DATETIME_PACKAGE_WIX          "Enable WiX MSI installer"           OFF )
```

### Using in Your Project

#### Option 1: Using FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
  nfx-datetime
  GIT_REPOSITORY https://github.com/nfx-libs/nfx-datetime.git
  GIT_TAG        main  # or use specific version tag like "0.1.0"
)
FetchContent_MakeAvailable(nfx-datetime)

# Link with static library
target_link_libraries(your_target PRIVATE nfx-datetime::static)
```

#### Option 2: As a Git Submodule

```bash
# Add as submodule
git submodule add https://github.com/nfx-libs/nfx-datetime.git third-party/nfx-datetime
```

```cmake
# In your CMakeLists.txt
add_subdirectory(third-party/nfx-datetime)
target_link_libraries(your_target PRIVATE nfx-datetime::static)
```

#### Option 3: Using find_package (After Installation)

```cmake
find_package(nfx-datetime REQUIRED)
target_link_libraries(your_target PRIVATE nfx-datetime::static)
```

### Building

**Build Commands:**

```bash
# Clone the repository
git clone https://github.com/nfx-libs/nfx-datetime.git
cd nfx-datetime

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
cmake --build . --config Release --parallel

# Run tests (optional)
ctest -C Release --output-on-failure

# Run benchmarks (optional)
./bin/benchmarks/BM_DateTime
./bin/benchmarks/BM_DateTimeOffset
./bin/benchmarks/BM_TimeSpan
```

### Documentation

nfx-datetime includes API documentation generated with Doxygen.

#### 📚 Online Documentation

The complete API documentation is available online at:
**https://nfx-libs.github.io/nfx-datetime**

#### Building Documentation Locally

```bash
# Configure with documentation enabled
cmake .. -DCMAKE_BUILD_TYPE=Release -DNFX_DATETIME_BUILD_DOCUMENTATION=ON

# Build the documentation
cmake --build . --target nfx-datetime-documentation
```

#### Requirements

- **Doxygen** - Documentation generation tool
- **Graphviz Dot** (optional) - For generating class diagrams

#### Accessing Local Documentation

After building, open `./build/doc/html/index.html` in your web browser.

## Usage Examples

### DateTime - UTC Operations

```cpp
#include <nfx/datetime/DateTime.h>

using namespace nfx::time;

// Current time
DateTime now = DateTime::now();

// Construction from components
DateTime dt1(2025, 1, 24, 5, 42, 0);  // 2025-01-24 05:42:00 UTC

// ISO 8601 parsing (using constructor - throws on error)
DateTime dt2("2025-01-24T05:42:00Z");

// ISO 8601 parsing (safe - returns std::optional)
auto dt3 = DateTime::fromString("2025-01-24T05:42:00Z");
if (dt3) {
    // Use *dt3
}

// ISO 8601 parsing (safe - returns bool)
DateTime dt4;
if (DateTime::fromString("2025-01-24T05:42:00Z", dt4)) {
    // Use dt4
}

// Arithmetic operations
TimeSpan oneHour = TimeSpan::fromHours(1);
DateTime later = dt1 + oneHour;

// Formatting
std::string iso = dt1.toString(DateTime::Format::Iso8601);                     // "2025-01-24T05:42:00Z"
std::string precise = dt1.toString(DateTime::Format::Iso8601Precise);          // "2025-01-24T05:42:00.0000000Z"
std::string trimmed = dt1.toString(DateTime::Format::Iso8601PreciseTrimmed);   // "2025-01-24T05:42:00.0Z" (trailing zeros removed)
std::string millis = dt1.toString(DateTime::Format::Iso8601Millis);            // "2025-01-24T05:42:00.000Z" (3-digit fixed)
std::string micros = dt1.toString(DateTime::Format::Iso8601Micros);            // "2025-01-24T05:42:00.000000Z" (6-digit fixed)
std::string extended = dt1.toString(DateTime::Format::Iso8601Extended);        // "2025-01-24T05:42:00+00:00"
std::string basic = dt1.toString(DateTime::Format::Iso8601Basic);              // "20250124T054200Z"

// Epoch timestamp conversions
std::int64_t epochSeconds = dt1.toEpochSeconds();
std::int64_t epochMillis = dt1.toEpochMilliseconds();
DateTime fromEpoch = DateTime::fromEpochSeconds(epochSeconds);

// Component access
int year = dt1.year();
int month = dt1.month();
int day = dt1.day();
int hour = dt1.hour();
int minute = dt1.minute();
int second = dt1.second();
```

### DateTimeOffset - Timezone-Aware Operations

```cpp
#include <nfx/datetime/DateTimeOffset.h>

using namespace nfx::time;

// Current time with local offset
DateTimeOffset now = DateTimeOffset::now();

// Construction with offset
TimeSpan offset = TimeSpan::fromHours(2);  // +02:00
DateTimeOffset dto1(2025, 1, 24, 5, 42, 0, offset);

// ISO 8601 parsing with offset (using constructor - throws on error)
DateTimeOffset dto2("2025-01-24T05:42:00+02:00");

// ISO 8601 parsing (safe - returns std::optional)
auto dto3 = DateTimeOffset::fromString("2025-01-24T05:42:00+02:00");
if (dto3) {
    // Use *dto3
}

// ISO 8601 parsing (safe - returns bool)
DateTimeOffset dto4;
if (DateTimeOffset::fromString("2025-01-24T05:42:00+02:00", dto4)) {
    // Use dto4
}

// Convert to different timezone
DateTimeOffset utc = dto1.toUniversalTime();
DateTimeOffset newYork = dto1.toOffset(TimeSpan::fromHours(-5));

// Get underlying UTC datetime
DateTime utcDateTime = dto1.utcDateTime();

// Epoch timestamp conversions
std::int64_t epochSeconds = dto1.toEpochSeconds();
std::int64_t epochMillis = dto1.toEpochMilliseconds();
DateTimeOffset fromEpoch = DateTimeOffset::fromEpochSeconds(epochSeconds);

// Formatting
std::string iso = dto1.toString();                                              // "2025-01-24T05:42:00+02:00"
std::string precise = dto1.toString(DateTime::Format::Iso8601Precise);          // "2025-01-24T05:42:00.0000000+02:00"
std::string trimmed = dto1.toString(DateTime::Format::Iso8601PreciseTrimmed);   // "2025-01-24T05:42:00.0+02:00" (trailing zeros removed)
std::string millis = dto1.toString(DateTime::Format::Iso8601Millis);            // "2025-01-24T05:42:00.000+02:00" (3-digit fixed)
std::string micros = dto1.toString(DateTime::Format::Iso8601Micros);            // "2025-01-24T05:42:00.000000+02:00" (6-digit fixed)
std::string extended = dto1.toString(DateTime::Format::Iso8601Extended);        // "2025-01-24T05:42:00+02:00" (same as default)
std::string basic = dto1.toString(DateTime::Format::Iso8601Basic);              // "20250124T054200+0200"
```

### TimeSpan - Duration Calculations

```cpp
#include <nfx/datetime/TimeSpan.h>

using namespace nfx::time;

// Construction from various units
TimeSpan ts1 = TimeSpan::fromHours(2);
TimeSpan ts2 = TimeSpan::fromMinutes(30);
TimeSpan ts3 = TimeSpan::fromSeconds(45);
TimeSpan ts4 = TimeSpan::fromMilliseconds(500);

// Using user-defined literals
using namespace nfx::time::literals;
TimeSpan ts5 = 2_h + 30_min + 45_s;  // 2 hours, 30 minutes, 45 seconds

// ISO 8601 duration parsing (using constructor - throws on error)
TimeSpan ts6("PT2H30M");  // 2 hours 30 minutes

// ISO 8601 duration parsing (safe - returns std::optional)
auto ts7 = TimeSpan::fromString("PT2H30M45S");
if (ts7) {
    // Use *ts7
}

// ISO 8601 duration parsing (safe - returns bool)
TimeSpan ts8;
if (TimeSpan::fromString("PT2H30M", ts8)) {
    // Use ts8
}

// Arithmetic
TimeSpan total = ts1 + ts2;  // 2.5 hours
TimeSpan diff = ts1 - ts2;   // 1.5 hours
TimeSpan doubled = ts1 * 2;  // 4 hours
TimeSpan half = ts1 / 2;     // 1 hour
double ratio = ts1 / ts2;    // 4.0 (ts1 is 4x ts2)

// Conversions
double hours = total.hours();         // 2.5
double minutes = total.minutes();     // 150
double seconds = total.seconds();     // 9000
std::int64_t ticks = total.ticks();   // 100-nanosecond units

// Component access
std::int32_t h = total.hours();            // 2
std::int32_t m = total.minutes();          // 30
std::int32_t s = total.seconds();          // 0

// Formatting
std::string iso = total.toString();  // "PT2H30M"

// std::chrono interoperability
auto chronoDuration = total.toChrono();
TimeSpan fromChrono = TimeSpan::fromChrono(std::chrono::hours(2));
```

### Complete Example

```cpp
#include <iostream>
#include <nfx/datetime/DateTime.h>
#include <nfx/datetime/DateTimeOffset.h>
#include <nfx/datetime/TimeSpan.h>

int main()
{
    using namespace nfx::time;

    // Get current UTC time
    DateTime now = DateTime::utcNow();
    std::cout << "Current UTC time: " << now.toString(DateTime::Format::Iso8601Precise) << std::endl;

    // Calculate future date using TimeSpan
    TimeSpan oneWeek = TimeSpan::fromDays(7);
    DateTime nextWeek = now + oneWeek;
    std::cout << "One week from now: " << nextWeek.toString(DateTime::Format::Iso8601Precise) << std::endl;

    // Work with timezones
    DateTimeOffset localNow = DateTimeOffset::now();
    std::cout << "Local time: " << localNow.toString() << std::endl;

    DateTimeOffset utcNow = DateTimeOffset::utcNow();
    std::cout << "UTC time with offset: " << utcNow.toString() << std::endl;

    // Calculate duration between dates
    TimeSpan duration = nextWeek - now;
    std::cout << "Duration (ISO 8601): " << duration.toString() << std::endl;
    std::cout << "Duration in hours: " << duration.hours() << std::endl;
    std::cout << "Duration in days: " << duration.days() << std::endl;

    // Parse ISO 8601 strings
    auto parsedDate = DateTime::fromString("2025-12-31T23:59:59Z");
    if (parsedDate) {
        std::cout << "New Year's Eve: " << parsedDate->toString() << std::endl;
    }

    // Epoch timestamp conversions
    std::int64_t epochSeconds = now.toEpochSeconds();
    DateTime fromEpoch = DateTime::fromEpochSeconds(epochSeconds);
    std::cout << "Epoch timestamp: " << epochSeconds << std::endl;
    std::cout << "From epoch: " << fromEpoch.toString() << std::endl;

    // Component access
    std::cout << "Year: " << now.year() << ", Month: " << now.month()
              << ", Day: " << now.day() << std::endl;

    return 0;
}
```

**Sample Output:**

```
Current UTC time: 2025-11-16T12:42:36.8324975Z
One week from now: 2025-11-23T12:42:36.8324975Z
Local time: 2025-11-16T13:42:36+01:00
UTC time with offset: 2025-11-16T12:42:36+00:00
Duration (ISO 8601): P7D
Duration in hours: 168
Duration in days: 7
New Year's Eve: 2025-12-31T23:59:59Z
Epoch timestamp: 1763258556
From epoch: 2025-11-16T12:42:36Z
Year: 2025, Month: 11, Day: 16
```

## Installation & Packaging

nfx-datetime provides packaging options for distribution.

### Package Generation

```bash
# Configure with packaging options
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DNFX_DATETIME_BUILD_STATIC=ON \
         -DNFX_DATETIME_BUILD_SHARED=ON \
         -DNFX_DATETIME_PACKAGE_ARCHIVE=ON \
         -DNFX_DATETIME_PACKAGE_DEB=ON \
         -DNFX_DATETIME_PACKAGE_RPM=ON

# Generate binary packages
cmake --build . --target package
# or
cd build && cpack

# Generate source packages
cd build && cpack --config CPackSourceConfig.cmake
```

### Supported Package Formats

| Format      | Platform       | Description                        | Requirements |
| ----------- | -------------- | ---------------------------------- | ------------ |
| **TGZ/ZIP** | Cross-platform | Compressed archive packages        | None         |
| **DEB**     | Debian/Ubuntu  | Native Debian packages             | `dpkg-dev`   |
| **RPM**     | RedHat/SUSE    | Native RPM packages                | `rpm-build`  |
| **WiX**     | Windows        | Professional MSI installer         | `WiX 3.11+`  |
| **Source**  | Cross-platform | Source code distribution (TGZ+ZIP) | None         |

### Installation

```bash
# Linux (DEB-based systems)
sudo dpkg -i nfx-datetime_*_amd64.deb

# Linux (RPM-based systems)
sudo rpm -ivh nfx-datetime-*-Linux.rpm

# Windows (MSI installer)
nfx-datetime-0.1.0-MSVC.msi

# Manual installation (extract archive)
tar -xzf nfx-datetime-*-Linux.tar.gz -C /usr/local/
```

## Project Structure

```
nfx-datetime/
├── benchmark/                   # Performance benchmarks
├── cmake/                       # CMake modules and configuration
├── include/nfx/                 # Public headers
│   ├── DateTime.h               # Main umbrella header (includes all)
│   ├── datetime/                # Core datetime classes
│   │   ├── DateTime.h           # UTC datetime with 100ns precision
│   │   ├── DateTimeOffset.h     # Timezone-aware datetime
│   │   └── TimeSpan.h           # Duration/interval representation
│   └── detail/datetime/         # Inline implementation details
├── samples/                     # Example usage and demonstrations
├── src/                         # Implementation files
└── test/                        # Unit tests
```

## Performance

For detailed performance metrics and benchmarks, see the [benchmark documentation](benchmark/README.md).

## Roadmap

See [TODO.md](TODO.md) for upcoming features and project roadmap.

## Changelog

See the [CHANGELOG.md](CHANGELOG.md) for a detailed history of changes, new features, and bug fixes.

## License

This project is licensed under the MIT License.

## Acknowledgments

- Inspired by .NET's DateTime API design

## Dependencies

### Runtime Dependencies

- **[nfx-stringbuilder](https://github.com/nfx-libs/nfx-stringbuilder)**: High-performance string builder (MIT License)

### Development Dependencies

- **[GoogleTest](https://github.com/google/googletest)**: Testing framework (BSD 3-Clause License) - Development only
- **[Google Benchmark](https://github.com/google/benchmark)**: Performance benchmarking framework (Apache 2.0 License) - Development only

All dependencies are automatically fetched via CMake FetchContent when building the library, tests, or benchmarks.

---

_Updated on February 03, 2026_
