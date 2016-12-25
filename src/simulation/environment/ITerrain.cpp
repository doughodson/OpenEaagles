
#include "openeaagles/simulation/environment/ITerrain.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(ITerrain, "ITerrain")

EMPTY_SLOTTABLE(ITerrain)
EMPTY_COPYDATA(ITerrain)
EMPTY_DELETEDATA(ITerrain)
EMPTY_SERIALIZER(ITerrain)

ITerrain::ITerrain()
{
   STANDARD_CONSTRUCTOR()
}

}
}
