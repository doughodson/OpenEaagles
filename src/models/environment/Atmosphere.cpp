
#include "openeaagles/models/environment/Atmosphere.hpp"

namespace oe {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(Atmosphere, "Atmosphere")

EMPTY_SLOTTABLE(Atmosphere)
EMPTY_COPYDATA(Atmosphere)
EMPTY_DELETEDATA(Atmosphere)
EMPTY_SERIALIZER(Atmosphere)

Atmosphere::Atmosphere()
{
   STANDARD_CONSTRUCTOR()
}

}
}
