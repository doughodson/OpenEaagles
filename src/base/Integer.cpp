//------------------------------------------------------------------------------
// Class: Integer
//------------------------------------------------------------------------------
#include "openeaagles/base/Integer.h"

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Integer,"int")


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
EMPTY_COPYDATA(Integer)
EMPTY_DELETEDATA(Integer)


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Integer::serialize(std::ostream& sout, const int, const bool) const
{
   sout << val;
   return sout;
}

}
}
