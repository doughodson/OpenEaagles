#include "openeaagles/dafif/Waypoint.h"
#include <iostream>

namespace Eaagles {
namespace Dafif {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Waypoint,"Waypoint")
EMPTY_SERIALIZER(Waypoint)


//  Waypoint class field Position Table
const Waypoint::Ptbl Waypoint::ptable = {
        WP_KEY_POS, WP_KEY_LEN,     // key, keyLength
        WP_IDENT_POS, WP_IDENT_LEN, // ident, identLength
        WP_NAME_POS, WP_NAME_LEN,   // name, nameLength
        WP_TYPE_POS, WP_TYPE_LEN,   // type, typeLength
        WP_CCODE_POS, WP_CCODE_LEN, // country, countryLength
        WP_STATE_POS, WP_STATE_LEN, // state, stateLength
        WP_WAC_POS, WP_WAC_LEN,     // wac, wacLength
        WP_LATITUDE,                // latitude
        WP_LONGITUDE,               // longitude
        WP_ELEVATION,               // elevation
        WP_MAGVAR                   // magVariance
};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Waypoint::Waypoint()
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

Waypoint::Waypoint(const char* const s) : Record(s)
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

EMPTY_COPYDATA(Waypoint)
EMPTY_DELETEDATA(Waypoint)


//------------------------------------------------------------------------------
// Printing functions
//------------------------------------------------------------------------------
void Waypoint::printRecord(std::ostream& sout) const
{
   char icas[32];
   icaoCode(icas);

   char keyCode[32];
   key(keyCode);

   char id[12];
   ident(id);

   char ccode[4];
   countryCode(ccode);

   sout << icas << ", ";

   sout << "\"";
   sout << keyCode;
   sout << "\", ";

   sout << id << "-";
   sout << ccode << ", ";

   std::streamoff old = sout.precision();
   sout.precision(12);

   sout << latitude();
   sout << ", ";
   sout << longitude();

   sout.precision(old);
}

} // End Dafif namespace
} // End Eaagles namespace
