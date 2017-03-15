
#ifndef __oe_iodevice_Controller_H__
#define __oe_iodevice_Controller_H__

#include "openeaagles/base/IoDevice.hpp"

#include <array>

namespace oe {
namespace base { class Number; }
namespace iodevice {

//------------------------------------------------------------------------------
// Class: Controller
//
// Description: Physically packages a set of analog and digital data reading
//              together to represent a controller interface (game, hotas, etc)
//
// Factory name: Controller
//
// Slots:
//    deviceIndex <Number>    Unit index
//
//------------------------------------------------------------------------------
class Controller : public base::IoDevice
{
    DECLARE_SUBCLASS(Controller, base::IoDevice)

public:
   Controller();

   virtual unsigned int getDeviceIndex() const;
   virtual bool setDeviceIndex(const int value);

   virtual unsigned short getNumDiscreteInputChannels() const override;
   virtual unsigned short getNumDiscreteInputPorts() const override;
   virtual bool getDiscreteInput(bool* const value, const unsigned int channel, const unsigned int port) const override;
   virtual unsigned short getNumAnalogInputs() const override;
   virtual bool getAnalogInput(double* const value, const unsigned int channel) const override;

protected:

   // Slot functions
   virtual bool setSlotDeviceIndex(const base::Number* const msg);

   unsigned int deviceIndex {}; // Device index

   // ---
   // analog
   // ---
   static const unsigned short MAX_AI = 16;
   unsigned short numAI {};                // Number of analog channels
   std::array<double, MAX_AI> inData {};   // Input analog array

   // ---
   // digital bits
   // ---
   static const unsigned short MAX_DI = 32;
   unsigned short numDI {};                 // Number of input bits
   std::array<bool, MAX_DI> inBits {};      // Input bit array

private:
   void initData();
};

}
}

#endif

