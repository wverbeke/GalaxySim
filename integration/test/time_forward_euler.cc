#include <chrono>
#include <iostream>
#include <random>

#include "../include/forward_euler.h"
#include "../../force/include/direct_sum_force_computer.h"
#include "../../body/include/body.h"
#include "../../body/include/star_system.h"
#include "../../vector/include/vector3D.h"

int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;

    // Generate <loop_size> random bodies.
    // The force between these bodies will be computed two-by-two.
    // The reason for not using the same body repeatedly is that numeric differences in the body
    // positions and masses might change the runtime of the force computation.
    constexpr std::size_t num_bodies = 1e3;
    std::mt19937 random_device{0};
    std::uniform_real_distribution<numeric_type> uniform(0., 10.);
    std::vector<body_type> bodies(num_bodies);
    for(std::size_t i = 0; i < num_bodies; ++i){
        vector_type pos = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        vector_type vel = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        numeric_type mass = uniform(random_device);
        bodies[i] = body_type(pos, vel, mass);
    }

    DirectSumForceComputer<body_type> force_computer(1.);
    StarSystem<body_type> star_system(bodies, force_computer);
    ForwardEuler<body_type> forward_euler_integrator;
    constexpr numeric_type time_step = 1.;
    constexpr std::size_t num_steps = 1000.;

    auto t1 = std::chrono::high_resolution_clock::now();
    for(std::size_t i{0}; i < 1000; ++i){
        forward_euler_integrator.timeStep(star_system, time_step);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    // Calculate the time it took to compute all pairwise forces.
    std::chrono::duration<double, std::milli> time = t2 - t1;
    std::cout << "Elapsed time = " << time.count() << " ms | for " << num_steps << " time steps with " << num_bodies << " bodies." << std::endl;
}
