#ifndef StarSystemWriter_H
#define StarSystemWriter_H

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "hdf5.h"

#include "../../body/include/star_system.h"
#include "numeric_types.h"



template<typename BodyType> class StarSystemWriter{
    public:
        StarSystemWriter(const std::size_t num_bodies, const std::string& output_path);

        StarSystemWriter(const StarSystemWriter&) = delete;
        StarSystemWriter(StarSystemWriter&&) = delete;
        StarSystemWriter& operator=(const StarSystemWriter&) = delete;
        StarSystemWriter& operator=(StarSystemWriter&&) = delete;

        ~StarSystemWriter();

        herr_t h5_status() const{ return _status; }

        // Writing a star system requires the star system and a timestamp.
        herr_t write_star_system(const StarSystem<BodyType>&, const typename BodyType::numeric_type);

    private:
        std::size_t _num_bodies;
        hid_t _file_id;

        // TODO: Check if we can already close this during the constructor or if we need to keep it.
        hid_t _dspace_id;

        hid_t _mem_write_space_id;
        hid_t _dset_id;
        herr_t _status;
        hsize_t _current_dims[2];
        hsize_t _chunk_size[2];
};


template<typename BodyType> StarSystemWriter<BodyType>::StarSystemWriter(const std::size_t num_bodies, const std::string& output_path):
    _num_bodies(num_bodies)
{
    _file_id = H5Fcreate(output_path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Each body has 7 numbers representing it: a mass, a position vector, and a velocity vector.
    // In addition we need to store a timestamp for each snapshot of the star system.
    _current_dims[0] = 0;
    _current_dims[1] = num_bodies*7 + 1;
    const hsize_t max_dims[2] = {H5S_UNLIMITED, _current_dims[1]};
    _dspace_id = H5Screate_simple(2, _current_dims, max_dims);

    _chunk_size[0] = 1;
    _chunk_size[1] = _current_dims[1];
    hid_t chunk_prop = H5Pcreate(H5P_DATASET_CREATE);
    _status = H5Pset_chunk(chunk_prop, 2, _chunk_size);

    // TODO: Fix type hardcoding here.
    _dset_id = H5Dcreate(_file_id, output_path.c_str(), H5T_IEEE_F64BE, _dspace_id, H5P_DEFAULT, chunk_prop, H5P_DEFAULT);

    // dataspace that will be used for writing star systems to disk one by one.
    // The chunk size corresponds to a single star system.
    _mem_write_space_id = H5Screate_simple(2, _chunk_size, max_dims);
}

template <typename BodyType> StarSystemWriter<BodyType>::~StarSystemWriter(){
    _status = H5Dclose(_dset_id);
    _status = H5Sclose(_dspace_id);
    _status = H5Sclose(_mem_write_space_id);
    _status = H5Fclose(_file_id);
}


template <typename BodyType> herr_t StarSystemWriter<BodyType>::write_star_system(const StarSystem<BodyType>& star_system, const typename BodyType::numeric_type timestamp){

    const std::size_t write_size = (_num_bodies*7 + 1);
    typename BodyType::numeric_type write_array[write_size];
    std::size_t write_index = 0;
    for(const auto& body: star_system){
        write_array[write_index] = body.mass();
        write_array[write_index + 1] = body.position().x();
        write_array[write_index + 2] = body.position().y();
        write_array[write_index + 3] = body.position().z();
        write_array[write_index + 4] = body.velocity().x();
        write_array[write_index + 5] = body.velocity().y();
        write_array[write_index + 6] = body.velocity().z();
        write_index += 7;
    }
    write_array[write_size - 1] = timestamp;

    hsize_t dataset_offset[2] = {_current_dims[0], 0};
    _current_dims[0] += 1;
    _status = H5Dset_extent(_dset_id, _current_dims);

    // TODO: Check if this line can be simplified.
    hid_t file_space = H5Dget_space(_dset_id);

    // After this call file_space refers to a certain hyperslab in the file.
    _status = H5Sselect_hyperslab(file_space, H5S_SELECT_SET, dataset_offset, NULL, _chunk_size, NULL);

    // TODO: Fix type hardcoding here.
    _status = H5Dwrite(_dset_id, H5T_NATIVE_DOUBLE, _mem_write_space_id, file_space, H5P_DEFAULT, write_array);

    return _status;
}
#endif
