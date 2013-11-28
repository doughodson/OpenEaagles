//------------------------------------------------------------------------------
// Distance, Meters, CentiMeters, KiloMeters, Inches, Feet, NauticalMiles,
// StatuteMiles 
//------------------------------------------------------------------------------
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/SlotTable.h"

namespace Eaagles {
namespace Basic {


//==============================================================================
// Distance --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Distance, "Distance")
EMPTY_SLOTTABLE(Distance)

// Conversion constants
const LCreal Distance::FT2M =  0.30480f;                  // Feet => Meters
const LCreal Distance::M2FT = (1 / Distance::FT2M);       // Meters => Feet
const LCreal Distance::IN2M =  0.02540f;                  // Inches => Meters
const LCreal Distance::M2IN =  (1 / Distance::IN2M);      // Meters => Inches

const LCreal Distance::NM2M = 1852.0f;                    // Nautical Miles => Meters
const LCreal Distance::M2NM = (1 / Distance::NM2M);       // Meters => Nautical Miles
const LCreal Distance::NM2FT = (NM2M * M2FT);             // Nautical Miles => Feet
const LCreal Distance::FT2NM = 1/NM2FT;                   // Feet => Nautical Miles

const LCreal Distance::SM2M = 1609.344f;                  // Statue Miles => Meters
const LCreal Distance::M2SM = (1 / Distance::SM2M);       // Meters => Statue Miles
const LCreal Distance::SM2FT = 5280.0f;                   // Statue Miles => Feet
const LCreal Distance::FT2SM = (1 / Distance::SM2FT);     // Feet => Statue Miles

const LCreal Distance::KM2M = 1000.0f;                    // Kilometers => Meters
const LCreal Distance::M2KM = (1 / Distance::KM2M);       // Meters => Kilometers
const LCreal Distance::CM2M = 0.01f;                      // Centimeters => Meters
const LCreal Distance::M2CM = (1 / Distance::CM2M);       // Meters => Centimeters
const LCreal Distance::UM2M = 0.000001f;                  // Micrometer (Micron) => Meters
const LCreal Distance::M2UM = (1 / Distance::UM2M);       // Meters => Micrometer (Micron)


// ---
// constructors
// ---
Distance::Distance() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Distance::Distance(const LCreal value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

// ---
// copy data
// ---
void Distance::copyData(const Distance& org, const bool)
{
   BaseClass::copyData(org);
   val = fromDistance(org.toDistance());;
}

// ---
// delete data
// ---
EMPTY_DELETEDATA(Distance)

// ---
// serialize() -- print functions
// ---
std::ostream& Distance::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if (!slotsOnly) {
        indent(sout, i);
        sout << "( " << getFormName() << std::endl;
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
// Meters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Meters, "Meters")
EMPTY_SERIALIZER(Meters)

Meters::Meters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Meters)
EMPTY_DELETEDATA(Meters)


//==============================================================================
// CentiMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CentiMeters, "CentiMeters")
EMPTY_SERIALIZER(CentiMeters)

CentiMeters::CentiMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(CentiMeters)
EMPTY_DELETEDATA(CentiMeters)



//==============================================================================
// MicroMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MicroMeters, "MicroMeters")
EMPTY_SERIALIZER(MicroMeters)

MicroMeters::MicroMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(MicroMeters)
EMPTY_DELETEDATA(MicroMeters)

//==============================================================================
// Microns -- Same as Micrometers
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Microns, "Microns")
EMPTY_SERIALIZER(Microns)

Microns::Microns() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Microns)
EMPTY_DELETEDATA(Microns)

//==============================================================================
// KiloMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloMeters, "KiloMeters")
EMPTY_SERIALIZER(KiloMeters)

KiloMeters::KiloMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(KiloMeters)
EMPTY_DELETEDATA(KiloMeters)


//==============================================================================
// Inches --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Inches, "Inches")
EMPTY_SERIALIZER(Inches)

Inches::Inches() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Inches)
EMPTY_DELETEDATA(Inches)


//==============================================================================
// Feet --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Feet, "Feet")
EMPTY_SERIALIZER(Feet)

Feet::Feet() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Feet)
EMPTY_DELETEDATA(Feet)


//==============================================================================
// NauticalMiles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NauticalMiles, "NauticalMiles")
EMPTY_SERIALIZER(NauticalMiles)

NauticalMiles::NauticalMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(NauticalMiles)
EMPTY_DELETEDATA(NauticalMiles)


//==============================================================================
// StatuteMiles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(StatuteMiles, "StatuteMiles")
EMPTY_SERIALIZER(StatuteMiles)

StatuteMiles::StatuteMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const LCreal value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(StatuteMiles)
EMPTY_DELETEDATA(StatuteMiles)

} // End Basic namespace
} // End Eaagles namespace
