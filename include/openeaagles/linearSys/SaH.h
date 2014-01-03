//------------------------------------------------------------------------------
// SaH class -- Sample & Hold function
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_SaH_H__
#define __Eaagles_LinearSystem_SaH_H__

#include "openeaagles/linearSys/ScalerFunc.h"

namespace Eaagles {
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
class SaH : public ScalerFunc {
    DECLARE_SUBCLASS(SaH,ScalerFunc)

public:
   SaH();
   SaH(const unsigned int rate, const unsigned int sampleRate);

   unsigned int getSampleRate() const  { return sampleRate; } // hz

   // Time since last sample (sec) [ 0 .. 1/getSampleRate() ]
   LCreal getAge() const { return time; }

   // Set Parameter
   virtual bool setSampleRate(const unsigned int v);   // hz

   // Set slot functions
   virtual bool setSlotSampleRate(const Basic::Frequency* const msg);
   virtual bool setSlotSampleRate(const Basic::Number* const msg);

   // ScalerFunc class interface
   virtual LCreal g(const LCreal x);
   virtual bool isValid() const;

protected:
   // ScalerFunc class protected interface
   virtual void initialize();

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 1;

   LCreal       time;       // Current time since last sample (s)
   LCreal       stime;      // Sample time: time of next sample (s)
   unsigned int sampleRate; // Sample rate (hz)
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
