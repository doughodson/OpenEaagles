//------------------------------------------------------------------------------
// Recorder Event ID (REID) Tokens
//
// Notes:
//    1) Users defined token IDs start at 1000.
//
//    2) Token IDs 500 to 999 are reserved
//
//    3) This file is used by both C++ and Eaagles input files, so only use
//       C pre-processor directives in this configuration file.
//
//    4) P1 to P4 are the required objects passed to the SAMPLE macro, and
//       V1 to V4 are the required values passed to the SAMPLE macros.
//       (see "openeaagles/simulation/DataRecorder.h")
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Id_Tokens_H__
#define __Eaagles_Simulation_Id_Tokens_H__

// Data recorder and handler messages <internal use only>
#define REID_END_OF_DATA          0    // Last data record message <can not be disabled>
#define REID_FILE_ID              1    // File identifier (header) message
#define REID_UNHANDLED_ID_TOKEN   2    // Unhandled or unknown ID token
#define REID_RESET_EVENT          3    // Simulation reset event message

// Simulation message
#define REID_MARKER              21    // Data marker message; V1 => id; V2 => source ID
#define REID_DI_EVENT            22    // Discrete input (switch, etc.) event message; V1 => id; V2 => source ID; V3 => value
#define REID_AI_EVENT            23    // Analog input (joystick, etc.) event message; V1 => id; V2 => source ID; V3 => value

// Player data messages
#define REID_NEW_PLAYER          41    // New Player message; P1 => (new player)
#define REID_PLAYER_REMOVED      42    // Player removed message; P1 => (player)
#define REID_PLAYER_DATA         43    // Record player data message; P1 => (player)
#define REID_PLAYER_DAMAGED      44    // Player damaged by detonation; P1 => (player); P2 => (weapon)
#define REID_PLAYER_COLLISION    45    // Player collision message; P1 => (player); P2 => (other player)
#define REID_PLAYER_CRASH        46    // Player crash message; P1 => (player)
#define REID_PLAYER_KILLED       47    // Player killed message; P1 => (player); P2 => (shooter)

// Weapon data messages
#define REID_WEAPON_RELEASED     61    // Weapon Released message; P1 => (weapon); P2 => (shooter); P3 => (tgt)
#define REID_WEAPON_HUNG         62    // Weapon Hung (hung store) message; P1 => (weapon); P2 => (shooter); P3 => (tgt)
#define REID_WEAPON_DETONATION   63    // Weapon Detonation message; P1 => (weapon); P2 => (shooter); P3 => (tgt)
                                       //                            V1 => detonation type id; V2 => miss distance

#define REID_GUN_FIRED           64    // Gun Fired message; P1 => (shooter); v[0] => rounds

// Track data messages
#define REID_NEW_TRACK           81    // New Track message; P1 => (player); P2 => (track)
#define REID_TRACK_REMOVED       82    // Track Removed message; P1 => (player); P2 => (track)
#define REID_TRACK_DATA          83    // Record Track Data message; P1 => (player); P2 => (track)

// Reserved messages
#define REID_FIRST_RESERVED      500   // First reserved ID token
#define REID_LAST_RESERVED       999   // Last reserved ID token

// User defined messages
#define REID_FIRST_USER_EVENT    1000  // First user defined event
#define REID_LAST_USER_EVENT     9999  // Last user defined event

#endif

