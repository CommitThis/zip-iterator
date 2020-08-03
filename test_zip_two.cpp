#include "zip_two.hpp"

#include <cassert>
#include <iostream>

#include <iostream>
#include <memory>
#include <type_traits>



auto main() -> int 
{
    auto a = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto const & b = a;
    auto c = std::vector<int>{};

    for (auto && [x, y] : c9::zip(a, b)) {
        c.push_back(x + y);
    }

    auto expected = std::vector<int>{2, 4, 6, 8, 10, 12};
    assert(c == expected);

}