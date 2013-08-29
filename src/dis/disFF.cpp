//------------------------------------------------------------------------------
// disFormFunc() -- parser form function for the DIS network
//------------------------------------------------------------------------------
#include "openeaagles/dis/disFF.h"
#include "openeaagles/dis/NetIO.h"
#include "openeaagles/dis/Ntm.h"
#include "openeaagles/dis/EmissionPduHandler.h"

namespace Eaagles {
namespace Network {
namespace Dis {

Basic::Object* disFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    if ( strcmp(formname, Dis::NetIO::getFormName()) == 0 ) {
        newform = new Dis::NetIO();
    }
    else if ( strcmp(formname, Ntm::getFormName()) == 0 ) {
        newform = new Ntm();
    }
    else if ( strcmp(formname, EmissionPduHandler::getFormName()) == 0 ) {
        newform = new EmissionPduHandler();
    }

    return newform;
}

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace
