
#ifndef __oe_iodevice_AnalogInput_H__
#define __oe_iodevice_AnalogInput_H__

#include "openeaagles/base/IoAdapter.hpp"

namespace oe {
namespace base { class Number; class Table1; }
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
class AnalogInput : public base::IoAdapter
{
   DECLARE_SUBCLASS(AnalogInput, base::IoAdapter)

public:
   AnalogInput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   double getValue() const;
   double getDeadband() const;
   double getOffset() const;
   double getGain() const;
   const base::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const double);
   bool setDeadband(const double);
   bool setOffset(const double);
   bool setGain(const double);
   bool setTable(const base::Table1* const);

   virtual void processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData) override;
   virtual void processOutputs(const double dt, const base::IoData* const outData, base::IoDevice* const device) override;

protected:
   virtual double convert(const double vin, const double dt);

   // Slot functions
   virtual bool setSlotLocation(const base::Number* const msg);
   virtual bool setSlotChannel(const base::Number* const msg);
   virtual bool setSlotValue(const base::Number* const msg);
   virtual bool setSlotDeadband(const base::Number* const);
   virtual bool setSlotOffset(const base::Number* const msg);
   virtual bool setSlotGain(const base::Number* const msg);

private:
   void initData();

   bool devEnb {};               // Device enabled
   unsigned int location {};     // IoData analog input channel number
   unsigned int channel {};      // Analog channel number
   double value {};              // Initial value
   double deadband {};           // Deadband value
   double offset {};             // Offset
   double gain {1.0};            // Gain
   const base::Table1* table {}; // Shaping table
};

}
}

#endif
