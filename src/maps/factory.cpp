
#include "openeaagles/maps/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/maps/rpfMap/MapDrawer.h"
#include "openeaagles/maps/rpfMap/CadrgMap.h"

#include <cstring>

namespace oe {
namespace maps  {

base::Object* factory(const char* name)
{
    base::Object* obj = nullptr;

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

}
}
