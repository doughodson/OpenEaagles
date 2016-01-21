//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/maps/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"

#include <cstring>

namespace oe {
namespace Maps {

Factory::Factory()
{}

basic::Object* Factory::createObj(const char* name)
{
    basic::Object* obj = nullptr;

    // Map Drawer
    if ( std::strcmp(name, Rpf::MapDrawer::getFactoryName()) == 0 ) {
        obj = new Rpf::MapDrawer();
    }
    // CadrgMap
    else if ( std::strcmp(name, Rpf::CadrgMap::getFactoryName()) == 0 ) {
        obj = new Rpf::CadrgMap();
    }

    return obj;
}

}  // end namespace Maps
}  // end namespace oe
