
#ifndef __oe_iodevice_windows_UsbJoystick_msvc_H__
#define __oe_iodevice_windows_UsbJoystick_msvc_H__

#include "openeaagles/iodevice/Controller.hpp"

namespace oe {
namespace iodevice {

//------------------------------------------------------------------------------
// Class: UsbJoystick
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
   bool setMaxMin(const unsigned int channel, const double max, const double min);
   bool setInputScaled(const unsigned int channel, const double raw);

   // ---
   // analog
   // ---
   double cmin[MAX_AI] {};     // channel min values
   double cmax[MAX_AI] {};     // channel max values

};

}
}

#endif


