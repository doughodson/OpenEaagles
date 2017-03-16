
#include "openeaagles/interop/dis/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/interop/dis/NetIO.hpp"
#include "openeaagles/interop/dis/Ntm.hpp"
#include "openeaagles/interop/dis/EmissionPduHandler.hpp"

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
