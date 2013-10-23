
#include "openeaagles/simulation/Gimbal.h"

#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Tdb.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Terrain.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: Gimbal
//==============================================================================

IMPLEMENT_SUBCLASS(Gimbal,"Gimbal")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Gimbal)
    "type",                   //  1: Physical gimbal type: "mechanical" or "electronic"

    "location",               //  2: Relative location vector         (meters) [ x y z ]

    "initPosition",           //  3: Initial position vector          (radians) [ az el roll ]
    "initPosAzimuth",         //  4: Initial azimuth position         (Basic::Angle)
    "initPosElevation",       //  5: Initial elevation position       (Basic::Angle)
    "initPosRoll",            //  6: Initial roll position            (Basic::Angle)

    "azimuthLimits",          //  7: Azimuth limit vector             (radians) [ left right ] [ range: -pi to pi; else unlimited ]
    "azimuthLimitLeft",       //  8: Left azimuth limit               (Basic::Angle) [ range: -pi to pi; else unlimited ]
    "azimuthLimitRight",      //  9: Right azimuth limit              (Basic::Angle) [ range: -pi to pi; else unlimited ]

    "elevationLimits",        // 10: Elevation limit vector           (radians) [ lower upper ] [ range: -pi to pi; else unlimited ]
    "elevationLimitLower",    // 11: Lower elevation limit            (Basic::Angle) [ range: -pi to pi; else unlimited ]
    "elevationLimitUpper",    // 12: Upper elevation limit            (Basic::Angle) [ range: -pi to pi; else unlimited ]

    "rollLimits",             // 13: Roll limit vector                (radians) [ lower upper ] [ range: -pi to pi; else unlimited ]
    "rollLimitLower",         // 14: Lower roll limit                 (Basic::Angle) [ range: -pi to pi; else unlimited ]
    "rollLimitUpper",         // 15: Upper roll limit                 (Basic::Angle) [ range: -pi to pi; else unlimited ]

    "maxRates",               // 16: Max "mechanical" rate vector     (rad/sec) [ az el roll ]
    "maxRateAzimuth",         // 17: Max "mechanical" azimuth rate    (Basic::Angle/sec)
    "maxRateElevation",       // 18: Max "mechanical" elevation rate  (Basic::Angle/sec)
    "maxRateRoll",            // 19: Max "mechanical" roll rate       (Basic::Angle/sec)

    "commandPosition",        // 20: Commanded position vector        (radians) [ az el roll ] (sets POSITION_SERVO)
    "commandPosAzimuth",      // 21: Commanded azimuth position       (Basic::Angle) (sets POSITION_SERVO)
    "commandPosElevation",    // 22: Commanded elevation position     (Basic::Angle) (sets POSITION_SERVO)
    "commandPosRoll",         // 23: Commanded roll position          (Basic::Angle) (sets POSITION_SERVO)

    "commandRates",           // 24: Commanded rate vector            (rad/sec) [ az el roll ] (sets RATE_SERVO)
    "commandRateAzimuth",     // 25: Commanded azimuth rate           (Basic::Angle/sec) (sets RATE_SERVO)
    "commandRateElevation",   // 26: Commanded elevation rate         (Basic::Angle/sec) (sets RATE_SERVO)
    "commandRateRoll",        // 27: Commanded roll rate              (Basic::Angle/sec) (sets RATE_SERVO)

    "terrainOcculting",             // 28: Enable terrain occulting of tye players of interest (default: false)
    "checkHorizon",                 // 29:  Enable horizon masking check (default: true)
    "playerOfInterestTypes",        // 30: Player of interest types (default: 0 )
                                    //     types: { "air" "ground" "weapon" "ship" "building" "lifeform" "space" }
    "maxPlayersOfInterest",         // 31: Max number of players of interest (default: 0)
    "maxRange2PlayersOfInterest",   // 32: Max range to players of interest or zero for all (default: 0)
    "maxAngle2PlayersOfInterest",   // 33: Max angle of gimbal boresight to players of interest or zero for all (default: 0)
    "localPlayersOfInterestOnly",   // 34: Sets the local only players of interest flag (default: false)
    "useWorldCoordinates",          // 35: Using player of interest's world (ECEF) coordinate system
    "ownHeadingOnly",               // 36: Whether only the ownship heading is used by the target data block
END_SLOTTABLE(Gimbal)

// Map slot table to handles 
BEGIN_SLOT_MAP(Gimbal)

    ON_SLOT(1, setSlotType, Basic::String)               // Physical gimbal type: "mechanical" or "electronic"

    ON_SLOT(2, setSlotLocation, Basic::List)             // Relative location vector (meters) [ x y z ]

    ON_SLOT(3, setSlotPosition, Basic::List)             // Initial position vector (radians) [ az el roll ]
    ON_SLOT(4, setSlotPosAzimuth, Basic::Angle)          // Initial azimuth position
    ON_SLOT(5, setSlotPosElevation, Basic::Angle)        // Initial elevation position
    ON_SLOT(6, setSlotPosRoll, Basic::Angle)             // Initial roll position

    ON_SLOT(7, setSlotAzimuthLimits, Basic::List)        // Azimuth limit vector (radians) [ left right ]
    ON_SLOT(8, setSlotAzimuthLimitLeft, Basic::Angle)    // Left azimuth limit
    ON_SLOT(9, setSlotAzimuthLimitRight, Basic::Angle)   // Right azimuth limit

    ON_SLOT(10, setSlotElevationLimits, Basic::List)     // Elevation limit vector (radians) [ lower upper ]
    ON_SLOT(11, setSlotElevationLower, Basic::Angle)     // Lower elevation limit
    ON_SLOT(12, setSlotElevationUpper, Basic::Angle)     // Upper elevation limit

    ON_SLOT(13, setSlotRollLimits, Basic::List)          // Roll limit vector (radians) [ lower upper ]
    ON_SLOT(14, setSlotRollLimitLower, Basic::Angle)     // Lower roll limit 
    ON_SLOT(15, setSlotRollLimitUpper, Basic::Angle)     // Upper roll limit 

    ON_SLOT(16, setSlotMaxRates, Basic::List)            // Max "mechanical" rate vector (rad/sec) [ az el roll ]
    ON_SLOT(17, setSlotMaxRateAzimuth, Basic::Angle)     // Max "mechanical" azimuth rate (Basic::Angle/sec)
    ON_SLOT(18, setSlotMaxRateElevation, Basic::Angle)   // Max "mechanical" elevation rate (Basic::Angle/sec)
    ON_SLOT(19, setSlotMaxRateRoll, Basic::Angle)        // Max "mechanical" roll rate (Basic::Angle/sec)

    ON_SLOT(20, setSlotCmdPos, Basic::List)              // Commanded position vector  [ az el roll ] (sets POSITION_SERVO)
    ON_SLOT(21, setSlotCmdPosAzimuth, Basic::Angle)      // Commanded azimuth position  (sets POSITION_SERVO)
    ON_SLOT(22, setSlotCmdPosElevation, Basic::Angle)    // Commanded elevation position (sets POSITION_SERVO)
    ON_SLOT(23, setSlotCmdPosRoll, Basic::Angle)         // Commanded roll position  (sets POSITION_SERVO)

    ON_SLOT(24, setSlotCmdRate, Basic::List)             // Commanded rate vector (rad/sec) [ az el roll ] (sets RATE_SERVO)
    ON_SLOT(25, setSlotCmdRateAzimuth, Basic::Angle)     // Commanded azimuth rate (sets RATE_SERVO)
    ON_SLOT(26, setSlotCmdRateElevation, Basic::Angle)   // Commanded elevation rate (sets RATE_SERVO)
    ON_SLOT(27, setSlotCmdRateRoll, Basic::Angle)        // Commanded roll rate (sets RATE_SERVO)

    ON_SLOT(28, setSlotTerrainOcculting, Basic::Number)  // Enable terrain occulting (default: false)
    ON_SLOT(29, setSlotCheckHorizon, Basic::Number)      // Enable horizon masking check (default: true)
    ON_SLOT(30, setSlotPlayerTypes, Basic::PairStream)   // Player of interest types (default: 0 )
                                                         //    types: { "air" "ground" "weapon" "ship" "building" "lifeform" }
    ON_SLOT(31, setSlotMaxPlayers, Basic::Number)        // Max number of players of interest (default: 0)
    ON_SLOT(32, setSlotMaxRange2PlayersOfInterest, Basic::Distance) // Max range to players of interest or zero for all (default: 0)
    ON_SLOT(33, setSlotMaxAngle2PlayersOfInterest, Basic::Angle)    // Max angle of gimbal boresight to players of interest or zero for all (default: 0)
    ON_SLOT(34, setSlotLocalPlayersOfInterestOnly, Basic::Number)   // Sets the local only players of interest flag (default: false)

    ON_SLOT(35, setSlotUseWorldCoordinates, Basic::Number)   // Using player of interest's world (ECEF) coordinate system
    ON_SLOT(36,setSlotUseOwnHeadingOnly,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Gimbal)
    ON_EVENT_OBJ(RF_EMISSION,onRfEmissionEvent,Emission)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Static variables
//------------------------------------------------------------------------------
const double Gimbal::defaultTolerance = 0.1 * (PI/180.0);

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Gimbal::Gimbal()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Gimbal::initData()
{
   type = ELECTRONIC;
   servoMode = FREEZE_SERVO;

   pos.set(0,0,0);
   rate.set(0,0,0);
   cmdPos.set(0,0,0);
   location.set(0,0,0);

   cmdRate.set((Basic::Angle::D2RCC *   0.0), (Basic::Angle::D2RCC *   0.0), (Basic::Angle::D2RCC *   0.0));
   maxRate.set((Basic::Angle::D2RCC * 120.0), (Basic::Angle::D2RCC * 120.0), (Basic::Angle::D2RCC * 120.0));

   atLimit = false;
   fastSlew = true;

   lowLimits.set( (Basic::Angle::D2RCC * -720.0), (Basic::Angle::D2RCC * -720.0), (Basic::Angle::D2RCC * -720.0));   // default: no limits
   highLimits.set((Basic::Angle::D2RCC *  720.0), (Basic::Angle::D2RCC *  720.0), (Basic::Angle::D2RCC *  720.0));    // default: no limits

   initPos = pos;
   initCmdRate = cmdRate;
   initCmdPos = cmdPos;

   maxRngPlayers = 0;
   maxAnglePlayers = 0;
   terrainOcculting = false;
   checkHorizon = true;
   localOnly = false;
   useWorld = false;
   ownHeadingOnly = true;
   playerTypes = 0xFFFF;   // all types
   maxPlayers = 200;

   tdb = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Gimbal::copyData(const Gimbal& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   type = org.type;
   servoMode = org.servoMode;

   pos = org.pos;
   rate = org.rate;
   cmdPos = org.cmdPos;
   cmdRate = org.cmdRate;
   maxRate = org.maxRate;
   lowLimits = org.lowLimits;
   highLimits = org.highLimits;
   location = org.location;
   atLimit = org.atLimit;
   fastSlew = org.fastSlew;
   initPos = org.initPos;
   initCmdRate = org.initCmdRate;
   initCmdPos = org.initCmdPos;

   maxRngPlayers = org.maxRngPlayers;
   maxAnglePlayers = org.maxAnglePlayers;
   localOnly = org.localOnly;
   terrainOcculting = org.terrainOcculting;
   checkHorizon = org.checkHorizon;
   useWorld = org.useWorld;
   ownHeadingOnly = org.ownHeadingOnly;
   playerTypes = org.playerTypes;
   maxPlayers = org.maxPlayers;

   tdb = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Gimbal::deleteData()
{
   tdb = 0;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Gimbal::reset()
{
   pos = initPos;
   cmdRate = initCmdRate;
   cmdPos = initCmdPos;
   updateMatrix();
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool Gimbal::shutdownNotification()
{
    tdb = 0;

    return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// dynamics() -- System class "Dynamics phase" call back
//------------------------------------------------------------------------------
void Gimbal::dynamics(const LCreal dt)
{
   servoController(dt);
   BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// onRfEmissionEvent() -- process events for RF Emission not sent by us.
//------------------------------------------------------------------------------
bool Gimbal::onRfEmissionEvent(Emission* const em)
{
   if (isComponentSelected()) {
      // Just pass it to our selected subcomponent
      Gimbal* sc = dynamic_cast<Gimbal*>( getSelectedComponent() );
      if (sc != 0 && sc->getPowerSwitch() != System::PWR_OFF) sc->onRfEmissionEvent(em);
   }
   else {
      // Pass it down to all of our subcomponents
      Basic::PairStream* subcomponents = getComponents();
      if (subcomponents != 0) {
         for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Gimbal* sc = dynamic_cast<Gimbal*>( pair->object() );
            if (sc != 0 && sc->getPowerSwitch() != System::PWR_OFF) sc->onRfEmissionEvent(em);
         }
         subcomponents->unref();
         subcomponents = 0;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Returns true if this is a player of interest
//------------------------------------------------------------------------------
bool Gimbal::fromPlayerOfInterest(const Emission* const em)
{
   bool ok = false;
   if (em != 0) {
      const Player* const p = em->getOwnship();
      if (p != 0) {
         ok = ((playerTypes & p->getMajorType()) != 0);
         if (ok && localOnly) {
            ok = p->isLocalPlayer();
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Returns earth radius (meters)
//------------------------------------------------------------------------------
double Gimbal::getEarthRadius() const
{
   double erad = Basic::Nav::ERAD60 * Basic::Distance::NM2M;
   const Player* own = getOwnship();
   if (own != 0) {
      erad = own->getEarthRadius();
   }

   return erad;
}

//------------------------------------------------------------------------------
// servoController() -- Gimbal's core servo model
//------------------------------------------------------------------------------
void Gimbal::servoController(const double dt)
{
   // Only if we're not frozen ...
   if (servoMode != FREEZE_SERVO) {
 
      // ---
      // Compute rate
      // ---
      osg::Vec3d rate1( 0.0f, 0.0f, 0.0f );
      if (servoMode == POSITION_SERVO) {
       
         // position servo: drive the gimbal toward the commanded position
         rate1 = cmdPos - pos;
         rate1[AZ_IDX]   = Basic::Angle::aepcdRad(rate1[AZ_IDX]);
         rate1[ELEV_IDX] = Basic::Angle::aepcdRad(rate1[ELEV_IDX]);
         rate1[ROLL_IDX] = Basic::Angle::aepcdRad(rate1[ROLL_IDX]);

         // ---
         // rate1 is radians per frame (step)
         // Limit rate1:
         //   Mechanical, fast-slew: rate is limited to maximum mechanical rate
         //   Electronic, fast-slew: rate is unlimited!
         //   Mechanical, slow-slew: rate is commanded rate limited to max mechanical rate
         //   Electronic, slow-slew: rate is commanded rate (unlimited)
         // ---
         if (isFastSlewMode() && type == MECHANICAL) {
               osg::Vec3d step = maxRate * dt;
               limitVec(rate1, step);
         }
         else if (isSlowSlewMode()) {
               osg::Vec3d cmdRate1 = cmdRate;
               if (type == MECHANICAL) {
                  limitVec(cmdRate1, maxRate);
               }
               osg::Vec3d step = cmdRate1 * dt;
               limitVec(rate1, step);
         }

         if (dt != 0.0) rate = rate1 * (1.0f/dt);
         else rate.set(0.0,0.0,0.0);
      }

      else if (servoMode == RATE_SERVO) {
         // rate servo: follow commanded rate
         rate1 = cmdRate;

         // set servo rate to limited rate
         if (type == MECHANICAL) limitVec(rate1, maxRate);
           
         rate = rate1;
      }

      // ---
      // Compute a new gimbal position
      //    newPos = pos{old} + dt*rate
      // ---
      osg::Vec3d newPos = pos + (rate*dt);
      newPos[AZ_IDX]   = Basic::Angle::aepcdRad(newPos[AZ_IDX]);
      newPos[ELEV_IDX] = Basic::Angle::aepcdRad(newPos[ELEV_IDX]);
      newPos[ROLL_IDX] = Basic::Angle::aepcdRad(newPos[ROLL_IDX]);

      // ---
      // limit gimbal's azimuth position
      //  1) lowLimits[AZ_IDX] is the left limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[AZ_IDX] is the right limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool azLimit = false;
      if (highLimits[AZ_IDX] >= -PI && highLimits[AZ_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(highLimits[AZ_IDX] - newPos[AZ_IDX]) <= 0.0f ) {
            newPos[AZ_IDX] = highLimits[AZ_IDX];
            azLimit = true;
         }
      }
      if (lowLimits[AZ_IDX] >= -PI && lowLimits[AZ_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(lowLimits[AZ_IDX] - newPos[AZ_IDX]) >= 0.0f) {
            newPos[AZ_IDX] = lowLimits[AZ_IDX];
            azLimit = true;
         }
      }

      // ---
      // limit gimbal's elevation position
      //  1) lowLimits[ELEV_IDX] is the lower limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[ELEV_IDX] is the upper limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool elLimit = false;
      if (highLimits[ELEV_IDX] >= -PI && highLimits[ELEV_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(highLimits[ELEV_IDX] - newPos[ELEV_IDX]) <= 0.0f ) {
            newPos[ELEV_IDX] = highLimits[ELEV_IDX];
            elLimit = true;
         }
      }
      if (lowLimits[ELEV_IDX] >= -PI && lowLimits[ELEV_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(lowLimits[ELEV_IDX] - newPos[ELEV_IDX]) >= 0.0f) {
            newPos[ELEV_IDX] = lowLimits[ELEV_IDX];
            elLimit = true;
         }
      }
       
      // ---
      // limit gimbal's roll position
      //  1) lowLimits[XXXX] is the counter-clockwise limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[XXXX] is the clockwise limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool rollLimit = false;
      if (highLimits[ROLL_IDX] >= -PI && highLimits[ROLL_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(highLimits[ROLL_IDX] - newPos[ROLL_IDX]) <= 0.0f ) {
            newPos[ROLL_IDX] = highLimits[ROLL_IDX];
            rollLimit = true;
         }
      }
      if (lowLimits[ROLL_IDX] >= -PI && lowLimits[ROLL_IDX] <= PI) {
         if ( Basic::Angle::aepcdRad(lowLimits[ROLL_IDX] - newPos[ROLL_IDX]) >= 0.0f) {
            newPos[ROLL_IDX] = lowLimits[ROLL_IDX];
            rollLimit = true;
         }
      }

      // update position
      atLimit = (azLimit || elLimit || rollLimit);
      pos = newPos;

   }

   // update matrix (a parent gimbal may have changed)
   updateMatrix();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
double Gimbal::getAzimuthD() const
{
   return pos[AZ_IDX] * Basic::Angle::R2DCC;
}

double Gimbal::getElevationD() const
{
   return pos[ELEV_IDX] * Basic::Angle::R2DCC;
}

double Gimbal::getRollD() const
{
   return pos[ROLL_IDX] * Basic::Angle::R2DCC;
}

void Gimbal::getAzimuthLimits(double* const leftLim, double* const rightLim) const
{
    if (leftLim != 0) *leftLim = lowLimits[AZ_IDX];
    if (rightLim != 0) *rightLim = highLimits[AZ_IDX];
}

void Gimbal::getElevationLimits(double* const lowerLim, double* const upperLim) const
{
    if (lowerLim != 0) *lowerLim = lowLimits[ELEV_IDX];
    if (upperLim != 0) *upperLim = highLimits[ELEV_IDX];
}

void Gimbal::getRollLimits(double* const lowerLim, double* const upperLim) const
{
    if (lowerLim != 0) *lowerLim = lowLimits[ROLL_IDX];
    if (upperLim != 0) *upperLim = highLimits[ROLL_IDX];
}

void Gimbal::getMaxRates(double* const azMaxRate, double* const ezMaxRate, double* const rollMaxRate) const
{
    if (azMaxRate != 0)   *azMaxRate = maxRate[AZ_IDX];
    if (ezMaxRate != 0)   *ezMaxRate = maxRate[ELEV_IDX];
    if (rollMaxRate != 0) *rollMaxRate = maxRate[ROLL_IDX];
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setType() -- sets our gimbal type
bool Gimbal::setType(const Type rt)
{
    type = rt;
    return true;
}

// setServoMode() -- Set servo mode: RATE_SERVO, POSITION_SERVO, etc.
bool Gimbal::setServoMode(const ServoMode m)        
{
    servoMode = m;
    return true;
}

// setFastSlewMode() -- Set the fast slew flag
bool Gimbal::setFastSlewMode(const bool flg)
{
   fastSlew = flg;
   return true;
}

// Max range to players of interest or zero for all (meters)
bool Gimbal::setMaxRange2PlayersOfInterest(const double meters)
{
   maxRngPlayers = meters;
   return true;
}

// Max angle of gimbal boresight to players of interest or zero for all (rad)
bool Gimbal::setMaxAngle2PlayersOfInterest(const double radians)
{
   maxAnglePlayers = radians;
   return true;
}

// Player of interest types (Player::MajorType bit-wise or'd)
bool Gimbal::setPlayerOfInterestTypes(const unsigned int typeMask)
{
   playerTypes = typeMask;
   return true;
}

// Max number of players of interest (i.e., size of the arrays)
bool Gimbal::setMaxPlayersOfInterest(const unsigned int n)
{
   maxPlayers = n;
   return true;
}

// Sets the local only players of interest flag
bool Gimbal::setLocalPlayersOfInterestOnly(const bool flg)
{
   localOnly = flg;
   return true;
}

// Sets the target terrain occulting enabled flag
bool Gimbal::setTerrainOccultingEnabled(const bool flg)
{
   terrainOcculting = flg;
   return true;
}

// Sets the horizon check enabled flag
bool Gimbal::setHorizonCheckEnabled(const bool flg)
{
   checkHorizon = flg;
   return true;
}

// Sets the using world coordinates flag
bool Gimbal::setUseWorld(const bool flg)
{
   useWorld = flg;
   return true;
}

// Sets the own heading only flag
bool Gimbal::setOwnHeadingOnly(const bool flg)
{
   ownHeadingOnly = flg;
   return true;
}


//------------------------------------------------------------------------------
// setPosition() - sets the initial azimuth and elevation position
//------------------------------------------------------------------------------
bool Gimbal::setPosition(const double azim, const double elev, const double roll)
{
    pos.set(azim, elev, roll);
    return true;
}

bool Gimbal::setPosition(const double azim, const double elev)
{
    return setPosition(azim, elev, 0);
}

//------------------------------------------------------------------------------
// setMaxRates:  set az & el rate limits
//------------------------------------------------------------------------------
bool Gimbal::setMaxRates(const double azMaxRate, const double elMaxRate, const double rollMaxRate)
{
    maxRate.set( fabs(azMaxRate), fabs(elMaxRate), fabs(rollMaxRate) );
    return true;
}

bool Gimbal::setMaxRates(const double azMaxRate, const double elMaxRate)
{
    return setMaxRates( fabs(azMaxRate), fabs(elMaxRate), 0 );
}

//------------------------------------------------------------------------------
// setAzimuthLimits() - sets our lower and upper azimuth limits
//------------------------------------------------------------------------------
bool Gimbal::setAzimuthLimits(const double leftLim, const double rightLim)
{
    lowLimits[AZ_IDX]  = leftLim;
    highLimits[AZ_IDX] = rightLim;
    return true;
}

//------------------------------------------------------------------------------
// setElevationLimits:  set lower & upper elevation limits
//------------------------------------------------------------------------------
bool Gimbal::setElevationLimits(const double lowerLim, const double upperLim)
{
    lowLimits[ELEV_IDX]  = lowerLim;
    highLimits[ELEV_IDX] = upperLim;
    return true; 
}

//------------------------------------------------------------------------------
// setRollLimits:  set lower & upper roll limits
//------------------------------------------------------------------------------
bool Gimbal::setRollLimits(const double lowerLim, const double upperLim)
{
    lowLimits[ROLL_IDX]  = lowerLim;
    highLimits[ROLL_IDX] = upperLim;
    return true; 
}

//------------------------------------------------------------------------------
// setCmdRate() -- put the gimbal servo in rate mode and set the commanded rate.
//------------------------------------------------------------------------------
bool Gimbal::setCmdRate(const osg::Vec3d& r)
{
   cmdRate = r;
   setServoMode(RATE_SERVO);
   return true;
}

bool Gimbal::setCmdRate(const osg::Vec2d& r)
{
   osg::Vec3d t;
   t[AZ_IDX]   = r[AZ_IDX];
   t[ELEV_IDX] = r[ELEV_IDX];
   t[ROLL_IDX] = 0;  // default roll to zero
   return setCmdRate(t);
}

//------------------------------------------------------------------------------
// setCmdRate() -- put the gimbal servo in rate mode and set the commanded rate.
//------------------------------------------------------------------------------
bool Gimbal::setCmdRate(const double azRate, const double elRate, const double rollRate)
{
   cmdRate.set(azRate,elRate,rollRate);
   setServoMode(RATE_SERVO);
   return true;
}

bool Gimbal::setCmdRate(const double azRate, const double elRate)
{
   return setCmdRate(azRate, elRate, 0);
}

//------------------------------------------------------------------------------
// setCmdPos() -- put the gimbal servo in position mode and set the commanded position.
//------------------------------------------------------------------------------
bool Gimbal::setCmdPos(const osg::Vec3d& p)
{
   // ---
   // Limit range from -pi to pi
   // ---
   osg::Vec3d newPos = p;
   newPos[AZ_IDX]   = Basic::Angle::aepcdRad(newPos[AZ_IDX]);
   newPos[ELEV_IDX] = Basic::Angle::aepcdRad(newPos[ELEV_IDX]);
   newPos[ROLL_IDX] = Basic::Angle::aepcdRad(newPos[ROLL_IDX]);

   // ---
   // limit commanded azimuth position
   //  1) lowLimits[AZ_IDX] is the left limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[AZ_IDX] is the right limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[AZ_IDX] >= -PI && highLimits[AZ_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(highLimits[AZ_IDX] - newPos[AZ_IDX]) <= 0.0f ) {
         newPos[AZ_IDX] = highLimits[AZ_IDX];
      }
   }
   if (lowLimits[AZ_IDX] >= -PI && lowLimits[AZ_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(lowLimits[AZ_IDX] - newPos[AZ_IDX]) >= 0.0f) {
         newPos[AZ_IDX] = lowLimits[AZ_IDX];
      }
   }

   // ---
   // limit commanded elevation position
   //  1) lowLimits[ELEV_IDX] is the lower limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[ELEV_IDX] is the upper limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[ELEV_IDX] >= -PI && highLimits[ELEV_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(highLimits[ELEV_IDX] - newPos[ELEV_IDX]) <= 0.0f ) {
         newPos[ELEV_IDX] = highLimits[ELEV_IDX];
      }
   }
   if (lowLimits[ELEV_IDX] >= -PI && lowLimits[ELEV_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(lowLimits[ELEV_IDX] - newPos[ELEV_IDX]) >= 0.0f) {
         newPos[ELEV_IDX] = lowLimits[ELEV_IDX];
      }
   }
      
   // ---
   // limit commanded roll position
   //  1) lowLimits[ROLL_IDX] is the counter-clockwise limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[ROLL_IDX] is the clockwise limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[ROLL_IDX] >= -PI && highLimits[ROLL_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(highLimits[ROLL_IDX] - newPos[ROLL_IDX]) <= 0.0f ) {
         newPos[ROLL_IDX] = highLimits[ROLL_IDX];
      }
   }
   if (lowLimits[ROLL_IDX] >= -PI && lowLimits[ROLL_IDX] <= PI) {
      if ( Basic::Angle::aepcdRad(lowLimits[ROLL_IDX] - newPos[ROLL_IDX]) >= 0.0f) {
         newPos[ROLL_IDX] = lowLimits[ROLL_IDX];
      }
   }

   // ---
   // Set the new (limited) commanded position
   // ---
   cmdPos = newPos;
   setServoMode(POSITION_SERVO);

   return true;
}

bool Gimbal::setCmdPos(const osg::Vec2d& p)
{
   osg::Vec3d t;
   t[AZ_IDX]   = p[AZ_IDX];
   t[ELEV_IDX] = p[ELEV_IDX];
   t[ROLL_IDX] = 0;  // default roll to zero
   return setCmdPos(t);
}

bool Gimbal::setCmdPos(const double az, const double el, const double roll)
{
   osg::Vec3d t;
   t[AZ_IDX]   = az;
   t[ELEV_IDX] = el;
   t[ROLL_IDX] = roll;
   return setCmdPos(t);
}

//------------------------------------------------------------------------------
// setLocation() -- set the location of the gimbal
//------------------------------------------------------------------------------
bool Gimbal::setLocation(const double x, const double y, const double z)
{
    location.set(x, y, z);
    return true;
}

//------------------------------------------------------------------------------
// Slot functions ---
//------------------------------------------------------------------------------

// setSlotType() -- calls setType()
bool Gimbal::setSlotType(const Basic::String* const msg)
{
    if (msg == 0) return false;
    
    bool ok = true;
    if (*msg == "mechanical") ok = setType(MECHANICAL);
    else if (*msg == "electronic") ok = setType(ELECTRONIC);
    else ok = false;
    return ok;
}

// setSlotLocation() - 
bool Gimbal::setSlotLocation(const Basic::List* const msg)
{
   bool ok = false;
   if (msg != 0) {
      double values[3];
      int n = msg->getNumberList(values, 3);
      if (n == 3) ok = setLocation(values[0], values[1], values[2]);
   }
   return ok;
}

// setSlotPosition() - Initial positon vector (radians) [ az el roll ]
bool Gimbal::setSlotPosition(const Basic::List* const msg)
{
   bool ok = false;
   if (msg != 0) {
      double values[3];
      int n = msg->getNumberList(values, 3);
      if (n == 2) ok = setPosition(values[0], values[1]);
      else if (n == 3) ok = setPosition(values[0], values[1], values[2]);
      if (ok) initPos = pos;
   }
    return ok;
}

// setSlotPosAzimuth() - Initial azimuth positon
bool Gimbal::setSlotPosAzimuth(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
       double az = Basic::Radians::convertStatic(*msg);
       ok = setPosition(az, getElevation(), getRoll());
       if (ok) initPos = pos;
   }
    return ok;
}

// setSlotPosElevation() - Initial elevation positon
bool Gimbal::setSlotPosElevation(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
       double el = Basic::Radians::convertStatic(*msg);
       ok = setPosition(getAzimuth(), el, getRoll());
       if (ok) initPos = pos;
   }
    return ok;
}

// setSlotPosRoll() - Initial roll positon
bool Gimbal::setSlotPosRoll(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != 0) {
       double roll = Basic::Radians::convertStatic(*msg);
       ok = setPosition(getAzimuth(), getElevation(), roll);
       if (ok) initPos = pos;
   }
    return ok;
}

// setSlotAzimuthLimits() - Azimuth limit vector (radians) [ left right ]
bool Gimbal::setSlotAzimuthLimits(const Basic::List* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double values[2];
        int n = msg->getNumberList(values, 2);
        if (n == 2) ok = setAzimuthLimits(values[0], values[1]);  // (left, right)
    }
    return ok;
}

// setSlotAzimuthLimitLeft() - Left azimuth limit
bool Gimbal::setSlotAzimuthLimitLeft(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double leftLim, rightLim;
        getAzimuthLimits(&leftLim, &rightLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setAzimuthLimits(value, rightLim);
    }
    return ok;
}

// setSlotAzimuthLimitRight() - Right azimuth limit
bool Gimbal::setSlotAzimuthLimitRight(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double leftLim, rightLim;
        getAzimuthLimits(&leftLim, &rightLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setAzimuthLimits(leftLim, value);
    }
    return ok;
}

// setSlotElevationLimits() - calls setElevationLimits()
bool Gimbal::setSlotElevationLimits(const Basic::List* const numList)
{
    bool ok = false;
    if (numList != 0) {
        double values[2];
        int n = numList->getNumberList(values, 2);
        if (n == 2) ok = setElevationLimits(values[0], values[1]);
    }
    return ok;
}

// setSlotElevationLower() - Lower elevation limit
bool Gimbal::setSlotElevationLower(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double lowerLim, upperLim;
        getElevationLimits(&lowerLim, &upperLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setElevationLimits(value, upperLim);
    }
    return ok;
}

// setSlotElevationUpper() - Upper elevation limit
bool Gimbal::setSlotElevationUpper(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double lowerLim, upperLim;
        getElevationLimits(&lowerLim, &upperLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setElevationLimits(lowerLim, value);
    }
    return ok;
}


// setSlotRollLimits() - calls setRollLimits()
bool Gimbal::setSlotRollLimits(const Basic::List* const numList)
{
    bool ok = false;
    double values[2];
    if (numList != 0) {
        int n = numList->getNumberList(values, 2);
        if (n == 2) ok = setRollLimits(values[0], values[1]);
    }
    return ok;
}

// setSlotRollLimitLower() - Lower roll limit
bool Gimbal::setSlotRollLimitLower(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double lowerLim, upperLim;
        getRollLimits(&lowerLim, &upperLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setRollLimits(value, upperLim);
    }
    return ok;
}

// setSlotRollLimitUpper() - Upper roll limit
bool Gimbal::setSlotRollLimitUpper(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double lowerLim, upperLim;
        getRollLimits(&lowerLim, &upperLim);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setRollLimits(lowerLim, value);
    }
    return ok;
}

// setSlotMaxRates() -- calls setMaxRates()
bool Gimbal::setSlotMaxRates(const Basic::List* const numList)
{
    bool ok = false;
    if (numList != 0) {
        double values[3];
        int n = numList->getNumberList(values, 3);
        if (n == 2) ok = setMaxRates(values[0], values[1]);
        else if (n == 3) ok = setMaxRates(values[0], values[1], values[2]);
    }
    
    return ok;
}

// setSlotMaxRateAzimuth() - Max "mechanical" azimuth rate (Basic::Angle/sec)
bool Gimbal::setSlotMaxRateAzimuth(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double azMaxRate, ezMaxRate, rollMaxRate;
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setMaxRates(value, ezMaxRate, rollMaxRate);
    }
    return ok;
}

// setSlotMaxRateElevation() - Max "mechanical" elevation rate (Basic::Angle/sec)
bool Gimbal::setSlotMaxRateElevation(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double azMaxRate, ezMaxRate, rollMaxRate;
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setMaxRates(azMaxRate, value, rollMaxRate);
    }
    return ok;
}

// setSlotMaxRateRoll() - Max "mechanical" roll rate (Basic::Angle/sec)
bool Gimbal::setSlotMaxRateRoll(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double azMaxRate, ezMaxRate, rollMaxRate;
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        double value = Basic::Radians::convertStatic(*msg);
        ok = setMaxRates(azMaxRate, ezMaxRate, value);
    }
    return ok;
}

// setSlotCmdPos() -- calls setCmdPos()
bool Gimbal::setSlotCmdPos(const Basic::List* const numList)
{
    bool ok = false;
    if (numList != 0) {
      double values[3];
      int n = numList->getNumberList(values, 3);
      if (n == 2) ok = setCmdPos(values[0],values[1]);
      else if (n == 3) ok = setCmdPos(values[0],values[1],values[2]);
      if (ok) initCmdPos = cmdPos;
    }

    return ok;
}

// setSlotCmdPosAzimuth - Commanded azimuth position  (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosAzimuth(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdPos(value, getCmdElev(), getCmdRoll());
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdPosElevation() - Commanded elevation position (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosElevation(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdPos(getCmdAz(), value, getCmdRoll());
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdPosRoll() - Commanded roll position  (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosRoll(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdPos(getCmdAz(), getCmdElev(), value);
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdRate() -- calls setCmdRate()
bool Gimbal::setSlotCmdRate(const Basic::List* const numList)
{
   bool ok = false;
   if (numList != 0) {
      double values[3];
      int n = numList->getNumberList(values, 3);
      if (n == 2) ok = setCmdRate(values[0],values[1]);
      else if (n == 3) ok = setCmdRate(values[0],values[1],values[2]);
      if (ok) initCmdRate = cmdRate;
   }
   return ok;
}

// setSlotCmdRateAzimuth() - Commanded azimuth rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateAzimuth(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdRate(value, getCmdElevRate(), getCmdRollRate());
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// setSlotCmdRateElevation() - Commanded elevation rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateElevation(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdRate(getCmdAzRate(), value, getCmdRollRate());
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// setSlotCmdRateRoll() - Commanded roll rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateRoll(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setCmdRate(getCmdAzRate(), getCmdElevRate(), value);
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// Enable target terrain occulting (default: false)
bool Gimbal::setSlotTerrainOcculting(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setTerrainOccultingEnabled(msg->getBoolean());
   }
   return ok;
}

// Enable horizon masking check (default: true)
bool Gimbal::setSlotCheckHorizon(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setHorizonCheckEnabled(msg->getBoolean());
   }
   return ok;
}

// Player of interest types (default: 0 )
bool Gimbal::setSlotPlayerTypes(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {
      unsigned int mask = 0;
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0) {
         const Basic::Pair* pair = (const Basic::Pair*) item->getValue();
         const Basic::String* type = dynamic_cast<const Basic::String*>( pair->object() );
         if (type != 0) {
            if ( lcStrcasecmp(*type,"air") == 0 ) { 
               mask = (mask | Player::AIR_VEHICLE);
            }
            else if ( lcStrcasecmp(*type,"ground") == 0 ) {
               mask = (mask | Player::GROUND_VEHICLE);
            }
            else if ( lcStrcasecmp(*type,"weapon") == 0 ) {
               mask = (mask | Player::WEAPON);
            }
            else if ( lcStrcasecmp(*type,"ship") == 0 ) {
               mask = (mask | Player::SHIP);
            }
            else if ( lcStrcasecmp(*type,"building") == 0 ) {
               mask = (mask | Player::BUILDING);
            }
            else if ( lcStrcasecmp(*type,"lifeform") == 0 ) {
               mask = (mask | Player::LIFE_FORM);
            }
            else if ( lcStrcasecmp(*type,"space") == 0 ) {
               mask = (mask | Player::SPACE_VEHICLE);
            }
         }
         item = item->getNext();
      }
      ok = setPlayerOfInterestTypes(mask);
   }
   return ok;
}

// Max number of players of interest (default: 0)
bool Gimbal::setSlotMaxPlayers(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setMaxPlayersOfInterest( msg->getInt() );
   }
   return ok;
}

// Max range to players of interest or zero for all (meters)
bool Gimbal::setSlotMaxRange2PlayersOfInterest(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Meters::convertStatic(*msg);
        ok = setMaxRange2PlayersOfInterest(value);
    }
    return ok;
}

// Max angle of gimbal boresight to players of interest or zero for all (rad)
bool Gimbal::setSlotMaxAngle2PlayersOfInterest(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        double value = Basic::Radians::convertStatic(*msg);
        ok = setMaxAngle2PlayersOfInterest(value);
    }
    return ok;
}

// Sets the local only players of interest flag
bool Gimbal::setSlotLocalPlayersOfInterestOnly(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setLocalPlayersOfInterestOnly(msg->getBoolean());
   }
   return ok;
}

// Using player of interest's world (ECEF) coordinate system
bool Gimbal::setSlotUseWorldCoordinates(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setUseWorld(msg->getBoolean());
   }
   return ok;
}

// Sets the own heading only flag
bool Gimbal::setSlotUseOwnHeadingOnly(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      ok = setOwnHeadingOnly(msg->getBoolean());
   }
   return ok;
}


//------------------------------------------------------------------------------
// updateMatrix() -- update the A/C coord to gimbal's coord matrix
//------------------------------------------------------------------------------
void Gimbal::updateMatrix()
{
   // Start with a rotational matrix
   osg::Matrixd mm1;
   Basic::Nav::computeRotationalMatrix( getRoll(), getElevation(), getAzimuth(), &mm1);

   // Post multiply by a translate to our location
   osg::Matrixd rr;
   rr.makeTranslate(getLocation());
   mm1 *= rr;

   // ---
   // Is our parent (container) also a gimbal?
   // If so then post multiply by our container's matrix
   // ---
   const Gimbal* p = dynamic_cast<const Gimbal*>( container() );
   if (p != 0) { mm1 *= p->getRotMat(); }

   // Set as the new matrix
   tm = mm1;
}

//------------------------------------------------------------------------------
// isPositioned() -- returns true of the gimbal has reached the commanded pos
//------------------------------------------------------------------------------
bool Gimbal::isPositioned(const double tol0) const
{
   double tol = tol0;
   if (tol < 0) tol = defaultTolerance;

   osg::Vec3d t;
   t[AZ_IDX]   = Basic::Angle::aepcdRad( pos[AZ_IDX]   - cmdPos[AZ_IDX] );
   t[ELEV_IDX] = Basic::Angle::aepcdRad( pos[ELEV_IDX] - cmdPos[ELEV_IDX] );
   t[ROLL_IDX] = Basic::Angle::aepcdRad( pos[ROLL_IDX] - cmdPos[ROLL_IDX] );
   return t.length2() < (tol*tol);
}

//------------------------------------------------------------------------------
// isAtLimits() -- Returns true if the gimbal is at a 'phyisical' limit
//------------------------------------------------------------------------------
bool Gimbal::isAtLimits() const
{
   return atLimit;
}

//------------------------------------------------------------------------------
// limitVec(Vec,lim) -- symetrical limit of a vector
//------------------------------------------------------------------------------
void Gimbal::limitVec(osg::Vec2d& vec, const osg::Vec2d& lim)
{
  double l0 = fabs(lim[0]);
  if (vec[0] >  l0)  { vec[0] =  l0; }
  if (vec[0] < -l0)  { vec[0] = -l0; }

  double l1 = fabs(lim[1]);
  if (vec[1] >  l1)  { vec[1] =  l1; }
  if (vec[1] < -l1)  { vec[1] = -l1; }
}

void Gimbal::limitVec(osg::Vec3d& vec, const osg::Vec3d& lim)
{
  double l0 = fabs(lim[0]);
  if (vec[0] >  l0)  { vec[0] =  l0; }
  if (vec[0] < -l0)  { vec[0] = -l0; }

  double l1 = fabs(lim[1]);
  if (vec[1] >  l1)  { vec[1] =  l1; }
  if (vec[1] < -l1)  { vec[1] = -l1; }

  double l2 = fabs(lim[2]);
  if (vec[2] >  l2)  { vec[2] =  l2; }
  if (vec[2] < -l2)  { vec[2] = -l2; }
}

//------------------------------------------------------------------------------
// limitVec(vec,lower,upper) -- asymetrical limit of a vector
//------------------------------------------------------------------------------
void Gimbal::limitVec(osg::Vec2d& vec, const osg::Vec2d& ll, const osg::Vec2d& ul)
{
  if (vec[0] > ul[0])  { vec[0] = ul[0]; }
  if (vec[0] < ll[0])  { vec[0] = ll[0]; }
  if (vec[1] > ul[1])  { vec[1] = ul[1]; }
  if (vec[1] < ll[1])  { vec[1] = ll[1]; }
}
void Gimbal::limitVec(osg::Vec3d& vec, const osg::Vec3d& ll, const osg::Vec3d& ul)
{
  if (vec[0] > ul[0])  { vec[0] = ul[0]; }
  if (vec[0] < ll[0])  { vec[0] = ll[0]; }
  if (vec[1] > ul[1])  { vec[1] = ul[1]; }
  if (vec[1] < ll[1])  { vec[1] = ll[1]; }
  if (vec[2] > ul[2])  { vec[2] = ul[2]; }
  if (vec[2] < ll[2])  { vec[2] = ll[2]; }
}

//------------------------------------------------------------------------------
// Process the Players-Of-Interest (POI) list
//------------------------------------------------------------------------------
unsigned int Gimbal::processPlayersOfInterest(Basic::PairStream* const poi)
{
   Tdb* tdb0 = new Tdb(maxPlayers, this);

   unsigned int ntgts = tdb0->processPlayers(poi);
   setCurrentTdb(tdb0);
   tdb0->unref();

   return ntgts;
}

//------------------------------------------------------------------------------
// Returns the current TDB (pre-ref())
//------------------------------------------------------------------------------
Tdb* Gimbal::getCurrentTDB()
{
   return tdb.getRefPtr();
}

//------------------------------------------------------------------------------
// Returns the current TDB (pre-ref())
//------------------------------------------------------------------------------
const Tdb* Gimbal::getCurrentTDB() const
{
   return tdb.getRefPtr();
}

//------------------------------------------------------------------------------
// Set the current TDB
//------------------------------------------------------------------------------
bool Gimbal::setCurrentTdb(Tdb* const newTdb)
{
   tdb = newTdb;
   return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Gimbal::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Gimbal::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFormName() << std::endl;
      j = 4;
    }

    // position [ az el ]
    indent(sout,i+j);
    sout << "position: [ ";
    sout << pos.x();
    sout << " ";
    sout << pos.y();
    sout << " ";
    sout << pos.z();
    sout << " ]" << std::endl;

    if (servoMode == RATE_SERVO) {
        // commandRates [ azRate elRate ]
        indent(sout,i+j);
        sout << "commandRates: [ ";
        sout << cmdRate.x();
        sout << " ";
        sout << cmdRate.y();
        sout << " ";
        sout << cmdRate.z();
        sout << " ]" << std::endl;
    }

    if (servoMode == POSITION_SERVO) {
        // commandPosition [ az el ]
        indent(sout,i+j);
        sout << "commandPosition: [ ";
        sout << cmdPos.x();
        sout << " ";
        sout << cmdPos.y();
        sout << " ";
        sout << cmdPos.z();
        sout << " ]" << std::endl;
    }

    // azimuthLimits [ azLower azUpper ]
    indent(sout,i+j);
    sout << "azimuthLimits: [ ";
    sout << lowLimits[AZ_IDX] << " ";
    sout << highLimits[AZ_IDX];
    sout << " ]" << std::endl;

    // elevationLimits [ elLower elUpper ]
    indent(sout,i+j);
    sout << "elevationLimits: [ ";
    sout << lowLimits[ELEV_IDX] << " ";
    sout << highLimits[ELEV_IDX];
    sout << " ]" << std::endl;

    // maxRates [ azMaxRate elMaxRate ]
    indent(sout,i+j);
    sout << "maxRates: [ ";
    sout << maxRate.x();
    sout << " ";
    sout << maxRate.y();
    sout << " ";
    sout << maxRate.z();
    sout << " ]" << std::endl;


    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
    	indent(sout,i);
    	sout << ")" << std::endl;
    }

    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
