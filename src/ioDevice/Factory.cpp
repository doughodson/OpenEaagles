//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------

#include "openeaagles/ioDevice/Factory.h"

#include "openeaagles/basic/Object.h"

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

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // Data buffers
    if ( strcmp(name, IoData::getFactoryName()) == 0 ) {
        obj = new IoData();
    }

    // Data Handlers
    else if ( strcmp(name, DiscreteInput::getFactoryName()) == 0 ) {
        obj = new DiscreteInput();
    }
    else if ( strcmp(name, DiscreteOutput::getFactoryName()) == 0 ) {
        obj = new DiscreteOutput();
    }
    else if ( strcmp(name, AnalogInput::getFactoryName()) == 0 ) {
        obj = new AnalogInput();
    }
    else if ( strcmp(name, AnalogOutput::getFactoryName()) == 0 ) {
        obj = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( strcmp(name, Ai2DiSwitch::getFactoryName()) == 0 ) {
        obj = new Ai2DiSwitch();
    }
    else if ( strcmp(name, SignalGen::getFactoryName()) == 0 ) {
        obj = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( strcmp(name, UsbJoystickImp::getFactoryName()) == 0 ) {
        obj = new UsbJoystickImp();
    }

    return obj;
}

}  // end namespace IoDevice
}  // end namespace Eaagles
