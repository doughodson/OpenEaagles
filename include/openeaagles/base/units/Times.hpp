//------------------------------------------------------------------------------
// Classes:  Time, Seconds, MilliSeconds, MicroSeconds, NanoSeconds, Minutes, Hours, Days
// Base class:  Object -> Number -> Time
//              Object -> Number -> Time -> Seconds
//              Object -> Number -> Time -> MilliSeconds
//              Object -> Number -> Time -> MicroSeconds
//              Object -> Number -> Time -> NanoSeconds
//              Object -> Number -> Time -> Minutes
//              Object -> Number -> Time -> Hours
//              Object -> Number -> Time -> Days
//------------------------------------------------------------------------------

#ifndef __oe_base_Times_H__
#define __oe_base_Times_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/util/math_utils.hpp"
#include "openeaagles/base/util/time_utils.hpp"
#include <iostream>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: Time
// Description:  Base class for times.  Defined as a second which is equivalent
// to an instance of Seconds with its value equal to 1.0.
//
// Public methods (Defined in Time, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Time derived instance with an double.
//
//     set(const Time& n)
//        Sets, and converts if necessary, a Time derived instance with
//          another Time derived instance.
//
//     double convert(const Time& n)
//        Converts the value of an Time derived instance into
//        the units of another Time derived instance.
//
//      Output stream operator:  >>
//        ostream& operator<<(ostream& sout, const Time& n)
//        Sends "( <the Time derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Seconds, MilliSeconds, Minutes, Hours, Days):
//
//     double convertStatic(const Time& n)
//        Static function to convert the given Time derived
//        instance into the units of another Time derived instance.
//
//------------------------------------------------------------------------------
class Time : public Number
{
    DECLARE_SUBCLASS(Time, Number)

public:
    Time();
    Time(const double value);

    void set(const double v) { val = v; }
    void set(const Time& n)  { val = fromTime(n.toTime()); }

    virtual double toTime() const =0;
    virtual double fromTime(const double a) const =0;
    double convert(const Time& n)                        { return fromTime(n.toTime()); }
};

inline std::ostream& operator<<(std::ostream& sout, const Time& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class: Seconds
// Description:  An instance of Seconds with its value equal to 1.0 is one base unit for times.
//------------------------------------------------------------------------------
class Seconds : public Time
{
    DECLARE_SUBCLASS(Seconds, Time)

public:
    Seconds();
    Seconds(const double value);
    Seconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime(); }
    virtual double toTime() const override                  { return static_cast<double>(val); }
    virtual double fromTime(const double a) const override  { return a; }
};

//------------------------------------------------------------------------------
// Class:  MilliSeconds
// Description:  Seconds / 1000.0
//------------------------------------------------------------------------------
class MilliSeconds : public Time
{
    DECLARE_SUBCLASS(MilliSeconds, Time)

public:
    MilliSeconds();
    MilliSeconds(const double value);
    MilliSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * time::S2MS; }
    virtual double toTime() const override                  { return static_cast<double>(val * time::MS2S); }
    virtual double fromTime(const double a) const override  { return a * time::S2MS; }
};

//------------------------------------------------------------------------------
// Class:  MicroSeconds
// Description:  Seconds / 1000000.0
//------------------------------------------------------------------------------
class MicroSeconds : public Time
{
    DECLARE_SUBCLASS(MicroSeconds, Time)

public:
    MicroSeconds();
    MicroSeconds(const double value);
    MicroSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * time::S2US; }
    virtual double toTime() const override                  { return static_cast<double>(val * time::US2S); }
    virtual double fromTime(const double a) const override  { return a * time::S2US; }
};

//------------------------------------------------------------------------------
// Class:  NanoSeconds
// Description:  Seconds / 1000000000.0
//------------------------------------------------------------------------------
class NanoSeconds : public Time
{
    DECLARE_SUBCLASS(NanoSeconds, Time)

public:
    NanoSeconds();
    NanoSeconds(const double value);
    NanoSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * time::S2NS; }
    virtual double toTime() const override                  { return static_cast<double>(val * time::NS2S); }
    virtual double fromTime(const double a) const override  { return a * time::S2NS; }
};


//------------------------------------------------------------------------------
// Class:  Minutes
// Description:  Seconds * 60.0
//------------------------------------------------------------------------------
class Minutes : public Time
{
    DECLARE_SUBCLASS(Minutes, Time)

public:
    Minutes();
    Minutes(const double value);
    Minutes(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * time::S2M; }
    virtual double toTime() const override                  { return static_cast<double>(val * time::M2S); }
    virtual double fromTime(const double a) const override  { return a * time::S2M; }
};

//------------------------------------------------------------------------------
// Class:  Hours
// Description:  Seconds * 3600.0
//------------------------------------------------------------------------------
class Hours : public Time
{
    DECLARE_SUBCLASS(Hours, Time)

public:
    Hours();
    Hours(const double value);
    Hours(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * time::S2H; }
    virtual double toTime() const override                  { return static_cast<double>(val * time::H2S); }
    virtual double fromTime(const double a) const override  { return a * time::S2H; }
};

//------------------------------------------------------------------------------
// Class:  Days
// Description:  Seconds * 3600.0 * 24.0
//------------------------------------------------------------------------------
class Days : public Time
{
    DECLARE_SUBCLASS(Days, Time)

public:
    Days();
    Days(const double value);
    Days(const Time& org);

    static double convertStatic(const Time &n)             { return n.toTime() * time::S2D; }
    virtual double toTime() const override                 { return static_cast<double>(val * time::D2S); }
    virtual double fromTime(const double a) const override { return a * time::S2D; }
};

}
}

#endif
