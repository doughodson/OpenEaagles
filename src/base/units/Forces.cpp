
#include "openeaagles/base/units/Forces.hpp"

#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Force() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Force, "AbstractForce")
EMPTY_SLOTTABLE(Force)
EMPTY_DELETEDATA(Force)

Force::Force()
{
    STANDARD_CONSTRUCTOR()
}

Force::Force(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Force::copyData(const Force& org, const bool)
{
   BaseClass::copyData(org);
   val = fromForce(org.toForce());;
}

std::ostream& Force::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// Newtons() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Newtons, "Newtons")
EMPTY_SERIALIZER(Newtons)
EMPTY_COPYDATA(Newtons)
EMPTY_DELETEDATA(Newtons)

Newtons::Newtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Kilonewtons() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloNewtons, "KiloNewtons")
EMPTY_SERIALIZER(KiloNewtons)
EMPTY_COPYDATA(KiloNewtons)
EMPTY_DELETEDATA(KiloNewtons)

KiloNewtons::KiloNewtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// PoundForces() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(PoundForces, "PoundForces")
EMPTY_SERIALIZER(PoundForces)
EMPTY_COPYDATA(PoundForces)
EMPTY_DELETEDATA(PoundForces)

PoundForces::PoundForces() : Force()
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Poundals() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Poundals, "Poundals")
EMPTY_SERIALIZER(Poundals)
EMPTY_COPYDATA(Poundals)
EMPTY_DELETEDATA(Poundals)

Poundals::Poundals() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

