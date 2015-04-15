//------------------------------------------------------------------------------
// Class: Action
//------------------------------------------------------------------------------

#include "openeaagles/basic/ubf/Action.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(Action, "UbfAction")
EMPTY_SLOTTABLE(Action)
EMPTY_DELETEDATA(Action)
EMPTY_SERIALIZER(Action)

Action::Action()
{
   STANDARD_CONSTRUCTOR()
   vote = 0;
}

void Action::copyData(const Action& org, const bool cc)
{
   BaseClass::copyData(org);
   vote = org.vote;
}

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

