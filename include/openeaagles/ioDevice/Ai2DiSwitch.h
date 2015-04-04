
#ifndef __Eaagles_IoDevice_Ai2DiSwitch_H__
#define __Eaagles_IoDevice_Ai2DiSwitch_H__

#include "openeaagles/basic/IoAdapter.h"

namespace Eaagles {

namespace Basic { class Number; }

namespace IoDevice {

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
class Ai2DiSwitch : public Basic::IoAdapter
{
   DECLARE_SUBCLASS(Ai2DiSwitch, Basic::IoAdapter)

public:
   Ai2DiSwitch();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   LCreal getLevel() const;
   bool getInvertFlag() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setLevel(const LCreal);
   bool setInvertFlag(const bool);

   void processInputs(const LCreal dt, const Basic::IoDevice* const device, Basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const Basic::IoData* const outData, Basic::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const Basic::Number* const msg);
   virtual bool setSlotChannel(const Basic::Number* const msg);
   virtual bool setSlotLevel(const Basic::Number* const msg);
   virtual bool setSlotInverted(const Basic::Number* const msg);

private:
   void initData();

   bool devEnb;            // Device enabled
   unsigned int location;     // IoData input bit location
   unsigned int channel;      // Port's channel (bit) number
   LCreal        level;       // Switching level
   bool         invert;       // Inverted bit flag
};

} // end IoDevice namespace
} // end Eaagles namespace

#endif
