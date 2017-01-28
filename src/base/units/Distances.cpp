//------------------------------------------------------------------------------
// Distance, Meters, CentiMeters, KiloMeters, Inches, Feet, NauticalMiles,
// StatuteMiles 
//------------------------------------------------------------------------------
#include "openeaagles/base/units/Distances.hpp"
#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Distance --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Distance, "AbstractDistance")
EMPTY_SLOTTABLE(Distance)
EMPTY_DELETEDATA(Distance)

Distance::Distance() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Distance::Distance(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Distance::copyData(const Distance& org, const bool)
{
   BaseClass::copyData(org);
   val = fromDistance(org.toDistance());;
}

std::ostream& Distance::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// Meters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Meters, "Meters")
EMPTY_SERIALIZER(Meters)
EMPTY_COPYDATA(Meters)
EMPTY_DELETEDATA(Meters)

Meters::Meters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// CentiMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CentiMeters, "CentiMeters")
EMPTY_SERIALIZER(CentiMeters)
EMPTY_COPYDATA(CentiMeters)
EMPTY_DELETEDATA(CentiMeters)

CentiMeters::CentiMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// MicroMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MicroMeters, "MicroMeters")
EMPTY_SERIALIZER(MicroMeters)
EMPTY_COPYDATA(MicroMeters)
EMPTY_DELETEDATA(MicroMeters)

MicroMeters::MicroMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Microns -- Same as Micrometers
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Microns, "Microns")
EMPTY_SERIALIZER(Microns)
EMPTY_COPYDATA(Microns)
EMPTY_DELETEDATA(Microns)

Microns::Microns() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// KiloMeters --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloMeters, "KiloMeters")
EMPTY_SERIALIZER(KiloMeters)
EMPTY_COPYDATA(KiloMeters)
EMPTY_DELETEDATA(KiloMeters)

KiloMeters::KiloMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Inches --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Inches, "Inches")
EMPTY_SERIALIZER(Inches)
EMPTY_COPYDATA(Inches)
EMPTY_DELETEDATA(Inches)

Inches::Inches() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Feet --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Feet, "Feet")
EMPTY_SERIALIZER(Feet)
EMPTY_COPYDATA(Feet)
EMPTY_DELETEDATA(Feet)

Feet::Feet() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// NauticalMiles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NauticalMiles, "NauticalMiles")
EMPTY_SERIALIZER(NauticalMiles)
EMPTY_COPYDATA(NauticalMiles)
EMPTY_DELETEDATA(NauticalMiles)

NauticalMiles::NauticalMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// StatuteMiles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(StatuteMiles, "StatuteMiles")
EMPTY_SERIALIZER(StatuteMiles)
EMPTY_COPYDATA(StatuteMiles)
EMPTY_DELETEDATA(StatuteMiles)

StatuteMiles::StatuteMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
