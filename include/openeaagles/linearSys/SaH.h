//------------------------------------------------------------------------------
// SaH class -- Sample & Hold function
//------------------------------------------------------------------------------
#ifndef __oe_LinearSystem_SaH_H__
#define __oe_LinearSystem_SaH_H__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace oe {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: SaH
// Base class:  Object -> ScalerFunc -> SaH
//
// Description: Sample and Hold.  The g() function's X input is sampled at
//              a rate of 'sampleRate' and held as the output Y until the
//              next sample is taken.
//
//
// Factory name: SaH
// Slots:
//    rate  <Frequency>  Sample rate
//    rate  <Number>     Sample rate Hz
//
//------------------------------------------------------------------------------
class SaH : public ScalerFunc
{
    DECLARE_SUBCLASS(SaH,ScalerFunc)

public:
   SaH();
   SaH(const unsigned int rate, const unsigned int sampleRate);

   unsigned int getSampleRate() const         { return sampleRate; } // hz

   // Time since last sample (sec) [ 0 .. 1/getSampleRate() ]
   LCreal getAge() const                      { return time; }

   // Set Parameter
   virtual bool setSampleRate(const unsigned int v);   // hz

   // Set slot functions
   virtual bool setSlotSampleRate(const basic::Frequency* const msg);
   virtual bool setSlotSampleRate(const basic::Number* const msg);

   LCreal g(const LCreal x) override;
   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();

   static const unsigned int ORDER = 1;

   LCreal       time;       // Current time since last sample (s)
   LCreal       stime;      // Sample time: time of next sample (s)
   unsigned int sampleRate; // Sample rate (hz)
};

} // End LinearSystem namespace
} // End oe namespace

#endif
