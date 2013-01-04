//------------------------------------------------------------------------------
// Form function for the I/O devices 'IoDevice' library
//------------------------------------------------------------------------------

#include "openeaagles/ioDevice/ioDeviceFF.h"

#include "openeaagles/ioDevice/Ai2DiSwitch.h"
#include "openeaagles/ioDevice/AnalogInput.h"
#include "openeaagles/ioDevice/AnalogOutput.h"
#include "openeaagles/ioDevice/DiscreteInput.h"
#include "openeaagles/ioDevice/DiscreteOutput.h"
#include "openeaagles/ioDevice/IoData.h"
#include "openeaagles/ioDevice/SignalGen.h"

#if defined(WIN32)
   #include "./windows/UsbJoystickImp.h"
#else
   #include "./linux/UsbJoystickImp.h"
#endif


namespace Eaagles {
namespace IoDevice {

Basic::Object* ioDeviceFormFunc(const char* formname)
{
    Basic::Object* newform = 0;

    // Data buffers
    if ( strcmp(formname, IoData::getFormName()) == 0 ) {
        newform = new IoData();
    }

    // Data Handlers
    else if ( strcmp(formname, DiscreteInput::getFormName()) == 0 ) {
        newform = new DiscreteInput();
    }
    else if ( strcmp(formname, DiscreteOutput::getFormName()) == 0 ) {
        newform = new DiscreteOutput();
    }
    else if ( strcmp(formname, AnalogInput::getFormName()) == 0 ) {
        newform = new AnalogInput();
    }
    else if ( strcmp(formname, AnalogOutput::getFormName()) == 0 ) {
        newform = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( strcmp(formname, Ai2DiSwitch::getFormName()) == 0 ) {
        newform = new Ai2DiSwitch();
    }
    else if ( strcmp(formname, SignalGen::getFormName()) == 0 ) {
        newform = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( strcmp(formname, UsbJoystickImp::getFormName()) == 0 ) {
        newform = new UsbJoystickImp();
    }

    return newform;
}

}  //end IoDevice
} // end Eaagles namespace

