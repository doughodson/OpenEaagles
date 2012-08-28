// =============================================================================
// Main configuration file for setting framework parameters
//
// Edit as needed, or redefine using compiler command line options
//
// Used within C++ source code and input files
// Important: only define C pre-processor directives in this file
//
// =============================================================================
#ifndef __Eaagles_Config_H__
#define __Eaagles_Config_H__

// Max number of 'registered' classes (see Object.h)
#ifndef EAAGLES_CONFIG_MAX_CLASSES
#define EAAGLES_CONFIG_MAX_CLASSES              1000
#endif

// Max number of interval timers (see Timers.h)
#ifndef EAAGLES_CONFIG_MAX_INTERVAL_TIMERS
#define EAAGLES_CONFIG_MAX_INTERVAL_TIMERS      500
#endif

// Max number of "player's of interest" (see Gimbal.h)
#ifndef EAAGLES_CONFIG_MAX_PLAYERS_OF_INTEREST
#define EAAGLES_CONFIG_MAX_PLAYERS_OF_INTEREST  4000
#endif

// Max size of the RF emission queues (see RfSystem.h)
#ifndef EAAGLES_CONFIG_RF_MAX_EMISSIONS
#define EAAGLES_CONFIG_RF_MAX_EMISSIONS         800
#endif

// Max tracks (see TrackManager.h)
#ifndef EAAGLES_CONFIG_MAX_TRACKS
#define EAAGLES_CONFIG_MAX_TRACKS               200
#endif

// Max number of radar reports (see Radar.h and TrackManager.h)
#ifndef EAAGLES_CONFIG_MAX_REPORTS
#define EAAGLES_CONFIG_MAX_REPORTS              200
#endif

// Max number of networked entites (see NetIO.h)
#ifndef EAAGLES_CONFIG_MAX_NETIO_ENTITIES
#define EAAGLES_CONFIG_MAX_NETIO_ENTITIES       5000
#endif

// Max number of networked entity types (see NetIO.h)
#ifndef EAAGLES_CONFIG_MAX_NETIO_ENTITY_TYPES
#define EAAGLES_CONFIG_MAX_NETIO_ENTITY_TYPES   1000
#endif

//  Max number of new, outgoing players published per frame (see NetIO.h)
#ifndef EAAGLES_CONFIG_MAX_NETIO_NEW_OUTGOING
#define EAAGLES_CONFIG_MAX_NETIO_NEW_OUTGOING   150
#endif


//------------------------------------------------------------------------------
// Configure LCreal
//------------------------------------------------------------------------------
namespace Eaagles {

// ---
// Define LCreal as double only
//    Because of the 64 bit systems, the LCreal will be removed in a future
//    release.  For now it's defined only as double.
// ---
#define LCREAL_DOUBLE
typedef double LCreal;

// ---
// To avoid warnings, define math functions to use with LCreal.
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
