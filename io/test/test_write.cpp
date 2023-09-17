#include "../include/star_system_writer.h"
#include "../../vector/include/vector3D.h"
#include "../../body/include/body.h"
#include "../../body/include/star_system.h"
#include "../../force/include/direct_sum_force_computer.h"

#include <string>
#include <cstddef>


int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;
    using star_system_type = StarSystem<body_type>;

    const std::size_t num_bodies = 2;
    const std::string out_name = "star_system_write.h5";

    StarSystemWriter<body_type> star_writer(num_bodies, out_name);

    std::vector<body_type> bodies;
    for(std::size_t b = 0; b < num_bodies; ++b){
        vector_type pos{0.0, 0.0, 0.0};
        vector_type vel{0.0, 0.0, 0.0};
        numeric_type mass{0.0};
        bodies.emplace_back(pos, vel, mass);
    }
    
    DirectSumForceComputer<body_type> force_computer;

    star_system_type star_system(bodies, force_computer);
    
    for(unsigned i = 0; i < 20; ++i){
        for(body_type& body: star_system){
            body.updatePosition({1.0, 1.0, 1.0});
            body.updateVelocity({0.1, 0.1, 0.1});
        }
        star_writer.write_star_system(star_system, static_cast<double>(i));
    }

    return 0;
}
