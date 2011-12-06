#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/Ntm.h"

#include "openeaagles/simulation/Aam.h"
#include "openeaagles/simulation/Agm.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Bomb.h"
#include "openeaagles/simulation/Effects.h"
#include "openeaagles/simulation/GroundVehicle.h"
#include "openeaagles/simulation/Guns.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Sam.h"
#include "openeaagles/simulation/SamVehicles.h"
#include "openeaagles/simulation/Ships.h"
#include "openeaagles/simulation/Signatures.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

#include "openeaagles/basic/Decibel.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

#if !defined(WIN32)
#include <arpa/inet.h>
#endif

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: NetIO
//==============================================================================

IMPLEMENT_ABSTRACT_SUBCLASS(NetIO,"NetIO")

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
const double NET_TIMEOUT          = 12.5;                //  seconds
const double NET_UPDATE_RATE      = 5.0;                 //  seconds
const double NET_THRESHOLD_MTR    = 3.0;                 //  meters
const double NET_THRESHOLD_RAD    = (3.0 * PI/180.0);    //  radians

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(NetIO)
   "networkID",            //  1: Network ID (this interface)
   "federationName",       //  2: Federation name
   "federateName",         //  3: Name of this federate

   "enableInput",          //  4: Input Enable: accept players from this network
   "enableOutput",         //  5: Output enable: send players to this network
   "enableRelay",          //  6: Relay enable: send other network players to this network
   "timeline",             //  7: Source of the timeline { UTC or EXEC }
                           //     (default: UTC)

   "inputEntityTypes",     //  8: Input entity type mappers (Ntm objects that map players to entity types)
   "outputEntityTypes",    //  9: Output entity type mappers (Ntm objects that entity types to players)

   "maxTimeDR",            // 10: Max DR time
   "maxPositionError",     // 11: Max DR position error
   "maxOrientationError",  // 12: Max DR anglular error
   "maxAge",               // 13: Max age (without update) of networked players
   "maxEntityRange",       // 14: Max enity range of networked players
END_SLOTTABLE(NetIO)

// Map slot table to handles
BEGIN_SLOT_MAP(NetIO)
   ON_SLOT(1,  setSlotNetworkID,          Basic::Number)
   ON_SLOT(2,  setSlotFederationName,     Basic::String)
   ON_SLOT(3,  setSlotFederateName,       Basic::String)

   ON_SLOT(4,  setSlotEnableInput,        Basic::Number)
   ON_SLOT(5,  setSlotEnableOutput,       Basic::Number)
   ON_SLOT(6,  setSlotEnableRelay,        Basic::Number)
   ON_SLOT(7,  setSlotTimeline,           Basic::Identifier)

   ON_SLOT(8,  setSlotInputEntityTypes,   Basic::PairStream)
   ON_SLOT(9,  setSlotOutputEntityTypes,  Basic::PairStream)

   ON_SLOT(10, setSlotMaxTimeDR,          Basic::Time)
   ON_SLOT(11, setSlotMaxPositionErr,     Basic::Distance)
   ON_SLOT(12, setSlotMaxOrientationErr,  Basic::Angle)
   ON_SLOT(13, setSlotMaxAge,             Basic::Time)
   ON_SLOT(14, setSlotMaxEntityRange,     Basic::Distance)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
NetIO::NetIO()
{
   STANDARD_CONSTRUCTOR()

   station = 0;
   simulation = 0;
   timeline = UTC;
   iffEventID = 0;
   emEventID = 0;
   netID = 1;  // default network ID

   inputFlg = true;
   outputFlg = true;
   relayFlg = true;
   netInit = false;
   netInitFail = false;

   // set the defaults
   setMaxEntityRange(0);                       // no range filtering
   setMaxTimeDR(NET_UPDATE_RATE);              //  (seconds)
   setMaxPositionErr(NET_THRESHOLD_MTR);       //  (meters)
   setMaxOrientationErr(NET_THRESHOLD_RAD);    //  (radians)
   setMaxAge(NET_TIMEOUT);                     //  (seconds)

   nInNibs = 0;
   nOutNibs = 0;

   for (unsigned int i = 0; i < MAX_ENTITY_TYPES; i++) {
      inputEntityTypes[i] = 0;
   }
   nInputEntityTypes = 0;

   for (unsigned int i = 0; i < MAX_ENTITY_TYPES; i++) {
      outputEntityTypes[i] = 0;
   }
   nOutputEntityTypes = 0;

   inputNtmTree = 0;
   outputNtmTree = 0;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NetIO::copyData(const NetIO& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {

      station = 0;
      simulation = 0;
      federateName = 0;
      federationName = 0;

      for (unsigned int i = 0; i < MAX_ENTITY_TYPES; i++) {
         inputEntityTypes[i] = 0;
      }
      nInputEntityTypes = 0;

      for (unsigned int i = 0; i < MAX_ENTITY_TYPES; i++) {
         outputEntityTypes[i] = 0;
      }
      nOutputEntityTypes = 0;

   }

   station = 0;
   simulation = 0;
   timeline = org.timeline;
   iffEventID = 0;
   emEventID = 0;

   netID = org.netID;

   const Basic::String* p1 = org.federateName;
   federateName = p1;
   const Basic::String* p2 = org.federationName;
   federationName = p2;

   inputFlg = org.inputFlg;
   outputFlg = org.outputFlg;
   relayFlg = org.relayFlg;
   netInit = org.netInit;
   netInitFail = org.netInitFail;

   setMaxEntityRange(org.maxEntityRange);
   setMaxTimeDR(org.maxTimeDR);
   setMaxPositionErr(org.maxPositionErr);
   setMaxOrientationErr(org.maxOrientationErr);
   setMaxAge(org.maxAge);

   nInNibs = 0;
   nOutNibs = 0;

   clearInputEntityTypes();
   for (unsigned int i = 0; i < org.nInputEntityTypes; i++) {
      Ntm* cp = (Ntm*) org.inputEntityTypes[i]->clone();
      addInputEntityType( cp );
      cp->unref();
   }

    clearOutputEntityTypes();
    for (unsigned int i = 0; i < org.nOutputEntityTypes; i++) {
      Ntm* cp = (Ntm*) org.outputEntityTypes[i]->clone();
      addOutputEntityType( cp );
      cp->unref();
    }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NetIO::deleteData()
{
   for (unsigned int i = 0; i < nInNibs; i++) {
      inputList[i]->unref();
      inputList[i] = 0;
   }
   nInNibs = 0;

   for (unsigned int i = 0; i < nOutNibs; i++) {
      outputList[i]->unref();
      outputList[i] = 0;
   }
   nOutNibs = 0;

   clearInputEntityTypes();
   clearOutputEntityTypes();

   station = 0;
   simulation = 0;
   federateName = 0;
   federationName = 0;

   netInit = false;
   netInitFail = false;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void NetIO::reset()
{
    // Initialization
    if (!isNetworkInitialized() && !didInitializationFail()) {
        networkInitialization();
    }

    //clearTracksAndQueues();
    BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool NetIO::shutdownNotification()
{
    for (unsigned int i = 0; i < nInNibs; i++) {
        inputList[i]->event(SHUTDOWN_EVENT);
    }

    for (unsigned int i = 0; i < nOutNibs; i++) {
        outputList[i]->event(SHUTDOWN_EVENT);
    }
    return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Data access (get) functions
//------------------------------------------------------------------------------

// getCurrentTime() -- Returns the current time from the selected source
double NetIO::getCurrentTime()
{
    if (getTimeline() == UTC) return getSimulation()->getSysTimeOfDay();
    else return getSimulation()->getExecTimeSec();
}

// Entity filter: Returns max entity ranged (meters)
LCreal NetIO::getMaxEntityRange(const Nib* const) const
{
   return maxEntityRange;
}

// Entity filter: Returns max entity ranged squared (meters^2)
LCreal NetIO::getMaxEntityRangeSquared(const Nib* const) const
{
   return maxEntityRange2;
}

// Dead-Reckoning: Returns max DR time before next 'heart beat' (seconds)
LCreal NetIO::getMaxTimeDR(const Nib* const) const
{
   return maxTimeDR;
    }

// Dead-Reckoning: Returns max DR position error (meters)
LCreal NetIO::getMaxPositionErr(const Nib* const) const
{
   return maxPositionErr;
}

// Dead-Reckoning: Returns max DR orientation error (radians)
LCreal NetIO::getMaxOrientationErr(const Nib* const) const
{
   return maxOrientationErr;
}

// Dead-Reckoning: Returns max age before a networked player is removed (seconds)
LCreal NetIO::getMaxAge(const Nib* const) const
{
   return maxAge;
}

// Federate name as String
const Basic::String* NetIO::getFederateName() const
{
   return federateName;
}

// Federation name as String
const Basic::String* NetIO::getFederationName() const
{
   return federationName;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

// Sets the Network's ID
bool NetIO::setNetworkID(const unsigned short v)
{
   bool ok = false;
   if (v >= 1 && v <= MAX_NETWORD_ID) {
      netID = v;
      ok = true;
   }
   return ok;
}

// Sets the timeline (UTC or EXEC)
bool NetIO::setTimeline(const TSource ts)
{
    timeline = ts;
    return true;
}

// Sets the max dead-rec time; forces next update (sec)
bool NetIO::setMaxTimeDR(const LCreal v)
{
   maxTimeDR = v;
   return true;
}

// Sets the max positional error (meters)
bool NetIO::setMaxPositionErr(const LCreal v)
{
   maxPositionErr = v;
   return true;
}

// Sets the max orientational error (rad)
bool NetIO::setMaxOrientationErr(const LCreal v)
{
   maxOrientationErr = v;
   return true;
}

// Sets the max age; for removal (sec)
bool NetIO::setMaxAge(const LCreal v)
{
   maxAge = v;
   return true;
}

// Sets the max entity range (meters)
bool NetIO::setMaxEntityRange(const LCreal v)
{
   maxEntityRange = v;
   maxEntityRange2 = (v*v);
   return true;
}

// Sets our federate name
bool NetIO::setFederateName(const Basic::String* const msg)
{
   federateName = msg;
   return true;
}

// Sets our federation name
bool NetIO::setFederationName(const Basic::String* const msg)
{
   federationName = msg;
   return true;
}

//------------------------------------------------------------------------------
// inputFrame() -- input side of the network
//------------------------------------------------------------------------------
void NetIO::inputFrame(const LCreal)
{
    if (isNetworkInitialized()) {

        netInputHander();     // Input handler

        processInputList();   // Update players/systems from the Input-list

        cleanupInputList();   // Cleanup the Input-List (remove old NABs)

      }
}

//------------------------------------------------------------------------------
// outputFrame() -- output side of the network
//------------------------------------------------------------------------------
void NetIO::outputFrame(const LCreal)
{
    if (isNetworkInitialized()) {

        updateOutputList();   // Update the Output-List from the simulation player list

        processOutputList();  // Create output packets from Output-List
   }
}

//------------------------------------------------------------------------------
// networkInitialization() -- Main network initialization routine
//                            (usually called by updateData())
//------------------------------------------------------------------------------
bool NetIO::networkInitialization()
{
    // reset flags
    netInit = false;
    netInitFail = false;

    // 1) Find our Station
    station = dynamic_cast<Station*>( findContainerByType(typeid(Station)) );
    if (station != 0) {
        // 2) Find the Simulation
        simulation = station->getSimulation();
        if (simulation != 0) {
            // 3) Network specific init
            netInit = initNetwork();
        }
    }

    // Did we fail?
    if (!netInit) {
        station = 0;
        simulation = 0;
        netInitFail = true;
    }

    return netInit;
}

//------------------------------------------------------------------------------
// cleanupInputList() -- Cleanup the Input-List (remove out of date items)
//------------------------------------------------------------------------------
void NetIO::cleanupInputList()
{
   // Current exec time
   const double curExecTime = getSimulation()->getExecTimeSec();

   for (unsigned int idx = 0; idx < nInNibs; idx++) {
      Nib* nib = inputList[idx];
      if ( (nib->isTimeoutEnabled() && ((curExecTime - nib->getTimeExec()) > getMaxAge(nib)) )) {
            // We have one that's timed-out --
            //std::cout << "REMOVED(TO): cur=" << curExecTime << ", NIB=" << nib->getTimeExec() << std::endl;

            // 1) Shift the rest of the list down one
            nInNibs--;
            for (unsigned int i = idx; i < nInNibs; i++) {
               inputList[i] = inputList[i+1];
            }
            inputList[nInNibs] = 0;

            // 2) Destroy the NIB
            destroyInputNib(nib);
      }
      else if (nib->isMode(Player::DELETE_REQUEST) ) {
            // We have one that has a DELETE_REQUEST
            //std::cout << "REMOVED(DR): cur=" << curExecTime << ", NIB=" << nib->getTimeExec() << std::endl;

            // 1) Shift the rest of the list down one
            nInNibs--;
            for (unsigned int i = idx; i < nInNibs; i++) {
               inputList[i] = inputList[i+1];
            }
            inputList[nInNibs] = 0;

            // 2) Destroy the NIB
            destroyInputNib(nib);
      }
   }
}

//------------------------------------------------------------------------------
// updateOutputList() --
//   Update the Output-List from the simulation player list (Background thread)
//------------------------------------------------------------------------------
void NetIO::updateOutputList()
{

   if (isNetworkInitialized()) {

      // ---
      // Remove all DELETE_REQUEST mode NIBs
      //   -- The DELETE_REQUEST were issued last pass, so the network
      //       specific software should have handled them by now.
      //   -- As NIBs are removed, the table above the NIB is shifted down.
      //   -- We're also clearing the NIB's 'checked' flag
      // ---
      {
         unsigned int i = 0;  // src index
         unsigned int j = 0;  // dest index
         while (i < nOutNibs) {
            if (outputList[i]->isMode(Player::DELETE_REQUEST)) {
               // Deleting this NIB
               //std::cout << "NetIO::updateOutputList() cleanup: nib = " << outputList[i] << std::endl;
               destroyOutputNib(outputList[i++]);
            }
            else {
               if (i != j) outputList[j] = outputList[i];
               outputList[j]->setCheckedFlag(false);
               i++;
               j++;
            }
         }
         nOutNibs = j;
      }

      // --- ---
      // Match the main player list with the output-list ...
      // --- ---
      if ( isOutputEnabled() ) {

         // Get the player list pointer (pre-ref()'d)
         Basic::PairStream* players = getSimulation()->getPlayers();

         // For all players
         bool finished = false;
         unsigned int newCount = 0;
         Basic::List::Item* playerItem = players->getFirstItem();
         while (playerItem != 0 && !finished) {

            // Get player list items
            Basic::Pair* playerPair = (Basic::Pair*)(playerItem->getValue());
            Player* player = (Player*)(playerPair->object());

            if (player->isLocalPlayer() || (isRelayEnabled() && player->getNetworkID() != getNetworkID()) )  {
               if ( player->isActive() && player->isNetOutputEnabled()) {

                  // We have (1) an active local player to output or
                  //         (2) an active networked player to relay ...

                  // Find the output NIB for this player
                  Nib* nib = findNib(player, OUTPUT_NIB);
                  if (nib == 0 && newCount < MAX_NEW_OUTGOING) {
                     // Not Found then create a new output NIB for this player
                     nib = insertNewOutputNib( player );
                     newCount++;
                  }

                  // Mark this NIB as checked
                  if (nib != 0) {
                     nib->setCheckedFlag(true);
                  }
               }
            }
            else {
               // Finished with local players and we're not relaying
               finished = !isRelayEnabled();
            }

            // get the next player
            playerItem = playerItem->getNext();
         }

         players->unref();
      }

      // ---
      // Any NIB that was not checked needs to be removed
      // ---
      for (unsigned int i = 0; i < nOutNibs; i++) {
         if ( !outputList[i]->isChecked() ) {
            // Request removel;
            // (note: the network specific code now has one frame to cleanup it's own code
            //  before the NIB is dropped from the output list next frame -- see above)
            outputList[i]->setMode(Player::DELETE_REQUEST);
         }
      }

   }

}

//------------------------------------------------------------------------------
// processOutputList() -- Process the output-list entities
//------------------------------------------------------------------------------
void NetIO::processOutputList()
{
   // ---
   // Send player states
   // ---
   for (unsigned int idx = 0; idx < getOutputListSize(); idx++) {

      Nib* nib = getOutputNib(idx);
      double curExecTime = getSimulation()->getExecTimeSec();

      if (nib->isEntityTypeValid()) {

         // While this NIB isn't being deleted ...

         // Check if a fire event is needed.  When a weapon type player is active,
         // but it's NIB state is still inactive, then we say the weapon has just
         // been fired. (delay until after the entity state PDU)
         bool fired =
            (nib->isMode(Player::INACTIVE) || nib->isMode(Player::PRE_RELEASE)) &&
            nib->getPlayer()->isMode(Player::ACTIVE) &&
            nib->getPlayer()->isMajorType(Player::WEAPON);

         // Send a detonation message
         if (nib->getPlayer()->isMode(Player::DETONATED) && !nib->wasDetonationMessageSent()) {
            nib->munitionDetonationMsgFactory((LCreal)curExecTime);
         }

         // Manager entity state updates (do this after detonation check because it updates the NIB's mode)
         nib->entityStateManager((LCreal)curExecTime);

         // Send a fire message; if a fire event was needed, we delayed sending
         // until after the weapon's entity state has been sent at least once.
         if (fired) {
            // Weapon player has just gone active and it's entity state packet has been sent.
            nib->weaponFireMsgFactory((LCreal)curExecTime);
         }

         // Manage all systems that require network functionality (IFF, Radios, Emitters, etc)
         nib->networkOutputManagers((LCreal)curExecTime);

      }
   }
}

//------------------------------------------------------------------------------
// Create a new NIBs
//------------------------------------------------------------------------------
Nib* NetIO::createNewInputNib()
{
    Nib* nib = nibFactory(INPUT_NIB);
    nib->setNetIO(this);
    return nib;
}

Nib* NetIO::createNewOutputNib(Player* const player)
{
   Nib* nib = nibFactory(OUTPUT_NIB);
   if (nib != 0) {
      nib->setNetIO(this);
      nib->setPlayer(player);
      nib->setPlayerID(player->getID());
      nib->setSide(player->getSide());

      // Default DR: World, No rotation, 2nd order linear
      nib->setDeadReckoning(Nib::FVW_DRM);

      const Basic::String* fName = getFederateName();
      if (player->isNetworkedPlayer()) {
         Nib* pNib = player->getNib();
         fName = pNib->getFederateName();
      }
      nib->setFederateName(fName);

      // Maps the player type to an output entity type.
      // Note: isEntityTypeValid() will return false if there
      // isn't a type mapper (Ntb) assigned to this Nib.
      nib->setOutputPlayerType(player);

      // Inform the player
      player->setOutgoingNib(nib, netID);
   }
   return nib;
}

//------------------------------------------------------------------------------
// Destory the NIBs
//------------------------------------------------------------------------------
void NetIO::destroyInputNib(Nib* const nib)
{
   if (nib->getPlayer() != 0) {
      // All we really need do is request deletion of the IPlayer.
      nib->getPlayer()->setMode(Player::DELETE_REQUEST);
   }
   // Once no one has a reference to us, our destructor will be called.
   nib->unref();
}

void NetIO::destroyOutputNib(Nib* const nib)
{
   Player* p = nib->getPlayer();
   if (p != 0) p->setOutgoingNib(0, netID);

   // Once no one has a reference to us, our destructor will be called.
   nib->unref();
}

//------------------------------------------------------------------------------
// createIPlayer() -- Create a new IPlayer
//------------------------------------------------------------------------------
Player* NetIO::createIPlayer(Nib* const nib)
{
   Player* player = 0;

   // Range filter
   bool inRange = true;
   double maxRng2 = getMaxEntityRangeSquared(nib);
   if (nib != 0 && maxRng2 > 0) {
      const Station* sta = getStation();
      const Player* own = sta->getOwnship();
      if (own != 0) {
         osg::Vec3d delta = nib->getDrPosition() - own->getGeocPosition();
         inRange = (delta.length2() <= maxRng2);
      }
   }

   // ---
   // In range and we haven't done a type check yet.
   // ---
   if (inRange && nib != 0 && nib->isEntityTypeUnchecked()) {

      // Mark that we've been here.
      nib->setEntityTypeChecked( true );

      // Find "by NIB type codes" on the input list
      const Ntm* typeMapper = findNetworkTypeMapper(nib);

      // ---
      // Clone the 'template' player object (if any)
      // ---
      if (typeMapper != 0) {
         const Player* templatePlayer = typeMapper->getTemplatePlayer();
         if (templatePlayer != 0) {
            player = (Player*) templatePlayer->clone();
         }
      }

      // ---
      // Set the player data
      // ---
      if (player != 0) {

         // and other stuff ...
         player->container(getSimulation());
         player->setID( nib->getPlayerID() );
         player->setSide( nib->getSide() );
         player->setName( nib->getPlayerName() );
         player->setNib(nib);
         player->setMode(Player::INACTIVE);
         player->setGeocPosition( nib->getDrPosition() );
         player->setGeocEulerAngles( nib->getDrEulerAngles() );
         player->setGeocVelocity( nib->getDrVelocity() );
         player->setGeocAcceleration( nib->getDrAcceleration() );
         player->setGeocAngularVelocities( nib->getDrAngularVelocities() );
         player->setCrashOverride(true);
         nib->setPlayer(player);

         // ---
         // Add it to the player list
         // ---
         Basic::Pair* playerPair = new Basic::Pair(nib->getPlayerName(), player);
         getSimulation()->addNewPlayer(playerPair);

         player->reset();

         // the simulation has it, so we should unref() both the player and the pair.
         player->unref();
         playerPair->unref();
      }
   }

   return player;
}

//------------------------------------------------------------------------------
// insertNewOutputNib() --
//    Create a new Network Interface Block (NIB) for 'player' and insert it
//    in the output list.  Returns a pointer to the new NIB or 0.
//------------------------------------------------------------------------------
Nib* NetIO::insertNewOutputNib(Player* const player)
{
    Nib* newNib = 0;
    if (player != 0) {
        newNib = createNewOutputNib(player);
        if (newNib != 0) {
            // Insert the new NIB into the 'output' list
            bool ok = addNibToList(newNib, OUTPUT_NIB);
            newNib->unref(); // the list owns it now.
            if (!ok) newNib = 0;
        }
    }
    return newNib;
}


//------------------------------------------------------------------------------
// addNib2InputList() --
//    Adds a new NIB to the input-list
//------------------------------------------------------------------------------
bool NetIO::addNib2InputList(Nib* const nib)
{
    // Only if we allow inputs
    if (!isInputEnabled()) return false;

    return addNibToList(nib, INPUT_NIB);
}

//------------------------------------------------------------------------------
// findNib() -- find the NIB that matches ALL IDs.
//------------------------------------------------------------------------------
Nib* NetIO::findNib(const unsigned short playerID, const Basic::String* const federateName, const IoType ioType)
{
   // Define the key
   NibKey key(playerID,federateName);

   // Binary search the table for the NIB
   Nib* found = 0;
   if (ioType == INPUT_NIB) {
      Nib** k =
         (Nib**) bsearch(&key, inputList, nInNibs, sizeof(Nib*), compareKey2Nib);
      if (k != 0) found = *k;
   }
   else {
      Nib** k =
         (Nib**) bsearch(&key, outputList, nOutNibs, sizeof(Nib*), compareKey2Nib);
      if (k != 0) found = *k;
   }
   return found;
}

Nib* NetIO::findNib(const Player* const player, const IoType ioType)
{
   Nib* found = 0;
   if (player != 0) {
      // Get the player's IDs
      const Basic::String* fName = getFederateName();
      if (player->isNetworkedPlayer()) {
         // If networked, used original IDs
         const Nib* pNib = player->getNib();
         fName = pNib->getFederateName();
      }
      // Now find the NIB using the player's IDs
      found = findNib(player->getID(), fName, ioType);
   }
   return found;
}

//------------------------------------------------------------------------------
// addNibToList() -- adds a NIB to the quick access table
//------------------------------------------------------------------------------
bool NetIO::addNibToList(Nib* const nib, const IoType ioType)
{
   bool ok = false;
   if (nib != 0) {
      Nib** tbl = inputList;
      int n = nInNibs;
      if (ioType == OUTPUT_NIB) {
         tbl = outputList;
         n = nOutNibs;
      }

      if (n < MAX_OBJECTS) {

         // Put the NIB on the top of the table
         nib->ref();
         tbl[n] = nib;

         // Create a key for this new NIB
         NibKey key(nib->getPlayerID(), nib->getFederateName());

         if (n > 0) {
            // Now, 'bubble down' to it's correct position
            int idx = n-1;
            while (idx >= 0 && compareKey2Nib(&key, &tbl[idx]) <= 0) {
               // Swap the table entries
               Nib* tmp = tbl[idx];
               tbl[idx] = tbl[idx+1];
               tbl[idx+1] = tmp;
               idx--;
            }
         }

         // Increment the count
         if (ioType == OUTPUT_NIB) nOutNibs++;
         else nInNibs++;

         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// removeNibFromList() -- removes a NIB from the quick access table
//------------------------------------------------------------------------------
void NetIO::removeNibFromList(Nib* const nib, const IoType ioType)
{
   Nib** tbl = inputList;
   int n = nInNibs;
   if (ioType == OUTPUT_NIB) {
      tbl = outputList;
      n = nOutNibs;
   }

   int found = -1;
   // Find the NIB
   for (int i = 0; i < n && found < 0; i++) {
      if (nib == tbl[i]) found = i;
   }

   // Shift down all items above this NIB one position
   if (found >= 0) {
      tbl[found]->unref();
      int n1 = (n - 1);
      for (int i = found; i < n1; i++) {
         tbl[i] = tbl[i+1];
      }
      tbl[n-1] = 0;

      // Decrement the count
      if (ioType == OUTPUT_NIB) --nOutNibs;
      else --nInNibs;
   }
}

//------------------------------------------------------------------------------
// bsearch callbacks: object name compare function --
//   True types are (const NibKey* key, const Nib** nib)
//------------------------------------------------------------------------------
int NetIO::compareKey2Nib(const void* key, const void* nib)
{

   // The Key
   const NibKey* pKey = (const NibKey*) key;

   // The NIB
   const Nib* pNib = *((const Nib**) nib);

   // Default to equal
   int result = 0;

   // First, compare the player IDs
   if (pKey->id > pNib->getPlayerID()) result = +1;
   else if (pKey->id < pNib->getPlayerID()) result = -1;

   if (result == 0) {
      // If they're the same, compare the federate names
      result = strcmp(*pKey->fName, *pNib->getFederateName());
   }

   return result;
}

//------------------------------------------------------------------------------
// Network type mapper functions
//------------------------------------------------------------------------------

// Finds the network type mapper by NIB type codes
const Ntm* NetIO::findNetworkTypeMapper(const Nib* const nib) const
{
   const Ntm* result = 0;
   if (inputNtmTree != 0 && nib != 0) {
      result = inputNtmTree->findNetworkTypeMapper(nib);
   }
   return result;
}

// Finds the network type mapper by Player
const Ntm* NetIO::findNetworkTypeMapper(const Player* const p) const
{
   const Ntm* result = 0;
   if (outputNtmTree != 0 && p != 0) {
      result = outputNtmTree->findNetworkTypeMapper(p);
   }
   return result;
}

// Adds an item to the input entity type table
bool NetIO::addInputEntityType(Ntm* const ntm)
{
   bool ok = false;
   if (nInputEntityTypes < MAX_ENTITY_TYPES) {

      ntm->ref();
      inputEntityTypes[nInputEntityTypes] = ntm;
      nInputEntityTypes++;

      // Make sure we have a root node for the quick look tree ...
      if (inputNtmTree == 0) {
         inputNtmTree = rootNtmInputNodeFactory();
      }

      // Add to the quick look tree
      if (inputNtmTree != 0 && ntm != 0) {
         ok = inputNtmTree->add2OurLists(ntm);
      }

   }
   return ok;
}

// Adds an item to the output entity type table
bool NetIO::addOutputEntityType(Ntm* const ntm)
{
   bool ok = false;
   if (nOutputEntityTypes < MAX_ENTITY_TYPES) {

      ntm->ref();
      outputEntityTypes[nOutputEntityTypes] = ntm;
      nOutputEntityTypes++;

      // Make sure we have a root node ...
      if (outputNtmTree == 0) {
         outputNtmTree = rootNtmOutputNodeFactory();
      }

      // add to the root node
      if (outputNtmTree != 0 && ntm != 0) {
         ok = outputNtmTree->add2OurLists(ntm);
      }
   }

   return ok;
}

// Clears the input entity type table
bool NetIO::clearInputEntityTypes()
{
   // Unref() the root node of the quick look tree
   if (inputNtmTree != 0) {
      inputNtmTree->unref();
      inputNtmTree = 0;
   }

   // Clear our old input entity type table --
   while (nInputEntityTypes > 0) {
      nInputEntityTypes--;
      inputEntityTypes[nInputEntityTypes]->unref();
      inputEntityTypes[nInputEntityTypes] = 0;
   }

   return true;
}

// Clears the output entity type table
bool NetIO::clearOutputEntityTypes()
{
   // Unref() the root node of the quick look tree
   if (outputNtmTree != 0) {
      outputNtmTree->unref();
      outputNtmTree = 0 ;
   }

   // Clear our old output entity type table --
   while (nOutputEntityTypes > 0) {
      nOutputEntityTypes--;
      outputEntityTypes[nOutputEntityTypes]->unref();
      outputEntityTypes[nOutputEntityTypes] = 0;
   }

   return true;
}

// Return the quick look root node for incoming entity types
const NetIO::NtmInputNode* NetIO::getRootNtmInputNode() const
{
   return inputNtmTree;
}

// Return the quick look root node for outgoing entity types
const NetIO::NtmOutputNode* NetIO::getRootNtmOutputNode() const
{
   return outputNtmTree;
}

// Return a incoming entity type by index
const Ntm* NetIO::getInputEntityType(const unsigned int idx) const
{
   return (idx < nInputEntityTypes) ? inputEntityTypes[idx] : 0;
}

// Return a outgoing entity type by index
const Ntm* NetIO::getOutputEntityTypes(const unsigned int idx) const
{
   return (idx < nOutputEntityTypes) ? outputEntityTypes[idx] : 0;
}

// Number of output types
unsigned int NetIO::getNumInputEntityTypes() const
{
   return nInputEntityTypes;
}

// Number of input types
unsigned int NetIO::getNumOutputEntityTypes() const
{
   return nOutputEntityTypes;
}

// Test rig: quick lookup of incoming entity types
void NetIO::testInputEntityTypes(const unsigned int)
{
   // Handled by derived classes
}

// Test rig: quick lookup of outgoing entity types
void NetIO::testOutputEntityTypes(const unsigned int)
{
   // Handled by derived classes
}

//------------------------------------------------------------------------------
// Slots functions
//------------------------------------------------------------------------------

// Set networkID
bool NetIO::setSlotNetworkID(const Basic::Number* const num)
{
    bool ok = false;
    if (num != 0) {
        int v = num->getInt();
        if (v >= 1 && v <= (int)MAX_NETWORD_ID) {
            ok = setNetworkID((unsigned short)(v));
        }
        else {
            std::cerr << "NetIO::setSlotNetworkID(): invalid number(" << v << "); ";
            std::cerr << "valid range:[1 ... " << MAX_NETWORD_ID << " ]" << std::endl;
        }
    }
    return ok;
}

// Sets our federate name
bool NetIO::setSlotFederateName(const Basic::String* const msg)
{
   return setFederateName(msg);
}

// Sets our federation name
bool NetIO::setSlotFederationName(const Basic::String* const msg)
{
   return setFederationName(msg);
}

// Set input enable flag
bool NetIO::setSlotEnableInput(const Basic::Number* const p)
{
    bool ok = false;
    if (p != 0) {
        inputFlg = p->getBoolean();
        ok = true;
    }
    return ok;
}

// Set output enable flag
bool NetIO::setSlotEnableOutput(const Basic::Number* const p)
{
    bool ok = false;
    if (p != 0) {
        outputFlg = p->getBoolean();
        ok = true;
    }
    return ok;
}

// Set relay enable flag
bool NetIO::setSlotEnableRelay(const Basic::Number* const p)
{
    bool ok = false;
    if (p != 0) {
        relayFlg = p->getBoolean();
        ok = true;
    }
    return ok;
}

// Sets the source of the time ( UTC or EXEC )
bool NetIO::setSlotTimeline(const Basic::Identifier* const p)
{
    bool ok = false;
    if (p != 0) {
        if (*p == "EXEC") {
            setTimeline( EXEC );
            ok = true;
        }
        else if (*p == "UTC") {
            setTimeline( UTC );
            ok = true;
        }
    }
    return ok;
}

// Sets the table of input entity to player mapper objects
bool NetIO::setSlotInputEntityTypes(Basic::PairStream* const msg)
{
    bool ok = false;
    if (msg != 0) {
       // First clear the old table
       clearInputEntityTypes();

       // Now scan the pair stream and put all Ntm objects into the table.
       Basic::List::Item* item = msg->getFirstItem();
       while (item != 0) {
          Basic::Pair* pair = (Basic::Pair*) (item->getValue());
          Ntm* ntm = dynamic_cast<Ntm*>( pair->object() );
          if (ntm != 0) {
             // We have an Ntm object, so put it in the table
             addInputEntityType(ntm);
          }
          item = item->getNext();
       }
       ok = true;
    }

    // Test rig
    //if (ok && inputNtmTree != 0) {
    //   inputNtmTree->print(std::cout);
    //   testInputEntityTypes(100);
    //}

    return ok;
}

// Sets the table of output entity to player mapper objects
bool NetIO::setSlotOutputEntityTypes(Basic::PairStream* const msg)
{
    bool ok = false;
    if (msg != 0) {
       // First clear the old table
       clearOutputEntityTypes();

       // Now scan the pair stream and put all Ntm objects into the table.
       Basic::List::Item* item = msg->getFirstItem();
       while (item != 0) {
          Basic::Pair* pair = (Basic::Pair*) (item->getValue());
          Ntm* ntm = dynamic_cast<Ntm*>( pair->object() );
          if (ntm != 0) {
            // We have an Ntm object, so put it in the table
            addOutputEntityType(ntm);
          }
          item = item->getNext();
       }
       ok = true;
    }

    // Test rig
    //if (ok && outputNtmTree != 0) {
    //   outputNtmTree->print(std::cout);
    //   //testOutputEntityTypes(100);
    //}
    return ok;
}

// Sets the mac DR time(s)
bool NetIO::setSlotMaxTimeDR(const Basic::Time* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal time = Basic::Seconds::convertStatic( *msg );
      ok = setMaxTimeDR( time );
   }
   return ok;
}

// Sets the max positional error(s)
bool NetIO::setSlotMaxPositionErr(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal err = Basic::Meters::convertStatic( *msg );
      ok = setMaxPositionErr( err );
   }
   return ok;
}

// Sets the max orientation error(s)
bool NetIO::setSlotMaxOrientationErr(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal err = (LCreal) Basic::Radians::convertStatic( *msg );
      ok = setMaxOrientationErr( err );
   }
   return ok;
}

// Sets the max age(s)
bool NetIO::setSlotMaxAge(const Basic::Time* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal age = Basic::Seconds::convertStatic( *msg );
      ok = setMaxAge( age );
   }
   return ok;
}

// Sets the max entity range(s)
bool NetIO::setSlotMaxEntityRange(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal rng = Basic::Meters::convertStatic( *msg );
      ok = setMaxEntityRange( rng );
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* NetIO::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& NetIO::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }

   indent(sout,i+j);
   sout << "networkID: ";
   sout << getNetworkID();
   sout << std::endl;

   {
      const Basic::String* name = getFederateName();
      if (name != 0 && name->len() > 0) {
         indent(sout,i+j);
         sout << "federateName: ";
         sout << *name;
         sout << std::endl;
      }
   }

   {
      const Basic::String* name = getFederationName();
      if (name != 0 && name->len() > 0) {
         indent(sout,i+j);
         sout << "federationName: ";
         sout << *name;
         sout << std::endl;
            }
         }

   indent(sout,i+j);
   sout << "enableInput: ";
   sout << isInputEnabled();
   sout << std::endl;

   indent(sout,i+j);
   sout << "enableOutput: ";
   sout << isOutputEnabled();
   sout << std::endl;

   indent(sout,i+j);
   sout << "enableRelay: ";
   sout << relayFlg;
   sout << std::endl;

   indent(sout,i+j);
   sout << "timeline: ";
   if (getTimeline() == EXEC) {
      sout << "EXEC";
   }
   else {
      sout << "UTC";
   }
   sout << std::endl;

   if (nInputEntityTypes > 0) {
      indent(sout,i+j);
      sout << "inputEntityTypes: {" << std::endl;
      for (unsigned int k = 0; k < nInputEntityTypes; k++) {
         inputEntityTypes[k]->serialize(sout,i+j+4);
      }
      indent(sout,i+j);
      sout << "}" << std::endl;
   }

   if (nOutputEntityTypes > 0) {
      indent(sout,i+j);
      sout << "outputEntityTypes: {" << std::endl;
      for (unsigned int k = 0; k < nOutputEntityTypes; k++) {
         outputEntityTypes[k]->serialize(sout,i+j+4);
      }
      indent(sout,i+j);
      sout << "}" << std::endl;
   }

   indent(sout,i+j);
   sout << "maxTimeDR: ( Seconds " << maxTimeDR << " )" << std::endl;

   indent(sout,i+j);
   sout << "maxPositionError: ( Meters " << maxPositionErr << " )" << std::endl;

   indent(sout,i+j);
   sout << "maxOrientationError: ( Degrees " << (maxOrientationErr*Basic::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "maxAge: ( Seconds " << maxAge << " )" << std::endl;

   indent(sout,i+j);
   sout << "maxEntityRange: ( NauticalMiles " << (maxEntityRange*Basic::Distance::M2NM) << " )" << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}




//==============================================================================
// Class: NtmInputNode
//==============================================================================

IMPLEMENT_PARTIAL_SUBCLASS(NetIO::NtmInputNode,"AbstractNtmInputNode")
EMPTY_SLOTTABLE(NetIO::NtmInputNode)
EMPTY_SERIALIZER(NetIO::NtmInputNode)
EMPTY_COPYDATA(NetIO::NtmInputNode)
EMPTY_DELETEDATA(NetIO::NtmInputNode)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NetIO::NtmInputNode::NtmInputNode()
{
   STANDARD_CONSTRUCTOR()
}

NetIO::NtmInputNode::~NtmInputNode()
{
   STANDARD_DESTRUCTOR()
}

NetIO::NtmInputNode& NetIO::NtmInputNode::operator=(const NetIO::NtmInputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

Basic::Object* NetIO::NtmInputNode::clone() const
{
   return 0;
}

//==============================================================================
// Class: NtmOutputNode
//==============================================================================

IMPLEMENT_PARTIAL_SUBCLASS(NetIO::NtmOutputNode,"AbstractNtmOutputNode")
EMPTY_SLOTTABLE(NetIO::NtmOutputNode)
EMPTY_SERIALIZER(NetIO::NtmOutputNode)
EMPTY_COPYDATA(NetIO::NtmOutputNode)
EMPTY_DELETEDATA(NetIO::NtmOutputNode)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NetIO::NtmOutputNode::NtmOutputNode()
{
   STANDARD_CONSTRUCTOR()
}

NetIO::NtmOutputNode::~NtmOutputNode()
{
   STANDARD_DESTRUCTOR()
}

NetIO::NtmOutputNode& NetIO::NtmOutputNode::operator=(const NetIO::NtmOutputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

Basic::Object* NetIO::NtmOutputNode::clone() const
{
   return 0;
}

//==============================================================================
// Class: NtmOutputNodeStd
// Description: Standard outgoing NTM node
//==============================================================================

class NtmOutputNodeStd : public NetIO::NtmOutputNode {
   DECLARE_SUBCLASS(NtmOutputNodeStd,NetIO::NtmOutputNode)

public:
   NtmOutputNodeStd(const Player* const p, const char* const formName);

   // NetIO::NtmOutputNode class functions
   virtual const Ntm* findNetworkTypeMapper(const Player* const p) const;
   virtual bool add2OurLists(Ntm* const ntm);
   virtual void print(std::ostream& sout, const int icnt) const;

private:
   bool checkAndAddNtm(Ntm* const ntm);
   bool addNtmSorted(Ntm* const ntm);

   char* nodeFormName;        // Form name for this node
   const Player* tp;          // Template player for this node
   Basic::List* ntmList;      // List of Ntm objects at this level
   Basic::List* subnodeList;  // List of NtmOutputNode nodes for players derived this level
};

IMPLEMENT_SUBCLASS(NtmOutputNodeStd,"NtmOutputNodeStd")
EMPTY_SLOTTABLE(NtmOutputNodeStd)
EMPTY_SERIALIZER(NtmOutputNodeStd)

//------------------------------------------------------------------------------
// root outgoing NTM node factory
//------------------------------------------------------------------------------
NetIO::NtmOutputNode* NetIO::rootNtmOutputNodeFactory() const
{
   return new NtmOutputNodeStd(0,0);   // root node has no form name
}

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NtmOutputNodeStd::NtmOutputNodeStd(const Player* const p, const char* const name)
{
   STANDARD_CONSTRUCTOR()

   nodeFormName = 0;
   tp = 0;

   if (name != 0) {
      const size_t LENGTH = strlen(name) + 1;
      nodeFormName = new char[LENGTH];
      lcStrcpy(nodeFormName,LENGTH,name);
   }

   if (p != 0) {
      p->ref();
      tp = p;
}

   ntmList = new Basic::List();
   subnodeList = new Basic::List();
}

void NtmOutputNodeStd::copyData(const NtmOutputNodeStd& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      nodeFormName = 0;
      tp = 0;
      ntmList = 0;
      subnodeList = 0;
   }

   if (nodeFormName != 0) {
      delete[] nodeFormName;
      nodeFormName = 0;
   }
   if (org.nodeFormName != 0) {
      const size_t LENGTH = strlen(org.nodeFormName) + 1;
      nodeFormName = new char[LENGTH];
      lcStrcpy(nodeFormName,LENGTH,org.nodeFormName);
   }

   if (tp != 0) {
      tp->unref();
      tp = 0;
   }
   if (org.tp != 0) {
      tp = (Player*) org.tp->clone();
   }

   if (ntmList != 0) {
      ntmList->unref();
      ntmList = 0;
   }
   if (org.ntmList != 0) {
      ntmList = (Basic::List*) org.ntmList->clone();
   }

   if (subnodeList != 0) {
      subnodeList->unref();
      subnodeList = 0;
   }
   if (org.subnodeList != 0) {
      subnodeList = (Basic::List*) org.subnodeList->clone();
   }

}

void NtmOutputNodeStd::deleteData()
{
   if (nodeFormName != 0) {
      delete[] nodeFormName;
      nodeFormName = 0;
   }

   if (ntmList != 0) {
      ntmList->unref();
      ntmList = 0;
   }

   if (subnodeList != 0) {
      subnodeList->unref();
      subnodeList = 0;
   }
}

//------------------------------------------------------------------------------
// Find the NTM node for the target player, 'p'.
//------------------------------------------------------------------------------
const Ntm* NtmOutputNodeStd::findNetworkTypeMapper(const Player* const p) const
{
   const Ntm* result = 0;

   // Using form names, is the target player either the same or derived from
   // our node?  (the root node, which has no form name, will always match)
   if ( nodeFormName == 0 || p->isFormName(nodeFormName) ) {

      // First, we'll search our subnodes to see if they'll find a match
      // (i.e., if it's derivied from our node then there may be a better match)
      const Basic::List::Item* item = subnodeList->getFirstItem();
      while (item != 0 && result == 0) {
         const NtmOutputNodeStd* subnode = (const NtmOutputNodeStd*) item->getValue();
         result = subnode->findNetworkTypeMapper(p);
         item = item->getNext();
      }

      // Second, we'll search our NTM list for a template player with a
      // type string matching the target player.
      if (result == 0 && nodeFormName != 0) {

         // Target player's type string and length
         const Basic::String* const pType = p->getType();
         const size_t pTypeLen = pType->len();

         // Search the NTM for a match with the most matching type string characters,
         // but not more than the target player's type string.
         const Basic::List::Item* item = ntmList->getFirstItem();
         while (item != 0 && result == 0) {

            // Get the template player and it's type string with length
            const Ntm* tstNtm = (const Ntm*) item->getValue();
            const Player* const tp = tstNtm->getTemplatePlayer();
            const Basic::String* const tpType = tp->getType();
            const size_t tpTypeLen = tpType->len();

            if (tpTypeLen <= pTypeLen) {
               bool match = (strncmp( pType->getString(), tpType->getString(), tpTypeLen ) == 0);
               if (match) {
                  result = tstNtm;
               }
            }

            item = item->getNext();
         }

      }

   }

   return result;
}

//------------------------------------------------------------------------------
// Check if the target NTM's template player is the same or derived from
// this node's player using form names.
//  1) if form name match: the Ntm is added to the NTM list, 'ntmList'
//  2) if the target form is derived from this form then a new node is created
//  3) if there's not match then nothing changes and false is returned.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::add2OurLists(Ntm* const tgtNtm)
{
   bool ok = false;

   if (tgtNtm != 0) {

      ok = true;  // default is true (root node matches all)
      if (nodeFormName != 0) {
         // Using form names, check if the target Ntm's
         // template player our form
         const Player* const p = tgtNtm->getTemplatePlayer();
         ok = p->isFormName(nodeFormName);
      }

      if (ok) {
         // Yes -- check to see if it's really belongs to one of our subnodes.
         bool found = false;
         Basic::List::Item* item = subnodeList->getFirstItem();
         while (item != 0 && !found) {
            NtmOutputNodeStd* subnode = (NtmOutputNodeStd*) item->getValue();
            found = subnode->add2OurLists(tgtNtm);
            item = item->getNext();
         }

         // If not found then add it to our local lists
         if (!found) ok = checkAndAddNtm(tgtNtm);
      }

   }

   return ok;
}

//------------------------------------------------------------------------------
// Add the target NTM to either (#1) the NTM list or (#2) our list of subnodes.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::checkAndAddNtm(Ntm* const tgtNtm)
{
   bool ok = false;
   if (tgtNtm != 0) {

      const Player* const tp = tgtNtm->getTemplatePlayer();
      const char* const tpfn = tp->getFormName();

      // Case #1 : when the Ntm's template player has the same
      // form name as this node then we just add it to the list
      // of Ntm objects assigned to this node.
      if (nodeFormName != 0) {
         ok = (strcmp(tpfn, nodeFormName) == 0);
         if (ok) addNtmSorted(tgtNtm);
      }

      // Case #2 : when the Ntm's template player does not have the
      // same form name as this node (i.e., it's derived from this
      // node's player class) then create a new NtmOutputNodeStd node for
      // this Ntm object and add it to our list of subnodes.
      if (!ok) {
         // Create a new node and add the NTM
         NtmOutputNodeStd* newNode = new NtmOutputNodeStd(tp,tpfn);
         newNode->addNtmSorted(tgtNtm);

         // Case #2A : check if any of our subnodes is really a subnode of the new node.
         Basic::List::Item* item = subnodeList->getFirstItem();
         while (item != 0) {
            NtmOutputNodeStd* subnode = (NtmOutputNodeStd*) item->getValue();
            item = item->getNext();

            if ( subnode->tp->isFormName( tpfn ) ) {
               // Move it from our subnode list to the new node's subnode list
               newNode->subnodeList->put(subnode);
               this->subnodeList->remove(subnode);
            }

         }

         // add the new node to our subnode list
         subnodeList->put(newNode);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Add a NTM to the list in sorted order
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::addNtmSorted(Ntm* const newNtm)
{
   bool ok = false;
   if (newNtm != 0) {

      // Create a new List::Item to contain this Ntm
      Basic::List::Item* newItem = new Basic::List::Item();
      newItem->value = newNtm;

      // Get the template player's type string from the 'new' Ntm
      const Player* newP = newNtm->getTemplatePlayer();
      const Basic::String* newTypeStr = newP->getType();
      const size_t newTypeLen = newTypeStr->len();

      bool inserted = false;
      bool err = false;
      Basic::List::Item* refItem = ntmList->getFirstItem();
      while (refItem != 0 && !inserted && !err) {

         // Get the ref player's string from the 'ref' Ntm.
         const Ntm* refNtm =  (const Ntm*) refItem->getValue();
         const Player* refP = refNtm->getTemplatePlayer();
         const Basic::String* refTypeStr = refP->getType();
         const size_t refTypeLen = refTypeStr->len();

         // compare to the shortest string length
         size_t len = newTypeLen;
         if (refTypeLen < len) len = refTypeLen;

         int r = strncmp(newTypeStr->getString(), refTypeStr->getString(), len);
         if ( r == 0 && refTypeLen < newTypeLen) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         }
         else if (r < 0) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         }
         else if ( r == 0 && refTypeLen == newTypeLen) {
            if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "Warning: duplicate outgoing NTM( ";
               std::cerr << newP->getFormName() << " type: " << *newTypeStr;
               std::cerr << " ), second ignored" << std::endl;
               err = true;
            }
            err = true;
         }

         refItem = refItem->getNext();
      }

      if (!inserted && !err) ntmList->insert(newItem,0);

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// print our data and our subnodes
//------------------------------------------------------------------------------
void NtmOutputNodeStd::print(std::ostream& sout, const int icnt) const
{
   // Print our node's form name
   indent(sout,icnt);
   sout << "( NtmOutputNodeStd: FormName: ";
   if (nodeFormName != 0) sout << nodeFormName;
   else sout << "ROOT";
   sout << std::endl;

   // Print our Ntm objects
   {
      const Basic::List::Item* item = ntmList->getFirstItem();
      while (item != 0) {
         const Ntm* ntm = (const Ntm*) item->getValue();
         ntm->serialize(sout, icnt+4);
         item = item->getNext();
      }
   }

   // Print our subnodes
   {
      const Basic::List::Item* item = subnodeList->getFirstItem();
      while (item != 0) {
         const NtmOutputNodeStd* subnode = (const NtmOutputNodeStd*) item->getValue();
         subnode->print(sout,icnt+4);
         item = item->getNext();
      }
   }

   indent(sout,icnt);
   sout << ")" << std::endl;
}

} // End Simulation namespace
} // End Eaagles namespace
