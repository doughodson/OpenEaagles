#include "openeaagles/basic/SlotTable.h"


namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
SlotTable::SlotTable(const char* s[], const unsigned int ns, const SlotTable& base)
{
   baseTable = (SlotTable*) &base;
   slots1 = (char**) s;
   nslots1 = ns;
}

SlotTable::SlotTable(const char* s[], const unsigned int ns)
{
   baseTable = (SlotTable*) 0;
   slots1 = (char**) s;
   nslots1 = ns;
}

void SlotTable::copyData(const SlotTable& org)
{
   baseTable = org.baseTable;
   slots1 = org.slots1;
   nslots1 = org.nslots1;
}

void SlotTable::deleteData()
{
   baseTable = 0;
   slots1 = 0;
   nslots1 = 0;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
SlotTable::~SlotTable()
{
   baseTable = 0;
   slots1 = 0;
   nslots1 = 0;
}

//------------------------------------------------------------------------------
// n() -- returns the number of slots
//------------------------------------------------------------------------------
unsigned int SlotTable::n() const
{
   if (baseTable != 0)
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
   if (slotindex == 0 || slotindex > n()) return 0;

   const char* name = 0;

   // check base table first
   if (baseTable != 0) name = baseTable->name(slotindex);

   // if not in baseTable, check our table
   if (name == 0) {
      int i = (int) slotindex;                  // a) start with slotindex
      if (baseTable != 0) i -= baseTable->n();  // b) subt baseTable->n()
      --i;                                      // c) make it zero based
      if (i >= 0) name = slots1[i];             // d) get the name
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
         if (strcmp(slotname,slots1[j]) == 0) break;
      }
      if (j < nslots1) {
         // if we're here, we found a match
         i = j;                                    // a) start with j
         i++;                                      // b) make it one based
         if (baseTable != 0) i += baseTable->n();	// c) add baseTable->n()
      }
   }

   // Second, check our baseTable
   if (i == 0 && baseTable != 0) i = baseTable->index(slotname);

   return i;
}


//------------------------------------------------------------------------------
// serialize() 
//------------------------------------------------------------------------------
std::ostream& SlotTable::serialize(std::ostream& sout, const int, const bool) const
{
   unsigned int n = 0;
   if (baseTable != 0) {
      baseTable->serialize(sout);
      n = baseTable->n();
   }

   for (unsigned int i = 0; i < nslots1; i++) {
      sout << (i+n) << "\t" << slots1[i] << std::endl;
   }
   return sout;
}

} // End Basic namespace
} // End Eaagles namespace
