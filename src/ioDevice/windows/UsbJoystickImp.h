//------------------------------------------------------------------------------
// Class: UsbJoystickImp -- MS Windows implementation
//------------------------------------------------------------------------------
#ifndef __oe_IoDevice_Windows_UsbJoystickImp_H__
#define __oe_IoDevice_Windows_UsbJoystickImp_H__

#include "openeaagles/ioDevice/UsbJoystick.h"

namespace oe {
namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  UsbJoystickImp
//
// Description:  MS Windows version of the USB Joystick device.
//               (See IoDevice::UsbJoystick)
//
// Notes:
//    1) Standard MS Windows joysticks have 8 channels, which are mapped ...
//          channel     Axis
//             0         X
//             1         Y
//             2         Z
//             3         R
//             4         U
//             5         V
//             6         POV: back(1.0); forward(-1.0); center(0.0)
//             7         POV: right(1.0); left(-1.0); center(0.0)
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
   bool setMaxMin(unsigned int channel, LCreal max, LCreal min);
   bool setInputScaled(unsigned int channel, LCreal raw);

   // ---
   // analog
   // ---
   LCreal cmin[MAX_AI];     // channel min values
   LCreal cmax[MAX_AI];     // channel max values

};

} // end IoDevice
} // end oe namespace

#endif
