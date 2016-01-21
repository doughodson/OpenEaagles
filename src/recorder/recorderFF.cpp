//------------------------------------------------------------------------------
// Form function for the 'recorder' library
//------------------------------------------------------------------------------
#include "openeaagles/recorder/recorderFF.h"
#include "openeaagles/recorder/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace Recorder {

Basic::Object* recorderFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function Recorder::recorderFormFunc() is deprecated - use Recorder::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

} // End Recorder namespace
} // End oe namespace
