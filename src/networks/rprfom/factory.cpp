
#include "openeaagles/networks/rprfom/factory.h"
#include "openeaagles/networks/rprfom/NetIO.h"

#include <cstring>

namespace oe {
namespace rprfom {

base::Object* formFunc(const char* const name)
{
    base::Object* obj = nullptr;

    if ( std::strcmp(name, NetIO::getFactoryName()) == 0 ) {
        obj = new NetIO();
    }

    return obj;
}

}
}
