
#include "openeaagles/dafif/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

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

