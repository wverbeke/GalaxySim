#include "../include/vector2D.h"

#include <iostream>

int main(){

    Vector2D<float> v{1.0f,2.0f};
    Vector2D<float> w{3.0f,2.0f};

    std::cout << v*w << std::endl;

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

    std::cout << v << std::endl;

    return 0;
}
