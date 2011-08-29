//==============================================================================
// class Player
//==============================================================================

#include "openeaagles/simulation/Player.h"

#include "openeaagles/simulation/Datalink.h"
#include "openeaagles/simulation/DynamicsModels.h"
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

#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Terrain.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Statistic.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Player,"Player")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Player)
   // Player's initial position relative to the simulation's reference point
   "initXPos",          //  1) Initial X position    (meters, Basic::Distance)
   "initYPos",          //  2) Initial Y position    (meters, Basic::Distance) 
   "initAlt",           //  3) Initial Altitude      (meters, Basic::Distance)
   "initPosition",      //  4) Initial Position Vector: meters [ north east down ]

   // Player's initial position in latitude, longitude and altitude (from initAlt above)
   "initLatitude",      //  5) Initial latitude    (Basic::Angle, Basic::LatLon or degrees) 
   "initLongitude",     //  6) Initial longitude    (Basic::Angle, Basic::LatLon or degrees)

   // Player's initial geocentric position
   "initGeocentric",    //  7) Initial geocentric position vector [ x y z ] (meters)

   // Player's initial euler angles
   "initRoll",          //  8) Initial roll:    (radians, Basic::Angle) (or use initEuler)
   "initPitch",         //  9) Initial pitch:   (radians, Basic::Angle) (or use initEuler)
   "initHeading",       // 10) Initial heading: (radians, Basic::Angle) (or use initEuler)
   "initEuler",         // 11) Initial euler Angles: radians [ roll pitch yaw ] (or use below)

   // Player's initial velocity
   "initVelocity",      // 12) Initial Velocity: meters/sec

   // Player's type, id and and other parameters
   "type",              // 13) Type of player vehicle ("F-16A", "Tank", "SA-6", etc.)
   "id",                // 14) Player id   [ 1 .. 65535 ]
   "side",              // 15) Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE }
   "mode",              // 16) Initial player mode ( INACTIVE, ACTIVE, DEAD )

   "signature",         // 17) Player's RCS signature
   "irSignature",       // 18) Player's IR Signature
   "camouflageType",    // 19) User defined camouflage type (positive integer or zero for none)

   "terrainElevReq",    // 20) Terrain elevation update requested
   "interpolateTerrain", // 21) Interpolate our terrain1 data
   "terrainOffset",     // 22) Ground clamp offset from terrain to player's CG (Basic::Distance)

   "positionFreeze",    // 23) Position freeze
   "altitudeFreeze",    // 24) Altitude freeze
   "attitudeFreeze",    // 25) Attitude freeze
   "fuelFreeze",        // 26) Fuel freeze
   "crashOverride",     // 27) Crash Override (i.e., ignore collision and crash events)
   "killOverride",      // 28) Kill/Damage Override -- player can not be killed or damaged(by a weapon)
   "killRemoval",       // 29) If true destroyed players are set to KILLED and are eventually removed (default: false)
   "enableNetOutput",   // 30) Enable the network output!

   "dataLogTime",       // 31) Data logging time -- time between player data samples 

   // Player's test angular velocities
   "testRollRate",      // 32) Test roll rate (units per second)
   "testPitchRate",     // 33) Test pitch rate (units per second)
   "testYawRate",       // 34) Test heading rate (units per second)
   "testBodyAxis",      // 35) Test rates are in body coordinates else euler rates (default: false)

   "useCoordSys"        // 36) Coord system to use for position updating { WORLD, GEOD, LOCAL }
END_SLOTTABLE(Player)

// Map slot table to handles 
BEGIN_SLOT_MAP(Player)

   ON_SLOT( 1, setSlotInitXPos, Basic::Distance)
   ON_SLOT( 1, setSlotInitXPos, Basic::Number)

   ON_SLOT( 2, setSlotInitYPos, Basic::Distance)
   ON_SLOT( 2, setSlotInitYPos, Basic::Number)

   ON_SLOT( 3, setSlotInitAlt,  Basic::Distance)
   ON_SLOT( 3, setSlotInitAlt,  Basic::Number)

   ON_SLOT( 4, setSlotInitPosition, Basic::List)

   ON_SLOT( 5, setSlotInitLat, Basic::LatLon)
   ON_SLOT( 5, setSlotInitLat, Basic::Angle)
   ON_SLOT( 5, setSlotInitLat, Basic::Number)

   ON_SLOT( 6, setSlotInitLon, Basic::LatLon)
   ON_SLOT( 6, setSlotInitLon, Basic::Angle)
   ON_SLOT( 6, setSlotInitLon, Basic::Number)

   ON_SLOT( 7, setSlotInitGeocentric, Basic::List)

   ON_SLOT( 8, setSlotInitRoll, Basic::Angle)
   ON_SLOT( 8, setSlotInitRoll, Basic::Number)

   ON_SLOT( 9, setSlotInitPitch, Basic::Angle)
   ON_SLOT( 9, setSlotInitPitch, Basic::Number)

   ON_SLOT(10, setSlotInitHeading, Basic::Angle)
   ON_SLOT(10, setSlotInitHeading, Basic::Number)

   ON_SLOT(11, setSlotInitEulerAngles, Basic::List)
   ON_SLOT(12, setSlotInitVelocity, Basic::Number)

   ON_SLOT(13, setType, Basic::String)
   ON_SLOT(14, setSlotID, Basic::Number)
   ON_SLOT(15, setSlotSide, Basic::String)
   ON_SLOT(16, setSlotInitMode, Basic::String)

   ON_SLOT(17, setSlotSignature, RfSignature)
   ON_SLOT(18, setSlotIrSignature, IrSignature)
   ON_SLOT(19, setSlotCamouflageType, Basic::Number)

   ON_SLOT(20, setSlotTerrainElevReq, Basic::Number)
   ON_SLOT(21, setSlotInterpolateTerrain, Basic::Number)
   ON_SLOT(22, setSlotTerrainOffset, Basic::Distance)

   ON_SLOT(23, setSlotPositionFreeze, Basic::Number)
   ON_SLOT(24, setSlotAltitudeFreeze, Basic::Number)
   ON_SLOT(25, setSlotAttitudeFreeze, Basic::Number)
   ON_SLOT(26, setSlotFuelFreeze, Basic::Number)
   ON_SLOT(27, setSlotCrashOverride, Basic::Number)
   ON_SLOT(28, setSlotKillOverride, Basic::Number)
   ON_SLOT(29, setSlotKillRemoval, Basic::Number)
   ON_SLOT(30, setSlotEnableNetOutput, Basic::Number)
   ON_SLOT(31, setSlotDataLogTime, Basic::Time)

   ON_SLOT(32, setSlotTestRollRate, Basic::Angle)
   ON_SLOT(33, setSlotTestPitchRate, Basic::Angle)
   ON_SLOT(34, setSlotTestYawRate, Basic::Angle)
   ON_SLOT(35, setSlotTestBodyAxis, Basic::Number)

   ON_SLOT(36, setSlotUseCoordSys, Basic::String)
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
   ON_EVENT_OBJ(RF_REFLECTIONS_REQUEST, onReflectionsRequest, Basic::Component)

   // We were just hit by a reflected R/F emission
   ON_EVENT_OBJ(RF_REFLECTED_EMISSION,onRfReflectedEmissionEventPlayer,Emission)

   // Another player is cancelling its request for reflected R/F emissions
   ON_EVENT_OBJ(RF_REFLECTIONS_CANCEL,  onReflectionsCancel,  Basic::Component)

   // Data link message event
   ON_EVENT_OBJ(DATALINK_MESSAGE,onDatalinkMessageEventPlayer,Basic::Object)

   // Weapon release button event (with switch state)
   ON_EVENT_OBJ(WPN_REL_EVENT,onWpnRelEvent,Basic::Boolean)

   // Weapon release button event
   ON_EVENT(WPN_REL_EVENT,onWpnRelEvent)

   // Trigger switch (with switch state)
   ON_EVENT_OBJ(TRIGGER_SW_EVENT,onTriggerSwEvent,Basic::Boolean)

   // Trigger event
   ON_EVENT(TRIGGER_SW_EVENT,onTriggerSwEvent)

   // Target step switch event
   ON_EVENT(TGT_STEP_EVENT,onTgtStepEvent)

   // An IR seeker is querying us for our IR signature
   ON_EVENT_OBJ(IR_QUERY,onIrMsgEventPlayer,IrQueryMsg)

   // We were just hit with a directed energy emission
   ON_EVENT_OBJ(DE_EMISSION,onDeEmissionEvent,Basic::Object)

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
   static Basic::String generic("GenericPlayer");
   type = 0;
   setType(&generic);

   id = 0;
   //pname = "";
   side = GRAY;

   mode = ACTIVE;
   latitude = 0;
   longitude = 0;
   altitude = 0;
   Basic::Nav::computeWorldMatrix(latitude, longitude, &wm);

   angles.set(0,0,0);
   Basic::Nav::computeRotationalMatrix(angles, &rm, &scPhi, &scTheta, &scPsi);

   rmW2B = rm * wm;
   Basic::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   q.set(rm);

   vp = 0;
   gndSpd = 0;
   gndTrk = 0;
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

   tElev    = 0.0f;
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
   tOffset = 0.0f;

   signature = 0;
   irSignature = 0;
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

   initAlt = 0;
   initVp = 0;
   initMode = mode;
   initAngles.set(0,0,0);
   testAngRates.set(0,0,0);
   testBodyAxis = false;

   dataLogTimer = 0.0f;
   dataLogTime  = 0.0f;

   nib = 0;
   netID = 0;
   enableNetOutput = true;
   nibList = new Nib*[NetIO::MAX_NETWORD_ID];
   for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
      nibList[i] = 0;
   }

   sim = 0;
   dyn = 0;
   datalink = 0;
   gimbal = 0;
   nav = 0;
   obc = 0;
   pilot = 0;
   radio = 0;
   sensor = 0;
   irSystem = 0;
   sms = 0;
   loadSysPtrs = true;

   for (int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      rfReflect[i] = 0;
      rfReflectTimer[i] = 0;
   }

}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Player::copyData(const Player& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   const Basic::String* tt = org.type;
   setType( (Basic::String*)tt );

   id = org.id;

   if (org.pname != 0) {
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

   if (org.signature != 0) setSlotSignature( dynamic_cast<RfSignature*>( org.signature->clone() ) );
   else setSlotSignature(0);

   if (org.irSignature != 0) setSlotIrSignature( dynamic_cast<IrSignature*>( org.irSignature->clone() ) );
   else setSlotIrSignature(0);

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
   setNib( 0 );
   for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
      setOutgoingNib(0, i);
   }

   // The following are not copied ..
   sim = 0;
   setDynamicsModel(0);
   setDatalink(0);
   setGimbal(0);
   setIrSystem(0);
   setNavigation(0);    
   setOnboardComputer(0);
   setPilot(0);
   setRadio(0); 
   setSensor(0);
   setStoresMgr(0);
   loadSysPtrs = true;

   // Reflected emission requests are not copied
   for (int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != 0) { rfReflect[i]->unref(); rfReflect[i] = 0; }
      rfReflectTimer[i] = 0;
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Player::deleteData()
{
   type = 0;
   signature = 0;
   irSignature = 0;
   sim = 0;

   setNib(0);
   if (nibList != 0) {
      for (unsigned int i = 0; i < NetIO::MAX_NETWORD_ID; i++) {
         setOutgoingNib(0, i);
      }
      delete[] nibList;
      nibList = 0;
   }
   

   setDynamicsModel(0);
   setDatalink(0);
   setGimbal(0);
   setIrSystem(0);
   setNavigation(0);    
   setOnboardComputer(0);
   setPilot(0);
   setRadio(0); 
   setSensor(0);
   setStoresMgr(0);

   for (int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != 0) { rfReflect[i]->unref(); rfReflect[i] = 0; }
   }
}

//------------------------------------------------------------------------------
// isFrozen() -- checks both player's freeze flag and the simulation's freeze flag
//------------------------------------------------------------------------------
bool Player::isFrozen() const
{
   bool frz = BaseClass::isFrozen();
   if (!frz && sim != 0) frz = sim->isFrozen();
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
   // Reset euler angles and rates
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
   setDamage(0.0f);
   setSmoke(0.0f);
   setFlames(0.0f);
   justKilled = false;
   killedBy = 0;

   altSlaved = false;
   posSlaved = false;

   tElev    = 0.0f;
   tElevValid = false;


   // ---
   // Reset our NIB
   // ---
   if (nib != 0) {
      nib->event(RESET_EVENT);
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
void Player::updateTC(const LCreal dt0)
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
      for (int i = 0; i < MAX_RF_REFLECTIONS; i++) {
         if (rfReflect[i] != 0) {
            rfReflectTimer[i] -= dt0;
            if (rfReflectTimer[i] <= 0) {
               // Clear the request
               rfReflect[i]->unref();
               rfReflect[i] = 0;
            }
         }
      }

      // ---
      // Delta time -- real or frozen?
      // ---
      LCreal dt = dt0;
      if (isFrozen()) dt = 0.0;

      // ---
      // Compute delta time for modules running every fourth phase
      // ---
      LCreal dt4 = dt * 4.0f; // Delta time for items running every fourth phase
      switch (getSimulation()->phase()) {

         // Phase 0 -- Dynamics
         case 0 : {
            // Our dynamics
            dynamics(dt4);

            // Log our player's dynamic data just after its been updated ...
            if (dataLogTime > 0.0) {
               // When we have a data logging time, update the timer
               dataLogTimer -= dt4;
               if (dataLogTimer <= 0.0f) {
                  // At timeout, log the player's data and ...
                  if (getAnyEventLogger() != 0) {
                     TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(2, this); // type 2: update
                     getAnyEventLogger()->log(evt);
                     evt->unref();
                  }
                  // reset the timer.
                  dataLogTimer = dataLogTime; 
               }
            }

            // Update signatures after we've updated our dynamics
            if (signature != 0) signature->updateTC(dt4);
            if (irSignature != 0) irSignature->updateTC(dt4);
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
void Player::updateData(const LCreal dt)
{         
   if (mode == ACTIVE || mode == PRE_RELEASE) {

      // Update signatures
      if (signature != 0) signature->updateData(dt);
      if (irSignature != 0) irSignature->updateData(dt);

      // ---
      // Update the terrain elevation
      // ---
      updateElevation();

      // ---
      // Note: our subsystems in the components list (e.g., pilot, nav, sms and obc) are update
      // by our call to BaseClass:updateData()
      // ---
      BaseClass::updateData(dt);
   }
}

//-----------------------------------------------------------------------------
// Get funtions
//-----------------------------------------------------------------------------

// getMajorType() -- Returns the player's major type
unsigned int Player::getMajorType() const
{
   return GENERIC;
}

// Player's gross weight (lbs)
LCreal Player::getGrossWeight() const
{
   return 0.0;
}

// Default: mach number
LCreal Player::getMach() const
{
   // Really only good up to around 30,000 feet, and
   // using standard air temp of 15 degrees Celsius

   const LCreal G = 1.4;     // density ratio      // number
   const LCreal R = 287.06;  // gas constant       // mps/degK
   const LCreal L = 0.0065;  // temp. lapse rate   // degC/meter
   
   LCreal altitude   = getAltitude();              // meters
   LCreal velocPlane = getTotalVelocity();         // meters/sec
   LCreal Tc         = 15.0 - L * altitude;        // degrees Celsius
   LCreal Tk         = Tc + 273.15;                // degrees Kelvin
   LCreal velocSound = std::sqrt(G * R * Tk);      // meters/sec
   LCreal mach       = velocPlane / velocSound;    // number

   return mach;
}

// Player's Center-of-Gravity (%)
LCreal Player::getCG() const
{
   return 0.0;
}

// Return true if heading-hold mode is on?
bool Player::isHeadingHoldOn() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->isHeadingHoldOn();
   else
      return false;
}

// Returns commanded heading, default (radians)
double Player::getCommandedHeading() const
{
   return getCommandedHeadingD() * (LCreal)Basic::Angle::D2RCC;
}

// Returns commanded heading (degrees)
double Player::getCommandedHeadingD() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->getCommandedHeadingD();
   else
      return 0;
}

// Returns commanded heading (radians)
double Player::getCommandedHeadingR() const
{
   return getCommandedHeadingD() * (LCreal)Basic::Angle::D2RCC;
}

// True if velocity-hold mode is on?
bool Player::isVelocityHoldOn() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->isVelocityHoldOn();
   else
      return false;
}

// Commanded (true) velocity (knots)
double Player::getCommandedVelocityKts() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->getCommandedVelocityKts();
   else
      return 0;
}

// Commanded (true) velocity (Feet/Sec)
double Player::getCommandedVelocityFps() const
{
   return getCommandedVelocityKts() * Basic::Distance::NM2FT / Basic::Time::H2S;
}

// Commanded (true) velocity (Meters/Sec)
double Player::getCommandedVelocityMps() const
{
   return getCommandedVelocityKts() * Basic::Distance::NM2M / Basic::Time::H2S;
}

// True if altitude-hold mode is on?
bool Player::isAltitudeHoldOn() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->isAltitudeHoldOn();
   else
      return false;
}

// Get cmmanded (HAE) altitude, default (meters)
double Player::getCommandedAltitude() const
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->getCommandedAltitude();
   else
      return 0;
}

// Get cmmanded (HAE) altitude (meters)
double Player::getCommandedAltitudeM() const
{
   return getCommandedAltitude();
}

// Get cmmanded (HAE) altitude (Feet)
double Player::getCommandedAltitudeFt() const
{
   return getCommandedAltitude() * Basic::Distance::M2FT;
}

// True if player is destroyed
bool Player::isDestroyed() const
{
   return (damage > 0.999f);
}

// Player's outgoing NIB(s)
Nib* Player::getLocalNib(const unsigned int netId)
{
   Nib* p = 0;
   if (nibList != 0 && netId >= 1 && netId <= NetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

// Player's outgoing NIB(s)  (const version)
const Nib* Player::getLocalNib(const unsigned int netId) const
{
   const Nib* p = 0;
   if (nibList != 0 && netId >= 1 && netId <= NetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

//------------------------------------------------------------------------------
// Controlling simulation access functions
//------------------------------------------------------------------------------

// Controlling simulation model
Simulation* Player::getSimulation()
{
   if (sim == 0) {
      sim = (Simulation*) findContainerByType(typeid(Simulation));
      if (sim == 0 && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Player::getSimulation(): ERROR, unable to locate the Simulation class!" << std::endl;
      }
   }
   return sim;
}

// Controlling simulation model (const version)
const Simulation* Player::getSimulation() const
{
   if (sim != 0) {
      return sim;
   }
   else {
      // Yes this is a "const cast-away", but its the non-const version
      // that initially finds our Simulation class.
      Player* p = (Player*) this;
      Simulation* s = p->getSimulation();
      return (const Simulation*) s;
   }
}

//------------------------------------------------------------------------------
// Dynamics model access functions
//------------------------------------------------------------------------------

// Player's dynamics model
DynamicsModel* Player::getDynamicsModel()
{
   return (dyn != 0) ? ((DynamicsModel*) dyn->object()) : 0;
}

// Player's dynamics model (const version)
const DynamicsModel* Player::getDynamicsModel() const
{
   return (dyn != 0) ? ((DynamicsModel*) dyn->object()) : 0;
}

// Name of the Player's dynamics model
const Basic::Identifier* Player::getDynamicsModelName() const
{
   return (dyn != 0) ? dyn->slot() : 0;
}

//------------------------------------------------------------------------------
// Pilot model (autopilot, pilot-decision-logic (PDL), pilot interface) access functions
//------------------------------------------------------------------------------

// Player's pilot model
Pilot* Player::getPilot()
{
   return (pilot != 0) ? ((Pilot*) pilot->object()) : 0;
}

// Player's pilot model (const version)
const Pilot* Player::getPilot() const
{
   return (pilot != 0) ? ((Pilot*) pilot->object()) : 0;
}

// Name of the player's pilot model
const Basic::Identifier* Player::getPilotName() const
{
   return (pilot != 0) ? pilot->slot() : 0;
}

// Returns a Pilot model by its name
Pilot* Player::getPilotByName(const char* const name1)
{
   Pilot* p = 0;
   if (pilot != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Pilot*>( pair->object() );
      }
   }
   return p;
}

// Returns a Pilot model by its type
Basic::Pair* Player::getPilotByType(const std::type_info& type)
{
   Basic::Pair* p = 0;  // Our return value

   if (pilot != 0) {
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
   return (sms != 0) ? ((StoresMgr*) sms->object()) : 0;
}

// Player's stores (weapons, fuel) manager model (const version)
const StoresMgr* Player::getStoresManagement() const
{
   return (sms != 0) ? ((StoresMgr*) sms->object()) : 0;
}

// Name of the player's stores (weapons, fuel) manager model
const Basic::Identifier* Player::getStoresManagementName() const
{
   return (sms != 0) ? sms->slot() : 0;
}

//------------------------------------------------------------------------------
// Datalink model access functions
//------------------------------------------------------------------------------

// Player's top level Datalink model
Datalink* Player::getDatalink()
{
   return (datalink != 0) ? ((Datalink*) datalink->object()) : 0;
}

// Player's top level Datalink (const version)
const Datalink* Player::getDatalink() const
{
   return (datalink != 0) ? ((Datalink*) datalink->object()) : 0;
}

// Name of the player's top level Datalink model
const Basic::Identifier* Player::getDatalinkName() const
{
   return (datalink != 0) ? datalink->slot() : 0;
}

// Returns a Datalink model by its name
Datalink* Player::getDatalinkByName(const char* const name1)
{
   Datalink* p = 0;
   if (datalink != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Datalink*>( pair->object() );
      }
   }
   return p;
}

// Returns a Datalink model by its type
Basic::Pair* Player::getDatalinkByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                 // Our return value

   if (datalink != 0) {
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
   return (gimbal != 0) ? ((Gimbal*) gimbal->object()) : 0;
}

// Player's top level Gimbal (const version)
const Gimbal* Player::getGimbal() const
{
   return (gimbal != 0) ? ((Gimbal*) gimbal->object()) : 0;
}

// Name of the player's top level Gimbal model
const Basic::Identifier* Player::getGimbalName() const
{
   return (gimbal != 0) ? gimbal->slot() : 0;
}

// Returns a Gimbal model by its name
Gimbal* Player::getGimbalByName(const char* const name1)
{
   Gimbal* p = 0;
   if (gimbal != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Gimbal*>( pair->object() );
      }
   }
   return p;
}

// Returns a Gimbal model by its type
Basic::Pair* Player::getGimbalByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (gimbal != 0) {
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
   return (nav != 0) ? ((Navigation*) nav->object()) : 0;
}

// Player's top level Navigation (const version)
const Navigation* Player::getNavigation() const
{
   return (nav != 0) ? ((Navigation*) nav->object()) : 0;
}

// Name of the player's top level Navigation model
const Basic::Identifier* Player::getNavigationName() const
{
   return (nav != 0) ? nav->slot() : 0;
}

// Returns a Navigation model by its name
Navigation* Player::getNavigationByName(const char* const name1)
{
   Navigation* p = 0;
   if (nav != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Navigation*>( pair->object() );
      }
   }
   return p;
}

// Returns a Navigation model by its type
Basic::Pair* Player::getNavigationByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (nav != 0) {
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
   return (obc != 0) ? ((OnboardComputer*) obc->object()) : 0;
}

// Player's top level OnboardComputer (const version)
const OnboardComputer* Player::getOnboardComputer() const 
{
   return (obc != 0) ? ((OnboardComputer*) obc->object()) : 0;
}

// Name of the player's top level OnboardComputer model
const Basic::Identifier* Player::getOnboardComputerName() const
{
   return (obc != 0) ? obc->slot() : 0;
}

// Returns an OnboardComputer model by its name
OnboardComputer* Player::getOnboardComputerByName(const char* const name1)
{
   OnboardComputer* p = 0;
   if (obc != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<OnboardComputer*>( pair->object() );
      }
   }
   return p;
}

// Returns an OnboardComputer model by its type
Basic::Pair* Player::getOnboardComputerByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (obc != 0) {
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
   return (radio != 0) ? ((Radio*) radio->object()) : 0;
}

// Player's top level Radio (const version)
const Radio* Player::getRadio() const
{
   return (radio != 0) ? ((Radio*) radio->object()) : 0;
}

// Name of the player's top level Radio model
const Basic::Identifier* Player::getRadioName() const
{
   return (radio != 0) ? radio->slot() : 0;
}

// Returns a Radio model by its name
Radio* Player::getRadioByName(const char* const name1)
{
   Radio* p = 0;
   if (radio != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<Radio*>( pair->object() );
      }
   }
   return p;
}

// Returns a Radio model by its type
Basic::Pair* Player::getRadioByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (radio != 0) {
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
   return (sensor != 0) ? ((RfSensor*) sensor->object()) : 0;
}

// Name of the player's top level R/F sensor model
const RfSensor* Player::getSensor() const
{
   return (sensor != 0) ? ((RfSensor*) sensor->object()) : 0;
}

// Name of the player's top level R/F sensor model
const Basic::Identifier* Player::getSensorName() const
{
   return (sensor != 0) ? sensor->slot() : 0;
}

// Returns a R/F sensor model by its name
RfSensor* Player::getSensorByName(const char* const name1)
{
   RfSensor* p = 0;
   if (sensor != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<RfSensor*>( pair->object() );
      }
   }
   return p;
}

// Returns a R/F sensor model by its type
Basic::Pair* Player::getSensorByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (sensor != 0) {
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
   return (irSystem != 0) ? ((IrSystem*) irSystem->object()) : 0;
}

// Name of the player's top level IR sensor model
const IrSystem* Player::getIrSystem() const
{
   return (irSystem != 0) ? ((IrSystem*) irSystem->object()) : 0;
}

// Name of the player's top level IR sensor model
const Basic::Identifier* Player::getIrSystemName() const
{
   return (irSystem != 0) ? irSystem->slot() : 0;
}

// Returns a IR sensor model by its name
IrSystem* Player::getIrSystemByName(const char* const name1)
{
   IrSystem* p = 0;
   if (irSystem != 0) {

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
      Basic::Pair* pair = 0;
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
      if (pair != 0) {
         // Yes, now make sure it's the correct type!
         p = dynamic_cast<IrSystem*>( pair->object() );
      }
   }
   return p;
}

// Returns a IR sensor model by its type
Basic::Pair* Player::getIrSystemByType(const std::type_info& type)
{
   Basic::Pair* p = 0;                // Our return value

   if (irSystem != 0) {
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
bool Player::setType(Basic::String* const msg)
{
   bool ok = true;
   type = msg;
   return ok;
}

// Set the player's name 
void Player::setName(const Basic::String& n)
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
bool Player::setDamage(const LCreal v)
{
   LCreal x = v;
   if (x < 0) x = 0.0f;
   if (x > 1) x = 1.0f;
   damage = x;
   return true;
}

// Sets the smoke for this player
bool Player::setSmoke(const LCreal v)
{
   LCreal x = v;
   if (x < 0) x = 0.0f;
   if (x > 1) x = 1.0f;
   smoking = x;
   return true;
}

// Sets the flames for this player
bool Player::setFlames(const LCreal v)
{
   LCreal x = v;
   if (x < 0) x = 0.0f;
   if (x > 1) x = 1.0f;
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
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setHeadingHoldOn(b);
   else
      return false;
}

// Sets the commanded (true) heading (radians)
bool Player::setCommandedHeading(const double h)
{
   return setCommandedHeadingD( h * Basic::Angle::R2DCC );
}

// Sets commanded (true) hheading (true: degs)
bool Player::setCommandedHeadingD(const double h)
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setCommandedHeadingD(h);
   else
      return false;
}

// Sets the commanded (true) hheading (radians)
bool Player::setCommandedHeadingR(const double h)
{
   return setCommandedHeadingD( h * Basic::Angle::R2DCC );
}

// Enable/Disable velocity hold
bool Player::setVelocityHoldOn(const bool b)
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setVelocityHoldOn(b);
   else
      return false;
}

// Sets the commanded (true) velocity (knots)
bool Player::setCommandedVelocityKts(const double a)
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setCommandedVelocityKts(a);
   else
      return false;
}

// Enable/Disable altitude hold
bool Player::setAltitudeHoldOn(const bool b)
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setAltitudeHoldOn(b);
   else
      return false;
}

// Sets cmmanded (HAE) altitude, default (meters)
bool Player::setCommandedAltitude(const double a)
{
   if (getDynamicsModel() != 0)
      return getDynamicsModel()->setCommandedAltitude(a);
   else
      return false;
}

// Sets cmmanded (HAE) altitude (meters)
bool Player::setCommandedAltitudeM(const double a)
{
   return setCommandedAltitude(a);
}

// Sets cmmanded (HAE) altitude (Feet)
bool Player::setCommandedAltitudeFt(const double a)
{
   return setCommandedAltitude( a * Basic::Distance::FT2M );
}

// Sets a pointer to the Network Interface Block (NIB)
bool Player::setNib(Nib* const n)
{
   if (nib != 0) nib->unref();
   nib = n;
   if (nib != 0) {
      // Ref() the new NIB and get the network ID
      nib->ref();
      NetIO* netIO = nib->getNetIO();
      if (netIO != 0) netID = netIO->getNetworkID();
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
   if (nibList != 0 && id >= 1 && id <= NetIO::MAX_NETWORD_ID) {
      unsigned int idx = id - 1;
      if (nibList[idx] != 0) nibList[idx]->unref();
      nibList[idx] = p;
      if (nibList[idx] != 0) nibList[idx]->ref();
   }
   return ok;
}

// Sets the elevation of the terrain at this player's location (meters)
void Player::setTerrainElevation(const LCreal v)
{
   tElev = v;
   tElevValid = true;
}

// Sets the ground clamping offset (meters)
bool Player::setTerrainOffset(const LCreal v)
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
   const Basic::EarthModel* em = s->getEarthModel();

   // Set the position vector relative to sim ref pt
   posVecNED.set(n, e, d);

   // The position vector is valid if the gaming area range is umlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   // Compute & set the lat/lon/alt position
   double refLat = s->getRefLatitude();
   double refLon = s->getRefLongitude();
   double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      double sinRlat = s->getSinRefLat();
      Basic::Nav::convertPosVec2llE(refLat, refLon, sinRlat, cosRlat, posVecNED, &latitude, &longitude, &altitude, em);
   }
   else {
      Basic::Nav::convertPosVec2llS(refLat, refLon, cosRlat, posVecNED, &latitude, &longitude, &altitude);
   }

   // compute the world matrix
   Basic::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute & set the  geocentric position
   double lla[3] = { latitude, longitude, altitude };
   double ecef[3] = { 0, 0, 0 };
   Basic::Nav::convertGeod2Ecef(lla, ecef, em);
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
   const Basic::EarthModel* em = s->getEarthModel();

   // Set the lat/lon position
   latitude = lat;
   longitude = lon;
   altitude = alt;

   // compute the world matrix
   Basic::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute and set the position vector relative to sim ref pt
   double refLat = s->getRefLatitude();
   double refLon = s->getRefLongitude();
   double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      double sinRlat = s->getSinRefLat();
      Basic::Nav::convertLL2PosVecE(refLat, refLon, sinRlat, cosRlat, lat, lon, alt, &posVecNED, em);   
   }
   else {
      Basic::Nav::convertLL2PosVecS(refLat, refLon, cosRlat, lat, lon, alt, &posVecNED);   
   }

   // The position vector is valid if the gaming area range is umlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   // Compute & set the geocentric position
   double lla[3] = { lat, lon, alt };
   double ecef[3] = { 0, 0, 0 };
   Basic::Nav::convertGeod2Ecef(lla, ecef, em);
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
   const Basic::EarthModel* em = s->getEarthModel();

   // Set the geocentric position
   posVecECEF = pos;

   // Compute & set the geodetic position
   double ecef[3] = { posVecECEF[0], posVecECEF[1], posVecECEF[2] };
   double lla[3] = { 0, 0, 0 };
   Basic::Nav::convertEcef2Geod(ecef, lla, em);
   latitude = lla[Basic::Nav::ILAT];
   longitude = lla[Basic::Nav::ILON];
   altitude = lla[Basic::Nav::IALT];

   // compute the world matrix
   Basic::Nav::computeWorldMatrix(latitude, longitude, &wm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // Compute and set the position vector relative to sim ref pt
   double refLat = s->getRefLatitude();
   double refLon = s->getRefLongitude();
   double cosRlat = s->getCosRefLat();
   if (s->isGamingAreaUsingEarthModel()) {
      double sinRlat = s->getSinRefLat();
      Basic::Nav::convertLL2PosVecE(refLat, refLon, sinRlat, cosRlat, latitude, longitude, altitude, &posVecNED, em);   
   }
   else {
      Basic::Nav::convertLL2PosVecS(refLat, refLon, cosRlat, latitude, longitude, altitude, &posVecNED);   
   }

   // The position vector is valid if the gaming area range is umlimited (zero) or
   // if the vector's length is less than or equal the max range.
   posVecValid = (maxRefRange <= 0.0) || (posVecNED.length2() <= (maxRefRange*maxRefRange));

   altSlaved = slaved;
   posSlaved = slaved;

   return true;
}


// Sets euler angles: (rad) [ roll pitch yaw ]
bool Player::setEulerAngles(const double r, const double p, const double y)
{
   // Set angles
   angles.set(r,p,y);

   // Compute rotational matrix and the sin/cos values of the angles
   Basic::Nav::computeRotationalMatrix(r, p, y, &rm, &scPhi, &scTheta, &scPsi);

   // Set quaternions
   q.set(rm);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   Basic::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   return true;
}

// Sets euler angle vector: (rad) [ roll pitch yaw ]
bool Player::setEulerAngles(const osg::Vec3d& newAngles)
{
   return setEulerAngles(newAngles.x(), newAngles.y(), newAngles.z());
}

// Sets geocentric (body/ECEF) euler angles: (radians) [ roll pitch yaw ]
bool Player::setGeocEulerAngles(const osg::Vec3d& newAngles)
{
   // Set the geocentric angles
   anglesW = newAngles;

   // Compute sin/cos values and directional cosine matrix
   Basic::Nav::computeRotationalMatrix(anglesW, &rmW2B, &scPhiW, &scThetaW, &scPsiW);

   // Transpose the world matrix
   osg::Matrixd wmT = wm;
   wmT.transpose();

   // Compute rotational matrix: body/NED directional cosines
   rm = rmW2B * wmT;

   // compute Geodetic orientation angles
   Basic::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // Set quaternions
   q.set(rm);

   return true;
}

// Sets the rotational matrix
bool Player::setRotMat(const osg::Matrixd& rr)
{
   // set the matrix
   rm = rr;

   // Set quaternion
   q.set(rm);

   // Compute the euler angles and the sin/cos values of the angles
   Basic::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   Basic::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

   return true;
}

// Sets the quaternon
bool Player::setQuaternions(const osg::Quat& newQ)
{
   // Set quaternions
   q = newQ;

   // Set the rotational matrix
   rm.set(q);

   // Compute the euler angles and the sin/cos values of the angles
   Basic::Nav::computeEulerAngles(rm, &angles, &scPhi, &scTheta, &scPsi);

   // compute body/ECEF directional cosines
   rmW2B = rm * wm;

   // compute geocentric orientation angles and their sin/cos values
   Basic::Nav::computeEulerAngles(rmW2B, &anglesW, &scPhiW, &scThetaW, &scPsiW);

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
bool Player::setVelocity(const LCreal ue, const LCreal ve, const LCreal we)
{
   velVecNED.set(ue,ve,we);      // set local NED velocity vectors
   velVecECEF = velVecNED * wm;   // compute geocentric velocity vector
   velVecBody = rm * velVecNED;  // compute body velocity vector

   // Compute other velocities
   vp = lcSqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = lcSqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = lcAtan2(ve,ue);            // Ground track

   return true;
}

// Sets local NED velocity vector; (meters/sec) NED
bool Player::setVelocity(const osg::Vec3& newVel)
{
   setVelocity(newVel.x(), newVel.y(), newVel.z());
   return true;
}

// Sets local NED acceleration vector; (m/s/s) NED
bool Player::setAcceleration(const LCreal due, const LCreal dve, const LCreal dwe)
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
bool Player::setVelocityBody(const LCreal ua, const LCreal va, const LCreal wa)
{
   velVecBody.set(ua,va,wa);
   velVecNED = velVecBody * rm;  // compute local NED velocity vector
   velVecECEF = velVecNED * wm;   // compute geocentric velocity vector

   // Compute other velocities
   LCreal ue = (LCreal) velVecNED[INORTH];
   LCreal ve = (LCreal) velVecNED[IEAST];
   LCreal we = (LCreal) velVecNED[IDOWN];
   vp = lcSqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = lcSqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = lcAtan2(ve,ue);            // Ground track
   return true;
}

bool Player::setVelocityBody(const osg::Vec3& newVelBody)
{
   setVelocityBody(newVelBody.x(), newVelBody.y(), newVelBody.z());
   return true;
}


// Sets body acceleration vector; (meters/second / second) NED
bool Player::setAccelerationBody(const LCreal dua, const LCreal dva, const LCreal dwa)
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
bool Player::setGeocVelocity(const LCreal vx, const LCreal vy, const LCreal vz)
{
   velVecECEF.set(vx,vy,vz);
   velVecNED = wm * velVecECEF;
   velVecBody = rm * velVecNED;

   // Compute other velocities
   LCreal ue = (LCreal) velVecNED[INORTH];
   LCreal ve = (LCreal) velVecNED[IEAST];
   LCreal we = (LCreal) velVecNED[IDOWN];
   vp = lcSqrt(ue*ue + ve*ve + we*we); // Total
   gndSpd = lcSqrt(ue*ue + ve*ve);     // Ground speed
   gndTrk = lcAtan2(ve,ue);            // Ground track
   return true;
}

bool Player::setGeocVelocity(const osg::Vec3& newVelEcef)
{
   setGeocVelocity(newVelEcef.x(), newVelEcef.y(), newVelEcef.z());
   return true;
}

// Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
bool Player::setGeocAcceleration(const LCreal dvx, const LCreal dvy, const LCreal dvz)
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
void Player::setControlStickRollInput(const LCreal value)
{
   if (getDynamicsModel() != 0) {
      getDynamicsModel()->setControlStickRollInput(value);
   }
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void Player::setControlStickPitchInput(const LCreal value)
{
   if (getDynamicsModel() != 0) {
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
int Player::setThrottles(const LCreal* const data, const int num)
{
   int  n = 0;
   if (getDynamicsModel() != 0) {
      n = getDynamicsModel()->setThrottles(data, num);
   }
   return n;
}

//------------------------------------------------------------------------------
// Process weapon detonation 
//------------------------------------------------------------------------------
void Player::processDetonation(const LCreal detRange, Weapon* const wpn)
{
   if (!isKillOverride()) {

      // Weapon, launcher & range info
      Player* launcher = 0;
      LCreal rng = detRange;
      LCreal blastRange  = 500.0f;    // burst range (meters)
      LCreal lethalRange =  50.0f;    // lethal range  (meters)
      if (wpn != 0) {
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
         LCreal damageRng = blastRange - lethalRange;
         if (damageRng <= 1.0f) damageRng = 1.0f;
         LCreal newDamage = 1.0f - ( (rng - lethalRange) / damageRng );
         setDamage(newDamage + getDamage());
         setFlames( getDamage() - 0.25f );
         setSmoke( getDamage() + 0.25f );
         if ( isDestroyed() ) {
            event(KILL_EVENT, launcher);
         }
      }

   }

   // record EVERYTHING that had the potential to cause damage, even if killOverride
   if (getAnyEventLogger() != 0) {
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
         Basic::PairStream* subcomponents = getComponents();
         if (subcomponents != 0) {
            for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
               Basic::Pair* pair = (Basic::Pair*)(item->getValue());
               Basic::Component* sc = (Basic::Component*) pair->object();
               sc->event(KILL_EVENT, p);
            }
            subcomponents->unref();
            subcomponents = 0;
         }
      }

      setDamage(1.0f);
      setSmoke(1.0f);
      setFlames(1.0f);

      // Set our status
      if (killRemoval && isLocalPlayer()) {

         justKilled = true;
         setMode(KILLED);

         if (p != 0) killedBy = p->getID();
         else  killedBy = 0;
      }

   }

   // record kill, even if killOverride 
   if (getAnyEventLogger() != 0) {
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
         Basic::PairStream* subcomponents = getComponents();
         if (subcomponents != 0) {
            for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
               Basic::Pair* pair = (Basic::Pair*)(item->getValue());
               Basic::Component* sc = (Basic::Component*) pair->object();
               sc->event(KILL_EVENT, p);
            }
            subcomponents->unref();
            subcomponents = 0;
         }
      }

      // Set our status
      justKilled = true;
      if (p != 0) killedBy = p->getID();
      else  killedBy = 0;
      setMode(CRASHED);        
   }

   // record EVERYTHING that had the potential to cause damage, even if crashOverride
   if (getAnyEventLogger() != 0) {
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
         Basic::PairStream* subcomponents = getComponents();
         if (subcomponents != 0) {
            for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
               Basic::Pair* pair = (Basic::Pair*)(item->getValue());
               Basic::Component* sc = (Basic::Component*) pair->object();
               sc->event(KILL_EVENT);
            }
            subcomponents->unref();
            subcomponents = 0;
         }
      }

      // Set our status
      justKilled = true;
      killedBy = 0;
      setMode(CRASHED);
   }

   // record EVERYTHING that had the potential to cause damage, even if crashOverride
   if (getAnyEventLogger() != 0) {
      TabLogger::TabLogEvent* evt = new TabLogger::LogPlayerData(6, this); // type 6: crash
      getAnyEventLogger()->log(evt);
      evt->unref();
   }

   return true;
}

//------------------------------------------------------------------------------
// onWpnRelEvent() -- Manage the Wpn Release event 
//------------------------------------------------------------------------------
bool Player::onWpnRelEvent(const Basic::Boolean* const sw)
{
   bool used = false;

   StoresMgr* p = getStoresManagement();
   if (p != 0) {
      // When we have an SMS, let it handle this event ...
      used = p->onWpnRelEvent(sw);
   }
   else {
      // We we dont' have an SMS, we can do some TBD default handling of this event.
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
bool Player::onTriggerSwEvent(const Basic::Boolean* const sw)
{
   bool used = false;

   if (sms != 0) {
      // When we have an SMS, let it handle this event ...
      used = getStoresManagement()->onTriggerSwEvent(sw);
   }
   else {
      // We we dont' have an SMS, we can do some TBD default handling of this event.
      used = true;
   }

   return used;
}

//------------------------------------------------------------------------------
// onTgtStepEvent() -- Manage the target step (reject) event
//------------------------------------------------------------------------------
bool Player::onTgtStepEvent()
{
   if (obc != 0) {
      getOnboardComputer()->updateShootList(true);
   }
   return true;
}


//------------------------------------------------------------------------------
// onRfEmissionEventPlayer() -- process RF Emisison events
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
      LCreal xa = aoi.x();
      LCreal ya = aoi.y();
      LCreal za = -aoi.z();

      // 3-b) Compute azimuth: az = atan2(ya, xa)
      LCreal aazr = lcAtan2(ya, xa);
      em->setAzimuthAoi(aazr);

      // 3-c) Compute elevation: el = atan2(za, ra), where 'ra' is sqrt of xa*xa & ya*ya
      LCreal ra = lcSqrt(xa*xa + ya*ya);
      LCreal aelr = lcAtan2(za,ra);
      em->setElevationAoi(aelr);
   }

   // 4) Compute and return the RCS
   if (em->isReturnRequested()) {

      if (signature != 0) {
         LCreal rcs = signature->getRCS(em);
         em->setRCS(rcs);
      }
      else em->setRCS(0);

      // Send reflected emissions back to the transmitter
      em->getGimbal()->event(RF_EMISSION_RETURN,em);
   }

   // 6) Pass the emission to our antennas
   {
      Gimbal* g = getGimbal();
      if (g != 0 && g->getPowerSwitch() != System::PWR_OFF) {
         g->event(RF_EMISSION,em);
      }
   }

   // 7) Pass the emission to anyone requesting reflected emissions
   //    (we're doing do calculations here, this is only meaningful to
   //     the receiving player)
   for (unsigned int i = 0; i < MAX_RF_REFLECTIONS; i++) {
      if (rfReflect[i] != 0) rfReflect[i]->event(RF_REFLECTED_EMISSION,em);
   }

   return true;
}

//------------------------------------------------------------------------------
// onRfReflectedEmissionEventPlayer() -- process reflected R/F Emisison events
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
bool Player::onReflectionsRequest(Basic::Component* const p)
{
   bool ok = false;        // Did we succeed?
   int idx = -1;           // Empty slot index

   // First see if this is a re-request ...
   //  (and look for an empty slot while we're at it)
   for (int i = 0; i < MAX_RF_REFLECTIONS && !ok; i++) {
      if (rfReflect[i] == p) {
         // Old request -- reset the timer
         rfReflectTimer[i] = 1.1f;
         ok = true;
      }
      else if (rfReflect[i] == 0) {
         idx = i;
      }
   }

   // New request and we have an empty slot?
   if (!ok && idx >= 0) {
      p->ref();
      rfReflect[idx] = p;
      rfReflectTimer[idx] = 1.1f;
      ok = true;
   }

   return ok;
}


//------------------------------------------------------------------------------
// onReflectionsCancel() -- cancel a request for reflected R/F emissions 
//------------------------------------------------------------------------------
bool Player::onReflectionsCancel(const Basic::Component* const p)
{
   bool ok = false;        // Did we succeed?

   // Find player 'p' and clear it's request
   for (int i = 0; i < MAX_RF_REFLECTIONS && !ok; i++) {
      if (rfReflect[i] == p) {
         // Clear the request
         rfReflect[i]->unref();
         rfReflect[i] = 0;
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
   if (isNotMode(ACTIVE) || irSignature == 0) {
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
   LCreal xa = aoi.x();
   LCreal ya = aoi.y();
   LCreal za = -aoi.z();

   // 3-b) Compute azimuth: az = atan2(ya, xa)
   LCreal aazr = lcAtan2(ya, xa);
   msg->setAzimuthAoi(aazr);

   // 3-c) Compute elevation: el = atan2(za, ra), where 'ra' is sqrt of xa*xa & ya*ya
   LCreal ra = lcSqrt(xa*xa + ya*ya);
   LCreal aelr = lcAtan2(za,ra);
   msg->setElevationAoi(aelr);

   // 4) Compute and return the IR Signature
   bool hasSignature = irSignature->getIrSignature(msg);

   // 5) If the target has a signature, send the query response back to seeker
   if (hasSignature) msg->getGimbal()->event(IR_QUERY_RETURN,msg);

   return true;
}

// onDatalinkMessageEventPlayer() -- process datalink message events
bool Player::onDatalinkMessageEventPlayer(Basic::Object* const msg)
{
   // Just pass it down to all of our datalink system
   if (getDatalink() != 0) {
      getDatalink()->event(DATALINK_MESSAGE,msg);
   }
   return true;
}

// Handles the DE_EMISSION event
bool Player::onDeEmissionEvent(Basic::Object* const msg)
{
   return false;
}

//------------------------------------------------------------------------------
// The player's dynamics
//------------------------------------------------------------------------------
void Player::dynamics(const LCreal dt)
{
   // ---
   // Local player ...
   // ---
   if (isLocalPlayer()) {
      // Update the external dynamics model (if any)
      if (getDynamicsModel() != 0) {
         // If we have a dynamics model ...
         getDynamicsModel()->freeze( isFrozen() );
         getDynamicsModel()->dynamics(dt);
      }

      // Update our position
      positionUpdate(dt);

      // ---
      // Check for ground collisions
      // ---
      if (getAltitudeAgl() < 0.0 && isLocalPlayer() && isMajorType(AIR_VEHICLE | WEAPON | SPACE_VEHICLE)) {
         // We're below the ground!
         this->event(CRASH_EVENT,0);
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
// If the player can be ground clampped (ground vehicles) or if either the
// position freeze or the altitude freeze is set then we'll integrate and
// set our position using our geodetic coordinates and our NED velocity vector.
// Otherwise we'll integrate and set our position using ECEF coordinates and our
// ECEF velocity.
//------------------------------------------------------------------------------
void Player::positionUpdate(const LCreal dt)
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
               // Ground clampping enabled
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
      // Update using geodetic latitude, longitude and height abover terrain
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
 
            double slat = std::sin(latitude * Basic::Angle::D2RCC);
            double clat = std::cos(latitude * Basic::Angle::D2RCC);

            const Basic::EarthModel* em = getSimulation()->getEarthModel();
            if (em == 0) em = &Basic::EarthModel::wgs84;

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
            newLatitude += (dlatR * Basic::Angle::R2DCC);
            if (newLatitude >  90.0) {
               newLatitude = 180.0 - newLatitude;
               newLongitude = Basic::Angle::aepcdDeg( newLongitude + 180.0 );
            }
            if (newLatitude < -90.0) {
               newLatitude = -180.0 - newLatitude;
               newLongitude = Basic::Angle::aepcdDeg( newLongitude + 180.0 );
            }

            // Update longitude
            if (clat != 0) {
               double de = (ve + ve0) * 0.5 * dt;
               double dlonR = ( 1.0 / (rn * clat) ) * de;

               newLongitude += (dlonR * Basic::Angle::R2DCC);
               if (newLongitude >  180.0) newLongitude -= 360.0;
               if (newLongitude < -180.0) newLongitude += 360.0;
            }
         }

         if (!afrz) {
            if (gcEnabled) {
               // Ground clampping enabled
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

               const Basic::EarthModel* em = getSimulation()->getEarthModel();
               Basic::Nav::convertEcef2Geod(ecef, lla, em);

               // 3) Set position using these ground clamped coordinates
               setPositionLLA(lla[Basic::Nav::ILAT], lla[Basic::Nav::ILON], alt);
            }
         }

         // And save our old velocity vector
         velVecN1 = velVecECEF;
      }

   }

   // ---
   // Test only: update the euler angles if we have non-zero test anguler rates
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
      newAngles[0] = Basic::Angle::aepcdRad(oldAngles[0] + (pe * dt));
      newAngles[1] = Basic::Angle::aepcdRad(oldAngles[1] + (qe * dt));
      newAngles[2] = Basic::Angle::aepcdRad(oldAngles[2] + (re * dt));
      if (newAngles[1] >  PI/2) newAngles[1] -= PI;
      if (newAngles[1] < -PI/2) newAngles[1] += PI;
      setEulerAngles(newAngles);

      // Reset the rates
      setAngularVelocities(pa, qa, ra);
      setVelocityBody(initVp, 0.0, 0.0);

      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "r1[ " << getRollD() << ", " << getPitchD() << ", " << getHeadingD() << " ]";
         std::cout << " tr[ " << testAngRates[0]*Basic::Angle::R2DCC << ", " << testAngRates[1]*Basic::Angle::R2DCC << ", " << testAngRates[2]*Basic::Angle::R2DCC << " ]";
         std::cout << " er[ " << pe*Basic::Angle::R2DCC << ", " << qe*Basic::Angle::R2DCC << ", " << re*Basic::Angle::R2DCC << " ]";
         std::cout << " br[ " << pa*Basic::Angle::R2DCC << ", " << qa*Basic::Angle::R2DCC << ", " << ra*Basic::Angle::R2DCC << " ]";
         std::cout << " na[ " << newAngles[0]*Basic::Angle::R2DCC << ", " << newAngles[1]*Basic::Angle::R2DCC << ", " << newAngles[2]*Basic::Angle::R2DCC << " ]";
         std::cout << " oa[ " << oldAngles[0]*Basic::Angle::R2DCC << ", " << oldAngles[1]*Basic::Angle::R2DCC << ", " << oldAngles[2]*Basic::Angle::R2DCC << " ]";
         std::cout << std::endl;
      }
   }
}

//------------------------------------------------------------------------------
// Default player dead-reckoning funciton (networked I-players only)
//------------------------------------------------------------------------------
void Player::deadReckonPosition(const LCreal dt)
{
   if ( !isNetworkedPlayer() ) return;

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
      const Basic::EarthModel* em = getSimulation()->getEarthModel();
      double ecef[3] = { drPos[0], drPos[1], drPos[2] };
      double lla[3] = { 0, 0, 0 };
      Basic::Nav::convertEcef2Geod(ecef, lla, em);
      double lat = lla[Basic::Nav::ILAT];
      double lon = lla[Basic::Nav::ILON];

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
}

//------------------------------------------------------------------------------
// updateSystemPointers() -- update all of our system (component) pointers
//------------------------------------------------------------------------------
void Player::updateSystemPointers()
{
   // ---
   // Set Basic::Pair pointers for our primary systems located in our list of subcomponents
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
// processComponets() -- process our components; make sure the are all of
//	type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Player::processComponents(
   Basic::PairStream* const list,
   const std::type_info& filter,
   Basic::Pair* const add,
   Basic::Component* const remove
   )
{
   // Set the 'load system pointers' flag if anything changes
   BaseClass::processComponents(list,filter,add,remove);
   loadSysPtrs = true;
}

////------------------------------------------------------------------------------
//// computeEulerRates() -- compute the vehicle's angular velocities (earth)
////------------------------------------------------------------------------------
//bool Player::computeEulerRates()
//{
//   LCreal pa = angularVel[0];
//   LCreal qa = angularVel[1];
//   LCreal ra = angularVel[2];
//
//   LCreal dpsi = 0;
//   if (ctheta != 0) dpsi = (ra*cphi + qa*sphi)/ctheta;
//   LCreal dtheta = qa*cphi - ra*sphi;
//   LCreal dphi = pa + dpsi*stheta;
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
   Simulation* s = getSimulation();
   if (s != 0 && !isTerrainElevationRequired()) {
      const Basic::Terrain* terrain = s->getTerrain();
      if (terrain != 0) {
         LCreal el = 0;
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
   const Basic::Statistic* ts = getTimingStats();
   std::cout << "Player(" << getSimulation()->cycle() << "," << getSimulation()->frame() << "," << getSimulation()->phase() << "): dt=" << ts->value() << ", ave=" << ts->mean() << ", max=" << ts->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// setDynamicsModel() -- Sets our dynamics models
//------------------------------------------------------------------------------
bool Player::setDynamicsModel(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (dyn != 0) dyn->unref();
      dyn = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(DynamicsModel)) ) {
      if (dyn != 0) dyn->unref();
      dyn = sys;
      dyn->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setDatalink() -- Sets our Datalink models
//------------------------------------------------------------------------------
bool Player::setDatalink(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (datalink != 0) datalink->unref();
      datalink = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Datalink)) ) {
      if (datalink != 0) datalink->unref();
      datalink = sys;
      datalink->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setGimbal() -- Sets our gimbal/antenna/optic models
//------------------------------------------------------------------------------
bool Player::setGimbal(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (gimbal != 0) gimbal->unref();
      gimbal = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Gimbal)) ) {
      if (gimbal != 0) gimbal->unref();
      gimbal = sys;
      gimbal->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setNavigation() -- Set our navigation system
//------------------------------------------------------------------------------
bool Player::setNavigation(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (nav != 0) nav->unref();
      nav = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Navigation)) ) {
      if (nav != 0) nav->unref();
      nav = sys;
      nav->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setOnboardComputer() -- Sets our onboard computer model
//------------------------------------------------------------------------------
bool Player::setOnboardComputer(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (obc != 0) obc->unref();
      obc = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(OnboardComputer)) ) {
      if (obc != 0) obc->unref();
      obc = sys;
      obc->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setPilot() -- Set our pilot model
//------------------------------------------------------------------------------
bool Player::setPilot(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (pilot != 0) pilot->unref();
      pilot = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Pilot)) ) {
      if (pilot != 0) pilot->unref();
      pilot = sys;
      pilot->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setRadio() -- Sets our radio models
//------------------------------------------------------------------------------
bool Player::setRadio(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (radio != 0) radio->unref();
      radio = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(Radio)) ) {
      if (radio != 0) radio->unref();
      radio = sys;
      radio->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSensor() -- Sets our sensor models
//------------------------------------------------------------------------------
bool Player::setSensor(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (sensor != 0) sensor->unref();
      sensor = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(RfSensor)) ) {
      if (sensor != 0) sensor->unref();
      sensor = sys;
      sensor->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setIrSystem() -- Sets our IR sensor models
//------------------------------------------------------------------------------
bool Player::setIrSystem(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (irSystem != 0) irSystem->unref();
      irSystem = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(IrSystem)) ) {
      if (irSystem != 0) irSystem->unref();
      irSystem = sys;
      irSystem->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setStoresMgr() -- Set our stores management system
//------------------------------------------------------------------------------
bool Player::setStoresMgr(Basic::Pair* const sys)
{
   bool ok = false;
   if (sys == 0) {
      if (sms != 0) sms->unref();
      sms = 0;
      ok = true;
   }
   else if ( sys->object()->isClassType(typeid(StoresMgr)) ) {
      if (sms != 0) sms->unref();
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
bool Player::setSlotInitXPos(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      osg::Vec2d pos = getInitPosition();
      pos[INORTH] = Basic::Meters::convertStatic(*msg);
      ok = setInitPosition(pos);
   }
   return ok;
}

// initXPos: X position (+north) (meters)
bool Player::setSlotInitXPos(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      osg::Vec2d pos = getInitPosition();
      pos[INORTH] = msg->getReal();
      ok = setInitPosition(pos);
   }
   return ok;
}

// initYPos: Y position (+east)
bool Player::setSlotInitYPos(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      osg::Vec2d pos = getInitPosition();
      pos[IEAST] = Basic::Meters::convertStatic(*msg);
      ok = setInitPosition(pos);
   }
   return ok;
}

// initYPos: Y position (+east)(meters)
bool Player::setSlotInitYPos(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      osg::Vec2d pos = getInitPosition();
      pos[IEAST] = msg->getReal();
      ok = setInitPosition(pos);
   }
   return ok;
}

// initAlt: Altitude (HAE @ sim ref pt) (+up)
bool Player::setSlotInitAlt(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal value = Basic::Meters::convertStatic(*msg);
      setInitAltitude( value );
      ok = true;
   }
   return ok;
}

// initAlt: Altitude (HAE @ sim ref pt) (+up) (meters)
bool Player::setSlotInitAlt(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal value = msg->getReal();
      setInitAltitude( value );
      ok = true;
   }
   return ok;
}

// initPosition: Position Vector: meters [ north east down ]
bool Player::setSlotInitPosition(const Basic::List* const msg)
{
   bool ok = false;
   double values[3];
   int n = msg->getNumberList(values, 3);
   if (n == 3) {
      setInitPosition(values[0], values[1]);
      setInitAltitude( -values[2] );
      ok = true;
   }
   return ok;
}

// initLatitude: Latitude
bool Player::setSlotInitLat(const Basic::LatLon* const msg)
{
   bool ok = false;
   if (msg != 0) {
      double val = msg->getDouble();
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
bool Player::setSlotInitLat(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal val = (LCreal) Basic::Degrees::convertStatic(*msg);
      if (val >= -90.0f && val <= 90.0f) {
         ok = setInitLat( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLatitude: Latitude (degrees)
bool Player::setSlotInitLat(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal val = msg->getReal();
      if (val >= -90.0f && val <= 90.0f) {
         ok = setInitLat( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLongitude: Longitude
bool Player::setSlotInitLon(const Basic::LatLon* const msg)
{
   bool ok = false;
   if (msg != 0) {
      double val = msg->getDouble();
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
bool Player::setSlotInitLon(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal val = (LCreal) Basic::Degrees::convertStatic(*msg);
      if (val >= -180.0f && val <= 180.0f) {
         ok = setInitLon( val );
      }
      else {
         std::cerr << "Player::setSlotInitLon(): invalid longitude: " << val << std::endl;
      }
   }
   return ok;
}

// initLongitude: Longitude (degrees)
bool Player::setSlotInitLon(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      LCreal val = msg->getReal();
      if (val >= -180.0f && val <= 180.0f) {
         ok = setInitLon( val );
      }
      else {
         std::cerr << "Player::setSlotInitLat(): invalid latitude: " << val << std::endl;
      }
   }
   return ok;
}

// initGeocentric: Position vector [ x y z ] (meters)
bool Player::setSlotInitGeocentric(const Basic::List* const msg)
{
   bool ok = false;
   double values[3];
   int n = msg->getNumberList(values, 3);
   if (n == 3) {
      osg::Vec3d pos(values[0], values[1], values[2]);
      ok = setInitGeocentricPosition(pos);
   }
   return ok;
}

// initRoll: Initial roll angle
bool Player::setSlotInitRoll(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);

      if ( value >= -PI && value <= PI ) {
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
bool Player::setSlotInitRoll(const Basic::Number* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = msg->getDouble();

      if ( value >= -(2.0*PI) && value <= (2.0*PI) ) {
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
bool Player::setSlotInitPitch(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);

      if ( value >= -(PI/2.0) && value <= (PI/2.0) ) {
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
bool Player::setSlotInitPitch(const Basic::Number* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = msg->getDouble();

      if ( value >= -(PI/2.0) && value <= (PI/2.0) ) {
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
bool Player::setSlotInitHeading(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);

      if ( value >= -PI && value <= (2.0*PI+0.001) ) {
         if (value >= 2.0*PI) value -= 2.0*PI;
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
bool Player::setSlotInitHeading(const Basic::Number* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = msg->getDouble();

      if ( value >= -PI && value <= (2.0*PI+0.001) ) {
         if (value >= 2.0*PI) value -= 2.0*PI;
         initAngles[IYAW] = value;
         ok = true;
      }
      else {
         std::cerr << "Player::setSlotInitHeading: invalid heading angle, valid range is [ -pi ... +2*pi ]" << std::endl;
      }
   }

   return ok;
}


// initEuler: Initial euler Angles: radians [ roll pitch yaw ]
bool Player::setSlotInitEulerAngles(const Basic::List* const numList)
{
   bool ok = false;
   double values[3];
   int n = numList->getNumberList(values, 3);
   if (n == 3) {
      if ( ( values[0] >= -(2.0f*PI) && values[0] <= (2.0f*PI) ) &&
         ( values[1] >= -PI && values[1] <= PI ) &&
         ( values[2] >= -(2.0f*PI) && values[2] <= (2.0f*PI) ) ) {

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
bool Player::setSlotTestRollRate(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);
      testAngRates[IROLL] = value;
      ok = true;
   }

   return ok;
}

// testPitchRate: Test pitch rate
bool Player::setSlotTestPitchRate(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);
      testAngRates[IPITCH] = value;
      ok = true;
   }

   return ok;
}

// testHeadingRate: Test heading rate
bool Player::setSlotTestYawRate(const Basic::Angle* const msg)
{
   bool ok = false;

   if (msg != 0) {
      double value = Basic::Radians::convertStatic(*msg);
      testAngRates[IYAW] = value;
      ok = true;
   }

   return ok;
}

// testBodyAxis: Test rates are in the body axis else they're euler rates (default: false)
bool Player::setSlotTestBodyAxis(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      testBodyAxis = msg->getBoolean();
      ok = true;
   }
   return ok;
}


// initVelocity: Initial Velocity: meters/second
bool Player::setSlotInitVelocity(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      initVp = msg->getReal();
      ok = true;
   }
   return ok;
}

// id: Player id  [ 1 .. 65535 ]
bool Player::setSlotID(const Basic::Number* const num)
{
   bool ok = false;
   int newID = num->getInt();
   if (newID > 0 && newID <= 65535) {
      setID( (unsigned short) newID  );
      ok = true;
   }
   else {
      std::cerr << "Player::setSlotID(): Invalid ID number: range 1 .. 65535" << std::endl;
   }
   return ok;
}

// side: Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE }
bool Player::setSlotSide(Basic::String* const msg)
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
bool Player::setSlotInitMode(Basic::String* const msg)
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
bool Player::setSlotUseCoordSys(Basic::String* const msg)
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
   if (signature != 0) {
      signature->container(0);
   }
   signature = s;
   if (signature != 0) {
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
bool Player::setSlotCamouflageType(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int ii = msg->getInt();
      if (ii >= 0) {
         ok = setCamouflageType( ii );
      }
   }
   return ok;
}

// terrainElevReq: Terrain elevation from the OTW system is requested; otherwise use DTED (default: false)
bool Player::setSlotTerrainElevReq(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setTerrainElevationRequired(num->getBoolean());
   }
   return ok;
}

// interpolateTerrain: Interpolate our DTED terrain elevation data (default: false)
bool Player::setSlotInterpolateTerrain(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setInterpolateTerrain(msg->getBoolean());
   }
   return ok;
}

// terrainOffset: Ground clamp offset from terrain to player's CG (Basic::Distance)
bool Player::setSlotTerrainOffset(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setTerrainOffset( Basic::Meters::convertStatic( *msg ) );
   }
   return ok;
}

// positionFreeze: Position freeze (default: false)
bool Player::setSlotPositionFreeze(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setPositionFreeze( num->getBoolean() );
   }
   return ok;
}

// altitudeFreeze: Altitude freeze (default: false)
bool Player::setSlotAltitudeFreeze(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setAltitudeFreeze( num->getBoolean() );
   }
   return ok;
}

// attitudeFreeze: Attitude freeze (default: false)
bool Player::setSlotAttitudeFreeze(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setAttitudeFreeze( num->getBoolean() );
   }
   return ok;
}

// fuelFreeze: Fuel freeze (default: false)
bool Player::setSlotFuelFreeze(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setFuelFreeze( num->getBoolean() );
   }
   return ok;
}

// crashOverride: Crash Override (i.e., ignore collision and crash events)(default: false)
bool Player::setSlotCrashOverride(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setCrashOverride( num->getBoolean() );
   }
   return ok;
}

// killOverride: Kill/Damage Override -- player can not be killed/damaged (default: false)
bool Player::setSlotKillOverride(const Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      ok = setKillOverride( num->getBoolean() );
   }
   return ok;
}

// killRemoval: If true destroyed players are set to KILLED and are eventually removed (default: false)
bool Player::setSlotKillRemoval(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setKillRemoval( msg->getBoolean() );
   }
   return ok;
}

// enableNetOutput: Enable network output of this player (default: true)
bool Player::setSlotEnableNetOutput(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setEnableNetOutput(msg->getBoolean());
   }
   return ok;
}

// dataLogTime: Time between player data samples to an optional data
bool Player::setSlotDataLogTime(const Basic::Time* const num)
{
   bool ok = false;
   if (num != 0) {
      dataLogTime = num->getReal();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Player::getSlotByIndex(const int si)
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
      sout << "( " << getFormName() << std::endl;
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
      sout << "initAlt: ( Feet " << (getInitAltitude() * Basic::Distance::M2FT) << " )" << std::endl;
   }

   indent(sout,i+j);
   sout << "initRoll: ( Degrees " << (initAngles[IROLL] * Basic::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "initPitch: ( Degrees " << (initAngles[IPITCH] * Basic::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "initYawRate: ( Degrees " << (initAngles[IYAW] * Basic::Angle::R2DCC) << " )" << std::endl;


   // Velocity (ft/s)
   indent(sout,i+j);
   sout << "initVelocity: ";
   sout << initVp;
   sout << std::endl;

   //  Type of player vehicle ("F-16A", "Tank", "SA-6", etc.)
   if (type != 0) {
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
   if (signature != 0) {
      indent(sout,i+j);
      sout << "signature: ";
      signature->serialize(sout,i+j+4);
   }

   // IR Signature
   if (irSignature != 0) {
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
   sout << "terrainOffset: ( Feet " << (getTerrainOffset() * Basic::Distance::M2FT) << " )" << std::endl;


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
   sout << "testRollRate: ( Degrees " << (testAngRates[IROLL] * Basic::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "testPitchRate: ( Degrees " << (testAngRates[IPITCH] * Basic::Angle::R2DCC) << " )" << std::endl;

   indent(sout,i+j);
   sout << "testHeadingRate: ( Degrees " << (testAngRates[IYAW] * Basic::Angle::R2DCC) << " )" << std::endl;

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

} // End Simulation namespace
} // End Eaagles namespace
