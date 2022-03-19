// Interface for computing forces.
// The force computation is done by the method computeForces and the result should be stored
#ifndef ForceComputer_H
#define ForceComputer_H

#include "../../body/include/body.h"
#include "../../body/include/star_system.h"
#include "../../vector/include/vector_math.h"

// Unit agnostic computation of force the left-hand side body exerts on the right-hand side body.
// The result of this calculation has to be multiplied with a suitable gravitational constant.
template<typename T> T _gravitationalForce(const Body<T>& lhs, const Body<T>& rhs){
    T position_difference = (rhs.position() - lhs.position());

    // Avoid repeating the vector subtraction here.
    typename T::value_type distance = abs(position_difference);

    // Use pre-computed distance to avoid having to compute an inner product of vectors or another
    // square root.
    return (lhs.mass() * rhs.mass() * position_difference) / (distance*distance*distance);
}


template<typename BodyType> class ForceComputerBase{

    public:
        using numeric_type = typename BodyType::numeric_type;
        using vector_type = typename BodyType::vector_type;

        // A ForceComputer can either be initialized with a particular gravitational constant or
        // with by defauly with a gravitational constant of 1.
        ForceComputerBase() = default;
        ForceComputerBase(const numeric_type G): _G(G) {}

        // Only one instance of a ForceComputer is needed, so it should not be copied.
        ForceComputerBase(const ForceComputerBase&) = delete;
        ForceComputerBase(ForceComputerBase&&) = delete;
        ForceComputerBase& operator=(const ForceComputerBase&) = delete;
        ForceComputerBase& operator=(ForceComputerBase&&) = delete;

        // Precompute the forces exerted on each body in the star system.
        void computeForces(const StarSystem<BodyType>& star_system){

            // Re-initialize the vector of _forces if required by the size of the star system.
            if(_forces.size() != star_system.size()){
                _forces = std::vector<vector_type>(star_system.size());
            } 

            // Do the actual force computation.
            computeForcesImpl(star_system);
        }

        // Retrieve the force being exerted on one body by the other bodies.
        // The gravitational constant gets applied at this step so that the rest of the
        // implementation is agnostic of it.
        vector_type totalForce(const std::size_t body_index){
            return _forces.at(body_index);
        }

        // Compute the pairwise force between two bodies.
        vector_type pairwiseForce(const BodyType& lhs, const BodyType& rhs) const{
            return _G * _gravitationalForce(lhs, rhs);
        }

    protected:

        // Function that does the actual force computation.
        // Override this function to implement a new ForceComputer;
        virtual void computeForcesImpl(const StarSystem<BodyType>&) = 0;

        // Reset all forces to be 0 vectors.
        void resetForces(){
            std::fill(_forces.begin(), _forces.end(), vector_type());
        }

        std::vector<vector_type> _forces;

    private:
        numeric_type _G = 1.;
};

#endif
