//------------------------------------------------------------------------------
// mapsFormFunc()
//------------------------------------------------------------------------------
#include "openeaagles/maps/mapsFF.h"
#include "openeaagles/maps/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Maps {

Basic::Object* mapsFormFunc(const char* name)
{
   static bool msgPrinted(false);
   if (!msgPrinted) {
      std::cout << std::endl;
      std::cout << "WARNING! The function Maps::mapsFormFunc() is deprecated - use Maps::Factory::createObj() instead.";
      std::cout << std::endl;
      msgPrinted = true;
   }
   return Factory::createObj(name);
}

} // end Maps namespace
} // end Eaagles namespace

