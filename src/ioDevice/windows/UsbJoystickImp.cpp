//==============================================================================
// UsbJoystickImp -- MS Windows implementation
//
// Standard MS Windows joystick has 8 channels ...
//
//    channel     Axis
//       0         X
//       1         Y
//       2         Z
//       3         R
//       4         U
//       5         V
//       6         POV: back(1.0); forward(-1.0); center(0.0)
//       7         POV: right(1.0); left(-1.0); center(0.0)
//
//==============================================================================

#include "./UsbJoystickImp.h"

namespace Eaagles {
namespace IoDevice {

IMPLEMENT_SUBCLASS(UsbJoystickImp,"UsbJoystick")
EMPTY_SLOTTABLE(UsbJoystickImp)
EMPTY_SERIALIZER(UsbJoystickImp)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
UsbJoystickImp::UsbJoystickImp()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void UsbJoystickImp::initData()
{
   for (unsigned int i = 0; i < MAX_AI; i++) {
      cmin[i] = 0;
      cmax[i] = 0;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UsbJoystickImp::copyData(const UsbJoystickImp& org, const bool)
{
   BaseClass::copyData(org);

   initData();

   for (unsigned int i = 0; i < MAX_AI; i++) {
      cmin[i] = org.cmin[i];
      cmax[i] = org.cmax[i];
   }

}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void UsbJoystickImp::deleteData()
{
}

//------------------------------------------------------------------------------
// reset() -- Reset (open) device
//------------------------------------------------------------------------------
void UsbJoystickImp::reset()
{
   BaseClass::reset();

   // ---
   // when the number if AIs and DIs are both zero, try to get the
   // capabilities of the joystick
   // ---
   if (numAI == 0 && numDI == 0) {

      JOYCAPS jsCaps;  // joystick capabilities

      // query windows for capabilities
      if (joyGetDevCaps(deviceIndex, &jsCaps, sizeof(jsCaps)) == JOYERR_NOERROR) {

         // number of analogs (MS Windows have 8 channels)
         numAI = 8;

         // Set channel max/min values
         setMaxMin( 0, LCreal(jsCaps.wXmax), LCreal(jsCaps.wXmin) );
         setMaxMin( 1, LCreal(jsCaps.wYmax), LCreal(jsCaps.wYmin) );
         setMaxMin( 2, LCreal(jsCaps.wZmax), LCreal(jsCaps.wZmin) );
         setMaxMin( 3, LCreal(jsCaps.wRmax), LCreal(jsCaps.wRmin) );
         setMaxMin( 4, LCreal(jsCaps.wUmax), LCreal(jsCaps.wUmin) );
         setMaxMin( 5, LCreal(jsCaps.wVmax), LCreal(jsCaps.wVmin) );

         // number of bits
         {
            unsigned short tmp = (unsigned short) jsCaps.wNumButtons;
            if (tmp > MAX_DI) tmp = MAX_DI;
            numDI = tmp;
         }
      }

   }
}

//------------------------------------------------------------------------------
// Go get our AIs and DIs here
//------------------------------------------------------------------------------
void UsbJoystickImp::processInputs(const Eaagles::LCreal dt, Basic::IoData* const pInData)
{
   JOYINFOEX js;
   js.dwFlags = JOY_RETURNALL;   // return all joystick information
   js.dwSize  = sizeof(js);      // set the size of the structure / effectively sets version

   // query joystick for its position and button status
   MMRESULT status = joyGetPosEx(deviceIndex, &js);
   if (status == JOYERR_NOERROR) {

      // First 6 channels are X, Y, Z, R, U, V, and they need to be scaled.
      setInputScaled(0, LCreal(js.dwXpos));
      setInputScaled(1, LCreal(js.dwYpos));
      setInputScaled(2, LCreal(js.dwZpos));
      setInputScaled(3, LCreal(js.dwRpos));
      setInputScaled(4, LCreal(js.dwUpos));
      setInputScaled(5, LCreal(js.dwVpos));

      // Last two channels are set using the POV angle
      {
         LCreal povLR = 0;
         LCreal povFB = 0;
         if (js.dwPOV != JOY_POVCENTERED) {

            // right/left
            if (js.dwPOV >= (45*100) && js.dwPOV <= (135*100)) {
               povLR = 1.0f;  //right
            }
            else if (js.dwPOV >= (225*100) && js.dwPOV <= (315*100)) {
               povLR = -1.0f; // left
            }

            // back/forward
            if (js.dwPOV >= (135*100) && js.dwPOV <= (225*100)) {
               povFB = 1.0f;  // back
            }
            else if (js.dwPOV >= (315*100) || js.dwPOV <= (45*100)) {
               povFB = -1.0f; // forward
            }

         }
         inData[6] = povLR;
         inData[7] = povFB;
      }

      // update all individual button states
      {
         unsigned long buttons = js.dwButtons;
         for (unsigned int i = 0; i < numDI; i++) {
            inBits[i] = (buttons & (1 << i)) != 0;
         }
      }
   }

   // Update our base class, which will call our component DI handlers
   BaseClass::processInputs(dt,pInData);
}


//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------

// Set an analog input channels min/max values
bool UsbJoystickImp::setMaxMin(unsigned int channel, LCreal max, LCreal min)
{
   bool ok = false;
   if (channel < numAI) {
      cmax[channel] = max;
      cmin[channel] = min;
      ok = true;
   }
   return ok;
}

// Set an analog input channel values using a raw input and the max/min values
bool UsbJoystickImp::setInputScaled(unsigned int cn, LCreal raw)
{
   bool ok = false;
   if (cn < numAI) {
      LCreal normalized = (raw - cmin[cn])/(cmax[cn] - cmin[cn]);  // range: [ 0 ... 1 ]
      LCreal v11 = (normalized * 2.0f) - 1.0f;                     // range: [ -1 ... 1 ]
      inData[cn] = v11;
      ok = true;
   }
   return ok;
}

} // IoDevice namespace
} // end Eaagles namespace
