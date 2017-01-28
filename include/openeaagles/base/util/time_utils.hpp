
#ifndef __oe_base_util_time_utils_H__
#define __oe_base_util_time_utils_H__

#include "openeaagles/base/util/math_utils.hpp"

namespace oe {
namespace base {
namespace time {

// conversion constants
const double S2NS = 1000000000.0f;           // Seconds => nanoseconds
const double NS2S = (1.0f / 1000000000.0f);  // nanoseconds => Seconds
const double S2US = 1000000.0f;              // Seconds => microseconds
const double US2S = (1.0f / 1000000.0f);     // microseconds => Seconds
const double S2MS = 1000.0f;                 // Seconds => MilliSeconds
const double MS2S = (1.0f / 1000.0f);        // MilliSeconds => Seconds
const double M2S  =  60.0f;                  // Minutes => Seconds
const double S2M  = (1.0f / 60.0f);          // Seconds => Minutes
const double H2S  = 3600.0f;                 // Hours => Seconds
const double S2H  = (1.0f / 3600.0f);        // Seconds => Hours
const double D2M  = 1440.0f;                 // Days => Minutes
const double M2D  = (1.0f / 1440.0f);        // Minutes => Days
const double D2S  = 86400.0f;                // Days => Seconds
const double S2D  = (1.0f / 86400.0f);       // Seconds => Days

// conversions between times
inline double SecondsToMilliSeconds(const double v)  { return v * S2MS; }
inline double MilliSecondsToSeconds(const double v)  { return v * MS2S; }

inline double MinutesToSeconds(const double v)       { return v * M2S; }
inline double SecondsToMinutes(const double v)       { return v * S2M; }

inline double HoursToMinutes(const double v)         { return (v * H2S) * S2M; }
inline double MinutesToHours(const double v)         { return (v * M2S) * S2H; }

inline double DaysToHours(const double v)            { return (v * D2S) * S2H; }
inline double HoursToDays(const double v)            { return (v * H2S) * S2D; }

// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void getHHMMSS(const double sec, int* const hh, int* const mm, int* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = nint(rem);
}

// getHHMMSS -- convert time in seconds to integer hours, minutes and seconds
inline void getHHMMSS(const double sec, int* const hh, int* const mm, double* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = rem;
}

// putHHMMSS -- convert integer hours, minutes and seconds to time in seconds
inline double putHHMMSS(const int hh, const int mm, const int ss)
{
    return static_cast<double>((3600.0f * hh) + (60.0f * mm) + ss);
}

}
}
}

#endif
