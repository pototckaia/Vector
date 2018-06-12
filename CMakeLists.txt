cmake_minimum_required(VERSION 3.9)
project(vector_library)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(VECTOR_SOURCE vector.cpp vector.h)
add_library(vector STATIC ${VECTOR_SOURCE})

include_directories(catch)
add_subdirectory(catch)

add_executable(test_lib test.cpp allocator.cpp allocator.h)
target_link_libraries(test_lib vector catch)


add_executable(allocator_benchmark allocator_benchmark.cpp allocator.cpp allocator.h )
target_link_libraries(allocator_benchmark benchmark -lpthread)



include(CodeCoverage)
APPEND_COVERAGE_COMPILER_FLAGS()
set(COVERAGE_EXCLUDES 'catch/*')
setup_target_for_coverage(NAME test_lib_coverage
        EXECUTABLE test_lib
        DEPENDENCIES test.cpp allocator.cpp allocator.h)

