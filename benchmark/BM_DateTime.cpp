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
 * @file BM_DateTime.cpp
 * @brief Benchmark DateTime parsing, formatting, and arithmetic operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datetime/DateTime.h>

namespace nfx::time::benchmark
{
    //=====================================================================
    // DateTime benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    static void BM_DateTime_Construct_YMD( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto dt{ DateTime{ 2024, 10, 23 } };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    static void BM_DateTime_Construct_YMDHMS( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto dt{ DateTime{ 2024, 10, 23, 15, 30, 45 } };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    static void BM_DateTime_Now( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto dt{ DateTime::now() };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    static void BM_DateTime_UtcNow( ::benchmark::State& state )
    {
        for ( auto _ : state )
        {
            auto dt{ DateTime::utcNow() };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    static void BM_DateTime_Parse( ::benchmark::State& state )
    {
        const std::string iso{ "2024-10-23T15:30:45Z" };

        for ( auto _ : state )
        {
            auto dt{ DateTime{ iso } };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    static void BM_DateTime_ParseExtended( ::benchmark::State& state )
    {
        const std::string iso{ "2024-10-23T15:30:45.1234567Z" };

        for ( auto _ : state )
        {
            auto dt{ DateTime{ iso } };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_DateTime_ToString_ISO8601( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };

        for ( auto _ : state )
        {
            auto str{ dt.toString() };
            ::benchmark::DoNotOptimize( str );
        }
    }

    static void BM_DateTime_toIso8601Precise( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };

        for ( auto _ : state )
        {
            auto str{ dt.toString( DateTime::Format::Iso8601Precise ) };
            ::benchmark::DoNotOptimize( str );
        }
    }

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    static void BM_DateTime_Add_TimeSpan( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };
        auto ts{ TimeSpan::fromHours( 24 ) };

        for ( auto _ : state )
        {
            auto result{ dt + ts };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DateTime_Subtract_TimeSpan( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };
        auto ts{ TimeSpan::fromHours( 24 ) };

        for ( auto _ : state )
        {
            auto result{ dt - ts };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DateTime_Subtract_DateTime( ::benchmark::State& state )
    {
        auto dt1{ DateTime{ 2024, 10, 23, 15, 0, 0 } };
        auto dt2{ DateTime{ 2024, 1, 1 } };

        for ( auto _ : state )
        {
            auto result{ dt1 - dt2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    static void BM_DateTime_ToEpochSeconds( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };

        for ( auto _ : state )
        {
            auto epochSecs{ dt.toEpochSeconds() };
            ::benchmark::DoNotOptimize( epochSecs );
        }
    }

    static void BM_DateTime_FromEpochSeconds( ::benchmark::State& state )
    {
        std::int64_t epochSecs{ 1729699845 };

        for ( auto _ : state )
        {
            auto dt{ DateTime::fromEpochSeconds( epochSecs ) };
            ::benchmark::DoNotOptimize( dt );
        }
    }

    //----------------------------------------------
    // Component extraction
    //----------------------------------------------

    static void BM_DateTime_GetComponents( ::benchmark::State& state )
    {
        auto dt{ DateTime::utcNow() };

        for ( auto _ : state )
        {
            auto y{ dt.year() };
            auto m{ dt.month() };
            auto d{ dt.day() };
            auto h{ dt.hour() };
            auto min{ dt.minute() };
            auto s{ dt.second() };
            ::benchmark::DoNotOptimize( y );
            ::benchmark::DoNotOptimize( m );
            ::benchmark::DoNotOptimize( d );
            ::benchmark::DoNotOptimize( h );
            ::benchmark::DoNotOptimize( min );
            ::benchmark::DoNotOptimize( s );
        }
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    static void BM_DateTime_Comparison( ::benchmark::State& state )
    {
        auto dt1{ DateTime{ 2024, 10, 23, 15, 0, 0 } };
        auto dt2{ DateTime{ 2024, 10, 23 } };

        for ( auto _ : state )
        {
            bool result{ dt1 > dt2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //=====================================================================
    // Benchmarks registration
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    BENCHMARK( BM_DateTime_Construct_YMD );
    BENCHMARK( BM_DateTime_Construct_YMDHMS );
    BENCHMARK( BM_DateTime_Now );
    BENCHMARK( BM_DateTime_UtcNow );

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    BENCHMARK( BM_DateTime_Parse );
    BENCHMARK( BM_DateTime_ParseExtended );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_DateTime_ToString_ISO8601 );
    BENCHMARK( BM_DateTime_toIso8601Precise );

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    BENCHMARK( BM_DateTime_Add_TimeSpan );
    BENCHMARK( BM_DateTime_Subtract_TimeSpan );
    BENCHMARK( BM_DateTime_Subtract_DateTime );

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    BENCHMARK( BM_DateTime_ToEpochSeconds );
    BENCHMARK( BM_DateTime_FromEpochSeconds );

    //----------------------------------------------
    // Component extraction
    //----------------------------------------------

    BENCHMARK( BM_DateTime_GetComponents );

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    BENCHMARK( BM_DateTime_Comparison );
} // namespace nfx::time::benchmark

BENCHMARK_MAIN();
