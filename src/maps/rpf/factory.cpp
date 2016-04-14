
#include "openeaagles/base/Object.h"

#include "openeaagles/maps/rpf/factory.h"
#include "openeaagles/maps/rpf/MapDrawer.h"
#include "openeaagles/maps/rpf/CadrgMap.h"

#include <string>

namespace oe {
namespace rpf  {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Map Drawer
    if ( name == MapDrawer::getFactoryName() ) {
        obj = new MapDrawer();
    }
    // CadrgMap
    else if ( name == CadrgMap::getFactoryName() ) {
        obj = new CadrgMap();
    }

    return obj;
}

}
}
