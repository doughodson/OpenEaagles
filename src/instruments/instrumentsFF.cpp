//--------------------------------------------------------------------------------
// Form function for the 'instruments' library
//--------------------------------------------------------------------------------
#include "openeaagles/instruments/instrumentsFF.h"
#include "openeaagles/instruments/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Instruments {

Basic::Object* instrumentsFormFunc(const char* name)
{
   static bool msgPrinted(false);
   if (!msgPrinted) {
      std::cout << std::endl;
      std::cout << "WARNING! The function Instruments::instrumentsFormFunc() is deprecated - use Instruments::Factory::createObj() instead.";
      std::cout << std::endl;
      msgPrinted = true;
   }
   return Factory::createObj(name);
}

}; // end of Instruments namespace  
}; // end of Eaagles namespace
