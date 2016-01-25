
#ifndef __oe_ioDevice_AnalogOutput_H__
#define __oe_ioDevice_AnalogOutput_H__

#include "openeaagles/basic/IoAdapter.h"

namespace oe {

namespace basic { class Number; class Table1; }

namespace ioDevice {

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
class AnalogOutput : public basic::IoAdapter
{
   DECLARE_SUBCLASS(AnalogOutput, basic::IoAdapter)

public:
   AnalogOutput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   LCreal getValue() const;
   LCreal getOffset() const;
   LCreal getGain() const;
   const basic::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const LCreal);
   bool setOffset(const LCreal);
   bool setGain(const LCreal);
   bool setTable(const basic::Table1* const);

   void processInputs(const LCreal dt, const basic::IoDevice* const device, basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const basic::IoData* const outData, basic::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const basic::Number* const msg);
   virtual bool setSlotChannel(const basic::Number* const msg);
   virtual bool setSlotValue(const basic::Number* const msg);
   virtual bool setSlotOffset(const basic::Number* const msg);
   virtual bool setSlotGain(const basic::Number* const msg);

private:
   void initData();

   bool  devEnb;              // Device enabled
   unsigned int location;     // IoData analog output channel number
   unsigned int channel;      // Analog channel number
   LCreal       value;        // Initial value
   LCreal       offset;       // Offset:  value = gain * (vin - offset)
   LCreal       gain;         // Gain:    value = gain * (vin - offset)
   const basic::Table1* table; // Shaping table
};

} // end ioDevice
} // end oe namespace

#endif
