//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/maps/Factory.h"

#include "openeaagles/basic/Object.h"

//
#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"

namespace Eaagles {
namespace Maps {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // Map Drawer
    if ( strcmp(name, Rpf::MapDrawer::getFormName()) == 0 ) {
        obj = new Rpf::MapDrawer();
    }
    // CadrgMap
    else if ( strcmp(name, Rpf::CadrgMap::getFormName()) == 0 ) {
        obj = new Rpf::CadrgMap();
    }

    return obj;
}

}  // end namespace Maps
}  // end namespace Eaagles
