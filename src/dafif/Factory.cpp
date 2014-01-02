//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/dafif/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

namespace Eaagles {
namespace Dafif {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    if ( strcmp(name, AirportLoader::getFactoryName()) == 0 ) {
        obj = new AirportLoader();
    }
    else if ( strcmp(name, NavaidLoader::getFactoryName()) == 0 ) {
        obj = new NavaidLoader();
    }
    else if ( strcmp(name, WaypointLoader::getFactoryName()) == 0 ) {
        obj = new WaypointLoader();
    }

    return obj;
}

}  // end namespace Dafif
}  // end namespace Eaagles

