
#include "openeaagles/base/units/Angles.hpp"

#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Angle --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Angle, "AbstractAngle")
EMPTY_SLOTTABLE(Angle)
EMPTY_DELETEDATA(Angle)

Angle::Angle() : Number()
{
   STANDARD_CONSTRUCTOR()
}

Angle::Angle(const double value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

void Angle::copyData(const Angle& org, const bool)
{
   BaseClass::copyData(org);
   val = fromAngle(org.toAngle());;
}

std::ostream& Angle::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// Degrees --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Degrees, "Degrees")
EMPTY_SERIALIZER(Degrees)
EMPTY_COPYDATA(Degrees)
EMPTY_DELETEDATA(Degrees)

Degrees::Degrees() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Radians --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Radians, "Radians")
EMPTY_SERIALIZER(Radians)
EMPTY_COPYDATA(Radians)
EMPTY_DELETEDATA(Radians)

Radians::Radians() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Semicircles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Semicircles, "Semicircles")
EMPTY_SERIALIZER(Semicircles)
EMPTY_COPYDATA(Semicircles)
EMPTY_DELETEDATA(Semicircles)

Semicircles::Semicircles() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
