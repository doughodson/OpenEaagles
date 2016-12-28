
#include "openeaagles/terrain/ITerrain.hpp"

namespace oe {
namespace terrain {

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
