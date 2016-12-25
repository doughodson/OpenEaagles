
#include "openeaagles/base/Integer.hpp"

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Integer, "int")
EMPTY_COPYDATA(Integer)
EMPTY_DELETEDATA(Integer)

std::ostream& Integer::serialize(std::ostream& sout, const int, const bool) const
{
   sout << val;
   return sout;
}

}
}
