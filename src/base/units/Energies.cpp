
#include "openeaagles/base/units/Energies.hpp"

#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Energy() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Energy, "AbstractEnergy")
EMPTY_SLOTTABLE(Energy)
EMPTY_DELETEDATA(Energy)

Energy::Energy()
{
    STANDARD_CONSTRUCTOR()
}

Energy::Energy(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Energy::copyData(const Energy& org, const bool)
{
   BaseClass::copyData(org);
   val = fromEnergy(org.toEnergy());;
}

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
EMPTY_COPYDATA(KiloWattHours)
EMPTY_DELETEDATA(KiloWattHours)

KiloWattHours::KiloWattHours() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// BTUs() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(BTUs, "BTUs")
EMPTY_SERIALIZER(BTUs)
EMPTY_COPYDATA(BTUs)
EMPTY_DELETEDATA(BTUs)

BTUs::BTUs() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Calories() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Calories, "Calories")
EMPTY_SERIALIZER(Calories)
EMPTY_COPYDATA(Calories)
EMPTY_DELETEDATA(Calories)

Calories::Calories() : Energy()
{
    STANDARD_CONSTRUCTOR()
}


Calories::Calories(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Calories::Calories(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Joules() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Joules, "Joules")
EMPTY_SERIALIZER(Joules)
EMPTY_COPYDATA(Joules)
EMPTY_DELETEDATA(Joules)

Joules::Joules() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// FootPounds() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(FootPounds, "FootPounds")
EMPTY_SERIALIZER(FootPounds)
EMPTY_COPYDATA(FootPounds)
EMPTY_DELETEDATA(FootPounds)

FootPounds::FootPounds() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

