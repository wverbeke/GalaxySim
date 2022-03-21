// Interface for computing forces.
// The force computation is done by the method computeForces and the result should be stored
#ifndef ForceComputer_H
#define ForceComputer_H

// For std::pair.
#include <utility>

#include "../../body/include/body.h"
#include "../../body/include/star_system.h"
#include "../../vector/include/vector_math.h"

template<typename BodyType> class StarSystem;
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

            // Clean up after the previous force calculation.
            cleanForces(star_system);

            // Do the actual force computation.
            computeForcesImpl(star_system);
        }

        // Precompute the forces exerted on each body in the star system and the potential energy
        // of the system.
        void computeForcesAndPotential(const StarSystem<BodyType>& star_system){
            cleanForces(star_system);
            _potential = 0.;
            computeForcesAndPotentialImpl(star_system);
        }

        // Retrieve the force being exerted on one body by the other bodies.
        // The gravitational constant gets applied at this step so that the rest of the
        // implementation is agnostic of it.
        vector_type totalForce(const std::size_t body_index) const{
            return _G * _forces.at(body_index);
        }

        numeric_type potentialEnergy() const{
            return _potential;
        }

        // Compute the unit agnostic pairwise force between two bodies.
        // This does not include the gravitational constant because it would be a waste of compute
        // to multiply each force component by it. Instead the total force can be multiplied by
        // this number.
        vector_type pairwiseForce(const BodyType& lhs, const BodyType& rhs) const{
            vector_type position_difference = (rhs.position() - lhs.position());

            // Avoid repeating the vector subtraction here.
            numeric_type distance = abs(position_difference);

            // Use pre-computed distance to avoid having to compute an inner product of vectors or
            // another square root.
            return (lhs.mass() * rhs.mass() * position_difference) / (distance*distance*distance);
        }

        // Conservation of energy can be used to check the correctness of the simulation over many
        // time steps. The computation of the gravitational potential energy also requires the
        // sumation of pairwise terms very similar to the force. Both the gravitational potential
        // energy and force can be computed with only a single square root operation as done below.
        std::pair<vector_type, numeric_type> pairwiseForceAndPotential(
            const BodyType& lhs,
            const BodyType& rhs) const
        {
            vector_type position_difference = (rhs.position() - lhs.position());
            numeric_type distance = abs(position_difference);
            numeric_type potential = lhs.mass() * rhs.mass() / distance;
            return {potential * position_difference / (distance*distance), potential};
        }

    protected:
        // Function that does the actual force computation.
        // Override this function to implement a new ForceComputer;
        virtual void computeForcesImpl(const StarSystem<BodyType>&) = 0;
        virtual void computeForcesAndPotentialImpl(const StarSystem<BodyType>&) = 0;

        // Reset all forces to be 0 vectors.
        void resetForces(){
            std::fill(_forces.begin(), _forces.end(), vector_type());
        }

        std::vector<vector_type> _forces{};
        numeric_type _potential = 0.;

    private:

        // Gravitational constant used in the calculations.
        numeric_type _G = 1.;

        // Some cleanup needed before every new force calculation.
        void cleanForces(const StarSystem<BodyType>& star_system){

            // Re-initialize the vector of _forces if required by the size of the star system.
            if(_forces.size() != star_system.size()){
                _forces = std::vector<vector_type>(star_system.size());

            // The previous force computation should always be erased so that forces components can
            // be added starting from zero.
            } else {
                resetForces();
            }
       }
};

#endif
