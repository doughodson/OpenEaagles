//------------------------------------------------------------------------------
// Form function for the default 'sensors' library
//------------------------------------------------------------------------------

#include "openeaagles/sensors/sensorsFF.h"

#include "openeaagles/sensors/Gmti.h"
#include "openeaagles/sensors/Tws.h"
#include "openeaagles/sensors/Stt.h"

namespace Eaagles {
namespace Sensor {

Basic::Object* sensorsFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    // Sensors
    if ( strcmp(formname, Gmti::getFormName()) == 0 ) {
        newform = new Gmti();
    }
    else if ( strcmp(formname, Stt::getFormName()) == 0 ) {
        newform = new Stt();
    }
    else if ( strcmp(formname, Tws::getFormName()) == 0 ) {
        newform = new Tws();
    }

    return newform;
}

} // End Sensor namespace
} // End Eaagles namespace
