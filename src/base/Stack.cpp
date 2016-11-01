
#include "openeaagles/base/Stack.hpp"
#include <stdlib.h>

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Stack,"Stack")

EMPTY_COPYDATA(Stack)
EMPTY_DELETEDATA(Stack)

std::ostream& Stack::serialize(std::ostream& sout, const int, const bool) const
{
    return sout;
}

}
}
