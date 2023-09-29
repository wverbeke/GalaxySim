#include "../include/star_system_reader.h"
#include "../../vector/include/vector3D.h"
#include "../../body/include/body.h"
#include "../../body/include/star_system.h"

#include <string>
#include <cstddef>


int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;
    using star_system_type = StarSystem<body_type>;

    const std::size_t num_bodies = 2;
    const std::string in_name = "/home/willem/code/GalaxySim/star_system_write.h5";
    StarSystemReader<body_type> star_reader(in_name);
    star_reader.at(0);
}
