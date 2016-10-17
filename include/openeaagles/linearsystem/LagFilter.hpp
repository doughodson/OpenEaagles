
#ifndef __oe_linearsystem_LagFilter_H__
#define __oe_linearsystem_LagFilter_H__

#include "openeaagles/linearsystem/FirstOrderTf.hpp"

namespace oe {
   namespace base { class Time; }

namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  LagFilter
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
   DECLARE_SUBCLASS(LagFilter, FirstOrderTf)

public:
   LagFilter();
   LagFilter(const unsigned int rate, const double tau);

   double getTau() const                         { return tau; }
   virtual bool setTau(const double v);

   virtual bool setSlotTau(const base::Time* const msg);
   virtual bool setSlotTau(const base::Number* const msg);

private:
   double tau;    // filter time constant (seconds)
};

}
}

#endif
