//
// Created by oem on 31.05.18.
//
#include <list>

#include <benchmark/benchmark.h>
#include "allocator.h"

template <typename Allocator>
void BM_fixed_allocator(benchmark::State& state) {
    std::list<int, Allocator> test;
    int limit = 10000000;
    for(int i = 0; i < limit; ++i) {
        test.push_back(i);
    }

    test.erase(test.begin());
    test.erase(test.end());

    auto iter = test.begin();
    for (int i = 0; i < 1000; i++) { iter++; }
    test.erase(iter);

    for(int i = 0; i < limit; ++i) {
        test.push_back(i);
    }

    iter = test.begin();
    for (int i = 0; i < 8; ++i) {
        for (int i = 0; i < 1000; i++) { iter++; }
        test.erase(iter);
    }

    for(int i = 0; i < limit; ++i) {
        test.push_back(i);
    }

    test.clear();
}


BENCHMARK_TEMPLATE(BM_fixed_allocator, std::allocator<int>);
BENCHMARK_TEMPLATE(BM_fixed_allocator, kkk::Allocator<int>);

BENCHMARK_MAIN();