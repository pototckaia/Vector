//
// Created by oem on 31.05.18.
//
#include <list>

#include <benchmark/benchmark.h>
#include <tgmath.h>
#include <random>
#include "allocator.h"



template <typename Allocator>
void BM_fixed_allocator_int(benchmark::State& state) {

    for (auto _: state) {

        std::list<int, Allocator> test;
        int limit = 1000000;
        for(int i = 0; i < limit; ++i) {
            test.push_back(i);
        }

        test.erase(test.begin());
        test.erase(std::prev(test.end()));

        auto iter = test.begin();
        std::advance(iter, 1000);
        test.erase(iter);

        for(int i = 0; i < limit; ++i) {
            test.push_back(i);
        }

        iter = test.begin();
        for (int i = 0; i < 8; ++i) {
            std::advance(iter, 1000);
            iter = test.erase(iter);
        }

        for(int i = 0; i < limit; ++i) {
            test.push_back(i);
        }

        test.clear();
    }
}

template <typename Allocator>
void BM_fixed_allocator_string(benchmark::State& state) {

    for (auto _: state) {

        std::list<std::string, Allocator> test;
        int limit = 100000;
        std::string s(100, ' ');
        for(int i = 0; i < limit; ++i) {
            test.push_back(s);
        }

        test.erase(test.begin());
        test.erase(std::prev(test.end()));

        auto iter = test.begin();
        std::advance(iter, 1000);
        test.erase(iter);

        for(int i = 0; i < limit; ++i) {
            test.push_back(s);
        }

        iter = test.begin();
        for (int i = 0; i < 8; ++i) {
            std::advance(iter, 1000);
            iter = test.erase(iter);
        }

        for(int i = 0; i < limit; ++i) {
            test.push_back(s);
        }

        test.clear();
    }
}



BENCHMARK_TEMPLATE(BM_fixed_allocator_int, std::allocator<int>);
BENCHMARK_TEMPLATE(BM_fixed_allocator_int, kkk::Allocator<int>);

BENCHMARK_TEMPLATE(BM_fixed_allocator_string, std::allocator<std::string>);
BENCHMARK_TEMPLATE(BM_fixed_allocator_string, kkk::Allocator<std::string>);

BENCHMARK_MAIN();