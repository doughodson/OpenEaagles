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
//
// Description:  Numbers as times -- seconds, milliseconds, minutes, hours,
//               or days.  Base unit for Time derivations is seconds
//
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
//     Conversion routines:
//        double SecondsToMilliSeconds(const double v) { return v * S2MS; }
//        double MilliSecondsToSeconds(const double v) { return v * MS2S; }
//        double MinutesToSeconds(const double v)      { return v * M2S; }
//        double SecondsToMinutes(const double v)      { return v * S2M; }
//        double HoursToMinutes(const double v)        { return (v * H2S) * S2M; }
//        double MinutesToHours(const double v)        { return (v * M2S) * S2H; }
//        double DaysToHours(const double v)           { return (v * D2S) * S2H; }
//        double HoursToDays(const double v)           { return (v * H2S) * S2D; }
//        Return the appropriate conversion factors.
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
#ifndef __oe_base_Times_H__
#define __oe_base_Times_H__

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/util/math_utils.hpp"

namespace oe {
namespace base {

// ----------------------------------------------------------------------------
// Defined Power Conversion Constants:
//
// These constants were obtained (and cross referenced) from the following
// website, and are assumed accurate as of 2/5/03.
// www.nist.gov - National Institute of Standard Technology
// ----------------------------------------------------------------------------
// Since all units are converted to or from KiloWatts, only those constants
// will be defined.
// ----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Class:  Time
// Base class:  Object -> Number -> Time
// Description:  Base class for times.  Defined as a second which is equivalent
// to an instance of Seconds with its value equal to 1.0.
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

    // Conversions between times
    static double SecondsToMilliSeconds(const double v)  { return v * S2MS; }
    static double MilliSecondsToSeconds(const double v)  { return v * MS2S; }

    static double MinutesToSeconds(const double v)       { return v * M2S; }
    static double SecondsToMinutes(const double v)       { return v * S2M; }

    static double HoursToMinutes(const double v)         { return (v * H2S) * S2M; }
    static double MinutesToHours(const double v)         { return (v * M2S) * S2H; }

    static double DaysToHours(const double v)            { return (v * D2S) * S2H; }
    static double HoursToDays(const double v)            { return (v * H2S) * S2D; }

    // Convert integer hours, minutes and seconds to time in seconds
    static double putHHMMSS(const int hh, const int mm, const int ss);

    // Convert time in seconds to integer hours, minutes and seconds
    static void getHHMMSS(const double sec, int* const hh, int* const mm, int* const ss);
    static void getHHMMSS(const double sec, int* const hh, int* const mm, double* const ss);

    // Conversion constants
    static const double S2NS;    // Seconds to nanoseconds
    static const double NS2S;    // Nanoseconds to seconds
    static const double S2US;    // Seconds to microseconds
    static const double US2S;    // Microseconds to seconds
    static const double S2MS;    // Seconds to milliseconds
    static const double MS2S;    // Milliseconds to seconds
    static const double S2M;     // Seconds to minutes
    static const double M2S;     // Minutes to seconds
    static const double S2H;     // Seconds to hours
    static const double H2S;     // Hours to seconds
    static const double M2D;     // Minutes to days
    static const double D2M;     // Days to minutes
    static const double S2D;     // Seconds to days
    static const double D2S;     // Days to seconds
};


inline std::ostream& operator<<(std::ostream& sout, const Time& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }


// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void Time::getHHMMSS(const double sec, int* const hh, int* const mm, int* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = nint(rem);
}

// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void Time::getHHMMSS(const double sec, int* const hh, int* const mm, double* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = rem;
}

// putHHMMSS -- convert integer hours, minutes and seconds to time in seconds
inline double Time::putHHMMSS(const int hh, const int mm, const int ss)
{
    return static_cast<double>((3600.0f * hh) + (60.0f * mm) + ss);
}

//------------------------------------------------------------------------------
// Class:  Seconds // Base class:  Object -> Number -> Time -> Seconds
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
// Base class:  Object -> Number -> Time -> MilliSeconds
// Description:  Seconds / 1000.0
//------------------------------------------------------------------------------
class MilliSeconds : public Time
{
    DECLARE_SUBCLASS(MilliSeconds, Time)

public:
    MilliSeconds();
    MilliSeconds(const double value);
    MilliSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * S2MS; }
    virtual double toTime() const override                  { return static_cast<double>(val * MS2S); }
    virtual double fromTime(const double a) const override  { return a * S2MS; }
};


//------------------------------------------------------------------------------
// Class:  MicroSeconds
// Base class:  Object -> Number -> Time -> MicroSeconds
// Description:  Seconds / 1000000.0
//------------------------------------------------------------------------------
class MicroSeconds : public Time
{
    DECLARE_SUBCLASS(MicroSeconds, Time)

public:
    MicroSeconds();
    MicroSeconds(const double value);
    MicroSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * S2US; }
    virtual double toTime() const override                  { return static_cast<double>(val * US2S); }
    virtual double fromTime(const double a) const override  { return a * S2US; }
};


//------------------------------------------------------------------------------
// Class:  NanoSeconds
// Base class:  Object -> Number -> Time -> NanoSeconds
// Description:  Seconds / 1000000000.0
//------------------------------------------------------------------------------
class NanoSeconds : public Time
{
    DECLARE_SUBCLASS(NanoSeconds, Time)

public:
    NanoSeconds();
    NanoSeconds(const double value);
    NanoSeconds(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * S2NS; }
    virtual double toTime() const override                  { return static_cast<double>(val * NS2S); }
    virtual double fromTime(const double a) const override  { return a * S2NS; }
};


//------------------------------------------------------------------------------
// Class:  Minutes
// Base class:  Object -> Number -> Time -> Minutes
// Description:  Seconds * 60.0
//------------------------------------------------------------------------------
class Minutes : public Time
{
    DECLARE_SUBCLASS(Minutes, Time)

public:
    Minutes();
    Minutes(const double value);
    Minutes(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * S2M; }
    virtual double toTime() const override                  { return static_cast<double>(val * M2S); }
    virtual double fromTime(const double a) const override  { return a * S2M; }
};

//------------------------------------------------------------------------------
// Class:  Hours
// Base class:  Object -> Number -> Time -> Hours
// Description:  Seconds * 3600.0
//------------------------------------------------------------------------------
class Hours : public Time
{
    DECLARE_SUBCLASS(Hours, Time)

public:
    Hours();
    Hours(const double value);
    Hours(const Time& org);

    static double convertStatic(const Time &n)              { return n.toTime() * S2H; }
    virtual double toTime() const override                  { return static_cast<double>(val * H2S); }
    virtual double fromTime(const double a) const override  { return a * S2H; }
};

//------------------------------------------------------------------------------
// Class:  Days
// Base class:  Object -> Number -> Time -> Days
// Description:  Seconds * 3600.0 * 24.0
//------------------------------------------------------------------------------
class Days : public Time
{
    DECLARE_SUBCLASS(Days, Time)

public:
    Days();
    Days(const double value);
    Days(const Time& org);

    static double convertStatic(const Time &n)             { return n.toTime() * S2D; }
    virtual double toTime() const override                 { return static_cast<double>(val * D2S); }
    virtual double fromTime(const double a) const override { return a * S2D; }
};

}
}

#endif
