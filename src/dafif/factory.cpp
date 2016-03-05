
#include "openeaagles/dafif/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

#include <cstring>

namespace oe {
namespace dafif {

base::Object* factory(const char* name)
{
    base::Object* obj = nullptr;

    if ( std::strcmp(name, AirportLoader::getFactoryName()) == 0 ) {
        obj = new AirportLoader();
    }
    else if ( std::strcmp(name, NavaidLoader::getFactoryName()) == 0 ) {
        obj = new NavaidLoader();
    }
    else if ( std::strcmp(name, WaypointLoader::getFactoryName()) == 0 ) {
        obj = new WaypointLoader();
    }

    return obj;
}

}
}

