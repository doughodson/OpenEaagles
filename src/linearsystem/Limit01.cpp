
#include "openeaagles/linearsystem/Limit01.hpp"
#include <iostream>

namespace oe {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit01, "Limit01")
EMPTY_SLOTTABLE(Limit01)
EMPTY_SERIALIZER(Limit01)

Limit01::Limit01() : LimitFunc(0,1)
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Limit01)
EMPTY_DELETEDATA(Limit01)

}
}
