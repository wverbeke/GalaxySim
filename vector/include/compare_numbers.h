#ifndef compare_numbers_H
#define compare_numbers_H

#include <algorithm>

constexpr double EPS = 1e-9;

template<typename T> bool is_close(const T lhs, const T rhs){
    return (std::abs(lhs - rhs) < EPS);
}
#endif
