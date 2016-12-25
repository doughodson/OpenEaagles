
#include "openeaagles/recorder/PrintPlayer.hpp"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.hpp"

#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/String.hpp"

#include <iomanip>

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace recorder {

//==============================================================================
// Class PrintPlayer
//==============================================================================
IMPLEMENT_SUBCLASS(PrintPlayer,"PrintPlayer")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(PrintPlayer)
   "playerName",   // 1) Player name
END_SLOTTABLE(PrintPlayer)

// Map slot table to handles
BEGIN_SLOT_MAP(PrintPlayer)
   ON_SLOT( 1, setName,        base::String)
END_SLOT_MAP()

EMPTY_SERIALIZER(PrintPlayer)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
PrintPlayer::PrintPlayer()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void PrintPlayer::initData()
{
   name = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void PrintPlayer::copyData(const PrintPlayer& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   { // clone player name
      const base::String* clone = nullptr;
      if (org.name != nullptr) clone = org.name->clone();
      setName(clone);
      if (clone != nullptr) clone->unref();
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void PrintPlayer::deleteData()
{
   setName(nullptr);
}

//------------------------------------------------------------------------------
// setName
//------------------------------------------------------------------------------
bool PrintPlayer::setName(const base::String* const msg)
{
   if (name != nullptr) { name->unref(); }
   name = msg;
   if (name != nullptr) { name->ref(); }
   return true;
}

//------------------------------------------------------------------------------
// Print the data
//------------------------------------------------------------------------------
void PrintPlayer::processRecordImp(const DataRecordHandle* const handle)
{
   if (handle == nullptr) return;  // cannot continue
   const pb::DataRecord* dataRecord = handle->getRecord();
   if (dataRecord == nullptr) return;  // cannot continue

   // Get the time msg
   const pb::Time* timeMsg = nullptr;
   if (dataRecord->has_time()) {
      timeMsg = &dataRecord->time();
   }

   // Get message id
   unsigned int messageId = dataRecord->id();
   enum MsgType { NEW, DATA, REMOVED, DAMAGE, COLLISION, CRASH, KILL, UNKNOWN };
   MsgType msgType = UNKNOWN;
   std::string msgTypeStr = "";

   const pb::PlayerId* playerIdMsg = nullptr;
   const pb::PlayerState* playerStMsg = nullptr;

   switch (messageId) {

      // events:
      case REID_NEW_PLAYER : {
         if (dataRecord->has_new_player_event_msg()) {
            msgType = NEW;
            msgTypeStr = "NEW";
            const pb::NewPlayerEventMsg* msg = &dataRecord->new_player_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
            std::string playerName = playerIdMsg->name();  // example
         }
         break;
      }

      case REID_PLAYER_REMOVED : {
         if (dataRecord->has_player_removed_event_msg()) {
            msgType = REMOVED;
            msgTypeStr = "REMOVED";
            const pb::PlayerRemovedEventMsg* msg = &dataRecord->player_removed_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
      }

      case REID_PLAYER_DATA : {
         if (dataRecord->has_player_data_msg()) {
            msgType = DATA;
            msgTypeStr = "DATA";
            const pb::PlayerDataMsg* msg = &dataRecord->player_data_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
      }
      case REID_PLAYER_DAMAGED : {
         if (dataRecord->has_player_damaged_event_msg()) {
            msgType = DAMAGE;
            msgTypeStr = "DAMAGE";
            const pb::PlayerDamagedEventMsg* msg = &dataRecord->player_damaged_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
     }
     case REID_PLAYER_COLLISION : {
         if (dataRecord->has_player_collision_event_msg()) {
            msgType = COLLISION;
            msgTypeStr = "COLLISION";
            const pb::PlayerCollisionEventMsg* msg = &dataRecord->player_collision_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
      }
      case REID_PLAYER_CRASH : {
         if (dataRecord->has_player_crash_event_msg()) {
            msgType = CRASH;
            msgTypeStr = "CRASH";
            const pb::PlayerCrashEventMsg* msg = &dataRecord->player_crash_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
      }
      case REID_PLAYER_KILLED : {
         if (dataRecord->has_player_killed_event_msg()) {
            msgType = KILL;
            msgTypeStr = "KILL";
            const pb::PlayerCrashEventMsg* msg = &dataRecord->player_crash_event_msg();
            playerIdMsg = &msg->id();
            playerStMsg = &msg->state();
         }
         break;
      }
      default: {
         // not a message handled here.
         msgType = UNKNOWN;
         return;
         break;
      }
   }

   // if message handled, continue
   if (msgType != UNKNOWN) {

      bool printIt = (name == nullptr);
      if (!printIt) {
         if (playerIdMsg != nullptr && playerIdMsg->has_name()) {
            const char* sname = playerIdMsg->name().c_str();
            printIt = (*name == sname);
         }
      }

      if (printIt) {
         std::stringstream sout;

         // Print the Message Type
         sout << "PLAYER " << msgTypeStr << "     ";

         // Print the Player ID data
         if (playerIdMsg->has_id()) sout << playerIdMsg->id() << ";  ";
         if (playerIdMsg->has_name()) sout << playerIdMsg->name() << ";  ";
         if (playerIdMsg->has_fed_name()) sout << playerIdMsg->fed_name() << ";  ";

         // Angles:
         if (playerStMsg->has_angles()) {
            if ((playerStMsg->angles().has_x()) &&
               (playerStMsg->angles().has_y()) &&
               (playerStMsg->angles().has_z())) {
                  sout << playerStMsg->angles().x() << ", "
                     << playerStMsg->angles().y() << ", "
                     << playerStMsg->angles().z() << ";  ";
            }
         }

         // Velocity:
         if (playerStMsg->has_vel()) {
            if ((playerStMsg->vel().has_x()) &&
               (playerStMsg->vel().has_y()) &&
               (playerStMsg->vel().has_z())) {
                  sout << playerStMsg->vel().x() << ", "
                     << playerStMsg->vel().y() << ", "
                     << playerStMsg->vel().z() << ";  ";
            }
         }

         // Position:
         if (playerStMsg->has_pos()) {
            if ((playerStMsg->pos().has_x()) &&
               (playerStMsg->pos().has_y()) &&
               (playerStMsg->pos().has_z())) {
                  sout << playerStMsg->pos().x() << ", "
                     << playerStMsg->pos().y() << ", "
                     << playerStMsg->pos().z() << ";  ";
            }
         }

         printToOutput( sout.str().c_str() );
      }
   }
}


}
}
