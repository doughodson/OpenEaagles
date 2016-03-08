//------------------------------------------------------------------------------
// Classes: RfSignature, SigSphere, SigPlate, SigCone
//
// Base class: Object -> Component -> RfSignature
//                          RfSignature -> SigConstant
//                          RfSignature -> SigSphere
//                          RfSignature -> SigPlate
//                          RfSignature -> SigPlate -> SigDihedralCR
//                          RfSignature -> SigPlate -> SigDihedralCR -> SigTrihedralCR
//                          RfSignature -> SigSwitch
//                          RfSignature -> SigAzEl
//
// Description: RF Signatures
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Signature_H__
#define __oe_simulation_Signature_H__

#include "openeaagles/base/Component.h"

namespace oe {
   namespace base {
      class Number;
      class Table2;
   }
namespace simulation {

class Emission;

//------------------------------------------------------------------------------
// Class: RfSignature
// Descriptions: Abstract class for signatures
// Public member functions:
//      double getRCS(Emission* em)
//          Computes the Radar Cross Section for the emission.
//------------------------------------------------------------------------------
class RfSignature : public base::Component
{
    DECLARE_SUBCLASS(RfSignature, base::Component)
public:
    RfSignature();
    virtual double getRCS(const Emission* const em)=0;
};

//------------------------------------------------------------------------------
// Class: SigConstant
// Descriptions: Constant RCS signature
// Factory name: SigConstant
// Slots:
//    rcs   <base::Number>  ! Constant Radar Cross Section value (default: 0.0)
//                           !  base::Number(square meters) or base::Decibel(square meters) or base::Area()
//------------------------------------------------------------------------------
class SigConstant : public RfSignature
{
    DECLARE_SUBCLASS(SigConstant,RfSignature)
public:
    SigConstant();
    SigConstant(const double c);
    SigConstant(const base::Number* const c);

    virtual bool setRCS(const base::Number* const num);

    double getRCS(const Emission* const em) override;
private:
    double rcs;         // Constant RCS value
};

//------------------------------------------------------------------------------
// Class: SigSphere
// Descriptions: RfSignature of a simple sphere based on radius
// Factory name: SigSphere
// Slots:
//    radius   <base::Number>    ! Radius of the sphere (default: 0)
//
//------------------------------------------------------------------------------
class SigSphere : public RfSignature
{
    DECLARE_SUBCLASS(SigSphere,RfSignature)
public:
    SigSphere();
    SigSphere(const double r);
    virtual bool setRadiusFromSlot(base::Number* const num);
    double computeRcs(const double r)                          { return static_cast<float>(base::PI * r * r); }
    void setRadius(const double r)                             { radius = r; rcs = computeRcs(radius); }

    double getRCS(const Emission* const em) override;
private:
    double radius;      // Sphere radius
    double rcs;         // RCS of sphere
};

//------------------------------------------------------------------------------
// Class: SigPlate
// Descriptions: RfSignature of a simple plate based on length & width, and always
//               normal to the transmitter.
// Factory name: SigPlate
// Slots:
//    a    <base::Number>    ! length of the plate (default: 0.0)
//    b    <base::Number>    ! width of the plate (default: 0.0)
//
//------------------------------------------------------------------------------
class SigPlate : public RfSignature
{
    DECLARE_SUBCLASS(SigPlate,RfSignature)
public:
    SigPlate();
    SigPlate(const double a, const double b);

    double getA() const                             { return a; }
    double getB() const                             { return b; }

    virtual bool setA(base::Number* const num);
    virtual bool setB(base::Number* const num);

    double getRCS(const Emission* const em) override;
private:
    double a;       // Length dimension
    double b;       // Width dimension
};

//------------------------------------------------------------------------------
// Class: SigDihedralCR
// Descriptions: RfSignature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigDihedralCR
//------------------------------------------------------------------------------
class SigDihedralCR : public SigPlate
{
    DECLARE_SUBCLASS(SigDihedralCR,SigPlate)
public:
    SigDihedralCR();
    SigDihedralCR(const double a);

    double getRCS(const Emission* const em) override;
private:
    double length;      // Length dimension
};


//------------------------------------------------------------------------------
// Class: SigTrihedralCR
// Descriptions: RfSignature of a Trihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Factory name: SigTrihedralCR
//------------------------------------------------------------------------------
class SigTrihedralCR : public SigDihedralCR
{
    DECLARE_SUBCLASS(SigTrihedralCR,SigDihedralCR)
public:
    SigTrihedralCR();
    SigTrihedralCR(const double a);

    double getRCS(const Emission* const em) override;
};


//------------------------------------------------------------------------------
// Class: SigSwitch
// Descriptions: Switch between subcomponent signatures based on ownship camouflage type
// Factory name: SigSwitch
// Note:
//  1) First pair (1:) is camouflage type 0, the second (2:) is camouflage type 1, etc.
//------------------------------------------------------------------------------
class SigSwitch : public RfSignature
{
   DECLARE_SUBCLASS(SigSwitch,RfSignature)
public:
   SigSwitch();

   double getRCS(const Emission* const em) override;
};


//------------------------------------------------------------------------------
// Class: SigAzEl
// Descriptions: RfSignature by a table of RCS based on target's az/el angles
//
// Factory name: SigAzEl
// Slots:
//
//    table      <base::Table2>   ! Table of RCS by target Az/El angles (default: 0)
//
//    swapOrder  <base::Number>   ! True if elevation is the table's first independent (default: false)
//                     ! variable and azimuth is the second.
//
//    inDegrees  <base::Number>   ! True if the table's independent variables az and
//                                 ! el are in degrees instead of the default radians (default: false)
//
//    inDecibel  <base::Number>   ! True if the dependent data is in decibel meters
//                                 ! squared instead of the default meters squared (default: false)
//
// Notes:
//  1) Must provide a base::Table2 (2 dimensional) table, where ...
//       -- Azimuth is the first independent variable (radians),
//       -- Elevation is the second independent variable (radians),
//       -- and the RCS is the dependent variable (meters squared)
//
//  2) If 'swapOrder' is set true then elevation is the table's first independent
//     variable and azimuth is the second.
//
//  3) If 'inDegrees' is set true then the table's independent variables az and
//     el are in degrees instead of the default radians
//
//  4) If 'inDecibel' is set true then the dependent data is in decibel meters
//     squared instead of the default meters squared
//------------------------------------------------------------------------------
class SigAzEl : public RfSignature
{
    DECLARE_SUBCLASS(SigAzEl,RfSignature)
public:
   SigAzEl();
   SigAzEl(const base::Table2* const tbl);

   virtual bool isTableValid() const;

   bool isOrderSwapped() const      { return swapOrderFlg; }
   virtual bool setSwapOrder(const bool flg);

   bool isInDegrees() const         { return degFlg; }
   virtual bool setInDegrees(const bool flg);

   bool isDecibel() const           { return dbFlg; }
   virtual bool setDecibel(const bool flg);

   // Slot functions
   virtual bool setSlotTable(const base::Table2* const tbl);
   virtual bool setSlotSwapOrder(const base::Number* const msg);
   virtual bool setSlotInDegrees(const base::Number* const msg);
   virtual bool setSlotDecibel(const base::Number* const msg);

   double getRCS(const Emission* const em) override;
protected:
   const base::Table2* tbl;      // The table
   bool swapOrderFlg;               // Swap independent data order from az/el to el/az
   bool degFlg;                     // independent data in degrees
   bool dbFlg;                      // dependent data in decibels
};

} // End simulation namespace
} // End oe namespace

#endif
