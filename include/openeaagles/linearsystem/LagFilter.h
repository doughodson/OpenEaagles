//------------------------------------------------------------------------------
// LagFilter class -- First order lag filter
//------------------------------------------------------------------------------
#ifndef __oe_linearsystem_LagFilter_H__
#define __oe_linearsystem_LagFilter_H__

#include "openeaagles/linearsystem/FirstOrderTf.h"

namespace oe {
   namespace basic { class Time; }

namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  LagFilter
// Base class:  Object -> ScalerFunc-> DiffEquation -> Sz1 -> LagFilter
//
// Description: Models a first order lag filter with a time constant 'tau' seconds
//
// Transfer function:
//
//           1
//       ---------
//        tau*S + 1
//
// Note: tau must be greater than zero.
//
// Factory name: LagFilter
// Slots:
//    tau    <Time>     Filer time constant
//    tau    <Number>   Filer time constant (sec)
//
//
//------------------------------------------------------------------------------
class LagFilter : public FirstOrderTf
{
    DECLARE_SUBCLASS(LagFilter,FirstOrderTf)

public:
   LagFilter();
   LagFilter(const unsigned int rate, const LCreal tau);

   LCreal getTau() const                         { return tau; }
   virtual bool setTau(const LCreal v);

   virtual bool setSlotTau(const basic::Time* const msg);
   virtual bool setSlotTau(const basic::Number* const msg);

private:
   LCreal tau;    // filter time constant (seconds)
};

} // End linearsystem namespace
} // End oe namespace

#endif
