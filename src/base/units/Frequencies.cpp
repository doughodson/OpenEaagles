//------------------------------------------------------------------------------
// Frequencies, Hertz
//------------------------------------------------------------------------------
#include "openeaagles/base/units/Frequencies.h"
#include "openeaagles/base/osg/Math"
#include "openeaagles/base/SlotTable.h"

namespace oe {
namespace base {


//==============================================================================
// Frequency --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Frequency, "Frequency")
EMPTY_SLOTTABLE(Frequency)

// Conversion constants
const double Frequency::KHz2Hz = 1000.0f;                   // KiloHertz => Hertz
const double Frequency::Hz2KHz = (1.0f / 1000.0f);          // Hertz => KiloHertz

const double Frequency::MHz2Hz = 1000000.0f;                // MegaHertz => Hertz
const double Frequency::Hz2MHz = (1.0f / 1000000.0f);       // Hertz => MegaHertz

const double Frequency::GHz2Hz = 1000000000.0f;             // GigaHertz => Hertz
const double Frequency::Hz2GHz = (1.0f / 1000000000.0f);    // Hertz => GigaHertz

const double Frequency::THz2Hz = 1000000000000.0f;          // TeraHertz => Hertz
const double Frequency::Hz2THz = (1.0f / 1000000000000.0f); // Hertz => TeraHertz

// ---
// constructors
// ---
Frequency::Frequency() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Frequency::Frequency(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

// ---
// copy data
// ---
void Frequency::copyData(const Frequency& org, const bool)
{
   BaseClass::copyData(org);
   val = fromFrequency(org.toFrequency());;
}

// ---
// delete data
// ---
EMPTY_DELETEDATA(Frequency)

// ---
// serialize() -- print functions
// ---
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

EMPTY_COPYDATA(Hertz)
EMPTY_DELETEDATA(Hertz)


//==============================================================================
// KiloHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloHertz, "KiloHertz")
EMPTY_SERIALIZER(KiloHertz)

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

EMPTY_COPYDATA(KiloHertz)
EMPTY_DELETEDATA(KiloHertz)

//==============================================================================
// MegaHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MegaHertz, "MegaHertz")
EMPTY_SERIALIZER(MegaHertz)

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

EMPTY_COPYDATA(MegaHertz)
EMPTY_DELETEDATA(MegaHertz)

//==============================================================================
// GigaHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(GigaHertz, "GigaHertz")
EMPTY_SERIALIZER(GigaHertz)

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

EMPTY_COPYDATA(GigaHertz)
EMPTY_DELETEDATA(GigaHertz)

//==============================================================================
// TeraHertz --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TeraHertz, "TeraHertz")
EMPTY_SERIALIZER(TeraHertz)

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

EMPTY_COPYDATA(TeraHertz)
EMPTY_DELETEDATA(TeraHertz)

} // End base namespace
}
