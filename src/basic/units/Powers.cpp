// Powers.cpp: implementation of the Power class.
// Powers, KiloWatts, Watts, Horsepower
//////////////////////////////////////////////////////////////////////

#include "openeaagles/basic/units/Powers.h"
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/SlotTable.h"

namespace Eaagles {
namespace Basic {

//////////////////////////////////////////////////////////////////////
// Power() --
//////////////////////////////////////////////////////////////////////
IMPLEMENT_ABSTRACT_SUBCLASS(Power, "Power")
EMPTY_SLOTTABLE(Power)

// Conversion constants
const LCreal Power::W2KW = 0.001f;                // Watts => KiloWatts
const LCreal Power::KW2W = (1 / Power::W2KW);   // KiloWatts => Watts
const LCreal Power::W2HP = 1341.0f;               // Watts => Horsepower
const LCreal Power::HP2W = (1 / Power::W2HP);   // Horsepower => Watts
const LCreal Power::W2MW = 1000.0f;               // Watts => MilliWatts
const LCreal Power::MW2W = (1 / Power::W2MW);   // MilliWatts => Watts

//------------------------------------------------------------------------------
//constructors
//------------------------------------------------------------------------------
Power::Power()
{
   STANDARD_CONSTRUCTOR()
}

Power::Power(const LCreal value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void Power::copyData(const Power& org, const bool)
{
   BaseClass::copyData(org);
   val = fromPower(org.toPower());;
}


//------------------------------------------------------------------------------
// deletedDataFunc() --
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(Power)


//------------------------------------------------------------------------------
// serialize()
//------------------------------------------------------------------------------
std::ostream& Power::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// KiloWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloWatts, "KiloWatts")
EMPTY_SERIALIZER(KiloWatts)

KiloWatts::KiloWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(KiloWatts)
EMPTY_DELETEDATA(KiloWatts)


//==============================================================================
// Watts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Watts, "Watts")
EMPTY_SERIALIZER(Watts)

Watts::Watts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Watts)
EMPTY_DELETEDATA(Watts)


//==============================================================================
// Horsepower() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Horsepower, "Horsepower")
EMPTY_SERIALIZER(Horsepower)

Horsepower::Horsepower() : Power()
{
    STANDARD_CONSTRUCTOR()
}


Horsepower::Horsepower(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Horsepower::Horsepower(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Horsepower)
EMPTY_DELETEDATA(Horsepower)

//==============================================================================
// DecibelWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelWatts, "DecibelWatts")
EMPTY_SERIALIZER(DecibelWatts)

DecibelWatts::DecibelWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(DecibelWatts)
EMPTY_DELETEDATA(DecibelWatts)

//==============================================================================
// MilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilliWatts, "MilliWatts")
EMPTY_SERIALIZER(MilliWatts)

MilliWatts::MilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(MilliWatts)
EMPTY_DELETEDATA(MilliWatts)


//==============================================================================
// DecibelMilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelMilliWatts, "DecibelMilliWatts")
EMPTY_SERIALIZER(DecibelMilliWatts)

DecibelMilliWatts::DecibelMilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const LCreal value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(DecibelMilliWatts)
EMPTY_DELETEDATA(DecibelMilliWatts)

} // End Basic namespace
} // End Eaagles namespace
