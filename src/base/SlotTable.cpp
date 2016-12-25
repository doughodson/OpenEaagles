
#include "openeaagles/base/SlotTable.hpp"
#include <cstring>

namespace oe {
namespace base {

SlotTable::SlotTable(const char* s[], const unsigned int ns, const SlotTable& base)
{
   baseTable = const_cast<SlotTable*>(&base);
   slots1 = const_cast<char**>(s);
   nslots1 = ns;
}

SlotTable::SlotTable(const char* s[], const unsigned int ns)
{
   baseTable = nullptr;
   slots1 = const_cast<char**>(s);
   nslots1 = ns;
}

SlotTable::~SlotTable()
{
   baseTable = nullptr;
   slots1 = nullptr;
   nslots1 = 0;
}

//------------------------------------------------------------------------------
// n() -- returns the number of slots
//------------------------------------------------------------------------------
unsigned int SlotTable::n() const
{
   if (baseTable != nullptr)
      return baseTable->n() + nslots1;
   else
      return nslots1;
}

//------------------------------------------------------------------------------
// name() -- returns the name of the slot at index 'slotindex'
//------------------------------------------------------------------------------
const char* SlotTable::name(const unsigned int slotindex) const
{
   // early out if it's not between 1 .. n()
   if (slotindex == 0 || slotindex > n()) return nullptr;

   const char* name = nullptr;

   // check base table first
   if (baseTable != nullptr) name = baseTable->name(slotindex);

   // if not in baseTable, check our table
   if (name == nullptr) {
      int i = static_cast<int>(slotindex);            // a) start with slotindex
      if (baseTable != nullptr) i -= baseTable->n();  // b) subt baseTable->n()
      --i;                                            // c) make it zero based
      if (i >= 0) name = slots1[i];                   // d) get the name
   }

   return name;
}

//------------------------------------------------------------------------------
// index() -- returns the index of the slot named 'slotname'
//------------------------------------------------------------------------------
unsigned int SlotTable::index(const char* const slotname) const
{
   unsigned int i = 0;

   // First, check our slot names
   {
      // search our table
      unsigned int j;
      for (j = 0; j < nslots1; j++) {
         if (std::strcmp(slotname, slots1[j]) == 0) break;
      }
      if (j < nslots1) {
         // if we're here, we found a match
         i = j;                                    // a) start with j
         i++;                                      // b) make it one based
         if (baseTable != nullptr) i += baseTable->n();  // c) add baseTable->n()
      }
   }

   // Second, check our baseTable
   if (i == 0 && baseTable != nullptr) i = baseTable->index(slotname);

   return i;
}

}
}

