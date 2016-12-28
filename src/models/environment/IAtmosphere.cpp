
#include "openeaagles/models/environment/IAtmosphere.hpp"

namespace oe {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IAtmosphere, "IAtmosphere")

EMPTY_SLOTTABLE(IAtmosphere)
EMPTY_COPYDATA(IAtmosphere)
EMPTY_DELETEDATA(IAtmosphere)
EMPTY_SERIALIZER(IAtmosphere)

IAtmosphere::IAtmosphere()
{
   STANDARD_CONSTRUCTOR()
}

}
}
