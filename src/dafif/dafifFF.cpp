//------------------------------------------------------------------------------
// Form function for the 'dafif' library
//------------------------------------------------------------------------------

#include "openeaagles/dafif/dafifFF.h"
#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/dafif/WaypointLoader.h"

namespace Eaagles {
namespace Dafif {

Basic::Object* dafifFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    if ( strcmp(formname, AirportLoader::getFormName()) == 0 ) {
        newform = new AirportLoader();
    }
    else if ( strcmp(formname, NavaidLoader::getFormName()) == 0 ) {
        newform = new NavaidLoader();
    }
    else if ( strcmp(formname, WaypointLoader::getFormName()) == 0 ) {
        newform = new WaypointLoader();
    }

    return newform;
}

} // End Dafif namespace
} // End Eaagles namespace
