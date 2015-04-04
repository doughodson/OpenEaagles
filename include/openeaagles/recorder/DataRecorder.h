//------------------------------------------------------------------------------
// OpenEaagles Data Recorder
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_DataRecorder_H__
#define __Eaagles_Recorder_DataRecorder_H__

#include "openeaagles/simulation/DataRecorder.h"

namespace Eaagles {
   namespace Simulation { class Player; class Track; class Emission; }
   namespace Basic { class String; }

namespace Recorder {
   namespace Pb { class DataRecord; class PlayerId; class PlayerState;
      class TrackData; class EmissionData; }
   class DataRecordHandle;
   class OutputHandler;

//------------------------------------------------------------------------------
// Class: DataRecorder
// Description: OpenEaagles data recorder; receives and processes data samples
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
class DataRecorder : public Simulation::DataRecorder
{
   DECLARE_SUBCLASS(DataRecorder, Simulation::DataRecorder)

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

   void processRecords() override;
   void reset() override;

protected:
   // Get functions
   OutputHandler* getOutputHandler();
   const OutputHandler* getOutputHandler() const;

   bool isFirstPass() const               { return firstPass; }

   // Set functions
   bool setOutputHandler(OutputHandler* const msg);
   bool setSlotEventName(Basic::String* const msg);
   bool setSlotApplication(Basic::String* const msg);
   bool setSlotCaseNum(Basic::Number* const msg);
   bool setSlotMissionNum(Basic::Number* const msg);
   bool setSlotSubjectNum(Basic::Number* const msg);
   bool setSlotRunNum(Basic::Number* const msg);
   bool setSlotDay(Basic::Number* const msg);
   bool setSlotMonth(Basic::Number* const msg);
   bool setSlotYear(Basic::Number* const msg);

   // data filler functions
   virtual void genPlayerId( Pb::PlayerId* const id, const Simulation::Player* const player );
   virtual void genPlayerState( Pb::PlayerState* const state, const Simulation::Player* const player );
   virtual void genTrackData( Pb::TrackData* const trkMsg, const Simulation::Track* const track );
   virtual void genEmissionData( Pb::EmissionData* const emMsg, const Simulation::Emission* const emData);
   virtual void sendDataRecord(Pb::DataRecord* const msg);       // Send the DataRecord to our output handler
   virtual void timeStamp(Pb::DataRecord* const msg);            // Time stamp the DataRecord
   virtual std::string genTrackId(const Simulation::Track* const track);
   void setFirstPass(const bool f);

   // Recorder data event handlers
   virtual bool recordMarker(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordAI(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordDI(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordNewPlayer(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerRemoved(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerData(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerDamaged(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerCollision(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerCrash(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordPlayerKilled(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordWeaponReleased(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordWeaponHung(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordWeaponDetonation(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordGunFired(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordNewTrack(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordTrackRemoved(const Basic::Object* objs[4], const double values[4]);
   virtual bool recordTrackData(const Basic::Object* objs[4], const double values[4]);

   // Simulation::DataRecorder class protected interface functions
   virtual bool recordDataImp(
      const unsigned int id,              // Recorder event Id
      const Basic::Object* pObjects[4],   // Sample objects
      const double values[4]              // Sample values
   );
   virtual bool processUnhandledId(const unsigned int id);

   // Basic::Component protected interface
   bool shutdownNotification() override;

private:
   void initData();

   OutputHandler* outputHandler;          // Our output handler
   bool firstPass;

   const char* eventName;
   const char* application;
   unsigned int caseNum;
   unsigned int missionNum;
   unsigned int subjectNum;
   unsigned int runNum;
   unsigned int day;
   unsigned int month;
   unsigned int year;
};

#include "openeaagles/recorder/DataRecorder.inl"

} // End Recorder namespace
} // End Eaagles namespace

#endif
