cmake_minimum_required(VERSION 3.16.3)

project(GalaxySim)

add_executable(time_force_computation force/test/time_force_computation.cc)
add_executable(test_equal_force_results force/test/test_equal_force_results.cc)
