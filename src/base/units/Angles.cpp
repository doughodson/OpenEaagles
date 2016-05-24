//------------------------------------------------------------------------------
// Angle, Degrees, Radians, Semicircles
//------------------------------------------------------------------------------
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"

namespace oe {
namespace base {


//==============================================================================
// Angle --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Angle, "Angle")
EMPTY_SLOTTABLE(Angle)

// Conversion constants:
const double Angle::D2SC  = 0.0055555555555556;  // Degrees => Semicircles
const double Angle::SC2D  = 180.0;               // Semicircles => Degrees
const double Angle::R2SC  = 0.3183098861837906;  // Radians => Semicircles
const double Angle::SC2R  = PI;                  // Semicircles => Radians
const double Angle::R2DCC = (180.0 / PI);        // Radians => Degrees
const double Angle::D2RCC = (PI / 180.0);        // Degrees => Radians


// ---
// constructors
// ---
Angle::Angle() : Number()
{
   STANDARD_CONSTRUCTOR()
}

Angle::Angle(const double value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

// ---
// copy data
// ---
void Angle::copyData(const Angle& org, const bool)
{
   BaseClass::copyData(org);
   val = fromAngle(org.toAngle());;
}

// ---
// delete data
// ---
EMPTY_DELETEDATA(Angle)

// ---
// serialize() -- print functions
// ---
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

EMPTY_COPYDATA(Degrees)
EMPTY_DELETEDATA(Degrees)



//==============================================================================
// Radians --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Radians, "Radians")
EMPTY_SERIALIZER(Radians)

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

EMPTY_COPYDATA(Radians)
EMPTY_DELETEDATA(Radians)


//==============================================================================
// Semicircles --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Semicircles, "Semicircles")
EMPTY_SERIALIZER(Semicircles)

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

EMPTY_COPYDATA(Semicircles)
EMPTY_DELETEDATA(Semicircles)

}
}
