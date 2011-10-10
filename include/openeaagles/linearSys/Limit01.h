//------------------------------------------------------------------------------
// Limit01 function
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_Limit01_H__
#define __Eaagles_LinearSystem_Limit01_H__

#include "openeaagles/linearSys/LimitFunc.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class: Limit01
// Description: Limits between 0 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit01 : public LimitFunc {
    DECLARE_SUBCLASS(Limit01,LimitFunc)

public:
   Limit01();
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
