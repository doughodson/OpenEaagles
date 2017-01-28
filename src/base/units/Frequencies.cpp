//------------------------------------------------------------------------------
// Frequencies, Hertz
//------------------------------------------------------------------------------
#include "openeaagles/base/units/Frequencies.hpp"

#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Frequency --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Frequency, "AbstractFrequency")
EMPTY_SLOTTABLE(Frequency)
EMPTY_DELETEDATA(Frequency)

Frequency::Frequency() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Frequency::Frequency(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Frequency::copyData(const Frequency& org, const bool)
{
   BaseClass::copyData(org);
   val = fromFrequency(org.toFrequency());;
}

std::ostream& Frequency::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// Hertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Hertz, "Hertz")
EMPTY_SERIALIZER(Hertz)
EMPTY_COPYDATA(Hertz)
EMPTY_DELETEDATA(Hertz)

Hertz::Hertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// KiloHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloHertz, "KiloHertz")
EMPTY_SERIALIZER(KiloHertz)
EMPTY_COPYDATA(KiloHertz)
EMPTY_DELETEDATA(KiloHertz)

KiloHertz::KiloHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// MegaHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MegaHertz, "MegaHertz")
EMPTY_SERIALIZER(MegaHertz)
EMPTY_COPYDATA(MegaHertz)
EMPTY_DELETEDATA(MegaHertz)

MegaHertz::MegaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// GigaHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GigaHertz, "GigaHertz")
EMPTY_SERIALIZER(GigaHertz)
EMPTY_COPYDATA(GigaHertz)
EMPTY_DELETEDATA(GigaHertz)

GigaHertz::GigaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// TeraHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TeraHertz, "TeraHertz")
EMPTY_SERIALIZER(TeraHertz)
EMPTY_COPYDATA(TeraHertz)
EMPTY_DELETEDATA(TeraHertz)

TeraHertz::TeraHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
