#ifndef generate_random_vectors_H
#define generate_random_vectors_H

#include <random>
#include <stdexcept>

#include "../include/vector3D.h"

template<typename T> T single_random_vector_2D(std::mt19937& random_device, std::uniform_real_distribution<T>& random_dist){
    return Vector2D<typename T::value_type>{random_dist(random_device), random_dist(random_device)};
}


template<typename T> T single_random_vector_3D(std::mt19937& random_device, std::uniform_real_distribution<typename T::value_type>& random_dist){
    return Vector3D<typename T::value_type>(random_dist(random_device), random_dist(random_device), random_dist(random_device));
}


template <typename T> std::vector<T> _random_vectors(T (*single_random_vector)(std::mt19937&, std::uniform_real_distribution<typename T::value_type>&), size_t num_random_vectors, typename T::value_type min_val, typename T::value_type max_val, unsigned seed = 0){

    // Set up the random number generator.
    std::mt19937 random_device{seed};
    std::uniform_real_distribution<typename T::value_type> uniform(min_val, max_val);

    // Random vectors to return.
    std::vector<T> random_vectors(num_random_vectors);
    for(size_t i = 0; i < num_random_vectors; ++i){
        T vec = (*single_random_vector)(random_device, uniform);
        random_vectors[i] = vec;
    }
    return random_vectors;
}


template <typename T> std::vector<T> random_vectors_2D(size_t num_random_vectors, typename T::value_type min_val, typename T::value_type max_val, unsigned seed = 0){
    return _random_vectors<T>(&single_random_vector_2D, num_random_vectors, min_val, max_val, seed);
}


template <typename T> std::vector<T> random_vectors_3D(size_t num_random_vectors, typename T::value_type min_val, typename T::value_type max_val, unsigned seed = 0){
    return _random_vectors<T>(&single_random_vector_3D, num_random_vectors, min_val, max_val, seed);
}
#endif
