//------------------------------------------------------------------------------
// Form function for the default 'dynamics' library
//------------------------------------------------------------------------------
#include "openeaagles/dynamics/dynamicsFF.h"
#include "openeaagles/dynamics/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Dynamics {

Basic::Object* dynamicsFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Dynamics::dynamicsFormFunc() is deprecated - use Dynamics::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Vehicle namespace
} // End Eaagles namespace
