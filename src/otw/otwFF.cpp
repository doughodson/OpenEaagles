//------------------------------------------------------------------------------
// Form function for the Out-The-Window, 'otw', library
//------------------------------------------------------------------------------

#include "openeaagles/otw/OtwCigiCl.h"
#include "openeaagles/otw/OtwPC.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Otw {

Basic::Object* otwFormFunc(const char* formname)
{
    Basic::Object* newform = 0;
    
    // Common Image Generation Interface (CIGI)
    if ( strcmp(formname, OtwCigiCl::getFormName()) == 0 ) {
        newform = new OtwCigiCl();
    }
    else if ( strcmp(formname, CigiClNetwork::getFormName()) == 0 ) {
        newform = new CigiClNetwork();
    }
  
    // PC Visual Driver
    else if ( strcmp(formname, OtwPC::getFormName()) == 0 ) {
        newform = new OtwPC();
    }

    return newform;
}

} // End Otw namespace
} // End Eaagles namespace
