
#include "openeaagles/simulation/AbstractNetIO.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractNetIO, "AbstractNetIO")

EMPTY_SLOTTABLE(AbstractNetIO)
EMPTY_COPYDATA(AbstractNetIO)
EMPTY_DELETEDATA(AbstractNetIO)
EMPTY_SERIALIZER(AbstractNetIO)

AbstractNetIO::AbstractNetIO()
{
   STANDARD_CONSTRUCTOR()
}

}
}
