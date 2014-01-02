//------------------------------------------------------------------------------
// Form function for the guiGlut object library
//------------------------------------------------------------------------------

#include "openeaagles/gui/glut/glutFF.h"
#include "openeaagles/gui/glut/Factory.h"

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Glut {

Basic::Object* glutFormFunc(const char* name)
{
   static bool msgPrinted(false);
   if (!msgPrinted) {
      std::cout << std::endl;
      std::cout << "WARNING!! The function Glut::glutFormFunc() is deprecated - use Glut::Factory::createObj() instead!";
      std::cout << std::endl;
      msgPrinted = true;
   }
   return Factory::createObj(name);
}

} // End Glut namespace
} // End Eaagles namespace

