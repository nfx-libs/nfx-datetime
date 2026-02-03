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
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | GCC 14.2.0-x64         |        v0.3.0        |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | Clang 19.1.7-x64       |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MinGW GCC 14.2.0-x64   |        v0.3.0        |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MSVC 19.44.35217.0-x64 |        v0.3.0        |

---

# Performance Results

## DateTime Operations

| Operation                      | Linux GCC | Linux Clang | Windows MinGW | Windows MSVC |
| ------------------------------ | --------: | ----------: | ------------: | -----------: |
| **Construct from Y-M-D**       |   10.5 ns |     11.2 ns |       11.1 ns |      22.7 ns |
| **Construct from Y-M-D-H-M-S** |   10.6 ns |     13.6 ns |       11.5 ns |      21.8 ns |
| **DateTime::now()**            |   36.4 ns |     41.2 ns |       48.9 ns |      51.7 ns |
| **DateTime::utcNow()**         |   14.3 ns |     15.0 ns |       35.6 ns |      19.1 ns |
| **Parse ISO 8601 Basic**       |   16.4 ns |     15.8 ns |       17.1 ns |      28.2 ns |
| **Parse ISO 8601 Precise**     |   21.5 ns |     19.8 ns |       24.7 ns |      34.6 ns |
| **ToString ISO 8601**          |   37.6 ns |     35.0 ns |       55.5 ns |      72.4 ns |
| **toIso8601Extended**          |   43.0 ns |     39.1 ns |       62.4 ns |      81.6 ns |
| **Add TimeSpan**               |  0.108 ns |    0.224 ns |      0.109 ns |     0.737 ns |
| **Subtract TimeSpan**          |  0.111 ns |    0.224 ns |      0.110 ns |     0.738 ns |
| **Subtract DateTime**          |  0.233 ns |    0.267 ns |      0.233 ns |     0.769 ns |
| **ToEpochSeconds**             |  0.106 ns |    0.222 ns |      0.111 ns |     0.943 ns |
| **FromEpochSeconds**           |  0.107 ns |    0.216 ns |      0.110 ns |     0.729 ns |
| **GetComponents**              |   20.2 ns |     24.3 ns |       20.2 ns |      37.9 ns |
| **Comparison**                 |  0.423 ns |    0.354 ns |      0.368 ns |     0.774 ns |

---

## DateTimeOffset Operations

| Operation                       | Linux GCC | Linux Clang | Windows MinGW | Windows MSVC |
| ------------------------------- | --------: | ----------: | ------------: | -----------: |
| **Construct with Offset**       |   11.0 ns |     11.0 ns |       11.0 ns |      20.5 ns |
| **DateTimeOffset::now()**       |   36.3 ns |     41.5 ns |       56.1 ns |      57.5 ns |
| **Parse with Offset**           |   20.0 ns |     20.0 ns |       20.2 ns |      34.7 ns |
| **Parse UTC**                   |   17.7 ns |     19.1 ns |       17.9 ns |      33.6 ns |
| **ToUTC**                       |  0.752 ns |    0.509 ns |      0.499 ns |      1.00 ns |
| **ToOffset**                    |   1.01 ns |    0.527 ns |      0.655 ns |      1.05 ns |
| **GetUTCDateTime**              |  0.307 ns |    0.491 ns |      0.307 ns |      1.01 ns |
| **ToString**                    |   50.3 ns |     55.7 ns |       71.6 ns |      97.6 ns |
| **Add TimeSpan**                |  0.220 ns |    0.222 ns |      0.230 ns |     0.758 ns |
| **Subtract DateTimeOffset**     |  0.239 ns |    0.297 ns |      0.296 ns |     0.807 ns |
| **Comparison_DifferentOffsets** |  0.325 ns |    0.397 ns |      0.341 ns |     0.796 ns |

---

## TimeSpan Operations

| Operation                    | Linux GCC | Linux Clang | Windows MinGW | Windows MSVC |
| ---------------------------- | --------: | ----------: | ------------: | -----------: |
| **FromHours**                |  0.108 ns |    0.219 ns |      0.109 ns |     0.744 ns |
| **FromMinutes**              |  0.107 ns |    0.230 ns |      0.110 ns |     0.738 ns |
| **FromSeconds**              |  0.107 ns |    0.223 ns |      0.108 ns |     0.728 ns |
| **FromMilliseconds**         |  0.107 ns |    0.229 ns |      0.112 ns |     0.725 ns |
| **Parse ISO 8601 Simple**    |   26.2 ns |     26.9 ns |       28.2 ns |       144 ns |
| **Parse ISO 8601 with Days** |   46.6 ns |     43.5 ns |       63.8 ns |       138 ns |
| **Parse Numeric**            |   26.5 ns |     15.5 ns |       37.5 ns |      83.7 ns |
| **ToString ISO 8601**        |   13.3 ns |     12.0 ns |       15.9 ns |      30.2 ns |
| **Add**                      |  0.110 ns |    0.228 ns |      0.111 ns |     0.729 ns |
| **Subtract**                 |  0.110 ns |    0.231 ns |      0.107 ns |     0.874 ns |
| **Negate**                   |  0.108 ns |    0.223 ns |      0.110 ns |     0.769 ns |
| **TotalHours**               |  0.107 ns |    0.225 ns |      0.109 ns |     0.759 ns |
| **TotalSeconds**             |  0.108 ns |    0.225 ns |      0.109 ns |     0.736 ns |
| **TotalMilliseconds**        |  0.120 ns |    0.232 ns |      0.109 ns |     0.761 ns |
| **Comparison**               |  0.112 ns |    0.231 ns |      0.108 ns |     0.735 ns |

---

_Updated on February 03, 2026_

