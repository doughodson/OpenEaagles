
#include "openeaagles/simulation/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/simulation/ISimulation.hpp"
#include "openeaagles/simulation/Station.hpp"

#include <string>

namespace oe {
namespace simulation {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Simulation
    if ( name == ISimulation::getFactoryName() ) {
        obj = new ISimulation();
    }
    else if ( name == Station::getFactoryName() ) {
        obj = new Station();
    }

    return obj;
}

}
}

