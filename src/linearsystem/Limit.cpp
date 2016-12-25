
#include "openeaagles/linearsystem/Limit.hpp"
#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit, "Limit")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Limit)
    "lower",   //  1: Lower boundary (default: -1) (must be less than the upper)
    "upper",   //  2: Upper boundary (default:  1) (must be greater than the lower)
END_SLOTTABLE(Limit)

// Map slot table to handles
BEGIN_SLOT_MAP(Limit)
   ON_SLOT( 1, setSlotLowerLimit, base::Number)
   ON_SLOT( 2, setSlotUpperLimit, base::Number)
END_SLOT_MAP()

Limit::Limit()
{
   STANDARD_CONSTRUCTOR()
}

Limit::Limit(const double ll, const double uu) : LimitFunc(ll,uu)
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Limit)
EMPTY_DELETEDATA(Limit)

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Limit::setSlotLowerLimit(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setLowerLimit( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Limit::setSlotUpperLimit(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      setUpperLimit( msg->getReal() );
      ok = true;
   }
   return ok;
}

std::ostream& Limit::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "lower:   " << getLowerLimit() << std::endl;

    indent(sout,i+j);
    sout << "upper:   " << getUpperLimit() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
