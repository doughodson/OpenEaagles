// Area.cpp: implementation of the Area class .          
// Square - Meters, Inches, Yards, Miles, KiloMeters, CentiMeters, MilliMeters,
// DecibelMeters, Feet
///////////////////////////////////////////////////////////////////////////////

#include "openeaagles/base/units/Areas.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"  

namespace oe {
namespace basic {

//////////////////////////////////////////////////////////////////////
// Area()
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Area, "Area")
EMPTY_SLOTTABLE(Area)

// Conversion constants
const LCreal Area::SM2SFT = 10.76391f;                 // Sq. Meters      => Sq. Feet
const LCreal Area::SFT2SM = (1.0f / Area::SM2SFT);     // Sq. Feet        => Sq. Meters
const LCreal Area::SM2SYD = 1.19599f;                  // Sq. Meters      => Sq. Yards
const LCreal Area::SYD2SM = (1.0f / Area::SM2SYD);     // Sq. Yards       => Sq. Meters
const LCreal Area::SM2SMI = 0.00000038610216f;         // Sq. Meters      => Sq. Miles
const LCreal Area::SMI2SM = (1.0f / Area::SM2SMI);     // Sq. Miles       => Sq. Meters
const LCreal Area::SM2SIN = 1550.0030399f;             // Sq. Meters      => Sq. Inches
const LCreal Area::SIN2SM = (1.0f / Area::SM2SIN);     // Sq. Inches      => Sq. Meters
const LCreal Area::SM2SCM = 10000.0f;                  // Sq. Meters      => Sq. CentiMeters
const LCreal Area::SCM2SM = (1.0f / Area::SM2SCM);     // Sq. CentiMeters => Sq. Meters
const LCreal Area::SM2SMM = 1000000.0f;                // Sq. Meters      => Sq. MilliMeters
const LCreal Area::SMM2SM = (1.0f / Area::SM2SMM);     // Sq. MilliMeters => Sq. Meters
const LCreal Area::SM2SKM = 0.000001f;                 // Sq. Meters      => Sq. KiloMeters
const LCreal Area::SKM2SM = (1.0f / Area::SM2SKM);     // Sq. KiloMeters  => Sq. Meters


//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Area::Area()
{
   STANDARD_CONSTRUCTOR()
}

Area::Area(const LCreal value) : Number(value) 
{
   STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Area::copyData(const Area& org, const bool)
{
   BaseClass::copyData(org);
   val = fromArea(org.toArea());;
}


//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Area)


//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
std::ostream& Area::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// SquareMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareMeters, "SquareMeters")
EMPTY_SERIALIZER(SquareMeters)

SquareMeters::SquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareMeters)
EMPTY_DELETEDATA(SquareMeters)

//==============================================================================
// SquareFeet() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareFeet, "SquareFeet")
EMPTY_SERIALIZER(SquareFeet)

SquareFeet::SquareFeet() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareFeet)
EMPTY_DELETEDATA(SquareFeet)

//==============================================================================
// SquareInches() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareInches, "SquareInches")
EMPTY_SERIALIZER(SquareInches)

SquareInches::SquareInches() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareInches)
EMPTY_DELETEDATA(SquareInches)


//==============================================================================
// SquareYards() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareYards, "SquareYards")
EMPTY_SERIALIZER(SquareYards)

SquareYards::SquareYards() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareYards)
EMPTY_DELETEDATA(SquareYards)

//==============================================================================
// SquareMiles() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareMiles, "SquareMiles")
EMPTY_SERIALIZER(SquareMiles)

SquareMiles::SquareMiles() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareMiles)
EMPTY_DELETEDATA(SquareMiles)


//==============================================================================
// SquareCentiMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareCentiMeters, "SquareCentiMeters")
EMPTY_SERIALIZER(SquareCentiMeters)

SquareCentiMeters::SquareCentiMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareCentiMeters)
EMPTY_DELETEDATA(SquareCentiMeters)

//==============================================================================
// SquareMilliMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareMilliMeters, "SquareMilliMeters")
EMPTY_SERIALIZER(SquareMilliMeters)

SquareMilliMeters::SquareMilliMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareMilliMeters)
EMPTY_DELETEDATA(SquareMilliMeters)

//==============================================================================
// SquareKiloMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SquareKiloMeters, "SquareKiloMeters")
EMPTY_SERIALIZER(SquareKiloMeters)

SquareKiloMeters::SquareKiloMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(SquareKiloMeters)
EMPTY_DELETEDATA(SquareKiloMeters)

//==============================================================================
// DecibelSquareMeters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelSquareMeters, "DecibelSquareMeters")
EMPTY_SERIALIZER(DecibelSquareMeters)

DecibelSquareMeters::DecibelSquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const LCreal value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(DecibelSquareMeters)
EMPTY_DELETEDATA(DecibelSquareMeters)

} // End basic namespace
} // End oe namespace
