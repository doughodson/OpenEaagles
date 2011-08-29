
#include "openeaagles/linearSys/Limit01.h"

namespace Eaagles {
namespace LinearSystem {

//==============================================================================
// Class Limit01
//==============================================================================
IMPLEMENT_SUBCLASS(Limit01,"Limit01")
EMPTY_SLOTTABLE(Limit01)
EMPTY_SERIALIZER(Limit01)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Limit01::Limit01() : LimitFunc(0,1)
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Limit01)
EMPTY_DELETEDATA(Limit01)

} // End LinearSystem namespace
} // End Eaagles namespace
