#include "openeaagles/simulation/Signatures.h"

#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"

#include "openeaagles/base/Decibel.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/functors/Tables.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Areas.h"
#include "openeaagles/base/units/Distances.h"

#include <cmath>

namespace oe {
namespace simulation {

//==============================================================================
// Class: RfSignature
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(RfSignature,"Signature")
EMPTY_SLOTTABLE(RfSignature)
EMPTY_SERIALIZER(RfSignature)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
RfSignature::RfSignature()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RfSignature::copyData(const RfSignature& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void RfSignature::deleteData()
{
}


//==============================================================================
// Class: SigConstant
//==============================================================================
IMPLEMENT_SUBCLASS(SigConstant,"SigConstant")
EMPTY_SERIALIZER(SigConstant)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SigConstant)
    "rcs",          // 1 Constant Radar Cross Section value
                    //   base::Number(square meters) or base::Decibel(square meters) or base::Area()
END_SLOTTABLE(SigConstant)

// Map slot table to handles
BEGIN_SLOT_MAP(SigConstant)
    ON_SLOT(1,setRCS,base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigConstant::SigConstant()
{
    STANDARD_CONSTRUCTOR()
    rcs = 0.0;
}

SigConstant::SigConstant(const double r)
{
    STANDARD_CONSTRUCTOR()
    rcs = r;
}

SigConstant::SigConstant(const base::Number* const c)
{
    STANDARD_CONSTRUCTOR()
    setRCS(c);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigConstant::copyData(const SigConstant& org, const bool)
{
    BaseClass::copyData(org);
    rcs = org.rcs;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigConstant::deleteData()
{
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigConstant::getRCS(const Emission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRCS() -- Set the RCS
//------------------------------------------------------------------------------
bool SigConstant::setRCS(const base::Number* const num)
{
    bool ok = false;
    double r = -1.0;

    const base::Area* d = dynamic_cast<const base::Area*>(num);
    if (d != nullptr) {
        // Has area units and we need square meters
        base::SquareMeters m2;
        r = m2.convert(*d);
    }
    else if (num != nullptr) {
        // square meters (Number or Decibel)
        r = num->getReal();
    }

    if (r >= 0.0) { rcs = r; ok = true; }
    else { std::cerr << "SigConstant::setRCS: invalid rcs; must be greater than or equal to zero!" << std::endl; }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* SigConstant::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Class: SigSphere
//==============================================================================
IMPLEMENT_SUBCLASS(SigSphere,"SigSphere")
EMPTY_SERIALIZER(SigSphere)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SigSphere)
    "radius",       // 1 Radius of the sphere
END_SLOTTABLE(SigSphere)

// Map slot table to handles
BEGIN_SLOT_MAP(SigSphere)
    ON_SLOT(1,setRadiusFromSlot,base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigSphere::SigSphere()
{
    STANDARD_CONSTRUCTOR()
    setRadius(0);
}

SigSphere::SigSphere(const double r)
{
    STANDARD_CONSTRUCTOR()
    setRadius(r);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigSphere::copyData(const SigSphere& org, const bool)
{
    BaseClass::copyData(org);
    setRadius(org.radius);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigSphere::deleteData()
{
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigSphere::getRCS(const Emission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRadiusFromSlot() -- Set the radius from Slot table
//------------------------------------------------------------------------------
bool SigSphere::setRadiusFromSlot(base::Number* const num)
{
    bool ok = false;
    double r = -1.0;

    base::Distance* d = dynamic_cast<base::Distance*>(num);
    if (d != nullptr) {
        // Has distance units and we need meters
        base::Meters meters;
        r = meters.convert(*d);
    }
    else if (num != nullptr) {
        // Just a Number
        r = num->getReal();
    }

    if (r >= 0.0) { setRadius(r); ok = true; }
    else { std::cerr << "SigSphere::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl; }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* SigSphere::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Class: SigPlate
//==============================================================================
IMPLEMENT_SUBCLASS(SigPlate,"SigPlate")
EMPTY_SERIALIZER(SigPlate)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SigPlate)
    "a",        // 1 length of the plate
    "b",        // 2 width of the plate
END_SLOTTABLE(SigPlate)

// Map slot table to handles
BEGIN_SLOT_MAP(SigPlate)
    ON_SLOT(1,setA,base::Number)
    ON_SLOT(2,setB,base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigPlate::SigPlate()
{
    STANDARD_CONSTRUCTOR()
    a = 0.0;
    b = 0.0;
}

SigPlate::SigPlate(const double a1, const double b1)
{
    STANDARD_CONSTRUCTOR()
    a = a1;
    b = b1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigPlate::copyData(const SigPlate& org, const bool)
{
    BaseClass::copyData(org);
    a = org.a;
    b = org.b;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigPlate::deleteData()
{
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigPlate::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != nullptr) {
        double lambda = em->getWavelength();
        double area = a * b;
        if (lambda > 0.0 && area > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            rcs = (4.0 * base::PI * area * area) / (lambda * lambda);
        }
    }
    return static_cast<double>(rcs);
}

//------------------------------------------------------------------------------
// setA() -- Set the length
//------------------------------------------------------------------------------
bool SigPlate::setA(base::Number* const num)
{
    bool ok = false;
    double v = -1.0;

    base::Distance* d = dynamic_cast<base::Distance*>(num);
    if (d != nullptr) {
        // Has distance units and we need meters
        base::Meters meters;
        v = meters.convert(*d);
    }
    else if (num != nullptr) {
        // Just a Number
        v = num->getReal();
    }

    if (v >= 0.0) { a = v; ok = true; }
    else { std::cerr << "SigPlate::setWidthFromSlot: invalid: must be greater than or equal to zero!" << std::endl; }
    return ok;
}

//------------------------------------------------------------------------------
// setB() -- Set the width
//------------------------------------------------------------------------------
bool SigPlate::setB(base::Number* const num)
{
    bool ok = false;
    double v = -1.0;

    base::Distance* d = dynamic_cast<base::Distance*>(num);
    if (d != nullptr) {
        // Has distance units and we need meters
        base::Meters meters;
        v = meters.convert(*d);
    }
    else if (num != nullptr) {
        // Just a Number
        v = num->getReal();
    }

    if (v >= 0.0) { b = v; ok = true; }
    else { std::cerr << "SigPlate::setHeightFromSlot: invalid: must be greater than or equal to zero!" << std::endl; }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* SigPlate::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Class: SigDihedralCR
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SigDihedralCR,"SigDihedralCR")
EMPTY_SERIALIZER(SigDihedralCR)


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigDihedralCR::SigDihedralCR()
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

SigDihedralCR::SigDihedralCR(const double a) : SigPlate(a, 0.0)
{
    STANDARD_CONSTRUCTOR()
    length = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigDihedralCR::copyData(const SigDihedralCR& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigDihedralCR::deleteData()
{
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigDihedralCR::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != nullptr) {
        const double lambda = em->getWavelength();
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            const double a = getA();
            rcs = (8.0 * base::PI * a*a*a*a) / (lambda*lambda);
        }
    }
    return static_cast<double>(rcs);
}


//==============================================================================
// Class: SigTrihedralCR
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SigTrihedralCR,"SigTrihedralCR")
EMPTY_SERIALIZER(SigTrihedralCR)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigTrihedralCR::SigTrihedralCR()
{
    STANDARD_CONSTRUCTOR()
}

SigTrihedralCR::SigTrihedralCR(const double a) : SigDihedralCR(a)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigTrihedralCR::copyData(const SigTrihedralCR& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigTrihedralCR::deleteData()
{
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigTrihedralCR::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != nullptr) {
        const double lambda = em->getWavelength();
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            const double a = getA();
            rcs = (12.0 * base::PI * a*a*a*a) / (lambda*lambda);
        }
    }
    return static_cast<double>(rcs);
}

//==============================================================================
// Class: SigSwitch
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SigSwitch,"SigSwitch")
EMPTY_SERIALIZER(SigSwitch)


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigSwitch::SigSwitch()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigSwitch::copyData(const SigSwitch& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigSwitch::deleteData()
{
}


//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigSwitch::getRCS(const Emission* const em)
{
   double rcs = 0.0;

   // Find our ownship player ...
   const Player* ownship = static_cast<const Player*>(findContainerByType(typeid(Player)));
   if (ownship != nullptr) {

      // get our ownship's camouflage type
      unsigned int camouflage = ownship->getCamouflageType();
      camouflage++; // our components are one based

      // find a RfSignature with this index
      base::Pair* pair = findByIndex(camouflage);
      if (pair != nullptr) {
         RfSignature* sig = dynamic_cast<RfSignature*>( pair->object() );
         if (sig != nullptr) {

            // OK -- we've found the correct RfSignature subcomponent
            // now let it do all of the work
            rcs = sig->getRCS(em);

         }
      }

   }

   return rcs;
}


//==============================================================================
// Class: SigAzEl
//==============================================================================
IMPLEMENT_SUBCLASS(SigAzEl,"SigAzEl")
EMPTY_SERIALIZER(SigAzEl)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SigAzEl)
    "table",            // 1: Table of RCS by target Az/El angles  (base::Table2)
    "swapOrder",        // 2: True if elevation is the table's first independent
                        //    variable and azimuth is the second.
    "inDegrees",        // 3: True if the table's independent variables az and
                        //    el are in degrees instead of the default radians
    "inDecibel",        // 4: True if the dependent data is in decibel meters
                        //    squared instead of the default meters squared
END_SLOTTABLE(SigAzEl)

// Map slot table to handles
BEGIN_SLOT_MAP(SigAzEl)
    ON_SLOT(1, setSlotTable,        base::Table2)
    ON_SLOT(2, setSlotSwapOrder,    base::Number)
    ON_SLOT(3, setSlotInDegrees,    base::Number)
    ON_SLOT(4, setSlotDecibel,      base::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigAzEl::SigAzEl()
{
   STANDARD_CONSTRUCTOR()

   tbl = nullptr;
   swapOrderFlg = false;
   degFlg = false;
   dbFlg = false;
}

SigAzEl::SigAzEl(const base::Table2* const tbl0)
{
   STANDARD_CONSTRUCTOR()

   tbl = nullptr;
   if (tbl0 != nullptr) {
      tbl = tbl0->clone();
   }
   swapOrderFlg = false;
   degFlg = false;
   dbFlg = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SigAzEl::copyData(const SigAzEl& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
      tbl = nullptr;
   }

   if (tbl != nullptr) { tbl->unref(); tbl = nullptr; }
   if (org.tbl != nullptr) {
      tbl = org.tbl->clone();
   }

   swapOrderFlg = org.swapOrderFlg;
   degFlg = org.degFlg;
   dbFlg = org.dbFlg;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SigAzEl::deleteData()
{
    if (tbl != nullptr) { tbl->unref(); tbl = nullptr; }
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigAzEl::getRCS(const Emission* const em)
{
   double rcs = 0.0;
   if (em != nullptr && tbl != nullptr) {

      // angle of arrival (radians)
      double iv1 = em->getAzimuthAoi();
      double iv2 = em->getElevationAoi();

      // If the table's independent variable's order is swapped: (El, Az)
      if (isOrderSwapped()) {
         iv1 = em->getElevationAoi();
         iv2 = em->getAzimuthAoi();
      }

      // If the table's independent variables are in degrees ..
      if (isInDegrees()) {
         iv1 *= static_cast<double>(base::Angle::R2DCC);
         iv2 *= static_cast<double>(base::Angle::R2DCC);
      }

      rcs = tbl->lfi(iv1,iv2);

      // If the dependent data is in decibels ...
      if (isDecibel()) {
         rcs = std::pow(static_cast<double>(10.0), static_cast<double>(rcs / 10.0));
      }
   }
   return rcs;
}

//------------------------------------------------------------------------------
// isTableValid() -- Returns true if this signature has a good az/el table
//------------------------------------------------------------------------------
bool SigAzEl::isTableValid() const
{
   bool ok = false;
   if (tbl != nullptr) {
      ok = tbl->isValid();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SigAzEl::setSwapOrder(const bool flg)
{
   swapOrderFlg = flg;
   return true;
}

bool SigAzEl::setInDegrees(const bool flg)
{
   degFlg = flg;
   return true;
}

bool SigAzEl::setDecibel(const bool flg)
{
   dbFlg = flg;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Sets the signature table
bool SigAzEl::setSlotTable(const base::Table2* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (tbl != nullptr) tbl->unref();
      msg->ref();
      tbl = msg;
      ok = true;
   }
   return ok;
}

bool SigAzEl::setSlotSwapOrder(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setSwapOrder( msg->getBoolean() );
   }
   return ok;
}

bool SigAzEl::setSlotInDegrees(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInDegrees( msg->getBoolean() );
   }
   return ok;
}

bool SigAzEl::setSlotDecibel(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setDecibel( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* SigAzEl::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


} // End simulation namespace
}
