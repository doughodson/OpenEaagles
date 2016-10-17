
#ifndef __oe_base_util_constants_H__
#define __oe_base_util_constants_H__

#include <limits>

namespace oe {
namespace base {

// ---
// Make sure PI is undefined
// ---
#ifdef PI
#undef PI
#endif
const double PI = 3.1415926535897932384626433832795;
const double TWO_PI = PI + PI;

// ---
// Standard constants ----
//  Source: National Institute of Science & Technology (NIST) web site
//    Speed of light:       299,792,458 m/s
//    Boltzmann constant:   1.3806503E-23 (Joules/Kelvin)
//                          0.0000024E-23 (uncertainty)
// ---
const double LIGHTSPEED = 299792458.0;
const double BOLTZMANN = 1.3806503e-23;

// One "G" to feet per second per second
//  [This is an estimated average.
//  [One actual "G" is a function of latitude, longitude, and other exotic parameters.]
const double ETHG = 32.16;
const double ETHGM = ETHG * 0.3048;    // meters

// Convert Barometric Pressure (Inches Mercury to/from Millibars)
// This is an estimate.
const double INCH2MBAR = (1013.2 / 29.92);     // LDB -  Inches -> Mbars
const double MBAR2INCH = (1.0 / INCH2MBAR);     // LDB -  Mbars -> Inches

// Earth gravitational parameter.
// Units: meters cubed per seconds squared.
// 3986004.418E8 meters^3/sec^2.
const double MuM3perS2 = 3986004.418E8;

// ---
// default initial values for various readouts
// ---
const double UNDEFINED_VALUE = std::numeric_limits<float>::min();
const double INIT_VALUE = 0.0;
const int    INIT_INT_VALUE = 0;
const int    INIT_SELECT_VALUE = 1;
const double INIT_TIME_VALUE = 0.0;
const double INIT_DIRECTION_VALUE = 0.0;
const double INIT_LATITUDE_VALUE = 0.0;
const double INIT_LONGITUDE_VALUE = 0.0;

}
}

#endif
