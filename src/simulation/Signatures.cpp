#include "openeaagles/simulation/Signatures.h"

#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"

#include "openeaagles/basic/Decibel.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Areas.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

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
                    //   Basic::Number(square meters) or Basic::Decibel(square meters) or Basic::Area() 
END_SLOTTABLE(SigConstant)

// Map slot table to handles 
BEGIN_SLOT_MAP(SigConstant)
    ON_SLOT(1,setRCS,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigConstant::SigConstant()
{
    STANDARD_CONSTRUCTOR()
    rcs = 0.0;
}

SigConstant::SigConstant(const LCreal r)
{
    STANDARD_CONSTRUCTOR()
    rcs = r;
}

SigConstant::SigConstant(const Basic::Number* const c)
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
LCreal SigConstant::getRCS(const Emission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRCS() -- Set the RCS 
//------------------------------------------------------------------------------
bool SigConstant::setRCS(const Basic::Number* const num)
{
    bool ok = false;
    LCreal r = -1.0f;

    const Basic::Area* d = dynamic_cast<const Basic::Area*>(num);
    if (d != 0) {
        // Has area units and we need square meters
        Basic::SquareMeters m2;
        r = m2.convert(*d);
    }
    else if (num != 0) {
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
Basic::Object* SigConstant::getSlotByIndex(const int si)
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
    ON_SLOT(1,setRadiusFromSlot,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigSphere::SigSphere()
{
    STANDARD_CONSTRUCTOR()
    setRadius(0);
}

SigSphere::SigSphere(const LCreal r)
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
LCreal SigSphere::getRCS(const Emission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRadiusFromSlot() -- Set the radius from Slot table
//------------------------------------------------------------------------------
bool SigSphere::setRadiusFromSlot(Basic::Number* const num)
{
    bool ok = false;
    LCreal r = -1.0f;

    Basic::Distance* d = dynamic_cast<Basic::Distance*>(num);
    if (d != 0) {
        // Has distance units and we need meters
        Basic::Meters meters;
        r = meters.convert(*d);
    }
    else if (num != 0) {
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
Basic::Object* SigSphere::getSlotByIndex(const int si)
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
    ON_SLOT(1,setA,Basic::Number)
    ON_SLOT(2,setB,Basic::Number)
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

SigPlate::SigPlate(const LCreal a1, const LCreal b1)
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
LCreal SigPlate::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != 0) {
        double lambda = em->getWavelength();
        double area = a * b;
        if (lambda > 0.0 && area > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            rcs = (4.0 * PI * area * area) / (lambda * lambda); 
        }
    }
    return (LCreal) rcs;
}

//------------------------------------------------------------------------------
// setA() -- Set the length
//------------------------------------------------------------------------------
bool SigPlate::setA(Basic::Number* const num)
{
    bool ok = false;
    LCreal v = -1.0f;

    Basic::Distance* d = dynamic_cast<Basic::Distance*>(num);
    if (d != 0) {
        // Has distance units and we need meters
        Basic::Meters meters;
        v = meters.convert(*d);
    }
    else if (num != 0) {
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
bool SigPlate::setB(Basic::Number* const num)
{
    bool ok = false;
    LCreal v = -1.0f;

    Basic::Distance* d = dynamic_cast<Basic::Distance*>(num);
    if (d != 0) {
        // Has distance units and we need meters
        Basic::Meters meters;
        v = meters.convert(*d);
    }
    else if (num != 0) {
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
Basic::Object* SigPlate::getSlotByIndex(const int si)
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
}

SigDihedralCR::SigDihedralCR(const LCreal a) : SigPlate(a,0.0)
{
    STANDARD_CONSTRUCTOR()
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
LCreal SigDihedralCR::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != 0) {
        double lambda = em->getWavelength();
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            double a = getA();
            rcs = (8.0 * PI * a*a*a*a) / (lambda*lambda); 
        }
    }
    return (LCreal) rcs;
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

SigTrihedralCR::SigTrihedralCR(const LCreal a) : SigDihedralCR(a)
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
LCreal SigTrihedralCR::getRCS(const Emission* const em)
{
    double rcs = 0.0;
    if (em != 0) {
        double lambda = em->getWavelength();
        if (lambda > 0.0) {
            // If we have lambda and the area of the plate, compute the RCS
            double a = getA();
            rcs = (12.0 * PI * a*a*a*a) / (lambda*lambda); 
        }
    }
    return (LCreal) rcs;
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
LCreal SigSwitch::getRCS(const Emission* const em)
{
   LCreal rcs = 0.0;

   // Find our ownship player ...
   const Player* ownship = (const Player*) findContainerByType(typeid(Player));
   if (ownship != 0) {

      // get our ownship's camouflage type
      unsigned int camouflage = ownship->getCamouflageType();
      camouflage++; // our components are one based

      // find a RfSignature with this index
      Basic::Pair* pair = findByIndex(camouflage);
      if (pair != 0) {
         RfSignature* sig = dynamic_cast<RfSignature*>( pair->object() );
         if (sig != 0) {

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
    "table",            // 1: Table of RCS by target Az/El angles  (Basic::Table2)
    "swapOrder",        // 2: True if elevation is the table's first independent
                        //    variable and azimuth is the second.
    "inDegrees",        // 3: True if the table's independent variables az and
                        //    el are in degrees instead of the default radians
    "inDecibel",        // 4: True if the dependent data is in decibel meters
                        //    squared instead of the default meters squared
END_SLOTTABLE(SigAzEl)

// Map slot table to handles 
BEGIN_SLOT_MAP(SigAzEl)
    ON_SLOT(1, setSlotTable,        Basic::Table2)
    ON_SLOT(2, setSlotSwapOrder,    Basic::Number)
    ON_SLOT(3, setSlotInDegrees,    Basic::Number)
    ON_SLOT(4, setSlotDecibel,      Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SigAzEl::SigAzEl()
{
   STANDARD_CONSTRUCTOR()

   tbl = 0;
   swapOrderFlg = false;
   degFlg = false;
   dbFlg = false;
}

SigAzEl::SigAzEl(const Basic::Table2* const tbl0)
{
   STANDARD_CONSTRUCTOR()

   tbl = 0;
   if (tbl0 != 0) {
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
      tbl = 0;
   }

   if (tbl != 0) { tbl->unref(); tbl = 0; }
   if (org.tbl != 0) {
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
    if (tbl != 0) { tbl->unref(); tbl = 0; }
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
LCreal SigAzEl::getRCS(const Emission* const em)
{
   LCreal rcs = 0.0;
   if (em != 0 && tbl != 0) {

      // angle of arrival (radians)
      LCreal iv1 = em->getAzimuthAoi();
      LCreal iv2 = em->getElevationAoi();

      // If the table's independent variable's order is swapped: (El, Az)
      if (isOrderSwapped()) {
         iv1 = em->getElevationAoi();
         iv2 = em->getAzimuthAoi();
      }

      // If the table's independent variables are in degrees ..
      if (isInDegrees()) {
         iv1 *= (LCreal)Basic::Angle::R2DCC;
         iv2 *= (LCreal)Basic::Angle::R2DCC;
      }

      rcs = tbl->lfi(iv1,iv2);

      // If the dependent data is in decibels ...
      if (isDecibel()) {
         rcs = lcPow(LCreal(10.0f), LCreal(rcs/10.0f));
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
   if (tbl != 0) {
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
bool SigAzEl::setSlotTable(const Basic::Table2* const msg)
{
   bool ok = false;
   if (msg != 0) {
      if (tbl != 0) tbl->unref();
      msg->ref();
      tbl = msg;
      ok = true;
   }
   return ok;
}

bool SigAzEl::setSlotSwapOrder(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setSwapOrder( msg->getBoolean() );
   }
   return ok;
}

bool SigAzEl::setSlotInDegrees(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setInDegrees( msg->getBoolean() );
   }
   return ok;
}

bool SigAzEl::setSlotDecibel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setDecibel( msg->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* SigAzEl::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


} // End Simulation namespace
} // End Eaagles namespace
