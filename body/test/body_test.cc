#include "../include/body.h"
#include "../../vector/include/Vector2D.h"
#include "../../vector/include/Vector3D.h"

int main(){
    Body<Vector2D<float>> body_2D_float;
    Body<Vector2D<double>> body_2D_double;
    Body<Vector3D<float>> body_3D_float;
    Body<Vector3D<double>> body_3D_double;

    body_3D_float.position();
    body_3D_float.velocity();
    body_3D_float.mass();

    Vector3D<float> position_update{1., 2., 3.};
    body_3D_float.updatePosition(position_update);

    Vector3D<float> velocity_update{3., 2., 1.};
    body_3D_float.updateVelocity(velocity_update);
}
