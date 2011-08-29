//------------------------------------------------------------------------------
// Class: UsbJoystickImp -- Linux implementation
//------------------------------------------------------------------------------
#ifndef __IoDevice_Linu_UsbJoystickImp_H__
#define __IoDevice_Linu_UsbJoystickImp_H__

#include "openeaagles/ioDevice/UsbJoystick.h"

namespace Eaagles {
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
   virtual void processInputs(const LCreal dt, Basic::IoData* const inData);

   // Basic::Component functions
   virtual void reset();

private:
   void initData();

   int stream;
   char deviceName[128];
};

} // end IoDevice
} // end Eaagles namespace

#endif
