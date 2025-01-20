#include "zip_tuple.hpp"

#include <iostream>
#include <iostream>
#include <memory>
#include <type_traits>


#include <gtest/gtest.h>




TEST(ZipTuple, CanIterateVector)
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b = std::vector<int>{1, 2, 3, 4, 5, 6};
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    auto result = std::vector<int>{};

    for (auto && [i, j] : c9::zip(a, b)) {
        result.push_back(i + j);
    }
    EXPECT_EQ(result, expected);
}


TEST(ZipTuple, CanAssignZippedByReference)
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto c = std::vector<int>{0, 0, 0, 0, 0, 0};

    for (auto && [i, j, k] : c9::zip(a, b, c)) {
        k = i + j;
    }
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    EXPECT_EQ(c, expected);
}


TEST(ZipTuple, WillIterateOverShortestSize)
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b = std::vector<int>{1, 2, 3, 4, 5, 6, 7};
    auto c = std::vector<int>{0, 0, 0, 0, 0};

    for (auto && [i, j, k] : c9::zip(a, b, c)) {
        k = i + j;
    }
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10};
    EXPECT_EQ(c, expected);
}


TEST(ZipTuple, ZippedProvidesReference)
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto zipper = c9::zip(a);
    
    using iter_type = decltype(zipper.begin()); /* zip_iterator<...> */
    using iter_value_type = iter_type::value_type;
    using expected_value_type = std::tuple<int&>;

    constexpr static auto is_same = std::is_same_v<
            iter_value_type,
            expected_value_type>;
    EXPECT_TRUE(is_same);
}


TEST(ZipTuple, ConstZippedProvidesConstReference)
{
    const auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto zipper = c9::zip(a);
    
    using iter_type = decltype(zipper.begin()); /* zip_iterator<...> */
    using iter_value_type = iter_type::value_type;
    using expected_value_type = std::tuple<const int&>;

    constexpr static auto is_same = std::is_same_v<
            iter_type::value_type, 
            expected_value_type>;
    EXPECT_TRUE(is_same);
}


TEST(ZipTuple, MixedZippedProvidesMixedReference)
{
    const auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto zipper = c9::zip(a, b);
    
    using iter_type = decltype(zipper.begin()); /* zip_iterator<...> */
    using iter_value_type = iter_type::value_type;
    using expected_value_type = std::tuple<const int&, int&>;

    constexpr static auto is_same = std::is_same_v<
            iter_type::value_type, 
            expected_value_type>;
    EXPECT_TRUE(is_same);
}


TEST(ZipTuple, CanIterateArray)
{
    auto a = std::array<int, 6>{1, 2, 3, 4, 5, 6};
    auto b = std::array<int, 6>{1, 2, 3, 4, 5, 6};
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    auto result = std::vector<int>{};

    for (auto && [i, j] : c9::zip(a, b)) {
        result.push_back(i + j);
    }
    EXPECT_EQ(result, expected);
}


TEST(ZipTuple, CanIterateRawArray)
{
    int a[] = {1, 2, 3, 4, 5, 6};
    int b[] = {1, 2, 3, 4, 5, 6};
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    auto result = std::vector<int>{};

    for (auto && [i, j] : c9::zip(a, b)) {
        result.push_back(i + j);
    }
    EXPECT_EQ(result, expected);
}


TEST(ZipTuple, CanIterateConstRawArray)
{
    const int a[] = {1, 2, 3, 4, 5, 6};
    const int b[] = {1, 2, 3, 4, 5, 6};
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    auto result = std::vector<int>{};

    for (auto && [i, j] : c9::zip(a, b)) {
        result.push_back(i + j);
    }
    EXPECT_EQ(result, expected);
}


TEST(ZipTuple, CanAssignToVectorOfBool)
{
    const auto a = std::vector<bool>{1, 0, 1, 0, 1, 0};
    const auto b = std::vector<bool>{0, 1, 0, 1, 0, 1};

    
    auto expected = std::vector<bool>{1, 1, 1, 1, 1, 1};
    auto result = std::vector<bool>{};
    result.resize(a.size());

    for (auto && [i, j, k] : c9::zip(a, b, result)) {
        k = i | j;
    }
    EXPECT_EQ(result, expected);
}




template <typename T, std::size_t N>
auto iter_ref(T (&arr)[N])
{
    for (auto && [i] : c9::zip(arr)) {
        i = 0;
    }
}


TEST(ZipTuple, CanIterateRawArrayReference)
{
    int raw[6] = {1, 2, 3, 4, 5, 6};
    int expected[6] = {0, 0, 0, 0, 0, 0};
    iter_ref(raw);
    EXPECT_EQ(raw[0], 0);
    EXPECT_EQ(raw[1], 0);
    EXPECT_EQ(raw[2], 0);
    EXPECT_EQ(raw[3], 0);
    EXPECT_EQ(raw[4], 0);
    EXPECT_EQ(raw[5], 0);
}