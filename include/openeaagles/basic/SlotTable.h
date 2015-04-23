//------------------------------------------------------------------------------
// Class: SlotTable
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_SlotTable_H__
#define __Eaagles_Basic_SlotTable_H__

#include <iostream>

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: SlotTable
// Description: Table of slot names (see Object.h)
//
// Slot tables define the names of the slots (i.e., attributes) accepted by
// the class of objects and maps these slot names to slot index numbers.
//
// The mapping of slot names and index numbers includes all base class slot
// tables that may exist.  Therefore, the first slot of the first base class
// that contains a slot table is slot number one, and the last slot in this
// table is slot N, where N is the total number of slot in this table and all
// base class tables.  See function n() below.
//
// Slot tables are usually defined using the macros BEGIN_SLOTTABLE and
// END_SLOTTABLE (see macros.h).
//
//------------------------------------------------------------------------------
class SlotTable
{
public:
   SlotTable(const char* slotnames[], const unsigned int nslots, const SlotTable& baseTable);
   SlotTable(const char* slotnames[], const unsigned int nslots);
   virtual ~SlotTable();

   // Returns the last slot index number, which includes all base class slots.
   unsigned int n() const;

   // Returns the index, [ 1 .. n() ], for slot name 'slotname', or zero if not found
   unsigned int index(const char* const slotname) const;

   // Returns the name of the slot at index 'slotindex', range [ 1 .. n() ],
   // or zero is returned if the index is out of range.
   const char* name(const unsigned int slotindex) const;

   virtual std::ostream& serialize(std::ostream& sout, const int i = 0, const bool slotsOnly = false) const;

   virtual void copyData(const SlotTable& org);
   virtual void deleteData();

private:
   SlotTable* baseTable;   // Pointer to base class's slot table
   char** slots1;          // Array of slot names
   unsigned int nslots1;   // Number of slots in table
};

} // End Basic namespace
} // End Eaagles namespace

#endif

