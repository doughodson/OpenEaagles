
#include "openeaagles/linearSys/Limit11.h"

namespace Eaagles {
namespace LinearSystem {

//==============================================================================
// Class Limit11
//==============================================================================
IMPLEMENT_SUBCLASS(Limit11,"Limit11")
EMPTY_SLOTTABLE(Limit11)
EMPTY_SERIALIZER(Limit11)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Limit11::Limit11() : LimitFunc(1,1)
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Limit11)
EMPTY_DELETEDATA(Limit11)

} // End LinearSystem namespace
} // End Eaagles namespace
