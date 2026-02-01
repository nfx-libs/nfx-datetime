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
 * @file BM_DateTimeOffset.cpp
 * @brief Benchmark DateTimeOffset parsing, formatting, and timezone operations
 */

#include <benchmark/benchmark.h>

#include <nfx/datetime/DateTimeOffset.h>

namespace nfx::time::benchmark
{
    //=====================================================================
    // DateTimeOffset benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    static void BM_DateTimeOffset_Construct( ::benchmark::State& state )
    {
        auto offset{ TimeSpan::fromHours( 5.5 ) };

        for( auto _ : state )
        {
            auto dto{ DateTimeOffset{ 2024, 10, 23, 15, 30, 45, offset } };
            ::benchmark::DoNotOptimize( dto );
        }
    }

    static void BM_DateTimeOffset_Now( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            auto dto{ DateTimeOffset::now() };
            ::benchmark::DoNotOptimize( dto );
        }
    }

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    static void BM_DateTimeOffset_Parse( ::benchmark::State& state )
    {
        const std::string iso{ "2024-10-23T15:30:45+05:30" };

        for( auto _ : state )
        {
            auto dto{ DateTimeOffset{ iso } };
            ::benchmark::DoNotOptimize( dto );
        }
    }

    static void BM_DateTimeOffset_ParseZ( ::benchmark::State& state )
    {
        const std::string iso{ "2024-10-23T15:30:45Z" };

        for( auto _ : state )
        {
            auto dto{ DateTimeOffset{ iso } };
            ::benchmark::DoNotOptimize( dto );
        }
    }

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    static void BM_DateTimeOffset_ToUniversalTime( ::benchmark::State& state )
    {
        auto dto{ DateTimeOffset::now() };

        for( auto _ : state )
        {
            auto utc{ dto.toUniversalTime() };
            ::benchmark::DoNotOptimize( utc );
        }
    }

    static void BM_DateTimeOffset_ToOffset( ::benchmark::State& state )
    {
        auto dto{ DateTimeOffset::now() };
        auto newOffset{ TimeSpan::fromHours( -8 ) };

        for( auto _ : state )
        {
            auto result{ dto.toOffset( newOffset ) };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DateTimeOffset_UtcDateTime( ::benchmark::State& state )
    {
        auto dto{ DateTimeOffset::now() };

        for( auto _ : state )
        {
            auto utcDt{ dto.utcDateTime() };
            ::benchmark::DoNotOptimize( utcDt );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_DateTimeOffset_ToString( ::benchmark::State& state )
    {
        auto dto{ DateTimeOffset::now() };

        for( auto _ : state )
        {
            auto str{ dto.toString() };
            ::benchmark::DoNotOptimize( str );
        }
    }

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    static void BM_DateTimeOffset_Add( ::benchmark::State& state )
    {
        auto dto{ DateTimeOffset::now() };
        auto ts{ TimeSpan::fromHours( 24 ) };

        for( auto _ : state )
        {
            auto result{ dto + ts };
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_DateTimeOffset_Subtract( ::benchmark::State& state )
    {
        auto dto1{ DateTimeOffset::now() };
        auto dto2{ DateTimeOffset{ 2024, 1, 1, TimeSpan::fromHours( 0 ) } };

        for( auto _ : state )
        {
            auto result{ dto1 - dto2 };
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    static void BM_DateTimeOffset_Comparison( ::benchmark::State& state )
    {
        auto dto1{ DateTimeOffset{ 2024, 10, 23, 15, 0, 0, TimeSpan::fromHours( 5 ) } };
        auto dto2{ DateTimeOffset{ 2024, 10, 23, 10, 0, 0, TimeSpan::fromHours( 0 ) } };

        for( auto _ : state )
        {
            bool result{ dto1 == dto2 }; // Should be true - same UTC time
            ::benchmark::DoNotOptimize( result );
        }
    }

    //=====================================================================
    // Benchmarks registration
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_Construct );
    BENCHMARK( BM_DateTimeOffset_Now );

    //----------------------------------------------
    // Parsing
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_Parse );
    BENCHMARK( BM_DateTimeOffset_ParseZ );

    //----------------------------------------------
    // Conversion
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_ToUniversalTime );
    BENCHMARK( BM_DateTimeOffset_ToOffset );
    BENCHMARK( BM_DateTimeOffset_UtcDateTime );

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_ToString );

    //----------------------------------------------
    // Arithmetic
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_Add );
    BENCHMARK( BM_DateTimeOffset_Subtract );

    //----------------------------------------------
    // Comparison
    //----------------------------------------------

    BENCHMARK( BM_DateTimeOffset_Comparison );
} // namespace nfx::time::benchmark

BENCHMARK_MAIN();
