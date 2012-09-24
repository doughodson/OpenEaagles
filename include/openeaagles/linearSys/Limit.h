//------------------------------------------------------------------------------
// Limit function
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_Limit_H__
#define __Eaagles_LinearSystem_Limit_H__

#include "openeaagles/linearSys/LimitFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit
// Base class:  Basic::Object -> ScalerFunc -> LimitFunc -> Limit
//
// Description: Limits function: limits X to the upper and lower boundaries.
//
// Form name: Limit
// Slots:
//    lower  <Number>   Lower boundary (default: -1) (must be less than the upper)
//    upper  <Number>   Upper boundary (default:  1) (must be greater than the lower)
//
//------------------------------------------------------------------------------
class Limit : public LimitFunc {
    DECLARE_SUBCLASS(Limit,LimitFunc)

public:
   Limit();
   Limit(const LCreal lower, const LCreal upper);

   // Set slot functions
   virtual bool setSlotLowerLimit(const Basic::Number* const msg);
   virtual bool setSlotUpperLimit(const Basic::Number* const msg);
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
