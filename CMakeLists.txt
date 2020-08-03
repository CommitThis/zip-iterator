cmake_minimum_required(VERSION 3.12)
project(ZipIterator)

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
