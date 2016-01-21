//------------------------------------------------------------------------------
// Class: UsbJoystickImp -- Linux implementation
//------------------------------------------------------------------------------
#ifndef __oe_IoDevice_Linux_UsbJoystickImp_H__
#define __oe_IoDevice_Linux_UsbJoystickImp_H__

#include "openeaagles/ioDevice/UsbJoystick.h"

namespace oe {
namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  UsbJoystickImp
//
// Description:  Linux version of the USB Joystick device.
//               (See IoDevice::UsbJoystick)
//
// Form Name: UsbJoystick
//
//------------------------------------------------------------------------------
class UsbJoystickImp : public UsbJoystick {
    DECLARE_SUBCLASS(UsbJoystickImp,UsbJoystick)

public:
   UsbJoystickImp();

   // Basic::IoDevice functions
   void processInputs(const LCreal dt, Basic::IoData* const inData) override;

   // Basic::Component functions
   void reset() override;

private:
   void initData();

   int stream;
   char deviceName[128];
};

} // end IoDevice
} // end oe namespace

#endif
