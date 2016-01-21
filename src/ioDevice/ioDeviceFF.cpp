//------------------------------------------------------------------------------
// Form function for the I/O devices 'IoDevice' library
//------------------------------------------------------------------------------

#include "openeaagles/ioDevice/ioDeviceFF.h"
#include "openeaagles/ioDevice/Factory.h"

#include "openeaagles/basic/Object.h"

namespace oe {
namespace IoDevice {

Basic::Object* ioDeviceFormFunc(const char* name)
{
   //static bool msgPrinted(false);
   //if (!msgPrinted) {
   //   std::cout << std::endl;
   //   std::cout << "WARNING! The function IoDevice::ioDeviceFormFunc() is deprecated - use IoDevice::Factory::createObj() instead.";
   //   std::cout << std::endl;
   //   msgPrinted = true;
   //}
   return Factory::createObj(name);
}

}  //end IoDevice
} // end oe namespace

