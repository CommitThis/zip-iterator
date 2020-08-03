#include "zip_tuple.hpp"

#include <cassert>
#include <iostream>

#include <iostream>
#include <memory>
#include <type_traits>

#include <cxxabi.h>


auto main() -> int 
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b = std::vector<int>{1, 2, 3, 4, 5, 6, 7};
    auto c = std::vector<int>{0, 0, 0, 0, 0};
    auto const & d = b;

    for (auto && [x, y, z] : c9::zip(a, d, c)) {
        z = x + y;
    }
    
    auto expected = std::vector<int>{2, 4, 6, 8, 10};
    assert(c == expected);
}