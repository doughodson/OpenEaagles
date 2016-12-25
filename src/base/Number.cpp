
#include "openeaagles/base/Number.hpp"
#include <iostream>

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Number, "Number")
EMPTY_SERIALIZER(Number)
EMPTY_DELETEDATA(Number)

BEGIN_SLOTTABLE(Number)
    "value",
END_SLOTTABLE(Number)

BEGIN_SLOT_MAP(Number)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()

void Number::copyData(const Number& org, const bool)
{
   BaseClass::copyData(org);
   val = org.val;
}

//------------------------------------------------------------------------------
// slot functions
//------------------------------------------------------------------------------
bool Number::setSlotValue(const Number* const svobj)
{
   bool ok = false;
   if (svobj != nullptr) {
      val = svobj->val;
      ok = true;
   }
   return ok;
}

}
}
