cmake_minimum_required(VERSION 3.9)
project(vector_library)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

set(VECTOR_SOURCE vector.cpp vector.h)
add_library(vector STATIC ${VECTOR_SOURCE})

SET(CATCH_PATH /home/oem/Documents/Homeworks/Cpp-homework/test_library)
add_library(catch STATIC IMPORTED)
set_property(TARGET catch PROPERTY IMPORTED_LOCATION "${CATCH_PATH}/libcatch.a")


add_executable(test_vector test.cpp)
target_include_directories(test_vector PRIVATE ${CATCH_PATH})
target_link_libraries(test_vector vector catch)
