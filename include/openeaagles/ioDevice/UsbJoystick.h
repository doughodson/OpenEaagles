//------------------------------------------------------------------------------
// Class: UsbJoystick
//------------------------------------------------------------------------------
#ifndef __Eaagles_IoDevice_UsbJoystick_H__
#define __Eaagles_IoDevice_UsbJoystick_H__

#include "openeaagles/basic/IoDevice.h"

namespace Eaagles {
   namespace Basic { class Number; }

namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  UsbJoystick
//
// Description:  Communicates with a standard USB Joystick device.
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
//    2) Standard Linux just maps one for one (axis to channel)
//
//
//
// Form Name: UsbJoystick
//
// Slots:
//    deviceIndex <Number>    Unit index
//
//------------------------------------------------------------------------------
class UsbJoystick : public Basic::IoDevice {
    DECLARE_SUBCLASS(UsbJoystick,Basic::IoDevice)

public:
   UsbJoystick();

   virtual unsigned int getDeviceIndex() const;
   virtual bool setDeviceIndex(const int value);

   // IoDevice interface
   virtual unsigned short getNumDiscreteInputChannels() const;
   virtual unsigned short getNumDiscreteInputPorts() const;
   virtual bool getDiscreteInput(bool* const value, const unsigned int channel, const unsigned int port) const;
   virtual unsigned short getNumAnalogInputs() const;
   virtual bool getAnalogInput(LCreal* const value, const unsigned int channel) const;

protected:

   // Slot functions
   virtual bool setSlotDeviceIndex(const Basic::Number* const msg);

   unsigned int deviceIndex; // Device index

   // ---
   // analog 
   // ---
   static const unsigned short MAX_AI = 16;
   unsigned short numAI;   // Number of analog channels
   LCreal inData[MAX_AI];   // Input analog array

   // ---
   // digital bits 
   // ---
   static const unsigned short MAX_DI = 32;
   unsigned short numDI;      // Number of input bits
   bool inBits[MAX_DI];       // Input bit array

private:
   void initData();
};

} // end IoDevice
} // end Eaagles namespace

#endif
