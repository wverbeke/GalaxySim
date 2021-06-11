#include <chrono>
#include <iostream>
#include <array>
#include <random>

#include "../include/force.h"
#include "../include/body.h"
#include "../include/gravitational_constant.h"
#include "../../vector/include/vector3D.h"


int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;
    vector_type position_body_1(10., 10., 10.);
    vector_type velocity_body_1(0., 0., 0.);
    numeric_type mass_body_1 = 1.;
    body_type body_1{position_body_1, velocity_body_1, mass_body_1};

    vector_type position_body_2(0., 0., 0.);
    vector_type velocity_body_2(0., 0., 0.);
    numeric_type mass_body_2 = 1.;
    body_type body_2{position_body_2, velocity_body_2, mass_body_2};

    GravityComputer<body_type> forceComputer(1.);
    forceComputer.force(body_1, body_2);

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

    GravityComputer<body_type> forceComputer_SI(gravity::kG_SI_D);
    auto t1 = std::chrono::high_resolution_clock::now();
    for(std::size_t i{0}; i < loop_size; ++i){
        forceComputer_SI.force(bodies[i], bodies[i + loop_size]);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = t2 - t1;
    std::cout << time.count() << " ms | for " << loop_size << " force computations." << std::endl;
    delete[] bodies;
}
