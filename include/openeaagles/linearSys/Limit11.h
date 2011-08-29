//------------------------------------------------------------------------------
// Limit11 function
//------------------------------------------------------------------------------
#ifndef __LinearSystem_Limit11_H_EDDAC812_2D80_4baf_971E_229F1272592B__
#define __LinearSystem_Limit11_H_EDDAC812_2D80_4baf_971E_229F1272592B__

#include "openeaagles/linearSys/LimitFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit11
// Base class:  Object -> ScalerFunc -> LimitFunc -> Limit11
//
// Description: Limits between -1 and 1, enclusive
//
// GUID: {EDDAC812-2D80-4baf-971E-229F1272592B}
//------------------------------------------------------------------------------
class Limit11 : public LimitFunc {
    DECLARE_SUBCLASS(Limit11,LimitFunc)

public:
   Limit11();
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif      /* __LinearSystem_Limit11_H_EDDAC812_2D80_4baf_971E_229F1272592B__ */
