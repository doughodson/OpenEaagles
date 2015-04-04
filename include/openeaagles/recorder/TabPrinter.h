//------------------------------------------------------------------------------
// Class: TabPrinter
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_TabPrinter_H__
#define __Eaagles_Recorder_TabPrinter_H__

#include "openeaagles/recorder/OutputHandler.h"
#include "openeaagles/recorder/PrintHandler.h"
#include <string>
#include <sstream>
#include <fstream>

namespace Eaagles {
namespace Recorder {
   namespace Pb {
      class Time; class FileIdMsg; class NewPlayerEventMsg; class PlayerRemovedEventMsg; class PlayerDataMsg;
      class PlayerDamagedEventMsg; class PlayerCollisionEventMsg; class PlayerCrashEventMsg;
      class PlayerKilledEventMsg; class WeaponReleaseEventMsg; class WeaponHungEventMsg;
      class WeaponDetonationEventMsg; class GunFiredEventMsg; class NewTrackEventMsg;
      class TrackRemovedEventMsg; class TrackDataMsg; class PlayerId; class PlayerState;
      class TrackData; class EmissionData; class MarkerMsg; class InputDeviceMsg;
   }

//------------------------------------------------------------------------------
// Class: TabPrinter
// Description: Print the data from a protocol buffer DataRecord
//
// Factory name: TabPrinter
// Slots:
//    msgHdrOptn   ! Msg Header options (see TabPrinter.h)
//    divider      ! Field divider (default: tab)
//------------------------------------------------------------------------------
class TabPrinter : public PrintHandler
{
    DECLARE_SUBCLASS(TabPrinter, PrintHandler)

public:
   enum MsgHdrOptions { NO_HDR, ALL_MSGS, NEW_MSG, ON_CHANGE };

public:
   TabPrinter();

   // set message header options
   void setMsgHdrOptions(MsgHdrOptions msg);
   bool isSimReset() const;                          // True if reset msg recorded

   // set divider string
   bool setSlotDivider(const Basic::String* const msg);

protected:

   virtual bool setSlotMsgHdr(const Basic::String* const msg);

   // Print functions

   // Event Messages
   virtual void printFileIdMsg(const Pb::Time* const timeMsg, const Pb::FileIdMsg* const msg);
   virtual void printNewPlayerEventMsg(const Pb::Time* const timeMsg, const Pb::NewPlayerEventMsg* const msg);
   virtual void printPlayerRemovedEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerRemovedEventMsg* const msg);
   virtual void printPlayerDataMsg(const Pb::Time* const timeMsg, const Pb::PlayerDataMsg* const msg);
   virtual void printPlayerDamagedEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerDamagedEventMsg* const msg);
   virtual void printPlayerCollisionEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerCollisionEventMsg* const msg);
   virtual void printPlayerCrashEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerCrashEventMsg* const msg);
   virtual void printPlayerKilledEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerKilledEventMsg* const msg);
   virtual void printWeaponReleaseEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponReleaseEventMsg* const msg);
   virtual void printWeaponHungEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponHungEventMsg* const msg);
   virtual void printWeaponDetonationEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponDetonationEventMsg* const msg);
   virtual void printGunFiredEventMsg(const Pb::Time* const timeMsg, const Pb::GunFiredEventMsg* const msg);
   virtual void printNewTrackEventMsg(const Pb::Time* const timeMsg, const Pb::NewTrackEventMsg* const msg);
   virtual void printTrackRemovedEventMsg(const Pb::Time* const timeMsg, const Pb::TrackRemovedEventMsg* const msg);
   virtual void printTrackDataMsg(const Pb::Time* const timeMsg, const Pb::TrackDataMsg* const msg);
   virtual void printMarkerMsg(const Pb::Time* const timeMsg, const Pb::MarkerMsg* const msg);
   virtual void printInputDeviceMsg(const Pb::Time* const timeMsg, const Pb::InputDeviceMsg* const msg, const  unsigned int msgId);

   // Events without messages
   virtual void printUnhandledIdToken(const Pb::Time* const timeMsg);
   virtual void printEndOfData(const Pb::Time* const timeMsg);
   virtual void printResetEvent(const Pb::Time* const timeMsg);

   // Common Data Messages
   virtual void printTimeMsg(std::ostream& sout, const Pb::Time* const timeMsg);
   virtual void printPlayerIdMsg(std::ostream& sout, const Pb::PlayerId* const playerIdMsg);
   virtual void printPlayerStateMsg(std::ostream& sout, const Pb::PlayerState* const playerStateMsg);
   virtual void printCommonTrackDataMsg(std::ostream& sout, const Pb::TrackData* const msg);
   virtual void printEmissionDataMsg(std::ostream& sout, const Pb::EmissionData* const msg);

   // Message Field Header functions
   virtual void printTimeMsgHdr(std::ostream& sout);
   virtual void printPlayerIdMsgHdr(std::ostream& sout);
   virtual void printPlayerStateMsgHdr(std::ostream& sout);
   virtual void printTrackDataHdr(std::ostream& sout);
   virtual void printEmissionDataMsgHdr(std::ostream& sout);
   virtual void printWeaponMsgHdr(std::ostream& sout);
   virtual void printTrackMsgHdr(std::ostream& sout);

   virtual void printEmissionDataSpacer(std::ostream& sout);
   virtual void printPlayerIdSpacer(std::ostream& sout);
   virtual void printPlayerDataSpacer(std::ostream& sout);
   virtual void printTrackDataSpacer(std::ostream& sout);

   void printExecTimeMsg(std::ostream& sout, double execTime);
   void printSimTimeMsg(std::ostream& sout, double simTime);
   void printUtcTimeMsg(std::ostream& sout, double utcTime);

   void processRecordImp(const DataRecordHandle* const handle) override;

   virtual void setMsgHeaders(const bool f);
private:
   void initData();

   // print options
   bool printHeader;
   bool simReset;

   // individual headers
   bool fileIdHdr;
   bool playerNewHdr;
   bool playerRemovedHdr;
   bool playerDataHdr;
   bool playerDamagedHdr;
   bool playerCollisionHdr;
   bool playerCrashHdr;
   bool playerKilledHdr;
   bool weaponReleaseHdr;
   bool weaponHungHdr;
   bool weaponDetonateHdr;
   bool gunFiredHdr;
   bool trackNewHdr;
   bool trackRemovedHdr;
   bool trackDataHdr;
   bool markerHdr;
   bool inputDeviceHdr;

   // Group headers
   bool playerHeader;
   bool weaponHeader;
   bool trackHeader;

   MsgHdrOptions option;     // options for printing field names
   unsigned int lastMessage; // previous message printed
   const char* divider;      // divider between message fields
};


// inline functions
inline bool TabPrinter::isSimReset() const { return simReset; }

} // End Recorder namespace
} // End Eaagles namespace

#endif
