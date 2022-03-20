#ifndef IntegratorBase_H
#define IntegratorBase_H

#include "../../body/include/star_system.h"

template <typename BodyType> class IntegratorBase{

    public:
        using numeric_type = typename BodyType::numeric_type;
        virtual void timeStep(StarSystem<BodyType>&, const numeric_type) const = 0;
};




#endif
