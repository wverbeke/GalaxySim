// A collection of bodies.
#ifndef StarSystem_H
#define StarSystem_H

#include "../../force/include/force_computer_base.h"
template<typename BodyType> class ForceComputerBase;
template<typename BodyType> class StarSystem{

    public:
        using numeric_type = typename BodyType::numeric_type;
        using vector_type = typename BodyType::vector_type;
        using const_iterator = typename std::vector<BodyType>::const_iterator;
        using iterator = typename std::vector<BodyType>::iterator;

        StarSystem(const std::vector<BodyType>& bodies):
            _bodies(bodies)
        {};
        ~StarSystem() = default;

        // Star systems can not be copy constructed or copy assigned.
        StarSystem(const StarSystem&) = default;
        StarSystem(StarSystem&&) = default;
        StarSystem& operator=(const StarSystem&) = default;
        StarSystem& operator=(StarSystem&&) = default;

        // Properties of the while star system.
        
        // Total energy in the star system.
        numeric_type energy(ForceComputerBase<BodyType>& force_computer) const{
            return (kineticEnergy() + potentialEnergy(force_computer));
        }

        // Kinetic energy.
        numeric_type kineticEnergy() const{
            numeric_type kinetic_energy = 0.;
            for(const auto& body: _bodies){
                kinetic_energy += square(body.velocity())*body.mass();
            }
            kinetic_energy *= 0.5;
            return kinetic_energy;
        }

        // Potential energy.
        // This will only return correct results if the forces in the star system were already
        // computed. The force and potential energy computations largely overlap so they are done
        // simultaneously for efficiency.
        numeric_type potentialEnergy(ForceComputerBase<BodyType>& force_computer) const{
            return force_computer.potentialEnergy();
        }

        // Access individual bodies.
        std::size_t size() const{ return _bodies.size();}

        const BodyType& operator[](const std::size_t index) const{
            return _bodies.at(index);
        }

        BodyType& operator[](const std::size_t index){
            return _bodies.at(index);
        }

        const_iterator begin() const{ return _bodies.cbegin(); }
        const_iterator cbegin() const{ return _bodies.cbegin(); }
        const_iterator end() const{ return _bodies.cend(); }
        const_iterator cend() const{ return _bodies.cend(); }

        iterator begin(){return _bodies.begin();}
        iterator end(){return _bodies.end();}

        // Interface to compute forces and accelerations, as well as the potential energy.
        void computeForces(ForceComputerBase<BodyType>& force_computer) const{ force_computer.computeForces(*this); }
        void computeForcesAndPotential(ForceComputerBase<BodyType>& force_computer) const{ force_computer.computeForcesAndPotential(*this); }
        vector_type acceleration(ForceComputerBase<BodyType>& force_computer, const std::size_t index){
            return (force_computer.totalForce(index)/_bodies.at(index).mass());
        }

    private:
        std::vector<BodyType> _bodies;
};

template <typename BodyType> bool all_close(const StarSystem<BodyType>& lhs, const StarSystem<BodyType>& rhs){
    if(lhs.size() != rhs.size()){
        return false;
    }
    for(std::size_t b{0}; b < lhs.size(); ++b){
        if(!is_close(lhs[b], rhs[b])){
            return false;
        }
    }
    return true;
}

#endif
