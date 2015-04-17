//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/dafif/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

#include <cstring>

namespace Eaagles {
namespace Dafif {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

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

}  // end namespace Dafif
}  // end namespace Eaagles

