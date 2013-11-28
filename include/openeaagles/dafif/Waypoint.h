//------------------------------------------------------------------------------
// Class: Waypoint
// Base class: Basic::Object -> Basic::String -> Record -> Waypoint
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
#ifndef __Eaagles_Dafif_Waypoint_H__
#define __Eaagles_Dafif_Waypoint_H__

#include "Record.h"

namespace Eaagles {
namespace Dafif {

class Waypoint : public Record
{
    DECLARE_SUBCLASS(Waypoint,Record)

public:
   static const int RECORD_LENGTH = WAYPOINT_RECORD_LEN;

public:
   Waypoint();
   Waypoint(const char* const s);

   // Record Interface
   virtual void printRecord(std::ostream& sout) const;

private:
   static const Ptbl ptable;

};

} // End Dafif namespace
} // End Eaagles namespace

#endif
