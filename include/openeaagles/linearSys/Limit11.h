//------------------------------------------------------------------------------
// Limit11 function
//------------------------------------------------------------------------------
#ifndef __oe_LinearSystem_Limit11_H__
#define __oe_LinearSystem_Limit11_H__

#include "openeaagles/linearSys/LimitFunc.h"

namespace oe {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit11
// Base class:  Object -> ScalerFunc -> LimitFunc -> Limit11
//
// Description: Limits between -1 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit11 : public LimitFunc {
    DECLARE_SUBCLASS(Limit11,LimitFunc)

public:
   Limit11();
};

} // End LinearSystem namespace
} // End oe namespace

#endif
