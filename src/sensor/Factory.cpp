//------------------------------------------------------------------------------
// Class: Factory
//------------------------------------------------------------------------------
#include "openeaagles/sensor/Factory.h"

#include "openeaagles/basic/Object.h"

#include "openeaagles/sensor/Gmti.h"
#include "openeaagles/sensor/Tws.h"
#include "openeaagles/sensor/Stt.h"

#include <cstring>

namespace oe {
namespace sensor {

Factory::Factory()
{}

basic::Object* Factory::createObj(const char* name)
{
    basic::Object* obj = nullptr;

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

}  // end namespace sensor
}  // end namespace oe
