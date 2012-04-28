//
// Behavior
//

#include "openeaagles/ubf/Behavior.h"
#include "openeaagles/ubf/State.h"
#include "openeaagles/ubf/Action.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(Behavior, "UbfBehavior")
EMPTY_SLOTTABLE(Behavior)
EMPTY_DELETEDATA(Behavior)
EMPTY_COPYDATA(Behavior)
EMPTY_SERIALIZER(Behavior)

Behavior::Behavior()
{
   STANDARD_CONSTRUCTOR()
}

}
}

