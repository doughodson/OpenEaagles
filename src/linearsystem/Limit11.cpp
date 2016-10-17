
#include "openeaagles/linearsystem/Limit11.hpp"

namespace oe {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit11,"Limit11")
EMPTY_SLOTTABLE(Limit11)
EMPTY_SERIALIZER(Limit11)

Limit11::Limit11() : LimitFunc(1,1)
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Limit11)
EMPTY_DELETEDATA(Limit11)

}
}
