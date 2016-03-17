
#include "openeaagles/base/Object.h"

#include "openeaagles/maps/rpf/factory.h"
#include "openeaagles/maps/rpf/MapDrawer.h"
#include "openeaagles/maps/rpf/CadrgMap.h"

#include <cstring>

namespace oe {
namespace rpf  {

base::Object* factory(const char* name)
{
    base::Object* obj = nullptr;

    // Map Drawer
    if ( std::strcmp(name, MapDrawer::getFactoryName()) == 0 ) {
        obj = new MapDrawer();
    }
    // CadrgMap
    else if ( std::strcmp(name, CadrgMap::getFactoryName()) == 0 ) {
        obj = new CadrgMap();
    }

    return obj;
}

}
}
