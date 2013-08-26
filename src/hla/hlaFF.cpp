
#include "eaagles/hla/hlaFF.h"

//#include "mc02_fom/Mc02FomIO.h"
//#include "rpr_fomRprFomIO.h"
//#include "eaagles/hla/mc02_fom/Mc02Fom.h"

namespace Eaagles {
namespace Network {
namespace Hla {

Basic::Object* hlaFormFunc(const char* const formname)
{
    Basic::Object* newform = 0;

    //if ( strcmp(formname, getFormName()) == 0 ) {
    //    newform = new Mc02FomIO();
    //}
    //else if ( strcmp(formname, RprFomIO::getFormName()) == 0 ) {
    //    newform = new RprFomIO;
    //}

    return newform;
}

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
