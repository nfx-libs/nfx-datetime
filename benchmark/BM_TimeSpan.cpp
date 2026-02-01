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
 * @file BM_TimeSpan.cpp
 * @brief Benchmark TimeSpan parsing, formatting, and arithmetic operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datetime/TimeSpan.h>

namespace nfx::time::benchmark
{
    //=====================================================================
    // TimeSpan benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    static void BM_TimeSpan_FromHours( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            auto ts{ TimeSpan::fromHours( 24.5 ) };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    static void BM_TimeSpan_FromMinutes( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            auto ts{ TimeSpan::fromMinutes( 90 ) };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    static void BM_TimeSpan_FromSeconds( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            auto ts{ TimeSpan::fromSeconds( 3661.5 ) };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    static void BM_TimeSpan_FromMilliseconds( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            auto ts{ TimeSpan::fromMilliseconds( 123456.789 ) };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    static void BM_TimeSpan_ParseISO( ::benchmark::State& state )
    {
        const std::string duration{ "PT1H30M45S" };

        for( auto _ : state )
        {
            auto ts{ TimeSpan{ duration } };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    static void BM_TimeSpan_ParseISOComplex( ::benchmark::State& state )
    {
        const std::string duration{ "P5DT12H30M" };

        for( auto _ : state )
        {
            auto ts{ TimeSpan{ duration } };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    static void BM_TimeSpan_Parse( ::benchmark::State& state )
    {
        const std::string duration{ "3600.5" };

        for( auto _ : state )
        {
            auto ts{ TimeSpan{ duration } };
            ::benchmark::DoNotOptimize( ts );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_TimeSpan_ToString_ISO8601( ::benchmark::State& state )
    {
        auto ts{ TimeSpan::fromHours( 25.5 ) };

        for( auto _ : state )
        {
            auto str{ ts.toString() };
            ::benchmark::DoNotOptimize( str );
        }
    }

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    static void BM_TimeSpan_Add( ::benchmark::State& state )
    {
        auto ts1{ TimeSpan::fromHours( 1 ) };
        auto ts2{ TimeSpan::fromMinutes( 30 ) };
        for( auto _ : state )
        {
            auto result{ ts1 + ts2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_TimeSpan_Subtract( ::benchmark::State& state )
    {
        auto ts1{ TimeSpan::fromHours( 5 ) };
        auto ts2{ TimeSpan::fromMinutes( 30 ) };
        for( auto _ : state )
        {
            auto result{ ts1 - ts2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_TimeSpan_Negate( ::benchmark::State& state )
    {
        auto ts{ TimeSpan::fromHours( 5 ) };

        for( auto _ : state )
        {
            auto result{ -ts };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    static void BM_TimeSpan_TotalHours( ::benchmark::State& state )
    {
        auto ts{ TimeSpan::fromMinutes( 150 ) };

        for( auto _ : state )
        {
            auto hours{ ts.hours() };
            ::benchmark::DoNotOptimize( hours );
        }
    }

    static void BM_TimeSpan_TotalSeconds( ::benchmark::State& state )
    {
        auto ts{ TimeSpan::fromHours( 2.5 ) };

        for( auto _ : state )
        {
            auto seconds{ ts.seconds() };
            ::benchmark::DoNotOptimize( seconds );
        }
    }

    static void BM_TimeSpan_TotalMilliseconds( ::benchmark::State& state )
    {
        auto ts{ TimeSpan::fromSeconds( 1.5 ) };

        for( auto _ : state )
        {
            auto ms{ ts.milliseconds() };
            ::benchmark::DoNotOptimize( ms );
        }
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    static void BM_TimeSpan_Comparison( ::benchmark::State& state )
    {
        auto ts1{ TimeSpan::fromHours( 2 ) };
        auto ts2{ TimeSpan::fromMinutes( 90 ) };
        for( auto _ : state )
        {
            bool result{ ts1 > ts2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //=====================================================================
    // Benchmarks registration
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_FromHours );
    BENCHMARK( BM_TimeSpan_FromMinutes );
    BENCHMARK( BM_TimeSpan_FromSeconds );
    BENCHMARK( BM_TimeSpan_FromMilliseconds );

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_ParseISO );
    BENCHMARK( BM_TimeSpan_ParseISOComplex );
    BENCHMARK( BM_TimeSpan_Parse );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_ToString_ISO8601 );

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_Add );
    BENCHMARK( BM_TimeSpan_Subtract );
    BENCHMARK( BM_TimeSpan_Negate );

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_TotalHours );
    BENCHMARK( BM_TimeSpan_TotalSeconds );
    BENCHMARK( BM_TimeSpan_TotalMilliseconds );

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    BENCHMARK( BM_TimeSpan_Comparison );
} // namespace nfx::time::benchmark

BENCHMARK_MAIN();
