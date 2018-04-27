// Created by oem on 25.04.18.
//
//
#include <vector>

#include "catch.hpp"
#include "vector.h"


template <typename T>
void alert_vector(kkk::Vector<T> testable, std::vector<T> required) {
    REQUIRE(testable.size() == required.size());
    for (int i = 0; i < required.size(); i++) {
        REQUIRE(testable[i] == required[i]);
    }
}

TEST_CASE("create_vector") {
    SECTION("default constructor") {
        kkk::Vector<int> t1;
        std::vector<int> result;
        alert_vector(t1, result);
    }
    SECTION("count constructor") {
        kkk::Vector<int> t1(3);
        std::vector<int> result(3);
        alert_vector(t1, result);

    }
    SECTION("count construction with copy element") {
        int value = 78;
        kkk::Vector<int> t1(2, value);
        std::vector<int> result(2, value);
        alert_vector(t1, result);
    }
    SECTION("copy construction"){
        kkk::Vector<int> t1;
        kkk::Vector<int> t2(t1);
        std::vector<int> result;
        alert_vector(t2, result);

        kkk::Vector<int> t3(5, 8);
        kkk::Vector<int> t4(t3);
        result = {8, 8, 8, 8, 8};
        alert_vector(t4, result);

//        kkk::Vector<int> t4 = {1, 2, 3, 4, 5, 6};
//        t2 = t4;
//        result = {1, 2, 3, 4, 5, 6};
//        alert_vector(t2, result);

    }
    SECTION("move construction") {
        kkk::Vector<int> t0(1, -4);
        kkk::Vector<int> t1 = std::move(t0);
        std::vector<int> result(1, -4);
        alert_vector(t1, result);

//        kkk::Vector<int> t1{1, 2, 3, 4};
//        kkk::Vector t1(std::move(t1));
//        std::vector result(std::vector{1, 2, 3, 4});
    }

    SECTION("range construction") { // [first, last)
    }
}
