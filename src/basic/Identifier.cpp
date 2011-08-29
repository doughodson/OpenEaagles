#include "openeaagles/basic/Identifier.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Identifier,"Identifier")
EMPTY_SLOTTABLE(Identifier)

EMPTY_COPYDATA(Identifier)
EMPTY_DELETEDATA(Identifier)

//------------------------------------------------------------------------------
// output stream operator
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& sout, const Identifier& ident)
{
    const char* s = ident;
    sout << s;
    return sout;
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Identifier::serialize(std::ostream& sout, const int, const bool) const
{
    const char* s = *this;
    sout << s;
    return sout;
}

} // End Basic namespace
} // End Eaagles namespace
