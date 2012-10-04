// =============================================================================
// Main configuration file for setting framework parameters
//
// Used within C++ source code and input files
// Important: only define C pre-processor directives in this file
// =============================================================================
#ifndef __Eaagles_Config_H__
#define __Eaagles_Config_H__

// Max number of interval timers (see Timers.h)
#define EAAGLES_CONFIG_MAX_INTERVAL_TIMERS      400


//------------------------------------------------------------------------------
// Configure LCreal as double or float
//------------------------------------------------------------------------------
namespace Eaagles {

// ---
// Define LCreal as double only
//    Because of the 64 bit systems, the LCreal is being removed in a future
//    release.  For now it's defined only as double.
// ---
#define LCREAL_DOUBLE
typedef double LCreal;

// ---
// To avoid warnings, define math functions to use with LCreal (double or float)
// Since LCreal is deprecated and set only to double, we're defining only the
// double functions.
// ---
#define lcSqrt      ::sqrt
#define lcSin       ::sin
#define lcCos       ::cos
#define lcTan       ::tan
#define lcAtan      ::atan
#define lcAtan2     ::atan2
#define lcAcos      ::acos
#define lcAbs       ::fabs
#define lcFmod      ::fmod
#define lcPow       ::pow
#define lcExp       ::exp
#define lcLog       ::log
#define lcLog10     ::log10
#define lcAepcDeg   Basic::Angle::aepcdDeg   // needs basic/units/Angles.h
#define lcAepcRad   Basic::Angle::aepcdRad   // needs basic/units/Angles.h

} // end namespace Eaagles

#endif
