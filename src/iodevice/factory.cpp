
#include "openeaagles/iodevice/factory.h"

#include "openeaagles/base/Object.h"

#include "openeaagles/iodevice/Ai2DiSwitch.h"
#include "openeaagles/iodevice/AnalogInput.h"
#include "openeaagles/iodevice/AnalogOutput.h"
#include "openeaagles/iodevice/DiscreteInput.h"
#include "openeaagles/iodevice/DiscreteOutput.h"
#include "openeaagles/iodevice/IoData.h"
#include "openeaagles/iodevice/SignalGen.h"

#include <string>

#if defined(WIN32)
   #include "./platform/UsbJoystick_msvc.h"
#else
   #include "./platform/UsbJoystick_linux.h"
#endif

namespace oe {
namespace iodevice {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Data buffers
    if ( name == IoData::getFactoryName() ) {
        obj = new IoData();
    }

    // Data Handlers
    else if ( name == DiscreteInput::getFactoryName() ) {
        obj = new DiscreteInput();
    }
    else if ( name == DiscreteOutput::getFactoryName() ) {
        obj = new DiscreteOutput();
    }
    else if ( name == AnalogInput::getFactoryName() ) {
        obj = new AnalogInput();
    }
    else if ( name == AnalogOutput::getFactoryName() ) {
        obj = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( name == Ai2DiSwitch::getFactoryName() ) {
        obj = new Ai2DiSwitch();
    }
    else if ( name == SignalGen::getFactoryName() ) {
        obj = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( name == UsbJoystick::getFactoryName() ) {
        obj = new UsbJoystick();
    }

    return obj;
}

}
}
