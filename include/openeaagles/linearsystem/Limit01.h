//------------------------------------------------------------------------------
// Limit01 function
//------------------------------------------------------------------------------
#ifndef __oe_linearsystem_Limit01_H__
#define __oe_linearsystem_Limit01_H__

#include "openeaagles/linearsystem/LimitFunc.h"

namespace oe {
namespace linearsystem {

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

} // End linearsystem namespace
}

#endif
