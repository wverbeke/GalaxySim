#ifndef StarSystemReader_H
#define StarSystemReader_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "hdf5.h"

#include "../../body/include/star_system.h"
#include "../../body/include/body.h"
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
        StarSystem at(const std::size_t) const;

    private:
        hid_t _file_id;
        hid_t _dset_id;
        std::size_t _num_timestamps;

        // This should be generalized to 2D vectors as well.
        std::size_t _num_bodies;

        // TODO: StarSystem should probably be refactored not to depend on this.
        DirectSumForceComputer<BodyType> _dummy_force_computer;
};


template<typename BodyType> StarSystemReader<BodyType>::StarSystemReader(const std::string& read_path):
    _file_id(H5Fopen(read_path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT)),
    _dset_id(H5Dopen(_file_id, DSET_NAME, H5P_DEFAULT),
    _dummy_force_computer()
{
    hid_t dspace_id = H5Dget_space(_dset_id);
    const int ndims{H5Sget_simple_extent_ndims(dspace_id)};
    hsize_t dims[ndims]; 
    H5Sget_simple_extent_dims(dspace_id, dims, NULL);
    _num_timestamps = dims[0];
    
    if dims[1] % 7 != 0{
        throw std::length_error("The numbers representing a snapshot of a star system must be 7 numbers (3 position, 3 velocities, 1 mass) per body and must therefore be divisible by 7");
    }

    _num_bodies = dims
}

template<typename BodyType> StarSystemReader<BodyType>::~StarSystemReader(){
    H5Fclose(_file_id);
}

template<typename BodyType> StarSystem StarSystemReader<BodyType>::at(const std::size_t index){

}
#endif
