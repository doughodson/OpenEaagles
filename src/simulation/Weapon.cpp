// Weapon.cxx: implementation of the AirVehicle class.

#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/simulation/Designator.h"
#include "openeaagles/simulation/DynamicsModels.h"
#include "openeaagles/simulation/Guns.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Stores.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace Eaagles {
namespace Simulation {

//==============================================================================
// class Weapon
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Weapon,"Weapon")

// parameters
const LCreal Weapon::DEFAULT_MAX_TGT_RNG = 2000.0f;    // meters
const LCreal Weapon::DEFAULT_MAX_TGT_LOS_ERR = 1.0f;   // radians

// Slot table
BEGIN_SLOTTABLE(Weapon)
    "released",         //  1: Weapon has been released
    "failed",           //  2: Weapon failed (e.g., reasonableness Test)
    "power",            //  3: Weapon power flag
    "hang",             //  4: Will be a hung store
    "hung",             //  5: Hung store
    "maxTOF",           //  6: max time of flight        (sec)
    "tsg",              //  7: time to start guidance    (sec - tof)
    "maxBurstRng",      //  8: max burst rng             (meters)
    "lethalRange",      //  9: lethal range              (meters)
    "sobt",             // 10: start-of-burn time        (sec - tof)
    "eobt",             // 11: end-of-burn time          (sec - tof)
    "maxGimbal",        // 12: max gimbal angle          (Basic::Angle)
    "tgtPos",           // 13: TEST target position [ n e d ] (meters)
    "weaponID",         // 14: Weapon type ID (user defined number)
    "dummy",            // 15: Dummy store (launch, but don't flyout or detonate)
    "jettisonable",     // 16: Weapon can be jettisoned (default: true)
    "testTgtName"       // 17: TEST only: target player name
END_SLOTTABLE(Weapon)

// Map slot table to handles 
BEGIN_SLOT_MAP(Weapon)
    ON_SLOT( 1,  setSlotReleased,    Basic::Number)
    ON_SLOT( 2,  setSlotFailed,      Basic::Number)
    ON_SLOT( 3,  setSlotPower,       Basic::Number)
    ON_SLOT( 4,  setSlotWillHang,    Basic::Number)
    ON_SLOT( 5,  setSlotHung,        Basic::Number)

    ON_SLOT( 6,  setSlotMaxTOF,      Basic::Time)
    ON_SLOT( 6,  setSlotMaxTOF,      Basic::Number)

    ON_SLOT( 7,  setSlotTSG,         Basic::Time)
    ON_SLOT( 7,  setSlotTSG,         Basic::Number)

    ON_SLOT( 8,  setSlotMaxBurstRng, Basic::Distance)
    ON_SLOT( 8,  setSlotMaxBurstRng, Basic::Number)

    ON_SLOT( 9, setSlotLethalRange, Basic::Distance)
    ON_SLOT( 9, setSlotLethalRange, Basic::Number)

    ON_SLOT(10, setSlotSOBT,        Basic::Time)
    ON_SLOT(10, setSlotSOBT,        Basic::Number)

    ON_SLOT(11, setSlotEOBT,        Basic::Time)
    ON_SLOT(11, setSlotEOBT,        Basic::Number)

    ON_SLOT(12, setSlotMaxGimbal,   Basic::Angle)

    ON_SLOT(13, setSlotTgtPos,      Basic::List)
    ON_SLOT(14, setSlotWeaponID,    Basic::Number)
    ON_SLOT(15, setSlotDummy,       Basic::Number)
    ON_SLOT(16, setSlotJettisonable, Basic::Number)
    ON_SLOT(17, setSlotTestTgtName, Basic::String)
END_SLOT_MAP()

// Event() map
BEGIN_EVENT_HANDLER(Weapon)
    ON_EVENT_OBJ(DESIGNATOR_EVENT, onDesignatorEvent, Designator)
    ON_EVENT( JETTISON_EVENT, onJettisonEvent)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Weapon::Weapon()
{
   STANDARD_CONSTRUCTOR()

   static Basic::String generic("GenericWeapon");
   setType(&generic);
   setMode(INACTIVE);
   setInitMode(INACTIVE);

   initData();
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Weapon::initData()
{
   flyoutWpn = 0;
   initialWpn = 0;

   tgtPos.set(0,0,0);
   tgtPosValid = false;
   tgtPlayer = 0;
   tgtTrack = 0;
   launchVehicle = 0;
   posTrkEnb = false;
   maxTgtRng = DEFAULT_MAX_TGT_RNG;
   maxTgtLosErr = DEFAULT_MAX_TGT_LOS_ERR;
   detonationRange = 0;
   tgtDetLoc.set(0,0,0);
   launcher  = 0;
   station   = 0;
   weaponID = 0;
   eventID = 0;

   power     = true;
   failed    = false;
   released  = false;
   releaseHold  = false;
   willHang = false;
   hung      = false;
   blocked   = 0;
   canJettison = true; // default
   jettisoned = false;
   dummyFlg  = false;
   results = DETONATE_NONE;
   tstTgtNam = 0;

   tof = 0;
   setMaxTOF(60.0f);
   setTSG(9999.0f);
   setMaxBurstRng(500.0f);
   setLethalRange(50.0f);
   setSOBT(9999.0f);
   setEOBT(0.0f);
   setMaxGimbalAngle(30.0f * (LCreal)Basic::Angle::D2RCC);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Weapon::copyData(const Weapon& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setFlyoutWeapon(0);
   setInitialWeapon(0);
   setTargetTrack(0,false);
   setTargetPlayer(0,false);
   setLauncher(0,0);
   setLaunchVehicle(0);

   tgtPos = org.tgtPos;
   tgtPosValid = org.tgtPosValid;
   posTrkEnb = org.posTrkEnb;
   maxTgtRng = org.maxTgtRng;
   maxTgtLosErr = org.maxTgtLosErr;
   detonationRange = org.detonationRange;
   tgtDetLoc = org.tgtDetLoc;
   station = org.station;
   weaponID = org.weaponID;
   eventID = org.eventID;
   power     = org.power;
   failed    = org.failed;
   released  = org.released;
   releaseHold  = org.releaseHold;
   willHang  = org.willHang;
   hung      = org.hung;
   blocked   = org.blocked;
   canJettison  = org.canJettison;
   jettisoned = org.jettisoned;
   dummyFlg  = org.dummyFlg;
   results = org.results;
   tstTgtNam = org.tstTgtNam;

   tof = org.tof;
   maxTOF = org.maxTOF;
   tsg = org.tsg;
   maxBurstRng = org.maxBurstRng;
   lethalRange = org.lethalRange;
   eobt = org.eobt;
   sobt = org.sobt;
   maxGimbal = org.maxGimbal;
}

void Weapon::deleteData()
{
   setFlyoutWeapon(0);
   setInitialWeapon(0);
   setTargetTrack(0,false);
   setTargetPlayer(0,false);
   setLauncher(0,0);
   setLaunchVehicle(0);
}

//------------------------------------------------------------------------------
// reset() -- Reset vehicle
//------------------------------------------------------------------------------
void Weapon::reset()
{
   BaseClass::reset();

   Weapon* flyout = getFlyoutWeapon();

   // If there's a flyout weapon still in PRE_RELEASE then reset it
   if (flyout != 0 && flyout != this && flyout->isMode(PRE_RELEASE) ) {
      flyout->reset();
   }

   // If this is flyout weapon then set the mode to DELETE_REQUEST to
   // remove us from the player list.
   if (flyout == this) {
      setMode(DELETE_REQUEST);
   }

   hung      = false;
   released  = false;
   failed    = false;
   jettisoned = false;

   setDetonationResults(DETONATE_NONE);

   setFlyoutWeapon(0);
   setInitialWeapon(0);
   setTargetTrack(0,false);
   setTargetPlayer(0,false);

   // launch vehicle
   if (flyout != this) {
      setLaunchVehicle( (Player*) findContainerByType( typeid(Player) ) );
   }

   // Test player?
   if (tstTgtNam != 0) {
      Simulation* s = getSimulation();
      if (s != 0) {
         Player* t = s->findPlayerByName( *tstTgtNam );
         if (t != 0) setTargetPlayer(t,true);
     }
   }

   setTOF(0.0);

   if (flyout != 0) flyout->unref();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Weapon::updateTC(const LCreal dt)
{
   BaseClass::updateTC(dt);

   unsigned int ph = getSimulation()->phase();

   // Phase #0 -- Transition from pre-release to active at the end of dynamics
   // phase (after the call to BaseClass), so that our position, which was
   // relative to our launch vehicle, has been computed.
   if (ph == 0 && isMode(PRE_RELEASE) && !isReleaseHold() ) {
      atReleaseInit();
      setMode(ACTIVE);
   }

   // Phase #3
   if (ph == 3 && isActive() && isLocalPlayer() && !isJettisoned() && !isDummy()) {

      // Simple function to get target coordinates
      if (posTrkEnb) positionTracking();

      // Update our Time-Of-Flight (TOF)
      if (isMode(ACTIVE)) updateTOF(dt*4.0f);
   }
}

//------------------------------------------------------------------------------
// dynamics() -- update vehicle dynamics
//------------------------------------------------------------------------------
void Weapon::dynamics(const LCreal dt)
{
   if (isMode(PRE_RELEASE)) {
      // Weapon is on the same side as the launcher
      setSide( getLaunchVehicle()->getSide() );

      // Launch vehicles rotational matrix
      osg::Matrixd lvM = getLaunchVehicle()->getRotMat();

      // Set weapon's position at launch
      // 1) Weapon's position is its position relative to the launcher (launcher's body coordinates)
      // 2) Rotate to earth coordinates
      // 3) Add the launcher's position
      osg::Vec2d ip = getInitPosition();
      osg::Vec3d pos0b(ip.x(), ip.y(), -getInitAltitude());
      osg::Vec3d pos0e = pos0b * lvM; // body to earth
      osg::Vec3d lpos = getLaunchVehicle()->getPosition();
      osg::Vec3d pos1 = lpos + pos0e;
      setPosition( pos1 );

      // Weapon's orientation at launch
      const osg::Vec3d ia = getInitAngles();
      osg::Matrixd rr;
      Basic::Nav::computeRotationalMatrix( ia[0], ia[1], ia[2], &rr);
      rr *= lvM;

      setRotMat(rr);

      // Set velocities are the same as the launcher
      setVelocity( getLaunchVehicle()->getVelocity() );

      // Not accelerations or angular velocities
      setAcceleration( 0, 0, 0 );
      setAngularVelocities( 0, 0, 0 );
   }
   else if (!isJettisoned()) {

      if (isLocalPlayer() && !isDummy() && getDynamicsModel() == 0) {
         // Use our default (simple) weapon model
         weaponGuidance(dt);
         weaponDynamics(dt);
      }
      BaseClass::dynamics(dt);

   }
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool Weapon::shutdownNotification()
{
   // Clear all of our pointers
   setFlyoutWeapon(0);
   setInitialWeapon(0);
   setTargetTrack(0,false);
   setTargetPlayer(0,false);
   setLauncher(0,0);
   setLaunchVehicle(0);

   return BaseClass::shutdownNotification();
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int Weapon::getMajorType() const
{
   return WEAPON;
}

//------------------------------------------------------------------------------
// Default designator event handler
//------------------------------------------------------------------------------
bool Weapon::onDesignatorEvent(const Designator* const)
{
   // In the future, we'll want to pass this to our LASER detector
   // But we don't have any yet, so yet our derived classes override
   // this one for now.
   return true;
}

//------------------------------------------------------------------------------
// Default jettison event handler --
//  -- We're setting the initial weapon's mode to LAUNCHED with the "we've been
//  jettisoned" flag, and we're removing the pre-released flyout weapon, if any.
//------------------------------------------------------------------------------
bool Weapon::onJettisonEvent()
{
   bool ok = false;
   if (!isReleased() && !isJettisoned() && isJettisonable()) {

      // If we haven't already been release or jettisoned,
      // and we can be jettisoned ...

      Weapon* flyout = getFlyoutWeapon();
      Weapon* initWpn = getInitialWeapon();

      // If there is a flyout weapon that's still in PRE_RELEASE mode
      // then call its jettison event handler.
      if (flyout != 0 && flyout != this && flyout->isMode(PRE_RELEASE) ) {
         flyout->onJettisonEvent();
      }

      // If this is the flyout weapon then set our mode to DELETE_REQUEST
      if (flyout == this) {
         setMode(DELETE_REQUEST);
      }

      // If this is initial weapon then set our mode to LAUNCHED
      if (initWpn == this) {
         initWpn->setMode(Player::LAUNCHED);
      }

      // And set the 'jettisoned' and 'released' flags
      setJettisoned(true);
      setReleased(true);

      // cleanup
      if (flyout != 0) flyout->unref();
      if (initWpn != 0) initWpn->unref();

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// Check local players for the effects of the detonation -- did we hit anyone?
//------------------------------------------------------------------------------
void Weapon::checkDetonationEffect()
{
   Simulation* s = getSimulation();
   if (s != 0) {
      // Only local players within 10X max burst range
      LCreal maxRng = 10.0f * getMaxBurstRng();

      // Find our target (if any)
      const Player* tgt = getTargetPlayer();
      if (tgt == 0) {
         const Track* trk = getTargetTrack();
         if (trk != 0) tgt = trk->getTarget();
      }

      Basic::PairStream* plist = s->getPlayers();
      if (plist != 0) {
      Basic::List::Item* item = plist->getFirstItem();

      // Process the detonation for all local, in-range players
      bool finished = false;
      while (item != 0 && !finished) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         Player* p = (Player*) pair->object(); 
         finished = p->isNetworkedPlayer();  // local only
         if (!finished && (p != this) ) {
            osg::Vec3 dpos = p->getPosition() - getPosition();
            LCreal rng = dpos.length();
            if ( (rng <= maxRng) || (p == tgt) ) p->processDetonation(rng, this);
         }
         item = item->getNext();
      }

         // cleanup
         plist->unref();
         plist = 0;
      }

   }
}

//------------------------------------------------------------------------------
// collisionNotification() -- We just impacted with another player
//------------------------------------------------------------------------------
bool Weapon::collisionNotification(Player* const other)
{
   bool ok = killedNotification(other);

   // We've detonated!
   setMode(DETONATED);
   setDetonationResults(DETONATE_ENTITY_IMPACT);

   // Compute detonation location relative to the other ship
   setTargetPlayer(other,false);
   setLocationOfDetonation();

   // Ground detonation -- anyone here to see it?
   checkDetonationEffect();

   // Log the event
   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_WEAPON_DETONATION )
      SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
      SAMPLE_2_VALUES( DETONATE_ENTITY_IMPACT, getDetonationRange() )
   END_RECORD_DATA_SAMPLE()

   // TabLogger is deprecated
   if (getAnyEventLogger() != 0) {
      TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(2, getLaunchVehicle(), this, getTargetPlayer(), DETONATE_ENTITY_IMPACT, getDetonationRange());  // type 2 for "detonate"
      getAnyEventLogger()->log(evt);
      evt->unref();
   }
   return ok;
}

//------------------------------------------------------------------------------
// crashNotification() -- We just impacted the ground
//------------------------------------------------------------------------------
bool Weapon::crashNotification()
{
   // ---
   // We've detonated because we've hit the ground
   // ---
   bool ok = killedNotification();
   setDetonationResults(DETONATE_GROUND_IMPACT);
   setMode(DETONATED);

   // ---
   // Compute location of detonation relative to target
   // ---
   if (getTargetPlayer() != 0) {
      setLocationOfDetonation();
   }

   // ---
   // Ground detonation -- anyone here to see it?
   // ---
   checkDetonationEffect();

   // ---
   // Log the event
   // ---
   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_WEAPON_DETONATION )
      SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
      SAMPLE_2_VALUES( DETONATE_GROUND_IMPACT, getDetonationRange() )
   END_RECORD_DATA_SAMPLE()

   // TabLogger is deprecated
   if (getAnyEventLogger() != 0) {
      TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(2, getLaunchVehicle(), this, getTargetPlayer(), DETONATE_GROUND_IMPACT, getDetonationRange());  // type 2 for "detonate"
      getAnyEventLogger()->log(evt);
      evt->unref();
   }
   return ok;
}

//------------------------------------------------------------------------------
// prerelease() -- prerelease this weapon.
//
// Returns a point to the flyout weapon player, which is still in 'release hold'.
//------------------------------------------------------------------------------
Weapon* Weapon::prerelease()
{
   Weapon* flyout = getFlyoutWeapon();

   // If we're not already (pre)released or jettisoned,
   //   and we'll need a launching player and a simulation
   Simulation* sim = (Simulation*)( findContainerByType(typeid(Simulation)) );
   Player* lplayer = getLaunchVehicle();
   if (!isReleased() && !isJettisoned() && flyout == 0 && lplayer != 0 && sim != 0) {

      // we'll get an event at the time of release.
      // but get an event ID to use as our player ID
      eventID = 0;

      // Next we'll clone ourself --
      //  -- this will be the actual weapon player what will do the fly-out.
      flyout = (Weapon*) this->clone();

      flyout->container( sim );
      flyout->reset();

      flyout->setFlyoutWeapon(flyout);
      flyout->setInitialWeapon(this);
      flyout->setID( sim->getNewReleasedWeaponID() );

      flyout->setLaunchVehicle( lplayer );
      flyout->setSide( lplayer->getSide() );

      // and set the weapon prerelease
      flyout->setMode(PRE_RELEASE);
      flyout->setReleased(false);
      flyout->setReleaseHold(true);

      // Set our mode flags to fully released.
      setFlyoutWeapon(flyout);
      setInitialWeapon(this);
      setReleased(false);
      setReleaseHold(true);

      // add it to the flyout weapont to the player list
      char pname[32];
      std::sprintf(pname,"W%05d", flyout->getID());
      sim->addNewPlayer(pname,flyout);

   }

   return flyout;
}

//------------------------------------------------------------------------------
// release() -- release the weapon
//
//  1) We'll clone the initial weapon, which will be the flyout weapon,
//     reset the flyout weapon, set a few flags, and add the flyout weapon
//     to the player list.
//
//  2) If we already have a flyout weapon in 'release hold', then this
//      function, which can be called for either the initial or flyout
//      weapons, will set a few flags and clear the 'release hold'.
//
// Return a pointer to the flyout weapon player
//------------------------------------------------------------------------------
Weapon* Weapon::release()
{
   Weapon* flyout = 0;

   // When this weapon isn't already released, blocked or jettisoned.
   if ( !isReleased() && !isBlocked() && !isJettisoned() ) {

      // and isn't flagged to be a hung store (i.e., failure mode),
      if (!getWillHang()) {

         // and we have a launching player and a simulation ...
         Player* lplayer = getLaunchVehicle();
            Simulation* sim = (Simulation*)( findContainerByType(typeid(Simulation)) );
         if ( lplayer != 0 && sim != 0) {

            // then release the weapon!

            flyout = getFlyoutWeapon();
            if (flyout != 0) {
               // When we've already created a flyout weapon, which is on the
               // player list in 'release hold' ...

               // we'll just need to clear the "release
               // hold" flag, which will let the flyout weapon go ACTIVE.
               flyout->setReleased(true);
               flyout->setReleaseHold(false);

               // Set the initial weapon's mode flags to fully released.
               Weapon* initWpn = getInitialWeapon();
               initWpn->setMode(Player::LAUNCHED);
               initWpn->setReleased(true);
               initWpn->setReleaseHold(false);
            
           }
           else {
               // When we haven't already created a flyout then this is
               // a direct release ...

               // Get a release event
               eventID = sim->getNewWeaponEventID();

               // Next we'll clone ourself --
               //  -- this will be the actual weapon player what will do the fly-out.
               flyout = (Weapon*) this->clone();

               flyout->container( sim );
               flyout->reset();

               flyout->setFlyoutWeapon(flyout);
               flyout->setInitialWeapon(this);
               flyout->setID( sim->getNewReleasedWeaponID() );

               flyout->setLaunchVehicle( lplayer );
               flyout->setSide( lplayer->getSide() );

               // and set the weapon prerelease
               flyout->setMode(PRE_RELEASE);
               flyout->setReleased(true);
               flyout->setReleaseHold(false);

               // Set our mode flags to fully released.
               setFlyoutWeapon(flyout);
               setInitialWeapon(this);
            setMode(Player::LAUNCHED);
            setReleased(true);
               setReleaseHold(false);

               // add it to the flyout weapont to the player list
               char pname[32];
               std::sprintf(pname,"W%05d", flyout->getID());
               sim->addNewPlayer(pname,flyout);
            }

           if (isMessageEnabled(MSG_DATA)) {
              BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_WEAPON_RELEASED )
                 SAMPLE_3_OBJECTS( this, getLaunchVehicle(), 0 )  // weapon, shooter, target
                 SAMPLE_2_VALUES( 0, 0.0 )
              END_RECORD_DATA_SAMPLE()
           }

           // TabLogger is deprecated
            if (isMessageEnabled(MSG_DATA) && getAnyEventLogger() != 0) {
               // type 1 for "launch", last two fields effectively null
               TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(1, getLaunchVehicle(), 0, 0, 0, 0.0);
               getAnyEventLogger()->log(evt);
               evt->unref();
            }

         }

        }
        else {
         // We have a hung store
            setHung(true);

           if (isMessageEnabled(MSG_DATA)) {
              BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_WEAPON_HUNG )
                 SAMPLE_2_OBJECTS( this, getLaunchVehicle() )
              END_RECORD_DATA_SAMPLE()
           }

           // TabLogger is deprecated
         if (isMessageEnabled(MSG_DATA) && getAnyEventLogger() != 0) {
            // type 4 for "hung store"
            TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(4, getLaunchVehicle(), this, 0, 0, 0.0);
            getAnyEventLogger()->log(evt);
            evt->unref();
        }
    }
   }

   return flyout;
}

//------------------------------------------------------------------------------
// atReleaseInit() -- Init weapon data at release
//------------------------------------------------------------------------------
void Weapon::atReleaseInit()
{
   // Set the release event
   if (eventID == 0) {
      eventID = getSimulation()->getNewWeaponEventID();
   }

   // Reset the dynamics mode (if any)
   if (getDynamicsModel() != 0) {
        getDynamicsModel()->atReleaseInit();
   }
}

//------------------------------------------------------------------------------
// setTOF() -- Set the time of flight
//------------------------------------------------------------------------------
void Weapon::setTOF(const LCreal newTOF)
{
   tof = newTOF;
}

//------------------------------------------------------------------------------
// weaponGuidance() -- default guidance
//------------------------------------------------------------------------------
void Weapon::weaponGuidance(const LCreal)
{
}

//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics
//------------------------------------------------------------------------------
void Weapon::weaponDynamics(const LCreal)
{
}

//------------------------------------------------------------------------------
// updateTOF -- default time of flight
//------------------------------------------------------------------------------
void Weapon::updateTOF(const LCreal dt)
{
   // As long as we're active ...
   if (isMode(ACTIVE)) {

      // update time of flight,
      setTOF( getTOF() + dt );

      // and check for the end of the flight
      if (getTOF() >= getMaxTOF()) {
         setMode(DETONATED);
         setDetonationResults( DETONATE_DETONATION );

         if (isMessageEnabled(MSG_DATA)) {
            BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_WEAPON_DETONATION )
               SAMPLE_3_OBJECTS( this, getLaunchVehicle(), getTargetPlayer() )
               SAMPLE_2_VALUES( DETONATE_DETONATION, 0.0 )
            END_RECORD_DATA_SAMPLE()
         }

         // TabLogger is deprecated
         if (getAnyEventLogger() != 0) {
            TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(2, getLaunchVehicle(), this, getTargetPlayer(), DETONATE_DETONATION);  // type 2 for "detonate"
            getAnyEventLogger()->log(evt);
            evt->unref();
         }
         return;
      }
   }
}


//------------------------------------------------------------------------------
// positionTracking() -- update target position from target player position
//------------------------------------------------------------------------------
void Weapon::positionTracking()
{
    if (posTrkEnb) {
        
        // When we have track manager -- follow the first track
        if (tgtTrack != 0) {
            setTargetPosition(tgtTrack->getPosition());
        }

        else if (tgtPlayer != 0) {
            // No sensor, but we have a target player -- fake it and just follow the target
            osg::Vec3 p0 = getPosition();
            setTargetPosition(tgtPlayer->getPosition() - p0);
        }
        
        else {
            // Loss of position tracking ...
            posTrkEnb = false;
        }

    }
}

//------------------------------------------------------------------------------
// Computes and sets 'loc' to our location relative to the target player, 'tgt'
//------------------------------------------------------------------------------
bool Weapon::computeTargetLocation(osg::Vec3* const loc, const Player* const tgt)
{
   bool ok = false;
   if (tgt != 0 && loc != 0) {
      osg::Vec3 posP = getPosition() - tgt->getPosition();
      osg::Vec3 posB = tgt->getRotMat() * posP;
      *loc = posB;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Compute the location of the detonation relative to the target player
//------------------------------------------------------------------------------
bool Weapon::setLocationOfDetonation()
{
   bool ok = false;

   // Find our target (if any)
   const Player* tgt = getTargetPlayer();
   if (tgt == 0) {
      const Track* trk = getTargetTrack();
      if (trk != 0) tgt = trk->getTarget();
   }

   // computer the location of the detonation relative to the target player
   if (tgt != 0) {
      osg::Vec3 loc;
      ok = computeTargetLocation(&loc, tgt);
      if (ok) setDetonationLocation(loc);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Standard get routines
//------------------------------------------------------------------------------

// Returns pre-ref()'d pointer to the initial or fly-out based on modes
Weapon* Weapon::getPointer()
{
   if (flyoutWpn != 0) {
      return flyoutWpn.getRefPtr();
   }
   else {
      this->ref();
      return this;
   }
}

// Returns pre-ref()'d pointer to the initial or fly-out based on modes (const version)
const Weapon* Weapon::getPointer() const
{
   if (flyoutWpn != 0) {
      return flyoutWpn.getRefPtr();
   }
   else {
      this->ref();
      return this;
   }
}

// True if weapon type IDs match
bool Weapon::isWeaponID(const int n) const
{
   return (getWeaponID() == n);
}

// Weapon type ID number
int Weapon::getWeaponID() const
{
   return weaponID;
}

// Returns true if the weapon is a member of the test category
bool Weapon::isCategory(const int testCategory) const
{
   return (testCategory & getCategory()) != 0;
}

// Our launcher, if any
Stores* Weapon::getLauncher()
{
   return launcher;
}

// Our launcher, if any (const version)
const Stores* Weapon::getLauncher() const
{
   return launcher;
}

// Station index (number)
unsigned int Weapon::getStation() const
{
   return station;
}

// True if  the weapon has been released
bool Weapon::isReleased() const
{
   return released;
}

// Weapon power flag
bool Weapon::isPowerOn() const
{
   return power;
}

// Blocked weapon flag (can not be released if true)
bool Weapon::isBlocked() const
{
   return blocked;
}

// True if the weapon can be jettisioned
bool Weapon::isJettisonable() const
{
   return canJettison;
}

// True if the weapon has been jettisioned
bool Weapon::isJettisoned() const
{
   return jettisoned;
}

// True if the weapon has failed
bool Weapon::isFailed() const
{
   return failed;
}

// True if the weapon is hung 
bool Weapon::isHung() const
{
   return hung;
}

// True if the weapon will hang on release
bool Weapon::getWillHang() const
{
   return willHang;
}

// True if this is a dummy weapon (someone else with fly it out)
bool Weapon::isDummy() const
{
   return dummyFlg;
}

// Time Of Flight (seconds) since release
LCreal Weapon::getTOF() const
{
   return tof;
}

// Max TOF (seconds)
LCreal Weapon::getMaxTOF() const
{
   return maxTOF;
}

// Time-to-Start guidance (seconds since release)
LCreal Weapon::getTSG() const
{
   return tsg;
}

// Start-Of-Burn time (seconds since release)
LCreal Weapon::getSOBT() const
{
   return sobt;
}

// End-Of-Burn time (seconds since release)
LCreal Weapon::getEOBT() const
{
   return eobt;
}

// is guidance system enabled (default check)
bool Weapon::isGuidanceEnabled() const
{
   return (getTOF() >= tsg) && ((getCategory() & GUIDED) != 0) && isTargetPositionValid();
}

// Weapon engine (rocket) on
bool Weapon::isEngineBurnEnabled() const
{
   return (tof >= sobt && tof <= eobt);
}

// Max burst range (meters) -- most players will be damaged within this range
LCreal Weapon::getMaxBurstRng() const
{
   return maxBurstRng;
}

// Lethal range (meters) -- most players will be killed within this range
LCreal Weapon::getLethalRange() const
{
   return lethalRange;
}

// Max gimbal angle (radians)
LCreal Weapon::getMaxGimbalAngle() const
{
   return maxGimbal;
}

// Pointer to the player that launched us
Player* Weapon::getLaunchVehicle()
{
   return launchVehicle;
}

// Pointer to the player that launched us (const version)
const Player* Weapon::getLaunchVehicle() const
{
   return launchVehicle;
}

// True if we have the target position and is it valid
bool Weapon::isTargetPositionValid() const
{
   return tgtPosValid;
}

// Returns the target position (meters -- NED from simulation ref point)
const osg::Vec3& Weapon::getTargetPosition() const
{
   return tgtPos;
}

// Our target track, if any
Track* Weapon::getTargetTrack()
{
   return tgtTrack;
}

// Our target track, if any (const version)
const Track* Weapon::getTargetTrack() const
{
   return tgtTrack;
}

// Our target player, if any
Player* Weapon::getTargetPlayer()
{
   return tgtPlayer;
}

// Our target player, if any (const version)
const Player* Weapon::getTargetPlayer() const
{
   return tgtPlayer;
}

// Pre-ref()'d pointer to the fly-out weapon 
Weapon* Weapon::getFlyoutWeapon()
{
   return flyoutWpn.getRefPtr();
}

// Pre-ref()'d pointer to the fly-out weapon (const version)
const Weapon* Weapon::getFlyoutWeapon() const
{
   return flyoutWpn.getRefPtr();
}

// Pre-ref()'d pointer to the initial weapon 
Weapon* Weapon::getInitialWeapon()
{
   return initialWpn.getRefPtr();
}

// Pre-ref()'d pointer to the initial weapon (const version)
const Weapon* Weapon::getInitialWeapon() const
{
   return initialWpn.getRefPtr();
}

// Release event ID (to help match weapon launch and detonation events)
unsigned short Weapon::getReleaseEventID() const
{
   return eventID;
}

// Is weapon is holding in PRE_RELEASE mode?
bool Weapon::isReleaseHold() const
{
   return releaseHold;
}

// Detonation result code (see 'Detonation' enum)
Weapon::Detonation Weapon::getDetonationResults() const
{
   return results;
}

// Range to target at detonation (meters)
LCreal Weapon::getDetonationRange() const
{
   return detonationRange;
}

// Location of detonation in target player's coord (meters)
const osg::Vec3& Weapon::getDetonationLocation() const
{
   return tgtDetLoc;
}

//------------------------------------------------------------------------------
// Standard set routins
//------------------------------------------------------------------------------

// setTargetPlayer() -- sets a pointer to the target player
bool Weapon::setTargetPlayer(Player* const tgt, const bool pt)
{
    tgtPlayer = tgt;
    tgtTrack = 0;
       
    // Track position?
    posTrkEnb = (pt && tgt != 0);
    positionTracking();
    return true;
}

// setTargetTrack() -- sets a pointer to the target track
bool Weapon::setTargetTrack(Track* const trk, const bool pt)
{
    tgtPlayer = 0;
    tgtTrack = trk;
    
    // Track position?
    posTrkEnb = (pt && trk != 0);
    positionTracking();
    return true;
}

// setTargetPosition() -- set target position -- platform coord (NED)
bool Weapon::setTargetPosition(const osg::Vec3& newTgtPos)
{
    tgtPos = newTgtPos;
    setTargetPositionValid(true);
    return true;
}

// Sets the target position valid flag
bool Weapon::setTargetPositionValid(const bool b)
{
   tgtPosValid = b;
   return true;
}

// setLaunchVehicle() -- sets a pointer to the launching player
bool Weapon::setLaunchVehicle(Player* const lch)
{
    launchVehicle = lch;
    return true;
}

// Sets a weapon type player to release hold mode
bool Weapon::setReleaseHold(const bool f)
{
    releaseHold = f;
    return true;
}

// Sets the weapon jettisoned flag
bool Weapon::setJettisoned(const bool f)
{
   jettisoned = f;
   return true;
}

// setFlyoutWeapon() -- sets a pointer to the "fly-out" weapon player
bool Weapon::setFlyoutWeapon(Weapon* const p)
{
    flyoutWpn = p;
    return true;
}

// setInitialWeapon() -- sets a pointer to the "initial" weapon
bool Weapon::setInitialWeapon(Weapon* const p)
{
    initialWpn = p;
    return true;
}

// setMaxTOF() -- Set max Time-Of-Flight (seconds)
bool Weapon::setMaxTOF(const LCreal v)
{
    maxTOF =  v;
    return true;
}

// setTSG() -- Set Time-to-Start-Guidance (seconds)
bool Weapon::setTSG(const LCreal v)
{
    tsg = v;
    return true;
}

// Sets the detonation result code
bool Weapon::setDetonationResults(const Detonation dr)
{
   results = dr;
   return true;
}

// Sets the detonation location in target player's coord (meters)
bool Weapon::setDetonationLocation(const osg::Vec3& loc)
{
   bool ok = false;
   if (&loc != 0) {
      tgtDetLoc = loc;
      detonationRange = loc.length();
      ok = true;
   }
   return ok;
}

// setMaxGimbalAngle() -- Set max gimbal angle (radians)
bool Weapon::setMaxGimbalAngle(const LCreal v)
{
    maxGimbal =  v;
    return true;
}

// setMaxBurstRng() -- Set max Burst Range (meters)
bool Weapon::setMaxBurstRng(const LCreal v)
{
    maxBurstRng =  v;
    return true;
}

// setLethalRange() -- Set max kill Range (meters)
bool Weapon::setLethalRange(const LCreal v)
{
    lethalRange =  v;
    return true;
}

// setSOBT() -- Set Start-Of-Burn-Time (seconds)
bool Weapon::setSOBT(const LCreal v)
{
    sobt =  v;
    return true;
}

// setEOBT() -- Set End-Of-Burn-Time (seconds)
bool Weapon::setEOBT(const LCreal v)
{
   eobt =  v;
   return true;
}

// Sets the weapon's type ID number 
bool Weapon::setWeaponID(const int n)
{
   weaponID = n;
   return true;
}

// Sets our launcher and station number
bool Weapon::setLauncher(Stores* const l, const unsigned int s)
{
   launcher = l;
   station = s;
   return true;
}

// Sets the weapon released flag
bool Weapon::setReleased(const bool f)
{
   released = f;
   return true;
}

// Sets the weapon power flag
bool Weapon::setPower(const bool f)
{
   power = f;
   return true;
}

// Sets the weapon blocked flag
bool Weapon::setBlocked(const bool b)
{
   blocked = b;
   return true;
}

// Sets the jettision enable flag
bool Weapon::setJettisonable(const bool f)
{
   canJettison = f;
   return true;
}

// Sets the weapon failed flag
bool Weapon::setFailed(const bool f)
{
   failed = f;
   return true;
}

// Sets the hung weapon flag
bool Weapon::setHung(const bool f)
{
   hung = f;
   return true;
}

// Sets the 'will' hang flag
bool Weapon::setWillHang(const bool f)
{
   willHang = f;
   return true;
}

// Sets the dummy weapon flag
bool Weapon::setDummy(const bool f)
{
   dummyFlg = f;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

// released:  Weapon has been released
bool Weapon::setSlotReleased(const Basic::Number* const p)
{
    setReleased( p->getBoolean() );
    return true;
}

// failed: Weapon failed (e.g., reasonableness Test)
bool Weapon::setSlotFailed(const Basic::Number* const p)
{
    setFailed( p->getBoolean() );
    return true;
}

// Power: weapon power flag 
bool Weapon::setSlotPower(const Basic::Number* const p)
{
    setPower( p->getBoolean() );
    return true;
}

// hang: Will be a hung store
bool Weapon::setSlotWillHang(const Basic::Number* const p)
{
    setWillHang( p->getBoolean() );
    return true;
}

// hung: Hung store
bool Weapon::setSlotHung(const Basic::Number* const p)
{
    setHung( p->getBoolean() );
    return true;
}

// dummy: Dummy store
bool Weapon::setSlotDummy(const Basic::Number* const p)
{
    setDummy( p->getBoolean() );
    return true;
}

// maxTOF:  max time of flight      (Basic::Time)
bool Weapon::setSlotMaxTOF(const Basic::Time* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setMaxTOF( Basic::Seconds::convertStatic( *p ) );
   }
   return ok;
}

// maxTOF:  max time of flight      (sec)
bool Weapon::setSlotMaxTOF(const Basic::Number* const p)
{
    return setMaxTOF( p->getReal() );
}

// tsg: time to start guidance    (Basic::Time)
bool Weapon::setSlotTSG(const Basic::Time* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setTSG( Basic::Seconds::convertStatic( *p ) );
   }
   return ok;
}

// tsg: time to start guidance    (sec)
bool Weapon::setSlotTSG(const Basic::Number* const p)
{
    return setTSG( p->getReal() );
}

// maxBurstRng: max burst range    (Basic::Distance)
bool Weapon::setSlotMaxBurstRng(const Basic::Distance* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setMaxBurstRng( Basic::Meters::convertStatic( *p ) );
   }
   return ok;
}

// maxBurstRng: max burst range    (meters)
bool Weapon::setSlotMaxBurstRng(const Basic::Number* const p)
{
    return setMaxBurstRng( p->getReal() );
}


// lethalRange: lethal range    (Basic::Distance)
bool Weapon::setSlotLethalRange(const Basic::Distance* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setLethalRange( Basic::Meters::convertStatic( *p ) );
   }
   return ok;
}

// lethalRange: lethal range    (meters)
bool Weapon::setSlotLethalRange(const Basic::Number* const p)
{
    return setLethalRange( p->getReal() );
}

// sobt: start-of-burn time        (Basic::Time)
bool Weapon::setSlotSOBT(const Basic::Time* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setSOBT( Basic::Seconds::convertStatic( *p ) );
   }
   return ok;
}

// sobt: start-of-burn time        (sec)
bool Weapon::setSlotSOBT(const Basic::Number* const p)
{
    setSOBT( p->getReal() );
    return true;
}

// eobt: end-of-burn time        (Basic::Time)
bool Weapon::setSlotEOBT(const Basic::Time* const p)
{
   bool ok = false;
   if (p != 0) {
      ok = setEOBT( Basic::Seconds::convertStatic( *p ) );
   }
   return ok;
}

// eobt: end-of-burn time        (sec)
bool Weapon::setSlotEOBT(const Basic::Number* const p)
{
    setEOBT( p->getReal() );
    return true;
}

// maxBurstRng: max burst rng    (meters)
bool Weapon::setSlotMaxGimbal(const Basic::Angle* const p)
{
    setMaxGimbalAngle( (LCreal)Basic::Radians::convertStatic(*p) );
    return true;
}

// tgtPos: TEST
bool Weapon::setSlotTgtPos(const Basic::List* const numList)
{
    bool ok = false;
    LCreal values[3];
    int n = numList->getNumberList(values, 3);
    if (n == 3) {
      osg::Vec3 tp(values[0], values[1], values[2]);
      setTargetPosition(tp);
      ok = true;
    }
    return ok;
}

// weaponID: weapon type ID
bool Weapon::setSlotWeaponID(const Basic::Number* const p)
{
    setWeaponID( p->getInt() );
    return true;
}

// jettisonable: weapon can be jettisoned
bool Weapon::setSlotJettisonable(const Basic::Number* const p)
{
    setJettisonable( p->getBoolean() );
    return true;
}

// testTgtName: TEST only: target player name
bool Weapon::setSlotTestTgtName(const Basic::String* const p)
{
   tstTgtNam = p;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Weapon::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Weapon::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "released: " << isReleased() << std::endl;

    indent(sout,i+j);
    sout << "failed: " << isFailed() << std::endl;

    indent(sout,i+j);
    sout << "power: " << isPowerOn() << std::endl;

    indent(sout,i+j);
    sout << "hang: " << getWillHang() << std::endl;

    if ( isHung() ) {
        indent(sout,i+j);
        sout << "hung: true" << std::endl;
    }

    indent(sout,i+j);
    sout << "maxTOF: " << getMaxTOF() << std::endl;

    indent(sout,i+j);
    sout << "tsg: " << getTSG() << std::endl;

    indent(sout,i+j);
    sout << "maxBurstRng: ( Meters " << getMaxBurstRng() << " )" << std::endl;

    indent(sout,i+j);
    sout << "lethalRange: ( Meters " << getLethalRange() << " )" << std::endl;

    indent(sout,i+j);
    sout << "sobt: " << getSOBT() << std::endl;

    indent(sout,i+j);
    sout << "eobt: " << getEOBT() << std::endl;

    indent(sout,i+j);
    sout << "maxGimbal: ( Radians " << getMaxGimbalAngle() << " )" << std::endl;

    indent(sout,i+j);
    sout << "weaponID: " << getWeaponID() << std::endl;

    if ( isDummy() ) {
        indent(sout,i+j);
        sout << "dummy: true" << std::endl;
    }

    indent(sout,i+j);
    sout << "jettisonable: " << isJettisonable() << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
    	indent(sout,i);
    	sout << ")" << std::endl;
    }

    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
