
#include "openeaagles/simulation/IOtw.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IOtw, "IOtw")

EMPTY_SLOTTABLE(IOtw)
EMPTY_COPYDATA(IOtw)
EMPTY_DELETEDATA(IOtw)
EMPTY_SERIALIZER(IOtw)

IOtw::IOtw()
{
   STANDARD_CONSTRUCTOR()
}

}
}
