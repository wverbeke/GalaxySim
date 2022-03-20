#ifndef DirectSumForceComputer_H
#define DirectSumForceComputer_H

#include <algorithm>

#include "force_computer_base.h"


template<typename BodyType> class DirectSumForceComputer: public ForceComputerBase<BodyType>{
    public:
        using vector_type = typename ForceComputerBase<BodyType>::vector_type;
        using ForceComputerBase<BodyType>::_forces;
        using ForceComputerBase<BodyType>::ForceComputerBase;
        using ForceComputerBase<BodyType>::pairwiseForce;

    protected:
        virtual void computeForcesImpl(const StarSystem<BodyType>& star_system) override{
            this->resetForces();
            for(std::size_t i{0}; i < star_system.size() - 1; ++i){
                for(std::size_t j{i + 1}; j < star_system.size(); ++j){

                    // Compute the force from body i on body j.
                    vector_type pairwise_force{pairwiseForce(star_system[i], star_system[j])};
                    _forces[i] += pairwise_force;

                    // The force of body j on body i is the inverse of that of body i on body j.
                    _forces[j] -= pairwise_force;
                }
            }
        }
};

#endif
