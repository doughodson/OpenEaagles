
#include "openeaagles/basic/functors/Function.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/functors/Tables.h"

namespace oe {
namespace basic {

//==============================================================================
// class FStorage
//==============================================================================
IMPLEMENT_SUBCLASS(FStorage,"FStorage")
EMPTY_SLOTTABLE(FStorage)
EMPTY_CONSTRUCTOR(FStorage)
EMPTY_COPYDATA(FStorage)
EMPTY_DELETEDATA(FStorage)
EMPTY_SERIALIZER(FStorage)

//==============================================================================
// Class: Function
//==============================================================================
IMPLEMENT_SUBCLASS(Function, "Function")

BEGIN_SLOTTABLE(Function)
    "table",          // Optional LFI table
END_SLOTTABLE(Function)

BEGIN_SLOT_MAP(Function)
    ON_SLOT( 1, setSlotLfiTable, Table)
END_SLOT_MAP()

Function::Function()
{
   STANDARD_CONSTRUCTOR()

   table = nullptr;
}

void Function::copyData(const Function& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) table = nullptr;

   {
      const Table* copy = nullptr;
      if (org.table != nullptr) {
         copy = static_cast<const Table*>(org.table->clone());
      }
      setSlotLfiTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void Function::deleteData()
{
   setSlotLfiTable(nullptr);
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
FStorage* Function::storageFactory() const
{
   // Since no derived class handled this ...
   if (table != nullptr) {
      // then we're going to let the table handle it, if any
      return table->storageFactory();
   }
   else {
      // or we're just going to return a standard FStorage object.
      return new FStorage();
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Function::setSlotLfiTable(const Table* const msg)
{
   table = msg;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* Function::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Function::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if (!slotsOnly) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if (table != nullptr) {
      indent(sout, i + j);
      sout << "table: ";
      table->serialize(sout,(i+j+4));
   }

   BaseClass::serialize(sout, i + j, true);

   if (!slotsOnly) {
      indent(sout, i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // End basic namespace
} // End oe namespace
