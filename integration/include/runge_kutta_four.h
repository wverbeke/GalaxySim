// Fourth order Runge-Kutta integrator
#ifndef RungeKuttaFour_H
#define RungeKuttaFour_H

#include <vector>

#include "integrator_base.h"

template<typename BodyType> class RungeKuttaFour: public IntegratorBase<BodyType>{

    public:
        using numeric_type = typename BodyType::numeric_type;
        using vector_type = typename BodyType::vector_type;

        virtual void timeStep(StarSystem<BodyType>& star_system,
                              const numeric_type time_step) override
        {
            // During the first step, or if the size of the star system changes, the vector holding
            // the intermediate velocity updates must be set to the right size.
            if(star_system.size() != _k_vel_1.size()){
                _k_vel_1 = std::vector<vector_type>(star_system.size());
                _k_vel_2 = std::vector<vector_type>(star_system.size());
                _k_vel_3 = std::vector<vector_type>(star_system.size());
            }

            // Start by computing the forces at the initial positions.
            star_system.computeForcesAndPotential();
            for(std::size_t b{0U}; b < star_system.size(); ++b){
                star_system[b].updatePosition(star_system[b].velocity() * time_step / 2.);
                _k_vel_1[b] = star_system.acceleration(b) * time_step;
            }

            // The position updates happen in-place so some algebra is necessary to obtain the
            // equations used below from the normal Runge-Kutta update rules.
            star_system.computeForces();
            for(std::size_t b{0U}; b < star_system.size(); ++b){
                star_system[b].updatePosition(time_step * _k_vel_1[b] / 4.);
                _k_vel_2[b] = star_system.acceleration(b) * time_step;
            }
            star_system.computeForces();
            for(std::size_t b{0U}; b < star_system.size(); ++b){
                star_system[b].updatePosition(
                    time_step / 2. * ( star_system[b].velocity() + _k_vel_2[b] - _k_vel_1[b] / 2.)
                );
                _k_vel_3[b] = star_system.acceleration(b) * time_step;
            }
            star_system.computeForces();
            for(std::size_t b{0U}; b < star_system.size(); ++b){
                star_system[b].updatePosition(
                    time_step / 6. * (_k_vel_1[b] + _k_vel_3[b] - 2. * _k_vel_2[b])
                );
                star_system[b].updateVelocity(
                    1./6. * (_k_vel_1[b] + 2. * _k_vel_2[b] +
                    2. * _k_vel_3[b] + star_system.acceleration(b) * time_step)
                );
            }
        }

    private:

        // Intermediate update of the velocities of bodies.
        std::vector<vector_type> _k_vel_1;
        std::vector<vector_type> _k_vel_2;
        std::vector<vector_type> _k_vel_3;
};
#endif
