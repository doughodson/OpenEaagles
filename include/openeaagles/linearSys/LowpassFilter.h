//------------------------------------------------------------------------------
// LowpassFilter class -- First order low pass filter
//------------------------------------------------------------------------------
#ifndef __LinearSystem_LowpassFilter_H_3E25DB1C_24FD_4f9b_A65D_40CBF4442ABC__
#define __LinearSystem_LowpassFilter_H_3E25DB1C_24FD_4f9b_A65D_40CBF4442ABC__

#include "openeaagles/linearSys/FirstOrderTf.h"

namespace Eaagles {
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
// Form name: Lag1Filter
// Slots:
//    wc    <Frequency>  Cutoff Frequency
//    wc    <Number>     Cutoff frequency (radian/sec)
//
//
// GUID: {3E25DB1C-24FD-4f9b-A65D-40CBF4442ABC}
//------------------------------------------------------------------------------
class LowpassFilter : public FirstOrderTf {
    DECLARE_SUBCLASS(LowpassFilter,FirstOrderTf)

public:
   LowpassFilter();
   LowpassFilter(const unsigned int rate, const LCreal w);

   LCreal getWc() const { return wc; }
   virtual bool setWc(const LCreal v);

   virtual bool setSlotWc(const Basic::Frequency* const msg);
   virtual bool setSlotWc(const Basic::Number* const msg);

private:
   LCreal wc;    // Cutoff frequency (rad/sec)
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif      /* __LinearSystem_LowpassFilter_H_3E25DB1C_24FD_4f9b_A65D_40CBF4442ABC__ */
