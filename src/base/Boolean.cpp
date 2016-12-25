
#include "openeaagles/base/Boolean.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Boolean, "boolean")
EMPTY_SLOTTABLE(Boolean)

EMPTY_COPYDATA(Boolean)
EMPTY_DELETEDATA(Boolean)

std::ostream& Boolean::serialize(std::ostream& sout, const int, const bool) const
{
   sout << val;
   return sout;
}

}
}
