#ifndef Vector2D_H
#define Vector2D_H

#include <ostream>
#include <cmath>

template<typename T> class Vector2D{
    template<typename U> friend U operator*(const Vector2D<U>& lhs, const Vector2D<U>& rhs);
    template<typename U> friend U operator/(const Vector2D<U>& lhs, const Vector2D<U>& rhs);
    template<typename U> friend Vector2D<U> operator/(const U scale, const Vector2D<U>& rhs);

    public:
        using value_type = T;

        Vector2D() = default;

        Vector2D(const T x, const T y): _x(x), _y(y) {}

        Vector2D(const Vector2D<T>&) = default;

        Vector2D(Vector2D<T>&&) = default;

        ~Vector2D() = default;

        Vector2D<T>& operator=(const Vector2D<T>&) = default;

        Vector2D<T>& operator=(Vector2D<T>&&) = default;

        Vector2D<T>& operator+=(const Vector2D<T>& rhs);

        Vector2D<T>& operator+=(const T shift);

        Vector2D<T>& operator-=(const Vector2D<T>& rhs);

        Vector2D<T>& operator-=(const T shift);

        Vector2D<T>& operator*=(const T scale);

        Vector2D<T>& operator/=(const T scale);

        Vector2D<T>& projectAssign(const Vector2D<T>& rhs);

        Vector2D<T> project(const Vector2D<T>& rhs) const;

        T x() const{ return _x; }
        T y() const{ return _y; }


    private:
        T _x = 0;
        T _y = 0;
};


template<typename T> Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& rhs){
    _x += rhs._x;
    _y += rhs._y;
    return (*this);
}


template<typename T> Vector2D<T> operator+(const Vector2D<T>& lhs, const Vector2D<T>& rhs){
    Vector2D<T> ret{lhs};
    ret+=rhs;
    return ret;
}


template<typename T> Vector2D<T>& Vector2D<T>::operator+=(const T shift){
    _x += shift;
    _y += shift;
    return (*this);
}


template<typename T> Vector2D<T> operator+(const Vector2D<T>& lhs, const T shift){
    Vector2D<T> ret{lhs};
    ret += shift;
    return ret;
}


template<typename T> Vector2D<T> operator+(const T shift, const Vector2D<T>& rhs){
    return (rhs + shift);
}


template<typename T> Vector2D<T> operator-(const Vector2D<T>& lhs){
    return Vector2D<T>{-lhs.x(), -lhs.y()};
}


template<typename T> Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& rhs){
    // Don't call the unary - operator here to avoid the construction of a temporary vector;
    _x -= rhs._x;
    _y -= rhs._y;
    return *this;
}


template<typename T> Vector2D<T> operator-(const Vector2D<T>& lhs, const Vector2D<T>& rhs){
    Vector2D<T> ret{lhs};
    ret -= rhs;
    return ret;
}


template<typename T> Vector2D<T>& Vector2D<T>::operator-=(const T shift){
    return (*this += (-shift));
}


template<typename T> Vector2D<T> operator-(const Vector2D<T>& lhs, const T shift){
    Vector2D<T> ret{lhs};
    ret -= shift;
    return ret;
}


template<typename T> Vector2D<T> operator-(const T shift, const Vector2D<T>& rhs){
    Vector2D<T> ret{-rhs};
    ret += shift;
    return ret;
}


template<typename T> T operator*(const Vector2D<T>& lhs, const Vector2D<T>& rhs){
    return (
        (lhs._x * rhs._x) +
        (lhs._y * rhs._y)
    );
}


template<typename T> Vector2D<T>& Vector2D<T>::operator*=(const T scale){
    _x *= scale;
    _y *= scale;
    return *this;
}


template<typename T> Vector2D<T> operator*(const Vector2D<T>& lhs, const T scale){
    Vector2D<T> ret{lhs};
    ret *= scale;
    return ret;
}


template<typename T> Vector2D<T> operator*(const T scale, const Vector2D<T>& rhs){
    return (rhs * scale);
}


template<typename T> T operator/(const Vector2D<T>& lhs, const Vector2D<T>& rhs){
    return (
        (lhs._x/rhs._x) +
        (lhs._y/rhs._y)
    );
}


template<typename T> Vector2D<T>& Vector2D<T>::operator/=(const T scale){
    _x /= scale;
    _y /= scale;
    return *this;
}


template<typename T> Vector2D<T> operator/(const Vector2D<T>& lhs, const T scale){
    Vector2D<T> ret{lhs};
    ret /= scale;
    return ret;
}


template<typename T> Vector2D<T> operator/(const T scale, const Vector2D<T>& rhs){
    return Vector2D<T>{
        (scale/rhs._x),
        (scale/rhs._y),
    };
}


template<typename T> std::ostream& operator<<(std::ostream& os, const Vector2D<T>& rhs){
    os << "(" << rhs.x() << ", "<< rhs.y() << ")";
    return os;
}

#endif
