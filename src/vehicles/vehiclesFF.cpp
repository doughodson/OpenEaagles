//------------------------------------------------------------------------------
// Form function for the default 'vehicles' library
//------------------------------------------------------------------------------

#include "openeaagles/vehicles/vehiclesFF.h"

#include "openeaagles/vehicles/JSBSimModel.h"
#include "openeaagles/vehicles/RacModel.h"

namespace Eaagles {
namespace Vehicle {

Basic::Object* vehiclesFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    // RAC Model
    if ( strcmp(formname, RacModel::getFormName()) == 0 ) {
        newform = new RacModel();
    }

    // JSBSim models
    else if ( strcmp(formname, JSBSimModel::getFormName()) == 0 ) {
        newform = new JSBSimModel();
    }

    return newform;
}

} // End Vehicle namespace
} // End Eaagles namespace
