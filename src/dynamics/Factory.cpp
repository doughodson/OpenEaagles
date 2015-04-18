//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/dynamics/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/dynamics/JSBSimModel.h"
#include "openeaagles/dynamics/RacModel.h"
#include "openeaagles/dynamics/LaeroModel.h"

#include <cstring>

namespace Eaagles {
namespace Dynamics {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // RAC model
    if ( std::strcmp(name, RacModel::getFactoryName()) == 0 ) {
        obj = new RacModel();
    }
    // JSBSim model
    else if ( std::strcmp(name, JSBSimModel::getFactoryName()) == 0 ) {
        obj = new JSBSimModel();
    }
    // Laero model
    else if ( std::strcmp(name, LaeroModel::getFactoryName()) == 0 ) {
        obj = new LaeroModel();
    }

    return obj;
}

}  // end namespace Dynamics
}  // end namespace Eaagles
