
#include "openeaagles/otw/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/otw/Otm.hpp"

#include "openeaagles/otw/cigi/OtwCigiCl.hpp"
#include "openeaagles/otw/cigi/CigiClNetwork.hpp"

#include "openeaagles/otw/pc/OtwPC.hpp"

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

    else if ( name == Otm::getFactoryName() ) {
        obj = new Otm();
    }

    return obj;
}

}
}
