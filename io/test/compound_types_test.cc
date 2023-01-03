#include <vector>
#include <iostream>

#include "../include/compound_types.h"
#include "../../vector/include/vector2D.h"
#include "../../vector/include/vector3D.h"
#include "../../vector/include/vector_math.h"
#include "../../vector/include/generate_random_vectors.h"

template<typename T> void write_vectors(const std::string& file_name, const std::vector<T>& vectors){

    // Open the HDF5 file.
    hid_t file_id = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hsize_t data_dims[1] = {vectors.size()};
    hid_t dataspace_id = H5Screate_simple(1, data_dims, NULL);

    // Get the appropriate compound datatype.
    hid_t vector_type_id = h5_vector_type<T>();

    // Make the dataset and write to file.
    hid_t dataset_id = H5Dcreate(file_id, "vectors", vector_type_id, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Dwrite(dataset_id, vector_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vectors[0]);

    // Release resources.
    status = H5Dclose(dataset_id);
    status = H5Tclose(vector_type_id);
    status = H5Sclose(dataspace_id);
    status = H5Fclose(file_id);
}


std::vector<Vector3D<double>> read_vectors(const std::string& file_name){
    hid_t file_id = H5Fopen(file_name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dataset_id = H5Dopen(file_id, "vectors", H5P_DEFAULT);
    hid_t dataspace_id = H5Dget_space(dataset_id);
    hsize_t dims[1] = {0};
    H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
    std::vector<Vector3D<double>> read_vector(dims[0]);
    hid_t vector_type_id = h5_vector_type<Vector3D<double>>();
    herr_t status = H5Dread(dataset_id, vector_type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, &read_vector[0]);
    return read_vector;
}


int main(){

    // Try initializing all the templates.
    h5_Vector2D<float>();
    h5_Vector2D<double>();

    h5_Vector3D<float>();
    h5_Vector3D<double>();

    h5_vector_type<Vector2D<float>>();
    h5_vector_type<Vector2D<double>>();

    h5_vector_type<Vector3D<float>>();
    h5_vector_type<Vector3D<double>>();

    
    auto random_vectors = random_vectors_3D<Vector3D<double>>(100, 0.0, 10.0, 0);
    write_vectors("h5_vector_test.h5", random_vectors);
    auto vectors_from_file = read_vectors("h5_vector_test.h5");

    if(random_vectors.size() != vectors_from_file.size()){
        throw std::length_error(std::to_string(random_vectors.size()) + " were written to file, while only " + std::to_string(vectors_from_file.size()) + " vectors were read.");
    }

    for(size_t i = 0; i < random_vectors.size(); ++i){
        if(abs(random_vectors[i] - vectors_from_file[i]) > 1e-8){
            throw std::runtime_error("Incompatible vector values.");
        }
    }

    return 0;
}
