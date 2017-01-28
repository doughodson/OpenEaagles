
#include "openeaagles/base/units/Powers.hpp"
#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Power() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Power, "Power")
EMPTY_SLOTTABLE(Power)
EMPTY_DELETEDATA(Power)

Power::Power()
{
   STANDARD_CONSTRUCTOR()
}

Power::Power(const double value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

void Power::copyData(const Power& org, const bool)
{
   BaseClass::copyData(org);
   val = fromPower(org.toPower());;
}

std::ostream& Power::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// KiloWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloWatts, "KiloWatts")
EMPTY_SERIALIZER(KiloWatts)
EMPTY_COPYDATA(KiloWatts)
EMPTY_DELETEDATA(KiloWatts)

KiloWatts::KiloWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Watts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Watts, "Watts")
EMPTY_SERIALIZER(Watts)
EMPTY_COPYDATA(Watts)
EMPTY_DELETEDATA(Watts)

Watts::Watts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Horsepower() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Horsepower, "Horsepower")
EMPTY_SERIALIZER(Horsepower)
EMPTY_COPYDATA(Horsepower)
EMPTY_DELETEDATA(Horsepower)

Horsepower::Horsepower() : Power()
{
    STANDARD_CONSTRUCTOR()
}


Horsepower::Horsepower(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Horsepower::Horsepower(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// DecibelWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelWatts, "DecibelWatts")
EMPTY_SERIALIZER(DecibelWatts)
EMPTY_COPYDATA(DecibelWatts)
EMPTY_DELETEDATA(DecibelWatts)

DecibelWatts::DecibelWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// MilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilliWatts, "MilliWatts")
EMPTY_SERIALIZER(MilliWatts)
EMPTY_COPYDATA(MilliWatts)
EMPTY_DELETEDATA(MilliWatts)

MilliWatts::MilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// DecibelMilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelMilliWatts, "DecibelMilliWatts")
EMPTY_SERIALIZER(DecibelMilliWatts)
EMPTY_COPYDATA(DecibelMilliWatts)
EMPTY_DELETEDATA(DecibelMilliWatts)

DecibelMilliWatts::DecibelMilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

}
}
