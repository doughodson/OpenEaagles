//------------------------------------------------------------------------------
// Form function for the 'basic' library
//------------------------------------------------------------------------------

#include "openeaagles/basic/basicFF.h"
#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/Factory.h"
#include <iostream>

namespace Eaagles {
namespace Basic {

Object* basicFormFunc(const char* name)
{
   std::cout << std::endl;
   std::cout << "WARNING!! basicFormFunc() is deprecated, use Basic::Factory::createObj() instead!";
   std::cout << std::endl;
   return Factory::createObj(name);
}

} // End Basic namespace
} // End Eaagles namespace
