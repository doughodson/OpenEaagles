
#ifndef __oe_iodevice_Ai2DiSwitch_H__
#define __oe_iodevice_Ai2DiSwitch_H__

#include "openeaagles/base/IoAdapter.hpp"

namespace oe {

namespace base { class Number; }

namespace iodevice {

class IoDevice;

//------------------------------------------------------------------------------
// Class:  Ai2DiSwitch
//
// Description: Signal converter:  switches a DI based on an analog input.
//              The DI is switches to true if the analog input becomes greater
//              than or equal to the 'level' value, otherwise the DI is set to
//              false.  If the 'inverted' flag is true then the DI's value is
//              inverted before being set.
//
//    Analog      Invert      DI
//    input       flag        value
//    ----------  ----------  -----------
//      < level      false      false
//     >= level      false      true
//      < level      true       true
//     >= level      true       false
//
//
// Factory name: Ai2DiSwitch
// Slots:
//      di           <Number>    Discrete Input location (IoData's DI channel)
//      channel      <Number>    Device's AI channel number
//      level        <Number>    Level to switch DI (default: 0)
//      inverted     <Boolean>   Inverted bit flag (default: false)
//
//------------------------------------------------------------------------------
class Ai2DiSwitch : public base::IoAdapter
{
   DECLARE_SUBCLASS(Ai2DiSwitch, base::IoAdapter)

public:
   Ai2DiSwitch();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   double getLevel() const;
   bool getInvertFlag() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setLevel(const double);
   bool setInvertFlag(const bool);

   virtual void processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData) override;
   virtual void processOutputs(const double dt, const base::IoData* const outData, base::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const base::Number* const msg);
   virtual bool setSlotChannel(const base::Number* const msg);
   virtual bool setSlotLevel(const base::Number* const msg);
   virtual bool setSlotInverted(const base::Number* const msg);

private:
   void initData();

   bool devEnb;             // Device enabled
   unsigned int location;   // IoData input bit location
   unsigned int channel;    // Port's channel (bit) number
   double level;            // Switching level
   bool invert;             // Inverted bit flag
};

}
}

#endif
