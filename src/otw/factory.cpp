
#include "openeaagles/otw/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/otw/OtwCigiCl.h"
#include "openeaagles/otw/OtwPC.h"

#include <string>

namespace oe {

namespace otw {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Common Image Generation Interface (CIGI)
    if ( name == OtwCigiCl::getFactoryName() ) {
        obj = new OtwCigiCl();
    }
    else if ( name == CigiClNetwork::getFactoryName() ) {
        obj = new CigiClNetwork();
    }

    // PC Visual Driver
    else if ( name == OtwPC::getFactoryName() ) {
        obj = new OtwPC();
    }

    return obj;
}

}
}
