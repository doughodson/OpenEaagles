
#include "openeaagles/base/Object.hpp"

#include "openeaagles/maps/rpf/factory.hpp"
#include "openeaagles/maps/rpf/MapDrawer.hpp"
#include "openeaagles/maps/rpf/CadrgMap.hpp"

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
