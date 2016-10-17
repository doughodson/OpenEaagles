
#ifndef __oe_recorder_PrintPlayer_H__
#define __oe_recorder_PrintPlayer_H__

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
      class TrackData; class EmissionData;
   }

//------------------------------------------------------------------------------
// Class: PrintPlayer
// Description: Print player data
//
// Factory name: PrintPlayer
// Slots:
//   playerName  <base::String>  ! Player name
//------------------------------------------------------------------------------
class PrintPlayer : public PrintHandler
{
    DECLARE_SUBCLASS(PrintPlayer, PrintHandler)

public:

   PrintPlayer();

protected:

   virtual bool setName(const base::String* const msg);

   virtual void processRecordImp(const DataRecordHandle* const handle) override;

private:
   void initData();

   const base::String* name;    // Player name
};

}
}

#endif
