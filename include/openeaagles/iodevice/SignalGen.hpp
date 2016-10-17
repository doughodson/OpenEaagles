
#ifndef __oe_iodevice_SignalGen_H__
#define __oe_iodevice_SignalGen_H__

#include "openeaagles/base/IoAdapter.hpp"

namespace oe {

namespace base { class Angle; class Frequency; class String; }

namespace iodevice {

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
class SignalGen : public base::IoAdapter
{
   DECLARE_SUBCLASS(SignalGen, base::IoAdapter)

public:
   enum Signal { SINE, COSINE, SQUARE, SAW };

public:
   SignalGen();

   Signal getSignalType() const;
   double getFrequency() const;     // Hz
   double getPhase() const;         // Radians

   bool setSignalType(const Signal);
   bool setFrequency(const double); // Hz
   bool setPhase(const double);     // Radians

   // IoData's AI channel (AI signal generator only)
   unsigned int getLocation() const;
   bool setLocation(const unsigned int);

   // Output device channel number (AO signal generator only)
   unsigned int getChannel() const;
   bool setChannel(const unsigned int);

   virtual void processInputs(const double dt, const base::IoDevice* const device, base::IoData* const inData) override;
   virtual void processOutputs(const double dt, const base::IoData* const outData, base::IoDevice* const device) override;

   virtual void reset() override;

protected:
   // AnalogOutput interface
   double calc(const double dt);

   // Slot functions
   virtual bool setSlotLocation(const base::Number* const msg);
   virtual bool setSlotChannel(const base::Number* const msg);
   virtual bool setSlotSignal(const base::String* const msg);
   virtual bool setSlotFrequency(const base::Frequency* const msg);
   virtual bool setSlotPhase(const base::Angle* const msg);

private:
   void initData();

   unsigned int location;  // AI signal gen: IoData's AI channel
   unsigned int channel;   // AO signal gen: Output device channel number

   Signal signal;          // Signal type
   double phase;           // Phase shift (rad)
   double freq;            // Frequency (rad/sec)
   double time;            // Time since last reset
};


}
}

#endif
