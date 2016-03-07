
#include "openeaagles/networks/hla/rprfom/factory.h"

#include "openeaagles/networks/hla/rprfom/NetIO.h"

#include <cstring>

namespace oe {
namespace hla {
namespace rprfom {

base::Object* formFunc(const char* const name)
{
    base::Object* obj = nullptr;

    if ( std::strcmp(name, hla::rprfom::NetIO::getFactoryName()) == 0 ) {
        obj = new hla::rprfom::NetIO();
    }

    return obj;
}

}
}
}
