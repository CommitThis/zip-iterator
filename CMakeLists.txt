set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_minimum_required(VERSION 3.12)
project(ZipIterator)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)
FetchContent_Declare(googletest SYSTEM
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.13.x
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(test_zip_two test_zip_two.cpp)
target_compile_features(test_zip_two PRIVATE cxx_std_17)
target_link_libraries(test_zip_two PRIVATE 
    gtest 
    gtest_main 
    pthread)

add_test(test_zip_two test_zip_two)


add_executable(test_zip_tuple test_zip_tuple.cpp)
target_compile_features(test_zip_tuple PRIVATE cxx_std_17)
target_link_libraries(test_zip_tuple PRIVATE 
    gtest 
    gtest_main 
    pthread)

add_test(test_zip_tuple test_zip_tuple)
