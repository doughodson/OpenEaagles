// =============================================================================
// Main configuration file for setting framework parameters
//
// Used within C++ source code and input files
// Important: only define C pre-processor directives in this file
// =============================================================================
#ifndef __Eaagles_Config_H__
#define __Eaagles_Config_H__

// Max number of interval timers (see Timers.h)
#define EAAGLES_CONFIG_MAX_INTERVAL_TIMERS      40


//------------------------------------------------------------------------------
// Configure LCreal as double or float
//------------------------------------------------------------------------------
namespace Eaagles {

// Defined the following macro to use double for LCreal
#define LCREAL_DOUBLE

// ---
// Define LCreal
// ---
#ifdef LCREAL_DOUBLE
   typedef double LCreal;
#else
   typedef float  LCreal;
#endif

// ---
// To avoid warnings, define math functions to use with LCreal (double or float)
// ---
#ifdef LCREAL_DOUBLE
    // ---
    // When double use standard C functions
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
#else
    // Standard C functions
    #define lcSqrt      ::sqrtf
    #define lcSin       ::sinf
    #define lcCos       ::cosf  
    #define lcTan       ::tanf
    #define lcAtan      ::atanf
    #define lcAtan2     ::atan2f
    #define lcAcos      ::acosf
    #define lcAbs       ::fabsf
    #define lcFmod      ::fmodf
    #define lcPow       ::powf
    #define lcExp       ::expf
    #define lcLog       ::logf
    #define lcLog10     ::log10f
    #define lcAepcDeg   Basic::Angle::aepcfDeg   // needs basic/units/Angles.h
    #define lcAepcRad   Basic::Angle::aepcfRad   // needs basic/units/Angles.h
#endif

} // end namespace Eaagles

#endif
