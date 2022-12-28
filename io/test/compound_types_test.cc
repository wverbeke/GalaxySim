#include "../include/compound_types.h"
#include "../../vector/include/vector2D.h"
#include "../../vector/include/vector3D.h"

int main(){
    h5_Vector2D<float>();
    h5_Vector2D<double>();

    h5_Vector3D<float>();
    h5_Vector3D<double>();

    h5_vector_type<Vector2D<float>>();
    h5_vector_type<Vector2D<double>>();

    h5_vector_type<Vector3D<float>>();
    h5_vector_type<Vector3D<double>>();
    return 0;
}
