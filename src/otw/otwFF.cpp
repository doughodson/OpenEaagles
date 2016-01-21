//------------------------------------------------------------------------------
// Form function for the Out-The-Window, 'otw', library
//------------------------------------------------------------------------------
#include "openeaagles/otw/otwFF.h"
#include "openeaagles/otw/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Otw {

Basic::Object* otwFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Otw::otwFormFunc() is deprecated - use Otw::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Otw namespace
} // End oe namespace
