#ifndef Body_H
#define Body_H

template<typename T> class Body{

    public:
        using numeric_type = typename T::value_type;
        Body() = default;
        Body(const T& position, const T& velocity, const numeric_type _mass);

        Body(const Body<T>&) = default;
        Body(Body<T>&&) = default;
        Body<T>& operator=(const Body<T>&) = default;
        Body<T>& operator=(Body<T>&) = default;

        const T& position() const{ return _position; }
        const T& velocity() const{ return _velocity; }
        const numeric_type mass() const{ return _mass; }

        void updatePosition(const T& update){ _position += update; }
        void updateVelocity(const T& update){ _velocity += update; }

    private:
        T _position;
        T _velocity;
        numeric_type _mass;
};


#endif
