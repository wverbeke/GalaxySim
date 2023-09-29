#ifndef StarSystemReader_H
#define StarSystemReader_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "hdf5.h"

#include "../../body/include/star_system.h"
#include "../../body/include/body.h"
#include "../../vector/include/vector3D.h"
#include "../../force/include/direct_sum_force_computer.h"
#include "numeric_types.h"

// The only dependency on this file is the name of the dataset.
#include "star_system_writer.h"


//TODO Would it not be cleaner to template this in the StarSystem type?
template<typename BodyType> class StarSystemReader{
    public:

        // TODO: can we read the number of bodies from the file somehow?
        // TODO: can we derive the type information and vector dimensionality somehow?
        StarSystemReader(const std::string& read_path);

        StarSystemReader(const StarSystemReader&) = delete;
        StarSystemReader(StarSystemReader&&) = delete;
        StarSystemReader& operator=(const StarSystemReader&) = delete;
        StarSystemReader& operator=(StarSystemReader&&) = delete;

        ~StarSystemReader();

        herr_t h5_status() const{ return _status; }

        // Number of timestamps stored in the file.
        std::size_t num_timestamps() const{ return _num_timestamps; }

        // Element access.
        // No reference is returned since we initialize a StarSystem object from a read array.
        // How should this work with interpolation between timestamps for visualization? Maybe it
        // is fine to interpolate between StarSystem objects at each timestamp there.
        std::pair<double, StarSystem<BodyType>> at(const std::size_t) const;

    private:
        hid_t _file_id;
        hid_t _dset_id;
        hid_t _dspace_id;
        hsize_t _block_size[2];
        herr_t _status;
        std::size_t _num_timestamps;

        // For memory space to read to reading.
        // One block will be read into a 1D array in memory.
        hid_t _mem_space_id;
        hsize_t _mem_offset[1] = {0};

        // This should be generalized to 2D vectors as well.
        std::size_t _num_bodies;
};


template<typename BodyType> StarSystemReader<BodyType>::StarSystemReader(const std::string& read_path):
    _file_id(H5Fopen(read_path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT)),
    _dset_id(H5Dopen(_file_id, DSET_NAME, H5P_DEFAULT)),
    _dspace_id(H5Dget_space(_dset_id))
{
    // Read the dimensionality from the dataspace.
    const int ndims{H5Sget_simple_extent_ndims(_dspace_id)};
    hsize_t dims[ndims]; 
    _status = H5Sget_simple_extent_dims(_dspace_id, dims, NULL);
    _num_timestamps = dims[0];

    // TODO: Generalize this to 2D vectors.
    // There are 7 numbers stored per body, and one timestamp.
    if((dims[1] - 1) % 7 != 0){
        throw std::length_error("The numbers representing a snapshot of a star system must be 7 numbers (3 position, 3 velocities, 1 mass) per body and must therefore be divisible by 7");
    }
    _num_bodies = (dims[1] - 1)/7;

    // Size of the read block for a single star system time point.
    _block_size[0] = 1;
    _block_size[1] = dims[1];

    // Initialize the memory data space for the reading.
    // The data at each timestamp is read into a 1D array in memory.
    hsize_t mem_block_size[1] = {_block_size[1]};
    _mem_space_id = H5Screate_simple(1, mem_block_size, NULL);
}

template<typename BodyType> StarSystemReader<BodyType>::~StarSystemReader(){
    H5Fclose(_file_id);
}

template<typename BodyType> std::pair<double, StarSystem<BodyType>> StarSystemReader<BodyType>::at(const std::size_t time_index) const{
    // Select the correct file hyperslab to read out the requested timestamp.
    hsize_t read_offset[2] = {time_index, 0};
    H5Sselect_hyperslab(_dspace_id, H5S_SELECT_SET, read_offset, NULL, _block_size, NULL);

    // Read the data into an array.
    // TODO: Generalize this to other types.
    double read_array[_block_size[1]];
    H5Dread(_dset_id, H5T_NATIVE_DOUBLE, _mem_space_id, _dspace_id, H5P_DEFAULT, read_array);

    // Convert the read array into a star system.
    // TODO: Generalize this to other vector types.
    double timestamp = read_array[_block_size[1] - 1];
    std::vector<BodyType> bodies(_num_bodies);
    for(std::size_t b{0}; b < _num_bodies; ++b){
        std::size_t s{b*7};
        Vector3D<double> pos{read_array[s], read_array[s + 1], read_array[s + 2]};
        Vector3D<double> vel{read_array[s + 3], read_array[s + 4], read_array[s + 5]};
        double mass{read_array[s + 6]};
        bodies[b] = BodyType{pos, vel, mass};
    }
    return {timestamp, StarSystem<BodyType>{bodies}};
}
#endif
