//------------------------------------------------------------------------------
// Class: PrintPlayer
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_PrintPlayer_H__
#define __Eaagles_Recorder_PrintPlayer_H__

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
      class TrackData; class EmissionData;
   }

//------------------------------------------------------------------------------
// Class:   PrintPlayer
// Description: Print player data
//
// Form name: PrintPlayer
// Slots: 
//   playerName  <Basic::String>  ! Player name 
//------------------------------------------------------------------------------
class PrintPlayer : public PrintHandler {
    DECLARE_SUBCLASS(PrintPlayer, PrintHandler)

public:

   PrintPlayer();

protected:

   virtual bool setName(const Basic::String* const msg);

   // OutputHandler class private functions
   virtual void processRecordImp(const DataRecordHandle* const handle);

private:
   void initData();

   const Basic::String* name;    // Player name
};

} // End Recorder namespace
} // End Eaagles namespace

#endif
