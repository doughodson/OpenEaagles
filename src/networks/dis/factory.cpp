
#include "openeaagles/networks/dis/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/networks/dis/NetIO.h"
#include "openeaagles/networks/dis/Ntm.h"
#include "openeaagles/networks/dis/EmissionPduHandler.h"

#include <string>

namespace oe {

namespace dis {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == NetIO::getFactoryName() ) {
        obj = new NetIO();
    }
    else if ( name == Ntm::getFactoryName() ) {
        obj = new Ntm();
    }
    else if ( name == EmissionPduHandler::getFactoryName() ) {
        obj = new EmissionPduHandler();
    }

    return obj;
}

}
}
