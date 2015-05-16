
#include "openeaagles/simulation/Autopilot.h"
#include "openeaagles/simulation/DynamicsModel.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/Route.h"
#include "openeaagles/simulation/Steerpoint.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

   // default flight control attributes associated with flying a pattern
   const double Autopilot::STD_RATE_TURN_DPS   = 3.0;       // 3.0 degrees per second
   const double Autopilot::STD_MAX_BANK_ANGLE  = 30.0;      // 30.0 degrees of roll
   const double Autopilot::STD_MAX_CLIMB_RATE  = 2000.0 * (Basic::Distance::FT2M) / (Basic::Time::M2S);
   const double Autopilot::STD_MAX_PITCH_ANGLE = 10.0;      // 10.0 degrees of pitch
   const double Autopilot::STD_MAX_ACCEL_NPS   = 5.0;       // NPS


// =============================================================================
// class: Autopilot
// =============================================================================

IMPLEMENT_SUBCLASS(Autopilot,"Autopilot")

// slot table for this class type
BEGIN_SLOTTABLE(Autopilot)
   "navMode",                    //  1) Nav (route follow) mode flag (default: true)
   "holdAltitude",               //  2) Hold altitude (default: player's altitude) (Distance)
   "altitudeHoldMode",           //  3) altitude hold mode flag (default: true)
   "holdVelocityKts",            //  4) Hold velocity (Kts) (default: player's velocity) (Number)
   "velocityHoldMode",           //  5) Velocity hold mode flag (default: true)
   "holdHeading",                //  6) Hold heading (default: player's heading) (Angle)
   "headingHoldMode",            //  7) Heading hold mode flag
   "loiterMode",                 //  8) Loiter mode flag (default: false)
   "loiterPatternLength",        //  9) Loiter pattern length (default: 10NM)    (Distance or nautical miles)
   "loiterPatternCcwFlag",       // 10) Loiter pattern counter-clockwise flag    (default: clockwise)
   "leadFollowingDistanceTrail", // 11) Desired distance behind(+) the lead      (Distance or meters)
   "leadFollowingDistanceRight", // 12) Desired distance right(+) of the lead    (Distance or meters)
   "leadFollowingDeltaAltitude", // 13) Desired delta altitude above(+) the lead (Distance or meters)
   "leadPlayerName",             // 14) Name of our lead player                  (Identifier)
   "followTheLeadMode",          // 15)"Follow the lead" mode flag               (Number)
   "maxRateOfTurnDps",           // 16) Maximum rate of turn                     (Number) - degrees per second
   "maxBankAngle",               // 17) Maximum bank angle                       (Number) - degrees
   "maxClimbRateFpm",            // 18) Maximum climb / dive rate                (Number) - feet per minute
   "maxClimbRateMps",            // 19) Maximum climb / dive rate                (Number) - meters per second (no conversion required)
   "maxPitchAngle",              // 20) Maximum pitch angle                      (Number) - degrees
   "loiterPatternTime",          // 21) Loiter pattern time                      (Time)
   "maxAcceleration",            // 22) Maximum velocity acceleration            (Number) - NPS
END_SLOTTABLE(Autopilot)

//  Map slot names to handlers
BEGIN_SLOT_MAP(Autopilot)
    ON_SLOT( 1, setSlotNavMode,                    Basic::Number)
    ON_SLOT( 2, setSlotHoldAltitude,               Basic::Distance)
    ON_SLOT( 3, setSlotAltitudeHoldMode,           Basic::Number)
    ON_SLOT( 4, setSlotHoldVelocityKts,            Basic::Number)
    ON_SLOT( 5, setSlotVelocityHoldMode,           Basic::Number)
    ON_SLOT( 6, setSlotHoldHeading,                Basic::Angle)
    ON_SLOT( 7, setSlotHeadingHoldMode,            Basic::Number)
    ON_SLOT( 8, setSlotLoiterMode,                 Basic::Number)
    ON_SLOT( 9, setSlotLoiterPatternLength,        Basic::Distance)
    ON_SLOT( 9, setSlotLoiterPatternLength,        Basic::Number)
    ON_SLOT(10, setSlotLoiterPatternCcwFlag,       Basic::Number)
    ON_SLOT(11, setSlotLeadFollowingDistanceTrail, Basic::Distance)
    ON_SLOT(11, setSlotLeadFollowingDistanceTrail, Basic::Number)
    ON_SLOT(12, setSlotLeadFollowingDistanceRight, Basic::Distance)
    ON_SLOT(12, setSlotLeadFollowingDistanceRight, Basic::Number)
    ON_SLOT(13, setSlotLeadFollowingDeltaAltitude, Basic::Distance)
    ON_SLOT(13, setSlotLeadFollowingDeltaAltitude, Basic::Number)
    ON_SLOT(14, setSlotLeadPlayerName,             Basic::Identifier)
    ON_SLOT(15, setSlotFollowTheLeadMode,          Basic::Number)
    ON_SLOT(16, setSlotMaxRateOfTurnDps,           Basic::Number)
    ON_SLOT(17, setSlotMaxBankAngle,               Basic::Number)
    ON_SLOT(18, setSlotMaxClimbRateFpm,            Basic::Number)
    ON_SLOT(19, setSlotMaxClimbRateMps,            Basic::Number)
    ON_SLOT(20, setSlotMaxPitchAngle,              Basic::Number)
    ON_SLOT(21, setSlotLoiterPatternTime,          Basic::Time)
    ON_SLOT(22, setSlotMaxVelAccNps,               Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Autopilot::Autopilot()
{
   STANDARD_CONSTRUCTOR()

   stickRollPos   = 0.0;
   stickPitchPos  = 0.0;

   for (unsigned int i = 0; i < MAX_THR; i++) {
      thrPos[i] = 0;
   }
   nThrPos = 0;

   cmdHdg         = 0.0;
   cmdAlt         = 0.0;
   cmdSpd         = 0.0;

   holdHdgSet     = false;
   holdAltSet     = false;
   holdSpdSet     = false;

   rollSasOn      = true;
   pitchSasOn     = true;
   yawSAsOn       = true;
   hdgHoldOn      = true;
   altHoldOn      = true;
   spdHoldOn      = true;
   navModeOn      = true;
   loiterModeOn   = false;

   loiterAnchorLat = 0.0;
   loiterAnchorLon = 0.0;
   loiterMirrorLat = 0.0;
   loiterMirrorLon = 0.0;
   //loiterState     = 0;
   loiterLength    = 10.0;
   loiterInboundCourse = 0.0;
   loiterCcwFlag   = false;
   loiterEntryMode  = PREENTRY;
   loiterEntryPhase = 0;
   isInbound = false;
   loiterTime = 0.0;
   loiterTimeBased = false;

   setLeadFollowingDistanceTrail( Basic::Distance::NM2M );             // Default: 1 NM trail
   setLeadFollowingDistanceRight( Basic::Distance::NM2M );             // Default: 1 NM right
   setLeadFollowingDeltaAltitude( -2000.0f * Basic::Distance::FT2M );  // Default: 2000ft below

   lead = nullptr;
   leadName = nullptr;
   leadHdg = 0.0;
   followLeadModeOn = false;
   maxTurnRateDps = STD_RATE_TURN_DPS;
   maxBankAngleDegs = STD_MAX_BANK_ANGLE;
   maxClimbRateMps = STD_MAX_CLIMB_RATE;
   maxPitchAngleDegs = STD_MAX_PITCH_ANGLE;
   maxVelAccNps = STD_MAX_ACCEL_NPS;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Autopilot::copyData(const Autopilot& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      lead = nullptr;
      leadName = nullptr;
   }

   stickRollPos  = org.stickRollPos;
   stickPitchPos = org.stickPitchPos;

   for (unsigned int i = 0; i < MAX_THR; i++) {
      thrPos[i] = org.thrPos[i];
   }
   nThrPos = org.nThrPos;

   cmdHdg = org.cmdHdg;
   cmdAlt = org.cmdAlt;
   cmdSpd = org.cmdSpd;

   holdHdgSet = org.holdHdgSet;
   holdAltSet = org.holdAltSet;
   holdSpdSet = org.holdSpdSet;

   rollSasOn    = org.rollSasOn;
   pitchSasOn   = org.pitchSasOn;
   yawSAsOn     = org.yawSAsOn;
   hdgHoldOn    = org.hdgHoldOn;
   altHoldOn    = org.altHoldOn;
   spdHoldOn    = org.spdHoldOn;
   navModeOn    = org.navModeOn;
   loiterModeOn = org.loiterModeOn;

   loiterAnchorLat = org.loiterAnchorLat;
   loiterAnchorLon = org.loiterAnchorLon;
   loiterMirrorLat = org.loiterMirrorLat;
   loiterMirrorLon = org.loiterMirrorLon;
   //loiterState = org.loiterState;
   loiterLength = org.loiterLength;
   loiterCcwFlag = org.loiterCcwFlag;
   loiterInboundCourse = org.loiterInboundCourse;
   loiterEntryMode  = org.loiterEntryMode;
   loiterEntryPhase = org.loiterEntryPhase;
   isInbound = org.isInbound;
   loiterTime = org.loiterTime;
   loiterTimeBased = org.loiterTimeBased;

   leadOffset = org.leadOffset;
   setLeadPlayer( org.lead );
   setSlotLeadPlayerName( org.leadName );
   followLeadModeOn =  org.followLeadModeOn;
   leadHdg = org.leadHdg;
   maxTurnRateDps = org.maxTurnRateDps;
   maxBankAngleDegs = org.maxBankAngleDegs;
   maxClimbRateMps = org.maxClimbRateMps;
   maxPitchAngleDegs = org.maxPitchAngleDegs;
   maxVelAccNps = org.maxVelAccNps;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Autopilot::deleteData()
{
   setLeadPlayer(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool Autopilot::shutdownNotification()
{
   setLeadPlayer(nullptr);
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Autopilot::reset()
{
    BaseClass::reset();

    Player* pv = getOwnship();
    if (pv != nullptr) {
        // If heading, altitude or velocity hold modes are set and their
        // hold values were not set by a slot function, then use the player's
        // current values.
        if (hdgHoldOn && !holdHdgSet) cmdHdg = static_cast<LCreal>(pv->getHeadingD());
        if (altHoldOn && !holdAltSet) cmdAlt = static_cast<LCreal>(pv->getAltitudeFt());
        if (spdHoldOn && !holdSpdSet) cmdSpd = pv->getTotalVelocityKts();
    }
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void Autopilot::process(const LCreal dt)
{
   modeManager();
   headingController();
   altitudeController();
   velocityContoller();

   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// Manage the modes
//------------------------------------------------------------------------------
bool Autopilot::modeManager()
{
   // ---
   // Re-latch the modes -- (just command the previous mode.)
   //  If the mode was off, it still is.
   //  If the mode was on, the 'is' functions will make sure that all
   //  prerequisite are still met.
   // ---
   setNavMode( isNavModeOn() );
   if (!isLoiterModeOn()) {
      //loiterState = 0;
      loiterEntryMode = PREENTRY;
      loiterEntryPhase = 0;
   }

   // ---
   // Follow our leader mode
   // ---
   if ( isFollowTheLeadModeOn() ) {
      processModeFollowTheLead();
   }

   // ---
   // Loiter Mode
   // ---
   else if ( isLoiterModeOn() ) {
      processModeLoiter();
   }

   // ---
   // Navigation (e.g., waypoint follow) Mode
   // ---
   else if ( isNavModeOn() ) {
      processModeNavigation();
   }

   return true;
}


//------------------------------------------------------------------------------
// processModeNavigation() -- Process for the "navigation system" mode
//------------------------------------------------------------------------------
bool Autopilot::processModeNavigation()
{
   bool ok = false;

   const Navigation* nav = getOwnship()->getNavigation();
   if (nav != nullptr) {
      // Do we have valid NAV steering data?
      if (nav->isNavSteeringValid()) {
         const LCreal a = nav->getTrueBrgDeg();
         setCommandedHeadingD( a );
      }

      // Do we have NAV commanded altitude?
      const Route* route = nav->getPriRoute();
      if (route != nullptr) {
         const Steerpoint* sp = route->getSteerpoint();
         if (sp != nullptr) {
            if (sp->isCmdAltValid()) {
               setCommandedAltitudeFt( sp->getCmdAltitudeFt() );
            }
            {
               const LCreal spd = sp->getCmdAirspeedKts();
               if (spd > 0) {
                  setCommandedVelocityKts( spd );
               }
            }
         }
      }
      ok = true;
   }

   if (!ok) setNavMode( false );
   return ok;
}

//------------------------------------------------------------------------------
// processModeLoiter() -- Process for the "loiter" mode
//------------------------------------------------------------------------------
bool Autopilot::processModeLoiter()
{
   // fly entry
   if (loiterEntryMode != LOITER) flyLoiterEntry();
   else flyLoiter();

   return true;
}

bool Autopilot::flyLoiterEntry()
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   Player* pPlr = getOwnship();
   bool ok = (pPlr != nullptr);
   if (ok) {

      Navigation* nav = pPlr->getNavigation();
      bool haveNav = (nav != nullptr);

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double MAX_BANK_RAD = maxBankAngleDegs * Basic::Angle::D2RCC;
//      const double OFFSET_MTR   = 20.0;

      //----------------------------------------------------
      // get current data - try to get it from the navigation, otherwise from
      // the actual player
      //----------------------------------------------------
      double hdgDeg = 0.0;
      double osLatDeg = 0.0;
      double osLonDeg = 0.0;
      if (haveNav) {
         hdgDeg = nav->getHeadingDeg();
         // make sure the nav data is valid
         if (nav->isPositionDataValid()) {
            osLatDeg = nav->getLatitude();
            osLonDeg = nav->getLongitude();
         }
         else {
            osLatDeg = pPlr->getLatitude();
            osLonDeg = pPlr->getLongitude();
         }
      }
      else {
         hdgDeg = pPlr->getHeadingD();
         osLatDeg  = pPlr->getLatitude();
         osLonDeg  = pPlr->getLongitude();
      }

      // Player only data
      const double velMps    = pPlr->getTotalVelocity();
      const double rocMtr    = velMps * velMps / Eaagles::ETHGM / std::tan(MAX_BANK_RAD);
      const double rocNM     = rocMtr * Basic::Distance::M2NM;
      const double obCrsDeg  = Basic::Angle::aepcdDeg(loiterInboundCourse + 180.0);

      // this is for flying to the loiter first, go back to that
      double brgDeg = 0.0;
      double distNM = 0.0;
      Basic::Nav::fll2bd(osLatDeg, osLonDeg, loiterAnchorLat, loiterAnchorLon, &brgDeg, &distNM);

      const double hdgErrDeg = Basic::Angle::aepcdDeg(hdgDeg - loiterInboundCourse);
//      double posErrDeg = Basic::Angle::aepcdDeg(brgDeg - loiterInboundCourse);
      double distCmdNM = 0.0;
      double hdgCmdDeg = 0.0;

      switch (loiterEntryMode) {
         //-----------------------------------------------------------
         case PREENTRY:
         {
            if (loiterEntryPhase == 0) {
               // just started loitering, let's setup our anchor and inbound course
               if (haveNav) {
                  loiterInboundCourse = nav->getHeadingDeg();
                  // make sure the nav data is valid
                  if (nav->isPositionDataValid()) {
                     loiterAnchorLat = nav->getLatitude();
                     loiterAnchorLon = nav->getLongitude();
                  }
                  else {
                     loiterAnchorLat = pPlr->getLatitude();
                     loiterAnchorLon = pPlr->getLongitude();
                  }
               }
               else {
                  loiterInboundCourse = pPlr->getHeadingD();
                  loiterAnchorLat = pPlr->getLatitude();
                  loiterAnchorLon = pPlr->getLongitude();
               }

               // assume direct entry for now
               loiterEntryPhase = 1;
               loiterEntryMode = DIRECT;

               // SLS - for flying to loiter - implement later
               //ok = fly2LL(anchorLat, anchorLon);
               if (distNM < 0.1) {
                  loiterEntryPhase = 1;
                  if (!loiterCcwFlag) {
                     if      (hdgErrDeg < -70.0)  { loiterEntryMode = PARALLEL; }
                     else if (hdgErrDeg < 110.0)  { loiterEntryMode = DIRECT; }
                     else                         { loiterEntryMode = TEARDROP; }
                  }
                  else {
                     if      (hdgErrDeg < -110.0) { loiterEntryMode = TEARDROP; }
                     else if (hdgErrDeg <  70.0)  { loiterEntryMode = DIRECT; }
                     else                         { loiterEntryMode = PARALLEL; }
                  }
               }
            }
         }
         break;  // end case PREENTRY

         //-----------------------------------------------------------
         case DIRECT:
         {
            if (loiterEntryPhase == 1) {
               // fly our standard rate of turn to the next point
               ok = flySRT();
               calcMirrorLatLon();
               if (std::fabs(hdgErrDeg) > 90.0) {
                  loiterEntryMode = LOITER;
                  loiterEntryPhase = 0;
               }
            }
         }
         break;  // end case DIRECT

         //-----------------------------------------------------------
         case PARALLEL:
         {
            if (loiterEntryPhase == 1) {
               setCommandedHeadingD(obCrsDeg);
               //distCmdNM = velMps * 60.0 * Basic::Distance::M2NM;  // 1 min = 60 sec
               distCmdNM = 4.0 * rocNM;  // 1/tan(15 deg) = 0.268 = 3.732 ~= 4.0
               if (distNM > distCmdNM) loiterEntryPhase = 2;
            }
            else if (loiterEntryPhase == 2) {
               flySRT();
               if (std::fabs(hdgErrDeg) < 90.0) {
                  loiterEntryPhase = 3;
               }
            }
            else if (loiterEntryPhase == 3) {
               ok = flyCRS(loiterAnchorLat, loiterAnchorLon, loiterInboundCourse);
               if (distNM < 0.1) {
                  loiterEntryMode = LOITER;
                  loiterEntryPhase = 0;
               }
            }
         }
         break;  // end case PARALLEL

         case TEARDROP:
         {
            if (loiterEntryPhase == 1) {
               if (!loiterCcwFlag)  { hdgCmdDeg = Basic::Angle::aepcdDeg(obCrsDeg - 30.0); }
               else                 { hdgCmdDeg = Basic::Angle::aepcdDeg(obCrsDeg + 30.0); }
               setCommandedHeadingD(hdgCmdDeg);
               //distCmdNM = velMps * 60.0 * Basic::Distance::M2NM;  // 1 min = 60 sec
               distCmdNM = 4.0 * rocNM;  // 1/tan(15 deg) = 0.268 = 3.732 ~= 4.0
               if (distNM > distCmdNM) { loiterEntryPhase = 2; }
            }
            else if (loiterEntryPhase == 2) {
               flySRT();
               if (std::fabs(hdgErrDeg) < 90.0) {
                  loiterEntryPhase = 3;
               }
            }
            else if (loiterEntryPhase == 3) {
               flyCRS(loiterAnchorLat, loiterAnchorLon, loiterInboundCourse);
               if (distNM < 0.1) {
                  loiterEntryMode = LOITER;
                  loiterEntryPhase = 0;
               }
            }
         }
         break;  // end case TEARDROP

         //-----------------------------------------------------------
         case LOITER:
         {
            flyLoiter();
         }
         break;  // end case LOITER

         //-----------------------------------------------------------
         default:
            break;
      }  // end switch
   }

   return ok;
}

bool Autopilot::flyLoiter()
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   Player* pPlr = getOwnship();

   bool ok = (pPlr != nullptr);
   if (ok) {

      // get current data
      const double osLatDeg = pPlr->getLatitude();
      const double osLonDeg = pPlr->getLongitude();
      const double obCrs = Basic::Angle::aepcdDeg(loiterInboundCourse + 180.0);
      calcMirrorLatLon();

      double brgDeg = 0.0;
      double distNM = 0.0;
      if (isInbound) {
         ok = Basic::Nav::fll2bd(osLatDeg, osLonDeg, loiterAnchorLat, loiterAnchorLon, &brgDeg, &distNM);
      }
      else {
         ok = Basic::Nav::fll2bd(osLatDeg, osLonDeg, loiterMirrorLat, loiterMirrorLon, &brgDeg, &distNM);
      }

      // get position error to determine inbound or outbound
      double posErrDeg = 0.0;
      if (isInbound) {
         ok = flyCRS(loiterAnchorLat, loiterAnchorLon, loiterInboundCourse);
         posErrDeg = Basic::Angle::aepcdDeg(brgDeg - loiterInboundCourse);
         if (std::fabs(posErrDeg) > 90.0) { isInbound = false; }
      }
      else {
         ok = flyCRS(loiterMirrorLat, loiterMirrorLon, obCrs);
         posErrDeg = Basic::Angle::aepcdDeg(brgDeg - obCrs);
         if (std::fabs(posErrDeg) > 90.0) { isInbound = true; }
      }
   }
   return true;
}

// ---
// calcMirrorLatLon() - from our loiter anchor, this will calculate the mirror lat/lon position
// ---
bool Autopilot::calcMirrorLatLon()
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   Player* pPlr = getOwnship();

   bool ok = (pPlr != nullptr);
   if (ok) {

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double MAX_BANK_RAD  = maxBankAngleDegs * Basic::Angle::D2RCC;
      const double SRT_RPS       = STD_RATE_TURN_DPS * Basic::Angle::D2RCC;

      //----------------------------------------------------
      // get current data
      //----------------------------------------------------
      const double velMps = pPlr->getTotalVelocity();
      double phiCmdRad = std::atan2(velMps * SRT_RPS, Eaagles::ETHGM);
      if (phiCmdRad > MAX_BANK_RAD) { phiCmdRad = MAX_BANK_RAD; }

      // radius of the center (nautical miles)
      const double rocMtr    = velMps * velMps / (Eaagles::ETHGM * std::tan(phiCmdRad));
      const double rocNM     = rocMtr * Basic::Distance::M2NM;
      const double xtDistNM  = 2.0 * rocNM;

      // do our outbound distance based on time or length
      // default to 2 minutes
      double obTimeSec = 2.0 * Basic::Time::M2S;
      double obDistNM = 0;
      if (loiterTimeBased) {
         obTimeSec = loiterTime;
         obDistNM  = velMps * obTimeSec * Basic::Distance::M2NM;
      }
      else obDistNM = loiterLength;

      // SLS - ask Larry about this
      //double altFt     = pPlr->getAltitudeFt();
      //if (altFt > 14000.0) { obDistNM *= 1.5; }

      const double distNM    = std::sqrt(xtDistNM*xtDistNM + obDistNM*obDistNM);
      const double obCrsDeg  = Basic::Angle::aepcdDeg(loiterInboundCourse + 180.0);
      double brgDeg          = std::atan2(xtDistNM, obDistNM) * Basic::Angle::R2DCC;

      // turning clockwise
      if (!loiterCcwFlag) brgDeg = Basic::Angle::aepcdDeg(obCrsDeg - brgDeg);
      else brgDeg = Basic::Angle::aepcdDeg(obCrsDeg + brgDeg);

      ok = Basic::Nav::fbd2ll(loiterAnchorLat, loiterAnchorLon, brgDeg, distNM, &loiterMirrorLat, &loiterMirrorLon);
   }

   return ok;
}

// ---
// flyCRS() - flies to a given lat/lon using the inbound course
// ---
bool Autopilot::flyCRS(const double latDeg, const double lonDeg, const double crsDeg)
{
   //-------------------------------------------------------

   // get data pointers
   //-------------------------------------------------------
   Player* pPlr = getOwnship();

   bool ok = (pPlr != nullptr);
   if (ok) {

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double MAX_BANK_RAD = maxBankAngleDegs * Basic::Angle::D2RCC;
      const double OFFSET_MTR   = 20.0;

      //----------------------------------------------------
      // get current data
      //----------------------------------------------------
      const double velMps = pPlr->getTotalVelocity();
      const double hdgDeg = pPlr->getHeadingD();
      const double osLat  = pPlr->getLatitude();
      const double osLon  = pPlr->getLongitude();
      double brgDeg = 0.0;
      double distNM = 0.0;
      Basic::Nav::fll2bd(osLat, osLon, latDeg, lonDeg, &brgDeg, &distNM);

      //-------------------------------------------------------
      // get current gama error (deg)
      //-------------------------------------------------------
      const double posErrDeg = Basic::Angle::aepcdDeg(brgDeg - crsDeg);
      const double posErrRad = posErrDeg * Basic::Angle::D2RCC;

      const double rocMtr    = velMps * velMps / Eaagles::ETHGM / std::tan(MAX_BANK_RAD);
      //double rocNM     = rocMtr * Basic::Distance::M2NM;

      const double xtRngNM   = std::fabs(distNM * std::sin(posErrRad));
      const double xtRngMtr  = xtRngNM * Basic::Distance::NM2M;
      const double xtRngRoc  = xtRngMtr / rocMtr;

      double hdgCmdDeg = hdgDeg;
      if (xtRngRoc >= 1.2) {
         hdgCmdDeg = sign(posErrDeg) * 90.0 + crsDeg;
      }
      else {
         double x = 1.0 - xtRngRoc;
         if (x >  1.0) x =  1.0;
         if (x < -1.0) x = -1.0;
         const double alfaDeg = std::acos(x) * Basic::Angle::R2DCC;

         const double y = (rocMtr - OFFSET_MTR) / rocMtr;
         const double betaDeg = std::acos(y) * Basic::Angle::R2DCC;

         const double gamaDeg = sign(posErrDeg) * (alfaDeg - betaDeg);
         hdgCmdDeg = Basic::Angle::aepcdDeg(gamaDeg + crsDeg);
      }

      //-------------------------------------------------------
      // fly to heading necessary to follow commanded course
      //-------------------------------------------------------
      ok = setCommandedHeadingD(hdgCmdDeg);
   }

   return ok;
}


// ---
// flySRT() - the pilot will try to fly the given heading using a standard rate of turn
// ---
bool Autopilot::flySRT()
{
   setCommandedVelocityKts(210.0f);
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   Player* pPlr = getOwnship();

   bool ok = (pPlr != nullptr);
   if (ok) {

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double SRT_RPS       = STD_RATE_TURN_DPS * Basic::Angle::D2RCC;

      //----------------------------------------------------
      // get current data
      //----------------------------------------------------
      const double velMps    = pPlr->getTotalVelocity();
      double phiCmdRad = std::atan2(velMps*SRT_RPS, Eaagles::ETHGM);
      double psiDotCmdRps = Eaagles::ETHGM * std::tan(phiCmdRad) / velMps;

      // turn left
      if (loiterCcwFlag) {
         phiCmdRad    = -phiCmdRad;
         psiDotCmdRps = -psiDotCmdRps;
      }

      const double hdg = pPlr->getHeadingD();
      // set our commanded heading as our current heading + the heading to get a standard rate of turn
      const double phiCmdDeg = phiCmdRad * Basic::Angle::R2DCC;
      setCommandedHeadingD(hdg + phiCmdDeg);
   }

   return ok;
}


//------------------------------------------------------------------------------
// processModeFollowTheLead() -- Process for the "follow the lead" mode
//------------------------------------------------------------------------------
bool Autopilot::processModeFollowTheLead()
{
   bool ok = false;

   // attempt to get our lead player, if we don't have one initially
   if (lead == nullptr) getLeadPlayer();

   // if we got a lead player, great... if not, we break out of follow the lead mode.
   if (lead != nullptr && lead->isActive() && lead->getDamage() < 0.5) {

      // Position error gains
      static const double KX = 0.05;
      static const double KY = 0.05;

      // Velocity limits (m/s)
      static const double T_VPMAX = 350.0;
      static const double T_VPMIN = 150.0;

      // ---
      // Get the heading of the lead player --
      // ---
      {
         // To keep from chasing small changes, let's filter
         // our lead's heading

         // ... first check for angle wrap around.
         double newHdg = Basic::Angle::aepcdRad( lead->getHeading() );
         if ( (leadHdg - newHdg) >  PI) newHdg += (2.0 * PI);
         if ( (leadHdg - newHdg) < -PI) newHdg -= (2.0 * PI);

         // ... then filter it
         const double leadHdg0 = 0.98 * leadHdg + 0.02 * newHdg;
         leadHdg = Basic::Angle::aepcdRad( leadHdg0 );
      }

      // ---
      // Compute sin/cos of new heading
      // ---
      const double aa1 = std::sin(leadHdg);
      const double aa2 = std::cos(leadHdg);

      // ---
      // Compute displacement vector
      // ---
      const osg::Vec3d posLead = lead->getPosition();
      const osg::Vec3d posOwn  = getOwnship()->getPosition();
      const double xi = posLead.x() - posOwn.x();
      const double yi = posLead.y() - posOwn.y();

      // ---
      // Rotate to lead's (heading) coord system
      // ---
      const double dx =  xi * aa2 + yi * aa1;
      const double dy = -xi * aa1 + yi * aa2;

      // ---
      // Translate to station keeping point
      // ---
      const double dx0 = dx + leadOffset.x();
      const double dy0 = dy + leadOffset.y();

      // ---
      // Compute cmd velocity vel (lead's coord sys) ...
      // ... match leaders velocity vector [ vp  0 ] plus
      //     a delta velocity vector for position errors.
      // ---
      const double vx = lead->getTotalVelocity() + KX * dx0;
      const double vy = 0 + KY * dy0;

      // ---
      // Compute total velocity
      // ---
      double vt = std::sqrt(vx*vx + vy*vy);

      // ---
      // Compute normalized velocity vector giving us only
      // the direction we want to go in lead's coord sys.
      // ---
      double ux = 1.0;
      double uy = 0.0;
      if (vt > 0) {
         ux = vx/vt;
         uy = vy/vt;
      }

      // ---
      // limit to aircraft's capabilities
      // ---
      if (vt > T_VPMAX)  vt = T_VPMAX;
      if (vt < T_VPMIN)  vt = T_VPMIN;

      // ---
      // Compute commanded heading, velocity and altitude
      // ---
      const double dhdg = std::atan2(uy,ux);
      const double chhdg = Basic::Angle::aepcdRad(dhdg + leadHdg);
      setCommandedHeadingD( chhdg * Basic::Angle::R2DCC );

      const double calt = lead->getAltitude() + (-leadOffset.z());
      setCommandedAltitudeFt( calt * Basic::Distance::M2FT );

      const double vKts = vt * Basic::Time::H2S / Basic::Distance::NM2M;
      setCommandedVelocityKts( vKts );
      ok = true;
   }

   if (!ok) setFollowTheLeadMode(false);
   return ok;
}

//------------------------------------------------------------------------------
// orbitwp() -- computes the orbit holding pattern's 'mirror' waypoint
//  where:
//     alat, alon    -- Anchor point latitude & longitude (degs)
//     vp            -- Vehicle 'true' ground speed (m/s)
//     length        -- Orbit length (meters)
//     ccwFlg        --
//     rlat, rlon    -- Result: mirror point latitude & longitude (degs)
//------------------------------------------------------------------------------
//bool Autopilot::computerOrbitHoldingPatternMirrorWaypoint(
//            const double alat,      // In:   Anchor point latitude (degs)
//            const double alon,      // In:   Anchor' point longitude (degs)
//            const double crs,       // In:   In-bound 'true' course (degs)
//            const double length,    // In:   Pattern length (nm)
//            const double speed,     // In:   True airspeed speed (kts)
//            const bool ccwFlg,      // In:   True if counter-clockwise orbit (else clockwise orbit)
//            double* const mlat,     // Out:  Mirror point latitude (degs)
//            double* const mlon      // Out:  Mirror point longitude (degs)
//            )
//{
//   bool ok = false;
//   if (mlat != 0 && mlon != 0) {
//
//      // Turn radius (ft)
//      //double radius  = 0.053833866 * speed * speed;
//
//      // Offset (width) of the orbit pattern (ft)
//      //double orbitOffsetFt  = 2.0 * radius;
//
//      const double AOG = 32.174;  // Acceleration of gravity, 32.174 feet/sec/sec
//      const double SRT = 3.0;     // Standard rate turn, 3 degrees/sec (also, 180 deg/min)
//
//      double airSpeedFps   = speed * Basic::Distance::NM2FT / Basic::Time::H2S;  // airspeed  [feet/sec]
//      double turnRateRps   = SRT * Basic::Angle::D2RCC;                          // standard rate turn  [radians/sec]
//      double tanBankAngle  = airSpeedFps * turnRateRps / AOG;                    // standard rate turn tan(bank angle)  [no units]
//      double radiusFT      = airSpeedFps * airSpeedFps / AOG / tanBankAngle;     // standard rate turn radius  [feet]
//      //double bankAngleDeg  = std::atan(tanBankAngle) * Basic::Angle::R2DCC;      // standard rate turn bank angle  [degrees]
//      double orbitOffsetFt = 2.0 * radiusFT;
//
//      // ---
//      // Pattern length (ft)
//      //  -- must be at least 1 nm greater than the offset
//
//      double orbitLengthFt = length * Basic::Distance::NM2FT;
//      const double lengthLowerLimit = orbitOffsetFt + Basic::Distance::NM2FT;
//      if (orbitLengthFt < lengthLowerLimit) {
//         orbitLengthFt = lengthLowerLimit;
//      }
//
//      // ---
//      // Bearing to mirror point (degs)
//      // ---
//      double relBrgDeg = std::atan2(orbitOffsetFt, -orbitLengthFt) * Basic::Angle::R2DCC;
//      double trueBrgDeg = 0;
//      if (ccwFlg) {
//         // Counter-clockwise (left) turn
//         trueBrgDeg = Basic::Angle::aepcdDeg( crs - relBrgDeg );
//      }
//      else {
//         // Clockwise (right) turn
//         trueBrgDeg = Basic::Angle::aepcdDeg( crs + relBrgDeg );
//      }
//
//
//      // ---
//      // Distance to mirror point (nm)
//      // ---
//      double distFt = sqrt( orbitLengthFt*orbitLengthFt + orbitOffsetFt*orbitOffsetFt );
//      double distNM = distFt * Basic::Distance::FT2NM;
//
//      // ---
//      // Calculate mirror point
//      // ---
//      double lat2, lon2;
//      Basic::Nav::gbd2ll(alat, alon, trueBrgDeg, distNM, &lat2, &lon2);
//      *mlat = lat2;
//      *mlon = lon2;
//
//      ok = true;
//   }
//   return ok;
//}


//------------------------------------------------------------------------------
// Heading/roll controller --
//------------------------------------------------------------------------------
bool Autopilot::headingController()
{
   // Re-latch the mode
   setHeadingHoldMode( isHeadingHoldOn() );

   Player* pv = getOwnship();
   if (pv != nullptr) {
      DynamicsModel* md = pv->getDynamicsModel();
      if (md != nullptr) {
         // why mess with the player?  All it does is send it to the dynamics model anyways!  Skip the middle man!
         if ( isHeadingHoldOn() || isNavModeOn() ) {
            const int ihdg10 = static_cast<int>( getCommandedHeadingD() * 10.0f );
            const LCreal hdg = static_cast<LCreal>(ihdg10) / 10.0;

            md->setCommandedHeadingD(hdg, maxTurnRateDps, maxBankAngleDegs);
            md->setHeadingHoldOn( true );
         } else {
            md->setHeadingHoldOn( false );
            md->setControlStickRollInput( getControlStickRollInput() );
         }
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Altitude/pitch controller
//------------------------------------------------------------------------------
bool Autopilot::altitudeController()
{
   setAltitudeHoldMode( isAltitudeHoldOn() );

   Player* pv = getOwnship();
   if (pv != nullptr) {
      // skip the middle man
      DynamicsModel* md = pv->getDynamicsModel();
      if (md != nullptr) {
         if ( isAltitudeHoldOn() || isNavModeOn() ) {
            md->setCommandedAltitude(getCommandedAltitudeFt() * Basic::Distance::FT2M,  maxClimbRateMps, maxPitchAngleDegs);
            md->setAltitudeHoldOn( true );
         } else {
            md->setAltitudeHoldOn( false );
            md->setControlStickPitchInput( getControlStickPitchInput() );
         }
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Velocity/throttle controller
//------------------------------------------------------------------------------
bool Autopilot::velocityContoller()
{
   Player* pv = getOwnship();
   if (pv != nullptr) {
      // skip the middle man
      DynamicsModel* md = pv->getDynamicsModel();
      if (md != nullptr) {
         if ( isVelocityHoldOn() ) {
            md->setCommandedVelocityKts( getCommandedVelocityKts(), maxVelAccNps );
            md->setVelocityHoldOn(true);
         }
         else {
            md->setVelocityHoldOn(false);
            if (nThrPos > 0) pv->setThrottles(thrPos, nThrPos);
         }
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Get (access) functions
//------------------------------------------------------------------------------

// Attempt to get our lead player
const Player* Autopilot::getLeadPlayer()
{
   if (lead == nullptr && leadName != nullptr) {
      // we have no lead player, but we have a lead name, let's try to get this player
      // find the player in the simulation
      const Simulation* const sim = getSimulation();
      if (sim != nullptr) {
         const Basic::PairStream* players = sim->getPlayers();
         if (players != nullptr) {
            const Basic::Pair* pair = players->findByName(*leadName);
            if (pair != nullptr) {
               setLeadPlayer( static_cast<const Player*>( pair->object() ) );
            }
            players->unref();
            players = nullptr;
         }
      }
   }
   return lead;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool Autopilot::setCommandedHeadingD(const double v)
{
   cmdHdg = v;
   return true;
}

bool Autopilot::setCommandedAltitudeFt(const double v)
{
   cmdAlt = v;
   return true;
}

bool Autopilot::setCommandedVelocityKts(const double v)
{
   cmdSpd = v;
   return true;
}

bool Autopilot::setRollSasMode(const bool flag)
{
   rollSasOn = flag;
   return true;
}

bool Autopilot::setPitchSasMode(const bool flag)
{
   pitchSasOn = flag;
   return true;
}

bool Autopilot::setYawSasMode(const bool flag)
{
   yawSAsOn = flag;
   return true;
}

bool Autopilot::setHeadingHoldMode(const bool flag)
{
   hdgHoldOn = flag && isRollSasOn();
   return true;
}

bool Autopilot::setAltitudeHoldMode(const bool flag)
{
   bool b = flag && isPitchSasOn();
   if (b && !altHoldOn && !holdAltSet && getOwnship() != nullptr) {
      // Altitude hold just came on, sample the vehicles altitude
      setCommandedAltitudeFt( static_cast<LCreal>(getOwnship()->getAltitudeFt()) );
   }
   altHoldOn = b;
   return true;
}

bool Autopilot::setVelocityHoldMode(const bool flag)
{
   spdHoldOn = flag;
   return true;
}

bool Autopilot::setNavMode(const bool flag)
{
   bool navModeOn1 = navModeOn;

   // Set NAV mode
   navModeOn = flag && isRollSasOn() && isPitchSasOn();
   if (navModeOn) {
      setHeadingHoldMode(true);
      setAltitudeHoldMode(true);
      setVelocityHoldMode(true);
   }

   // If Nav mode was just turned off,
   // set commanded heading and altitude to our current values
   if ( !navModeOn && navModeOn1 ) {
      Player* pv = getOwnship();
      if (pv != nullptr) {
        const LCreal hdg = static_cast<LCreal>(pv->getHeadingD());
        setCommandedHeadingD(hdg);
        setCommandedAltitudeFt( static_cast<LCreal>(pv->getAltitudeFt()) );
        setCommandedVelocityKts( pv->getTotalVelocityKts() );
      }
   }

   return (flag == navModeOn);
}

bool Autopilot::setLoiterMode(const bool flag)
{
   bool loiterModeOn1 = loiterModeOn;

   // Set loiter mode
   loiterModeOn = flag && isRollSasOn() && isPitchSasOn();

   // If loiter mode was just turned on ...
   // then make sure all hold mode gets turned on
   if (loiterModeOn && !loiterModeOn1) {
      setHeadingHoldMode(true);
      setAltitudeHoldMode(true);
      setVelocityHoldMode(true);
   }

   // If loiter mode was just turned off ...
   // the set commanded heading to our current values
   if ( !loiterModeOn && loiterModeOn1 ) {
      Player* pv = getOwnship();
      if (pv != nullptr) {
         const LCreal hdg = static_cast<LCreal>(pv->getHeadingD());
         setCommandedHeadingD(hdg);
      }
   }

   return (flag == loiterModeOn);
}

// Request loiter at given anchor point and (front side) course
bool Autopilot::requestLoiter(const double anchorLat, const double anchorLon, const LCreal course)
{
   bool ok = false;

   //const Navigation* nav = getOwnship()->getNavigation();
   //if (nav != 0) {
   //   loiterInboundCourse = course;
   //   ok = setLoiterMode(true);
   //   if (ok) {
   //      loiterAnchorLat = anchorLat;
   //      loiterAnchorLon = anchorLon;
   //      computerOrbitHoldingPatternMirrorWaypoint(
   //         anchorLat, anchorLon,               // Anchor point
   //         course,                             // In-bound course
   //         getLoiterPatternLengthNM(),         // Min length (nm)
   //         nav->getTrueAirspeedKts(),          // Speed (kts)
   //         isLoiterPatternCounterClockwise(),  // Counter-Clockwise pattern flag
   //         &loiterMirrorLat, &loiterMirrorLon  // Mirror point
   //      );

   //      loiterState = 1;  // start heading toward the anchor point
   //   }
   //}

   return ok;
}

// Get our anchor points for our loiter pattern
bool Autopilot::getLoiterPointAnchors(double* const anLat, double* const anLon, double* const mAnLat, double* const mAnLon) const
{
   if (anLat != nullptr) *anLat = loiterAnchorLat;
   if (anLon != nullptr) *anLon = loiterAnchorLon;
   if (mAnLat != nullptr) *mAnLat = loiterMirrorLat;
   if (mAnLon != nullptr) *mAnLon = loiterMirrorLon;
   return true;
}

// gets the loiter inbound course
double Autopilot::getLoiterCourse()
{
    return loiterInboundCourse;
}

// Sets the loiter pattern length (nm)
bool Autopilot::setLoiterPatternLengthNM(const double nm)
{
   bool ok = false;
   if (nm > 0.0) {
      // Must be greater than zero
      loiterLength = nm;
      ok = true;
   }
   return ok;
}

// Sets the Loiter pattern is counter-clockwise flag
bool Autopilot::setLoiterPatternCounterClockwise(const bool ccw)
{
   loiterCcwFlag = ccw;
   return true;
}

// Desired distance (meters) behind(+) the lead
bool Autopilot::setLeadFollowingDistanceTrail(const double trail)
{
   leadOffset[0] = -trail;
   return true;
}

// Desired distance (meters) right(+) of the lead
bool Autopilot::setLeadFollowingDistanceRight(const double right)
{
   leadOffset[1] =  right;
   return true;
}

// Desired delta altitude (meters) above(+) the lead
bool Autopilot::setLeadFollowingDeltaAltitude(const double above)
{
   leadOffset[2] = -above;
   return true;
}

// Our lead player
bool Autopilot::setLeadPlayer(const Player* const p)
{
   // remove old lead information
   if (lead != nullptr) lead->unref();
   if (leadName != nullptr) {
      leadName->unref();
      leadName = nullptr;
   }

   // set our lead
   lead = p;

   if (lead != nullptr) {
      lead->ref();
      leadHdg = static_cast<LCreal>(lead->getHeadingR());
      // grab our lead name
      if (lead->getName() != nullptr) leadName = lead->getName()->clone();
   }

   return true;
}

// set the lead player dynamically by name
bool Autopilot::setLeadPlayerName(const Basic::Identifier* const msg)
{
   // find the player in the simulation
   const Simulation* const sim = getSimulation();
   bool found = false;
   if (sim != nullptr) {
      const Basic::PairStream* players = sim->getPlayers();
      if (players != nullptr) {
         const Basic::Pair* pair = players->findByName(*msg);
         if (pair != nullptr) {
            setLeadPlayer( static_cast<const Player*>( pair->object() ) );
            found = true;
         }
         players->unref();
         players = nullptr;
      }
   }
   // if we didn't find the player, remove the lead name and player
   if (!found) setLeadPlayer(nullptr);

   return true;
}

// set the lead player dynamically by name using char*
bool Autopilot::setLeadPlayerName(const char* x)
{
   // find the player in the simulation
   const Simulation* const sim = getSimulation();
   bool found = false;
   if (sim != nullptr) {
      const Basic::PairStream* players = sim->getPlayers();
      if (players != nullptr) {
         const Basic::Pair* pair = players->findByName(x);
         if (pair != nullptr) {
            setLeadPlayer( static_cast<const Player*>( pair->object() ) );
            found = true;
         }
         players->unref();
         players = nullptr;
      }
   }
   // if we didn't find the player, remove the lead name and player
   if (!found) setLeadPlayer(nullptr);

   return true;
}

// "Follow the lead" mode flag
bool Autopilot::setFollowTheLeadMode(const bool f)
{
   followLeadModeOn = f;
   return true;
}

//------------------------------------------------------------------------------
// setControlStickRollInput() -- Set stick roll position (degs); left(-), right(+)
//------------------------------------------------------------------------------
bool Autopilot::setControlStickRollInput(const LCreal pos)
{
   bool ok = false;
   if (pos >= -1.0 && pos <= 1.0) {
      // Within valid limits, so ...
      stickRollPos = pos;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setControlStickPitchInput() -- Set stick pitch position (degs); aft(-), fwd(+)
//------------------------------------------------------------------------------
bool Autopilot::setControlStickPitchInput(const LCreal pos)
{
   bool ok = false;
   if (pos >= -1.0 && pos <= 1.0) {
      // Within valid limits, so ...
      stickPitchPos = pos;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Throttle(s) Control Input
//      Normalized inputs
//          Throttle inputs: where ...
//              'positions' is an array of throttle positions
//                  (for each throttle)
//                  < 0.0       -> Cutoff (as needed)
//                  0.0         -> Min power (Idle)
//                  1.0         -> Full power (MIL)
//                  2.0         -> A/B (as needed)
//              'num' is the number of throttle positions
//          returns the actual number of throttle positions
//------------------------------------------------------------------------------
int Autopilot::setThrottles(const LCreal* const positions, const unsigned int num)
{
   unsigned int n = 0;
   if (positions != nullptr) {
      for (unsigned int i = 0; i < num && i < MAX_THR; i++) {
         const LCreal pos = positions[i];
         if (pos >= -1.0f && pos <= 2.0f) {
            thrPos[n++] = pos;
         }
      }
   }
   nThrPos = n;
   return nThrPos;
}


//-----------------------------------------------------------------------------
// Slot functions
//-----------------------------------------------------------------------------

// Nav (route follow) mode flag
bool Autopilot::setSlotNavMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setNavMode( msg->getBoolean() );
    }
    return ok;
}

// Hold altitude (alt hold mode)
bool Autopilot::setSlotHoldAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setCommandedAltitudeFt( Basic::Feet::convertStatic( *msg ) );
       holdAltSet = ok;
    }
    return ok;
}

// Altitude hold mode flag
bool Autopilot::setSlotAltitudeHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setAltitudeHoldMode( msg->getBoolean() );
    }
    return ok;
}

// Hold velocity (kts)
bool Autopilot::setSlotHoldVelocityKts(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setCommandedVelocityKts( msg->getReal() );
       holdSpdSet = ok;
    }
    return ok;
}

// Velocity hold mode flag
bool Autopilot::setSlotVelocityHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setVelocityHoldMode( msg->getBoolean() );
    }
    return ok;
}


// Hold heading
bool Autopilot::setSlotHoldHeading(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setCommandedHeadingD( static_cast<LCreal>(Basic::Degrees::convertStatic( *msg )) );
       holdHdgSet = ok;
    }
    return ok;
}

// Heading altitude mode flag
bool Autopilot::setSlotHeadingHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setHeadingHoldMode( msg->getBoolean() );
    }
    return ok;
}

// Loiter mode flag
bool Autopilot::setSlotLoiterMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       ok = setLoiterMode( msg->getBoolean() );
    }
    return ok;
}

// Set slot: Loiter orbit pattern length
bool Autopilot::setSlotLoiterPatternLength(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       const LCreal lenNM = Basic::NauticalMiles::convertStatic(*msg);
       ok = setLoiterPatternLengthNM( lenNM );
    }
    return ok;
}

// Set slot: Loiter orbit pattern length (NM)
bool Autopilot::setSlotLoiterPatternLength(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setLoiterPatternLengthNM( msg->getReal() );
    }
    return ok;
}

// Set slot: Loiter orbit pattern time
bool Autopilot::setSlotLoiterPatternTime(const Basic::Time* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       loiterTime = Basic::Seconds::convertStatic(*msg);
       loiterTimeBased = true;
       ok = true;
    }
    return ok;
}


// Set slot: Loiter orbit pattern counter-clockwise flag
bool Autopilot::setSlotLoiterPatternCcwFlag(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setLoiterPatternCounterClockwise( msg->getBoolean() );
    }
    return ok;
}

// Set slot: Desired distance behind(+) the lead
bool Autopilot::setSlotLeadFollowingDistanceTrail(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDistanceTrail( distM );
    }
    return ok;
}

// Set slot: Desired distance (meters) behind(+) the lead
bool Autopilot::setSlotLeadFollowingDistanceTrail(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setLeadFollowingDistanceTrail( msg->getReal() );
    }
    return ok;
}

// Set slot: Desired distance right(+) of the lead
bool Autopilot::setSlotLeadFollowingDistanceRight(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       const LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDistanceRight( distM );
    }
    return ok;
}

// Set slot: Desired distance (meters) right(+) of the lead
bool Autopilot::setSlotLeadFollowingDistanceRight(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setLeadFollowingDistanceRight( msg->getReal() );
    }
    return ok;
}

// Set slot: Desired delta altitude above(+) the lead
bool Autopilot::setSlotLeadFollowingDeltaAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       const LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDeltaAltitude( distM );
    }
    return ok;
}

// Set slot: Desired delta altitude (meters) above(+) the lead
bool Autopilot::setSlotLeadFollowingDeltaAltitude(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setLeadFollowingDeltaAltitude( msg->getReal() );
    }
    return ok;
}

// Initial name of our lead player
bool Autopilot::setSlotLeadPlayerName(const Basic::Identifier* const p)
{
   if (leadName != nullptr) leadName->unref();
   leadName = p;
   if (leadName != nullptr) leadName->ref();
   return true;
}


// Set slot: "Follow the lead" mode flag
bool Autopilot::setSlotFollowTheLeadMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        bool flg = msg->getBoolean();
        ok = setFollowTheLeadMode( flg );
        if (flg && !ok) {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "Autopilot::setSlotFollowTheLeadMode: follow the lead mode was not set" << std::endl;
            }
        }
    }
    return ok;
}

// Set slot: Maximum turn rate - limits how fast (or slow) the pilot turns
bool Autopilot::setSlotMaxRateOfTurnDps(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxTurnRateDps(msg->getDouble());
   return ok;
}

// Set slot: Maximum bank angle - limits how far the pilot can bank
bool Autopilot::setSlotMaxBankAngle(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxBankAngleDeg(msg->getDouble());
   return ok;
}

// Set slot: Maximum climb / dive rate - limits how fast the pilot can dive/climb
bool Autopilot::setSlotMaxClimbRateFpm(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxClimbRateMps((msg->getDouble() * Basic::Distance::FT2M / Basic::Time::M2S));
   return ok;
}

// Set slot: Maximum climb / dive rate - limits how fast the pilot can dive/climb
bool Autopilot::setSlotMaxClimbRateMps(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxClimbRateMps(msg->getDouble());
   return ok;
}

// Set slot: Maximum pitch angle - limits how much pitch the pilot can climb/dive to
bool Autopilot::setSlotMaxPitchAngle(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxPitchAngleDeg(msg->getDouble());
   return ok;
}

// Set slot: Maximum acceleration - limits how fast the pilot can accelerate
bool Autopilot::setSlotMaxVelAccNps(const Basic::Number* const msg)
{
   bool ok = (msg != nullptr);
   if (ok) ok = setMaxVelAccNps(msg->getDouble());
   return ok;
}

// serializer
std::ostream& Autopilot::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName();
        j = 4;
    }

    if (loiterLength > 0) {
        // loiter pattern length
        indent(sout,i+j);
        sout << "loiterPatternLength:  " << loiterLength << std::endl;
        // loiter pattern counter clockwise flag
        indent(sout,i+j);
        sout << "loiterPatternCcwFlag:  " << loiterCcwFlag << std::endl;
    }

    // follow the leader stuff
    if (leadName != nullptr) {
        // leader name
        indent(sout,i+j);
        sout << "leadPlayerName:  " << *leadName << std::endl;
        // distance behind lead
        indent(sout,i+j);
        sout << "leadFollowingDistanceTrail:  " <<  leadOffset[0] << std::endl;
        // distance to the right lead
        indent(sout,i+j);
        sout << "leadFollowingDistanceRight:  " <<  leadOffset[1] << std::endl;
        // distance above (or below) lead
        indent(sout,i+j);
        sout << "leadFollowingDeltaAltitude:  " <<  leadOffset[2] << std::endl;
        // follow the lead mode
        indent(sout,i+j);
        sout << "followTheLeadMode:  " <<  followLeadModeOn << std::endl;

    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Autopilot::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
