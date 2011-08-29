//------------------------------------------------------------------------------
// Class:	Waypoint
// Base class:	Basic::Object -> Basic::String -> DafifRecord -> Waypoint
//
// Description: Access to the DAFIF Waypoint records.
//
//
// Public member functions:
//
//	[all public members from the base classes]
//
//	Waypoint::recordLength
//		Length of a DAFIF Waypoint record.
//
//------------------------------------------------------------------------------
#ifndef __Waypoint_H_89108B7B_89A8_49cd_BF81_154C92427337__
#define __Waypoint_H_89108B7B_89A8_49cd_BF81_154C92427337__

#include "Dafif.h"

namespace Eaagles {
namespace Dafif {

class Waypoint : public DafifRecord {
    DECLARE_SUBCLASS(Waypoint,DafifRecord)

public:
   static const int RECORD_LENGTH = WAYPOINT_RECORD_LEN;

public:
   Waypoint();
   Waypoint(const char* const s);

   // DafifRecord Interface
   virtual void printRecord(std::ostream& sout) const;

private:
   static const Ptbl ptable;

};

} // End Dafif namespace
} // End Eaagles namespace

#endif
