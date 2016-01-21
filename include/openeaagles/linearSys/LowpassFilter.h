//------------------------------------------------------------------------------
// LowpassFilter class -- First order low pass filter
//------------------------------------------------------------------------------
#ifndef __oe_LinearSystem_LowpassFilter_H__
#define __oe_LinearSystem_LowpassFilter_H__

#include "openeaagles/linearSys/FirstOrderTf.h"

namespace oe {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  LowpassFilter
// Base class:  Object -> ScalerFunc-> DiffEquation -> Sz1 -> LowpassFilter
//
// Description: Models a first order low pass filter with cutoff frequency of Wc radians/sec
//
// Transfer function:
//
//           wc
//       ---------
//         s + wc
//
// Factory name: Lag1Filter
// Slots:
//    wc    <Frequency>  Cutoff Frequency
//    wc    <Number>     Cutoff frequency (radian/sec)
//
//
//------------------------------------------------------------------------------
class LowpassFilter : public FirstOrderTf
{
    DECLARE_SUBCLASS(LowpassFilter,FirstOrderTf)

public:
   LowpassFilter();
   LowpassFilter(const unsigned int rate, const LCreal w);

   LCreal getWc() const                           { return wc; }
   virtual bool setWc(const LCreal v);

   virtual bool setSlotWc(const Basic::Frequency* const msg);
   virtual bool setSlotWc(const Basic::Number* const msg);

private:
   LCreal wc;    // Cutoff frequency (rad/sec)
};

} // End LinearSystem namespace
} // End oe namespace

#endif
