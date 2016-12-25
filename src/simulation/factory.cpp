
#include "openeaagles/simulation/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/simulation/Simulation.hpp"
#include "openeaagles/simulation/Station.hpp"

#include <string>

namespace oe {
namespace simulation {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Simulation
    if ( name == Simulation::getFactoryName() ) {
        obj = new Simulation();
    }
    else if ( name == Station::getFactoryName() ) {
        obj = new Station();
    }

    return obj;
}

}
}

