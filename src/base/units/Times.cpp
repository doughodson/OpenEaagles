//------------------------------------------------------------------------------
// Time, Seconds, MilliSeconds, Minutes, Hours, Days
//------------------------------------------------------------------------------
#include "openeaagles/base/units/Times.hpp"
#include <iostream>

namespace oe {
namespace base {

//==============================================================================
// Time --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Time, "AbstractTime")
EMPTY_SLOTTABLE(Time)
EMPTY_DELETEDATA(Time)

Time::Time() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Time::Time(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Time::copyData(const Time& org, const bool)
{
   BaseClass::copyData(org);
   val = fromTime(org.toTime());;
}

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
EMPTY_COPYDATA(Seconds)
EMPTY_DELETEDATA(Seconds)

Seconds::Seconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Seconds::Seconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// MilliSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilliSeconds, "MilliSeconds")
EMPTY_SERIALIZER(MilliSeconds)
EMPTY_COPYDATA(MilliSeconds)
EMPTY_DELETEDATA(MilliSeconds)

MilliSeconds::MilliSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliSeconds::MilliSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// MicroSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MicroSeconds, "MicroSeconds")
EMPTY_SERIALIZER(MicroSeconds)
EMPTY_COPYDATA(MicroSeconds)
EMPTY_DELETEDATA(MicroSeconds)

MicroSeconds::MicroSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroSeconds::MicroSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// NanoSeconds --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NanoSeconds, "NanoSeconds")
EMPTY_SERIALIZER(NanoSeconds)
EMPTY_COPYDATA(NanoSeconds)
EMPTY_DELETEDATA(NanoSeconds)

NanoSeconds::NanoSeconds() : Time()
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

NanoSeconds::NanoSeconds(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Minutes --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Minutes, "Minutes")
EMPTY_SERIALIZER(Minutes)
EMPTY_COPYDATA(Minutes)
EMPTY_DELETEDATA(Minutes)

Minutes::Minutes() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Minutes::Minutes(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Hours --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Hours, "Hours")
EMPTY_SERIALIZER(Hours)
EMPTY_COPYDATA(Hours)
EMPTY_DELETEDATA(Hours)

Hours::Hours() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Hours::Hours(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Days --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Days, "Days")
EMPTY_SERIALIZER(Days)
EMPTY_COPYDATA(Days)
EMPTY_DELETEDATA(Days)

Days::Days() : Time()
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const double value) : Time(value)
{
    STANDARD_CONSTRUCTOR()
}

Days::Days(const Time& org) : Time()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

}
}
