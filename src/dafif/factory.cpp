
#include "openeaagles/dafif/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/dafif/AirportLoader.hpp"
#include "openeaagles/dafif/NavaidLoader.hpp"
#include "openeaagles/dafif/WaypointLoader.hpp"

#include <string>

namespace oe {
namespace dafif {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == AirportLoader::getFactoryName() ) {
        obj = new AirportLoader();
    }
    else if ( name == NavaidLoader::getFactoryName() ) {
        obj = new NavaidLoader();
    }
    else if ( name == WaypointLoader::getFactoryName() ) {
        obj = new WaypointLoader();
    }

    return obj;
}

}
}

