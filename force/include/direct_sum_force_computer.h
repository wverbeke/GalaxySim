#ifndef DirectSumForceComputer_H
#define DirectSumForceComputer_H

#include <algorithm>

#include "force_computer_base.h"


template<typename BodyType> class DirectSumForceComputer: public ForceComputerBase<BodyType>{

    public:
        using ForceComputerBase<BodyType>::pairwiseForce;
        using ForceComputerBase<BodyType>::pairwiseForceAndPotential;

    protected:
        virtual void computeForcesImpl(const StarSystem<BodyType>& star_system) override{
            computeAllTerms(star_system, &DirectSumForceComputer<BodyType>::addForceComponent);
        }

        virtual void computeForcesAndPotentialImpl(const StarSystem<BodyType>& star_system) override{
            computeAllTerms(star_system, &DirectSumForceComputer<BodyType>::addForceAndPotentialComponent);
        }

    private:
        using vector_type = typename ForceComputerBase<BodyType>::vector_type;
        using ForceComputerBase<BodyType>::_forces;
        using ForceComputerBase<BodyType>::_potential;
        using ForceComputerBase<BodyType>::ForceComputerBase;

        void addForceComponent(
            const StarSystem<BodyType>& star_system,
            const std::size_t lhs_index,
            const std::size_t rhs_index
        ){
            // Compute the force from body i on body j.
            vector_type pairwise_force{
                pairwiseForce(star_system[lhs_index], star_system[rhs_index])
            };
            _forces[lhs_index] += pairwise_force;

            // The force of body j on body i is the inverse of that of body i on body j.
            _forces[rhs_index] -= pairwise_force;
        }

        void addForceAndPotentialComponent(
            const StarSystem<BodyType>& star_system,
            const std::size_t lhs_index,
            const std::size_t rhs_index
        ){
            // Compute the force from body i on body j and the contribution to the potential energy.
            // The first component of the returned pair is the force and the second the potential.
            auto force_and_potential = 
                pairwiseForceAndPotential(star_system[lhs_index], star_system[rhs_index]);
            _forces[lhs_index] += force_and_potential.first;
            _forces[rhs_index] -= force_and_potential.first;
            
            // The potential energy body i has due to body j's gravitation is the same as the
            // potential energy body j has due to body i's gravitation. The potential energy for
            // each body has a factor 1/2 preceding it. We can thus simply sum a potential energy
            // without this factor and account for both bodies in the pair.
            _potential += force_and_potential.second;
        }

        void computeAllTerms(
            const StarSystem<BodyType>& star_system,
            void (DirectSumForceComputer<BodyType>::*addComponent)(const StarSystem<BodyType>& star_system, const std::size_t, const std::size_t)
        ){
            for(std::size_t i{0U}; i < star_system.size() - 1; ++i){
                for(std::size_t j{i+1}; j < star_system.size(); ++j){
                    (this->*addComponent)(star_system, i, j);
                }
            }

        }
};

#endif
