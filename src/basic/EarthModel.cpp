
#include "openeaagles/basic/EarthModel.h"

#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/units/Distances.h"

#include <cstring>

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Pre-defined earth models
//------------------------------------------------------------------------------
const EarthModel EarthModel::wgs84              ( 6378137.0,    1.0 / 298.257223563 );
const EarthModel EarthModel::airy               ( 6377563.396,  1.0 / 299.3249646 );
const EarthModel EarthModel::australianNational ( 6378160.0,    1.0 / 298.25 );
const EarthModel EarthModel::bessel1841         ( 6377397.155,  1.0 / 299.1528128 );
const EarthModel EarthModel::clark1866          ( 678206.4,     1.0 / 294.9786982 );
const EarthModel EarthModel::clark1880          ( 6378249.145,  1.0 / 293.465 );
const EarthModel EarthModel::everest            ( 6377276.345,  1.0 / 300.8017 );
const EarthModel EarthModel::fischer1960        ( 6378166.0,    1.0 / 298.3 );
const EarthModel EarthModel::fischer1968        ( 6378150.0,    1.0 / 298.3 );
const EarthModel EarthModel::grs1967            ( 6378160.0,    1.0 / 298.247167427 );
const EarthModel EarthModel::grs1980            ( 6378137.0,    1.0 / 298.257222101 );
const EarthModel EarthModel::helmert1906        ( 6378200.0,    1.0 / 298.3 );
const EarthModel EarthModel::hough              ( 6378270.0,    1.0 / 297.0 );
const EarthModel EarthModel::international      ( 6378388.0,    1.0 / 297.0 );
const EarthModel EarthModel::kravosky           ( 6378245.0,    1.0 / 298.3 );
const EarthModel EarthModel::modAiry            ( 6377340.189,  1.0 / 299.3249646 );
const EarthModel EarthModel::modEverest         ( 6377304.063,  1.0 / 300.8017 );
const EarthModel EarthModel::modFischer         ( 6378155.0,    1.0 / 298.3 );
const EarthModel EarthModel::southAmerican1969  ( 6378160.0,    1.0 / 298.25 );
const EarthModel EarthModel::wgs60              ( 6378165.0,    1.0 / 298.3 );
const EarthModel EarthModel::wgs66              ( 6378145.0,    1.0 / 298.25);
const EarthModel EarthModel::wgs72              ( 6378135.0,    1.0 / 298.26);

//------------------------------------------------------------------------------
// EarthModel class
//------------------------------------------------------------------------------

IMPLEMENT_SUBCLASS(EarthModel,"EarthModel")
EMPTY_DELETEDATA(EarthModel)

BEGIN_SLOTTABLE(EarthModel)
    "a",    // 1: Semi major axis
    "b",    // 2: Semi minor axis
    "f",    // 3: Flattening
END_SLOTTABLE(EarthModel)

BEGIN_SLOT_MAP(EarthModel)
   ON_SLOT(1, setSlotA, Distance) /* always check units before numbers */
   ON_SLOT(1, setSlotA, Number)

   ON_SLOT(2, setSlotB, Distance) /* always check units before numbers */
   ON_SLOT(2, setSlotB, Number)

   ON_SLOT(3, setSlotF, Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
EarthModel::EarthModel(const double a0, const double f0)
{
   STANDARD_CONSTRUCTOR()

   initData(a0,f0);
}

EarthModel::EarthModel()
{
   STANDARD_CONSTRUCTOR()

   initData(Nav::WGS84_A, Nav::WGS84_F);
}

void EarthModel::initData(const double a0, const double f0)
{
   a = a0;
   f = f0;
   b  = a*(1.0 - f);
   e2 = f*(2.0 - f);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void EarthModel::copyData(const EarthModel& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData(Nav::WGS84_A, Nav::WGS84_F);

   a = org.a;
   b = org.b;
   f = org.f;
   e2 = org.e2;
}

//------------------------------------------------------------------------------
// Get a standard earth model by case independent name (see list above)
//------------------------------------------------------------------------------
const EarthModel* EarthModel::getEarthModel(const char* const name)
{
   const EarthModel* em = 0;
   if (name != nullptr) {

      if      (std::strcmp( name, "airy") == 0) em = &airy;
      else if (std::strcmp( name, "australianNational") == 0) em = &australianNational;
      else if (std::strcmp( name, "bessel1841") == 0) em = &bessel1841;
      else if (std::strcmp( name, "clark1866") == 0) em = &clark1866;
      else if (std::strcmp( name, "clark1880") == 0) em = &clark1880;
      else if (std::strcmp( name, "everest") == 0) em = &everest;
      else if (std::strcmp( name, "fischer1960") == 0) em = &fischer1960;
      else if (std::strcmp( name, "fischer1968") == 0) em = &fischer1968;
      else if (std::strcmp( name, "grs1967") == 0) em = &grs1967;
      else if (std::strcmp( name, "grs1980") == 0) em = &grs1980;
      else if (std::strcmp( name, "helmert1906") == 0) em = &helmert1906;
      else if (std::strcmp( name, "hough") == 0) em = &hough;
      else if (std::strcmp( name, "international") == 0) em = &international;
      else if (std::strcmp( name, "kravosky") == 0) em = &kravosky;
      else if (std::strcmp( name, "modairy") == 0) em = &modAiry;
      else if (std::strcmp( name, "modeverest") == 0) em = &modEverest;
      else if (std::strcmp( name, "modfischer") == 0) em = &modFischer;
      else if (std::strcmp( name, "southAmerican1969") == 0) em = &southAmerican1969;
      else if (std::strcmp( name, "wgs60") == 0) em = &wgs60;
      else if (std::strcmp( name, "wgs66") == 0) em = &wgs66;
      else if (std::strcmp( name, "wgs72") == 0) em = &wgs72;
      else if (std::strcmp( name, "wgs84") == 0) em = &wgs84;

   }
   return em;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool EarthModel::setA(const double a0)
{
   bool ok = false;
   if (a0 > 0) {
      initData(a0,f);
      ok = true;
   }
   return ok;
}

bool EarthModel::setB(const double b0)
{
   bool ok = false;
   if (b0 > 0) {
      b = b0;
      f  = (a-b)/a;
      e2 = f*(2.0 - f);
      ok = true;
   }
   return ok;
}

bool EarthModel::setF(const double f0)
{
   initData(a,f0);
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool EarthModel::setSlotA(const Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setA( Meters::convertStatic( *msg ) );
   }
   return ok;
}

bool EarthModel::setSlotA(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setA( msg->getDouble() );
   }
   return ok;
}

bool EarthModel::setSlotB(const Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setB( Meters::convertStatic( *msg ) );
   }
   return ok;
}

bool EarthModel::setSlotB(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setB( msg->getDouble() );
   }
   return ok;
}

bool EarthModel::setSlotF(const Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setF( msg->getDouble() );
   }
   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex() for EarthModel
//------------------------------------------------------------------------------
Object* EarthModel::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& EarthModel::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "a:   " << getA() << std::endl;

    indent(sout,i+j);
    sout << "b: " << getB() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


}  // End Basic namespace
}  // End Eaagles namespace
