// Energies.cpp: implementation of the Energy class.
// Energy, KiloWattHours, BTUs, Joules, FootPounds
//////////////////////////////////////////////////////////////////////

#include "openeaagles/base/units/Energies.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"

namespace oe {
namespace basic {


//////////////////////////////////////////////////////////////////////
// Energy() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Energy, "Energy")
EMPTY_SLOTTABLE(Energy)

// Conversion constants
const LCreal Energy::J2KWH = 0.0000002777777778f;     // Joules => KiloWattHours
const LCreal Energy::KWH2J = (1 / Energy::J2KWH);   // KiloWattHours => Joules
const LCreal Energy::J2BTU = 0.0009478f;              // Joules => BTUs
const LCreal Energy::BTU2J = (1 / Energy::J2BTU);   // BTUs => Joules
const LCreal Energy::J2FP = 0.7376f;                  // Joules => FootPounds
const LCreal Energy::FP2J = (1 / Energy::J2FP);     // FootPounds => Joules
const LCreal Energy::J2C = 0.2388888889f;             // Joules => Calories
const LCreal Energy::C2J = (1 / Energy::J2C);       // Calories => Joules


//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Energy::Energy()
{
    STANDARD_CONSTRUCTOR()
}

Energy::Energy(const LCreal value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Energy::copyData(const Energy& org, const bool)
{
   BaseClass::copyData(org);
   val = fromEnergy(org.toEnergy());;
}


//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Energy)


//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
std::ostream& Energy::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// KiloWattHours() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloWattHours, "KiloWattHours")
EMPTY_SERIALIZER(KiloWattHours)

KiloWattHours::KiloWattHours() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const LCreal value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(KiloWattHours)
EMPTY_DELETEDATA(KiloWattHours)


//==============================================================================
// BTUs() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(BTUs, "BTUs")
EMPTY_SERIALIZER(BTUs)

BTUs::BTUs() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const LCreal value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(BTUs)
EMPTY_DELETEDATA(BTUs)


//==============================================================================
// Calories() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Calories, "Calories")
EMPTY_SERIALIZER(Calories)

Calories::Calories() : Energy()
{
    STANDARD_CONSTRUCTOR()
}


Calories::Calories(const LCreal value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Calories::Calories(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Calories)
EMPTY_DELETEDATA(Calories)


//==============================================================================
// Joules() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Joules, "Joules")
EMPTY_SERIALIZER(Joules)

Joules::Joules() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const LCreal value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(Joules)
EMPTY_DELETEDATA(Joules)


//==============================================================================
// FootPounds() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FootPounds, "FootPounds")
EMPTY_SERIALIZER(FootPounds)

FootPounds::FootPounds() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const LCreal value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

EMPTY_COPYDATA(FootPounds)
EMPTY_DELETEDATA(FootPounds)

} // End basic namespace
} // End oe namespace

