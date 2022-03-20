#ifndef ForwardEuler_H
#define ForwardEuler_H

#include "integrator_base.h"

#include <iostream>

template<typename BodyType> class ForwardEuler: IntegratorBase<BodyType>{

    public:
        using numeric_type = typename BodyType::numeric_type;
        
        virtual void timeStep(StarSystem<BodyType>& star_system,
                              ForceComputerBase<BodyType>& force_computer,
                              const numeric_type time_step) const override
        {
            // Compute forces
            force_computer.computeForces(star_system);
            for(std::size_t b{0}; b < star_system.size(); ++b){
                BodyType& body = star_system[b];
                body.updatePosition(time_step * body.velocity());        
                body.updateVelocity(time_step * force_computer.totalForce(b)/body.mass());
            }
        }
};

#endif
