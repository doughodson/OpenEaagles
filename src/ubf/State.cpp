//
// State
//

#include "openeaagles/ubf/State.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(State, "UbfState")
EMPTY_SLOTTABLE(State)
EMPTY_DELETEDATA(State)
EMPTY_COPYDATA(State)
EMPTY_SERIALIZER(State)

State::State()
{
   STANDARD_CONSTRUCTOR()
}

}
}
