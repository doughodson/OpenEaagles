//==============================================================================
// Class DataRecorder
//==============================================================================

#include "openeaagles/recorder/DataRecorder.h"

#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/DataRecordHandle.h"
#include "openeaagles/recorder/DataRecord.pb.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/String.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {

IMPLEMENT_SUBCLASS(DataRecorder,"DataRecorder")
EMPTY_SERIALIZER(DataRecorder)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(DataRecorder)
   "outputHandler",     // 1)  Output handler
   "eventName",         // 2) Event (i.e., test, study, demo, exersize) name
   "application",       // 3) Application name (e.g., "mainSimExec")
   "caseNum",           // 4) Case id number (i.e., conditions)
   "missionNum",        // 5) Unique mission, sortie, scenario id number
   "subjectNum",        // 6) Subject id number
   "runNum",            // 7) Run or trial number
   "day",               // 8) Day of the month (1 .. 31))
   "month",             // 9) Month (1 .. 12)
   "year",              // 10) Year (e.g., 2010 or 10)
END_SLOTTABLE(DataRecorder)

BEGIN_SLOT_MAP(DataRecorder)
   ON_SLOT( 1, setOutputHandler,   OutputHandler)
   ON_SLOT( 2, setSlotEventName,   Basic::String)
   ON_SLOT( 3, setSlotApplication, Basic::String)
   ON_SLOT( 4, setSlotCaseNum,     Basic::Number)
   ON_SLOT( 5, setSlotMissionNum,  Basic::Number)
   ON_SLOT( 6, setSlotSubjectNum,  Basic::Number)
   ON_SLOT( 7, setSlotRunNum,      Basic::Number)
   ON_SLOT( 8, setSlotDay,         Basic::Number)
   ON_SLOT( 9, setSlotMonth,       Basic::Number)
   ON_SLOT( 10, setSlotYear,       Basic::Number)

END_SLOT_MAP()


//------------------------------------------------------------------------------
// DataRecorder dispatch table
//------------------------------------------------------------------------------
BEGIN_RECORDER_HANDLER_TABLE(DataRecorder)
   ON_RECORDER_EVENT_ID( REID_MARKER,            recordMarker )
   ON_RECORDER_EVENT_ID( REID_DI_EVENT,          recordAI )
   ON_RECORDER_EVENT_ID( REID_AI_EVENT,          recordDI )

   ON_RECORDER_EVENT_ID( REID_NEW_PLAYER,        recordNewPlayer )
   ON_RECORDER_EVENT_ID( REID_PLAYER_REMOVED,    recordPlayerRemoved )
   ON_RECORDER_EVENT_ID( REID_PLAYER_DATA,       recordPlayerData )
   ON_RECORDER_EVENT_ID( REID_PLAYER_DAMAGED,    recordPlayerDamaged )
   ON_RECORDER_EVENT_ID( REID_PLAYER_COLLISION,  recordPlayerCollision )
   ON_RECORDER_EVENT_ID( REID_PLAYER_CRASH,      recordPlayerCrash )
   ON_RECORDER_EVENT_ID( REID_PLAYER_KILLED,     recordPlayerKilled )
   ON_RECORDER_EVENT_ID( REID_WEAPON_RELEASED,   recordWeaponReleased)
   ON_RECORDER_EVENT_ID( REID_WEAPON_HUNG,       recordWeaponHung)
   ON_RECORDER_EVENT_ID( REID_WEAPON_DETONATION, recordWeaponDetonation)
   ON_RECORDER_EVENT_ID( REID_GUN_FIRED,         recordGunFired)
   ON_RECORDER_EVENT_ID( REID_NEW_TRACK,         recordNewTrack)
   ON_RECORDER_EVENT_ID( REID_TRACK_REMOVED,     recordTrackRemoved)
   ON_RECORDER_EVENT_ID( REID_TRACK_DATA,        recordTrackData)
END_RECORDER_HANDLER_TABLE()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataRecorder::DataRecorder()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void DataRecorder::initData()
{
   outputHandler = 0;
   caseNum = 0;
   missionNum = 0;
   subjectNum = 0;
   runNum = 0;
   day = 0;
   month = 0;
   year = 0;

   eventName = "";
   application = "";

   setFirstPass(true);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DataRecorder::copyData(const DataRecorder& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   {  // clone the original's output handler
      OutputHandler* copy = 0;
      if (org.outputHandler != 0) copy = (OutputHandler*) org.outputHandler->clone();
      setOutputHandler(copy);
      if (copy != 0) copy->unref();
   }

   caseNum = org.caseNum;
   missionNum = org.missionNum;
   subjectNum = org.subjectNum;
   runNum = org.runNum;
   day = org.day;
   month = org.month;
   year = org.year;
   eventName = org.eventName;
   application = org.application;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void DataRecorder::deleteData()
{
   setOutputHandler(0);
}


//------------------------------------------------------------------------------
// Background thread processing of the output data record queue
//------------------------------------------------------------------------------
void DataRecorder::processRecords()
{
   if (outputHandler != 0) outputHandler->processQueue();
}

//------------------------------------------------------------------------------
// Process the unhandled or unknown recorder event ID
//------------------------------------------------------------------------------
bool DataRecorder::processUnhandledId(const unsigned int id)
{
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // Record the unknown ID
   Pb::UnknownIdMsg* unknownIdMsg = msg->mutable_unknown_id_msg();
   unknownIdMsg->set_id(id);

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_UNHANDLED_ID_TOKEN );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}


//------------------------------------------------------------------------------
// Simulation reset
//------------------------------------------------------------------------------
void DataRecorder::reset()
{
   BaseClass::reset();

   Pb::DataRecord* msg = new Pb::DataRecord();
   timeStamp(msg);
   msg->set_id( REID_RESET_EVENT );
   sendDataRecord(msg);
}


//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool DataRecorder::shutdownNotification()
{
   if (outputHandler != 0) {

      // Send an end-of-data message
      Pb::DataRecord* msg = new Pb::DataRecord();
      timeStamp(msg);
      msg->set_id( REID_END_OF_DATA );
      sendDataRecord(msg);

      // Flush the output queues
      outputHandler->processQueue();

      // Shutdown the output handlers
      outputHandler->event(SHUTDOWN_EVENT);
   }

   return BaseClass::shutdownNotification();
}


//------------------------------------------------------------------------------
// Marker event handler
//    value[0] => marker ID
//    value[1] => marker source ID
//------------------------------------------------------------------------------
bool DataRecorder::recordMarker(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_MARKER );

   // new Marker message
   Pb::MarkerMsg* markerMsg = msg->mutable_marker_msg();
   markerMsg->set_id( (unsigned int) Eaagles::nintd(values[0]) );
   markerMsg->set_source_id( (unsigned int) Eaagles::nintd(values[1]) );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Analog Input (AI) event handler
//    value[0] => input ID
//    value[1] => input source ID
//    value[2] => value
//------------------------------------------------------------------------------
bool DataRecorder::recordAI(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_AI_EVENT );

   // new Marker message
   Pb::InputDeviceMsg* aiMsg = msg->mutable_input_device_msg();
   aiMsg->set_id( (unsigned int) Eaagles::nintd(values[0]) );
   aiMsg->set_source_id( (unsigned int) Eaagles::nintd(values[1]) );
   aiMsg->set_value( float(values[2]) );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}


//------------------------------------------------------------------------------
// Descrete Input (DI) event handler
//    value[0] => input ID
//    value[1] => input source ID
//    value[2] => value
//------------------------------------------------------------------------------
bool DataRecorder::recordDI(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_DI_EVENT );

   // new Marker message
   Pb::InputDeviceMsg* diMsg = msg->mutable_input_device_msg();
   diMsg->set_id( (unsigned int) Eaagles::nintd(values[0]) );
   diMsg->set_source_id( (unsigned int) Eaagles::nintd(values[1]) );
   diMsg->set_value( float(values[2]) );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}


//------------------------------------------------------------------------------
// New player event handler
//    objs[0] => the new player
//------------------------------------------------------------------------------
bool DataRecorder::recordNewPlayer(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_NEW_PLAYER );

   // new player message
   Pb::NewPlayerEventMsg* newPlayerMsg = msg->mutable_new_player_event_msg();

   genPlayerId( newPlayerMsg->mutable_id(), player );
   genPlayerState( newPlayerMsg->mutable_state(), player );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Player removed event handler
//    objs[0] => the player being removed
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerRemoved(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_REMOVED );

   // removed player message
   Pb::PlayerRemovedEventMsg* removedEventMsg = msg->mutable_player_removed_event_msg();

   genPlayerId( removedEventMsg->mutable_id(), player );
   genPlayerState( removedEventMsg->mutable_state(), player );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Record player's data handler
//    objs[0] => the player being recorded
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerData(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_DATA );

   // player data 
   Pb::PlayerDataMsg* playerDataMsg = msg->mutable_player_data_msg();

   genPlayerId( playerDataMsg->mutable_id(), player );
   genPlayerState( playerDataMsg->mutable_state(), player );

   const Simulation::AirVehicle* av = dynamic_cast<const Simulation::AirVehicle*>( player );
   if (av != 0) {
      playerDataMsg->set_alpha( av->getAngleOfAttackD() );
      playerDataMsg->set_beta( av->getSideSlipD() );
      playerDataMsg->set_cas( av->getCalibratedAirspeed() );
   }

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Player damaged event handler
//    objs[0] => the player damaged
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerDamaged(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_DAMAGED );

   // player damaged message
   Pb::PlayerDamagedEventMsg* playerDamagedMsg = msg->mutable_player_damaged_event_msg();

   genPlayerId( playerDamagedMsg->mutable_id(), player );
   genPlayerState( playerDamagedMsg->mutable_state(), player );

   // Send the message for processing
   sendDataRecord(msg);
   return true;
}

//------------------------------------------------------------------------------
// Player collision event handler
//    objs[0] => the player in the collision
//    objs[1] => the other player in the collision
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerCollision(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Player* otherPlayer = dynamic_cast<const Simulation::Player*>( objs[1] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_COLLISION );

   // player collision message
   Pb::PlayerCollisionEventMsg* playerCollisionMsg = msg->mutable_player_collision_event_msg();

   genPlayerId( playerCollisionMsg->mutable_id(), player );
   genPlayerState( playerCollisionMsg->mutable_state(), player );
   genPlayerId( playerCollisionMsg->mutable_other_player_id(), otherPlayer );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Player crashed event handler
//    objs[0] => the player crashed
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerCrash(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_CRASH );

   // player crashed message
   Pb::PlayerCrashEventMsg* playerCrashMsg = msg->mutable_player_crash_event_msg();

   genPlayerId( playerCrashMsg->mutable_id(), player );
   genPlayerState( playerCrashMsg->mutable_state(), player );

   // Send the message for processing
   sendDataRecord(msg);
   return true;
}

//------------------------------------------------------------------------------
// Player killed event handler
//    objs[0] => the player killed
//------------------------------------------------------------------------------
bool DataRecorder::recordPlayerKilled(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   Pb::DataRecord* msg = new Pb::DataRecord();
   
   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_PLAYER_KILLED );

   // player killed message
   Pb::PlayerKilledEventMsg* playerKilledMsg = msg->mutable_player_killed_event_msg();

   genPlayerId( playerKilledMsg->mutable_id(), player );
   genPlayerState( playerKilledMsg->mutable_state(), player );

   // Send the message for processing
   sendDataRecord(msg);
   return true;
}

//------------------------------------------------------------------------------
// Weapon Released event handler
//    objs[0] => the weapon
//    objs[1] => the shooter
//    objs[2] => the target
//------------------------------------------------------------------------------
bool DataRecorder::recordWeaponReleased(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* wpn = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Player* shooter = dynamic_cast<const Simulation::Player*>( objs[1] );
   const Simulation::Player* tgt = dynamic_cast<const Simulation::Player*>( objs[2] );
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_WEAPON_RELEASED );

   // Weapon Released message
   Pb::WeaponReleaseEventMsg* wpnRelMsg = msg->mutable_weapon_release_event_msg();

   genPlayerId( wpnRelMsg->mutable_wpn_id(), wpn );
   genPlayerState( wpnRelMsg->mutable_wpn_state(), wpn );
   genPlayerId( wpnRelMsg->mutable_shooter_id(), shooter );
   genPlayerId( wpnRelMsg->mutable_tgt_id(), tgt );

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Weapon Hung event handler
//    objs[0] => the weapon
//    objs[1] => the shooter
//    objs[2] => the target  // probably not needed
//------------------------------------------------------------------------------
bool DataRecorder::recordWeaponHung(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* wpn = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Player* shooter = dynamic_cast<const Simulation::Player*>( objs[1] );
   const Simulation::Player* tgt = dynamic_cast<const Simulation::Player*>( objs[2] );
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_WEAPON_HUNG );

   // Weapon Hung message
   Pb::WeaponHungEventMsg* wpnHungMsg = msg->mutable_weapon_hung_event_msg();

   genPlayerId( wpnHungMsg->mutable_wpn_id(), wpn );
   genPlayerState( wpnHungMsg->mutable_wpn_state(), wpn );
   genPlayerId( wpnHungMsg->mutable_shooter_id(), shooter );
   genPlayerId( wpnHungMsg->mutable_tgt_id(), tgt );

   // Send the message for processing
   sendDataRecord(msg);
   return true;
}

//------------------------------------------------------------------------------
// Weapon Detonation event handler
//    objs[0] => the weapon
//    objs[1] => the shooter
//    objs[2] => the target
//    values[0] => detonation type
//    values[1] => miss distance
//------------------------------------------------------------------------------
bool DataRecorder::recordWeaponDetonation(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* wpn = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Player* shooter = dynamic_cast<const Simulation::Player*>( objs[1] );
   const Simulation::Player* tgt = dynamic_cast<const Simulation::Player*>( objs[2] );
   const unsigned int detType =  (unsigned int) values[0];
   const double missDist = values[1];
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_WEAPON_DETONATION );

   // Weapon Hung message
   Pb::WeaponDetonationEventMsg* wpnDetMsg = msg->mutable_weapon_detonation_event_msg();

   genPlayerId( wpnDetMsg->mutable_wpn_id(), wpn );
   genPlayerState( wpnDetMsg->mutable_wpn_state(), wpn );
   genPlayerId( wpnDetMsg->mutable_shooter_id(), shooter );
   genPlayerId( wpnDetMsg->mutable_tgt_id(), tgt );

   // Get detonation type
   switch (detType) {
      case Simulation::Weapon::DETONATE_OTHER: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_OTHER);
         break;
      }
      case Simulation::Weapon::DETONATE_ENTITY_IMPACT: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_ENTITY_IMPACT);
         break;
      }
      case Simulation::Weapon::DETONATE_ENTITY_PROXIMATE_DETONATION: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_ENTITY_PROXIMATE_DETONATION);
         break;
      }
      case Simulation::Weapon::DETONATE_GROUND_IMPACT: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_GROUND_IMPACT);
         break;
      }
      case Simulation::Weapon::DETONATE_GROUND_PROXIMATE_DETONATION: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_GROUND_PROXIMATE_DETONATION);
         break;
      }
      case Simulation::Weapon::DETONATE_DETONATION: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_DETONATION);
         break;
      }
      case Simulation::Weapon::DETONATE_NONE: {
         wpnDetMsg->set_det_type(Pb::WeaponDetonationEventMsg_DetonationType_DETONATE_NONE);
         break;
      }
      default: {
         break;
      }
   }

   // Missile Distance
   wpnDetMsg->set_miss_dist(missDist);

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Gun Fired event handler
//    objs[0] => the shooter
//    values[0] => rounds
//------------------------------------------------------------------------------
bool DataRecorder::recordGunFired(const Basic::Object* objs[4], const double values[4])
{
   const Simulation::Player* shooter = dynamic_cast<const Simulation::Player*>( objs[0] );
   const unsigned int rounds = (unsigned int) values[0];
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_GUN_FIRED );

   // Gun Fired message
   Pb::GunFiredEventMsg* gunFiredMsg = msg->mutable_gun_fired_event_msg();

   genPlayerId( gunFiredMsg->mutable_shooter_id(), shooter );
   gunFiredMsg->set_rounds(rounds);

   // Send the message for processing
   sendDataRecord(msg);
   return true;
}

//------------------------------------------------------------------------------
// New Track event handler
//    objs[0] => the player (ownship)
//    objs[1] => the track
//------------------------------------------------------------------------------
bool DataRecorder::recordNewTrack(const Basic::Object* objs[4], const double values[4])
{
   // objects
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Track* newTrack = dynamic_cast<const Simulation::Track*>( objs[1] );

   // message
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_NEW_TRACK );

   // New Track message
   Pb::NewTrackEventMsg* newTrackMsg = msg->mutable_new_track_event_msg();

   // player ID and state
   genPlayerId( newTrackMsg->mutable_player_id(), player );
   genPlayerState( newTrackMsg->mutable_player_state(), player );

   // Track ID
   newTrackMsg->set_track_id(genTrackId(newTrack));

   // track data
   genTrackData(newTrackMsg->mutable_track_data(), newTrack);

   // Emission Data
   const Simulation::RfTrack* const rfTrk = dynamic_cast<const Simulation::RfTrack*>(newTrack);
   if (rfTrk != 0) {

      const Simulation::Emission* emissionData = rfTrk->getLastEmission();
      genEmissionData( newTrackMsg->mutable_emission_data(), emissionData);

      if (emissionData != 0) {
         const Simulation::Player* trkPlayer = emissionData->getOwnship();

         // Track ID and state 
         genPlayerId( newTrackMsg->mutable_trk_player_id(), trkPlayer);
         genPlayerState( newTrackMsg->mutable_trk_player_state(), trkPlayer );

         // Altitude
         if (trkPlayer != 0) newTrackMsg->mutable_track_data()->set_altitude(trkPlayer->getAltitude());
         else newTrackMsg->mutable_track_data()->set_altitude(0);
      }

   }

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Track Removed event handler
//    objs[0] => the player (ownship)
//    objs[1] => the track
//------------------------------------------------------------------------------
bool DataRecorder::recordTrackRemoved(const Basic::Object* objs[4], const double values[4])
{
   // objects
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Track* track = dynamic_cast<const Simulation::Track*>( objs[1] );

   // message
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_TRACK_REMOVED );

   // Track Removed message
   Pb::TrackRemovedEventMsg* trackRemovedMsg = msg->mutable_track_removed_event_msg();

   genPlayerId(trackRemovedMsg->mutable_player_id(), player);

   // Track ID
   trackRemovedMsg->set_track_id(genTrackId(track));

   // Send the message for processing
   sendDataRecord(msg);


   return true;
}

//------------------------------------------------------------------------------
// Track Data event handler
//    objs[0] => the player (ownship)
//    objs[1] => the track
//------------------------------------------------------------------------------
bool DataRecorder::recordTrackData(const Basic::Object* objs[4], const double values[4])
{
   // objects
   const Simulation::Player* player = dynamic_cast<const Simulation::Player*>( objs[0] );
   const Simulation::Track* trackData = dynamic_cast<const Simulation::Track*>( objs[1] );

   // message
   Pb::DataRecord* msg = new Pb::DataRecord();

   // DataRecord header
   timeStamp(msg);
   msg->set_id( REID_TRACK_DATA );

   // Track Data message
   Pb::TrackDataMsg* trackDataMsg = msg->mutable_track_data_msg();

   // player ID and state
   genPlayerId( trackDataMsg->mutable_player_id(), player );
   genPlayerState( trackDataMsg->mutable_player_state(), player );

   // Track ID
   trackDataMsg->set_track_id(genTrackId(trackData));

   // track data
   genTrackData(trackDataMsg->mutable_track_data(), trackData);

   // Emission Data
   const Simulation::RfTrack* const rfTrk = dynamic_cast<const Simulation::RfTrack*>(trackData);
   if (rfTrk != 0) {
      const Simulation::Emission* emissionData = rfTrk->getLastEmission();
      genEmissionData( trackDataMsg->mutable_emission_data(), emissionData);

      if (emissionData != 0) {
         const Simulation::Player* trkPlayer = emissionData->getOwnship();

         // Track ID and state 
         genPlayerId( trackDataMsg->mutable_trk_player_id(), trkPlayer);
         genPlayerState( trackDataMsg->mutable_trk_player_state(), trkPlayer );

         // Altitude
         trackDataMsg->mutable_track_data()->set_altitude(trkPlayer->getAltitude());
      }
   }

   // Send the message for processing
   sendDataRecord(msg);

   return true;
}

//------------------------------------------------------------------------------
// Generate the Player ID data
//------------------------------------------------------------------------------
void DataRecorder::genPlayerId( Pb::PlayerId* const id, const Simulation::Player* const player )
{
   // Check for valid message pointer
   if (id != 0) {
      // Check for valid player pointer
      if (player != 0) {

      // Player Id
      id->set_id( player->getID() );

      // Player name
      const Basic::String* name = player->getName();
      if (name != 0) id->set_name( *name );

      // Networked player federation name
      if ( player->isNetworkedPlayer() ) {
         const Simulation::Nib* nib = player->getNib();
         const Basic::String* fedName = nib->getFederateName();
         if (fedName != 0) id->set_fed_name( *fedName );
      }

      // Player side
      id->set_side(player->getSide());
   }
      else {
         // We don't have a player, set default values
         id->set_id(0);
         id->set_name("");
      }
   }
}

//------------------------------------------------------------------------------
// Generate the player state data
//------------------------------------------------------------------------------
void DataRecorder::genPlayerState( Pb::PlayerState* const state, const Simulation::Player* const player )
{

   if (state != 0) {
      if (player != 0) {

      // position
      osg::Vec3d pos = player->getGeocPosition();
      state->mutable_pos()->set_x(pos[0]);
      state->mutable_pos()->set_y(pos[1]);
      state->mutable_pos()->set_z(pos[2]);

      // angles
      osg::Vec3d angles = player->getGeocEulerAngles();
      state->mutable_angles()->set_x(angles[0]);
      state->mutable_angles()->set_y(angles[1]);
      state->mutable_angles()->set_z(angles[2]);

      // velocity
      osg::Vec3d vel = player->getGeocVelocity();
      state->mutable_vel()->set_x(vel[0]);
      state->mutable_vel()->set_y(vel[1]);
      state->mutable_vel()->set_z(vel[2]);

      // Damage
      state->set_damage(player->getDamage());
      }
      else {
         // No player, set default values in message
         state->mutable_pos()->set_x(0);
         state->mutable_pos()->set_y(0);
         state->mutable_pos()->set_z(0);

         // angles
         state->mutable_angles()->set_x(0);
         state->mutable_angles()->set_y(0);
         state->mutable_angles()->set_z(0);

         // velocity
         state->mutable_vel()->set_x(0);
         state->mutable_vel()->set_y(0);
         state->mutable_vel()->set_z(0);

         // Damage
         state->set_damage(0);
      }
   }
}

//------------------------------------------------------------------------------
// Generate the Track ID 
//------------------------------------------------------------------------------
std::string DataRecorder::genTrackId( const Simulation::Track* const track )
{
   std::string trackIdStr = "";
   if (track != 0) {
      // Track ID
      // Convert track ID to string
      int trackId = track->getTrackID();
      char cbuf[8];
      std::sprintf(cbuf, "%d", trackId);
      trackIdStr.assign("trk");
      trackIdStr.append(cbuf);
   }
   return trackIdStr;
}


//------------------------------------------------------------------------------
// Generate the Track data
//------------------------------------------------------------------------------
void DataRecorder::genTrackData( Pb::TrackData* const trkMsg, const Simulation::Track* const track )
{
   if (trkMsg != 0) {
      if (track != 0) {
         // set message data from track
      trkMsg->set_type(track->getType());
      trkMsg->set_quality(track->getQuality());
      trkMsg->set_true_az(track->getTrueAzimuthR());
      trkMsg->set_rel_az(track->getRelAzimuthR());
      trkMsg->set_elevation(track->getElevationR());
      trkMsg->set_range(track->getRange());

      // lat/lon:
      double lat;
      double lon;
      track->getLatLonPosition(&lat, &lon);

      trkMsg->set_latitude(lat);
      trkMsg->set_longitude(lon);

      // Position: getPosition returns osg::Vec3
      osg::Vec3 pos = track->getPosition();
      trkMsg->mutable_position()->set_x(pos[0]);
      trkMsg->mutable_position()->set_y(pos[1]);
      trkMsg->mutable_position()->set_z(pos[2]);

      // Velocity: getVelocity returns osg::Vec3
      osg::Vec3 vel = track->getVelocity();
      trkMsg->mutable_velocity()->set_x(vel[0]);
      trkMsg->mutable_velocity()->set_y(vel[1]);
      trkMsg->mutable_velocity()->set_z(vel[2]);

      // Avg signal for RF/IR track:
      const Simulation::RfTrack* const rfTrk = dynamic_cast<const Simulation::RfTrack*>(track);
      if (rfTrk != 0) {
         trkMsg->set_avg_signal(rfTrk->getAvgSignal());
      }

      trkMsg->set_sl_index(track->getShootListIndex());
      trkMsg->set_wpn_rel(track->isWeaponReleased());
   }
      else {
         // no track, just set default data
         trkMsg->set_type(0);
         trkMsg->set_quality(0);
         trkMsg->set_true_az(0);
         trkMsg->set_rel_az(0);
         trkMsg->set_elevation(0);
         trkMsg->set_range(0);
         trkMsg->set_latitude(0);
         trkMsg->set_longitude(0);
         trkMsg->mutable_position()->set_x(0);
         trkMsg->mutable_position()->set_y(0);
         trkMsg->mutable_position()->set_z(0);

         trkMsg->mutable_velocity()->set_x(0);
         trkMsg->mutable_velocity()->set_y(0);
         trkMsg->mutable_velocity()->set_z(0);
         trkMsg->set_avg_signal(0);

         trkMsg->set_sl_index(0);
         trkMsg->set_wpn_rel(0);
      }
   }
}

//------------------------------------------------------------------------------
// Generate the Emission data
//------------------------------------------------------------------------------
void DataRecorder::genEmissionData( Pb::EmissionData* const emMsg, const Simulation::Emission* const emData)
{
   if (emMsg != 0) {
      if (emData != 0) {
         // set message data from emission data
      emMsg->set_frequency(emData->getFrequency());
      emMsg->set_wave_length(emData->getWavelength());
      emMsg->set_pulse_width(emData->getPulseWidth());
      emMsg->set_bandwidth(emData->getBandwidth());
      emMsg->set_prf(emData->getPRF());
      emMsg->set_power(emData->getPower());
      emMsg->set_azimuth_aoi(emData->getAzimuthAoi());
      emMsg->set_elevation_aoi(emData->getElevationAoi());

      // polarization
      Simulation::Antenna::Polarization polarization = emData->getPolarization();
      switch (polarization) {
         case Simulation::Antenna::NONE: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_NONE);
            break;
         }
         case Simulation::Antenna::VERTICAL: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_VERTICAL);
            break;
         }
         case Simulation::Antenna::HORIZONTAL: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_HORIZONTAL);
            break;
         }
         case Simulation::Antenna::SLANT: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_SLANT);
            break;
         }
         case Simulation::Antenna::RHC: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_RHC);
            break;
         }
         case Simulation::Antenna::LHC: {
            emMsg->set_polarization(Pb::EmissionData_Polarization_LHC);
            break;
         }
         default: {
            break;
         }
         }
      }
      else {
         // set default message values
         emMsg->set_polarization(Pb::EmissionData_Polarization_NONE);
         emMsg->set_frequency(0);
         emMsg->set_wave_length(0);
         emMsg->set_pulse_width(0);
         emMsg->set_bandwidth(0);
         emMsg->set_prf(0);
         emMsg->set_power(0);
         emMsg->set_azimuth_aoi(0);
         emMsg->set_elevation_aoi(0);
      }
   }
}


//------------------------------------------------------------------------------
// Time stamp and send the DataRecord to our output handler
//------------------------------------------------------------------------------
void DataRecorder::sendDataRecord(Pb::DataRecord* const msg)
{
   if (msg != 0 && outputHandler != 0) {

      // If first pass, create and send a FILE ID msg
      if (isFirstPass()) {

         // reset first pass
         setFirstPass(false);

         // create and send File ID
         Pb::DataRecord* msg = new Pb::DataRecord();
         // DataRecord header
         timeStamp(msg);
         msg->set_id( REID_FILE_ID );

         // File ID message
         Pb::FileIdMsg* fileIdMsg = msg->mutable_file_id_msg();

         // from slot data:
         fileIdMsg->set_application(getApplication());
         fileIdMsg->set_case_num(getCaseNum());
         fileIdMsg->set_day( getDay());
         fileIdMsg->set_event_name(getEventName());
         fileIdMsg->set_mission_num(getMissionNum());
         fileIdMsg->set_month(getMonth());
         fileIdMsg->set_run_num(getRunNum());
         fileIdMsg->set_subject_num(getSubjectNum());
         fileIdMsg->set_year(getYear());

         // Create a handle and send the message to be processed
         DataRecordHandle* h = new DataRecordHandle(msg);
         outputHandler->processRecord(h);
         h->unref();
      }

      // Create a handle and send the message to be processed
      DataRecordHandle* h = new DataRecordHandle(msg);
      outputHandler->addToQueue(h);
      h->unref();
   }
}

//------------------------------------------------------------------------------
// Time stamp the DataRecord
//------------------------------------------------------------------------------
void DataRecorder::timeStamp(Pb::DataRecord* const msg)
{
   if (msg != 0) {
      Simulation::Simulation* sim = getSimulation();
      if (sim != 0) {
         Pb::Time* time = msg->mutable_time();
         time->set_exec_time( sim->getExecTimeSec() );
         time->set_sim_time( sim->getSimTimeOfDay() );
         time->set_utc_time( sim->getSysTimeOfDay() );
      }
      else {
         // unknown time
         Pb::Time* time = msg->mutable_time();
         time->set_exec_time( -1.0 );
         time->set_sim_time( -1.0 );
         time->set_utc_time( -1.0 );
      }
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void DataRecorder::setFirstPass(const bool f)
{
   firstPass = f;
}

bool DataRecorder::setOutputHandler(OutputHandler* const msg)
{
   if (outputHandler != 0) outputHandler->unref();
   outputHandler = msg;
   if (outputHandler != 0) outputHandler->ref();
   return true;
}

bool DataRecorder::setSlotEventName(Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      eventName = msg->getCopyString();
      ok = true;
   }
   return ok;
}

bool DataRecorder::setSlotApplication(Basic::String* const msg)
{
   bool ok = false;
   if (msg != 0) {
      application = msg->getCopyString();
      ok = true;
   }
   return ok;
}

bool DataRecorder::setSlotCaseNum(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      caseNum = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotMissionNum(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      missionNum = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotSubjectNum(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      subjectNum = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotRunNum(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      runNum = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotDay(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      day = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotMonth(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      month = msg->getInt();
   }
   return ok;
}

bool DataRecorder::setSlotYear(Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = true;
      year = msg->getInt();
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* DataRecorder::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Recorder namespace
} // End Eaagles namespace
