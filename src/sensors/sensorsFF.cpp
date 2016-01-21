//------------------------------------------------------------------------------
// Form function for the default 'sensors' library
//------------------------------------------------------------------------------
#include "openeaagles/sensors/sensorsFF.h"
#include "openeaagles/sensors/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Sensor {

Basic::Object* sensorsFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Sensor::sensorsFormFunc() is deprecated - use Sensor::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Sensor namespace
} // End oe namespace
