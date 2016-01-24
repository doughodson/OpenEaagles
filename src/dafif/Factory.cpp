//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/dafif/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

#include <cstring>

namespace oe {
namespace dafif {

Factory::Factory()
{}

basic::Object* Factory::createObj(const char* name)
{
    basic::Object* obj = nullptr;

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

}  // end namespace dafif
}  // end namespace oe

