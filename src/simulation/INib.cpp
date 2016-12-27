
#include "openeaagles/simulation/INib.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(INib, "INib")

EMPTY_SLOTTABLE(INib)
EMPTY_COPYDATA(INib)
EMPTY_DELETEDATA(INib)
EMPTY_SERIALIZER(INib)

INib::INib()
{
   STANDARD_CONSTRUCTOR()
}

}
}
