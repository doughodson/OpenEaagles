//------------------------------------------------------------------------------
// Time, Seconds, MilliSeconds, Minutes, Hours, Days
//------------------------------------------------------------------------------
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/osg/Math"
#include "openeaagles/basic/SlotTable.h"

namespace oe {
namespace Basic {

//==============================================================================
// Time --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Time, "Time")
EMPTY_SLOTTABLE(Time)

// Conversion constants
const LCreal Time::S2NS = 1000000000.0f;           // Seconds => nanoseconds
const LCreal Time::NS2S = (1.0f / 1000000000.0f);  // nanoseconds => Seconds
const LCreal Time::S2US = 1000000.0f;              // Seconds => microseconds
const LCreal Time::US2S = (1.0f / 1000000.0f);     // microseconds => Seconds
const LCreal Time::S2MS = 1000.0f;                 // Seconds => MilliSeconds
const LCreal Time::MS2S = (1.0f / 1000.0f);        // MilliSeconds => Seconds
const LCreal Time::M2S  =  60.0f;                  // Minutes => Seconds
const LCreal Time::S2M  = (1.0f / 60.0f);          // Seconds => Minutes
const LCreal Time::H2S  = 3600.0f;                 // Hours => Seconds
const LCreal Time::S2H  = (1.0f / 3600.0f);        // Seconds => Hours
const LCreal Time::D2M  = 1440.0f;                 // Days => Minutes
const LCreal Time::M2D  = (1.0f / 1440.0f);        // Minutes => Days
const LCreal Time::D2S  = 86400.0f;                // Days => Seconds
const LCreal Time::S2D  = (1.0f / 86400.0f);       // Seconds => Days

//------------------------------------------------------------------------------
// constructors
//------------------------------------------------------------------------------
Time::Time() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Time::Time(const LCreal value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

// ---
// copy data
// ---
void Time::copyData(const Time& org, const bool)
{
   BaseClass::copyData(org);
   val = fromTime(org.toTime());;
}

// ---
// delete data
// ---
EMPTY_DELETEDATA(Time)

// ---
// serialize() -- print functions
// ---
std::ostream& Time::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
// Seconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Seconds, "Seconds")
EMPTY_SERIALIZER(Seconds)

Seconds::Seconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Seconds)
EMPTY_DELETEDATA(Seconds)


//==============================================================================
// MilliSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilliSeconds, "MilliSeconds")
EMPTY_SERIALIZER(MilliSeconds)

MilliSeconds::MilliSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(MilliSeconds)
EMPTY_DELETEDATA(MilliSeconds)

//==============================================================================
// MicroSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MicroSeconds, "MicroSeconds")
EMPTY_SERIALIZER(MicroSeconds)

MicroSeconds::MicroSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(MicroSeconds)
EMPTY_DELETEDATA(MicroSeconds)

//==============================================================================
// NanoSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NanoSeconds, "NanoSeconds")
EMPTY_SERIALIZER(NanoSeconds)

NanoSeconds::NanoSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(NanoSeconds)
EMPTY_DELETEDATA(NanoSeconds)


//==============================================================================
// Minutes --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Minutes, "Minutes")
EMPTY_SERIALIZER(Minutes)

Minutes::Minutes() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Minutes)
EMPTY_DELETEDATA(Minutes)


//==============================================================================
// Hours --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Hours, "Hours")
EMPTY_SERIALIZER(Hours)

Hours::Hours() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Hours)
EMPTY_DELETEDATA(Hours)


//==============================================================================
// Days --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Days, "Days")
EMPTY_SERIALIZER(Days)

Days::Days() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const LCreal value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

EMPTY_COPYDATA(Days)
EMPTY_DELETEDATA(Days)

} // End Basic namespace
} // End oe namespace
