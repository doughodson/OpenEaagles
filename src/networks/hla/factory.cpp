
#include "openeaagles/networks/hla/factory.h"

#include <cstring>

namespace oe {
namespace hla {

base::Object* factory(const char* const name)
{
    base::Object* obj = nullptr;

    //if ( std::strcmp(formname, getFormName()) == 0 ) {
    //    newform = new Mc02FomIO();
    //}
    //if ( std::strcmp(formname, RprFomIO::getFormName()) == 0 ) {
    //    newform = new RprFomIO;
    //}

    return obj;
}

}
}
