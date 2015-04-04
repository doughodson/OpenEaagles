//------------------------------------------------------------------------------
// Class: Airport
// Base class: Basic::Object -> Basic::String -> Record -> Airport
//
// Description: DAFIF Airport record class
//
//
// Public member functions:
//
//    [all public members from the base classes]
//
//    Airport::AirportType airportType() const
//        Returns the airport type.
//
//    int isAirportType(Airport::AirportType type) const
//        Returns true if 'type' is equal to the airport type.
//
//    ident(char id[]) const
//        Returns the value of the airports's host country identifier in 'id'.
//        When no host country identifier exists, the airport identifier key
//        is used.  Therefore, the size of 'id' must be one more than the
//        longer of AP_IDENT_LEN and AP_KEY_LEN.
//
//    Airport::recordLength
//        Length of a DAFIF Airport record.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_Airport_H__
#define __Eaagles_Dafif_Airport_H__

#include "Record.h"

namespace Eaagles {
namespace Dafif {

class Airport : public Record {

    DECLARE_SUBCLASS(Airport,Record)

public:

   Airport();
   Airport(const char* const s);

   enum { RECORD_LENGTH = AIRPORT_RECORD_LEN  };

   enum AirportType { INVALID = -1, ANY = 'Z',
                      CIVIL  = 'A', JOINT = 'B', MILITARY = 'C', INACTIVE = 'D' };

   virtual AirportType airportType() const;
   virtual int isAirportType(const AirportType type) const;

   void ident(char id[]) const override;

   void printRecord(std::ostream& sout) const override;

private:
   static const Ptbl ptable;
};


} // End Dafif namespace
} // End Eaagles namespace

#endif
