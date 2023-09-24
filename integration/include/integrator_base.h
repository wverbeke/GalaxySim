#ifndef IntegratorBase_H
#define IntegratorBase_H

#include "../../body/include/star_system.h"
#include "../../force/include/force_computer_base.h"

template <typename BodyType> class IntegratorBase{

    public:
        using numeric_type = typename BodyType::numeric_type;

        // Integrators might be stateful, so this method can not be const for all integrators.
        virtual void timeStep(StarSystem<BodyType>&, ForceComputerBase<BodyType>&, const numeric_type) = 0;
};




#endif
