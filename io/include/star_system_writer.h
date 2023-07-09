#ifndef StarSystemWriter_H
#define StarSystemWriter_H

#include <cstddef>
#include "hdf5.h"

#include "body/include/star_system.h"
#include "numeric_types.h"



template<typename BodyType> class StarSystemWriter{
    public:
        StarSystemWriter(const std::size_t num_bodies, const std::string& output_path):

        StarSystemWriter(const StarSystemWriter&) = delete;
        StarSystemWriter(StarSystemWriter&&) = delete;
        StarSystemWriter& operator=(const StarSystemWriter&) = delete;
        StarSystemWriter& operator=(StarSystemWriter&&) = delete;

        ~StarSystemWriter();

        herr_t h5_status() const{ return _status; }
        herr_t write_star_system(const StarSystem<BodyType>&, const BodyType::numeric_type);

    private:
        std::size_t _num_bodies;
        hid_t _file_id;
        hid_t _dspace_id;
        hid_t _dset_id;
        herr_t _status;
        hsize_t _current_dims[2];
};


template<typename BodyType> StarSystemWriter<BodyType>::StarSystemWriter(const std::size_t num_bodies, const std::string& output_path):
    _num_bodies(_num_bodies)
{
    _file_id = H5Fcreate(output_path.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Each body has 7 numbers representing it: a mass, a position vector, and a velocity vector.
    // In addition we need to store a timestamp for each snapshot of the star system.
    _current_dims[0] = 1;
    _current_dims[1] = num_bodies*7 + 1;
    const hsize_t max_dims[2] = {H5S_UNLIMITED, _current_dims[1]};
    _dspace_id = H5Screate_simple(2, _current_dims, max_dims);

    const hsize_t chunk_dims = {1, _current_dims[1]};
    hid_t chuk_prop = H5Pcreate(H5P_DATASET_CREATE);
    _status = H5Pset_chunk(chuk_prop, 2, chunk_dims);

    _dset_id = H5Dcreate(_file_id, output_path.c_str(), h5_memory_type<BodyType::numeric_type>(), _dspace_id, H5P_DEFAULT, chuk_prop, H5P_DEFAULT);
}

StarSystemWriter::~StarSystemWriter(){
    
}

#endif
