// Volumes.cpp: implementation of the Volume class.
// Liters, Cubic - Meters, Feet, Inches
///////////////////////////////////////////////////////////////////////////////

#include "openeaagles/basic/units/Volumes.h"
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/SlotTable.h"

namespace oe {
namespace basic {

//////////////////////////////////////////////////////////////////////
// Volume() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Volume, "Volume")
EMPTY_SLOTTABLE(Volume)

// Conversion constants
const LCreal Volume::CM2CFT = 35.3146667215f;         // CubicMeters => CubicFeet
const LCreal Volume::CFT2CM = (1 / Volume::CM2CFT); // CubicFeet => CubicMeters
const LCreal Volume::CM2CIN = 61023.7440947f;         // CubicMeters => CubicInches
const LCreal Volume::CIN2CM = (1 / Volume::CM2CIN); // CubicInches => CubicMeters
const LCreal Volume::CM2L = 1000.0f;                  // CubicMeters => Liters
const LCreal Volume::L2CM = (1 / Volume::CM2L);     // Liters => CubicMeters

//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Volume::Volume()
{
    STANDARD_CONSTRUCTOR()
}

Volume::Volume(const LCreal value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Volume::copyData(const Volume& org, const bool)
{
   BaseClass::copyData(org);
   val = fromVolume(org.toVolume());;
}


//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Volume)


//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
std::ostream& Volume::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
        indent(sout, i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout, i+j);
    sout << val << std::endl;

    BaseClass::serialize(sout, i+j, true);

    if (!slotsOnly) {
        indent(sout, i);
        sout << ")" << std::endl;
    }
    return sout;
}


//==============================================================================
// CubicMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CubicMeters, "CubicMeters")
EMPTY_SERIALIZER(CubicMeters)

CubicMeters::CubicMeters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicMeters::CubicMeters(const LCreal value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicMeters::CubicMeters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}
    

EMPTY_COPYDATA(CubicMeters)
EMPTY_DELETEDATA(CubicMeters)

//==============================================================================
// CubicFeet() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CubicFeet, "CubicFeet")
EMPTY_SERIALIZER(CubicFeet)

CubicFeet::CubicFeet() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicFeet::CubicFeet(const LCreal value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicFeet::CubicFeet(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(CubicFeet)
EMPTY_DELETEDATA(CubicFeet)

//==============================================================================
// CubicInches() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CubicInches, "CubicInches")
EMPTY_SERIALIZER(CubicInches)

CubicInches::CubicInches() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicInches::CubicInches(const LCreal value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicInches::CubicInches(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(CubicInches)
EMPTY_DELETEDATA(CubicInches)

//==============================================================================
// Liters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Liters, "Liters")
EMPTY_SERIALIZER(Liters)

Liters::Liters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

Liters::Liters(const LCreal value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
Liters::Liters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Liters)
EMPTY_DELETEDATA(Liters)

} // End basic namespace
} // End oe namespace

