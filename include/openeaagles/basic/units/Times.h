//------------------------------------------------------------------------------
// Classes:  Time, Seconds, MilliSeconds, Minutes, Hours, Days
// Base class:  Object -> Number -> Time
//              Object -> Number -> Time -> Seconds
//              Object -> Number -> Time -> MilliSeconds
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
//	    set(const LCreal v)
//		    Sets an Time derived instance with an LCreal.
//
//	    set(const Time& n)
//		    Sets, and converts if necessary, a Time derived instance with
//          another Time derived instance.
//
//	    LCreal convert(const Time& n)
//		    Converts the value of an Time derived instance into
//		    the units of another Time derived instance.
//
//	    Conversion routines:
//		    LCreal SecondsToMilliSeconds(const LCreal v) { return v * S2MS; }
//		    LCreal MilliSecondsToSeconds(const LCreal v) { return v * MS2S; }
//		    LCreal MinutesToSeconds(const LCreal v)      { return v * M2S; }
//		    LCreal SecondsToMinutes(const LCreal v)      { return v * S2M; }
//		    LCreal HoursToMinutes(const LCreal v)        { return (v * H2S) * S2M; }
//		    LCreal MinutesToHours(const LCreal v)        { return (v * M2S) * S2H; }
//		    LCreal DaysToHours(const LCreal v)           { return (v * D2S) * S2H; }
//		    LCreal HoursToDays(const LCreal v)           { return (v * H2S) * S2D; }
//			 Return the appropriate conversion factors.
//
//      Output stream operator:  >> 
//		    ostream& operator<<(ostream& sout, const Time& n)
//			 Sends "( <the Time derived instance class name and value> )"
//			 to the output stream.
//
//
// Public methods (For classes:  Seconds, MilliSeconds, Minutes, Hours, Days):                           
//
//	    LCreal convertStatic(const Time& n)
//		    Static function to convert the given Time derived 
//		    instance into the units of another Time derived instance.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Times_H__
#define __Eaagles_Basic_Times_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

// ----------------------------------------------------------------------------
// Defined Power Conversion Constants:
// 
// These constants were obtained (and cross referenced) from the following
// website, and are assumed accurate as of 2/5/03.  
// www.nist.gov - National Institute of Standard Technology
// ----------------------------------------------------------------------------
// Since all units are converted to or from KiloWatts, only those constants
// will be defined.
//////////////////////////////////////////////////////


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
    Time(const LCreal value);

    void set(const LCreal v) { val = v; }
    void set(const Time& n)  { val = fromTime(n.toTime()); }

    virtual LCreal toTime() const =0;
    virtual LCreal fromTime(const LCreal a) const =0;  
    LCreal convert(const Time& n)
	{ return fromTime(n.toTime()); }

    // Conversions between times
    static LCreal SecondsToMilliSeconds(const LCreal v)  { return v * S2MS; }
    static LCreal MilliSecondsToSeconds(const LCreal v)  { return v * MS2S; }

    static LCreal MinutesToSeconds(const LCreal v)       { return v * M2S; }
    static LCreal SecondsToMinutes(const LCreal v)       { return v * S2M; }

    static LCreal HoursToMinutes(const LCreal v)         { return (v * H2S) * S2M; }
    static LCreal MinutesToHours(const LCreal v)         { return (v * M2S) * S2H; }

    static LCreal DaysToHours(const LCreal v)            { return (v * D2S) * S2H; }
    static LCreal HoursToDays(const LCreal v)            { return (v * H2S) * S2D; }

    // Convert integer hours, minutes and seconds to time in secounds
    static LCreal putHHMMSS(const int hh, const int mm, const int ss);

    // Convert time in seconds to integer hours, minutes and seconds
    static void getHHMMSS(const LCreal sec, int* const hh, int* const mm, int* const ss);
    static void getHHMMSS(const LCreal sec, int* const hh, int* const mm, LCreal* const ss);

    // Conversion constants
    static const LCreal S2NS;    // Seconds to nanoseconds
    static const LCreal NS2S;    // Nanoseconds to seconds
    static const LCreal S2US;    // Seconds to microseconds
    static const LCreal US2S;    // Microseconds to seconds
    static const LCreal S2MS;    // Seconds to milliseconds
    static const LCreal MS2S;    // Milliseconds to seconds
    static const LCreal S2M;     // Seconds to minutes
    static const LCreal M2S;     // Minutes to seconds
    static const LCreal S2H;     // Seconds to hours
    static const LCreal H2S;     // Hours to seconds
	static const LCreal M2D;     // Minutes to days
    static const LCreal D2M;     // Days to minutes
    static const LCreal S2D;     // Seconds to days
    static const LCreal D2S;     // Days to seconds
};


inline std::ostream& operator<<(std::ostream& sout, const Time& n)
	    { sout << "( " << n.getFormName() << " " << n.getReal() << " )"; return sout; }


// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void Time::getHHMMSS(const LCreal sec, int* const hh, int* const mm, int* const ss)
{
    LCreal rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= LCreal(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= LCreal(*mm) * 60.0f;
    *ss  = nint(rem);
}

// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void Time::getHHMMSS(const LCreal sec, int* const hh, int* const mm, LCreal* const ss)
{
    LCreal rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= LCreal(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= LCreal(*mm) * 60.0f;
    *ss  = rem;
}

// putHHMMSS -- convert integer hours, minutes and seconds to time in secounds
inline LCreal Time::putHHMMSS(const int hh, const int mm, const int ss)
{
    return LCreal((3600.0f * hh) + (60.0f * mm) + ss);
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
    Seconds(const LCreal value);
    Seconds(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime(); }
    virtual LCreal toTime() const                  { return (LCreal)(val); }
    virtual LCreal fromTime(const LCreal a) const  { return a; }
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
    MilliSeconds(const LCreal value);
    MilliSeconds(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime() * S2MS; }
    virtual LCreal toTime() const                  { return (LCreal)(val * MS2S); }
    virtual LCreal fromTime(const LCreal a) const  { return a * S2MS; }
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
    MicroSeconds(const LCreal value);
    MicroSeconds(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime() * S2US; }
    virtual LCreal toTime() const                  { return (LCreal)(val * US2S); }
    virtual LCreal fromTime(const LCreal a) const  { return a * S2US; }
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
    NanoSeconds(const LCreal value);
    NanoSeconds(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime() * S2NS; }
    virtual LCreal toTime() const                  { return (LCreal)(val * NS2S); }
    virtual LCreal fromTime(const LCreal a) const  { return a * S2NS; }
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
    Minutes(const LCreal value);
    Minutes(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime() * S2M; }
    virtual LCreal toTime() const                  { return (LCreal)(val * M2S); }
    virtual LCreal fromTime(const LCreal a) const  { return a * S2M; }
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
    Hours(const LCreal value);
    Hours(const Time& org);

    static LCreal convertStatic(const Time &n)     { return n.toTime() * S2H; }
    virtual LCreal toTime() const                  { return (LCreal)(val * H2S); }
    virtual LCreal fromTime(const LCreal a) const  { return a * S2H; }
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
    Days(const LCreal value);
    Days(const Time& org);

    static LCreal convertStatic(const Time &n)    { return n.toTime() * S2D; }
    virtual LCreal toTime() const                 { return (LCreal)(val * D2S); }
    virtual LCreal fromTime(const LCreal a) const { return a * S2D; }
};

} // End Basic namespace
} // End Eaagles namespace

#endif
