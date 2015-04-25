//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/sensors/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/sensors/Gmti.h"
#include "openeaagles/sensors/Tws.h"
#include "openeaagles/sensors/Stt.h"

#include <cstring>

namespace Eaagles {
namespace Sensor {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = nullptr;

    // Sensors
    if ( std::strcmp(name, Gmti::getFactoryName()) == 0 ) {
        obj = new Gmti();
    }
    else if ( std::strcmp(name, Stt::getFactoryName()) == 0 ) {
        obj = new Stt();
    }
    else if ( std::strcmp(name, Tws::getFactoryName()) == 0 ) {
        obj = new Tws();
    }

    return obj;
}

}  // end namespace Sensor
}  // end namespace Eaagles
