#include <chrono>
#include <iostream>
#include <random>

#include "../include/forward_euler.h"
#include "../include/runge_kutta_two.h"
#include "../include/runge_kutta_four.h"
#include "../../force/include/direct_sum_force_computer.h"
#include "../../body/include/body.h"
#include "../../body/include/star_system.h"
#include "../../vector/include/vector3D.h"


template <typename IntegratorType, typename BodyType> void time_integrator(const std::string& integrator_name){
    using vector_type = typename BodyType::vector_type;
    using numeric_type = typename BodyType::numeric_type;

    // Generate <loop_size> random bodies.
    // The force between these bodies will be computed two-by-two.
    // The reason for not using the same body repeatedly is that numeric differences in the body
    // positions and masses might change the runtime of the force computation.
    constexpr std::size_t num_bodies = 1e3;
    std::mt19937 random_device{0};
    std::uniform_real_distribution<numeric_type> uniform(0., 10.);
    std::vector<BodyType> bodies(num_bodies);
    for(std::size_t i = 0; i < num_bodies; ++i){
        vector_type pos = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        vector_type vel = vector_type(uniform(random_device), uniform(random_device), uniform(random_device));
        numeric_type mass = uniform(random_device);
        bodies[i] = BodyType(pos, vel, mass);
    }

    DirectSumForceComputer<BodyType> force_computer(1.);
    StarSystem<BodyType> star_system(bodies);

    // The Integrator is the template type
    IntegratorType integrator;
    constexpr numeric_type time_step = 1.;
    constexpr std::size_t num_steps = 1000.;

    auto t1 = std::chrono::high_resolution_clock::now();
    for(std::size_t i{0}; i < 1000; ++i){
        integrator.timeStep(star_system, force_computer, time_step);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    // Calculate the time it took to compute all pairwise forces.
    std::chrono::duration<double, std::milli> time = t2 - t1;
    std::cout << "Elapsed time = " << time.count() << " ms | for " << num_steps << " time steps with " << num_bodies << " bodies using the " << integrator_name << " integrator." << std::endl;
}

int main(){
    using body_type = Body<Vector3D<double>>;
    time_integrator<ForwardEuler<body_type>, body_type>("forward Euler");
    time_integrator<RungeKuttaTwo<body_type>, body_type>("Runge-Kutta 2");
    time_integrator<RungeKuttaFour<body_type>, body_type>("Runge-Kutta 4");
}
