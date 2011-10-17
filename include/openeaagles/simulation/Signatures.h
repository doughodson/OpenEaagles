//------------------------------------------------------------------------------
// Classes:	RfSignature, SigSphere, SigPlate, SigCone
//
// Base class  Object -> Component -> RfSignature
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
#ifndef __Eaagles_Simulation_Signature_H__
#define __Eaagles_Simulation_Signature_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
   namespace Basic {
      class Number;
      class Table2;
   }
namespace Simulation {

class Emission;

//------------------------------------------------------------------------------
// Class: RfSignature
// Descriptions: Abstract class for signatures
// Public member functions:
//      LCreal getRCS(Emission* em)
//          Computes the Radar Cross Section for the emission.
//------------------------------------------------------------------------------
class RfSignature : public Basic::Component  
{
    DECLARE_SUBCLASS(RfSignature,Basic::Component)
public:
    RfSignature();
    virtual LCreal getRCS(const Emission* const em)=0;
};

//------------------------------------------------------------------------------
// Class: SigConstant
// Descriptions: Constant RCS signature
// Form name: SigConstant
// Slots:
//    rcs   <Basic::Number>  ! Constant Radar Cross Section value (default: 0.0)
//                           !  Basic::Number(square meters) or Basic::Decibel(square meters) or Basic::Area() 
//------------------------------------------------------------------------------
class SigConstant : public RfSignature  
{
    DECLARE_SUBCLASS(SigConstant,RfSignature)
public:
    SigConstant();
    SigConstant(const LCreal c);
    SigConstant(const Basic::Number* const c);

    virtual bool setRCS(const Basic::Number* const num);

    // RfSignature interface
    virtual LCreal getRCS(const Emission* const em);
private:
    LCreal rcs;         // Constant RCS value
};

//------------------------------------------------------------------------------
// Class: SigSphere
// Descriptions: RfSignature of a simple sphere based on radius
// Form name: SigSphere
// Slots:
//    radius   <Basic::Number>    ! Radius of the sphere (default: 0)
//
//------------------------------------------------------------------------------
class SigSphere : public RfSignature  
{
    DECLARE_SUBCLASS(SigSphere,RfSignature)
public:
    SigSphere();
    SigSphere(const LCreal r);
    virtual bool setRadiusFromSlot(Basic::Number* const num);
    LCreal computeRcs(const LCreal r)                          { return float(PI * r * r); }
    void setRadius(const LCreal r)                             { radius = r; rcs = computeRcs(radius); }

    // RfSignature interface
    virtual LCreal getRCS(const Emission* const em);
private:
    LCreal radius;      // Sphere radius
    LCreal rcs;         // RCS of sphere
};

//------------------------------------------------------------------------------
// Class: SigPlate
// Descriptions: RfSignature of a simple plate based on length & width, and always
//               normal to the transmitter.
// Form name: SigPlate
// Slots:
//    a    <Basic::Number>    ! length of the plate (default: 0.0)
//    b    <Basic::Number>    ! width of the plate (default: 0.0)
//
//------------------------------------------------------------------------------
class SigPlate : public RfSignature  
{
    DECLARE_SUBCLASS(SigPlate,RfSignature)
public:
    SigPlate();
    SigPlate(const LCreal a, const LCreal b);

    LCreal getA() const                             { return a; }
    LCreal getB() const                             { return b; }

    virtual bool setA(Basic::Number* const num);
    virtual bool setB(Basic::Number* const num);

    // RfSignature interface
    virtual LCreal getRCS(const Emission* const em);
private:
    LCreal a;       // Length dimension
    LCreal b;       // Width dimension
};

//------------------------------------------------------------------------------
// Class: SigDihedralCR
// Descriptions: RfSignature of a Dihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Form name: SigDihedralCR
//------------------------------------------------------------------------------
class SigDihedralCR : public SigPlate  
{
    DECLARE_SUBCLASS(SigDihedralCR,SigPlate)
public:
    SigDihedralCR();
    SigDihedralCR(const LCreal a);

    // RfSignature interface
    virtual LCreal getRCS(const Emission* const em);
private:
    LCreal length;      // Length dimension
};


//------------------------------------------------------------------------------
// Class: SigTrihedralCR
// Descriptions: RfSignature of a Trihedral Corner Reflector based on edge length,
//               and always facing the transmitter.
// Form name: SigTrihedralCR
//------------------------------------------------------------------------------
class SigTrihedralCR : public SigDihedralCR  
{
    DECLARE_SUBCLASS(SigTrihedralCR,SigDihedralCR)
public:
    SigTrihedralCR();
    SigTrihedralCR(const LCreal a);

    // RfSignature interface
    virtual LCreal getRCS(const Emission* const em);
};


//------------------------------------------------------------------------------
// Class: SigSwitch
// Descriptions: Switch between subcomponent signatures based on ownship camouflage type
// Form name: SigSwitch
// Note:
//  1) First pair (1:) is comouflage type 0, the second (2:) is camouflage type 1, etc.
//------------------------------------------------------------------------------
class SigSwitch : public RfSignature  
{
   DECLARE_SUBCLASS(SigSwitch,RfSignature)
public:
   SigSwitch();

   // RfSignature interface
   virtual LCreal getRCS(const Emission* const em);
};


//------------------------------------------------------------------------------
// Class: SigAzEl
// Descriptions: RfSignature by a table of RCS based on target's az/el angles
//
// Form name: SigAzEl
// Slots:
//
//    table      <Basic::Table2>   ! Table of RCS by target Az/El angles (default: 0)
//
//    swapOrder  <Basic::Number>   ! True if elevation is the table's first independent (default: false)
//                     ! variable and azumuth is the second.
//
//    inDegrees  <Basic::Number>   ! True if the table's independent variables az and
//                                 ! el are in degrees instead of the default radians (default: false)
//
//    inDecibel  <Basic::Number>   ! True if the dependent data is in decibel meters
//                                 ! squared instead of the default meters squared (default: false)
//
// Notes:
//  1) Must provide a Basic::Table2 (2 dimensional) table, where ...
//       -- Azimuth is the first independent variable (radians),
//       -- Elevation is the second independent variable (radians),
//       -- and the RCS is the dependent variable (meters squared)
//
//  2) If 'swapOrder' is set true then elevation is the table's first independent
//     variable and azumuth is the second.
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
   SigAzEl(const Basic::Table2* const tbl);

   virtual bool isTableValid() const;

   bool isOrderSwapped() const      { return swapOrderFlg; }
   virtual bool setSwapOrder(const bool flg);

   bool isInDegrees() const         { return degFlg; }
   virtual bool setInDegrees(const bool flg);

   bool isDecibel() const           { return dbFlg; }
   virtual bool setDecibel(const bool flg);

   // Slot functions
   virtual bool setSlotTable(const Basic::Table2* const tbl);
   virtual bool setSlotSwapOrder(const Basic::Number* const msg);
   virtual bool setSlotInDegrees(const Basic::Number* const msg);
   virtual bool setSlotDecibel(const Basic::Number* const msg);

   // RfSignature interface
   virtual LCreal getRCS(const Emission* const em);
protected:
   const Basic::Table2* tbl;      // The table
   bool swapOrderFlg;               // Swap independent data order from az/el to el/az
   bool degFlg;                     // independent data in degrees 
   bool dbFlg;                      // dependent data in decibels 
};


} // End Simulation namespace
} // End Eaagles namespace

#endif
