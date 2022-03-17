#include <chrono>
#include <iostream>
#include <array>
#include <random>

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
    body_type* bodies = new body_type[loop_size*2];
    for(std::size_t i = 0; i < loop_size * 2; ++i){
        vector_type pos = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        vector_type vel = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        numeric_type mass = uniform(random_device);
        bodies[i] = body_type(pos, vel, mass);
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    for(std::size_t i{0}; i < loop_size; ++i){
        force_computer.pairwiseForce(bodies[i], bodies[i + loop_size]);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    // Calculate the time it took to compute all pairwise forces.
    std::chrono::duration<double, std::milli> time = t2 - t1;
    std::cout << "Elapsed time = " << time.count() << " ms | for " << loop_size << " force computations." << std::endl;
    delete[] bodies;
}
