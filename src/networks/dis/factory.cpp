
#include "openeaagles/networks/dis/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/networks/dis/NetIO.h"
#include "openeaagles/networks/dis/Ntm.h"
#include "openeaagles/networks/dis/EmissionPduHandler.h"

#include <cstring>

namespace oe {

namespace dis {

base::Object* factory(const char* name)
{
    base::Object* obj = nullptr;

    if ( std::strcmp(name, dis::NetIO::getFactoryName()) == 0 ) {
        obj = new dis::NetIO();
    }
    else if ( std::strcmp(name, Ntm::getFactoryName()) == 0 ) {
        obj = new Ntm();
    }
    else if ( std::strcmp(name, EmissionPduHandler::getFactoryName()) == 0 ) {
        obj = new EmissionPduHandler();
    }

    return obj;
}

}
}
