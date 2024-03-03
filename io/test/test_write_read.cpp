#include "../include/star_system_writer.h"
#include "../include/star_system_reader.h"
#include "../../vector/include/vector3D.h"
#include "../../body/include/body.h"
#include "../../body/include/star_system.h"

#include <stdexcept>
#include <string>
#include <cstddef>
#include <exception>


int main(){
    using numeric_type = double;
    using vector_type = Vector3D<numeric_type>;
    using body_type = Body<vector_type>;
    using star_system_type = StarSystem<body_type>;

    const std::size_t num_bodies = 2;

    std::vector<body_type> bodies;
    for(std::size_t b = 0; b < num_bodies; ++b){
        vector_type pos{0.0, 0.0, 0.0};
        vector_type vel{0.0, 0.0, 0.0};
        numeric_type mass{static_cast<numeric_type>(b)};
        bodies.emplace_back(pos, vel, mass);
    }
    
    star_system_type star_system(bodies);
    std::vector<star_system_type> snapshots;
    
    // Make a star system writer.
    const std::string file_name = "/home/willem/code/GalaxySim/star_system_write.h5";
    StarSystemWriter<body_type> star_writer(num_bodies, file_name);

    // Do an update of all stars and write them to the hdf5 file.
    for(unsigned i = 0; i < 20; ++i){
        for(unsigned j = 0; j < star_system.size(); ++j){

            double pos_update = static_cast<numeric_type>(j);
            double velocity_update = pos_update/10;
            star_system[j].updatePosition({pos_update, pos_update, pos_update});
            star_system[j].updateVelocity({velocity_update, velocity_update, velocity_update});
        }
        snapshots.emplace_back(star_system);
        star_writer.write_star_system(star_system, static_cast<numeric_type>(i));
    }

    StarSystemReader<body_type> star_reader(file_name);
    
    auto star_system_and_time = star_reader.at(0);
    auto ss = star_system_and_time.second;

    // Compare the read star systems to the snapshots.
    for(unsigned t = 0; t < star_reader.num_timestamps(); ++t){
        auto star_system_and_time = star_reader.at(t);
        auto ts = star_system_and_time.first;
        auto ss = star_system_and_time.second;
        if(!all_close(ss, snapshots[t])){
            throw std::runtime_error("Read star system not equal to original.");
        }
    }
    std::cout << "Test run successfully." << std::endl;
    return 0;
}
