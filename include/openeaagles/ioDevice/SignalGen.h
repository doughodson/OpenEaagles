
#ifndef __oe_ioDevice_SignalGen_H__
#define __oe_ioDevice_SignalGen_H__

#include "openeaagles/basic/IoAdapter.h"

namespace oe {

namespace basic { class Angle; class Frequency; class String; }

namespace ioDevice {

class IoDevice;
class IoData;

//------------------------------------------------------------------------------
// Class:  SignalGen
//
// Description: Analog Input (AI) Signal Generator: sine, cosine, square and
//              saw tooth waves.
//
//  Cycle Angle     SINE    COSINE    SQUARE     SAW
//  -----------    ------   ------    ------    ------
//      0           0.0      1.0       1.0       0.0
//     90           1.0      0.0       1.0       0.25
//    180           0.0     -1.0       0.0       0.5
//    270          -1.0      0.0       0.0       0.75
//   <360           0.0      1.0       1.0       1.0
//
//
// Factory name: SignalGenAI
// Slots:
//      signal    <Identifier>   ! Signal type { SINE, COSINE, SQUARE, SAW }
//                               !  (default: SINE )
//
//      frequency <Frequency>    ! Signal frequency (should be <much> less
//                               !   than 1/2 of our core update rate)
//                               !   (default: 1/2 HZ or 2 seconds per cycle)
//
//      phase     <Angle>        ! Phase shift (default: 0 degrees)
//
//      ai        <Number>       ! IoData's AI channel (AI signal generator only)
//      channel   <Number>       ! Output device channel number (AO signal generator only)
//
//------------------------------------------------------------------------------
class SignalGen : public basic::IoAdapter
{
   DECLARE_SUBCLASS(SignalGen, basic::IoAdapter)

public:
   enum Signal { SINE, COSINE, SQUARE, SAW };

public:
   SignalGen();

   Signal getSignalType() const;
   LCreal getFrequency() const;     // Hz
   LCreal getPhase() const;         // Radians

   bool setSignalType(const Signal);
   bool setFrequency(const LCreal); // Hz
   bool setPhase(const LCreal);     // Radians

   // IoData's AI channel (AI signal generator only)
   unsigned int getLocation() const;
   bool setLocation(const unsigned int);

   // Output device channel number (AO signal generator only)
   unsigned int getChannel() const;
   bool setChannel(const unsigned int);

   void processInputs(const LCreal dt, const basic::IoDevice* const device, basic::IoData* const inData) override;
   void processOutputs(const LCreal dt, const basic::IoData* const outData, basic::IoDevice* const device) override;

   void reset() override;

protected:
   // AnalogOutput interface
   LCreal calc(const LCreal dt);

   // Slot functions
   virtual bool setSlotLocation(const basic::Number* const msg);
   virtual bool setSlotChannel(const basic::Number* const msg);
   virtual bool setSlotSignal(const basic::String* const msg);
   virtual bool setSlotFrequency(const basic::Frequency* const msg);
   virtual bool setSlotPhase(const basic::Angle* const msg);

private:
   void initData();

   unsigned int location;  // AI signal gen: IoData's AI channel
   unsigned int channel;   // AO signal gen: Output device channel number

   Signal signal;          // Signal type
   LCreal phase;           // Phase shift (rad)
   LCreal freq;            // Frequency (rad/sec)
   double time;            // Time since last reset
};


} // end ioDevice
} // end oe namespace

#endif
