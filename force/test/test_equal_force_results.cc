#include <iostream>
#include <random>
#include<string>

#include "../include/direct_sum_force_computer.h"
#include "../../body/include/body.h"
#include "../../vector/include/vector3D.h"

int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;
    DirectSumForceComputer<body_type> force_computer(1.);

    // Generate <loop_size> random bodies.
    // The force between these bodies will be computed two-by-two.
    // The reason for not using the same body repeatedly is that numeric differences in the body
    // positions and masses might change the runtime of the force computation.
    constexpr std::size_t loop_size = 1e6;
    std::mt19937 random_device{0};
    std::uniform_real_distribution<numeric_type> uniform(0., 10.);
    std::vector<body_type> bodies(loop_size*2);
    for(std::size_t i = 0; i < loop_size * 2; ++i){
        vector_type pos = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        vector_type vel = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        numeric_type mass = uniform(random_device);
        bodies[i] = body_type(pos, vel, mass);
    }

    // Ensure that both a force-only and force-plus-potential calculation gives the same force result.
    for(std::size_t i{0}; i < loop_size; ++i){
        vector_type force = force_computer.pairwiseForce(bodies[i], bodies[i + loop_size]);
        auto force_potential = force_computer.pairwiseForceAndPotential(bodies[i], bodies[i + loop_size]);
        vector_type diff = (force - force_potential.first);
        if(abs(diff) > 1e-10){
            std::string error_message = "Different result from separate force computation and force computation with potential energy.\n";
            throw std::runtime_error(error_message);
        }
    }

}
