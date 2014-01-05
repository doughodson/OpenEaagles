//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/sensors/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/sensors/Gmti.h"
#include "openeaagles/sensors/Tws.h"
#include "openeaagles/sensors/Stt.h"

namespace Eaagles {
namespace Sensor {

Factory::Factory()
{}

Basic::Object* Factory::createObj(const char* name)
{
    Basic::Object* obj = 0;

    // Sensors
    if ( strcmp(name, Gmti::getFactoryName()) == 0 ) {
        obj = new Gmti();
    }
    else if ( strcmp(name, Stt::getFactoryName()) == 0 ) {
        obj = new Stt();
    }
    else if ( strcmp(name, Tws::getFactoryName()) == 0 ) {
        obj = new Tws();
    }

    return obj;
}

}  // end namespace Sensor
}  // end namespace Eaagles
