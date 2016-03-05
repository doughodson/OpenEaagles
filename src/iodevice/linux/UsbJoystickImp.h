//------------------------------------------------------------------------------
// Class: UsbJoystickImp -- Linux implementation
//------------------------------------------------------------------------------
#ifndef __oe_iodevice_linux_UsbJoystickImp_H__
#define __oe_iodevice_linux_UsbJoystickImp_H__

#include "openeaagles/iodevice/UsbJoystick.h"

namespace oe {
namespace iodevice {

//------------------------------------------------------------------------------
// Class:  UsbJoystickImp
//
// Description:  Linux version of the USB Joystick device.
//               (See IoDevice::UsbJoystick)
//
// Form Name: UsbJoystick
//
//------------------------------------------------------------------------------
class UsbJoystickImp : public UsbJoystick
{
    DECLARE_SUBCLASS(UsbJoystickImp,UsbJoystick)

public:
   UsbJoystickImp();

   // base::IoDevice functions
   void processInputs(const LCreal dt, base::IoData* const inData) override;

   // base::Component functions
   void reset() override;

private:
   void initData();

   int stream;
   char deviceName[128];
};

}
}

#endif

