
#include "openeaagles/simulation/INetIO.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(INetIO, "INetIO")

EMPTY_SLOTTABLE(INetIO)
EMPTY_COPYDATA(INetIO)
EMPTY_DELETEDATA(INetIO)
EMPTY_SERIALIZER(INetIO)

INetIO::INetIO()
{
   STANDARD_CONSTRUCTOR()
}

}
}
