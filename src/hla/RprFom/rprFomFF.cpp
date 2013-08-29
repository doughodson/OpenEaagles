
#include "openeaagles/hla/rprFom/rprFomFF.h"

#include "openeaagles/hla/rprFom/NetIO.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace RprFom {

Basic::Object* formFunc(const char* const formname)
{
    Basic::Object* newform = 0;

    if ( strcmp(formname, Hla::RprFom::NetIO::getFormName()) == 0 ) {
        newform = new Hla::RprFom::NetIO();
    }

    return newform;
}

} // End RprFom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
