#include "openeaagles/dafif/Ils.h"
#include "openeaagles/base/Nav.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include <iostream>

namespace oe {
namespace dafif {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Ils,"Ils")
EMPTY_SERIALIZER(Ils)

// Ils class field Position Table
const Ils::Ptbl Ils::ptable = {
        ILS_KEY_POS, ILS_KEY_LEN,       // key, keyLength
        ILS_IDENT_POS, ILS_IDENT_LEN,   // ident, identLength
        ILS_NAME_POS, ILS_NAME_LEN,     // name, nameLength
        ILS_TYPE_POS, ILS_TYPE_LEN,     // type, typeLength
        ILS_CCODE_POS, ILS_CCODE_LEN,   // country, countryLength
        ILS_STATE_POS, ILS_STATE_LEN,   // state, stateLength
        ILS_WAC_POS, ILS_WAC_LEN,       // wac, wacLength
        ILS_LATITUDE,                   // latitude
        ILS_LONGITUDE,                  // longitude
        ILS_ELEVATION,                  // elevation
        ILS_MAGVAR                      // magVariance
};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Ils::Ils()
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

Ils::Ils(const char* const s) : Record(s)
{
   STANDARD_CONSTRUCTOR()
   ptbl = &ptable;
}

EMPTY_COPYDATA(Ils)
EMPTY_DELETEDATA(Ils)

//------------------------------------------------------------------------------
// Type functions
//------------------------------------------------------------------------------
Ils::IlsType Ils::ilsType() const
{
   char tt[ILS_TYPE_LEN+1];
   dafifType(tt);
   return IlsType(tt[0]);
}

int Ils::isIlsType(const Ils::IlsType tt) const
{
   return tt == ilsType();
}


//------------------------------------------------------------------------------
// Get ILS glide slope and aircraft glide slope
//------------------------------------------------------------------------------
void Ils::getGlideSlopeData(const double aclat, const double aclon, const double acelev, float* ilsGlideSlope, float* acGlideSlope, float* deltaGlideSlope)const
{
   double bearing(0.0), range(0.0), grdrange(0.0), lookangle(0.0);
   base::Nav::glla2bd(aclat, aclon, acelev, latitude(), longitude(), elevation(), &bearing, &grdrange, &range, &lookangle);
   *ilsGlideSlope = glideSlopeAngle();
   *acGlideSlope = static_cast<float>(-lookangle);
   *deltaGlideSlope = *ilsGlideSlope - *acGlideSlope;
}


//------------------------------------------------------------------------------
// Printing functions
//------------------------------------------------------------------------------
void Ils::printRecord(std::ostream& sout) const
{
   char keyValue[ILS_KEY_LEN+1];
   key(keyValue);

   sout << keyValue;
   sout << ": ";

   char id[ILS_IDENT_LEN+1];
   ident(id);
   sout << id;
   sout << ": ";

   char typeValue[ILS_TYPE_LEN+1];
   dafifType(typeValue);
   sout << typeValue;
   sout << "-";
   typeValue[0] = getChar(ILS_CLTYPE_POS);
   sout << typeValue;
   sout << ": ";

   sout << "( ";

   std::streamoff old = sout.precision();
   sout.precision(12);

   sout << latitude();
   sout << ", ";
   sout << longitude();

   sout.precision(old);

   sout << ", ";
   sout << elevation();
   sout << ", ";
   sout << magVariance();
   sout << ") ";

   sout << frequency();
   sout << " ";
   sout << channel();
   sout << " ";
   sout << glideSlopeAngle();
}

//------------------------------------------------------------------------------
//  print Glide Slope Data for current ILS
//------------------------------------------------------------------------------
void Ils::printGlideSlopeData(std::ostream& sout, const double aclat, const double aclon, const double acelev)const
{
   float ilsGlideSlope(0.0), acGlideSlope(0.0), deltaGlideSlope(0.0);
   getGlideSlopeData(aclat, aclon, acelev, &ilsGlideSlope, &acGlideSlope, &deltaGlideSlope);
   sout << "  ILS glide slope " << ilsGlideSlope << "  aircraft_glide_slope " << acGlideSlope << " delta_angle " << deltaGlideSlope;
}

} // End dafif namespace
} // End oe namespace
