
#include "openeaagles/iodevice/factory.hpp"

#include "openeaagles/base/Object.hpp"

#include "openeaagles/iodevice/Ai2DiSwitch.hpp"
#include "openeaagles/iodevice/AnalogInput.hpp"
#include "openeaagles/iodevice/AnalogOutput.hpp"
#include "openeaagles/iodevice/DiscreteInput.hpp"
#include "openeaagles/iodevice/DiscreteOutput.hpp"
#include "openeaagles/iodevice/IoData.hpp"
#include "openeaagles/iodevice/SignalGen.hpp"

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
