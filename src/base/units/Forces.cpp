// Forces.cpp: implementation of the Forces class.
// Forces, Newtons, KiloNewtons, PoundForces, Poundals
//////////////////////////////////////////////////////////////////////

#include "openeaagles/base/units/Forces.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"

namespace oe {
namespace base {

//////////////////////////////////////////////////////////////////////
// Force() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Force, "Force")
EMPTY_SLOTTABLE(Force)

// Conversion constants
const double Force::N2KN = 0.001f;                // Newtons => KiloNewtons
const double Force::KN2N = (1 / Force::N2KN);   // KiloNewtons => Newtons
const double Force::N2PF = 0.224809f;             // Newtons => PoundForces
const double Force::PF2N = (1 / Force::N2PF);   // PoundForces => Newtons
const double Force::N2PD = 7.23301f;              // Newtons => Poundals
const double Force::PD2N = (1 / Force::N2PD);   // Poundals => Newtons

//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Force::Force()
{
    STANDARD_CONSTRUCTOR()
}

Force::Force(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Force::copyData(const Force& org, const bool)
{
   BaseClass::copyData(org);
   val = fromForce(org.toForce());;
}


//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Force)


//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
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

EMPTY_COPYDATA(Newtons)
EMPTY_DELETEDATA(Newtons)


//==============================================================================
// Kilonewtons() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloNewtons, "KiloNewtons")
EMPTY_SERIALIZER(KiloNewtons)

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

EMPTY_COPYDATA(KiloNewtons)
EMPTY_DELETEDATA(KiloNewtons)


//==============================================================================
// PoundForces() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(PoundForces, "PoundForces")
EMPTY_SERIALIZER(PoundForces)

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

EMPTY_COPYDATA(PoundForces)
EMPTY_DELETEDATA(PoundForces)


//==============================================================================
// Poundals() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Poundals, "Poundals")
EMPTY_SERIALIZER(Poundals)

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

EMPTY_COPYDATA(Poundals)
EMPTY_DELETEDATA(Poundals)


}
}

