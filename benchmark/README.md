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
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | Clang 19.1.7-x64       |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MinGW GCC 14.2.0-x64   |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MSVC 19.44.35217.0-x64 |        v0.3.0        |

---

# Performance Results

## DateTime Operations

| Operation                      | Linux Clang | Windows MSVC |
| ------------------------------ | ----------: | -----------: |
| **Construct from Y-M-D**       |     11.9 ns |      21.4 ns |
| **Construct from Y-M-D-H-M-S** |     14.3 ns |      21.6 ns |
| **DateTime::now()**            |     43.4 ns |      52.9 ns |
| **DateTime::utcNow()**         |     16.0 ns |      20.2 ns |
| **Parse ISO 8601 Basic**       |     49.4 ns |      74.2 ns |
| **Parse ISO 8601 Extended**    |     61.9 ns |      82.3 ns |
| **ToString ISO 8601**          |     38.1 ns |      68.9 ns |
| **toIso8601Extended**          |     40.6 ns |      77.1 ns |
| **Add TimeSpan**               |    0.269 ns |     0.755 ns |
| **Subtract TimeSpan**          |    0.266 ns |     0.761 ns |
| **Subtract DateTime**          |    0.286 ns |     0.761 ns |
| **ToEpochSeconds**             |    0.256 ns |     0.945 ns |
| **FromEpochSeconds**           |    0.268 ns |     0.728 ns |
| **GetComponents**              |     28.0 ns |      36.7 ns |
| **Comparison**                 |    0.410 ns |     0.789 ns |

---

## DateTimeOffset Operations

| Operation                       | Linux Clang | Windows MSVC |
| ------------------------------- | ----------: | -----------: |
| **Construct with Offset**       |     11.8 ns |      20.7 ns |
| **DateTimeOffset::now()**       |     46.7 ns |      56.1 ns |
| **Parse with Offset**           |     83.3 ns |      91.5 ns |
| **Parse UTC**                   |     64.0 ns |      75.1 ns |
| **ToUTC**                       |    0.532 ns |      1.00 ns |
| **ToOffset**                    |    0.565 ns |      1.03 ns |
| **GetUTCDateTime**              |    0.527 ns |      1.01 ns |
| **ToString**                    |     59.1 ns |       102 ns |
| **Add TimeSpan**                |    0.269 ns |     0.784 ns |
| **Subtract DateTimeOffset**     |    0.325 ns |     0.808 ns |
| **Comparison_DifferentOffsets** |    0.426 ns |     0.801 ns |

---

## TimeSpan Operations

| Operation                    | Linux Clang | Windows MSVC |
| ---------------------------- | ----------: | -----------: |
| **FromHours**                |    0.222 ns |     0.744 ns |
| **FromMinutes**              |    0.221 ns |     0.738 ns |
| **FromSeconds**              |    0.222 ns |     0.728 ns |
| **FromMilliseconds**         |    0.228 ns |     0.725 ns |
| **Parse ISO 8601 Simple**    |     45.5 ns |       144 ns |
| **Parse ISO 8601 with Days** |     44.7 ns |       138 ns |
| **Parse Numeric**            |     15.5 ns |      83.7 ns |
| **ToString ISO 8601**        |     12.2 ns |      30.2 ns |
| **Add**                      |    0.225 ns |     0.729 ns |
| **Subtract**                 |    0.226 ns |     0.874 ns |
| **Negate**                   |    0.226 ns |     0.769 ns |
| **TotalHours**               |    0.227 ns |     0.759 ns |
| **TotalSeconds**             |    0.228 ns |     0.736 ns |
| **TotalMilliseconds**        |    0.226 ns |     0.761 ns |
| **Comparison**               |    0.227 ns |     0.735 ns |

---

_Updated on Frebruary 02, 2026_

