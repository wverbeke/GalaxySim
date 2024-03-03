#ifndef Body_H
#define Body_H

#include <ostream>
#include "../../vector/include/compare_numbers.h"
#include "../../vector/include/vector3D.h"
#include "../../vector/include/vector2D.h"

template<typename T> class Body{

    public:
        using numeric_type = typename T::value_type;
        using vector_type = T;

        Body() = default;
        Body(const T& position, const T& velocity, const numeric_type mass):
            _position(position),
            _velocity(velocity),
            _mass(mass)
        {}

        Body(const Body&) = default;
        Body(Body&&) = default;
        Body& operator=(const Body&) = default;
        Body& operator=(Body&) = default;

        const T& position() const{ return _position; }
        const T& velocity() const{ return _velocity; }
        const numeric_type mass() const{ return _mass; }

        void updatePosition(const T& update){ _position += update; }
        void updateVelocity(const T& update){ _velocity += update; }

    private:
        T _position;
        T _velocity;
        numeric_type _mass;
};

template<typename T> std::ostream& operator<<(std::ostream& os, const Body<T>& body){
    os << "mass: " << body.mass() << " | position: " << body.position() << " | velocity: " << body.velocity();
    return os;
}

template<typename T> bool is_close(const Body<T>& lhs, const Body<T>& rhs){
    return (
        is_close(lhs.position(), rhs.position()) &&
        is_close(lhs.velocity(), rhs.velocity()) &&
        is_close(lhs.mass(), rhs.mass())
    );
}

#endif
