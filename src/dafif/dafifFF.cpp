//------------------------------------------------------------------------------
// Form function for the 'dafif' library
//------------------------------------------------------------------------------

#include "openeaagles/dafif/dafifFF.h"
#include "openeaagles/dafif/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Dafif {

Basic::Object* dafifFormFunc(const char* name)
{
   static bool msgPrinted(false);
   if (!msgPrinted) {
      std::cout << std::endl;
      std::cout << "WARNING! The function Dafif::dafifFormFunc() is deprecated - use Dafif::Factory::createObj() instead.";
      std::cout << std::endl;
      msgPrinted = true;
   }
   return Factory::createObj(name);
}

} // End Dafif namespace
} // End Eaagles namespace
