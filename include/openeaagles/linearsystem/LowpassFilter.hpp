
#ifndef __oe_linearsystem_LowpassFilter_H__
#define __oe_linearsystem_LowpassFilter_H__

#include "openeaagles/linearsystem/FirstOrderTf.hpp"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  LowpassFilter
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
//------------------------------------------------------------------------------
class LowpassFilter : public FirstOrderTf
{
   DECLARE_SUBCLASS(LowpassFilter, FirstOrderTf)

public:
   LowpassFilter();
   LowpassFilter(const unsigned int rate, const double w);

   double getWc() const                           { return wc; }
   virtual bool setWc(const double v);

   virtual bool setSlotWc(const base::Frequency* const msg);
   virtual bool setSlotWc(const base::Number* const msg);

private:
   double wc {};    // Cutoff frequency (rad/sec)
};

}
}

#endif
