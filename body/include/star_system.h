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

        StarSystem(const std::vector<BodyType>& bodies, ForceComputerBase<BodyType>& force_computer):
            _bodies(bodies),
            _force_computer(force_computer)
        {};
        ~StarSystem() = default;

        // Star systems can not be copy constructed or copy assigned.
        StarSystem(const StarSystem&) = delete;
        StarSystem(StarSystem&&) = delete;
        StarSystem& operator=(const StarSystem&) = delete;
        StarSystem& operator=(StarSystem&&) = delete;

        // Properties of the while star system.
        numeric_type energy() const;
        numeric_type potentialEnergy() const;
        numeric_type kineticEnergy() const;

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

        // Interface to compute forces and accelerations.
        void computeForces() const{ _force_computer.computeForces(*this); }
        vector_type acceleration(const std::size_t index){
            return (_force_computer.totalForce(index)/_bodies.at(index).mass());
        }

    private:
        std::vector<BodyType> _bodies;
        ForceComputerBase<BodyType>& _force_computer;
};

#endif
