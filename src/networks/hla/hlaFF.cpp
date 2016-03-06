//------------------------------------------------------------------------------
// hlaFormFunc() -- parser form function for HLA networks
//------------------------------------------------------------------------------

#include "openeaagles/networks/hla/hlaFF.h"

//#include "rpr_fomRprFomIO.h"

#include <cstring>

namespace oe {
namespace hla {

base::Object* hlaFormFunc(const char* const formname)
{
    base::Object* newform = nullptr;

    //if ( std::strcmp(formname, getFormName()) == 0 ) {
    //    newform = new Mc02FomIO();
    //}
    //if ( std::strcmp(formname, RprFomIO::getFormName()) == 0 ) {
    //    newform = new RprFomIO;
    //}

    return newform;
}

}
}
