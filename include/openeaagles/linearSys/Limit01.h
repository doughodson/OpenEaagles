//------------------------------------------------------------------------------
// Limit01 function
//------------------------------------------------------------------------------
#ifndef __LinearSystem_Limit01_H_A226813B_7CB8_4014_92AA_6B4B6DD59609__
#define __LinearSystem_Limit01_H_A226813B_7CB8_4014_92AA_6B4B6DD59609__

#include "openeaagles/linearSys/LimitFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit01
// Base class:  Object -> ScalerFunc -> LimitFunc -> Limit01
//
// Description: Limits between 0 and 1, inclusive
//
// GUID: {A226813B-7CB8-4014-92AA-6B4B6DD59609}
//------------------------------------------------------------------------------
class Limit01 : public LimitFunc {
    DECLARE_SUBCLASS(Limit01,LimitFunc)

public:
   Limit01();
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif      /* __LinearSystem_Limit01_H_A226813B_7CB8_4014_92AA_6B4B6DD59609__ */
