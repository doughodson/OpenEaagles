
#ifndef __oe_iodevice_platform_UsbJoystick_linux_H__
#define __oe_iodevice_platform_UsbJoystick_linux_H__

#include "openeaagles/iodevice/Controller.hpp"

namespace oe {
namespace iodevice {

//------------------------------------------------------------------------------
// Class: UsbJoystick
//
// Factory name: UsbJoystick
//------------------------------------------------------------------------------
class UsbJoystick : public Controller
{
    DECLARE_SUBCLASS(UsbJoystick, Controller)

public:
   UsbJoystick();

   // base::IoDevice functions
   virtual void processInputs(const double dt, base::IoData* const inData) override;

   // base::Component functions
   virtual void reset() override;

private:
   void initData();

   int stream {};
   char deviceName[128];
};

}
}

#endif

