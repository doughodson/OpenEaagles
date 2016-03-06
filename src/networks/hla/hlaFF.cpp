//------------------------------------------------------------------------------
// hlaFormFunc() -- parser form function for HLA networks
//------------------------------------------------------------------------------

#include "openeaagles/hla/hlaFF.h"

//#include "rpr_fomRprFomIO.h"

#include <cstring>

namespace Eaagles {
namespace Network {
namespace Hla {

Basic::Object* hlaFormFunc(const char* const formname)
{
    Basic::Object* newform = nullptr;

    //if ( std::strcmp(formname, getFormName()) == 0 ) {
    //    newform = new Mc02FomIO();
    //}
    //if ( std::strcmp(formname, RprFomIO::getFormName()) == 0 ) {
    //    newform = new RprFomIO;
    //}

    return newform;
}

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
