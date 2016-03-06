//------------------------------------------------------------------------------
// rprFormFunc() -- parser form function for HLA RPR FOM
//------------------------------------------------------------------------------

#include "openeaagles/networks/hla/rprFom/rprFomFF.h"

#include "openeaagles/networks/hla/rprFom/NetIO.h"

#include <cstring>

namespace oe {
namespace hla {
namespace rprfom {

base::Object* formFunc(const char* const formname)
{
    base::Object* newform = nullptr;

    if ( std::strcmp(formname, hla::rprfom::NetIO::getFactoryName()) == 0 ) {
        newform = new hla::rprfom::NetIO();
    }

    return newform;
}

}
}
}
