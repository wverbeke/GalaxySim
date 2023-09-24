// Second order Runge-Kutta integrator
#ifndef RungeKuttaTwo_H
#define RungeKuttaTwo_H

#include <vector>

#include "integrator_base.h"

template<typename BodyType> class RungeKuttaTwo: public IntegratorBase<BodyType>{

    public:
        using numeric_type = typename BodyType::numeric_type;
        using vector_type = typename BodyType::vector_type;

        virtual void timeStep(StarSystem<BodyType>& star_system,
                              ForceComputerBase<BodyType>& force_computer,
                              const numeric_type time_step) override
        {
            // During the first step, or if the size of the star system changes, the vector holding
            // the intermediate velocity updates must be set to the right size.
            if(star_system.size() != _k_vel.size()){
                _k_vel = std::vector<vector_type>(star_system.size());
            }

            // Start by computing the forces at the initial positions.
            star_system.computeForcesAndPotential(force_computer);
            for(std::size_t b{0U}; b < star_system.size(); ++b){

                // Part of the velocity update depends on the acceleration at the starting point of
                // the step.
                _k_vel[b] = star_system.acceleration(force_computer, b) * time_step;

                // The update to the position is of the form:
                // x_new = x + (kx1 + kx2)/2
                // kx1 = time_step * v 
                // kx2 = time_step * (v + kv1)
                // so: x_new = x + v + kv1/2 
                // However the update of the velocity depends on the force at x + kx1, so we do
                // that partial update first and then add kv1/2 in the next step.
                star_system[b].updatePosition(star_system[b].velocity() * time_step);
            }

            // We don't need to compute the potential energy at the intermediate stage of the
            // integrator.
            star_system.computeForces(force_computer);
            for(std::size_t b{0U}; b < star_system.size(); ++b){
                star_system[b].updatePosition( _k_vel[b] / 2.);
                star_system[b].updateVelocity( (_k_vel[b] + time_step*star_system.acceleration(force_computer, b))/2.);
            }
        }


    private:

        // Intermediate update of the velocities of bodies.
        std::vector<vector_type> _k_vel;
};
#endif
