
#ifndef __oe_recorder_TabPrinter_H__
#define __oe_recorder_TabPrinter_H__

#include "openeaagles/recorder/OutputHandler.hpp"
#include "openeaagles/recorder/PrintHandler.hpp"
#include <string>
#include <sstream>
#include <fstream>

namespace oe {
namespace recorder {
namespace pb {
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
   enum class MsgHdrOptions { NO_HDR, ALL_MSGS, NEW_MSG, ON_CHANGE };

public:
   TabPrinter();

   // set message header options
   void setMsgHdrOptions(MsgHdrOptions msg);
   bool isSimReset() const;                   // True if reset msg recorded

   // set divider string
   bool setSlotDivider(const base::String* const msg);

protected:

   virtual bool setSlotMsgHdr(const base::String* const msg);

   // Print functions

   // Event Messages
   virtual void printFileIdMsg(const pb::Time* const timeMsg, const pb::FileIdMsg* const msg);
   virtual void printNewPlayerEventMsg(const pb::Time* const timeMsg, const pb::NewPlayerEventMsg* const msg);
   virtual void printPlayerRemovedEventMsg(const pb::Time* const timeMsg, const pb::PlayerRemovedEventMsg* const msg);
   virtual void printPlayerDataMsg(const pb::Time* const timeMsg, const pb::PlayerDataMsg* const msg);
   virtual void printPlayerDamagedEventMsg(const pb::Time* const timeMsg, const pb::PlayerDamagedEventMsg* const msg);
   virtual void printPlayerCollisionEventMsg(const pb::Time* const timeMsg, const pb::PlayerCollisionEventMsg* const msg);
   virtual void printPlayerCrashEventMsg(const pb::Time* const timeMsg, const pb::PlayerCrashEventMsg* const msg);
   virtual void printPlayerKilledEventMsg(const pb::Time* const timeMsg, const pb::PlayerKilledEventMsg* const msg);
   virtual void printWeaponReleaseEventMsg(const pb::Time* const timeMsg, const pb::WeaponReleaseEventMsg* const msg);
   virtual void printWeaponHungEventMsg(const pb::Time* const timeMsg, const pb::WeaponHungEventMsg* const msg);
   virtual void printWeaponDetonationEventMsg(const pb::Time* const timeMsg, const pb::WeaponDetonationEventMsg* const msg);
   virtual void printGunFiredEventMsg(const pb::Time* const timeMsg, const pb::GunFiredEventMsg* const msg);
   virtual void printNewTrackEventMsg(const pb::Time* const timeMsg, const pb::NewTrackEventMsg* const msg);
   virtual void printTrackRemovedEventMsg(const pb::Time* const timeMsg, const pb::TrackRemovedEventMsg* const msg);
   virtual void printTrackDataMsg(const pb::Time* const timeMsg, const pb::TrackDataMsg* const msg);
   virtual void printMarkerMsg(const pb::Time* const timeMsg, const pb::MarkerMsg* const msg);
   virtual void printInputDeviceMsg(const pb::Time* const timeMsg, const pb::InputDeviceMsg* const msg, const  unsigned int msgId);

   // Events without messages
   virtual void printUnhandledIdToken(const pb::Time* const timeMsg);
   virtual void printEndOfData(const pb::Time* const timeMsg);
   virtual void printResetEvent(const pb::Time* const timeMsg);

   // Common Data Messages
   virtual void printTimeMsg(std::ostream& sout, const pb::Time* const timeMsg);
   virtual void printPlayerIdMsg(std::ostream& sout, const pb::PlayerId* const playerIdMsg);
   virtual void printPlayerStateMsg(std::ostream& sout, const pb::PlayerState* const playerStateMsg);
   virtual void printCommonTrackDataMsg(std::ostream& sout, const pb::TrackData* const msg);
   virtual void printEmissionDataMsg(std::ostream& sout, const pb::EmissionData* const msg);

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

   virtual void processRecordImp(const DataRecordHandle* const handle) override;

   virtual void setMsgHeaders(const bool f);
private:
   // print options
   bool printHeader {};
   bool simReset {true};

   // individual headers
   bool fileIdHdr {true};
   bool playerNewHdr {true};
   bool playerRemovedHdr {true};
   bool playerDataHdr {true};
   bool playerDamagedHdr {true};
   bool playerCollisionHdr {true};
   bool playerCrashHdr {true};
   bool playerKilledHdr {true};
   bool weaponReleaseHdr {true};
   bool weaponHungHdr {true};
   bool weaponDetonateHdr {true};
   bool gunFiredHdr {true};
   bool trackNewHdr {true};
   bool trackRemovedHdr {true};
   bool trackDataHdr {true};
   bool markerHdr {true};
   bool inputDeviceHdr {true};

   // Group headers
   bool playerHeader {true};
   bool weaponHeader {true};
   bool trackHeader {true};

   MsgHdrOptions option {MsgHdrOptions::NO_HDR};        // options for printing field names
   unsigned int lastMessage {REID_UNHANDLED_ID_TOKEN};  // previous message printed
   const char* divider {"\t"};                          // divider between message fields
};

inline bool TabPrinter::isSimReset() const { return simReset; }

}
}

#endif
