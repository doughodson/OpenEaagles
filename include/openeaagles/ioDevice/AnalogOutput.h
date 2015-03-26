
#ifndef __Eaagles_IoDevice_AnalogOutput_H__
#define __Eaagles_IoDevice_AnalogOutput_H__

#include "openeaagles/basic/IoAdapter.h"

namespace Eaagles {

namespace Basic { class Number; class Table1; }

namespace IoDevice {

//------------------------------------------------------------------------------
// Class:  AnalogOutput
//
// Description:  Manages an analog output (AO).
//
// AO processing:
//
//    1) Offset and Gain: Defaults are zero offset and unity gain.
//
//          V1 = (value / gain ) + offset
//
//
//    2) Optional shaping function: the output value (after gain and offset)
//       can be shaped using an 1-D LFI table.
//
//          Vout = f(V1)
// 
// Slots:
//      ao           <Number>    Analog Output location (IoData AO's channel)
//      channel      <Number>    Device channel number
//      value        <Number>    Initial value [ -1.0 ... 1.0 ] (default: 0)
//      offset       <Number>    Offset value (default: 0.0)
//      gain         <Number>    Gain value   (default: 1.0)
//      table        <Table1>    Shaping function table (default: none)
//
//------------------------------------------------------------------------------
class AnalogOutput : public Basic::IoAdapter
{
   DECLARE_SUBCLASS(AnalogOutput, Basic::IoAdapter)

public:
   AnalogOutput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   LCreal getValue() const;
   LCreal getOffset() const;
   LCreal getGain() const;
   const Basic::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const LCreal);
   bool setOffset(const LCreal);
   bool setGain(const LCreal);
   bool setTable(const Basic::Table1* const);

   // Basic::IoAdapter interface
   void processInputs(const LCreal dt, const Basic::IoDevice* const device, Basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const Basic::IoData* const outData, Basic::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const Basic::Number* const msg);
   virtual bool setSlotChannel(const Basic::Number* const msg);
   virtual bool setSlotValue(const Basic::Number* const msg);
   virtual bool setSlotOffset(const Basic::Number* const msg);
   virtual bool setSlotGain(const Basic::Number* const msg);

private:
   void initData();

   bool  devEnb;              // Device enabled
   unsigned int location;     // IoData analog output channel number
   unsigned int channel;      // Analog channel number
   LCreal       value;        // Initial value
   LCreal       offset;       // Offset:  value = gain * (vin - offset)
   LCreal       gain;         // Gain:    value = gain * (vin - offset)
   const Basic::Table1* table; // Shaping table
};

} // end IoDevice
} // end Eaagles namespace

#endif
