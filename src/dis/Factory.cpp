//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/dis/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Ntm.h"
#include "openeaagles/dis/EmissionPduHandler.h"

#include <cstring>

namespace oe {
namespace Network {
namespace Dis {

Factory::Factory()
{}

basic::Object* Factory::createObj(const char* name)
{
    basic::Object* obj = nullptr;

    if ( std::strcmp(name, Dis::NetIO::getFactoryName()) == 0 ) {
        obj = new Dis::NetIO();
    }
    else if ( std::strcmp(name, Ntm::getFactoryName()) == 0 ) {
        obj = new Ntm();
    }
    else if ( std::strcmp(name, EmissionPduHandler::getFactoryName()) == 0 ) {
        obj = new EmissionPduHandler();
    }

    return obj;
}

} // End Dis namespace
} // End Network namespace
} // End oe namespace

