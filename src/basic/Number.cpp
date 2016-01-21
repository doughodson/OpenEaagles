//------------------------------------------------------------------------------
// Number
//------------------------------------------------------------------------------
#include "openeaagles/basic/Number.h"

namespace oe {
namespace Basic {

IMPLEMENT_SUBCLASS(Number,"Number")
EMPTY_SERIALIZER(Number)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Number)
    "value",
END_SLOTTABLE(Number)


//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Number)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Number::copyData(const Number& org, const bool)
{
   BaseClass::copyData(org);
   val = org.val;
}

EMPTY_DELETEDATA(Number)


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* Number::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
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

} // End Basic namespace
} // End oe namespace
