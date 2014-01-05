//------------------------------------------------------------------------------
// LatLon
//------------------------------------------------------------------------------
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(LatLon,"LatLon")

// DPG_NOTE -- This needs to be handled in the lexical section of the parser

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(LatLon)
    "direction",    // { "n", "s", "e", "w" }
    "degrees",
    "minutes",
    "seconds",
END_SLOTTABLE(LatLon)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(LatLon)
    ON_SLOT(1,setDirection,String)
    ON_SLOT(2,setDegrees,Number)
    ON_SLOT(3,setMinutes,Number)
    ON_SLOT(4,setSeconds,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
LatLon::LatLon()
{
    STANDARD_CONSTRUCTOR()
    dir = NONE;
    deg = 0;
    min = 0;
    sec = 0.0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void LatLon::copyData(const LatLon& org, const bool)
{
   BaseClass::copyData(org);
   dir = org.dir;
   deg = org.deg;
   min = org.min;
   sec = org.sec;
}

EMPTY_DELETEDATA(LatLon)


//------------------------------------------------------------------------------
// computeVal() --
//     Converts direction, degrees, minutes, and seconds to a lat/long value.
//------------------------------------------------------------------------------
void LatLon::computeVal()
{
    val = deg + (double(min) + double(sec)/60.0) / 60.0;
    if (dir == SOUTH || dir == WEST) val = -val;
}


//------------------------------------------------------------------------------
// Support Functions
//------------------------------------------------------------------------------

// Sets direction
bool LatLon::setDir(const char* const d)
{
    dir = NONE;
    if (d != 0) {
        if (d[0] == 'N' || d[0] == 'n')      dir = NORTH;
        else if (d[0] == 'S' || d[0] == 's') dir = SOUTH;
        else if (d[0] == 'E' || d[0] == 'e') dir = EAST;
        else if (d[0] == 'W' || d[0] == 'w') dir = WEST;
    }

    if (dir == NONE) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDir: invalid lat/long direction: " << d << std::endl;
        }
    }
    return (dir != NONE);
}

// Sets degrees, minutes, seconds -- then calls computeVal().
bool LatLon::setDeg(const double d)
{
   bool ok = true;

   double dd = fabs(d);
   if (dir == NORTH || dir == SOUTH) {
      // check for valid degree numbers
      if (dd > 90.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDeg: invalid latitude: " << d << std::endl;
         }
         ok = false;
      }
   }
   else if (dir == EAST || dir == WEST) {
      // check for valid degree numbers
      if (dd > 180.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDeg: invalid longitude: " << d << std::endl;
         }
         ok = false;
      }
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setDeg: invalid degrees value: " << d << std::endl;
      }
      ok = false;
   }

   if (ok) {
      deg = int(dd);
      double m = (dd - double(deg)) * 60.0;
      min = int(m);
      sec = double(m - double(min)) * 60.0;
      computeVal();
   }
   return ok;
}

// Sets minutes, seconds -- then calls computeVal().
bool LatLon::setMin(const double m) //
{
   bool ok = true;

   double mm = fabs(m);
   // check for valid minute numbers
   if (mm >= 0.0 && mm < 60.0) {
      min = int(mm);
      sec = (mm - double(min)) * 60.0;
      computeVal();
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setMin: invalid minutes value: " << m << std::endl;
      }
      ok = false;
   }

   return ok;
}

// Sets seconds -- then calls computeVal().
bool LatLon::setSec(const double s)
{
   bool ok = true;

   double ss = fabs(s);
   // check for valid second numbers
   if (ss >= 0.0 && ss < 60.0) {
      sec = ss;
      computeVal();
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setMin: invalid seconds value: " << s << std::endl;
      }
      ok = false;
   }

   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for LatLon
//------------------------------------------------------------------------------
Object* LatLon::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& LatLon::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    //int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName()  << std::endl;
        //j = 4;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// setDirection() -- sets the initial direction "n", "s", "e", "w"
//------------------------------------------------------------------------------
bool LatLon::setDirection(const String* const sdobj)
{
   bool ok = false;
   if (sdobj != 0) ok = setDir(*sdobj);
   return ok;
}

//------------------------------------------------------------------------------
//  setDegrees() -- sets the initial degrees
//------------------------------------------------------------------------------
bool LatLon::setDegrees(const Number* const sdeobj)
{
   bool ok = false;
   if (sdeobj != 0) ok = setDeg(sdeobj->getReal());
   return ok;
}

//------------------------------------------------------------------------------
// setMinutes() -- sets the minutes
//------------------------------------------------------------------------------
bool LatLon::setMinutes(const Number* const smobj)
{
   bool ok = false;
   if (smobj != 0) ok = setMin(smobj->getReal());
   return ok;
}

//------------------------------------------------------------------------------
// setSeconds() --
//------------------------------------------------------------------------------
bool LatLon::setSeconds(const Number* const ssobj)
{
   bool ok = false;
   if (ssobj != 0) ok = setSec(ssobj->getReal());
   return ok;
}

} // End Basic namespace
} // End Eaagles namespace
