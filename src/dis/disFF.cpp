
#include "openeaagles/dis/disFF.h"
#include "openeaagles/dis/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Network {
namespace Dis {

Basic::Object* disFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Network::Dis::disFormFunc() is deprecated - use Network::Dis::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Dis namespace
} // End Network namespace
} // End oe namespace
