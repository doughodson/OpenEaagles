//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/vehicles/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/vehicles/JSBSimModel.h"
#include "openeaagles/vehicles/RacModel.h"
#include "openeaagles/vehicles/LaeroModel.h"

namespace Eaagles {
namespace Vehicle {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // RAC model
    if ( strcmp(name, RacModel::getFactoryName()) == 0 ) {
        obj = new RacModel();
    }
    // JSBSim model
    else if ( strcmp(name, JSBSimModel::getFactoryName()) == 0 ) {
        obj = new JSBSimModel();
    }
    // Laero model
    else if ( strcmp(name, LaeroModel::getFactoryName()) == 0 ) {
        obj = new LaeroModel();
    }

    return obj;
}

}  // end namespace Vehicle
}  // end namespace Eaagles
