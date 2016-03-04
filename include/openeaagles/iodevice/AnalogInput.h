
#ifndef __oe_iodevice_AnalogInput_H__
#define __oe_iodevice_AnalogInput_H__

#include "openeaagles/base/IoAdapter.h"

namespace oe {

namespace basic { class Number; class Table1; }

namespace iodevice {

class IoDevice;

//------------------------------------------------------------------------------
// Class:  AnalogInput
//
// Description:  Manages an analog input (AI).
//
// AI processing:
//
//    1) Deadband; The AI value is clamped to zero(0) if it's less
//       than +deadband and greater than -deadband.  There is no clamping if
//       deadband is zero (the default)
//
//          V1 = clamp(AI, deadband)
//
//
//    2) Offset and Gain: the input value (after deadband clamping) can
//       be offset and then scaled by gain.  Defaults are zero offset
//       and unity gain.
//
//          V2 = (V1 - offset) * gain
//
//
//    3) Optional shaping function: the input value (after gain and offset)
//       can be shaped using an 1-D LFI table.
//
//          V3 = f(V2)
//
// Factory name: AnalogInput
// Slots:
//      ai           <Number>    Analog Input location (IoData's AI channel)
//      channel      <Number>    Device channel number
//      value        <Number>    Initial value [ -1.0 ... 1.0 ] (default: 0)
//      deadband     <Number>    Deadband: [ 0 .. 1 ] (default: 0.0)
//      offset       <Number>    Offset value (default: 0.0)
//      gain         <Number>    Gain value   (default: 1.0)
//      table        <Table1>    Shaping function table (default: none)
//
//------------------------------------------------------------------------------
class AnalogInput : public basic::IoAdapter
{
   DECLARE_SUBCLASS(AnalogInput,basic::IoAdapter)

public:
   AnalogInput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   LCreal getValue() const;
   LCreal getDeadband() const;
   LCreal getOffset() const;
   LCreal getGain() const;
   const basic::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const LCreal);
   bool setDeadband(const LCreal);
   bool setOffset(const LCreal);
   bool setGain(const LCreal);
   bool setTable(const basic::Table1* const);

   void processInputs(const LCreal dt, const basic::IoDevice* const device, basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const basic::IoData* const outData, basic::IoDevice* const device) override;

protected:
   virtual LCreal convert(const LCreal vin, const LCreal dt);

   // Slot functions
   virtual bool setSlotLocation(const basic::Number* const msg);
   virtual bool setSlotChannel(const basic::Number* const msg);
   virtual bool setSlotValue(const basic::Number* const msg);
   virtual bool setSlotDeadband(const basic::Number* const);
   virtual bool setSlotOffset(const basic::Number* const msg);
   virtual bool setSlotGain(const basic::Number* const msg);

private:
   void initData();

   bool devEnb;            // Device enabled
   unsigned int location;  // IoData analog input channel number
   unsigned int channel;   // Analog channel number
   LCreal value;            // Initial value
   LCreal deadband;         // Deadband value
   LCreal offset;           // Offset
   LCreal gain;             // Gain
   const basic::Table1* table; // Shaping table
};

} // end iodevice
} // end oe namespace

#endif
