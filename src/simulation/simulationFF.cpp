//------------------------------------------------------------------------------
// Form function for the 'simulation' library
//------------------------------------------------------------------------------
#include "openeaagles/simulation/simulationFF.h"
#include "openeaagles/simulation/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Simulation {

Basic::Object* simulationFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Simulation::simulationFormFunc() is deprecated - use Simulation::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Simulation namespace
} // End Eaagles namespace

