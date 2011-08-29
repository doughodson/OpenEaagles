// Forces.cpp: implementation of the Forces class.
// Forces, Newtons, KiloNewtons, PoundForces, Poundals
//////////////////////////////////////////////////////////////////////

#include "openeaagles/basic/units/Forces.h"
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/SlotTable.h"

namespace Eaagles {
namespace Basic {

//////////////////////////////////////////////////////////////////////
// Force() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Force, "Force")
EMPTY_SLOTTABLE(Force)

// Conversion constants
const LCreal Force::N2KN = 0.001f;                // Newtons => KiloNewtons
const LCreal Force::KN2N = (1 / Force::N2KN);   // KiloNewtons => Newtons
const LCreal Force::N2PF = 0.224809f;             // Newtons => PoundForces
const LCreal Force::PF2N = (1 / Force::N2PF);   // PoundForces => Newtons
const LCreal Force::N2PD = 7.23301f;              // Newtons => Poundals
const LCreal Force::PD2N = (1 / Force::N2PD);   // Poundals => Newtons

//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Force::Force()
{
    STANDARD_CONSTRUCTOR()
}

Force::Force(const LCreal value) : Number(value)
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
// Newtons() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Newtons, "Newtons")
EMPTY_SERIALIZER(Newtons)

Newtons::Newtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const LCreal value) : Force(value)
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

KiloNewtons::KiloNewtons(const LCreal value) : Force(value)
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


PoundForces::PoundForces(const LCreal value) : Force(value)
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

Poundals::Poundals(const LCreal value) : Force(value)
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


} // End Basic namespace
} // End Eaagles namespace

