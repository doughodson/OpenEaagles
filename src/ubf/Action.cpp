//
// Action
//
//

#include "openeaagles/ubf/Action.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_SUBCLASS(Action, "UbfAction")
EMPTY_SLOTTABLE(Action)
EMPTY_DELETEDATA(Action)
EMPTY_COPYDATA(Action)
EMPTY_SERIALIZER(Action)

Action::Action()
{
   STANDARD_CONSTRUCTOR()

   vote = 0;
}

}
}
