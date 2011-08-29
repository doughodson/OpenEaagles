//------------------------------------------------------------------------------
// Form function for the Keithley I/O device
//------------------------------------------------------------------------------

#include "openeaagles/ioDevice/keithleyFF.h"

#if defined(WIN32)
   #include "./windows/KeithleyDeviceImp.h"
#else
   #include "./linux/KeithleyDeviceImp.h"
#endif

namespace Eaagles {
namespace IoDevice {
namespace Keithley {

Eaagles::Basic::Object* keithleyFormFunc(const char* formname)
{
    Eaagles::Basic::Object* newform = 0;

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    if ( strcmp(formname, KeithleyDeviceImp::getFormName()) == 0 ) {
        newform = new KeithleyDeviceImp();
    }

    return newform;
}

}  //end Keithley
} // end IoDevice
} // end Eaagles namespace
