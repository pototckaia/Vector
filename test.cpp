// Created by oem on 25.04.18.
//
//
#include <vector>
#include <iostream>


#include "catch.hpp"
#include "vector.h"
#include "allocator.h"

struct person {
    std::string name;
    int age;

    person() = delete;

    person(std::string& my_name, int my_age) : name("& name with age"), age(my_age) {}
    person(std::string&& my_name, int my_age) : name("&& name with age"), age(my_age) {};

    explicit person(int my_age): name("age"), age(my_age) {};
    person(std::string& my_name) : name("& name "), age() {};
    person(std::string&& my_name) : name("&& name"), age() {};
    person(const person& p): name(), age() {};

    person& operator=(const person& p) {
        name = p.name;
        age = p.age + 2;
        return *this;
    }

    person& operator=(person&& p) {
        name = p.name;
        age = p.age + 3;
        return *this;
    }

    ~person() = default;

    friend bool operator==(const person& first, const person& second) {
        return (first.name == second.name && first.age == second.age);
    }
};

struct copy_throw {
    double id;
    copy_throw() : id() {};
    ~copy_throw() = default;
    copy_throw(const copy_throw& ) { throw "copy constructor is invalide"; }
    copy_throw(copy_throw&& ) { throw "move constructor is invalide"; }
    copy_throw& operator=(const copy_throw& ) { throw "operator= lvalue is invalide"; }
    copy_throw& operator=(copy_throw&& ) { throw "operator= rvalue is invalide"; }

    friend bool operator==(const copy_throw& a, const copy_throw& b) {
        return a.id == b.id;
    }
};


template <typename T>
void alert_vector(kkk::Vector<T>& testable, std::vector<T>& required) {
    REQUIRE(testable.size() == required.size());
    for (int i = 0; i < required.size(); i++) {
        REQUIRE(testable[i] == required[i]);
    }
}

TEST_CASE("create_vector") {
    SECTION("default constructor int") {
        kkk::Vector<int> t1;
        std::vector<int> result;
        alert_vector(t1, result);
    }

    SECTION("default constructor string") {
        kkk::Vector<std::string> words1;
        std::vector<std::string> words2;
        alert_vector(words1, words2);
    }

    SECTION("without default constructor ") {
        kkk::Vector<person> t2;
        std::vector<person> result_;
        alert_vector(t2, result_);
    }

    SECTION("count constructor int") {
        kkk::Vector<int> t1(3);
        std::vector<int> result(3);
        alert_vector(t1, result);
    }

    SECTION("count construct string") {
        kkk::Vector<std::string> words(5);
        std::vector<std::string> result_str(5);
        alert_vector(words, result_str);

    }

    SECTION("count construction with int copy element") {
        int value = 78;
        kkk::Vector<int> t1(2, value); //  fixed
        std::vector<int> result(2, value);
        alert_vector(t1, result);
    }

    SECTION("count construct with string copy element") {
        kkk::Vector<std::string> words(5, "Mo");
        std::vector<std::string> result_str(5, "Mo");
        alert_vector(words, result_str);
    }

    SECTION("copy construction int") {
        kkk::Vector<int> t1;
        kkk::Vector<int> t2(t1);
        std::vector<int> result_int;
        alert_vector(t2, result_int);


        kkk::Vector<int> t3(5, 8); // fixed
        kkk::Vector<int> t4(t3);
        result_int = {8, 8, 8, 8, 8};
        alert_vector(t4, result_int);

        t4 = {1, 2, 3, 4, 5, 6};
        kkk::Vector<int> t5(t4);
        result_int = {1, 2, 3, 4, 5, 6};
        alert_vector(t5, result_int);
    }

    SECTION("copy construction string") {
        kkk::Vector<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
        kkk::Vector<std::string> words3(words1);
        std::vector<std::string> result_str{"the", "frogurt", "is", "also", "cursed"};
        alert_vector(words3, result_str);

    }

    SECTION("move construction int") {
        kkk::Vector<int> t0(1, -4); // fixed
        kkk::Vector<int> t1(std::move(t0));
        std::vector<int> result_int(1, -4);
        alert_vector(t1, result_int);

        t1 = {1, 2, 3, 4};
        kkk::Vector<int> t2(std::move(t1));
        result_int = {1, 2, 3, 4};
        alert_vector(t2, result_int);
    }

    SECTION("range construction string") { // [first, last)
        std::vector<std::string> result_str{"the", "frogurt", "is", "also", "cursed"};
        kkk::Vector<std::string> words1(result_str.begin(), result_str.end());
        alert_vector(words1, result_str);

        kkk::Vector<std::string> words2(result_str.begin() + 2, result_str.end());
        result_str = {"is", "also", "cursed"};
        alert_vector(words2, result_str);


        kkk::Vector<std::string> words3(result_str.begin(), --result_str.end());
        result_str = {"is", "also"};
        alert_vector(words3, result_str);
    }

    SECTION("constuction with initializer_list") {
        kkk::Vector<int> numbers1{1, 4, 5, 7, 7, -1, 8};
        std::vector<int> result_int{1, 4, 5, 7, 7, -1, 8};
        alert_vector(numbers1, result_int);
    }
}

TEST_CASE("operator=") {
    std::vector<int> result_int{3, 1, 4, 6, 5, 9};
    kkk::Vector<int> test1;
    SECTION("const lvalue operator=") {
        kkk::Vector<int> test2(result_int.begin(), result_int.end());
        test1 = test2;
        alert_vector(test1, result_int);
    }
    SECTION("const rvalue operator=") {
        kkk::Vector<int> test2{3, 1, 4, 6, 5, 9};
        test1 = test2;
        alert_vector(test1, result_int);
    }
    SECTION("initializer_list operator=") {
        test1 = {3, 1, 4, 6, 5, 9};
        alert_vector(test1, result_int);
    }
}

TEST_CASE("assign") {
    kkk::Vector<std::string> words;
    std::vector<std::string> result_str;
    SECTION("count asssign") {
        words.assign(10, "ghjhkdkjf");
        result_str.assign(10, "ghjhkdkjf");
        alert_vector(words, result_str);
    }

    SECTION("range assign") {
        result_str = {"Ghbfft", "fghk", "kkkdhjdfd", "kdgadfg", "fgksjfd", "kfdghdjg"};
        words.assign(result_str.begin(), result_str.end());

    }
    SECTION("assign initializer_list") {
        result_str = {"lol", ",", "kek", ",", "cheburek"};
        words.assign({"lol", ",", "kek", ",", "cheburek"});
        alert_vector(words, result_str);
    }
}

TEST_CASE("access to an item") {

    SECTION("at") {
        kkk::Vector<std::string> words({"G", "df"});
        REQUIRE(words.at(0) == words[0]);
        REQUIRE(words.at(1) == words[1]);
        REQUIRE_THROWS(words.at(-1));
        REQUIRE_THROWS(words.at(3));
    }
    SECTION("operator[]") {
        kkk::Vector<double> numbers(12, 3.0);

        for (int i = 0; i < numbers.size(); ++i) {
            REQUIRE(numbers[i] == 3.0);
            numbers[i] = 5.0;
            REQUIRE(numbers[i] == 5.0);
        }
    }
    kkk::Vector<char> letters {'o', 'm', 'g', 'w', 't', 'f'};

    SECTION("front") {
        REQUIRE(letters.front() == 'o');
        letters.push_back('j');
        REQUIRE(letters.front() == 'o');

        auto iter = letters.insert(letters.cbegin(), 'k');
        REQUIRE(*iter == 'k');
    }
    SECTION("back") {
        REQUIRE(letters.back() == 'f');
        letters.push_back('g');
        REQUIRE(letters.back() == 'g');
        letters.pop_back();
        REQUIRE(letters.back() == 'f');
    }
    SECTION("data") {
        char* ptr = letters.data();
        for (std::size_t i = 0; i < letters.size(); ++i) {
            REQUIRE(letters[i] == *(ptr + i));
        }

        letters.clear();
        REQUIRE(letters.data() == nullptr);
    }
}

TEST_CASE("size vector") {
    SECTION("empty") {
        kkk::Vector<int> numbers;
        REQUIRE(numbers.empty());

        numbers.push_back(123);
        numbers.push_back(234);
        REQUIRE(!numbers.empty());

        numbers.pop_back();
        numbers.pop_back();
        REQUIRE(numbers.empty());
    }
    SECTION("size") {
        kkk::Vector<int> numbers{1, 2, 3, 4};
        REQUIRE(numbers.size() == 4);
    }
    SECTION("reserve and capacity") {
        kkk::Vector<double> characters;
        characters.reserve(10);
        REQUIRE(characters.capacity() >= 10);

    }
    SECTION("shrink_to_fit") {
        kkk::Vector<int> v;
        v.reserve(100);
        REQUIRE(v.capacity() == 100);
        v.clear();
        REQUIRE(v.capacity() == 100);
        v.shrink_to_fit();
        REQUIRE(v.capacity() == 0);

        v.reserve(100);
        v.resize(67);
        v.shrink_to_fit();
        REQUIRE(v.capacity() == 67);
    }
}

TEST_CASE("operation modifiers") {
    kkk::Vector<int> numbers;
    SECTION("push_back and pop") {
        std::vector<int> result_int = {5, 3, 4};

        numbers.push_back(5);
        numbers.push_back(3);
        numbers.push_back(4);
        alert_vector(numbers, result_int);

        numbers.pop_back();
        result_int = {5, 3};
        alert_vector(numbers, result_int);
    }

    SECTION("resize") {
        std::vector<int> result{1, 2, 3, 0, 0};
        kkk::Vector<int> numbers{1, 2, 3};
        numbers.resize(5);
        alert_vector(numbers, result);

        numbers.resize(2);
        result = {1, 2};
        alert_vector(numbers, result);

        numbers.resize(4, 2);
        result = {1, 2, 2, 2};
        alert_vector(numbers, result);

        numbers.resize(4);
        alert_vector(numbers, result);
    }

    SECTION("safe strong"){
        std::vector<copy_throw> result(4);
        kkk::Vector<copy_throw> try_resize(4);
        try{ try_resize.resize(7); } catch (...) {}
        alert_vector(try_resize, result);

        try { try_resize.emplace(try_resize.cend(), copy_throw()); } catch(...) {}
        alert_vector(try_resize, result);

        try { try_resize.emplace_back(copy_throw()); } catch(...) {}
        alert_vector(try_resize, result);

        try { try_resize.push_back(copy_throw()); } catch(...) {}
        alert_vector(try_resize, result);
    }

    kkk::Vector<person> persons;
    SECTION("emplace back") {
        std::string name("lora");
        std::vector<person> result;
        result.reserve(10);
        result.emplace_back(13);
        result.emplace_back("nasty", 19);
        result.emplace_back("larisa", 45);
        result.emplace_back(23);
        result.emplace_back(name, 23);
        result.emplace_back(name);

        persons.reserve(10);
        persons.emplace_back(13);
        persons.emplace_back("nasty", 19);
        persons.emplace_back("larisa", 45);
        persons.emplace_back(23);
        persons.emplace_back(name, 23);
        persons.emplace_back(name);

        alert_vector(persons, result);

        persons.pop_back();
        result.pop_back();
        alert_vector(persons, result);
    }

    SECTION("emplace") {
        std::string name("Jile");

        auto iter = persons.emplace(persons.cbegin(), "Pole", 12);
        person p = person("Pole", 12);
        REQUIRE(*iter == p);

        iter = persons.emplace(persons.cend(), name, 12);
        REQUIRE(*iter == person(name, 12));

        iter = persons.emplace(kkk::Vector<person>::const_iterator(iter.data() - 1), "Pol");
        REQUIRE(*iter == person("Pol"));
    }

    kkk::Vector<std::string> words{"Hello", ", ", "world"};
    std::vector<std::string> answer{"Hello", ", ", "world"};

    SECTION("insert") {
        SECTION("insert begin"){
            answer = {"k", "Hello", ", ", "world"};
            auto iter = words.insert(words.cbegin(), "k"); // почему не вызывается const_iterator
            REQUIRE(*iter == "k");
            alert_vector(words, answer);
        }
        SECTION("insert end") {
            auto iter = words.insert(words.cend(), "j");
            answer.insert(answer.cend(), "j");
            REQUIRE(*iter == "j");
            alert_vector(words, answer);

        }

        SECTION("insert in  any place") {
            answer = {"!", "Hello", ", ", "world", "!"};
            words = {"!", "Hello", ", ", "world", "!"};
            std::vector<std::string> insert_value{"Llll", "opop", "fdfdf", "dfadf", "gg"};
            auto iter_ans = answer.cbegin();
            auto iter = words.cbegin();

            for (std::size_t i = 0; i < insert_value.size(); i++) {
                iter = words.insert(iter, insert_value[i]);
                iter += 2;

                auto _iter = answer.insert(iter_ans, insert_value[i]);
                iter_ans = _iter + 2;

            }
            alert_vector(words, answer);
        }
    }

    SECTION("multiple insertion") {

        SECTION("2") {
            words.insert(words.cbegin(), 2, "k");
            answer.insert(answer.cbegin(), 2, "k");
            alert_vector(words, answer);

            words.insert(words.cbegin() + 1, 2, "k");
            answer.insert(answer.cbegin() + 1, 2, "k");
            alert_vector(words, answer);
        }

        SECTION("3") {
            words.insert(words.cbegin(), 3, "k");
            answer.insert(answer.cbegin(), 3, "k");
            alert_vector(words, answer);

            words.insert(words.cbegin() + 2, 3, "k");
            answer.insert(answer.cbegin() + 2, 3, "k");
            alert_vector(words, answer);
        }

        SECTION("15") {
            words.insert(words.cbegin(), 15, "k");
            answer.insert(answer.cbegin(), 15, "k");
            alert_vector(words, answer);

            words.insert(words.cbegin() + 6, 15, "2");
            answer.insert(answer.cbegin() + 6, 15, "2");
            alert_vector(words, answer);

            words.insert(words.cend(), 15, "k");
            answer.insert(answer.cend(), 15, "k");
            alert_vector(words, answer);
        }

        SECTION("insert 116 values") {
            numbers.resize(10, 18);
            numbers.insert(numbers.cend() - 6, 106, -1900);
            std::vector<int> result(10, 18);
            result.insert(result.cend() - 6, 106, -1900);
            alert_vector(numbers, result);
        }
    }

    SECTION("insert with iterator") {
        std::vector<int> vec2(9, 400);
        std::vector vec(12, 300);
        vec.insert(vec.cbegin() + 2, vec2.end() -2, vec2.end());

        kkk::Vector<int> numbers1(9, 400);
        kkk::Vector numbers2(12, 300);
        numbers2.insert(numbers2.begin() + 2, numbers1.end() - 2, numbers1.end());
        alert_vector(numbers2, vec);

        numbers2.insert(numbers2.cbegin() + 4, {1, 2, 3, 4, 5, 6, 7});
        vec.insert(vec.cbegin() + 4, {1, 2, 3, 4, 5, 6, 7});
        alert_vector(numbers2, vec);
    }

    std::vector<int> result{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    kkk::Vector<int> c1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        SECTION("erase one element") {
        c1.erase(c1.cbegin());
        result.erase(result.begin());
        alert_vector(c1, result);

        c1.erase(c1.cend());
        result.pop_back();
        alert_vector(c1, result);

        c1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        result = {1, 3, 5, 7, 9};
        for (auto it = c1.cbegin(); it != c1.cend();) {
            if (*it % 2 == 0) {
                auto _it = c1.erase(it);
                it = kkk::Vector<int>::const_iterator(_it.data());
            } else {
                ++it;
            }
        }
        alert_vector(c1, result);
    }
    SECTION("erase range element") {
        c1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        result = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        c1.erase(c1.cbegin() + 2, c1.cbegin() + 5);
        result.erase(result.begin() + 2, result.begin() + 5);
        alert_vector(c1, result);

    }

}


TEST_CASE("iterator") {

    kkk::Vector<int> numbers{12, 12, 23, -78, 34, 23, 1};
    const kkk::Vector<int> const_numbers = numbers;

    SECTION("begin and end") {
        REQUIRE(*numbers.begin() == numbers[0]);
        REQUIRE(*(--numbers.end()) == numbers[6]);

        REQUIRE(*const_numbers.begin() == const_numbers[0]);
        REQUIRE(*(--const_numbers.end()) == const_numbers[6]);

        REQUIRE(*numbers.cbegin() == const_numbers[0]);
        REQUIRE(*(--numbers.cend()) == const_numbers[6]);

        REQUIRE(*(numbers.rbegin()) == numbers[6]);
        REQUIRE(*(--numbers.rend()) == numbers[0]);

        long size_ = std::distance(numbers.begin(), numbers.end());
        REQUIRE(size_ == numbers.size());

        size_ = const_numbers.rend() - const_numbers.rbegin();
        REQUIRE(size_ == const_numbers.size());

        numbers = {1};
        REQUIRE(numbers.begin() != numbers.end());
        REQUIRE(numbers.rbegin() != numbers.rend());

        numbers = {};
        REQUIRE(numbers.begin() == numbers.end());
        REQUIRE((numbers.rbegin() == numbers.rend()));
    }

    SECTION("operator ++ -- [] *") {

        int index = 0;
        numbers = {012, 12, 23, -78, 34, 23, 1};
        for (auto iter = numbers.begin(); iter != numbers.end(); ++iter) {
            REQUIRE(*iter == numbers[index]);
            index++;
        }

        index = 6;
        numbers = {012, 12, 23, -78, 34, 23, 1};
        for (auto iter = numbers.rbegin(); iter != numbers.rend(); ++iter) {
            REQUIRE(*iter == numbers[index]);
            index--;
        }

        REQUIRE(numbers.begin() + 3 == 3 + numbers.begin());
        REQUIRE(numbers.end() - 3 == 3 - numbers.end());

        auto size =  numbers.end() - numbers.begin() - 2;
        REQUIRE(numbers.begin() + 2 + size == numbers.end());

        REQUIRE(numbers.begin()[4] == *(numbers.begin() + 4));

    }

    SECTION("comparison") {
//        Strict total ordering relation
        REQUIRE(numbers.begin() < numbers.begin() + 4);
        REQUIRE(!(numbers.begin() < numbers.begin()));


        auto iter_1 = numbers.end() - 3;
        auto iter_2 = numbers.begin() + 3;
        bool p = (iter_1 < iter_2) || (iter_1 > iter_2) || (iter_1 == iter_2);
        REQUIRE(p);

        if (iter_1 < iter_2) { REQUIRE(!(iter_2 < iter_1)); }
        if (iter_1 > iter_2) { REQUIRE(!(iter_2 > iter_1)); }

    }

    SECTION("convert") {
        kkk::Vector<int>::const_iterator c = numbers.begin();
        REQUIRE(*c == numbers[0]);

        c = --numbers.end();
        REQUIRE(*c == numbers.back());
    }


}

TEST_CASE("allocator") {

//    std::list<int, kkk::Allocator<int>> test;
//    int limit = 1000;
//    for(int i = 0; i < limit; ++i) {
//        test.push_back(i);
//    }
//
//    test.erase(test.begin());
//    test.erase(test.end());
//
//    test.clear();
}