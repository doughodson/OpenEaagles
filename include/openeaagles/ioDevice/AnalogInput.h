//------------------------------------------------------------------------------
// Class: AnalogInput
//------------------------------------------------------------------------------
#ifndef __Eaagles_IoDevice_AnalogInput_H__
#define __Eaagles_IoDevice_AnalogInput_H__

#include "openeaagles/basic/IoAdapter.h"

namespace Eaagles {
   namespace Basic { class Number; class Table1; }

namespace IoDevice {
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
// Form Name: AnalogInput
// Slots:
//      ai           <Number>    Analog Input location (IoData's AI channel)
//      channel      <Number>    Device channel number
//      value        <Number>    Initial value [ -1.0 ... 1.0 ] (default: 0)
//      deadband     <Number>    Deadband: [ 0 .. 1 ] (default: 0.0)
//      offset       <Number>    Offset value (default: 0.0)
//      gain         <Number>    Gain value   (default: 1.0)
//      table        <Table1>    Shapping function table (default: none)
//
//------------------------------------------------------------------------------
class AnalogInput : public Basic::IoAdapter {
   DECLARE_SUBCLASS(AnalogInput,Basic::IoAdapter)

public:
   AnalogInput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   LCreal getValue() const;
   LCreal getDeadband() const;
   LCreal getOffset() const;
   LCreal getGain() const;
   const Basic::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const LCreal);
   bool setDeadband(const LCreal);
   bool setOffset(const LCreal);
   bool setGain(const LCreal);
   bool setTable(const Basic::Table1* const);

   // Basic::IoAdapter functions
   virtual void processInputs(const LCreal dt, const Basic::IoDevice* const device, Basic::IoData* const inData);
   virtual void processOutputs(const LCreal dt, const Basic::IoData* const outData, Basic::IoDevice* const device);

protected:
   virtual LCreal convert(const LCreal vin, const LCreal dt);

   // Slot functions
   virtual bool setSlotLocation(const Basic::Number* const msg);
   virtual bool setSlotChannel(const Basic::Number* const msg);
   virtual bool setSlotValue(const Basic::Number* const msg);
   virtual bool setSlotDeadband(const Basic::Number* const);
   virtual bool setSlotOffset(const Basic::Number* const msg);
   virtual bool setSlotGain(const Basic::Number* const msg);

private:
   void initData();

   bool devEnb;            // Device enabled
   unsigned int location;  // IoData analog input channel number
   unsigned int channel;   // Analog channel number
   LCreal value;            // Initial value
   LCreal deadband;         // Deadband value
   LCreal offset;           // Offset
   LCreal gain;             // Gain
   const Basic::Table1* table; // Shaping table
};

} // end IoDevice
} // end Eaagles namespace

#endif
