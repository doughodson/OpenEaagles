//------------------------------------------------------------------------------
// Limit01 function
//------------------------------------------------------------------------------
#ifndef __oe_linearSys_Limit01_H__
#define __oe_linearSys_Limit01_H__

#include "openeaagles/linearSys/LimitFunc.h"

namespace oe {
namespace linearSys {

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

} // End linearSys namespace
} // End oe namespace

#endif
