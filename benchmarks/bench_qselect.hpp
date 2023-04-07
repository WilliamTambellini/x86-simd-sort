#include "bench-qsort-common.h"

template <typename T>
static void avx512_qselect(benchmark::State& state) {
    if (!cpu_has_avx512bw()) {
        state.SkipWithMessage("Requires AVX512 BW ISA");
    }
    if ((sizeof(T) == 2) && (!cpu_has_avx512_vbmi2())) {
        state.SkipWithMessage("Requires AVX512 VBMI2 ISA");
    }
    // Perform setup here
    size_t ARRSIZE = state.range(0);
    std::vector<T> arr;
    std::vector<T> arr_bkp;

    /* Initialize elements */
    arr = get_uniform_rand_array<T>(ARRSIZE);
    arr_bkp = arr;

    /* Choose random index to make sorted */
    int k = get_uniform_rand_array<int64_t>(1, ARRSIZE, 1).front();

    /* call avx512 quickselect */
    for (auto _ : state) {
        avx512_qselect<T>(arr.data(), k, ARRSIZE);

        state.PauseTiming();
        arr = arr_bkp;
        state.ResumeTiming();
    }
}

template <typename T>
static void stdnthelement(benchmark::State& state) {
    // Perform setup here
    size_t ARRSIZE = state.range(0);
    std::vector<T> arr;
    std::vector<T> arr_bkp;

    /* Initialize elements */
    arr = get_uniform_rand_array<T>(ARRSIZE);
    arr_bkp = arr;

    /* Choose random index to make sorted */
    int k = get_uniform_rand_array<int64_t>(1, ARRSIZE, 1).front();

    /* call std::nth_element */
    for (auto _ : state) {
        std::nth_element(arr.begin(), arr.begin() + k, arr.end());

        state.PauseTiming();
        arr = arr_bkp;
        state.ResumeTiming();
    }
}

// Register the function as a benchmark
BENCHMARK(avx512_qselect<float>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<float>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<uint32_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<uint32_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<int32_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<int32_t>)->Arg(10000)->Arg(1000000);

BENCHMARK(avx512_qselect<double>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<double>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<uint64_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<uint64_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<int64_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<int64_t>)->Arg(10000)->Arg(10000000);

//BENCHMARK(avx512_qselect<float16>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<uint16_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<uint16_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(avx512_qselect<int16_t>)->Arg(10000)->Arg(1000000);
BENCHMARK(stdnthelement<int16_t>)->Arg(10000)->Arg(10000000);
