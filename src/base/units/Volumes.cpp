
#include "openeaagles/base/units/Volumes.hpp"
#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Volume() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Volume, "AbstractVolume")
EMPTY_SLOTTABLE(Volume)
EMPTY_DELETEDATA(Volume)

Volume::Volume()
{
    STANDARD_CONSTRUCTOR()
}

Volume::Volume(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Volume::copyData(const Volume& org, const bool)
{
   BaseClass::copyData(org);
   val = fromVolume(org.toVolume());;
}

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
EMPTY_COPYDATA(CubicMeters)
EMPTY_DELETEDATA(CubicMeters)

CubicMeters::CubicMeters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicMeters::CubicMeters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicMeters::CubicMeters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}
    
//==============================================================================
// CubicFeet() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CubicFeet, "CubicFeet")
EMPTY_SERIALIZER(CubicFeet)
EMPTY_COPYDATA(CubicFeet)
EMPTY_DELETEDATA(CubicFeet)

CubicFeet::CubicFeet() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicFeet::CubicFeet(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicFeet::CubicFeet(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// CubicInches() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(CubicInches, "CubicInches")
EMPTY_SERIALIZER(CubicInches)
EMPTY_COPYDATA(CubicInches)
EMPTY_DELETEDATA(CubicInches)

CubicInches::CubicInches() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicInches::CubicInches(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicInches::CubicInches(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Liters() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Liters, "Liters")
EMPTY_SERIALIZER(Liters)
EMPTY_COPYDATA(Liters)
EMPTY_DELETEDATA(Liters)

Liters::Liters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

Liters::Liters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
Liters::Liters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

