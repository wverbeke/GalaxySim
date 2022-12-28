#ifndef compound_types_H
#define compound_types_H

#include <stdexcept>

#include "hdf5.h"

#include "numeric_types.h"
#include "../../vector/include/vector2D.h"
#include "../../vector/include/vector3D.h"

template<typename T> hid_t h5_Vector2D(){
    hid_t vector2d_type_id = H5Tcreate(H5T_COMPOUND, sizeof(Vector2D<T>));
    H5Tinsert(vector2d_type_id, "x", offsetof(Vector2D<T>, _x), h5_memory_type<T>());
    H5Tinsert(vector2d_type_id, "y", offsetof(Vector2D<T>, _y), h5_memory_type<T>());
    return vector2d_type_id;
}


template<typename T> hid_t h5_Vector3D(){
    hid_t vector3d_type_id = H5Tcreate(H5T_COMPOUND, sizeof(Vector3D<T>));
    H5Tinsert(vector3d_type_id, "x", offsetof(Vector3D<T>, _x), h5_memory_type<T>());
    H5Tinsert(vector3d_type_id, "y", offsetof(Vector3D<T>, _y), h5_memory_type<T>());
    H5Tinsert(vector3d_type_id, "z", offsetof(Vector3D<T>, _z), h5_memory_type<T>());
    return vector3d_type_id;
}


template<typename T> hid_t h5_vector_type(){
    throw std::domain_error("Unknown vector type for conversion to HDF5 compound type.");
}


template<> hid_t h5_vector_type<Vector2D<float>>(){
    return h5_Vector2D<float>();
}


template<> hid_t h5_vector_type<Vector2D<double>>(){
    return h5_Vector2D<double>();
}


template<> hid_t h5_vector_type<Vector3D<float>>(){
    return h5_Vector3D<float>();
}


template<> hid_t h5_vector_type<Vector3D<double>>(){
    return h5_Vector3D<double>();
}
#endif
