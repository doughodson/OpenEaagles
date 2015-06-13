
#include "openeaagles/recorder/TabPrinter.h"
#include "openeaagles/recorder/protobuf/DataRecord.pb.h"
#include "openeaagles/recorder/DataRecordHandle.h"

//#include "openeaagles/simulation/recorderTokens.h"
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/Nav.h"

// Disable all deprecation warnings for now.  Until we fix them,
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Recorder {


//==============================================================================
// Class TabPrinter
//==============================================================================
IMPLEMENT_SUBCLASS(TabPrinter,"TabPrinter")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(TabPrinter)
   "msgHdrOptn",   // 1) Msg Header options (see TabPrinter.h)
   "divider",      // 2) Field divider
END_SLOTTABLE(TabPrinter)

// Map slot table to handles
BEGIN_SLOT_MAP(TabPrinter)
   ON_SLOT( 1, setSlotMsgHdr,      Basic::String)
   ON_SLOT( 2, setSlotDivider,     Basic::String)
END_SLOT_MAP()

EMPTY_SERIALIZER(TabPrinter)
EMPTY_DELETEDATA(TabPrinter)

//------------------------------------------------------------------------------
// Default Constructor
//------------------------------------------------------------------------------
TabPrinter::TabPrinter()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void TabPrinter::initData()
{
   printHeader = false;   // print either Header or message values
   simReset = true;
   setMsgHeaders(true);
   option = NO_HDR;
   lastMessage = REID_UNHANDLED_ID_TOKEN;
   divider = "\t";
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TabPrinter::copyData(const TabPrinter& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   printHeader = org.printHeader;
   option = org.option;
   divider = org.divider;
}

//------------------------------------------------------------------------------
// setMsgHeaders() -- Set message header print flags
//------------------------------------------------------------------------------
void TabPrinter::setMsgHeaders(const bool f)
{
   fileIdHdr = f;
   playerNewHdr = f;
   playerRemovedHdr = f;
   playerDataHdr = f;
   playerDamagedHdr = f;
   playerCollisionHdr = f;
   playerCrashHdr = f;
   playerKilledHdr = f;
   weaponReleaseHdr = f;
   weaponHungHdr = f;
   weaponDetonateHdr = f;
   gunFiredHdr = f;
   trackNewHdr = f;
   trackRemovedHdr = f;
   trackDataHdr = f;
   markerHdr = f;
   inputDeviceHdr = f;

   playerHeader = f;
   weaponHeader = f;
   trackHeader = f;
}

//------------------------------------------------------------------------------
// slot functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// setSlotMsgHdr
//------------------------------------------------------------------------------
bool TabPrinter::setSlotMsgHdr(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {

      if ((*msg == "NO_HDR") || (*msg == "no_hdr") ) {
         option = NO_HDR;
         ok = true;
      }
      else if ((*msg == "ALL_MSGS") || (*msg == "all_msgs") ) {
         option = ALL_MSGS;
         ok = true;
      }
      else if ((*msg == "NEW_MSG") || (*msg == "new_msg") ) {
         option = NEW_MSG;
         ok = true;
      }
      else if ((*msg == "ON_CHANGE") || (*msg == "on_change") ) {
         option = ON_CHANGE;
         ok = true;
      }

      if (!ok && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "TabPrinter::setSlotMsgHdr(): Invalid header option type: " << *msg;
         std::cerr << ", use: { NO_HDR, ALL_MSGS, NEW_MSG, ON_CHANGE }" << std::endl;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotDivider
//------------------------------------------------------------------------------
bool TabPrinter::setSlotDivider(const Basic::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = true;
      divider = msg->getCopyString();
   }

   return ok;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* TabPrinter::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// Print the data from a DataRecord
//------------------------------------------------------------------------------
void TabPrinter::processRecordImp(const DataRecordHandle* const handle)
{
   if (handle == nullptr) return;  // cannot continue
   const Pb::DataRecord* dataRecord = handle->getRecord();
   if (dataRecord == nullptr) return;  // cannot continue

   // Get the time msg
   const Pb::Time* timeMsg = nullptr;
   if (dataRecord->has_time()) {
      timeMsg = &dataRecord->time();
   }

   // Get message id
   unsigned int messageId = dataRecord->id();
   // See if we print the header or not:
   printHeader = false;
   if (simReset) {
      lastMessage = REID_UNHANDLED_ID_TOKEN;
      simReset = false;
      printHeader = true;
      setMsgHeaders(true);

   }
   if (option == NO_HDR) {
      printHeader = false;
   }
   else if (option == ALL_MSGS) {
      printHeader = true;
   }
   else if (option == ON_CHANGE) {
      if (messageId != lastMessage) {
         printHeader = true;
      }
      else printHeader = false;
   }
   else if (option == NEW_MSG) {
      // Has this header been printed before
      printHeader = false;  // need to check each message

   }

   switch (messageId) {

      case REID_FILE_ID : {
         if (dataRecord->has_file_id_msg()) {
            if ((option == NEW_MSG) && (fileIdHdr)) printHeader = true;
            fileIdHdr = false;
            const Pb::FileIdMsg* msg = &dataRecord->file_id_msg();
            printFileIdMsg(timeMsg, msg);
         }
         break;
      }

      // events:
      case REID_NEW_PLAYER : {
         if (dataRecord->has_new_player_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::NewPlayerEventMsg* msg = &dataRecord->new_player_event_msg();
            printNewPlayerEventMsg(timeMsg, msg);
         }
         break;
      }

      case REID_PLAYER_REMOVED : {
         if (dataRecord->has_player_removed_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerRemovedEventMsg* msg = &dataRecord->player_removed_event_msg();
            printPlayerRemovedEventMsg(timeMsg, msg);
         }
         break;
      }

      case REID_PLAYER_DATA : {
         if (dataRecord->has_player_data_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerDataMsg* msg = &dataRecord->player_data_msg();
            printPlayerDataMsg(timeMsg, msg);
         }
         break;
      }
     case REID_PLAYER_DAMAGED : {
        if (dataRecord->has_player_damaged_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerDamagedEventMsg* msg = &dataRecord->player_damaged_event_msg();
            printPlayerDamagedEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_PLAYER_COLLISION : {
         if (dataRecord->has_player_collision_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerCollisionEventMsg* msg = &dataRecord->player_collision_event_msg();
            printPlayerCollisionEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_PLAYER_CRASH : {
         if (dataRecord->has_player_crash_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerCrashEventMsg* msg = &dataRecord->player_crash_event_msg();
            printPlayerCrashEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_PLAYER_KILLED : {
         if (dataRecord->has_player_killed_event_msg()) {
            if ((option == NEW_MSG) && (playerHeader)) printHeader = true;
            playerHeader = false;
            const Pb::PlayerKilledEventMsg* msg = &dataRecord->player_killed_event_msg();
            printPlayerKilledEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_WEAPON_RELEASED : {
         if (dataRecord->has_weapon_release_event_msg()) {
            if ((option == NEW_MSG) && (weaponHeader)) printHeader = true;
            weaponHeader = false;
            const Pb::WeaponReleaseEventMsg* msg = &dataRecord->weapon_release_event_msg();
            printWeaponReleaseEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_WEAPON_HUNG : {
         if (dataRecord->has_weapon_hung_event_msg()) {
            if ((option == NEW_MSG) && (weaponHeader)) printHeader = true;
            weaponHeader = false;
            const Pb::WeaponHungEventMsg* msg = &dataRecord->weapon_hung_event_msg();
            printWeaponHungEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_WEAPON_DETONATION : {
         if (dataRecord->has_weapon_detonation_event_msg()) {
            if ((option == NEW_MSG) && (weaponHeader)) printHeader = true;
            weaponHeader = false;
            const Pb::WeaponDetonationEventMsg* msg = &dataRecord->weapon_detonation_event_msg();
            printWeaponDetonationEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_GUN_FIRED : {
         if (dataRecord->has_gun_fired_event_msg()) {
            if ((option == NEW_MSG) && (gunFiredHdr)) printHeader = true;
            gunFiredHdr = false;
            const Pb::GunFiredEventMsg* msg = &dataRecord->gun_fired_event_msg();
            printGunFiredEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_NEW_TRACK : {
         if (dataRecord->has_new_track_event_msg()) {
            if ((option == NEW_MSG) && (trackHeader)) printHeader = true;
            trackHeader = false;
            const Pb::NewTrackEventMsg* msg = &dataRecord->new_track_event_msg();
            printNewTrackEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_TRACK_REMOVED : {
         if (dataRecord->has_track_removed_event_msg()) {
            if ((option == NEW_MSG) && (trackHeader)) printHeader = true;
            trackHeader = false;
            const Pb::TrackRemovedEventMsg* msg = &dataRecord->track_removed_event_msg();
            printTrackRemovedEventMsg(timeMsg, msg);
         }
         break;
      }
      case REID_TRACK_DATA : {
         if (dataRecord->has_track_data_msg()) {
            if ((option == NEW_MSG) && (trackHeader)) printHeader = true;
            trackHeader = false;
            const Pb::TrackDataMsg* msg = &dataRecord->track_data_msg();
            printTrackDataMsg(timeMsg, msg);
         }
         break;
      }
      case REID_UNHANDLED_ID_TOKEN: {
         // unknown event with valid ID and time fields.
         printUnhandledIdToken(timeMsg);
         break;
      }
      case REID_MARKER: {
         // Marker msg with ID, source, and time fields.
         if (dataRecord->has_marker_msg()) {
            if ((option == NEW_MSG) && (markerHdr)) printHeader = true;
            markerHdr = false;
            const Pb::MarkerMsg* msg = &dataRecord->marker_msg();
            printMarkerMsg(timeMsg, msg);
         }
         break;
      }
      case REID_AI_EVENT: {
         // Analog Input device msg with ID, source, value, and time fields.
         if (dataRecord->has_input_device_msg()) {
            if ((option == NEW_MSG) && (inputDeviceHdr)) printHeader = true;
            inputDeviceHdr = false;
            const Pb::InputDeviceMsg* msg = &dataRecord->input_device_msg();
            printInputDeviceMsg(timeMsg, msg, messageId);
         }
         break;
      }
      case REID_DI_EVENT: {
         // Discrete Input device msg with ID, source, value, and time fields.
         if (dataRecord->has_input_device_msg()) {
            if ((option == NEW_MSG) && (inputDeviceHdr)) printHeader = true;
            inputDeviceHdr = false;
            const Pb::InputDeviceMsg* msg = &dataRecord->input_device_msg();
            printInputDeviceMsg(timeMsg, msg, messageId);
         }
         break;
      }
      case REID_END_OF_DATA: {
         // Last record message
         printEndOfData(timeMsg);
         break;
      }
      case REID_RESET_EVENT: {
         // Reset
         printResetEvent(timeMsg);

         // indicate reset so we print message headings
         simReset = true;
         break;
      }
      default: {
         // not an Eaagles message.
         break;
      }
   }

   lastMessage = messageId;
}

//------------------------------------------------------------------------------
// Print functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// printFileIdMsg
//-----------------------------------------------------------------------------
void TabPrinter::printFileIdMsg(const Pb::Time* const timeMsg, const Pb::FileIdMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "FILE ID" << divider << "IDENTIFIER" << divider << "HEADER"  << divider;
      printTimeMsgHdr(sout);
      sout << "Event name" << divider <<
         "Application" << divider <<
         "Case" << divider <<
         "Mission" << divider <<
         "Subject" << divider <<
         "Run" << divider <<
         "Day" << divider <<
         "Month" << divider <<
         "Year";
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "FILE ID" << divider << "IDENTIFIER" << divider << "DATA"  << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // File Id
      if (msg->has_event_name()) {
         sout << msg->event_name() << divider;
      }
      else sout << divider;

      if (msg->has_application()) {
         sout << msg->application() << divider;
      }
      else sout << divider;

      if (msg->has_case_num()) {
         sout << msg->case_num() << divider;
      }
      else sout << divider;

      if (msg->has_mission_num()) {
         sout << msg->mission_num() << divider;
      }
      else sout << divider;

      if (msg->has_subject_num()) {
         sout << msg->subject_num() << divider;
      }
      else sout << divider;

      if (msg->has_run_num()) {
         sout << msg->run_num() << divider;
      }
      else sout << divider;

      if (msg->has_day()) {
         sout << msg->day() << divider;
      }
      else sout << divider;

      if (msg->has_month()) {
         sout << msg->month() << divider;
      }
      else sout << divider;

      if (msg->has_year()) {
         sout << msg->month() << divider;
      }
      else sout << divider;
   }
   else {
      // bad pointer, just print dividers
      for (unsigned int i = 0; i < 9; i++) {
         sout << divider;
      }
   }
   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printNewPlayerEventMsg
//--------------------------------------------------------------------------
void TabPrinter::printNewPlayerEventMsg(const Pb::Time* const timeMsg, const Pb::NewPlayerEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "NEW" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "PLAYER" << divider << "NEW" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);

   if (msg != nullptr) {
      // player ID
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Message
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);
      printPlayerDataSpacer(sout);
   }
   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerRemovedEventMsg
//--------------------------------------------------------------------------
void TabPrinter::printPlayerRemovedEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerRemovedEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "REMOVED" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

      sout << "PLAYER" << divider << "REMOVED" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);

   if (msg != nullptr) {
      // player ID
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Message
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);
      printPlayerDataSpacer(sout);
   }
   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerDataMsg
//--------------------------------------------------------------------------
void TabPrinter::printPlayerDataMsg(const Pb::Time* const timeMsg, const Pb::PlayerDataMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "DATA" << divider << "HEADER" << divider;

      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);
      printPlayerStateMsgHdr(sout);
      sout << "alpha" << divider
         << "beta" << divider
         << "cas" << divider;

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

      sout << "PLAYER" << divider << "DATA" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // player ID
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Message
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);

      if (msg->has_alpha()) {
         sout << msg->alpha() << divider;
      }
      else sout << divider;

      if (msg->has_beta()) {
         sout << msg->beta() << divider;
      }
      else sout << divider;

      if (msg->has_cas()) {
         sout << msg->cas() << divider;
      }
      else sout << divider;
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);
      printPlayerDataSpacer(sout);
      sout << divider << divider << divider;
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerDamagedEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerDamagedEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerDamagedEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "DAMAGE" << divider << "HEADER" << divider;

      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "PLAYER" << divider << "DAMAGE" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // player ID
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);
      printPlayerDataSpacer(sout);
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerCollisionEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerCollisionEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerCollisionEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "COLLISION" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // other player

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "PLAYER" << divider << "COLLISION" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // player ID
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State
      if (msg->has_state()) {
          printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);

      // Other Player ID
      if (msg->has_other_player_id()) {
         printPlayerIdMsg(sout, &msg->other_player_id());
      }
      else printPlayerIdSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // player
      printPlayerDataSpacer(sout);
      printPlayerIdSpacer(sout);    // other player
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerCrashEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerCrashEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerCrashEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "CRASH" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "PLAYER" << divider << "CRASH" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Player Id
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // player
      printPlayerDataSpacer(sout);
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printPlayerKilledEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerKilledEventMsg(const Pb::Time* const timeMsg, const Pb::PlayerKilledEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "PLAYER" << divider << "KILL" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // player
      printPlayerStateMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // shooter

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "PLAYER" << divider << "KILL" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Player Id
      if (msg->has_id()) {
         printPlayerIdMsg(sout, &msg->id());
      }
      else printPlayerIdSpacer(sout);

      // Player State
      if (msg->has_state()) {
         printPlayerStateMsg(sout, &msg->state());
      }
      else printPlayerDataSpacer(sout);

      // Player Id Shooter
      if (msg->has_shooter_id()) {
         printPlayerIdMsg(sout, &msg->shooter_id());
      }
      else printPlayerIdSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // player
      printPlayerDataSpacer(sout);
      printPlayerIdSpacer(sout);    // shooter
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printWeaponReleaseEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printWeaponReleaseEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponReleaseEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "WEAPON" << divider << "RELEASE" << divider << "HEADER" << divider;
      printWeaponMsgHdr(sout);
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "WEAPON" << divider << "RELEASE" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Player Id Wpn
      if (msg->has_wpn_id()) {
         printPlayerIdMsg(sout, &msg->wpn_id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Wpn
      if (msg->has_wpn_state()) {
         printPlayerStateMsg(sout, &msg->wpn_state());
      }
      else printPlayerDataSpacer(sout);

      // Player Id Shooter
      if (msg->has_shooter_id()) {
         printPlayerIdMsg(sout, &msg->shooter_id());
      }
      else printPlayerIdSpacer(sout);

      // Player Id Target
      if (msg->has_tgt_id()) {
         printPlayerIdMsg(sout, &msg->tgt_id());
      }
      else printPlayerIdSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // player
      printPlayerDataSpacer(sout);
      printPlayerIdSpacer(sout);    // shooter
      printPlayerIdSpacer(sout);    // target
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printWeaponHungEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printWeaponHungEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponHungEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "WEAPON" << divider << "HUNG" << divider << "HEADER" << divider;
      printWeaponMsgHdr(sout);
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "WEAPON" << divider << "HUNG" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Player Id Wpn
      if (msg->has_wpn_id()) {
         printPlayerIdMsg(sout, &msg->wpn_id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Wpn
      if (msg->has_wpn_state()) {
         printPlayerStateMsg(sout, &msg->wpn_state());
      }
      else printPlayerDataSpacer(sout);

      // Player Id Shooter
      if (msg->has_shooter_id()) {
         printPlayerIdMsg(sout, &msg->shooter_id());
      }
      else printPlayerIdSpacer(sout);

      // Player Id Target
      if (msg->has_tgt_id()) {
         printPlayerIdMsg(sout, &msg->tgt_id());
      }
      else printPlayerIdSpacer(sout);
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // wpn
      printPlayerDataSpacer(sout);
      printPlayerIdSpacer(sout);    // shooter
      printPlayerIdSpacer(sout);    // target
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printWeaponDetonationEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printWeaponDetonationEventMsg(const Pb::Time* const timeMsg, const Pb::WeaponDetonationEventMsg* const msg)
{
   std::stringstream sout;


   if (printHeader) {
      sout << "WEAPON" << divider << "DETONATION" << divider << "HEADER" << divider;
      printWeaponMsgHdr(sout);
      sout << "detonation type" << divider;
      sout << "missile Distance" << divider;
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "WEAPON" << divider << "DETONATION" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Player Id Wpn
      if (msg->has_wpn_id()) {
         printPlayerIdMsg(sout, &msg->wpn_id());
      }
      else printPlayerIdSpacer(sout);

      // Player State Wpn
      if (msg->has_wpn_state()) {
         printPlayerStateMsg(sout, &msg->wpn_state());
      }
      else printPlayerDataSpacer(sout);

      // Player Id Shooter
      if (msg->has_shooter_id()) {
         printPlayerIdMsg(sout, &msg->shooter_id());
      }
      else printPlayerIdSpacer(sout);

      // Player Id Target
      if (msg->has_tgt_id()) {
         printPlayerIdMsg(sout, &msg->tgt_id());
      }
      else printPlayerIdSpacer(sout);

      // Detonation type:
      if (msg->has_det_type()) {
         sout << msg->det_type() << divider;
      }
      else sout << divider;

      if (msg->has_miss_dist()) {
         // get missile distance
         sout << msg->miss_dist() << divider;
      }
      else sout << divider;
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);    // wpn
      printPlayerDataSpacer(sout);
      printPlayerIdSpacer(sout);    // shooter
      printPlayerIdSpacer(sout);    // target
      sout <<  divider;       // detonation type
      sout <<  divider;       // missile Distance
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printGunFiredEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printGunFiredEventMsg(const Pb::Time* const timeMsg, const Pb::GunFiredEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "GUN" << divider << "FIRED" << divider << "HEADER" << divider;

      printTimeMsgHdr(sout);
      printPlayerIdMsgHdr(sout);  // launcher
      sout << "rounds" << divider;

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "GUN" << divider << "FIRED" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // player ID shooter
      if (msg->has_shooter_id()) {
         printPlayerIdMsg(sout, &msg->shooter_id());
      }
      else printPlayerIdSpacer(sout);

      // rounds
      if (msg->has_rounds()) {
         sout << msg->rounds() << divider;
      }
      else sout << divider;    // (1 spacer field)
   }
   else {
      // bad pointer, just print spacers
      printPlayerIdSpacer(sout);
      sout << divider;
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printNewTrackEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printNewTrackEventMsg(const Pb::Time* const timeMsg, const Pb::NewTrackEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "TRACK" << divider << "ADDED" << divider << "HEADER" << divider;
      printTrackMsgHdr(sout);

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "TRACK" << divider << "ADDED" << divider << "DATA" << divider;
   printTimeMsg(sout, timeMsg);

    {
      if (msg != nullptr) {
         // print values
         // Track ID
         if (msg->has_track_id()) {
            sout << msg->track_id() << divider;
         }
         else sout << divider;

         // player ID
         if (msg->has_player_id()) {
            printPlayerIdMsg(sout, &msg->player_id());
         }
         else printPlayerIdSpacer(sout);

         // common track data
         if (msg->has_track_data()) {
            printCommonTrackDataMsg(sout, &msg->track_data());
         }
         else printTrackDataSpacer(sout);

         // player state
         if (msg->has_player_state()) {
            printPlayerStateMsg(sout, &msg->player_state());
         }
         else printPlayerDataSpacer(sout);

         // track player ID
         if (msg->has_trk_player_id()) {
            printPlayerIdMsg(sout, &msg->trk_player_id());
         }
         else printPlayerIdSpacer(sout);

         // track player state
         if (msg->has_trk_player_state()) {
            printPlayerStateMsg(sout, &msg->trk_player_state());
         }
         else printPlayerDataSpacer(sout);

         // emission data
         if (msg->has_emission_data()) {
            printEmissionDataMsg(sout, &msg->emission_data());
         }
         else printEmissionDataSpacer(sout);
      }
      else {
         // bad pointer; print spacers
         sout <<  divider;    // Track ID
         printPlayerIdSpacer(sout); // player ID header
         printTrackDataSpacer(sout);  // common track data
         printPlayerDataSpacer(sout);  // player state
         printPlayerIdSpacer(sout); // player ID header, track
         printPlayerDataSpacer(sout);  // player state, track
         printEmissionDataSpacer(sout);  // emission data
      }
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printTrackRemovedEventMsg
//------------------------------------------------------------------------------
void TabPrinter::printTrackRemovedEventMsg(const Pb::Time* const timeMsg, const Pb::TrackRemovedEventMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "TRACK" << divider << "REMOVED" << divider << "HEADER" << divider;
      printTrackMsgHdr(sout);
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "TRACK" << divider << "REMOVED" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      // Track ID
         if (msg->has_track_id()) {
            sout << msg->track_id() << divider;
         }
         else sout << divider;

      if (msg->has_player_id()) {
         printPlayerIdMsg(sout, &msg->player_id());
      }
      else printPlayerIdSpacer(sout);

   }
   else {
      // bad pointer; print spacers
      sout <<  divider;    // Track ID
      printPlayerIdSpacer(sout); // player ID header
      printTrackDataSpacer(sout);  // common track data
      printPlayerDataSpacer(sout);  // player state
      printPlayerIdSpacer(sout); // player ID header, track
      printPlayerDataSpacer(sout);  // player state, track
      printEmissionDataSpacer(sout);  // emission data
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printTrackDataMsg (event)
//------------------------------------------------------------------------------
void TabPrinter::printTrackDataMsg(const Pb::Time* const timeMsg, const Pb::TrackDataMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "TRACK" << divider << "UPDATE" << divider << "HEADER" << divider;
      printTrackMsgHdr(sout);

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "TRACK" << divider << "UPDATE" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   {
      if (msg != nullptr) {
         // print values
         // Track ID
         if (msg->has_track_id()) {
            sout << msg->track_id() << divider;
         }
         else sout << divider;

         // player ID
         if (msg->has_player_id()) {
            printPlayerIdMsg(sout, &msg->player_id());
         }
         else printPlayerIdSpacer(sout);

         // common track data
         if (msg->has_track_data()) {
            printCommonTrackDataMsg(sout, &msg->track_data());
         }
         else printTrackDataSpacer(sout);

         // player state
         if (msg->has_player_state()) {
            printPlayerStateMsg(sout, &msg->player_state());
         }
         else printPlayerDataSpacer(sout);

         // track player ID
         if (msg->has_trk_player_id()) {
            printPlayerIdMsg(sout, &msg->trk_player_id());
         }
         else printPlayerIdSpacer(sout);

         // track player state
         if (msg->has_trk_player_state()) {
            printPlayerStateMsg(sout, &msg->trk_player_state());
         }
         else printPlayerDataSpacer(sout);

         // emission data
         if (msg->has_emission_data()) {
            printEmissionDataMsg(sout, &msg->emission_data());
         }
         else printEmissionDataSpacer(sout);
      }
      else {
         // print spacers
         sout <<  divider;    // Track ID
         printPlayerIdSpacer(sout); // player ID header
         printTrackDataSpacer(sout);  // common track data
         printPlayerDataSpacer(sout);  // player state
         printPlayerIdSpacer(sout); // player ID header, track
         printPlayerDataSpacer(sout);  // player state, track
         printEmissionDataSpacer(sout);  // emission data
      }
   }

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printTrackMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printTrackMsgHdr(std::ostream& sout)
{
      printTimeMsgHdr(sout);

      sout << "TRACK ID" << divider;

      sout << "Player: " ;
      printPlayerIdMsgHdr(sout); // player ID header

      printTrackDataHdr(sout);  // common track data

      printPlayerStateMsgHdr(sout);  // player state

      sout << "Trk Player: " ;
      printPlayerIdMsgHdr(sout); // player ID header
      printPlayerStateMsgHdr(sout);  // player stat
      printEmissionDataMsgHdr(sout);  // emission data
}


//------------------------------------------------------------------------------
// printTimeMsg
//------------------------------------------------------------------------------
void TabPrinter::printTimeMsg(std::ostream& sout, const Pb::Time* const timeMsg)
{

   // print time message values:
   if (timeMsg != nullptr) {
      if (timeMsg->has_exec_time()) {
         printExecTimeMsg(sout, timeMsg->exec_time());
         sout << divider;
      }
      else sout << divider;

      if (timeMsg->has_sim_time()) {
         printSimTimeMsg(sout, timeMsg->sim_time());
         sout << divider;
      }
      else sout  << divider;

      if (timeMsg->has_utc_time()) {
         printUtcTimeMsg(sout, timeMsg->utc_time());
         sout << divider;
      }
      else sout << divider;
   }
   else sout << divider << divider << divider;

}

//------------------------------------------------------------------------------
// printTimeMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printTimeMsgHdr(std::ostream& sout)
{
   // print time message:
   // header
   sout << "Exec time" << divider;
   sout << "Sim Time" << divider;
   sout << "Utc Time"<< divider;
}

//------------------------------------------------------------------------------
// printPlayerIdMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerIdMsg(std::ostream& sout, const Pb::PlayerId* const msg)
{
   // values
   if (msg != nullptr) {
      if (msg->has_id()) {
         // player ID
         if (msg->has_id()) {
            sout << msg->id() << "           " << divider;
         }
         else sout << " " << "           " << divider;

         if (msg->has_fed_name()) {
            // federate name
            sout << msg->fed_name() << "    " << divider;
         }
         else sout << "   " <<  divider;

         if (msg->has_name()) {
            // player name
            sout << msg->name() << "           " << divider;
         }
         else sout << " " << "           " << divider;

         if (msg->has_side()) {
            // player side
            sout << msg->side() << "           " << divider;
      }
         else sout << " " << "           " << divider;

      }
   }
   else sout << divider << divider << divider << divider;

}

//------------------------------------------------------------------------------
// printPlayerIdMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printPlayerIdMsgHdr(std::ostream& sout)
{
   // header
   sout << "player ID"               << divider;
   sout << "federate name"           << divider;
   sout << "network ID"              << divider;  // player name
}

//------------------------------------------------------------------------------
// printPlayerIdSpacer() -- creates empty space for the player ID message
//------------------------------------------------------------------------------
void TabPrinter::printPlayerIdSpacer(std::ostream& sout)
{
    sout << divider;    // player ID
    sout << divider;    // player name
    sout << divider;    // federated name
}

//------------------------------------------------------------------------------
// printPlayerStateMsg
//------------------------------------------------------------------------------
void TabPrinter::printPlayerStateMsg(std::ostream& sout, const Pb::PlayerState* const msg)
{
   // Player State
   if (msg != nullptr) {
      // position
      if (msg->has_pos()) {

         // Convert x,y,z to lat/lon:
         double pLat(0.0);
         double pLon(0.0);
         double pAlt(0.0);
         if (( msg->pos().has_x()) && ( msg->pos().has_y()) && ( msg->pos().has_z())) {
            Eaagles::Basic::Nav::convertEcef2Geod(msg->pos().x(), msg->pos().y(),  msg->pos().z(),
               &pLat, &pLon, &pAlt);
            sout << pLat << divider << pLon << divider << pAlt << divider;
         }
         if (msg->pos().has_x()) {
            sout << msg->pos().x() << divider;
         }
         else sout << divider;

         if (msg->pos().has_y()) {
            sout << msg->pos().y() << divider;
         }
         else sout << divider;

         if (msg->pos().has_z()) {
            sout << msg->pos().z() << divider;
         }
         else sout << divider;
      }
      else sout << divider << divider << divider << divider;

      // angles (convert to degrees)
      if (msg->has_angles()) {
         if (msg->angles().has_x()) {
            sout << msg->angles().x() * Basic::Angle::R2DCC << divider;
         }
         else sout << divider;

         if (msg->angles().has_y()) {
            sout << msg->angles().y() * Basic::Angle::R2DCC << divider;
         }
         else sout << divider;

         if (msg->angles().has_z()) {
            sout << msg->angles().z() * Basic::Angle::R2DCC << divider;
         }
         else sout << divider;
      }
      else sout <<  divider << divider << divider << divider;

      // velocity
      if (msg->has_vel()) {
         if (msg->vel().has_x()) {
            sout << msg->vel().x() << divider;
         }
         else sout << divider;

         if (msg->vel().has_y()) {
            sout << msg->vel().y() << divider;
         }
         else sout << divider;

         if (msg->vel().has_z()) {
            sout << msg->vel().z() << divider;
         }
         else sout << divider;
      }
      else sout <<  divider << divider << divider << divider;

      if (msg->has_damage()) {
         sout << msg->damage() << divider;
      }
      else sout << divider;
   }
   else printPlayerDataSpacer(sout);
}

//------------------------------------------------------------------------------
// printPlayerStateMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printPlayerStateMsgHdr(std::ostream& sout)
{
   // header
   sout << "Latitude" << divider << "Longitude" << divider << "Altitude" << divider;
   sout << "X position" << divider
         << "Y position" << divider
         << "Z position" << divider;
   sout << "X velocity" << divider
         << "Y velocity" << divider
         << "Z velocity" << divider;
   sout << "X orientation" << divider
         << "Y orientation" << divider
         << "Z orientation" << divider;
   sout << "damage" << divider;
}

//------------------------------------------------------------------------------
// printPlayerDataSpacer() -- creates empty space for the player ID message
//------------------------------------------------------------------------------
void TabPrinter::printPlayerDataSpacer(std::ostream& sout)
{
   sout << divider       // X position
        << divider       // Y position
        << divider;      // Z position
   sout << divider       // X angle
        << divider       // Y angle
        << divider;      // X angle
   sout << divider       // X velocity
        << divider       // Y velocity
        << divider;      // Z velocity
   sout << divider;      // damage
}

//------------------------------------------------------------------------------
// printWeaponMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printWeaponMsgHdr(std::ostream& sout)
{
   // print common weapon message header:
   printTimeMsgHdr(sout);
   printPlayerIdMsgHdr(sout);     // weapon
   printPlayerStateMsgHdr(sout);
   printPlayerIdMsgHdr(sout);     // shooter
   printPlayerIdMsgHdr(sout);     // target
}

//------------------------------------------------------------------------------
// printCommonTrackDataMsg
//------------------------------------------------------------------------------
void TabPrinter::printCommonTrackDataMsg(std::ostream& sout, const Pb::TrackData* const msg)
{
   // Track Data

   if (msg != nullptr) {
      if (msg->has_type()) {
         sout << msg->type() << divider;
      }
      else sout << divider;

      if (msg->has_quality()) {
         sout << msg->quality() << divider;
      }
      else sout << divider;

      if (msg->has_true_az()) {
         sout << msg->true_az() * Basic::Angle::R2DCC << divider;
      }
      else sout << divider;

      if (msg->has_rel_az()) {
         sout << msg->rel_az() * Basic::Angle::R2DCC << divider;
      }
      else sout << divider;

      if (msg->has_elevation()) {
         sout << msg->elevation() * Basic::Angle::R2DCC << divider;
      }
      else sout << divider;

      if (msg->has_range()) {
         sout << msg->range() << divider;
      }
      else sout << divider;

      if (msg->has_latitude()) {
         sout << msg->latitude() << divider;
      }
      else sout << divider;

      if (msg->has_longitude()) {
         sout << msg->longitude() << divider;
      }
      else sout << divider;

      if (msg->has_altitude()) {
         sout << msg->altitude() << divider;
      }
      else sout << divider;

      if (msg->has_position()) {
         // vector
         if (msg->position().has_x()) sout << msg->position().x() << divider;
         else sout << divider;
         if (msg->position().has_y()) sout << msg->position().y() << divider;
         else sout << divider;
         if (msg->position().has_z()) sout << msg->position().z() << divider;
         else sout << divider;
      }
      else sout <<  divider << divider << divider;

      if (msg->has_velocity()) {
         // vector
         if (msg->velocity().has_x()) sout << msg->velocity().x() << divider;
         else sout << divider;
         if (msg->velocity().has_y()) sout << msg->velocity().y() << divider;
         else sout << divider;
         if (msg->velocity().has_z()) sout << msg->velocity().z() << divider;
         else sout << divider;
      }
      else sout <<  divider << divider << divider;

      if (msg->has_avg_signal()) {
         sout << msg->avg_signal() << divider;
      }
      else sout << divider;

      if (msg->has_sl_index()) {
         sout << msg->sl_index() << divider;
      }
      else sout << divider;

      if (msg->has_wpn_rel()) {
         sout << msg->wpn_rel() << divider;
      }
      else sout << divider;
   }
   else printTrackDataSpacer(sout);

}

//------------------------------------------------------------------------------
// printTrackDataHdr (common track data)
//------------------------------------------------------------------------------
void TabPrinter::printTrackDataHdr(std::ostream& sout)
{
   // Track Data Header
   sout << "type" << divider;
   sout << "quality" << divider;
   sout << "true AZ" << divider;
   sout << "rel AZ" << divider;
   sout << "elevation" << divider;
   sout << "range" << divider;
   sout << "latitude" << divider;
   sout << "longitude" << divider;
   sout << "altitude" << divider;
   sout << "position x" << divider;
   sout << "position y" << divider;
   sout << "position z" << divider;
   sout << "velocity x" << divider;
   sout << "velocity y" << divider;
   sout << "velocity z" << divider;
   sout << "avg signal" << divider;
   sout << "shoot list index" << divider;
   sout << "weapon rel" << divider;
}

//------------------------------------------------------------------------------
// printTrackDataSpacer (for common track data)
//------------------------------------------------------------------------------
void TabPrinter::printTrackDataSpacer(std::ostream& sout)
{
   // Track Data Header Spacers
   sout <<  divider;              // type
   sout <<  divider;              // quality
   sout <<  divider;              // true AZ
   sout <<  divider;              // rel AZ
   sout <<  divider;              // elevation
   sout <<  divider;              // range
   sout <<  divider;              // latitude
   sout <<  divider;              // longitude
   sout <<  divider;              // altitude
   sout <<  divider << divider << divider;           // position
   sout <<  divider << divider << divider;           // velocity
   sout <<  divider;              // avg signal
   sout <<  divider;              // shoot list index
   sout <<  divider;              // weapon rel
}

//------------------------------------------------------------------------------
// printEmissionDataMsg
//------------------------------------------------------------------------------
void TabPrinter::printEmissionDataMsg(std::ostream& sout, const Pb::EmissionData* const msg)
{
   if (msg != nullptr) {
      // emission data values
      if (msg->has_frequency()) {
         sout << msg->frequency() << divider;
      }
      else sout << " " << divider;
      if (msg->has_wave_length()) {
         sout <<  msg->wave_length() << divider;
      }
      else sout << " " << divider;
      if (msg->has_pulse_width()) {
         sout <<  msg->pulse_width() << divider;
      }
      else sout << " " << divider;
      if (msg->has_bandwidth()) {
         sout << msg->bandwidth() << divider;
      }
      else sout << " " << divider;
      if (msg->has_prf()) {
         sout << msg->prf() << divider;
      }
      else sout << " " << divider;
      if (msg->has_power()) {
         sout << msg->power() << divider;
      }
      else sout << " " << divider;
      if (msg->has_polarization()) {
         sout << msg->polarization() << divider;
      }
      else sout << " " << divider;
      if (msg->has_azimuth_aoi()) {
         // Convert to degrees
         sout << msg->azimuth_aoi() * Basic::Angle::R2DCC << divider;
      }
      else sout << " " << divider;
      if (msg->elevation_aoi()) {
         // Convert to degrees
         sout << msg->elevation_aoi() * Basic::Angle::R2DCC << divider;
      }
      else sout << " " << divider;
   }
   else printEmissionDataSpacer(sout);
}

//------------------------------------------------------------------------------
// printEmissionDataMsgHdr
//------------------------------------------------------------------------------
void TabPrinter::printEmissionDataMsgHdr(std::ostream& sout)
{
   // emission data header
   sout << "frequency" << divider;
   sout << "wave length" << divider;
   sout << "pulse width" << divider;
   sout << "bandwidth" << divider;
   sout << "PRF" << divider;
   sout << "power" << divider;
   sout << "polarization" << divider;
   sout << "azimuth AOI" << divider;
   sout << "elevation AOI " << divider;
}

//------------------------------------------------------------------------------
// printEmissionDataSpacer() -- print the tab spaces for emission data message
//------------------------------------------------------------------------------
void TabPrinter::printEmissionDataSpacer(std::ostream& sout)
{
   sout << divider;       //frequency
   sout << divider;       //wave length
   sout << divider;       //pulse width
   sout << divider;       //bandwidth
   sout << divider;       //PRF
   sout << divider;       //power
   sout << divider;       //polarization
   sout << divider;       //azimuth AOI
   sout << divider;       //elevation AOI
}


//------------------------------------------------------------------------------
// printMarkerMsg
//------------------------------------------------------------------------------
void TabPrinter::printMarkerMsg(const Pb::Time* const timeMsg, const Pb::MarkerMsg* const msg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "MARKER" << divider << "MESSAGE" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);  // time header
      sout << "ID    " << divider << "SOURCE ID" << divider;

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "MARKER" << divider << "MESSAGE" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      if (msg->has_id()) {
         sout << msg->id() << divider;
      }
      else sout << divider;

      if (msg->has_source_id()) {
         sout << msg->source_id() << divider;
      }
      else sout << divider;

   }
   else {
      // bad pointer; print spacers
      sout <<  divider;    // ID
      sout <<  divider;    // SOURCE ID
   }

   printToOutput( sout.str().c_str() );
}


//------------------------------------------------------------------------------
// printInputDeviceMsg
//------------------------------------------------------------------------------
void TabPrinter::printInputDeviceMsg(const Pb::Time* const timeMsg, const Pb::InputDeviceMsg* const msg, const unsigned int msgId)
{
   std::stringstream sout;
   std::string inputType = "";

   if (msgId == REID_AI_EVENT) inputType = "ANALOG ";
   else inputType = "DISCRETE";

   if (printHeader) {
      sout << "INPUT" << divider << "DEVICE" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);  // time header
      sout << "TYPE" << divider << "ID    " << divider << "SOURCE ID" << divider << "VALUE" << divider;

      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "INPUT"  << divider << "DEVICE" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);
   if (msg != nullptr) {
      sout << inputType << divider;  // analog or discrete
      if (msg->has_id()) {
         sout << msg->id() << divider;
      }
      else sout << divider;

      if (msg->has_source_id()) {
         sout << msg->source_id() << divider;
      }
      else sout << divider;

      if (msg->has_value()) {
         sout << msg->value() << divider;
      }
      else sout << divider;

   }
   else {
      // bad pointer; print spacers
      sout << divider;     // TYPE
      sout <<  divider;    // ID
      sout <<  divider;    // SOURCE ID
      sout <<  divider;    // VALUE
   }

   printToOutput( sout.str().c_str() );
}


//------------------------------------------------------------------------------
// printEndOfData
//------------------------------------------------------------------------------
void TabPrinter::printEndOfData(const Pb::Time* const timeMsg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "LAST " << divider << "MESSAGE" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);  // time header
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "LAST " << divider << "MESSAGE" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printUnhandledIdToken
//------------------------------------------------------------------------------
void TabPrinter::printUnhandledIdToken(const Pb::Time* const timeMsg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "UNHANDLED " << divider << "TOKEN" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);  // time header
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "UNHANDLED " << divider << "TOKEN" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printResetEvent
//------------------------------------------------------------------------------
void TabPrinter::printResetEvent(const Pb::Time* const timeMsg)
{
   std::stringstream sout;

   if (printHeader) {
      sout << "RESET " << divider << "EVENT" << divider << "HEADER" << divider;
      printTimeMsgHdr(sout);  // time header
      printToOutput( sout.str().c_str() );
      sout.str("");
   }

   sout << "RESET " << divider << "EVENT" << divider << "DATA" << divider;

   printTimeMsg(sout, timeMsg);

   printToOutput( sout.str().c_str() );
}

//------------------------------------------------------------------------------
// printExecTimeMsg() -- print the time string for EXEC time
//------------------------------------------------------------------------------
void TabPrinter::printExecTimeMsg(std::ostream& sout, double execTime)
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    double ss = 0;  // Sec

    // exec time
    Basic::Time::getHHMMSS(static_cast<LCreal>(execTime), &hh, &mm, &ss);
    std::sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;
}

//------------------------------------------------------------------------------
// printUtcTimeMsg() -- print the time string for UTC time
//------------------------------------------------------------------------------
void TabPrinter::printUtcTimeMsg(std::ostream& sout, double utcTime)
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // sim time
    Basic::Time::getHHMMSS(static_cast<LCreal>(utcTime), &hh, &mm, &ss);
    std::sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;
}

//------------------------------------------------------------------------------
// printSimTimeMsg() -- print the time string for Simulation time
//------------------------------------------------------------------------------
void TabPrinter::printSimTimeMsg(std::ostream& sout, double simTime)
{
    char cbuf[16];
    int hh = 0;     // Hours
    int mm = 0;     // Min
    LCreal ss = 0;  // Sec

    // utc time
    Basic::Time::getHHMMSS(static_cast<LCreal>(simTime), &hh, &mm, &ss);
    std::sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    sout << cbuf;
}

} // End Recorder namespace
} // End Eaagles namespace
