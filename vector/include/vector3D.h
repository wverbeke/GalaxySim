#ifndef Vector3D_H
#define Vector3D_H

#include <ostream>
#include <cmath>

#include "hdf5.h"
#include "compare_numbers.h"

// Friend functions for doing hdf5 IO.
// The function is defined under the io directory.
template<typename T> hid_t h5_Vector3D();

template <typename T> class Vector3D;
template <typename T> T operator*(const Vector3D<T>&, const Vector3D<T>&);
template <typename T> T operator/(const Vector3D<T>&, const Vector3D<T>&);
template <typename T> Vector3D<T> operator/(const T, const Vector3D<T>&);
template <typename T> Vector3D<T> cross(const Vector3D<T>&, const Vector3D<T>&);

// For testing.
template <typename T> bool is_close(const Vector3D<T>&, const Vector3D<T>&);

template<typename T> class Vector3D{

    friend T operator*<T>(const Vector3D&, const Vector3D&);
    friend T operator/<T>(const Vector3D&, const Vector3D&);
    friend Vector3D operator/<T>(const T, const Vector3D&);
    friend Vector3D cross<T>(const Vector3D&, const Vector3D&);
    friend bool is_close<T>(const Vector3D&, const Vector3D&);

    friend hid_t h5_Vector3D<T>();

    public:
        using value_type = T;

        Vector3D() = default;

        Vector3D(const T x, const T y, const T z): _x(x), _y(y), _z(z) {}

        Vector3D(const Vector3D&) = default;

        Vector3D(Vector3D&&) = default;

        ~Vector3D() = default;

        Vector3D& operator=(const Vector3D&) = default;

        Vector3D& operator=(Vector3D&&) = default;

        Vector3D& operator+=(const Vector3D& rhs);

        Vector3D& operator+=(const T shift);

        Vector3D& operator-=(const Vector3D& rhs);

        Vector3D& operator-=(const T shift);

        Vector3D& operator*=(const T scale);

        Vector3D& operator/=(const T scale);

        Vector3D& projectAssign(const Vector3D& rhs);

        Vector3D project(const Vector3D& rhs) const;

        T x() const{ return _x; }
        T y() const{ return _y; }
        T z() const{ return _z; }


    private:
        T _x = 0;
        T _y = 0;
        T _z = 0;
};


template<typename T> Vector3D<T>& Vector3D<T>::operator+=(const Vector3D& rhs){
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;
    return (*this);
}


template<typename T> Vector3D<T> operator+(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    Vector3D<T> ret{lhs};
    ret+=rhs;
    return ret;
}


template<typename T> Vector3D<T>& Vector3D<T>::operator+=(const T shift){
    _x += shift;
    _y += shift;
    _z += shift;
    return (*this);
}


template<typename T> Vector3D<T> operator+(const Vector3D<T>& lhs, const T shift){
    Vector3D<T> ret{lhs};
    ret += shift;
    return ret;
}


template<typename T> Vector3D<T> operator+(const T shift, const Vector3D<T>& rhs){
    return (rhs + shift);
}


template<typename T> Vector3D<T> operator-(const Vector3D<T>& lhs){
    return Vector3D<T>{-lhs.x(), -lhs.y(), -lhs.z()};
}


template<typename T> Vector3D<T>& Vector3D<T>::operator-=(const Vector3D& rhs){
    // Don't call the unary - operator here to avoid the construction of a temporary vector;
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;
    return *this;
}


template<typename T> Vector3D<T> operator-(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    Vector3D<T> ret{lhs};
    ret -= rhs;
    return ret;
}


template<typename T> Vector3D<T>& Vector3D<T>::operator-=(const T shift){
    return (*this += (-shift));
}


template<typename T> Vector3D<T> operator-(const Vector3D<T>& lhs, const T shift){
    Vector3D<T> ret{lhs};
    ret -= shift;
    return ret;
}


template<typename T> Vector3D<T> operator-(const T shift, const Vector3D<T>& rhs){
    Vector3D<T> ret{-rhs};
    ret += shift;
    return ret;
}


template<typename T> T operator*(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    return (
        (lhs._x * rhs._x) +
        (lhs._y * rhs._y) +
        (lhs._z * rhs._z)
    );
}


template<typename T> Vector3D<T>& Vector3D<T>::operator*=(const T scale){
    _x *= scale;
    _y *= scale;
    _z *= scale;
    return *this;
}


template<typename T> Vector3D<T> operator*(const Vector3D<T>& lhs, const T scale){
    Vector3D<T> ret{lhs};
    ret *= scale;
    return ret;
}


template<typename T> Vector3D<T> operator*(const T scale, const Vector3D<T>& rhs){
    return (rhs * scale);
}


template<typename T> T operator/(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    return (
        (lhs._x/rhs._x) +
        (lhs._y/rhs._y) +
        (lhs._z/rhs._z)
    );
}


template<typename T> Vector3D<T>& Vector3D<T>::operator/=(const T scale){
    _x /= scale;
    _y /= scale;
    _z /= scale;
    return *this;
}

template<typename T> Vector3D<T> operator/(const Vector3D<T>& lhs, const T scale){
    Vector3D<T> ret{lhs};
    ret /= scale;
    return ret;
}

template<typename T> Vector3D<T> operator/(const T scale, const Vector3D<T>& rhs){
    return Vector3D<T>{
        (scale/rhs._x),
        (scale/rhs._y),
        (scale/rhs._z)
    };
}


// The cross product should not be defined in terms of a compound assignment operator
// because it requires a temporary vector to be computed.
template<typename T> Vector3D<T> cross(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    return Vector3D<T>{
        ((lhs._y * rhs._z) - (lhs._z * rhs._y)),
        ((lhs._z * rhs._x) - (lhs._x * rhs._z)),
        ((lhs._x * rhs._y) - (lhs._y * rhs._x))
    };
}


template<typename T> std::ostream& operator<<(std::ostream& os, const Vector3D<T>& rhs){
    os << "(" << rhs.x() << ", "<< rhs.y() <<", " << rhs.z() << ")";
    return os;
}

template <typename T> bool is_close(const Vector3D<T>& lhs, const Vector3D<T>& rhs){
    return (is_close(lhs._x, rhs._x) && is_close(lhs._y, rhs._y) && is_close(lhs._z, rhs._z));
}

#endif
