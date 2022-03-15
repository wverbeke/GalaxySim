#ifndef force_H
#define force_H

#include "body.h"
#include "../../vector/include/vector_math.h"

// Unit agnostic computation of force the left-hand side body exerts on the right-hand side body.
// The result of this calculation has to be multiplied with a suitable gravitational constant.
template<typename T> T _gravitationalForce(const Body<T>& lhs, const Body<T>& rhs){
    T position_difference = (lhs.position() - rhs.position());

    // Avoid repeating the vector subtraction here.
    typename T::value_type distance = abs(position_difference);

    // Use pre-computed distance to avoid having to compute an inner product of vectors or another
    // square root.
    return (lhs.mass() * rhs.mass() * position_difference) / (distance*distance*distance);
}


// Interface class for force computation.
// A gravitational constant is specified and stored in the class, and the  results of a generic
// force computation are multiplied by this constant.
template<typename T> class InteractionComputer{

    using numeric_type = typename T::numeric_type;
    using vector_type = typename T::vector_type;

    public:
        InteractionComputer() = default;
        InteractionComputer(const numeric_type G): _G(G) {}

        // No copies of an InteractionComputer should be made since it does not contain any
        // changing data.
        InteractionComputer(const InteractionComputer<T>&) = delete;
        InteractionComputer(InteractionComputer<T>&&) = delete;
        InteractionComputer& operator=(const InteractionComputer<T>&) = delete;
        InteractionComputer& operator=(InteractionComputer<T>&&) = delete;

        // Compute the force of the left-hand side body on the right-hand side body.
        vector_type force(const T& lhs, const T& rhs){
            return (_G * _gravitationalForce(lhs, rhs));
        }

    private:
        numeric_type _G = 1.;
};

#endif
