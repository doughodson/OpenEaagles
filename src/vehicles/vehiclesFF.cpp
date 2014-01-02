//------------------------------------------------------------------------------
// Form function for the default 'vehicles' library
//------------------------------------------------------------------------------
#include "openeaagles/vehicles/vehiclesFF.h"
#include "openeaagles/vehicles/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Vehicle {

Basic::Object* vehiclesFormFunc(const char* name)
{
   static bool msgPrinted(false);
   if (!msgPrinted) {
      std::cout << std::endl;
      std::cout << "WARNING! The function Vehicle::vehiclesFormFunc() is deprecated - use Vehicle::Factory::createObj() instead.";
      std::cout << std::endl;
      msgPrinted = true;
   }
   return Factory::createObj(name);
}

} // End Vehicle namespace
} // End Eaagles namespace
