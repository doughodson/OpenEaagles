//------------------------------------------------------------------------------
// Limit function
//------------------------------------------------------------------------------
#ifndef __oe_linearSys_Limit_H__
#define __oe_linearSys_Limit_H__

#include "openeaagles/linearSys/LimitFunc.h"

namespace oe {
namespace linearSys {

//------------------------------------------------------------------------------
// Class: Limit
// Base class:  basic::Object -> ScalerFunc -> LimitFunc -> Limit
//
// Description: Limits function: limits X to the upper and lower boundaries.
//
// Factory name: Limit
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
   virtual bool setSlotLowerLimit(const basic::Number* const msg);
   virtual bool setSlotUpperLimit(const basic::Number* const msg);
};

} // End linearSys namespace
} // End oe namespace

#endif
