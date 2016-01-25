//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/maps/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"

#include <cstring>

namespace oe {
namespace maps  {

Factory::Factory()
{}

basic::Object* Factory::createObj(const char* name)
{
    basic::Object* obj = nullptr;

    // Map Drawer
    if ( std::strcmp(name, rpf::MapDrawer::getFactoryName()) == 0 ) {
        obj = new rpf::MapDrawer();
    }
    // CadrgMap
    else if ( std::strcmp(name, rpf::CadrgMap::getFactoryName()) == 0 ) {
        obj = new rpf::CadrgMap();
    }

    return obj;
}

}  // end namespace maps 
}  // end namespace oe
