
#include "openeaagles/base/Object.hpp"

#include "openeaagles/map/rpf/factory.hpp"
#include "openeaagles/map/rpf/MapDrawer.hpp"
#include "openeaagles/map/rpf/CadrgMap.hpp"

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
