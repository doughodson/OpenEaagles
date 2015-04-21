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

#include <cstring>

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
    Basic::Object* obj = nullptr;

    // Data buffers
    if ( std::strcmp(name, IoData::getFactoryName()) == 0 ) {
        obj = new IoData();
    }

    // Data Handlers
    else if ( std::strcmp(name, DiscreteInput::getFactoryName()) == 0 ) {
        obj = new DiscreteInput();
    }
    else if ( std::strcmp(name, DiscreteOutput::getFactoryName()) == 0 ) {
        obj = new DiscreteOutput();
    }
    else if ( std::strcmp(name, AnalogInput::getFactoryName()) == 0 ) {
        obj = new AnalogInput();
    }
    else if ( std::strcmp(name, AnalogOutput::getFactoryName()) == 0 ) {
        obj = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( std::strcmp(name, Ai2DiSwitch::getFactoryName()) == 0 ) {
        obj = new Ai2DiSwitch();
    }
    else if ( std::strcmp(name, SignalGen::getFactoryName()) == 0 ) {
        obj = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( std::strcmp(name, UsbJoystickImp::getFactoryName()) == 0 ) {
        obj = new UsbJoystickImp();
    }

    return obj;
}

}  // end namespace IoDevice
}  // end namespace Eaagles
