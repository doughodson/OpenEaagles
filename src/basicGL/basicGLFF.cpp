//------------------------------------------------------------------------------
// Form function for the 'basicGL' library
//------------------------------------------------------------------------------

#include "openeaagles/basicGL/basicGLFF.h"
#include "openeaagles/basicGL/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace BasicGL {

Basic::Object* basicGLFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function BasicGL::basicGLFormFunc() is deprecated - use BasicGL::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End BasicGL namespace
} // End oe namespace
