
#ifndef __oe_iodevice_AnalogOutput_H__
#define __oe_iodevice_AnalogOutput_H__

#include "openeaagles/base/io/IoAdapter.hpp"

namespace oe {
namespace base { class Number; class Table1; }
namespace iodevice {

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
class AnalogOutput : public base::IoAdapter
{
   DECLARE_SUBCLASS(AnalogOutput, base::IoAdapter)

public:
   AnalogOutput();

   unsigned int getLocation() const;
   unsigned int getChannel() const;
   double getValue() const;
   double getOffset() const;
   double getGain() const;
   const base::Table1* getTable() const;

   bool setLocation(const unsigned int);
   bool setChannel(const unsigned int);
   bool setValue(const double);
   bool setOffset(const double);
   bool setGain(const double);
   bool setTable(const base::Table1* const);

   virtual void processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData) override;
   virtual void processOutputs(const double dt, const base::IoData* const outData, base::IoDevice* const device) override;

protected:
   // Slot functions
   virtual bool setSlotLocation(const base::Number* const msg);
   virtual bool setSlotChannel(const base::Number* const msg);
   virtual bool setSlotValue(const base::Number* const msg);
   virtual bool setSlotOffset(const base::Number* const msg);
   virtual bool setSlotGain(const base::Number* const msg);

private:
   bool devEnb {};               // Device enabled
   unsigned int location {};      // IoData analog output channel number
   unsigned int channel {};       // Analog channel number
   double value {};               // Initial value
   double offset {};              // Offset:  value = gain * (vin - offset)
   double gain {1.0};             // Gain:    value = gain * (vin - offset)
   const base::Table1* table {};  // Shaping table
};

}
}

#endif
