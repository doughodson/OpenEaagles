//==============================================================================
// class Player
//==============================================================================

#include "openeaagles/simulation/Player.h"

#include "openeaagles/simulation/Datalink.h"
#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/simulation/DynamicsModel.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Gimbal.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrSignature.h"
#include "openeaagles/simulation/IrSystem.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Pilot.h"
#include "openeaagles/simulation/Radio.h"
#include "openeaagles/simulation/RfSensor.h"
#include "openeaagles/simulation/Signatures.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/Terrain.h"
#include "openeaagles/base/LatLon.h"
#include "openeaagles/base/Nav.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Statistic.h"
#include "openeaagles/base/units/Times.h"
#include "openeaagles/base/osg/Quat"

#include "openeaagles/simulation/SynchronizedState.h"

#include <cmath>

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(Player,"Player")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Player)
   // Player's initial position relative to the simulation's reference point
   "initXPos",          //  1) Initial X position    (meters, base::Distance)
   "initYPos",          //  2) Initial Y position    (meters, base::Distance)
   "initAlt",           //  3) Initial Altitude      (meters, base::Distance)
   "initPosition",      //  4) Initial Position Vector: meters [ north east down ]

   // Player's initial position in latitude, longitude and altitude (from initAlt above)
   "initLatitude",      //  5) Initial latitude    (base::Angle, base::LatLon or degrees)
   "initLongitude",     //  6) Initial longitude    (base::Angle, base::LatLon or degrees)

   // Player's initial geocentric position
   "initGeocentric",    //  7) Initial geocentric position vector [ x y z ] (meters)

   // Player's initial Euler angles
   "initRoll",          //  8) Initial roll:    (radians, base::Angle) (or use initEuler)
   "initPitch",         //  9) Initial pitch:   (radians, base::Angle) (or use initEuler)
   "initHeading",       // 10) Initial heading: (radians, base::Angle) (or use initEuler)
   "initEuler",         // 11) Initial Euler Angles: radians [ roll pitch yaw ] (or use below)

   // Player's initial velocity
   "initVelocity",      // 12) Initial Velocity: meters/sec
   "initVelocityKts",   // 13) Initial Velocity: knots

   // Player's type, id and and other parameters
   "type",              // 14) Type of player vehicle ("F-16A", "Tank", "SA-6", etc.)
   "id",                // 15) Player id   [ 1 .. 65535 ]
   "side",              // 16) Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE }
   "mode",              // 17) Initial player mode ( INACTIVE, ACTIVE, DEAD )

   "signature",         // 18) Player's RCS signature
   "irSignature",       // 19) Player's IR Signature
   "camouflageType",    // 20) User defined camouflage type (positive integer or zero for none)

   "terrainElevReq",    // 21) Terrain elevation update requested
   "interpolateTerrain",// 22) Interpolate our terrain1 data
   "terrainOffset",     // 23) Ground clamp offset from terrain to player's CG (base::Distance)

   "positionFreeze",    // 24) Position freeze
   "altitudeFreeze",    // 25) Altitude freeze
   "attitudeFreeze",    // 26) Attitude freeze
   "fuelFreeze",        // 27) Fuel freeze
   "crashOverride",     // 28) Crash Override (i.e., ignore collision and crash events)
   "killOverride",      // 29) Kill/Damage Override -- player can not be killed or damaged(by a weapon)
   "killRemoval",       // 30) If true destroyed players are set to KILLED and are eventually removed (default: false)
   "enableNetOutput",   // 31) Enable the network output!

   "dataLogTime",       // 32) Data logging time -- time between player data samples

   // Player's test angular velocities
   "testRollRate",      // 33) Test roll rate (units per second)
   "testPitchRate",     // 34) Test pitch rate (units per second)
   "testYawRate",       // 35) Test heading rate (units per second)
   "testBodyAxis",      // 36) Test rates are in body coordinates else Euler rates (default: false)

   "useCoordSys"        // 37) Coord system to use for position updating { WORLD, GEOD, LOCAL }
END_SLOTTABLE(Player)

// Map slot table to handles
BEGIN_SLOT_MAP(Player)

   ON_SLOT( 1, setSlotInitXPos, base::Distance)
   ON_SLOT( 1, setSlotInitXPos, base::Number)

   ON_SLOT( 2, setSlotInitYPos, base::Distance)
   ON_SLOT( 2, setSlotInitYPos, base::Number)

   ON_SLOT( 3, setSlotInitAlt,  base::Distance)
   ON_SLOT( 3, setSlotInitAlt,  base::Number)

   ON_SLOT( 4, setSlotInitPosition, base::List)

   ON_SLOT( 5, setSlotInitLat, base::LatLon)
   ON_SLOT( 5, setSlotInitLat, base::Angle)
   ON_SLOT( 5, setSlotInitLat, base::Number)

   ON_SLOT( 6, setSlotInitLon, base::LatLon)
   ON_SLOT( 6, setSlotInitLon, base::Angle)
   ON_SLOT( 6, setSlotInitLon, base::Number)

   ON_SLOT( 7, setSlotInitGeocentric, base::List)

   ON_SLOT( 8, setSlotInitRoll, base::Angle)
   ON_SLOT( 8, setSlotInitRoll, base::Number)

   ON_SLOT( 9, setSlotInitPitch, base::Angle)
   ON_SLOT( 9, setSlotInitPitch, base::Number)

   ON_SLOT(10, setSlotInitHeading, base::Angle)
   ON_SLOT(10, setSlotInitHeading, base::Number)

   ON_SLOT(11, setSlotInitEulerAngles, base::List)
   ON_SLOT(12, setSlotInitVelocity, base::Number)
   ON_SLOT(13, setSlotInitVelocityKts, base::Number)

   ON_SLOT(14, setType, base::String)
   ON_SLOT(15, setSlotID, base::Number)
   ON_SLOT(16, setSlotSide, base::String)
   ON_SLOT(17, setSlotInitMode, base::String)

   ON_SLOT(18, setSlotSignature, RfSignature)
   ON_SLOT(19, setSlotIrSignature, IrSignature)
   ON_SLOT(20, setSlotCamouflageType, base::Number)

   ON_SLOT(21, setSlotTerrainElevReq, base::Number)
   ON_SLOT(22, setSlotInterpolateTerrain, base::Number)
   ON_SLOT(23, setSlotTerrainOffset, base::Distance)

   ON_SLOT(24, setSlotPositionFreeze, base::Number)
   ON_SLOT(25, setSlotAltitudeFreeze, base::Number)
   ON_SLOT(26, setSlotAttitudeFreeze, base::Number)
   ON_SLOT(27, setSlotFuelFreeze, base::Number)
   ON_SLOT(28, setSlotCrashOverride, base::Number)
   ON_SLOT(29, setSlotKillOverride, base::Number)
   ON_SLOT(30, setSlotKillRemoval, base::Number)
   ON_SLOT(31, setSlotEnableNetOutput, base::Number)
   ON_SLOT(32, setSlotDataLogTime, base::Time)

   ON_SLOT(33, setSlotTestRollRate, base::Angle)
   ON_SLOT(34, setSlotTestPitchRate, base::Angle)
   ON_SLOT(35, setSlotTestYawRate, base::Angle)
   ON_SLOT(36, setSlotTestBodyAxis, base::Number)

   ON_SLOT(37, setSlotUseCoordSys, base::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Player)

   // We're just killed by 'Player'
   ON_EVENT_OBJ(KILL_EVENT,killedNotification,Player)

   // We're just killed by unknown player
   ON_EVENT(KILL_EVENT,killedNotification)

   // We just collided 'Player'
   ON_EVENT_OBJ(CRASH_EVENT,collisionNotification,Player)

   // We just crashed
   ON_EVENT(CRASH_EVENT,crashNotification)

   // We were just hit by a R/F emission
   ON_EVENT_OBJ(RF_EMISSION,onRfEmissionEventPlayer,Emission)

   // Another player is requesting reflection of the R/F emission hitting us
   ON_EVENT_OBJ(RF_REFLECTIONS_REQUEST, onReflectionsRequest, base::Component)

   // We were just hit by a reflected R/F emission
   ON_EVENT_OBJ(RF_REFLECTED_EMISSION,onRfReflectedEmissionEventPlayer,Emission)

   // Another player is cancelling its request for reflected R/F emissions
   ON_EVENT_OBJ(RF_REFLECTIONS_CANCEL,  onReflectionsCancel,  base::Component)

   // Data link message event
   ON_EVENT_OBJ(DATALINK_MESSAGE,onDatalinkMessageEventPlayer,base::Object)

   // Weapon release button event (with switch state)
   ON_EVENT_OBJ(WPN_REL_EVENT,onWpnRelEvent,base::Boolean)

   // Weapon release button event
   ON_EVENT(WPN_REL_EVENT,onWpnRelEvent)

   // Trigger switch (with switch state)
   ON_EVENT_OBJ(TRIGGER_SW_EVENT,onTriggerSwEvent,base::Boolean)

   // Trigger event
   ON_EVENT(TRIGGER_SW_EVENT,onTriggerSwEvent)

   // Target step switch event
   ON_EVENT(TGT_STEP_EVENT,onTgtStepEvent)

   // An IR seeker is querying us for our IR signature
   ON_EVENT_OBJ(IR_QUERY,onIrMsgEventPlayer,IrQueryMsg)

   // We were just hit with a directed energy emission
   ON_EVENT_OBJ(DE_EMISSION,onDeEmissionEvent,base::Object)

END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Player::Player()
{
   STANDARD_CONSTRUCTOR()

   initData();
}


//------------------------------------------------------------------------------
// initData() -- init our member data
//------------------------------------------------------------------------------
void Player::initData()
{
   static base::String generic("GenericPlayer");
   type = nullptr;
   setType(&generic);

   id = 0;
   pname = "";
   side = GRAY;

   mode = ACTIVE;
   latitude = 0.0;
   longitude = 0.0;
   altitude = 0.0;
   base::Nav::computeWorldMatrix(latitude, longitude, &wm);

   angles.set(0,0,0);
   base::Nav::computeRotationalMatrix(angles, &rm, &scPhi, &scTheta, &scPsi);

   rmW2B = rm * wm;
   base::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   q.set(rm);

   vp = 0.0;
   gndSpd = 0.0;
   gndTrk = 0.0;
   useCoordSys = CS_NONE;
   useCoordSysN1 = CS_NONE;

   posVecNED.set(0,0,0);
   velVecNED.set(0,0,0);
   accelVecNED.set(0,0,0);
   velVecBody.set(0,0,0);
   posVecECEF.set(0,0,0);
   velVecECEF.set(0,0,0);
   velVecN1.set(0,0,0);
   accelVecECEF.set(0,0,0);
   accelVecBody.set(0,0,0);

   angularVel.set(0,0,0);
   gcAngVel.set(0,0,0);

   tElev    = 0.0;
   tElevValid = false;

   altSlaved = false;
   posSlaved = false;

   posVecValid = false;
   posFrz = false;
   altFrz = false;
   attFrz = false;
   fuelFrz = false;
   crashOverride = false;
   killOverride = false;
   killRemoval = false;
   tElevReq = false;     // default: terrain height isn't required
   interpTrrn = false;
   tOffset = 0.0;

   signature = nullptr;
   irSignature = nullptr;
   camouflage = 0;
   damage = 0.0;
   smoking = 0.0;
   flames = 0.0;
   justKilled = false;
   killedBy = 0;

   initPosVec.set(0,0);
   initPosFlg = false;

   initGeoPosVec.set(0,0,0);
   initGeoPosFlg = false;

   initLat = 0.0;
   initLon = 0.0;
   initLatLonFlg = false;

   initAlt = 0.0;
   initVp = 0.0;
   initMode = mode;
   initAngles.set(0,0,0);
   testAngRates.set(0,0,0);
   testBodyAxis = false;

   dataLogTimer = 0.0;
   dataLogTime  = 0.0;

   nib = nullptr;
   netID = 0;
   enableNetOutput = true;
   nibList = new Nib*[NetIO::MAX_NETWORD_ID];
   for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
      nibList[i] = nullptr;
   }

   sim = nullptr;
   dyn = nullptr;
   datalink = nullptr;
   gimbal = nullptr;
   nav = nullptr;
   obc = nullptr;
   pilot = nullptr;
   radio = nullptr;
   sensor = nullptr;
   irSystem = nullptr;
   sms = nullptr;
   loadSysPtrs = true;

   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      rfReflect[i] = nullptr;
      rfReflectTimer[i] = 0;
   }

   syncState1Ready = false;
   syncState2Ready = false;
   syncState1.clear();
   syncState2.clear();

}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Player::copyData(const Player& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   const base::String* tt = org.type;
   setType( const_cast<base::String*>(static_cast<const base::String*>(tt)) );

   id = org.id;

   if (org.pname != nullptr) {
      setName(org.pname);
   }

   side = org.side;

   mode = org.mode;
   latitude = org.latitude;
   longitude = org.longitude;
   altitude = org.altitude;
   vp = org.vp;
   gndSpd = org.gndSpd;
   gndTrk = org.gndTrk;
   useCoordSys = org.useCoordSys;
   useCoordSysN1 = org.useCoordSysN1;

   posVecNED = org.posVecNED;
   velVecNED = org.velVecNED;
   accelVecNED = org.accelVecNED;
   velVecBody = org.velVecBody;
   posVecECEF = org.posVecECEF;
   velVecECEF = org.velVecECEF;
   velVecN1 = org.velVecN1;
   accelVecECEF = org.accelVecECEF;
   accelVecBody = org.accelVecBody;

   wm = org.wm;

   angles = org.angles;
   rm = org.rm;
   scPhi = org.scPhi;
   scTheta = org.scTheta;
   scPsi = org.scPsi;

   anglesW = org.anglesW;
   rmW2B = org.rmW2B;
   scPhiW = org.scPhiW;
   scThetaW = org.scThetaW;
   scPsiW = org.scPsiW;

   q = org.q;

   angularVel = org.angularVel;
   gcAngVel = org.gcAngVel;

   tElev = org.tElev;
   tElevValid = org.tElevValid;

   altSlaved = org.altSlaved;
   posSlaved = org.posSlaved;
   posVecValid = org.posVecValid;
   posFrz = org.posFrz;
   altFrz = org.altFrz;
   attFrz = org.attFrz;
   fuelFrz = org.fuelFrz;
   crashOverride = org.crashOverride;
   killOverride = org.killOverride;
   killRemoval = org.killRemoval;
   tElevReq = org.tElevReq;
   interpTrrn = org.interpTrrn;
   tOffset = org.tOffset;

   if (org.signature != nullptr) {
      RfSignature* copy = org.signature->clone();
      setSlotSignature( copy );
      copy->unref();
   }
   else {
      setSlotSignature(nullptr);
   }

   if (org.irSignature != nullptr) {
      IrSignature* copy = org.irSignature->clone();
      setSlotIrSignature( copy );
      copy->unref();
   }
   else {
      setSlotIrSignature(nullptr);
   }

   camouflage = org.camouflage;
   damage = org.damage;
   smoking = org.smoking;
   flames = org.flames;
   justKilled = org.justKilled;
   killedBy = org.killedBy;

   initPosVec = org.initPosVec;
   initPosFlg = org.initPosFlg;

   initGeoPosVec = org.initGeoPosVec;
   initGeoPosFlg = org.initGeoPosFlg;

   initLat = org.initLat;
   initLon = org.initLon;
   initLatLonFlg = org.initLatLonFlg;

   initAlt = org.initAlt;
   initVp = org.initVp;
   initMode = org.initMode;
   initAngles = org.initAngles;
   testAngRates = org.testAngRates;
   testBodyAxis = org.testBodyAxis;

   dataLogTimer = org.dataLogTimer;
   dataLogTime  = org.dataLogTime;

   netID = org.netID;
   enableNetOutput = org.enableNetOutput;

   // NIB pointers are not copied!
   setNib( nullptr );
   for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
      setOutgoingNib(nullptr, i);
   }

   // The following are not copied ..
   sim = nullptr;
   setDynamicsModel(nullptr);
   setDatalink(nullptr);
   setGimbal(nullptr);
   setIrSystem(nullptr);
   setNavigation(nullptr);
   setOnboardComputer(nullptr);
   setPilot(nullptr);
   setRadio(nullptr);
   setSensor(nullptr);
   setStoresMgr(nullptr);
   loadSysPtrs = true;

   // Reflected emission requests are not copied
   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != nullptr) { rfReflect[i]->unref(); rfReflect[i] = nullptr; }
      rfReflectTimer[i] = 0;
   }

   syncState1Ready = org.syncState1Ready;
   syncState2Ready = org.syncState2Ready;
   syncState1 = org.syncState1;
   syncState2 = org.syncState2;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Player::deleteData()
{
   type = nullptr;
   signature = nullptr;
   irSignature = nullptr;
   sim = nullptr;

   setNib(nullptr);
   if (nibList != nullptr) {
      for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
         setOutgoingNib(nullptr, i);
      }
      delete[] nibList;
      nibList = nullptr;
   }

   setDynamicsModel(nullptr);
   setDatalink(nullptr);
   setGimbal(nullptr);
   setIrSystem(nullptr);
   setNavigation(nullptr);
   setOnboardComputer(nullptr);
   setPilot(nullptr);
   setRadio(nullptr);
   setSensor(nullptr);
   setStoresMgr(nullptr);

   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != nullptr) { rfReflect[i]->unref(); rfReflect[i] = nullptr; }
   }
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool Player::shutdownNotification()
{
   if (nib != nullptr) nib->event(SHUTDOWN_EVENT);
   if (nibList != nullptr) {
      for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
         if (nibList[i] != nullptr) nibList[i]->event(SHUTDOWN_EVENT);
      }
   }

   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != nullptr) { rfReflect[i]->unref(); rfReflect[i] = nullptr; }
   }

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// isFrozen() -- checks both player's freeze flag and the simulation's freeze flag
//------------------------------------------------------------------------------
bool Player::isFrozen() const
{
   bool frz = BaseClass::isFrozen();
   if (!frz && sim != nullptr) frz = sim->isFrozen();
   return frz;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Player::reset()
{
   // ---
   // Update our system (component) pointers
   // ---
   updateSystemPointers();
   loadSysPtrs = false;

   if (isLocalPlayer()) {

      // ---
      // Reset Position
      // ---

      if (initPosFlg) {
         setPosition(initPosVec[INORTH], initPosVec[IEAST], -initAlt);
         if (useCoordSys == CS_NONE) useCoordSys = CS_LOCAL;
      }
      else if (initLatLonFlg) {
         setPositionLLA(initLat, initLon, initAlt);
         if (useCoordSys == CS_NONE) useCoordSys = CS_GEOD;
      }
      else if (initGeoPosFlg) {
         setGeocPosition(initGeoPosVec);
         if (useCoordSys == CS_NONE) useCoordSys = CS_WORLD;
      }
      else {
         setPosition(0,0,-initAlt,false);
         if (useCoordSys == CS_NONE) useCoordSys = CS_LOCAL;
      }

      useCoordSysN1 = CS_NONE;

      // ---
      // Reset Euler angles and rates
      // ---
      setEulerAngles(initAngles);
      setAngularVelocities(testAngRates);
      velVecN1.set(0,0,0);

      // ---
      // Reset velocities
      // ---
      setVelocityBody(initVp, 0.0, 0.0);
      setAccelerationBody(0.0, 0.0, 0.0);

      // ---
      // Reset misc
      // ---
      setMode(initMode);
      setDamage(0.0);
      setSmoke(0.0);
      setFlames(0.0);
      justKilled = false;
      killedBy = 0;

      altSlaved = false;
      posSlaved = false;

      tElev    = 0.0;
      tElevValid = false;

      syncState1Ready = false;
      syncState2Ready = false;
   }

   // ---
   // Reset our base class
   // -- Do this last because it sends reset pulses to our components and
   //    we want to make sure our data is initialized first.
   // ---
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Player::updateTC(const double dt0)
{
   // Make sure we've loaded our system pointers
   if (loadSysPtrs) {
      updateSystemPointers();
      loadSysPtrs = false;
   }

   if (mode == ACTIVE || mode == PRE_RELEASE) {

      // ---
      // Time-out requests for reflections of RF emissions hitting us
      // ---
      for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
         if (rfReflect[i] != nullptr) {
            rfReflectTimer[i] -= dt0;
            if (rfReflectTimer[i] <= 0) {
               // Clear the request
               rfReflect[i]->unref();
               rfReflect[i] = nullptr;
            }
         }
      }

      // ---
      // Delta time -- real or frozen?
      // ---
      double dt = dt0;
      if (isFrozen()) dt = 0.0;

      // ---
      // Compute delta time for modules running every fourth phase
      // ---
      double dt4 = dt * 4.0f; // Delta time for items running every fourth phase
      switch (getSimulation()->phase()) {

         // Phase 0 -- Dynamics
         case 0 : {
            // Our dynamics
            dynamics(dt4);

            // Log our player's dynamic data just after its been updated ...
            if (dataLogTime > 0.0) {
               // When we have a data logging time, update the timer
               dataLogTimer -= dt4;
               if (dataLogTimer <= 0.0) {
                  // At timeout, log the player's data and ...

                  BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_PLAYER_DATA )
                     SAMPLE_1_OBJECT( this )
                  END_RECORD_DATA_SAMPLE()

                  // TabLogger is deprecated
                  if (getAnyEventLogger() != nullptr) {
                     TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(2, this); // type 2: update
                     getAnyEventLogger()->log(evt);
                     evt->unref();
                  }

                  // reset the timer.
                  dataLogTimer = dataLogTime;
               }
            }

            // Update signatures after we've updated our dynamics
            if (signature != nullptr) signature->updateTC(dt4);
            if (irSignature != nullptr) irSignature->updateTC(dt4);
         }
         break;

         // Phase 1 -- Sensors transmit
         case 1 :
         break;

         // Phase 2 -- Sensors Receive
         case 2 :
         break;

         // Phase 3 -- PDL and other logic
         case 3 :
         break;

      }

      // ---
      // Notes:
      //  a) Remember that our subsystems in the components list (e.g., pilot, nav,
      //     sms and obc) are updated by our call to BaseClass:updateTC()
      //  b) We're calling BaseClass::updateTC() class because we want to update
      //     our player dynamics, etc before our subsystems.
      // ---
      BaseClass::updateTC(dt);

   }
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void Player::updateData(const double dt)
{
   if (mode == ACTIVE || mode == PRE_RELEASE) {

      // Update signatures
      if (signature != nullptr) signature->updateData(dt);
      if (irSignature != nullptr) irSignature->updateData(dt);

      // ---
      // Update the terrain elevation
      // ---
      updateElevation();

      // ---
      // Note: our subsystems in the components list (e.g., pilot, nav, sms and obc) are updated
      // by our call to BaseClass:updateData()
      // ---
      BaseClass::updateData(dt);
   }
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------

// getMajorType() -- Returns the player's major type
unsigned int Player::getMajorType() const
{
   return GENERIC;
}

// Player's gross weight (lbs)
double Player::getGrossWeight() const
{
   return 0.0;
}

// Default: mach number
double Player::getMach() const
{
   // Really only good up to around 30,000 feet, and
   // using standard air temp of 15 degrees Celsius

   const double G = 1.4;     // density ratio      // number
   const double R = 287.06;  // gas constant       // mps/degK
   const double L = 0.0065;  // temp. lapse rate   // degC/meter

   double altitude   = getAltitude();              // meters
   double velocPlane = getTotalVelocity();         // meters/sec
   double Tc         = 15.0 - L * altitude;        // degrees Celsius
   double Tk         = Tc + 273.15;                // degrees Kelvin
   double velocSound = std::sqrt(G * R * Tk);      // meters/sec
   double mach       = velocPlane / velocSound;    // number

   return mach;
}

// Player's Center-of-Gravity (%)
double Player::getCG() const
{
   return 0.0;
}

// Return true if heading-hold mode is on
bool Player::isHeadingHoldOn() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->isHeadingHoldOn();
   else
      return false;
}

// Return commanded heading, default (radians)
double Player::getCommandedHeading() const
{
   return getCommandedHeadingD() * static_cast<double>(base::Angle::D2RCC);
}

// Return commanded heading (degrees)
double Player::getCommandedHeadingD() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->getCommandedHeadingD();
   else
      return 0;
}

// Return commanded heading (radians)
double Player::getCommandedHeadingR() const
{
   return getCommandedHeadingD() * static_cast<double>(base::Angle::D2RCC);
}

// Return true if velocity-hold mode is on
bool Player::isVelocityHoldOn() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->isVelocityHoldOn();
   else
      return false;
}

// Commanded (true) velocity (knots)
double Player::getCommandedVelocityKts() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->getCommandedVelocityKts();
   else
      return 0;
}

// Commanded (true) velocity (Feet/Sec)
double Player::getCommandedVelocityFps() const
{
   return getCommandedVelocityKts() * base::Distance::NM2FT / base::Time::H2S;
}

// Commanded (true) velocity (Meters/Sec)
double Player::getCommandedVelocityMps() const
{
   return getCommandedVelocityKts() * base::Distance::NM2M / base::Time::H2S;
}

// Return true if altitude-hold mode is on
bool Player::isAltitudeHoldOn() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->isAltitudeHoldOn();
   else
      return false;
}

// Get commanded (HAE) altitude, default (meters)
double Player::getCommandedAltitude() const
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->getCommandedAltitude();
   else
      return 0;
}

// Get commanded (HAE) altitude (meters)
double Player::getCommandedAltitudeM() const
{
   return getCommandedAltitude();
}

// Get commanded (HAE) altitude (feet)
double Player::getCommandedAltitudeFt() const
{
   return getCommandedAltitude() * base::Distance::M2FT;
}

// True if player is destroyed
bool Player::isDestroyed() const
{
   return (damage > 0.999f);
}

// Player's outgoing NIB(s)
Nib* Player::getLocalNib(const unsigned int netId)
{
   Nib* p = nullptr;
   if (nibList != nullptr && netId >= 1 && netId <= NetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

// Player's outgoing NIB(s)  (const version)
const Nib* Player::getLocalNib(const unsigned int netId) const
{
   const Nib* p = nullptr;
   if (nibList != nullptr && netId >= 1 && netId <= NetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

// Earth radius (meters)
double Player::getEarthRadius() const
{
   double erad = base::Nav::ERAD60 * base::Distance::NM2M;  // (default)

   const Simulation* sim = getSimulation();
   if (sim != nullptr) {
      const base::EarthModel* pModel = sim->getEarthModel();
      if (pModel == nullptr) pModel = &base::EarthModel::wgs84;

      const double b  = pModel->getB();   // semi-major axis
      const double e2 = pModel->getE2();  // eccentricity squared

      const double slat = getLatitude();
      const double cosSlat = std::cos(base::Angle::D2RCC * slat);

      erad = b / std::sqrt(1.0 - e2*cosSlat*cosSlat);
   }

   return erad;
}

//------------------------------------------------------------------------------
// Controlling simulation access functions
//------------------------------------------------------------------------------

// Controlling simulation model
Simulation* Player::getSimulation()
{
   if (sim == nullptr) {
      getSimulationImp();
   }
   return sim;
}

// Controlling simulation model (const version)
const Simulation* Player::getSimulation() const
{
   if (sim == nullptr) {
      (const_cast<Player*>(this))->getSimulationImp();
   }
   return sim;
}

// Find our simulation model
Simulation* Player::getSimulationImp()
{
   if (sim == nullptr) {
      sim = static_cast<Simulation*>(findContainerByType(typeid(Simulation)));
      if (sim == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Player::getSimulationImp(): ERROR, unable to locate the Simulation class!" << std::endl;
      }
   }
   return sim;
}


//------------------------------------------------------------------------------
// Dynamics model access functions
//------------------------------------------------------------------------------

// Player's dynamics model
DynamicsModel* Player::getDynamicsModel()
{
   return (dyn != nullptr) ? (static_cast<DynamicsModel*>(dyn->object())) : nullptr;
}

// Player's dynamics model (const version)
const DynamicsModel* Player::getDynamicsModel() const
{
   return (dyn != nullptr) ? (static_cast<DynamicsModel*>(dyn->object())) : nullptr;
}

// Name of the Player's dynamics model
const base::Identifier* Player::getDynamicsModelName() const
{
   return (dyn != nullptr) ? dyn->slot() : nullptr;
}

//------------------------------------------------------------------------------
// Pilot model (autopilot, pilot-decision-logic (PDL), pilot interface) access functions
//------------------------------------------------------------------------------

// Player's pilot model
Pilot* Player::getPilot()
{
   return (pilot != nullptr) ? (static_cast<Pilot*>(pilot->object())) : nullptr;
}

// Player's pilot model (const version)
const Pilot* Player::getPilot() const
{
   return (pilot != nullptr) ? (static_cast<Pilot*>(pilot->object())) : nullptr;
}

// Name of the player's pilot model
const base::Identifier* Player::getPilotName() const
{
   return (pilot != nullptr) ? pilot->slot() : nullptr;
}

// Returns a Pilot model by its name
Pilot* Player::getPilotByName(const char* const name1)
{
   Pilot* p = nullptr;
   if (pilot != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of our top level system
      base::Pair* pair = nullptr;
      if ( *getPilotName() == fname ) {
         // The first name matches our top level system name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getPilot()->findByName(name);
         }
         else {
            // It's a simple name and our top level system is a match.
            pair = pilot;
         }
      }

      // Our top level system isn't a match, so pass the whole name down
      else {
         pair = getPilot()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Pilot*>( pair->object() );
      }
   }
   return p;
}

// Returns a Pilot model by its type
base::Pair* Player::getPilotByType(const std::type_info& type)
{
   base::Pair* p = nullptr;  // Our return value

   if (pilot != nullptr) {
      Pilot* root = getPilot();  // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = pilot;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Stores (weapons, fuel) manager model access functions
//------------------------------------------------------------------------------

// Player's stores (weapons, fuel) manager model
StoresMgr* Player::getStoresManagement()
{
   return (sms != nullptr) ? (static_cast<StoresMgr*>(sms->object())) : nullptr;
}

// Player's stores (weapons, fuel) manager model (const version)
const StoresMgr* Player::getStoresManagement() const
{
   return (sms != nullptr) ? (static_cast<StoresMgr*>(sms->object())) : nullptr;
}

// Name of the player's stores (weapons, fuel) manager model
const base::Identifier* Player::getStoresManagementName() const
{
   return (sms != nullptr) ? sms->slot() : nullptr;
}

//------------------------------------------------------------------------------
// Datalink model access functions
//------------------------------------------------------------------------------

// Player's top level Datalink model
Datalink* Player::getDatalink()
{
   return (datalink != nullptr) ? (static_cast<Datalink*>(datalink->object())) : nullptr;
}

// Player's top level Datalink (const version)
const Datalink* Player::getDatalink() const
{
   return (datalink != nullptr) ? (static_cast<Datalink*>(datalink->object())) : nullptr;
}

// Name of the player's top level Datalink model
const base::Identifier* Player::getDatalinkName() const
{
   return (datalink != nullptr) ? datalink->slot() : nullptr;
}

// Returns a Datalink model by its name
Datalink* Player::getDatalinkByName(const char* const name1)
{
   Datalink* p = nullptr;
   if (datalink != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of our top level system
      base::Pair* pair = nullptr;
      if ( *getDatalinkName() == fname ) {
         // The first name matches our top level system name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getDatalink()->findByName(name);
         }
         else {
            // It's a simple name and our top level system is a match.
            pair = datalink;
         }
      }

      // Our top level system isn't a match, so pass the whole name down
      else {
         pair = getDatalink()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Datalink*>( pair->object() );
      }
   }
   return p;
}

// Returns a Datalink model by its type
base::Pair* Player::getDatalinkByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                 // Our return value

   if (datalink != nullptr) {
      Datalink* root = getDatalink();  // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = datalink;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Gimbal model access functions
//------------------------------------------------------------------------------

// Player's top level Gimbal model
Gimbal* Player::getGimbal()
{
   return (gimbal != nullptr) ? ((Gimbal*) gimbal->object()) : nullptr;
}

// Player's top level Gimbal (const version)
const Gimbal* Player::getGimbal() const
{
   return (gimbal != nullptr) ? (static_cast<Gimbal*>(gimbal->object())) : nullptr;
}

// Name of the player's top level Gimbal model
const base::Identifier* Player::getGimbalName() const
{
   return (gimbal != nullptr) ? gimbal->slot() : nullptr;
}

// Returns a Gimbal model by its name
Gimbal* Player::getGimbalByName(const char* const name1)
{
   Gimbal* p = nullptr;
   if (gimbal != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of our top level system
      base::Pair* pair = nullptr;
      if ( *getGimbalName() == fname ) {
         // The first name matches our top level system name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getGimbal()->findByName(name);
         }
         else {
            // It's a simple name and our top level system is a match.
            pair = gimbal;
         }
      }

      // Our top level system isn't a match, so pass the whole name down
      else {
         pair = getGimbal()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Gimbal*>( pair->object() );
      }
   }
   return p;
}

// Returns a Gimbal model by its type
base::Pair* Player::getGimbalByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                // Our return value

   if (gimbal != nullptr) {
      Gimbal* root = getGimbal();   // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = gimbal;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Navigational model access functions
//------------------------------------------------------------------------------

// Player's top level Navigation model
Navigation* Player::getNavigation()
{
   return (nav != nullptr) ? (static_cast<Navigation*>(nav->object())) : nullptr;
}

// Player's top level Navigation (const version)
const Navigation* Player::getNavigation() const
{
   return (nav != nullptr) ? (static_cast<Navigation*>(nav->object())) : nullptr;
}

// Name of the player's top level Navigation model
const base::Identifier* Player::getNavigationName() const
{
   return (nav != nullptr) ? nav->slot() : nullptr;
}

// Returns a Navigation model by its name
Navigation* Player::getNavigationByName(const char* const name1)
{
   Navigation* p = nullptr;
   if (nav != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of the root system
      base::Pair* pair = nullptr;
      if ( *getNavigationName() == fname ) {
         // The first name matches our root name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getNavigation()->findByName(name);
         }
         else {
            // It's a simple name and our root is a match.
            pair = nav;
         }
      }

      // Our root isn't a match, so pass the whole name down
      else {
         pair = getNavigation()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Navigation*>( pair->object() );
      }
   }
   return p;
}

// Returns a Navigation model by its type
base::Pair* Player::getNavigationByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                 // Our return value

   if (nav != nullptr) {
      Navigation* root = getNavigation();    // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = nav;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Onboard Computer model access functions
//------------------------------------------------------------------------------

// Player's top level OnboardComputer model
OnboardComputer* Player::getOnboardComputer()
{
   return (obc != nullptr) ? (static_cast<OnboardComputer*>(obc->object())) : nullptr;
}

// Player's top level OnboardComputer (const version)
const OnboardComputer* Player::getOnboardComputer() const
{
   return (obc != nullptr) ? (static_cast<OnboardComputer*>(obc->object())) : nullptr;
}

// Name of the player's top level OnboardComputer model
const base::Identifier* Player::getOnboardComputerName() const
{
   return (obc != nullptr) ? obc->slot() : nullptr;
}

// Returns an OnboardComputer model by its name
OnboardComputer* Player::getOnboardComputerByName(const char* const name1)
{
   OnboardComputer* p = nullptr;
   if (obc != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of the root system
      base::Pair* pair = nullptr;
      if ( *getOnboardComputerName() == fname ) {
         // The first name matches our root name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getOnboardComputer()->findByName(name);
         }
         else {
            // It's a simple name and our root is a match.
            pair = obc;
         }
      }

      // Our root isn't a match, so pass the whole name down
      else {
         pair = getOnboardComputer()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<OnboardComputer*>( pair->object() );
      }
   }
   return p;
}

// Returns an OnboardComputer model by its type
base::Pair* Player::getOnboardComputerByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                // Our return value

   if (obc != nullptr) {
      OnboardComputer* root = getOnboardComputer();    // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = obc;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Radio model access functions
//------------------------------------------------------------------------------

// Player's top level Radio model
Radio* Player::getRadio()
{
   return (radio != nullptr) ? (static_cast<Radio*>(radio->object())) : nullptr;
}

// Player's top level Radio (const version)
const Radio* Player::getRadio() const
{
   return (radio != nullptr) ? (static_cast<Radio*>(radio->object())) : nullptr;
}

// Name of the player's top level Radio model
const base::Identifier* Player::getRadioName() const
{
   return (radio != nullptr) ? radio->slot() : nullptr;
}

// Returns a Radio model by its name
Radio* Player::getRadioByName(const char* const name1)
{
   Radio* p = nullptr;
   if (radio != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of the root system
      base::Pair* pair = nullptr;
      if ( *getRadioName() == fname ) {
         // The first name matches our root name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getRadio()->findByName(name);
         }
         else {
            // It's a simple name and our root is a match.
            pair = radio;
         }
      }

      // Our root isn't a match, so pass the whole name down
      else {
         pair = getRadio()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Radio*>( pair->object() );
      }
   }
   return p;
}

// Returns a Radio model by its type
base::Pair* Player::getRadioByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                // Our return value

   if (radio != nullptr) {
      Radio* root = getRadio();    // Root node of the list
      if (root->isClassType(type)) {
         // Our root is the correct type.
         p = radio;
      }
      else {
         // See if our root owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// R/F sensor model access functions
//------------------------------------------------------------------------------

// Player's top level R/F sensor model
RfSensor* Player::getSensor()
{
   return (sensor != nullptr) ? (static_cast<RfSensor*>(sensor->object())) : nullptr;
}

// Name of the player's top level R/F sensor model
const RfSensor* Player::getSensor() const
{
   return (sensor != nullptr) ? (static_cast<RfSensor*>(sensor->object())) : nullptr;
}

// Name of the player's top level R/F sensor model
const base::Identifier* Player::getSensorName() const
{
   return (sensor != nullptr) ? sensor->slot() : nullptr;
}

// Returns a R/F sensor model by its name
RfSensor* Player::getSensorByName(const char* const name1)
{
   RfSensor* p = nullptr;
   if (sensor != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of our top level system
      base::Pair* pair = nullptr;
      if ( *getSensorName() == fname ) {
         // The first name matches our top level system name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getSensor()->findByName(name);
         }
         else {
            // It's a simple name and our top level system is a match.
            pair = sensor;
         }
      }

      // Our top level system isn't a match, so pass the whole name down
      else {
         pair = getSensor()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<RfSensor*>( pair->object() );
      }
   }
   return p;
}

// Returns a R/F sensor model by its type
base::Pair* Player::getSensorByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                // Our return value

   if (sensor != nullptr) {
      RfSensor* root = getSensor();   // Root node of the list
      if (root->isClassType(type)) {
         // Our root sensor is the correct type.
         p = sensor;
      }
      else {
         // See if our root sensor owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// IR sensor model access functions
//------------------------------------------------------------------------------

// Player's top level IR sensor model
IrSystem* Player::getIrSystem()
{
   return (irSystem != nullptr) ? (static_cast<IrSystem*>(irSystem->object())) : nullptr;
}

// Name of the player's top level IR sensor model
const IrSystem* Player::getIrSystem() const
{
   return (irSystem != nullptr) ? (static_cast<IrSystem*>(irSystem->object())) : nullptr;
}

// Name of the player's top level IR sensor model
const base::Identifier* Player::getIrSystemName() const
{
   return (irSystem != nullptr) ? irSystem->slot() : nullptr;
}

// Returns a IR sensor model by its name
IrSystem* Player::getIrSystemByName(const char* const name1)
{
   IrSystem* p = nullptr;
   if (irSystem != nullptr) {

      // Is this a complex (xxx.yyy name)?
      // To check, copy the first name up to a possible period.
      const char* name = name1;
      char fname[128];
      int i = 0;
      while (name[i] != '\0' && name[i] != '.') {
         fname[i] = name[i];
         i++;
      }
      fname[i] = '\0';

      // Now compare the first name with the name of our top level system
      base::Pair* pair = nullptr;
      if ( *getIrSystemName() == fname ) {
         // The first name matches our top level system name ...
         if (name[i] == '.') {
            // And this is a complex name (xxx.yyy), so pass the ".yyy" down
            pair = getIrSystem()->findByName(name);
         }
         else {
            // It's a simple name and our top level system is a match.
            pair = irSystem;
         }
      }

      // Our top level system isn't a match, so pass the whole name down
      else {
         pair = getIrSystem()->findByName(name1);
      }

      // Did we find a match?
      if (pair != nullptr) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<IrSystem*>( pair->object() );
      }
   }
   return p;
}

// Returns a IR sensor model by its type
base::Pair* Player::getIrSystemByType(const std::type_info& type)
{
   base::Pair* p = nullptr;                // Our return value

   if (irSystem != nullptr) {
      IrSystem* root = getIrSystem();   // Root node of the list
      if (root->isClassType(type)) {
         // Our root sensor is the correct type.
         p = irSystem;
      }
      else {
         // See if our root sensor owns a node of the correct type.
         p = root->findByType(type);
      }
   }

   return p;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets player's type string ("F-16A", "Tank", "SA-6", etc)
bool Player::setType(const base::String* const msg)
{
   if (msg != nullptr) {
      base::String* p = msg->clone();
      type.set(p, false);
   }
   else {
      type = nullptr;
   }
   return true;
}

// Set the player's name
void Player::setName(const base::Identifier& n)
{
   pname = n;
}

// Set the player's name
void Player::setName(const char* const str)
{
   pname = str;
}

// Sets the player's ID
void Player::setID(const unsigned short v)
{
   id = v;
}

// Sets the player's side (BLUE, RED, etc)
void Player::setSide(const Side s)
{
   side = s;
}

// Sets the player's mode (ACTIVE, DEAD, etc)
void Player::setMode(const Mode m)
{
   mode = m;
}

// Sets the player's initial (reset) mode
void Player::setInitMode(const Mode m)
{
   initMode = m;
}

// Sets the coord system to use for updating position
bool Player::setUseCoordSys(const CoordSys cs)
{
   useCoordSys = cs;
   return true;
}

// Sets the player's fuel flag
bool Player::setFuelFreeze(const bool f)
{
   fuelFrz = f;
   return true;
}

// Sets the player's crash override flag
bool Player::setCrashOverride(const bool f)
{
   crashOverride = f;
   return true;
}

// Sets the player's kill override flag
bool Player::setKillOverride(const bool f)
{
   killOverride = f;
   return true;
}

// Sets the player's kill removal flag
bool Player::setKillRemoval(const bool f)
{
   killRemoval = f;
   return true;
}

// Resets the just killed flag
void Player::resetJustKilled()
{
   justKilled = false;
}

// Sets the damage for this player
bool Player::setDamage(const double v)
{
   double x = v;
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   damage = x;
   return true;
}

// Sets the smoke for this player
bool Player::setSmoke(const double v)
{
   double x = v;
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   smoking = x;
   return true;
}

// Sets the flames for this player
bool Player::setFlames(const double v)
{
   double x = v;
   if (x < 0) x = 0.0;
   if (x > 1) x = 1.0;
   flames = x;
   return true;
}

// Sets the camouflage type for this player
bool Player::setCamouflageType(const unsigned int v)
{
   camouflage = v;
   return true;
}

// Sets the player's position freeze flag
bool Player::setPositionFreeze(const bool f)
{
   posFrz  = f;
   return true;
}

// Sets the player's altitude freeze flag
bool Player::setAltitudeFreeze(const bool f)
{
   altFrz = f;
   return true;
}

// Sets the player's attitude freeze flag
bool Player::setAttitudeFreeze(const bool f)
{
   attFrz = f;
   return true;
}

// Enable/Disable heading hold
bool Player::setHeadingHoldOn(const bool b)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setHeadingHoldOn(b);
   else
      return false;
}

// Sets the commanded (true) heading (radians)
bool Player::setCommandedHeading(const double h)
{
   return setCommandedHeadingD( h * base::Angle::R2DCC );
}

// Sets commanded (true) heading (true: degs)
bool Player::setCommandedHeadingD(const double h)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setCommandedHeadingD(h);
   else
      return false;
}

// Sets the commanded (true) heading (radians)
bool Player::setCommandedHeadingR(const double h)
{
   return setCommandedHeadingD( h * base::Angle::R2DCC );
}

// Enable/Disable velocity hold
bool Player::setVelocityHoldOn(const bool b)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setVelocityHoldOn(b);
   else
      return false;
}

// Sets the commanded (true) velocity (knots)
bool Player::setCommandedVelocityKts(const double a)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setCommandedVelocityKts(a);
   else
      return false;
}

// Enable/Disable altitude hold
bool Player::setAltitudeHoldOn(const bool b)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setAltitudeHoldOn(b);
   else
      return false;
}

// Sets commanded (HAE) altitude, default (meters)
bool Player::setCommandedAltitude(const double a)
{
   if (getDynamicsModel() != nullptr)
      return getDynamicsModel()->setCommandedAltitude(a);
   else
      return false;
}

// Sets commanded (HAE) altitude (meters)
bool Player::setCommandedAltitudeM(const double a)
{
   return setCommandedAltitude(a);
}

// Sets commanded (HAE) altitude (feet)
bool Player::setCommandedAltitudeFt(const double a)
{
   return setCommandedAltitude( a * base::Distance::FT2M );
}

// Sets a pointer to the Network Interface Block (NIB)
bool Player::setNib(Nib* const n)
{
   if (nib != nullptr) nib->unref();
   nib = n;
   if (nib != nullptr) {
      // Ref() the new NIB and get the network ID
      nib->ref();
      NetIO* netIO = nib->getNetIO();
      if (netIO != nullptr) netID = netIO->getNetworkID();
   }
   else {
      netID = 0;
   }
   return true;
}

// Sets the network output enabled flag
bool Player::setEnableNetOutput(const bool x)
{
   enableNetOutput = x;
   return true;
}

// Sets the outgoing NIB for network 'id'
bool Player::setOutgoingNib(Nib* const p, const unsigned int id)
{
   bool ok = false;
   if (nibList != nullptr && id >= 1 && id <= NetIO::MAX_NETWORD_ID) {
      unsigned int idx = id - 1;
      if (nibList[idx] != nullptr) nibList[idx]->unref();
      nibList[idx] = p;
      if (nibList[idx] != nullptr) nibList[idx]->ref();
   }
   return ok;
}

// Sets the elevation of the terrain at this player's location (meters)
void Player::setTerrainElevation(const double v)
{
   tElev = v;
   tElevValid = true;
}

// Sets the ground clamping offset (meters)
bool Player::setTerrainOffset(const double v)
{
   tOffset = v;
   return true;
}

// sets the DTED terrain interpolation flag
bool Player::setInterpolateTerrain(const bool x)
{
   interpTrrn = x;
   return true;
}

// Sets OTW terrain elevation request flag
bool Player::setTerrainElevationRequired(const bool b)
{
   tElevReq = b;
   return true;
}

// Set the player's altitude (m)
bool Player::setAltitude(const double alt, const bool slaved)
{
   bool saved = posSlaved;

   bool ok = false;
   if (useCoordSys == CS_LOCAL) {
      // Set altitude along with the local north and east gaming area coordinates
      ok = setPosition(posVecNED[INORTH], posVecNED[IEAST], -alt, slaved);
   }
   else if (useCoordSys == CS_GEOD || useCoordSys == CS_WORLD) {
      // Set altitude with the geodetic coordinates
      ok = setPositionLLA(latitude, longitude, alt, false);
   }

   if (ok) altSlaved = slaved;
   posSlaved = saved;

   return ok;
}

// Position relative to the simulation ref point (meters)
bool Player::setPosition(const double n, const double e, const bool slaved)
{
   bool saved = altSlaved;
   bool ok = setPosition(n, e, -getAltitudeM(), false);
   if (ok) {
      altSlaved = saved;
      posSlaved = slaved;
   }
   return ok;
}

// Position relative to the simulation ref point (meters)
bool Player::setPosition(const double n, const double e, const double d, const bool slaved)
{
   Simulation* s = getSimulation();
   const double maxRefRange = s->getMaxRefRange();
   const base::EarthModel* em = s->getEarthModel();

   // Set the position vector relative to sim ref pt
   posVecNED.set(n, e, d);

   // The position vector is valid if the gaming area range is unlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   // Compute & set the lat/lon/alt position
   const double refLat = s->getRefLatitude();
   const double refLon = s->getRefLongitude();
   const double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      const double sinRlat = s->getSinRefLat();
      base::Nav::convertPosVec2llE(refLat, refLon, sinRlat, cosRlat, posVecNED, &latitude, &longitude, &altitude, em);
   }
   else {
      base::Nav::convertPosVec2llS(refLat, refLon, cosRlat, posVecNED, &latitude, &longitude, &altitude);
   }

   // compute the world matrix
   base::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute & set the  geocentric position
   double lla[3] = { latitude, longitude, altitude };
   double ecef[3] = { 0, 0, 0 };
   base::Nav::convertGeod2Ecef(lla, ecef, em);
   posVecECEF.set( ecef[0], ecef[1], ecef[2] );

   altSlaved = slaved;
   posSlaved = slaved;

   return true;
}

// Position vector; NED from simulation ref point (meters)
bool Player::setPosition(const osg::Vec3d& pos, const bool slaved)
{
   return setPosition(pos[INORTH], pos[IEAST], pos[IDOWN], slaved);
}

// Sets present position using lat/long position; (degs)
bool Player::setPositionLL(const double lat, const double lon, const bool slaved)
{
   bool saved = altSlaved;
   bool ok = setPositionLLA(lat, lon, getAltitudeM(), false);
   if (ok) {
      altSlaved =  saved;
      posSlaved = slaved;
   }
   return ok;
}

// Sets present position using lat/long position; (degs) and altitude (m)
bool Player::setPositionLLA(const double lat, const double lon, const double alt, const bool slaved)
{
   Simulation* s = getSimulation();
   const double maxRefRange = s->getMaxRefRange();
   const base::EarthModel* em = s->getEarthModel();

   // Set the lat/lon position
   latitude = lat;
   longitude = lon;
   altitude = alt;

   // compute the world matrix
   base::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute and set the position vector relative to sim ref pt
   const double refLat = s->getRefLatitude();
   const double refLon = s->getRefLongitude();
   const double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      const double sinRlat = s->getSinRefLat();
      base::Nav::convertLL2PosVecE(refLat, refLon, sinRlat, cosRlat, lat, lon, alt, &posVecNED, em);
   }
   else {
      base::Nav::convertLL2PosVecS(refLat, refLon, cosRlat, lat, lon, alt, &posVecNED);
   }

   // The position vector is valid if the gaming area range is unlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   // Compute & set the geocentric position
   double lla[3] = { lat, lon, alt };
   double ecef[3] = { 0, 0, 0 };
   base::Nav::convertGeod2Ecef(lla, ecef, em);
   posVecECEF.set( ecef[0], ecef[1], ecef[2] );

   altSlaved = slaved;
   posSlaved = slaved;

   return true;
}


// Geocentric position vector (meters)
bool Player::setGeocPosition(const osg::Vec3d& pos, const bool slaved)
{
   Simulation* s = getSimulation();
   const double maxRefRange = s->getMaxRefRange();
   const base::EarthModel* em = s->getEarthModel();

   // Set the geocentric position
   posVecECEF = pos;

   // Compute & set the geodetic position
   double ecef[3] = { posVecECEF[0], posVecECEF[1], posVecECEF[2] };
   double lla[3] = { 0, 0, 0 };
   base::Nav::convertEcef2Geod(ecef, lla, em);
   latitude = lla[base::Nav::ILAT];
   longitude = lla[base::Nav::ILON];
   altitude = lla[base::Nav::IALT];

   // compute the world matrix
   base::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute and set the position vector relative to sim ref pt
   const double refLat = s->getRefLatitude();
   const double refLon = s->getRefLongitude();
   const double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      const double sinRlat = s->getSinRefLat();
      base::Nav::convertLL2PosVecE(refLat, refLon, sinRlat, cosRlat, latitude, longitude, altitude, &posVecNED, em);
   }
   else {
      base::Nav::convertLL2PosVecS(refLat, refLon, cosRlat, latitude, longitude, altitude, &posVecNED);
   }

   // The position vector is valid if the gaming area range is unlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   altSlaved = slaved;
   posSlaved = slaved;

   return true;
}


// Sets Euler angles: (rad) [ roll pitch yaw ]
bool Player::setEulerAngles(const double r, const double p, const double y)
{
   // Set angles
   angles.set(r,p,y);

   // Compute rotational matrix and the sin/cos values of the angles
   base::Nav::computeRotationalMatrix(r, p, y, &rm, &scPhi, &scTheta, &scPsi);

   // Set quaternions
   q.set(rm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   base::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   return true;
}

// Sets Euler angle vector: (rad) [ roll pitch yaw ]
bool Player::setEulerAngles(const osg::Vec3d& newAngles)
{
   return setEulerAngles(newAngles.x(), newAngles.y(), newAngles.z());
}

// Sets geocentric (body/ECEF) Euler angles: (radians) [ roll pitch yaw ]
bool Player::setGeocEulerAngles(const osg::Vec3d& newAngles)
{
   // Set the geocentric angles
   anglesW = newAngles;

   // Compute sin/cos values and directional cosine matrix
   base::Nav::computeRotationalMatrix(anglesW, &rmW2B, &scPhiW, &scThetaW, &scPsiW);

   // Transpose the world matrix
   osg::Matrixd wmT = wm;
   wmT.transpose();

   // Compute rotational matrix: body/NED directional cosines
   rm = rmW2B * wmT;

   // compute Geodetic orientation angles
   base::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // Set quaternions
   q.set(rm);

   return true;
}

// Sets the rotational matrix
bool Player::setRotMat(const osg::Matrixd& rr)
{
   // set the matrix
   rm = rr;

   // Set quaternions
   q.set(rm);

   // Compute the Euler angles and the sin/cos values of the angles
   base::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   base::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   return true;
}

// Sets the quaternion
bool Player::setQuaternions(const osg::Quat& newQ)
{
   // Set quaternions
   q = newQ;

   // Set the rotational matrix
   rm.makeRotate(q);

   // Compute the Euler angles and the sin/cos values of the angles
   base::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   base::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   return true;
}

// Sets the body angular velocities (radians/second)
bool Player::setAngularVelocities(const double pa, const double qa, const double ra)
{
   angularVel.set(pa,qa,ra);

   double dpsiW = 0;
   if (scThetaW[1] != 0.0) dpsiW = (ra*scPhiW[1] + qa*scPhiW[0])/scThetaW[1];
   double dthetaW = qa*scPhiW[1] - ra*scPhiW[0];
   double dphiW = pa + dpsiW*scThetaW[0];
   gcAngVel.set(dphiW, dthetaW, dpsiW);

   return true;
}

// Sets the body angular velocity vector (radians/second)
bool Player::setAngularVelocities(const osg::Vec3d& newAngVel)
{
   return setAngularVelocities(newAngVel[0],newAngVel[1],newAngVel[2]);
}

// Sets the body angular velocities (radians/second)
bool Player::setGeocAngularVelocities(const osg::Vec3d& newAngVel)
{
   gcAngVel = newAngVel;

   double pw = gcAngVel[0];
   double qw = gcAngVel[1];
   double rw = gcAngVel[2];

   double pa = pw - scThetaW[0] * rw;
   double qa = scPhiW[1] * qw + scThetaW[1] * scPhiW[0] * rw;
   double ra = -scPhiW[0] * qw + scThetaW[1] * scPhiW[1] * rw;

   angularVel.set(pa,qa,ra);

   return true;
}

// Sets local NED velocities; (m/s) [ ue -> north(+), ve -> east(+), we -> down(+) ]
bool Player::setVelocity(const double ue, const double ve, const double we)
{
   velVecNED.set(ue,ve,we);      // set local NED velocity vectors
   velVecECEF = velVecNED * wm;   // compute geocentric velocity vector
   velVecBody = rm * velVecNED;  // compute body velocity vector

   // Compute other velocities
   vp = std::sqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = std::sqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = std::atan2(ve,ue);            // Ground track

   return true;
}

// Sets local NED velocity vector; (meters/sec) NED
bool Player::setVelocity(const osg::Vec3& newVel)
{
   setVelocity(newVel.x(), newVel.y(), newVel.z());
   return true;
}

// Sets local NED acceleration vector; (m/s/s) NED
bool Player::setAcceleration(const double due, const double dve, const double dwe)
{
   accelVecNED.set(due, dve, dwe);
   accelVecECEF = accelVecNED * wm;
   accelVecBody = rm * accelVecNED;
   return true;
}

bool Player::setAcceleration(const osg::Vec3& newAccel)
{
   setAcceleration(newAccel.x(), newAccel.y(), newAccel.z());
   return true;
}

// Sets body velocities: (m/s) [ ua -> fwd(+), va -> right(+), wa -> down(+) ]
bool Player::setVelocityBody(const double ua, const double va, const double wa)
{
   velVecBody.set(ua,va,wa);
   velVecNED = velVecBody * rm;  // compute local NED velocity vector
   velVecECEF = velVecNED * wm;   // compute geocentric velocity vector

   // Compute other velocities
   const double ue = static_cast<double>(velVecNED[INORTH]);
   const double ve = static_cast<double>(velVecNED[IEAST]);
   const double we = static_cast<double>(velVecNED[IDOWN]);
   vp = std::sqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = std::sqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = std::atan2(ve,ue);            // Ground track
   return true;
}

bool Player::setVelocityBody(const osg::Vec3& newVelBody)
{
   setVelocityBody(newVelBody.x(), newVelBody.y(), newVelBody.z());
   return true;
}


// Sets body acceleration vector; (meters/second / second) NED
bool Player::setAccelerationBody(const double dua, const double dva, const double dwa)
{
   accelVecBody.set(dua,dva,dwa);
   accelVecNED = accelVecBody * rm;  // compute local NED acceleration vector
   accelVecECEF = accelVecNED * wm;   // compute geocentric acceleration vector
   return true;
}

bool Player::setAccelerationBody(const osg::Vec3& newAccelBody)
{
   setAccelerationBody(newAccelBody.x(), newAccelBody.y(), newAccelBody.z());
   return true;
}

// Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
bool Player::setGeocVelocity(const double vx, const double vy, const double vz)
{
   velVecECEF.set(vx,vy,vz);
   velVecNED = wm * velVecECEF;
   velVecBody = rm * velVecNED;

   // Compute other velocities
   const double ue = static_cast<double>(velVecNED[INORTH]);
   const double ve = static_cast<double>(velVecNED[IEAST]);
   const double we = static_cast<double>(velVecNED[IDOWN]);
   vp = std::sqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = std::sqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = std::atan2(ve,ue);            // Ground track
   return true;
}

bool Player::setGeocVelocity(const osg::Vec3& newVelEcef)
{
   setGeocVelocity(newVelEcef.x(), newVelEcef.y(), newVelEcef.z());
   return true;
}

// Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
bool Player::setGeocAcceleration(const double dvx, const double dvy, const double dvz)
{
   accelVecECEF.set(dvx,dvy,dvz);
   accelVecNED = wm * accelVecECEF;
   accelVecBody = rm * accelVecNED;
   return true;
}

bool Player::setGeocAcceleration(const osg::Vec3& newAccelEcef)
{
   setGeocAcceleration(newAccelEcef.x(), newAccelEcef.y(), newAccelEcef.z());
   return true;
}


// Initial geocentric position vector
bool Player::setInitGeocentricPosition(const osg::Vec3d& pos)
{
   initGeoPosVec = pos;
   initGeoPosFlg = true;
   return true;
}

// Initial position (after reset) relative to the simulation reference point (meters)
bool Player::setInitPosition(const double x, const double y)
{
   initPosVec.set(x, y);
   initPosFlg = true;
   return true;
}

// Initial position vector (after reset); North/East from simulation reference point (meters)
bool Player::setInitPosition(const osg::Vec2d& pos)
{
   return setInitPosition(pos.x(), pos.y());
}

// Sets the player's initial (reset) latitude (degrees)
bool Player::setInitLat(const double x)
{
   bool ok = false;
   if (x >= -90.0 && x <= 90.0) {
      initLat = x;
      initLatLonFlg = true;
      ok = true;
   }
   return ok;
}

// Sets the player's initial (reset) longitude (degrees)
bool Player::setInitLon(const double x)
{
   bool ok = false;
   if (x >= -180.0 && x <= 180.0) {
      initLon = x;
      initLatLonFlg = true;
      ok = true;
   }
   return ok;
}

// Initial altitude (HAE) (meters)
bool Player::setInitAltitude(const double alt)
{
   initAlt = alt;
   return true;
}

//------------------------------------------------------------------------------
// setControlStickRollInput(Roll) --  Control inputs: normalized
//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void Player::setControlStickRollInput(const double value)
{
   if (getDynamicsModel() != nullptr) {
      getDynamicsModel()->setControlStickRollInput(value);
   }
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void Player::setControlStickPitchInput(const double value)
{
   if (getDynamicsModel() != nullptr) {
      getDynamicsModel()->setControlStickPitchInput(value);
   }
}

//------------------------------------------------------------------------------
// int setThrottles(positions,num) -- Set throttle positions
//
//    positions -> Array of throttle positions
//      (for each throttle)
//              < 0.0       -> Cutoff
//              0.0         -> Idle
//              1.0         -> MIL
//              2.0         -> A/B
//    num -> number of throttle positions to get/set
//    returns the actual number of throttle positions
//------------------------------------------------------------------------------
int Player::setThrottles(const double* const data, const int num)
{
   int n = 0;
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->setThrottles(data, num);
   }
   return n;
}

//------------------------------------------------------------------------------
// Process weapon detonation
//------------------------------------------------------------------------------
void Player::processDetonation(const double detRange, Weapon* const wpn)
{
   if (!isKillOverride()) {

      // Weapon, launcher & range info
      Player* launcher = nullptr;
      double rng = detRange;
      double blastRange  = 500.0;    // burst range (meters)
      double lethalRange =  50.0;    // lethal range  (meters)
      if (wpn != nullptr) {
         launcher = wpn->getLaunchVehicle();
         blastRange = wpn->getMaxBurstRng();
         lethalRange  = wpn->getLethalRange();
         if (this == wpn->getTargetPlayer()) {
            // If we're the target -- use the weapon's detonation range
            rng = wpn->getDetonationRange();
         }
      }

      // Very close?
      if (rng < lethalRange) {
         // and like horseshoes -- being close does matter
         event(KILL_EVENT, launcher);
      }

      // Near by?
      else if (rng <= blastRange) {
         // use distance to compute amount of damage
         double damageRng = blastRange - lethalRange;
         if (damageRng <= 1.0) damageRng = 1.0;
         double newDamage = 1.0 - ( (rng - lethalRange) / damageRng );
         setDamage(newDamage + getDamage());
         setFlames( getDamage() - 0.25 );
         setSmoke( getDamage() + 0.25 );
         if ( isDestroyed() ) {
            event(KILL_EVENT, launcher);
         }
      }

   }

   // record EVERYTHING that had the potential to cause damage, even if killOverride

   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_PLAYER_DAMAGED )
      SAMPLE_2_OBJECTS( this, wpn )
   END_RECORD_DATA_SAMPLE()

   if (getAnyEventLogger() != nullptr) {  // EventLogger Deprecated
      TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(4, this, wpn); // type 4: damage state
      getAnyEventLogger()->log(evt);
      evt->unref();
   }

}

//------------------------------------------------------------------------------
// killedNotification() -- We were just killed by a weapon from player 'p'
//------------------------------------------------------------------------------
bool Player::killedNotification(Player* const p)
{
   if (!isKillOverride()) {
      // When not in 'kill override' mode ...

      // Let all of our subcomponents know that we were just killed
      {
         base::PairStream* subcomponents = getComponents();
         if (subcomponents != nullptr) {
            for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
               base::Pair* pair = static_cast<base::Pair*>(item->getValue());
               base::Component* sc = static_cast<base::Component*>(pair->object());
               sc->event(KILL_EVENT, p);
            }
            subcomponents->unref();
            subcomponents = nullptr;
         }
      }

      setDamage(1.0);
      setSmoke(1.0);
      setFlames(1.0);

      // Set our status
      if (killRemoval && isLocalPlayer()) {

         justKilled = true;
         setMode(KILLED);

         if (p != nullptr) killedBy = p->getID();
         else killedBy = 0;
      }

   }

   // record kill, even if killOverride
   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_PLAYER_KILLED )
      SAMPLE_2_OBJECTS( this, p )
   END_RECORD_DATA_SAMPLE()

   // TabLogger is deprecated
   if (getAnyEventLogger() != nullptr) {  // EventLogger Deprecated
      TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(7, this, p); // type 7: kill
      getAnyEventLogger()->log(evt);
      evt->unref();
   }

   return true;
}

//------------------------------------------------------------------------------
// collisionNotification() -- We were just killed by a collision with player 'p'
//------------------------------------------------------------------------------
bool Player::collisionNotification(Player* const p)
{
   if (!isCrashOverride() && isLocalPlayer()) {
      // When not in 'crash override' mode ...

      // Let all of our subcomponents know that we were just killed
      {
         base::PairStream* subcomponents = getComponents();
         if (subcomponents != nullptr) {
            for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
               base::Pair* pair = static_cast<base::Pair*>(item->getValue());
               base::Component* sc = static_cast<base::Component*>(pair->object());
               sc->event(KILL_EVENT, p);
            }
            subcomponents->unref();
            subcomponents = nullptr;
         }
      }

      // Set our status
      justKilled = true;
      if (p != nullptr) killedBy = p->getID();
      else killedBy = 0;
      setMode(CRASHED);
   }

   // record EVERYTHING that had the potential to cause damage, even if crashOverride

   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_PLAYER_COLLISION )
      SAMPLE_2_OBJECTS( this, p )
   END_RECORD_DATA_SAMPLE()

   // TabLogger is deprecated
   if (getAnyEventLogger() != nullptr) {  // EventLogger Deprecated
      TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(5, this, p); // type 5: collision
      getAnyEventLogger()->log(evt);
      evt->unref();
   }

   return true;
}

//------------------------------------------------------------------------------
// crashNotification() -- We were just crashed into terrain
//------------------------------------------------------------------------------
bool Player::crashNotification()
{
   if (!isCrashOverride() && isLocalPlayer()) {
      // When not in 'crash override' mode ...

      // Let our sensors know we were just killed
      // Let all of our subcomponents know that we were just killed
      {
         base::PairStream* subcomponents = getComponents();
         if (subcomponents != nullptr) {
            for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
               base::Pair* pair = static_cast<base::Pair*>(item->getValue());
               base::Component* sc = static_cast<base::Component*>(pair->object());
               sc->event(KILL_EVENT);
            }
            subcomponents->unref();
            subcomponents = nullptr;
         }
      }

      // Set our status
      justKilled = true;
      killedBy = 0;
      setMode(CRASHED);
   }

   // record EVERYTHING that had the potential to cause damage, even if crashOverride

   BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_PLAYER_CRASH )
      SAMPLE_1_OBJECT( this )
   END_RECORD_DATA_SAMPLE()

   // TabLogger is deprecated
   if (getAnyEventLogger() != nullptr) {  // EventLogger Deprecated
      TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(6, this); // type 6: crash
      getAnyEventLogger()->log(evt);
      evt->unref();
   }

   return true;
}

//------------------------------------------------------------------------------
// onWpnRelEvent() -- Manage the Wpn Release event
//------------------------------------------------------------------------------
bool Player::onWpnRelEvent(const base::Boolean* const sw)
{
   bool used = false;

   StoresMgr* p = getStoresManagement();
   if (p != nullptr) {
      // When we have an SMS, let it handle this event ...
      used = p->onWpnRelEvent(sw);
   }
   else {
      // We don't have an SMS, we can do some TBD default handling of this event.
      if (isMessageEnabled(MSG_WARNING)) {
         std::cout << "Player::onWpnRelEvent() No SMS!" << std::endl;
      }
      used = true;
   }

   return used;
}

//------------------------------------------------------------------------------
// onTriggerSwEvent() -- Manage the trigger switch event
//------------------------------------------------------------------------------
bool Player::onTriggerSwEvent(const base::Boolean* const sw)
{
   bool used = false;

   if (sms != nullptr) {
      // When we have an SMS, let it handle this event ...
      used = getStoresManagement()->onTriggerSwEvent(sw);
   }
   else {
      // We don't have an SMS, we can do some TBD default handling of this event.
      used = true;
   }

   return used;
}

//------------------------------------------------------------------------------
// onTgtStepEvent() -- Manage the target step (reject) event
//------------------------------------------------------------------------------
bool Player::onTgtStepEvent()
{
   if (obc != nullptr) {
      getOnboardComputer()->updateShootList(true);
   }
   return true;
}


//------------------------------------------------------------------------------
// onRfEmissionEventPlayer() -- process RF Emission events
//
// 1) compute the Line-Of-Sight (LOS) vectors back to the transmitter
//
// 2) Transform the LOS vector to our player coordinates to get the Angle
//    Of Incidence (AOI) vector
//
// 3) Compute the azimuth and elevation angles of incidence (AOI)
//
// 4) Compute the RCS value
//
// 5) Send the reflected emission back to transmitter
//
// 6) Pass the emission to our antennas and unref() the emission
//
// 7) Pass the emission to anyone requesting reflected emissions
//------------------------------------------------------------------------------
bool Player::onRfEmissionEventPlayer(Emission* const em)
{
   // Player must be active ...
   if (isNotMode(ACTIVE)) return false;

   // ---
   //  1) Compute the Line-Of-Sight vectors back to the transmitter (los0)
   // ---
   osg::Vec3 xlos = em->getTgtLosVec();
   osg::Vec4 los0( xlos.x(), xlos.y(), xlos.z(), 0.0 );

   // 2) Transform the LOS vector to our player coordinates to get
   // the Angle Of Incidence (AOI) vector
   osg::Vec4 aoi = rm * los0;
   em->setAoiVector(aoi);

   // 3) Compute the azimuth and elevation angles of incidence (AOI)
   {
      // 3-a) Get the aoi vector values & compute range squared
      const double xa = aoi.x();
      const double ya = aoi.y();
      const double za = -aoi.z();

      // 3-b) Compute azimuth: az = atan2(ya, xa)
      double aazr = std::atan2(ya, xa);
      em->setAzimuthAoi(aazr);

      // 3-c) Compute elevation: el = atan2(za, ra), where 'ra' is sqrt of xa*xa & ya*ya
      double ra = std::sqrt(xa*xa + ya*ya);
      double aelr = std::atan2(za,ra);
      em->setElevationAoi(aelr);
   }

   // 4) Compute and return the RCS
   if (em->isReturnRequested()) {

      if (signature != nullptr) {
         double rcs = signature->getRCS(em);
         em->setRCS(rcs);
      }
      else em->setRCS(0);

      // Send reflected emissions back to the transmitter
      em->getGimbal()->event(RF_EMISSION_RETURN,em);
   }

   // 6) Pass the emission to our antennas
   {
      Gimbal* g = getGimbal();
      if (g != nullptr && g->getPowerSwitch() != System::PWR_OFF) {
         g->event(RF_EMISSION,em);
      }
   }

   // 7) Pass the emission to anyone requesting reflected emissions
   //    (we're doing do calculations here, this is only meaningful to
   //     the receiving player)
   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != nullptr) rfReflect[i]->event(RF_REFLECTED_EMISSION,em);
   }

   return true;
}

//------------------------------------------------------------------------------
// onRfReflectedEmissionEventPlayer() -- process reflected R/F Emission events
//
//------------------------------------------------------------------------------
bool Player::onRfReflectedEmissionEventPlayer(Emission* const)
{
   return true;
}


//------------------------------------------------------------------------------
// onReflectionsRequest() -- request reflected R/F emissions
//                           (must continue to request once per second)
//------------------------------------------------------------------------------
bool Player::onReflectionsRequest(base::Component* const p)
{
   bool ok = false;           // Did we succeed?
   unsigned int idx = 0;      // Empty slot index
   bool haveEmptySlot = false;

   // First see if this is a re-request ...
   //  (and look for an empty slot while we're at it)
   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS && !ok; i++) {
      if (rfReflect[i] == p) {
         // Old request -- reset the timer
         rfReflectTimer[i] = 1.1;
         ok = true;
      }
      else if (rfReflect[i] == nullptr) {
         idx = i;
         haveEmptySlot = true;
      }
   }

   // New request and we have an empty slot?
   if (!ok && haveEmptySlot) {
      p->ref();
      rfReflect[idx] = p;
      rfReflectTimer[idx] = 1.1;
      ok = true;
   }

   return ok;
}


//------------------------------------------------------------------------------
// onReflectionsCancel() -- cancel a request for reflected R/F emissions
//------------------------------------------------------------------------------
bool Player::onReflectionsCancel(const base::Component* const p)
{
   bool ok = false;        // Did we succeed?

   // Find player 'p' and clear its request
   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS && !ok; i++) {
      if (rfReflect[i] == p) {
         // Clear the request
         rfReflect[i]->unref();
         rfReflect[i] = nullptr;
         ok = true;
      }
   }

   return ok;
}


//------------------------------------------------------------------------------
// onIrMsgEventPlayer() -- process IR query messages
//
// 1) compute the Line-Of-Sight (LOS) vectors back to the seeker
//
// 2) Transform the LOS vector to our player coordinates to get the Angle
//    Of Incidence (AOI) vector
//
// 3) Compute the azimuth and elevation angles of incidence (AOI)
//
// 4) Compute the IR Signature value
//
// 5) Send the query response back to seeker
//------------------------------------------------------------------------------
bool Player::onIrMsgEventPlayer(IrQueryMsg* const msg)
{
   // Player must be active and have an IR signature ...
   if (isNotMode(ACTIVE) || irSignature == nullptr) {
      msg->clearIrSignature();
      return true;
   }

   // ---
   //  1) Compute the Line-Of-Sight vectors back to the seeker (los0)
   // ---
   osg::Vec3 xlos = msg->getTgtLosVec();
   osg::Vec4 los0( xlos.x(), xlos.y(), xlos.z(), 0.0 );

   // 2) Transform the LOS vector to our player coordinates to get
   // the Angle Of Incidence (AOI) vector
   osg::Vec4 aoi = rm * los0;
   msg->setAoiVector(aoi);

   // 3) Compute the azimuth and elevation angles of incidence (AOI)

   // 3-a) Get the aoi vector values & compute range squared
   const double xa = aoi.x();
   const double ya = aoi.y();
   const double za = -aoi.z();

   // 3-b) Compute azimuth: az = atan2(ya, xa)
   double aazr = std::atan2(ya, xa);
   msg->setAzimuthAoi(aazr);

   // 3-c) Compute elevation: el = atan2(za, ra), where 'ra' is sqrt of xa*xa & ya*ya
   double ra = std::sqrt(xa*xa + ya*ya);
   double aelr = std::atan2(za,ra);
   msg->setElevationAoi(aelr);

   // 4) Compute and return the IR Signature
   bool hasSignature = irSignature->getIrSignature(msg);

   // 5) If the target has a signature, send the query response back to seeker
   if (hasSignature) msg->getGimbal()->event(IR_QUERY_RETURN,msg);

   return true;
}

// onDatalinkMessageEventPlayer() -- process datalink message events
bool Player::onDatalinkMessageEventPlayer(base::Object* const msg)
{
   // Just pass it down to all of our datalink system
   if (getDatalink() != nullptr) {
      getDatalink()->event(DATALINK_MESSAGE,msg);
   }
   return true;
}

// Handles the DE_EMISSION event
bool Player::onDeEmissionEvent(base::Object* const)
{
   return false;
}

//------------------------------------------------------------------------------
// The player's dynamics
//------------------------------------------------------------------------------
void Player::dynamics(const double dt)
{
   // ---
   // Local player ...
   // ---
   if (isLocalPlayer()) {
      // Update the external dynamics model (if any)
      if (getDynamicsModel() != nullptr) {
         // If we have a dynamics model ...
         getDynamicsModel()->freeze( isFrozen() );
         getDynamicsModel()->dynamics(dt);
      }

      // Update our position
      positionUpdate(dt);

      if(getNib() != nullptr || true) {
         if(!syncState1Ready) {
            syncState1.setGeocPosition(getGeocPosition());
            syncState1.setGeocVelocity(getGeocVelocity());
            syncState1.setGeocAcceleration(getGeocAcceleration());
            syncState1.setGeocEulerAngles(getGeocEulerAngles());
            syncState1.setAngularVelocities(getAngularVelocities());
            syncState1.setTimeExec(getSimulation()->getExecTimeSec());
            syncState1.setTimeUtc(getSimulation()->getSysTimeOfDay());
            syncState1.setValid(true);
            syncState1Ready = true;
            syncState2Ready = false;
            //std::cout << "Set syncState1" << std::endl;
         } else {
            syncState2.setGeocPosition(getGeocPosition());
            syncState2.setGeocVelocity(getGeocVelocity());
            syncState2.setGeocAcceleration(getGeocAcceleration());
            syncState2.setGeocEulerAngles(getGeocEulerAngles());
            syncState2.setAngularVelocities(getAngularVelocities());
            syncState2.setTimeExec(getSimulation()->getExecTimeSec());
            syncState2.setTimeUtc(getSimulation()->getSysTimeOfDay());
            syncState2.setValid(true);
            syncState2Ready = true;
            syncState1Ready = false;
            //std::cout << "Set syncState2" << std::endl;
         }
      }

      // ---
      // Check for ground collisions
      // ---
      if (getAltitudeAgl() < 0.0 && isLocalPlayer() && isMajorType(AIR_VEHICLE | WEAPON | SPACE_VEHICLE)) {
         // We're below the ground!
         this->event(CRASH_EVENT,nullptr);
      }
   }

   // ---
   // Network I-player ...
   // ---
   else {
      // dead reckoning our position and orientation
      deadReckonPosition(dt);
   }
}

//------------------------------------------------------------------------------
// Default update player position function (local players only)
//
// If the player can be ground clamped (ground vehicles) or if either the
// position freeze or the altitude freeze is set then we'll integrate and
// set our position using our geodetic coordinates and our NED velocity vector.
// Otherwise we'll integrate and set our position using ECEF coordinates and our
// ECEF velocity.
//------------------------------------------------------------------------------
void Player::positionUpdate(const double dt)
{
   if ( !isLocalPlayer() ) return;

   // (GEOD or LOCAL) Position frozen or slaved?
   bool pfrz = (isPositionFrozen() || isPositionSlaved());

   // (GEOD or LOCAL) Altitude frozen or slaved?
   bool afrz = (isAltitudeFrozen() || isAltitudeSlaved());

   // Ground clamping enabled?
   bool gcEnabled = tElevValid && isMajorType(GROUND_VEHICLE | SHIP | BUILDING | LIFE_FORM);

   // If switching systems then clear the previous velocity vector
   if (useCoordSys != useCoordSysN1) velVecN1.set(0,0,0);
   useCoordSysN1 = useCoordSys;

   // Position update enabled?
   //   Only if we have a velocity and a delta time
   //   and if either position or altitude is not frozen
   bool enabled = ( vp > 0 && dt != 0 && (!pfrz || !afrz) );
   if ( enabled ) {

      // ---
      // Update using local NED coordinates on the simulation's gaming area
      // ---
      if (useCoordSys == CS_LOCAL) {
         //std::cout << "Local: ";
         //std::cout << "( " << posVecNED[INORTH] << ", " << posVecNED[IEAST] << ", " << posVecNED[IDOWN] << " ); ";
         //std::cout << "( " << latitude << ", " << longitude << ", " << altitude << " ); ";
         //std::cout << "( " << posVecECEF[0] << ", " << posVecECEF[1] << ", " << posVecECEF[2] << " ); ";
         //std::cout << std::endl;

         osg::Vec3d  newPosVecNED = posVecNED;

         // Current velocities
         double ue = velVecNED.x();
         double ve = velVecNED.y();
         double we = velVecNED.z();

         // Previous velocities
         double ue0 = velVecN1.x();
         double ve0 = velVecN1.y();
         double we0 = velVecN1.z();

         if (!pfrz) {
            newPosVecNED[INORTH] += (ue + ue0) * 0.5 * dt;
            newPosVecNED[IEAST]  += (ve + ve0) * 0.5 * dt;
         }

         if (!afrz) {
            if (gcEnabled) {
               // Ground clamping enabled
               double alt = tElev + tOffset;
               newPosVecNED[IDOWN] = -alt;
            }
            else {
               // Update altitude
               newPosVecNED[IDOWN] += (we + we0) * 0.5 * dt;
            }
         }

         // Set the new position
         setPosition(newPosVecNED);

         // And save our old velocity vector
         velVecN1 = velVecNED;
      }

      // ---
      // Update using geodetic latitude, longitude and height above terrain
      // ---
      if (useCoordSys == CS_GEOD) {
         //std::cout << "Geod: ";
         //std::cout << "( " << posVecNED[INORTH] << ", " << posVecNED[IEAST] << ", " << posVecNED[IDOWN] << " ); ";
         //std::cout << "( " << latitude << ", " << longitude << ", " << altitude << " ); ";
         //std::cout << "( " << posVecECEF[0] << ", " << posVecECEF[1] << ", " << posVecECEF[2] << " ); ";
         //std::cout << std::endl;

         double newLatitude = latitude;
         double newLongitude = longitude;
         double newAltitude = altitude;

         // Current velocities
         double ue = velVecNED.x();
         double ve = velVecNED.y();
         double we = velVecNED.z();

         // Previous velocities
         double ue0 = velVecN1.x();
         double ve0 = velVecN1.y();
         double we0 = velVecN1.z();

         if (!pfrz) {

            double slat = std::sin(latitude * base::Angle::D2RCC);
            double clat = std::cos(latitude * base::Angle::D2RCC);

            const base::EarthModel* em = getSimulation()->getEarthModel();
            if (em == nullptr) em = &base::EarthModel::wgs84;

            const double a  = em->getA();   // semi-major axis (meters)
            const double e2 = em->getE2();  // eccentricity squared
            const double q  = 1.0 - e2 * slat * slat;
            double rn = a / std::sqrt(q);    // radius #1
            rn += getAltitude();             // radius #1 plus our altitude
            double rm = rn * (1.0 - e2) / q; // radius #2

            // Update latitude
            // ### May not support crossing the north or south pole correctly ###
            double dn = (ue + ue0) * 0.5 * dt;
            double dlatR = ( 1.0 / rm ) * dn;
            newLatitude += (dlatR * base::Angle::R2DCC);
            if (newLatitude >  90.0) {
               newLatitude = 180.0 - newLatitude;
               newLongitude = base::Angle::aepcdDeg( newLongitude + 180.0 );
            }
            if (newLatitude < -90.0) {
               newLatitude = -180.0 - newLatitude;
               newLongitude = base::Angle::aepcdDeg( newLongitude + 180.0 );
            }

            // Update longitude
            if (clat != 0) {
               double de = (ve + ve0) * 0.5 * dt;
               double dlonR = ( 1.0 / (rn * clat) ) * de;

               newLongitude += (dlonR * base::Angle::R2DCC);
               if (newLongitude >  180.0) newLongitude -= 360.0;
               if (newLongitude < -180.0) newLongitude += 360.0;
            }
         }

         if (!afrz) {
            if (gcEnabled) {
               // Ground clamping enabled
               double alt = tElev + tOffset;
               newAltitude = alt;
            }
            else {
               // Update altitude
               newAltitude -= (we + we0) * 0.5 * dt;
            }
         }

         // Set the new position
         setPositionLLA(newLatitude, newLongitude, newAltitude);

         // And save our old velocity vector
         velVecN1 = velVecNED;
      }

      // ---
      // Update using world (ECEF) coordinates
      // ---
      if (useCoordSys == CS_WORLD) {
            //std::cout << "World: ";
            //std::cout << "( " << posVecNED[INORTH] << ", " << posVecNED[IEAST] << ", " << posVecNED[IDOWN] << " ); ";
            //std::cout << "( " << latitude << ", " << longitude << ", " << altitude << " ); ";
            //std::cout << "( " << posVecECEF[0] << ", " << posVecECEF[1] << ", " << posVecECEF[2] << " ); ";
            //std::cout << std::endl;

         if (!pfrz) {
            // Update our position
            osg::Vec3d newPosVecECEF = posVecECEF + (velVecECEF + velVecN1) * 0.5 * dt;

            if (!gcEnabled) {
               // Set the our position
               setGeocPosition(newPosVecECEF);
            }

            else {
               // Ground clamping!

               // 1) Compute the ground clamped altitude
               double alt = tElev + tOffset;

               // 2) Compute the geodetic lat/lon position
               double ecef[3] = { newPosVecECEF[0], newPosVecECEF[1], newPosVecECEF[2] };
               double lla[3] = { 0, 0, 0 };

               const base::EarthModel* em = getSimulation()->getEarthModel();
               base::Nav::convertEcef2Geod(ecef, lla, em);

               // 3) Set position using these ground clamped coordinates
               setPositionLLA(lla[base::Nav::ILAT], lla[base::Nav::ILON], alt);
            }
         }

         // And save our old velocity vector
         velVecN1 = velVecECEF;
      }

   }

   // ---
   // Test only: update the Euler angles if we have non-zero test angular rates
   // ---
   if (testAngRates.length2() > 0 && !attFrz) {

      // Set body/earth rates
      double pa(0), qa(0), ra(0);
      double pe(0), qe(0), re(0);
      if (testBodyAxis) {
         // Body
         pa = testAngRates[0];
         qa = testAngRates[1];
         ra = testAngRates[2];
         // Earth
         re = 0;
         if (scTheta[1] != 0.0) re = (ra*scPhi[1] + qa*scPhi[0])/scTheta[1];
         qe = qa*scPhi[1] - ra*scPhi[0];
         pe = pa + re*scTheta[0];
      }
      else {
         // Earth
         pe = testAngRates[0];
         qe = testAngRates[1];
         re = testAngRates[2];
         // Body
         pa = pe - scTheta[0] * re;
         qa =  scPhi[1] * qe + scTheta[1] * scPhi[0] * re;
         ra = -scPhi[0] * qe + scTheta[1] * scPhi[1] * re;
      }

      // Update angles
      osg::Vec3d oldAngles = getEulerAngles();
      osg::Vec3d newAngles;
      newAngles[0] = base::Angle::aepcdRad(oldAngles[0] + (pe * dt));
      newAngles[1] = base::Angle::aepcdRad(oldAngles[1] + (qe * dt));
      newAngles[2] = base::Angle::aepcdRad(oldAngles[2] + (re * dt));
      if (newAngles[1] >  base::PI/2) newAngles[1] -= base::PI;
      if (newAngles[1] < -base::PI/2) newAngles[1] += base::PI;
      setEulerAngles(newAngles);

      // Reset the rates
      setAngularVelocities(pa, qa, ra);
      setVelocityBody(initVp, 0.0, 0.0);

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "r1[ " << getRollD() << ", " << getPitchD() << ", " << getHeadingD() << " ]";
         std::cout << " tr[ " << testAngRates[0]*base::Angle::R2DCC << ", " << testAngRates[1]*base::Angle::R2DCC << ", " << testAngRates[2]*base::Angle::R2DCC << " ]";
         std::cout << " er[ " << pe*base::Angle::R2DCC << ", " << qe*base::Angle::R2DCC << ", " << re*base::Angle::R2DCC << " ]";
         std::cout << " br[ " << pa*base::Angle::R2DCC << ", " << qa*base::Angle::R2DCC << ", " << ra*base::Angle::R2DCC << " ]";
         std::cout << " na[ " << newAngles[0]*base::Angle::R2DCC << ", " << newAngles[1]*base::Angle::R2DCC << ", " << newAngles[2]*base::Angle::R2DCC << " ]";
         std::cout << " oa[ " << oldAngles[0]*base::Angle::R2DCC << ", " << oldAngles[1]*base::Angle::R2DCC << ", " << oldAngles[2]*base::Angle::R2DCC << " ]";
         std::cout << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// Default player dead-reckoning function (networked I-players only)
//------------------------------------------------------------------------------
void Player::deadReckonPosition(const double dt)
{
   if ( !isNetworkedPlayer() ) return;

   if (getNib() != nullptr) {
      nib->ref();

      // Dead reckon our position and orientation
      osg::Vec3d drPos;
      osg::Vec3d drAngles;
      nib->updateDeadReckoning(dt, &drPos, &drAngles);
         //std::cout << "deadReckonPosition(): geoc pos(";
         //std::cout << drPos[0] << ", ";
         //std::cout << drPos[1] << ", ";
         //std::cout << drPos[2] << ") ";
         //std::cout << std::endl;

      // Ground clamping enabled?
      bool gcEnabled = tElevValid && isMajorType(GROUND_VEHICLE | SHIP | BUILDING | LIFE_FORM);

      if (!gcEnabled) {
         // Not ground clamping then set the DR position
         setGeocPosition( drPos );
      }

      else {
         // Ground clamping!

         // 1) Compute the ground clamped altitude
         double alt = tElev + tOffset;

         // 2) Compute the geodetic lat/lon position
         const base::EarthModel* em = getSimulation()->getEarthModel();
         double ecef[3] = { drPos[0], drPos[1], drPos[2] };
         double lla[3] = { 0, 0, 0 };
         base::Nav::convertEcef2Geod(ecef, lla, em);
         double lat = lla[base::Nav::ILAT];
         double lon = lla[base::Nav::ILON];

         // 3) Set position using these ground clamped coordinates
         setPositionLLA(lat, lon, alt);
      }

      // Set the DR orientation
      setGeocEulerAngles( drAngles );

      // Linear velocity and acceleration, as well as angular velocity, are set
      // using the initial (T=0) dead-reckoning values
      setGeocVelocity( nib->getDrVelocity() );
      setGeocAcceleration( nib->getDrAcceleration() );
      setGeocAngularVelocities( nib->getDrAngularVelocities() );

      nib->unref();
   }
}

//------------------------------------------------------------------------------
// updateSystemPointers() -- update all of our system (component) pointers
//------------------------------------------------------------------------------
void Player::updateSystemPointers()
{
   // ---
   // Set base::Pair pointers for our primary systems located in our list of subcomponents
   // ---
   loadSysPtrs = false;
   setDynamicsModel( findByType(typeid(DynamicsModel)) );
   setDatalink( findByType(typeid(Datalink)) );
   setGimbal( findByType(typeid(Gimbal)) );
   setIrSystem( findByType(typeid(IrSystem)) );
   setNavigation( findByType(typeid(Navigation)) );
   setOnboardComputer( findByType(typeid(OnboardComputer)) );
   setPilot( findByType(typeid(Pilot)) );
   setRadio( findByType(typeid(Radio)) );
   setSensor( findByType(typeid(RfSensor)) );
   setStoresMgr( findByType(typeid(StoresMgr)) );
}

//------------------------------------------------------------------------------
// processComponents() -- process our components; make sure the are all of
// type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Player::processComponents(
   base::PairStream* const list,
   const std::type_info& filter,
   base::Pair* const add,
   base::Component* const remove
   )
{
   // Set the 'load system pointers' flag if anything changes
   base::Component::processComponents(list,filter,add,remove);
   loadSysPtrs = true;
}

////------------------------------------------------------------------------------
//// computeEulerRates() -- compute the vehicle's angular velocities (earth)
////------------------------------------------------------------------------------
//bool Player::computeEulerRates()
//{
//   double pa = angularVel[0];
//   double qa = angularVel[1];
//   double ra = angularVel[2];
//
//   double dpsi = 0;
//   if (ctheta != 0) dpsi = (ra*cphi + qa*sphi)/ctheta;
//   double dtheta = qa*cphi - ra*sphi;
//   double dphi = pa + dpsi*stheta;
//
//   eulerRates.set(dphi, dtheta, dpsi);
//   return true;
//}

//------------------------------------------------------------------------------
// Get terrain elevation from the DTED database (if any)
//------------------------------------------------------------------------------
void Player::updateElevation()
{
   // Only if isTerrainElevationRequired() is false, otherwise the terrain
   // elevation is from the OTW system.
   const Simulation* s = getSimulation();
   if (s != nullptr && !isTerrainElevationRequired()) {
      const base::Terrain* terrain = s->getTerrain();
      if (terrain != nullptr) {
         double el = 0;
         terrain->getElevation(&el, getLatitude(), getLongitude(), isDtedTerrainInterpolationEnabled());
         setTerrainElevation(el);
      }
   }
}

//------------------------------------------------------------------------------
// printTimingStats() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Player::printTimingStats()
{
   const base::Statistic* ts = getTimingStats();
   std::cout << "Player(" << getSimulation()->cycle() << "," << getSimulation()->frame() << "," << getSimulation()->phase() << "): dt=" << ts->value() << ", ave=" << ts->mean() << ", max=" << ts->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// setDynamicsModel() -- Sets our dynamics models
//------------------------------------------------------------------------------
bool Player::setDynamicsModel(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (dyn != nullptr) dyn->unref();
      dyn = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(DynamicsModel)) ) {
      if (dyn != nullptr) dyn->unref();
      dyn = sys;
      dyn->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setDatalink() -- Sets our Datalink models
//------------------------------------------------------------------------------
bool Player::setDatalink(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (datalink != nullptr) datalink->unref();
      datalink = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Datalink)) ) {
      if (datalink != nullptr) datalink->unref();
      datalink = sys;
      datalink->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setGimbal() -- Sets our gimbal/antenna/optic models
//------------------------------------------------------------------------------
bool Player::setGimbal(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (gimbal != nullptr) gimbal->unref();
      gimbal = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Gimbal)) ) {
      if (gimbal != nullptr) gimbal->unref();
      gimbal = sys;
      gimbal->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setNavigation() -- Set our navigation system
//------------------------------------------------------------------------------
bool Player::setNavigation(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (nav != nullptr) nav->unref();
      nav = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Navigation)) ) {
      if (nav != nullptr) nav->unref();
      nav = sys;
      nav->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setOnboardComputer() -- Sets our onboard computer model
//------------------------------------------------------------------------------
bool Player::setOnboardComputer(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (obc != nullptr) obc->unref();
      obc = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(OnboardComputer)) ) {
      if (obc != nullptr) obc->unref();
      obc = sys;
      obc->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setPilot() -- Set our pilot model
//------------------------------------------------------------------------------
bool Player::setPilot(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (pilot != nullptr) pilot->unref();
      pilot = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Pilot)) ) {
      if (pilot != nullptr) pilot->unref();
      pilot = sys;
      pilot->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setRadio() -- Sets our radio models
//------------------------------------------------------------------------------
bool Player::setRadio(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (radio != nullptr) radio->unref();
      radio = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Radio)) ) {
      if (radio != nullptr) radio->unref();
      radio = sys;
      radio->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSensor() -- Sets our sensor models
//------------------------------------------------------------------------------
bool Player::setSensor(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (sensor != nullptr) sensor->unref();
      sensor = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(RfSensor)) ) {
      if (sensor != nullptr) sensor->unref();
      sensor = sys;
      sensor->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setIrSystem() -- Sets our IR sensor models
//------------------------------------------------------------------------------
bool Player::setIrSystem(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (irSystem != nullptr) irSystem->unref();
      irSystem = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(IrSystem)) ) {
      if (irSystem != nullptr) irSystem->unref();
      irSystem = sys;
      irSystem->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setStoresMgr() -- Set our stores management system
//------------------------------------------------------------------------------
bool Player::setStoresMgr(base::Pair* const sys)
{
   bool ok = false;
   if (sys == nullptr) {
      if (sms != nullptr) sms->unref();
      sms = nullptr;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(StoresMgr)) ) {
      if (sms != nullptr) sms->unref();
      sms = sys;
      sms->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// initXPos: X position (+north)
bool Player::setSlotInitXPos(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      osg::Vec2d pos = getInitPosition();
      pos[INORTH] = base::Meters::convertStatic(*msg);
      ok = setInitPosition(pos);
   }
   return ok;
}

// initXPos: X position (+north) (meters)
bool Player::setSlotInitXPos(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      osg::Vec2d pos = getInitPosition();
      pos[INORTH] = msg->getReal();
      ok = setInitPosition(pos);
   }
   return ok;
}

// initYPos: Y position (+east)
bool Player::setSlotInitYPos(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      osg::Vec2d pos = getInitPosition();
      pos[IEAST] = base::Meters::convertStatic(*msg);
      ok = setInitPosition(pos);
   }
   return ok;
}

// initYPos: Y position (+east)(meters)
bool Player::setSlotInitYPos(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      osg::Vec2d pos = getInitPosition();
      pos[IEAST] = msg->getReal();
      ok = setInitPosition(pos);
   }
   return ok;
}

// initAlt: Altitude (HAE @ sim ref pt) (+up)
bool Player::setSlotInitAlt(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Meters::convertStatic(*msg);
      setInitAltitude( value );
      ok = true;
   }
   return ok;
}

// initAlt: Altitude (HAE @ sim ref pt) (+up) (meters)
bool Player::setSlotInitAlt(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double value = msg->getReal();
      setInitAltitude( value );
      ok = true;
   }
   return ok;
}

// initPosition: Position Vector: meters [ north east down ]
bool Player::setSlotInitPosition(const base::List* const msg)
{
   bool ok = false;
   double values[3];
   const int n = msg->getNumberList(values, 3);
   if (n == 3) {
      setInitPosition(values[0], values[1]);
      setInitAltitude( -values[2] );
      ok = true;
   }
   return ok;
}

// initLatitude: Latitude
bool Player::setSlotInitLat(const base::LatLon* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = msg->getDouble();
      if (val >= -90.0 && val <= 90.0) {
         ok = setInitLat( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLatitude: Latitude
bool Player::setSlotInitLat(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = static_cast<double>(base::Degrees::convertStatic(*msg));
      if (val >= -90.0 && val <= 90.0) {
         ok = setInitLat( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLatitude: Latitude (degrees)
bool Player::setSlotInitLat(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = msg->getDouble();
      if (val >= -90.0 && val <= 90.0) {
         ok = setInitLat( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLongitude: Longitude
bool Player::setSlotInitLon(const base::LatLon* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = msg->getDouble();
      if (val >= -180.0 && val <= 180.0) {
         ok = setInitLon( val );
      }
      else {
         std::cerr << "Player::setSlotInitLon(): invalid longitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLongitude: Longitude
bool Player::setSlotInitLon(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = static_cast<double>(base::Degrees::convertStatic(*msg));
      if (val >= -180.0 && val <= 180.0) {
         ok = setInitLon( val );
      }
      else {
         std::cerr << "Player::setSlotInitLon(): invalid longitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLongitude: Longitude (degrees)
bool Player::setSlotInitLon(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double val = msg->getDouble();
      if (val >= -180.0 && val <= 180.0) {
         ok = setInitLon( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initGeocentric: Position vector [ x y z ] (meters)
bool Player::setSlotInitGeocentric(const base::List* const msg)
{
   bool ok = false;
   double values[3];
   const int n = msg->getNumberList(values, 3);
   if (n == 3) {
      osg::Vec3d pos(values[0], values[1], values[2]);
      ok = setInitGeocentricPosition(pos);
   }
   return ok;
}

// initRoll: Initial roll angle
bool Player::setSlotInitRoll(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Radians::convertStatic(*msg);
      if ( value >= -base::PI && value <= base::PI ) {
         initAngles[IROLL] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitRoll: invalid roll angle, valid range is [ -pi ... +pi]" << std::endl;
      }
   }

   return ok;
}

// initRoll: Initial roll angle (radians)
bool Player::setSlotInitRoll(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = msg->getDouble();
      if ( value >= -(2.0*base::PI) && value <= (2.0*base::PI) ) {
         initAngles[IROLL] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitRoll: invalid roll angle, valid range is [ -pi ... +pi]" << std::endl;
      }
   }

   return ok;
}

// initPitch: Initial pitch angle
bool Player::setSlotInitPitch(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Radians::convertStatic(*msg);
      if ( value >= -(base::PI/2.0) && value <= (base::PI/2.0) ) {
         initAngles[IPITCH] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitPitch: invalid pitch angle, valid range is [ -pi/2 ... +pi/2 ]" << std::endl;
      }
   }

   return ok;
}

// initPitch: Initial pitch angle (radians)
bool Player::setSlotInitPitch(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = msg->getDouble();
      if ( value >= -(base::PI/2.0) && value <= (base::PI/2.0) ) {
         initAngles[IPITCH] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitPitch: invalid pitch angle, valid range is [ -pi/2 ... +pi/2 ]" << std::endl;
      }
   }

   return ok;
}

// initHeading: Initial heading angle
bool Player::setSlotInitHeading(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double value = base::Radians::convertStatic(*msg);
      if ( value >= -base::PI && value <= (2.0*base::PI+0.001) ) {
         if (value >= 2.0*base::PI) value -= 2.0*base::PI;
         initAngles[IYAW] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitHeading: invalid heading angle, valid range is [ -pi ... +2*pi )" << std::endl;
      }
   }

   return ok;
}

// initHeading: Initial heading angle (radians)
bool Player::setSlotInitHeading(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      double value = msg->getDouble();
      if ( value >= -base::PI && value <= (2.0*base::PI+0.001) ) {
         if (value >= 2.0*base::PI) value -= 2.0*base::PI;
         initAngles[IYAW] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitHeading: invalid heading angle, valid range is [ -pi ... +2*pi ]" << std::endl;
      }
   }

   return ok;
}

// initEuler: Initial Euler Angles: radians [ roll pitch yaw ]
bool Player::setSlotInitEulerAngles(const base::List* const numList)
{
   bool ok = false;
   double values[3];
   const int n = numList->getNumberList(values, 3);
   if (n == 3) {
      if ( ( values[0] >= -(2.0f*base::PI) && values[0] <= (2.0f*base::PI) ) &&
         ( values[1] >= -base::PI && values[1] <= base::PI ) &&
         ( values[2] >= -(2.0f*base::PI) && values[2] <= (2.0f*base::PI) ) ) {

            initAngles.set(values[0], values[1], values[2]);
            ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitEulerAngles: invalid angle;";
         std::cerr << " valid roll & yaw are [-2*pi ... +2*pi], and valid pitch is [-pi ... +pi]";
         std::cerr << std::endl;
      }
   }
   return ok;
}

// testRollRate: Test roll rate
bool Player::setSlotTestRollRate(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Radians::convertStatic(*msg);
      testAngRates[IROLL] = value;
      ok = true;
   }

   return ok;
}

// testPitchRate: Test pitch rate
bool Player::setSlotTestPitchRate(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Radians::convertStatic(*msg);
      testAngRates[IPITCH] = value;
      ok = true;
   }

   return ok;
}

// testHeadingRate: Test heading rate
bool Player::setSlotTestYawRate(const base::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const double value = base::Radians::convertStatic(*msg);
      testAngRates[IYAW] = value;
      ok = true;
   }

   return ok;
}

// testBodyAxis: Test rates are in the body axis else they're Euler rates (default: false)
bool Player::setSlotTestBodyAxis(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      testBodyAxis = msg->getBoolean();
      ok = true;
   }
   return ok;
}


// initVelocity: Initial Velocity: meters/second
bool Player::setSlotInitVelocity(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      initVp = msg->getReal();
      ok = true;
   }
   return ok;
}

// initVelocityKts: Initial Velocity: knots (NM/hour)
bool Player::setSlotInitVelocityKts(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      initVp = (msg->getReal() * oe::base::Distance::NM2M) / 3600.0f;
      ok = true;
   }
   return ok;
}


// id: Player id  [ 1 .. 65535 ]
bool Player::setSlotID(const base::Number* const num)
{
   bool ok = false;
   int newID = num->getInt();
   if (newID > 0 && newID <= 65535) {
      setID( static_cast<unsigned short>(newID)  );
      ok = true;
   }
   else {
      std::cerr << "Player::setSlotID(): Invalid ID number: range 1 .. 65535" << std::endl;
   }
   return ok;
}

// side: Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE }
bool Player::setSlotSide(base::String* const msg)
{
   bool ok = false;
   if (*msg == "blue" || *msg == "BLUE") { setSide(BLUE); ok = true; }
   else if (*msg == "red" || *msg == "RED") { setSide(RED); ok = true; }
   else if (*msg == "gray" || *msg == "GRAY") { setSide(GRAY); ok = true; }
   else if (*msg == "white" || *msg == "WHITE") { setSide(WHITE); ok = true; }
   else if (*msg == "yellow" || *msg == "YELLOW") { setSide(YELLOW); ok = true; }
   else if (*msg == "cyan" || *msg == "CYAN") { setSide(CYAN); ok = true; }
   return ok;
}

// mode: Initial player mode ( INACTIVE, ACTIVE, DEAD )
bool Player::setSlotInitMode(base::String* const msg)
{
   bool ok = false;
   if (*msg == "inactive" || *msg == "INACTIVE") { setInitMode(INACTIVE); ok = true; }
   else if (*msg == "active" || *msg == "ACTIVE") { setInitMode(ACTIVE); ok = true; }
   else if (*msg == "killed" || *msg == "KILLED") { setInitMode(KILLED); ok = true; }
   else if (*msg == "crashed" || *msg == "CRASHED") { setInitMode(CRASHED); ok = true; }
   else if (*msg == "detonated" || *msg == "DETONATED") { setInitMode(DETONATED); ok = true; }
   else if (*msg == "launched" || *msg == "LAUNCHED") { setInitMode(LAUNCHED); ok = true; }
   return ok;
}

// useCoordSys: Coord system to use for updating player position
bool Player::setSlotUseCoordSys(base::String* const msg)
{
   bool ok = false;
   if (*msg == "local" || *msg == "LOCAL") { setUseCoordSys(CS_LOCAL); ok = true; }
   else if (*msg == "geod" || *msg == "GEOD") { setUseCoordSys(CS_GEOD); ok = true; }
   else if (*msg == "world" || *msg == "WORLD") { setUseCoordSys(CS_WORLD); ok = true; }
   return ok;
}

// signature: Player's RCS signature
bool Player::setSlotSignature(RfSignature* const s)
{
   if (signature != nullptr) {
      signature->container(nullptr);
   }
   signature = s;
   if (signature != nullptr) {
      signature->container(this);
   }
   return true;
}

// irSignature: Player's IR signature
bool Player::setSlotIrSignature(IrSignature* const s)
{
   irSignature = s;
   return true;
}

// camouflageType: User defined camouflage type (positive integer or zero for none)
bool Player::setSlotCamouflageType(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      int ii = msg->getInt();
      if (ii >= 0) {
         ok = setCamouflageType( ii );
      }
   }
   return ok;
}

// terrainElevReq: Terrain elevation from the OTW system is requested; otherwise use DTED (default: false)
bool Player::setSlotTerrainElevReq(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setTerrainElevationRequired(num->getBoolean());
   }
   return ok;
}

// interpolateTerrain: Interpolate our DTED terrain elevation data (default: false)
bool Player::setSlotInterpolateTerrain(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInterpolateTerrain(msg->getBoolean());
   }
   return ok;
}

// terrainOffset: Ground clamp offset from terrain to player's CG (base::Distance)
bool Player::setSlotTerrainOffset(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setTerrainOffset( base::Meters::convertStatic( *msg ) );
   }
   return ok;
}

// positionFreeze: Position freeze (default: false)
bool Player::setSlotPositionFreeze(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setPositionFreeze( num->getBoolean() );
   }
   return ok;
}

// altitudeFreeze: Altitude freeze (default: false)
bool Player::setSlotAltitudeFreeze(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setAltitudeFreeze( num->getBoolean() );
   }
   return ok;
}

// attitudeFreeze: Attitude freeze (default: false)
bool Player::setSlotAttitudeFreeze(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setAttitudeFreeze( num->getBoolean() );
   }
   return ok;
}

// fuelFreeze: Fuel freeze (default: false)
bool Player::setSlotFuelFreeze(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setFuelFreeze( num->getBoolean() );
   }
   return ok;
}

// crashOverride: Crash Override (i.e., ignore collision and crash events)(default: false)
bool Player::setSlotCrashOverride(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setCrashOverride( num->getBoolean() );
   }
   return ok;
}

// killOverride: Kill/Damage Override -- player can not be killed/damaged (default: false)
bool Player::setSlotKillOverride(const base::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setKillOverride( num->getBoolean() );
   }
   return ok;
}

// killRemoval: If true destroyed players are set to KILLED and are eventually removed (default: false)
bool Player::setSlotKillRemoval(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setKillRemoval( msg->getBoolean() );
   }
   return ok;
}

// enableNetOutput: Enable network output of this player (default: true)
bool Player::setSlotEnableNetOutput(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setEnableNetOutput(msg->getBoolean());
   }
   return ok;
}

// dataLogTime: Time between player data samples to an optional data
bool Player::setSlotDataLogTime(const base::Time* const num)
{
   bool ok = false;
   if (num != nullptr) {
      dataLogTime = num->getReal();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Player::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Player::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << std::endl;
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   if ( isInitGeocentricPositionValid() ) {
      // initial geocentric position
      const osg::Vec3d& vec = getInitGeocentricPosition();
      indent(sout,i+j);
      sout << "initGeocentric: [ ";
      sout << vec.x();
      sout << " ";
      sout << vec.y();
      sout << " ";
      sout << vec.z();
      sout << " ]" << std::endl;
   }

   else if ( isInitLatLonValid() ) {
      // initial lat, lon and alt position
      {
         double v = getInitLatitude();
         indent(sout,i+j);
         sout << "initLatitude: ( LatLon";

         sout << " direction: ";
         if (v >= 0) { sout << "n"; }
         else { sout << "s"; v = -v; }

         sout << " degrees: " << v;
         sout << " )" << std::endl;
      }
      {
         double v = getInitLongitude();

         indent(sout,i+j);
         sout << "initLongitude: ( LatLon";

         sout << " direction: ";
         if (v >= 0) { sout << "e"; }
         else { sout << "w"; v = -v; }

         sout << " degrees: " << v;
         sout << " )" << std::endl;
      }
      {
         indent(sout, i+j);
         sout << "initAlt: ";
         sout << getInitAltitude() << std::endl;
      }
   }

   else if ( isInitPositionValid() ) {
      // initial position vector from sim ref pt
      const osg::Vec2d& vec = getInitPosition();

      indent(sout,i+j);
      sout << "initXPos: ( Meters " << vec.x() << " )" << std::endl;

      indent(sout,i+j);
      sout << "initYPos: ( Meters " << vec.y() << " )" << std::endl;

      indent(sout, i+j);
      sout << "initAlt: ( Feet " << (getInitAltitude() * base::Distance::M2FT) << " )" << std::endl;
   }

   indent(sout,i+j);
   sout << "initRoll: ( Degrees " << (initAngles[IROLL] * base::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "initPitch: ( Degrees " << (initAngles[IPITCH] * base::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "initYawRate: ( Degrees " << (initAngles[IYAW] * base::Angle::R2DCC) << " )" << std::endl;


   // Velocity (ft/s)
   indent(sout,i+j);
   sout << "initVelocity: ";
   sout << initVp;
   sout << std::endl;

   //  Type of player vehicle ("F-16A", "Tank", "SA-6", etc.)
   if (type != nullptr) {
      indent(sout,i+j);
      sout << "type: " << *type << std::endl;
   }

   // player id
   indent(sout,i+j);
   sout << "id: " << getID() << std::endl;

   // Which side
   indent(sout,i+j);
   sout << "side: ";
   if (isSide(BLUE))
      sout << "BLUE";
   else if (isSide(RED))
      sout << "RED";
   else if (isSide(GRAY))
      sout << "GRAY";
   else if (isSide(WHITE))
      sout << "WHITE";
   else if (isSide(YELLOW))
      sout << "YELLOW";
   else if (isSide(CYAN))
      sout << "CYAN";
   else
      sout << "OTHER";
   sout << std::endl;

   // initial player mode
   indent(sout,i+j);
   sout << "mode: ";
   if (initMode == ACTIVE)
      sout << "ACTIVE";
   else if (initMode == KILLED)
      sout << "KILLED";
   else if (initMode == CRASHED)
      sout << "CRASHED";
   else if (initMode == DETONATED)
      sout << "DETONATED";
   else if (initMode == LAUNCHED)
      sout << "LAUNCHED";
   else
      sout << "INACTIVE";
   sout << std::endl;

   // initial player mode
   indent(sout,i+j);
   sout << "useCoordSys: ";
   if (useCoordSys == CS_WORLD)
      sout << "WORLD";
   else if (useCoordSys == CS_GEOD)
      sout << "GEOD";
   else
      sout << "LOCAL";
   sout << std::endl;

   // RCS Signature
   if (signature != nullptr) {
      indent(sout,i+j);
      sout << "signature: ";
      signature->serialize(sout,i+j+4);
   }

   // IR Signature
   if (irSignature != nullptr) {
      indent(sout, i + j);
      sout << "irSignature: ";
      irSignature->serialize(sout, i+j+4);
   }

   // User defined camouflage type
   indent(sout,i+j);
   sout << "camouflageType: " << getCamouflageType() << std::endl;

   // Terrain elevation request (default: false)
   if ( isTerrainElevationRequired() ) {
      indent(sout, i+j);
      sout << "terrainElevReq: true" << std::endl;
   }

   // Interpolate our DTED terrain elevation data (default: false)
   if ( isDtedTerrainInterpolationEnabled() ) {
      indent(sout, i+j);
      sout << "interpolateTerrain: true" << std::endl;
   }

   indent(sout, i+j);
   sout << "terrainOffset: ( Feet " << (getTerrainOffset() * base::Distance::M2FT) << " )" << std::endl;


   // Position Freeze (default: false)
   if (isPositionFrozen()) {
      indent(sout, i+j);
      sout << "positionFreeze: true" << std::endl;
   }

   // Altitude Freeze (default: false)
   if (isAltitudeFrozen()) {
      indent(sout, i+j);
      sout << "altitudeFreeze: true" << std::endl;
   }

   // Attitude Freeze (default: false)
   if (isAttitudeFrozen()) {
      indent(sout, i+j);
      sout << "attitudeFreeze: true" << std::endl;
   }

   // Fuel Freeze (default: false)
   if (isFuelFrozen()) {
      indent(sout, i+j);
      sout << "fuelFreeze: true" << std::endl;
   }

   // Crash Override (default: false)
   if (isCrashOverride()) {
      indent(sout, i+j);
      sout << "crashOverride: true" << std::endl;
   }

   // True if the player is in kill override mode (default: false)
   if (isKillOverride()) {
      indent(sout, i+j);
      sout << "killOverride: true" << std::endl;
   }

   // True if the player's kill removal flag is set (default: false)
   if (isKillRemovalEnabled()) {
      indent(sout, i+j);
      sout << "killRemoval: true" << std::endl;
   }

   // Enable network output of this player (default: true)
   if ( !isNetOutputEnabled() ) {
      indent(sout, i+j);
      sout << "enableNetOutput: false" << std::endl;
   }

   indent(sout,i+j);
   sout << "dataLogTime: ( Seconds " << dataLogTime << " )" << std::endl;

   indent(sout,i+j);
   sout << "testRollRate: ( Degrees " << (testAngRates[IROLL] * base::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "testPitchRate: ( Degrees " << (testAngRates[IPITCH] * base::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "testHeadingRate: ( Degrees " << (testAngRates[IYAW] * base::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);

   if (testBodyAxis) {
      indent(sout, i+j);
      sout << "testBodyAxis: true" << std::endl;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
