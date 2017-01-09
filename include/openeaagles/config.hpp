// =============================================================================
// Main configuration file for setting framework parameters
//
// Edit as needed, or redefine using compiler command line options
//
// Used within C++ source code and EDL/EPP files
// Important: only define C pre-processor directives in this file
//
// =============================================================================
#ifndef __oe_config_H__
#define __oe_config_H__

// OpenEaagles release version number (16.0X)
#ifndef OE_VERSION
#define OE_VERSION                         160X
#endif

// Max number of interval timers (see Timers.h)
#ifndef OE_CONFIG_MAX_INTERVAL_TIMERS
#define OE_CONFIG_MAX_INTERVAL_TIMERS      500
#endif

// Max number of "player's of interest" (see Gimbal.hpp)
#ifndef OE_CONFIG_MAX_PLAYERS_OF_INTEREST
#define OE_CONFIG_MAX_PLAYERS_OF_INTEREST  4000
#endif

// Max size of the RF emission queues (see RfSystem.hpp)
#ifndef OE_CONFIG_RF_MAX_EMISSIONS
#define OE_CONFIG_RF_MAX_EMISSIONS         800
#endif

// Max tracks (see TrackManager.h)
#ifndef OE_CONFIG_MAX_TRACKS
#define OE_CONFIG_MAX_TRACKS               200
#endif

// Max number of radar reports (see Radar.hpp and TrackManager.hpp)
#ifndef OE_CONFIG_MAX_REPORTS
#define OE_CONFIG_MAX_REPORTS              200
#endif

// Max number of networked entites (see NetIO.h)
#ifndef OE_CONFIG_MAX_NETIO_ENTITIES
#define OE_CONFIG_MAX_NETIO_ENTITIES       5000
#endif

// Max number of networked entity types (see NetIO.hpp)
#ifndef OE_CONFIG_MAX_NETIO_ENTITY_TYPES
#define OE_CONFIG_MAX_NETIO_ENTITY_TYPES   1000
#endif

//  Max number of new, outgoing players published per frame (see NetIO.hpp)
#ifndef OE_CONFIG_MAX_NETIO_NEW_OUTGOING
#define OE_CONFIG_MAX_NETIO_NEW_OUTGOING   150
#endif

#endif
