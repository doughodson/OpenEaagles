#include "openeaagles/dafif/Runway.h"
#include "openeaagles/base/Nav.h"
#include <iostream>

namespace oe {
namespace dafif {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Runway,"Runway")
EMPTY_SERIALIZER(Runway)


// Runway class field Position Table
const Runway::Ptbl Runway::ptable = {
    RW_KEY_POS, RW_KEY_LEN,     // key, keyLength
    RW_IDENT_POS, RW_IDENT_LEN, // ident, identLength
    RW_NAME_POS, RW_NAME_LEN,   // name, nameLength
    RW_TYPE_POS, RW_TYPE_LEN,   // type, typeLength
    RW_CCODE_POS, RW_CCODE_LEN, // country, countryLength
    RW_STATE_POS, RW_STATE_LEN, // state, stateLength
    RW_WAC_POS, RW_WAC_LEN,     // wac, wacLength
    RW_LATITUDE,                // latitude
    RW_LONGITUDE,               // longitude
    RW_ELEVATION,               // elevation
    RW_MAGVAR                   // magVariance
};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Runway::Runway()
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

Runway::Runway(const char* const s) : Record(s)
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

EMPTY_COPYDATA(Runway)
EMPTY_DELETEDATA(Runway)


//------------------------------------------------------------------------------
// Get ILS glide slope and aircraft glide slope
//------------------------------------------------------------------------------
void Runway::getRunwayMagHeading(const double aclat, const double aclon, const double acelev, float* magHeading1, float* magHeading2, double* trueBearing1, double* trueBearing2)const
{
   double range(0.0), grdrange(0.0);
   base::Nav::glla2bd(aclat, aclon, acelev, latitude(LOW_END), longitude(LOW_END), elevation(LOW_END), trueBearing1, &range, &grdrange);
   base::Nav::glla2bd(aclat, aclon, acelev, latitude(HIGH_END), longitude(HIGH_END), elevation(HIGH_END), trueBearing2, &range, &grdrange);
   *magHeading1 = magHeading(LOW_END);
   *magHeading2 = magHeading(HIGH_END);
}


//------------------------------------------------------------------------------
// Printing functions
//------------------------------------------------------------------------------
void Runway::printRecord(std::ostream& sout) const
{
   char key1[16];
   char id[12];

   key(key1);

   sout << key1;
   sout << ":";

   sout << " (";
   ident(id,HIGH_END);
   sout << id;
   sout << ": ";

   std::streamoff old = sout.precision();
   sout.precision(12);

   sout << latitude(HIGH_END);
   sout << ", ";
   sout << longitude(HIGH_END);

   sout.precision(old);

   sout << ", ";
   sout << elevation(HIGH_END);
   sout << ", ";
   sout << magHeading(HIGH_END);
   sout << ")";

   sout << " (";
   ident(id,LOW_END);
   sout << id;
   sout << ": ";

   old = sout.precision();
   sout.precision(12);

   sout << latitude(LOW_END);
   sout << ", ";
   sout << longitude(LOW_END);

   sout.precision(old);

   sout << ", ";
   sout << elevation(LOW_END);
   sout << ", ";
   sout << magHeading(LOW_END);
   sout << ")";

   sout << "  ";
   sout << length();
   sout << "X";
   sout << width();
}

//------------------------------------------------------------------------------
//  print Runway Magnetic Heading
//------------------------------------------------------------------------------
void Runway::printRunwayMagHeading(std::ostream& sout, const double aclat, const double aclon, const double acelev)const
{
   float magHeading1(0.0), magHeading2(0.0);
   double trueBearing1(0.0), trueBearing2(0.0);
   getRunwayMagHeading(aclat, aclon, acelev, &magHeading1, &magHeading2, &trueBearing1, &trueBearing2);
   sout << " Runway_Low_End_Mag_heading " << magHeading1 << " aircraft_bearing_Low_End " << trueBearing1 << "  Runway_High_End_Mag_heading " <<  magHeading2 << " aircraft_bearing_High_End " << trueBearing2;
}

}
}
