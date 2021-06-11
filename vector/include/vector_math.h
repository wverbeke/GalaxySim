#ifndef vector_math_H
#define vector_math_H

#include <cmath>

template<typename T> typename T::value_type square(const T& lhs){
    return lhs*lhs;
}


template<typename T> typename T::value_type abs(const T& lhs){
    return std::sqrt(square(lhs));
}


template<typename T> typename T::value_type distance(const T& lhs, const T& rhs){
    return abs(lhs - rhs);
}

#endif
