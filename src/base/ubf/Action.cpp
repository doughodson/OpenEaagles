
#include "openeaagles/base/ubf/Action.hpp"
#include <iostream>

namespace oe {
namespace base {
namespace ubf {

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

}
}
}

