//------------------------------------------------------------------------------
// Class: Waypoint
// Base class: basic::Object -> basic::String -> Record -> Waypoint
//
// Description: Access to the DAFIF Waypoint records.
//
//
// Public member functions:
//
//  [all public members from the base classes]
//
//  Waypoint::recordLength
//    Length of a DAFIF Waypoint record.
//
//------------------------------------------------------------------------------
#ifndef __oe_Dafif_Waypoint_H__
#define __oe_Dafif_Waypoint_H__

#include "Record.h"

namespace oe {
namespace Dafif {

class Waypoint : public Record
{
    DECLARE_SUBCLASS(Waypoint,Record)

public:
   static const int RECORD_LENGTH = WAYPOINT_RECORD_LEN;

public:
   Waypoint();
   Waypoint(const char* const s);

   void printRecord(std::ostream& sout) const override;

private:
   static const Ptbl ptable;

};

} // End Dafif namespace
} // End oe namespace

#endif
