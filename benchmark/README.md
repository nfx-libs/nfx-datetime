# Benchmarks

---

## Test Environment

### Hardware Configuration

| Component                | Specification                                                 |
| ------------------------ | ------------------------------------------------------------- |
| **Computer Model**       | Lenovo ThinkPad P15v Gen 3                                    |
| **CPU**                  | 12th Gen Intel Core i7-12800H (20 logical, 14 physical cores) |
| **Base Clock**           | 2.80 GHz                                                      |
| **Turbo Clock**          | 4.80 GHz                                                      |
| **L1 Data Cache**        | 48 KiB (×6 P-cores) + 32 KiB (×8 E-cores)                     |
| **L1 Instruction Cache** | 32 KiB (×6 P-cores) + 64 KiB (×2 E-core clusters)             |
| **L2 Unified Cache**     | 1.25 MiB (×6 P-cores) + 2 MiB (×2 E-core clusters)            |
| **L3 Unified Cache**     | 24 MiB (×1 shared)                                            |
| **RAM**                  | DDR4-3200 (32GB)                                              |
| **GPU**                  | NVIDIA RTX A2000 4GB GDDR6                                    |

### Software Configuration

| Platform    | OS         | Benchmark Framework     | C++ Compiler           | nfx-datetime Version |
| ----------- | ---------- | ----------------------- | ---------------------- | :------------------: |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | Clang 19.1.7-x64       |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64   |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64 |        v0.3.0        |

---

# Performance Results

## DateTime Operations

| Operation                      | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------ | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct from Y-M-D**       | **9.16 ns**  | 11.8 ns      | 15.0 ns           | 9.80 ns               | 13.5 ns                | 19.5 ns      |
| **Construct from Y-M-D-H-M-S** | **9.77 ns**  | 10.1 ns      | 15.5 ns           | 10.5 ns               | 13.8 ns                | 19.2 ns      |
| **DateTime::now()**            | **36.0 ns**  | 40.7 ns      | 131 ns            | 78.8 ns               | 94.6 ns                | 127 ns       |
| **DateTime::utcNow()**         | **13.0 ns**  | 13.8 ns      | 57.8 ns           | 18.2 ns               | 26.0 ns                | 19.9 ns      |
| **Parse ISO 8601 Basic**       | 50.0 ns      | **45.9 ns**  | 76.5 ns           | 49.4 ns               | 64.0 ns                | 77.4 ns      |
| **Parse ISO 8601 Extended**    | 65.1 ns      | 56.7 ns      | 97.7 ns           | **56.2 ns**           | 74.3 ns                | 90.0 ns      |
| **ToString ISO 8601**          | 265 ns       | **256 ns**   | 510 ns            | 1270 ns               | 1521 ns                | 1381 ns      |
| **toIso8601Extended**          | 369 ns       | **361 ns**   | 695 ns            | 1672 ns               | 2079 ns                | 1925 ns      |
| **Add TimeSpan**               | **0.105 ns** | 0.210 ns     | 0.180 ns          | 0.217 ns              | 0.332 ns               | 0.711 ns     |
| **Subtract TimeSpan**          | **0.105 ns** | 0.209 ns     | 0.176 ns          | 0.247 ns              | 0.274 ns               | 0.711 ns     |
| **Subtract DateTime**          | **0.213 ns** | 0.218 ns     | 0.347 ns          | 0.225 ns              | 0.274 ns               | 0.698 ns     |
| **ToEpochSeconds**             | **0.105 ns** | 0.210 ns     | 0.169 ns          | 0.448 ns              | 0.529 ns               | 0.872 ns     |
| **FromEpochSeconds**           | **0.105 ns** | 0.210 ns     | 0.168 ns          | 0.211 ns              | 0.262 ns               | 0.698 ns     |
| **GetComponents**              | **21.1 ns**  | 25.8 ns      | 29.5 ns           | 26.1 ns               | 29.9 ns                | 76.7 ns      |
| **Comparison**                 | 0.335 ns     | **0.320 ns** | 0.504 ns          | 0.334 ns              | 0.378 ns               | 0.698 ns     |

---

## DateTimeOffset Operations

| Operation                       | Linux GCC    | Linux Clang  | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------- | ------------ | ------------ | ----------------- | --------------------- | ---------------------- | ------------ |
| **Construct with Offset**       | 9.63 ns      | 9.92 ns      | **9.54 ns**       | 10.8 ns               | 13.5 ns                | 19.5 ns      |
| **DateTimeOffset::now()**       | **34.5 ns**  | 40.6 ns      | 118 ns            | 81.6 ns               | 81.7 ns                | 115 ns       |
| **Parse with Offset**           | 66.2 ns      | **59.4 ns**  | 66.3 ns           | 72.3 ns               | 75.2 ns                | 103 ns       |
| **Parse UTC**                   | 52.3 ns      | **44.8 ns**  | 53.7 ns           | 56.4 ns               | 60.2 ns                | 90.0 ns      |
| **ToUTC**                       | 0.695 ns     | **0.465 ns** | 0.468 ns          | 0.470 ns              | 0.507 ns               | 0.952 ns     |
| **ToOffset**                    | 0.933 ns     | **0.489 ns** | 0.705 ns          | 0.556 ns              | 0.704 ns               | 0.963 ns     |
| **GetUTCDateTime**              | **0.296 ns** | 0.442 ns     | 0.305 ns          | 0.464 ns              | 0.467 ns               | 0.984 ns     |
| **ToString**                    | 293 ns       | **282 ns**   | 416 ns            | 1603 ns               | 1644 ns                | 1800 ns      |
| **Add TimeSpan**                | 0.212 ns     | **0.209 ns** | 0.211 ns          | 0.227 ns              | 0.235 ns               | 0.715 ns     |
| **Subtract DateTimeOffset**     | **0.223 ns** | 0.282 ns     | 0.224 ns          | 0.350 ns              | 0.348 ns               | 0.732 ns     |
| **Comparison_DifferentOffsets** | **0.305 ns** | 0.369 ns     | 0.331 ns          | 0.411 ns              | 0.395 ns               | 0.750 ns     |

---

## TimeSpan Operations

| Operation                    | Linux GCC    | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------- | ------------ | ----------- | ----------------- | --------------------- | ---------------------- | ------------ |
| **FromHours**                | **0.106 ns** | 0.210 ns    | 0.110 ns          | 0.222 ns              | 0.209 ns               | 0.785 ns     |
| **FromMinutes**              | **0.105 ns** | 0.211 ns    | 0.108 ns          | 0.218 ns              | 0.210 ns               | 0.785 ns     |
| **FromSeconds**              | **0.105 ns** | 0.212 ns    | 0.111 ns          | 0.216 ns              | 0.220 ns               | 0.785 ns     |
| **FromMilliseconds**         | **0.105 ns** | 0.210 ns    | 0.109 ns          | 0.215 ns              | 0.218 ns               | 0.767 ns     |
| **Parse ISO 8601 Simple**    | 41.1 ns      | **37.5 ns** | 59.7 ns           | 81.6 ns               | 83.7 ns                | 128 ns       |
| **Parse ISO 8601 with Days** | 39.8 ns      | **36.8 ns** | 58.2 ns           | 77.6 ns               | 76.7 ns                | 117 ns       |
| **Parse Numeric**            | 22.7 ns      | **13.1 ns** | 33.3 ns           | 46.4 ns               | 46.0 ns                | 62.8 ns      |
| **ToString ISO 8601**        | **142 ns**   | 147 ns      | 176 ns            | 757 ns                | 750 ns                 | 858 ns       |
| **Add**                      | **0.105 ns** | 0.212 ns    | 0.107 ns          | 0.217 ns              | 0.220 ns               | 0.785 ns     |
| **Subtract**                 | **0.105 ns** | 0.214 ns    | 0.109 ns          | 0.218 ns              | 0.215 ns               | 0.767 ns     |
| **Negate**                   | **0.105 ns** | 0.213 ns    | 0.111 ns          | 0.218 ns              | 0.220 ns               | 0.785 ns     |
| **TotalHours**               | **0.105 ns** | 0.214 ns    | 0.111 ns          | 0.219 ns              | 0.220 ns               | 0.774 ns     |
| **TotalSeconds**             | **0.105 ns** | 0.211 ns    | 0.111 ns          | 0.218 ns              | 0.215 ns               | 0.785 ns     |
| **TotalMilliseconds**        | **0.105 ns** | 0.210 ns    | 0.110 ns          | 0.218 ns              | 0.254 ns               | 0.785 ns     |
| **Comparison**               | **0.105 ns** | 0.210 ns    | 0.110 ns          | 0.216 ns              | 0.225 ns               | 0.785 ns     |

---

_Benchmarks executed on November 17, 2025_
