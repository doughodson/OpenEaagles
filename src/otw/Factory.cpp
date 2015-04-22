//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/otw/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/otw/OtwCigiCl.h"
#include "openeaagles/otw/OtwPC.h"

#include <cstring>

namespace Eaagles {
namespace Otw {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // Common Image Generation Interface (CIGI)
    if ( std::strcmp(name, OtwCigiCl::getFactoryName()) == 0 ) {
        obj = new OtwCigiCl();
    }
    else if ( std::strcmp(name, CigiClNetwork::getFactoryName()) == 0 ) {
        obj = new CigiClNetwork();
    }

    // PC Visual Driver
    else if ( std::strcmp(name, OtwPC::getFactoryName()) == 0 ) {
        obj = new OtwPC();
    }

    return obj;
}

}  // end namespace Otw
}  // end namespace Eaagles
