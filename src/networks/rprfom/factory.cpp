
#include "openeaagles/networks/rprfom/factory.h"
#include "openeaagles/networks/rprfom/NetIO.h"

#include <string>

namespace oe {
namespace rprfom {

base::Object* formFunc(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == NetIO::getFactoryName() ) {
        obj = new NetIO();
    }

    return obj;
}

}
}
