//------------------------------------------------------------------------------
// Limit function
//------------------------------------------------------------------------------
#ifndef __LinearSystem_Limit_H_BDCF91BC_13FA_4bb1_A529_501BCC596388__
#define __LinearSystem_Limit_H_BDCF91BC_13FA_4bb1_A529_501BCC596388__

#include "openeaagles/linearSys/LimitFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit
// Base class:  Basic::Object -> ScalerFunc -> LimitFunc -> Limit
//
// Description: Limits function: limits X to the upper and lower boundries.
//
// Form name: Limit
// Slots:
//    lower  <Number>   Lower boundry (default: -1) (must be less than the upper)
//    upper  <Number>   Upper boundry (default:  1) (must be greater than the lower)
//
// GUID: {BDCF91BC-13FA-4bb1-A529-501BCC596388}
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

#endif      /* __LinearSystem_Limit_H_BDCF91BC_13FA_4bb1_A529_501BCC596388__ */
