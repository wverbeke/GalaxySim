#ifndef numeric_types_H
#define numeric_types_H

#include <stdexcept>
#include "hdf5.h"

template<typename T> hid_t get_h5_memory_type(){
    throw std::domain_error("Invalid type for I/O to HDF5 file.");
}


template<> hid_t get_h5_memory_type<float>(){
    return H5T_NATIVE_FLOAT;
}


template<> hid_t get_h5_memory_type<double>(){
    return H5T_NATIVE_DOUBLE;
}


template<typename T> hid_t get_h5_file_type(){
    throw std::domain_error("Invalid type for I/O to HDF5 file.");
}


template<> hid_t get_h5_file_type<float>(){
    return H5T_IEEE_F32BE;
}


template<> hid_t get_h5_file_type<double>(){
    return H5T_IEEE_F64BE;
}
#endif
