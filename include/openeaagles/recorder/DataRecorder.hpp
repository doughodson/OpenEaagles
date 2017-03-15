
#ifndef __oe_recorder_DataRecorder_H__
#define __oe_recorder_DataRecorder_H__

#include "openeaagles/simulation/AbstractDataRecorder.hpp"
#include <string>

namespace oe {
namespace models { class Player; class Track; class Emission; }
namespace base { class String; }
namespace recorder {
namespace pb { class DataRecord; class PlayerId; class PlayerState;
               class TrackData; class EmissionData; }
class DataRecordHandle;
class OutputHandler;

//------------------------------------------------------------------------------
// Class: DataRecorder
// Description: Data recorder; receives and processes data samples
//
// Factory name: DataRecorder
// Slots:
//    outputHandler     <OutputHandler>      ! Output handler (default: none)
//
// Notes:
//    1) negative time values are used when time is unknown.
//
//------------------------------------------------------------------------------
// Recorder events handled ---
//
//    REID_MARKER             ! v[0] => id;  v[1] => source id;
//    REID_DI_EVENT           ! v[0] => id;  v[1] => source id;  v[2] => value
//    REID_AI_EVENT           ! v[0] => id;  v[1] => source id;  v[2] => value
//
//    REID_NEW_PLAYER         ! obj[0] => (new player)
//    REID_PLAYER_REMOVED     ! obj[0] => (player)
//    REID_PLAYER_DATA        ! obj[0] => (player)
//    REID_PLAYER_DAMAGED     ! obj[0] => (player); obj[1] => (weapon)
//    REID_PLAYER_COLLISION   ! obj[0] => (player); obj[1] => (other player)
//    REID_PLAYER_CRASH       ! obj[0] => (player)
//    REID_PLAYER_KILLED      ! obj[0] => (player); obj[1] => (shooter)
//
//    REID_WEAPON_RELEASED    ! obj[0] => (weapon); obj[1] => (shooter); obj[2] => (tgt)
//    REID_WEAPON_HUNG        ! obj[0] => (weapon); obj[1] => (shooter); obj[2] => (tgt)
//    REID_WEAPON_DETONATION  ! obj[0] => (weapon); obj[1] => (shooter); obj[2] => (tgt)
//                            !   v[0] => detonation type id; v[1] => miss distance
//    REID_GUN_FIRED          ! obj[0] => (shooter); v[0] => rounds
//
//    REID_NEW_TRACK          ! obj[0] => (player); obj[1] => (track)
//    REID_TRACK_REMOVED      ! obj[0] => (player); obj[1] => (track)
//    REID_TRACK_DATA         ! obj[0] => (player); obj[1] => (track)
//
//------------------------------------------------------------------------------
class DataRecorder : public simulation::AbstractDataRecorder
{
   DECLARE_SUBCLASS(DataRecorder, simulation::AbstractDataRecorder)

public:
   DataRecorder();

   const char* getEventName() const       { return eventName; }
   const char* getApplication() const     { return application; }
   unsigned int getCaseNum() const        { return caseNum; }
   unsigned int getMissionNum() const     { return missionNum; }
   unsigned int getSubjectNum() const     { return subjectNum; }
   unsigned int getRunNum() const         { return runNum; }
   unsigned int getDay() const            { return day; }
   unsigned int getMonth() const          { return month; }
   unsigned int getYear() const           { return year; }

   virtual void processRecords() override;
   virtual void reset() override;

protected:
   // Get functions
   OutputHandler* getOutputHandler();
   const OutputHandler* getOutputHandler() const;

   bool isFirstPass() const               { return firstPass; }

   // Set functions
   bool setOutputHandler(OutputHandler* const msg);
   bool setSlotEventName(base::String* const msg);
   bool setSlotApplication(base::String* const msg);
   bool setSlotCaseNum(base::Number* const msg);
   bool setSlotMissionNum(base::Number* const msg);
   bool setSlotSubjectNum(base::Number* const msg);
   bool setSlotRunNum(base::Number* const msg);
   bool setSlotDay(base::Number* const msg);
   bool setSlotMonth(base::Number* const msg);
   bool setSlotYear(base::Number* const msg);

   // data filler functions
   virtual void genPlayerId( pb::PlayerId* const id, const models::Player* const player );
   virtual void genPlayerState( pb::PlayerState* const state, const models::Player* const player );
   virtual void genTrackData( pb::TrackData* const trkMsg, const models::Track* const track );
   virtual void genEmissionData( pb::EmissionData* const emMsg, const models::Emission* const emData);
   virtual void sendDataRecord(pb::DataRecord* const msg);       // Send the DataRecord to our output handler
   virtual void timeStamp(pb::DataRecord* const msg);            // Time stamp the DataRecord
   virtual std::string genTrackId(const models::Track* const track);
   void setFirstPass(const bool f);

   // Recorder data event handlers
   virtual bool recordMarker(const base::Object* objs[4], const double values[4]);
   virtual bool recordAI(const base::Object* objs[4], const double values[4]);
   virtual bool recordDI(const base::Object* objs[4], const double values[4]);
   virtual bool recordNewPlayer(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerRemoved(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerData(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerDamaged(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerCollision(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerCrash(const base::Object* objs[4], const double values[4]);
   virtual bool recordPlayerKilled(const base::Object* objs[4], const double values[4]);
   virtual bool recordWeaponReleased(const base::Object* objs[4], const double values[4]);
   virtual bool recordWeaponHung(const base::Object* objs[4], const double values[4]);
   virtual bool recordWeaponDetonation(const base::Object* objs[4], const double values[4]);
   virtual bool recordGunFired(const base::Object* objs[4], const double values[4]);
   virtual bool recordNewTrack(const base::Object* objs[4], const double values[4]);
   virtual bool recordTrackRemoved(const base::Object* objs[4], const double values[4]);
   virtual bool recordTrackData(const base::Object* objs[4], const double values[4]);

   // simulation::DataRecorder class protected interface functions
   virtual bool recordDataImp(
      const unsigned int id,              // Recorder event Id
      const base::Object* pObjects[4],    // Sample objects
      const double values[4]              // Sample values
   ) override;

   virtual bool processUnhandledId(const unsigned int id) override;

   // base::Component protected interface
   virtual bool shutdownNotification() override;

private:
   void initData();

   OutputHandler* outputHandler {};          // Our output handler
   bool firstPass {true};

   const char* eventName {};
   const char* application {};
   unsigned int caseNum {};
   unsigned int missionNum {};
   unsigned int subjectNum {};
   unsigned int runNum {};
   unsigned int day {};
   unsigned int month {};
   unsigned int year {};
};

#include "openeaagles/recorder/DataRecorder.inl"

}
}

#endif
