// A collection of bodies.
#ifndef StarSystem_H
#define StarSystem_H

template<typename BodyType> class StarSystem{

    public:
        using numeric_type = typename BodyType::numeric_type;
        using vector_type = typename BodyType::vector_type;
        using const_iterator = typename std::vector<BodyType>::const_iterator;

        StarSystem(const std::vector<BodyType>& bodies): _bodies(bodies) {};

        // Star systems can not be copy constructed or copy assigned.
        StarSystem(const StarSystem&) = delete;
        StarSystem(StarSystem&&) = delete;
        StarSystem& operator=(const StarSystem&) = delete;
        StarSystem& operator=(StarSystem&&) = delete;

        numeric_type energy() const;
        numeric_type potentialEnergy() const;
        numeric_type kineticEnergy() const;

        std::size_t size() const{ return _bodies.size();}
        const BodyType& operator[](const std::size_t index){
            return _bodies.at(index);
        }

        const_iterator begin() const{ return _bodies.cbegin(); }
        const_iterator cbegin() const{ return _bodies.cbegin(); }
        const_iterator end() const{ return _bodies.cend(); }
        const_iterator cend() const{ return _bodies.cend(); }

    private:
        std::vector<BodyType> _bodies;
};

#endif
