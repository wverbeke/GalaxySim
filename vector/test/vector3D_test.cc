#include "../include/vector3D.h"
#include "../include/vector_math.h"

#include <iostream>

int main(){

    Vector3D<float> v{1.0f,2.0f,3.0f};
    Vector3D<float> w{3.0f,2.0f,1.0f};

    std::cout << v*w << std::endl;
    std::cout << cross(v, w) << std::endl;

    v+=1.0f;
    1.0f+v;
    v+1.0f;
    v+=w;
    v+w;

    v-=1.0f;
    1.0f-v;
    v-1.0f;
    v-=w;
    v-w;

    v*=1.0f;
    1.0f*v;
    v*1.0f;
    v*w;

    v/=1.0f;
    1.0f/v;
    v/1.0f;
    v/w

    -v;

    square(v);
    abs(v);

    v.x();
    v.y();
    v.z();

    return 0;
}
