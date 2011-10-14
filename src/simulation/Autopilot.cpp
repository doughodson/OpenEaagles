
#include "openeaagles/simulation/Autopilot.h"

//
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

namespace Eaagles {
namespace Simulation {

// =============================================================================
// class: Autopilot
// =============================================================================

IMPLEMENT_SUBCLASS(Autopilot,"Autopilot")

// slot table for this class type
BEGIN_SLOTTABLE(Autopilot)
   "navMode",                    //  1) Nav (route follow) mode flag (default: true)
   "holdAltitude",               //  2) Hold altitude (default: player's altitude) (Distance)
   "altitudeHoldMode",           //  3) altitude holde mode flag (default: true)
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
END_SLOTTABLE(Autopilot)

//  Map slot table to handles
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

   loiterAnchorLat = 0;
   loiterAnchorLon = 0;
   loiterMirrorLat = 0;
   loiterMirrorLon = 0;
   loiterState = 0;
   loiterLength = 10.0f;
   loiterCourse = 0;
   loiterCcwFlag = false;

   setLeadFollowingDistanceTrail( Basic::Distance::NM2M );             // Default: 1 NM trail
   setLeadFollowingDistanceRight( Basic::Distance::NM2M );             // Default: 1 NM right
   setLeadFollowingDeltaAltitude( -2000.0f * Basic::Distance::FT2M );  // Default: 2000ft below

   lead = 0;
   leadName = 0;
   leadHdg = 0.0f;
   followLeadModeOn = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Autopilot::copyData(const Autopilot& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      lead = 0;
      leadName = 0;
   }

   stickRollPos   = org.stickRollPos;
   stickPitchPos  = org.stickPitchPos;

   for (unsigned int i = 0; i < MAX_THR; i++) {
      thrPos[i] = org.thrPos[i];
   }
   nThrPos = org.nThrPos;

   cmdHdg = org.cmdHdg;
   cmdAlt = org.cmdAlt;
   cmdSpd  = org.cmdSpd;

   holdHdgSet = org.holdHdgSet;
   holdAltSet = org.holdAltSet;
   holdSpdSet = org.holdSpdSet;

   rollSasOn   = org.rollSasOn;
   pitchSasOn  = org.pitchSasOn;
   yawSAsOn    = org.yawSAsOn;
   hdgHoldOn   = org.hdgHoldOn;
   altHoldOn   = org.altHoldOn;
   spdHoldOn   = org.spdHoldOn;
   navModeOn   = org.navModeOn;
   loiterModeOn  = org.loiterModeOn;

   loiterAnchorLat = org.loiterAnchorLat;
   loiterAnchorLon = org.loiterAnchorLon;
   loiterMirrorLat = org.loiterMirrorLat;
   loiterMirrorLon = org.loiterMirrorLon;
   loiterState = org.loiterState;
   loiterLength = org.loiterLength;
   loiterCcwFlag = org.loiterCcwFlag;
   loiterCourse = org.loiterCourse;

   leadOffset = org.leadOffset;
   setLeadPlayer( org.lead );
   setSlotLeadPlayerName( org.leadName );
   followLeadModeOn =  org.followLeadModeOn;
   leadHdg = org.leadHdg;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Autopilot::deleteData()
{
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Autopilot::reset()
{
    BaseClass::reset();

    Player* pv = getOwnship();
    if (pv != 0) {
        // If heading, altitude or velocity hold modes are set and their
        // hold values were not set by a slot function, then use the player's
        // current values.
        if (hdgHoldOn && !holdHdgSet) cmdHdg = (LCreal) pv->getHeadingD();
        if (altHoldOn && !holdAltSet) cmdAlt = (LCreal) pv->getAltitudeFt();
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
// Manage the modes --
//------------------------------------------------------------------------------
bool Autopilot::modeManager()
{
   // ---
   // Re-latch the modes -- (just command the previous mode.)
   //  If the mode was off, it still is.
   //  If the mode was on, the 'is' functions will make sure that all
   //  prerequesite are still met.
   // ---
   setNavMode( isNavModeOn() );
   if (!isLoiterModeOn()) loiterState = 0;

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
   if (nav != 0) {
      // Do we have valid NAV steering data?
      if (nav->isNavSteeringValid()) {
         LCreal a = nav->getTrueBrgDeg();
         setCommandedHeadingD( a );
      }

      // Do we have NAV commanded altitude?
      const Route* route = nav->getPriRoute();
      if (route != 0) {
         const Steerpoint* sp = route->getSteerpoint();
         if (sp != 0) {
            if (sp->isCmdAltValid()) {
               setCommandedAltitudeFt( sp->getCmdAltitudeFt() );
            }
            {
               LCreal spd = sp->getCmdAirspeedKts();
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
   bool ok = false;

   const Navigation* nav = getOwnship()->getNavigation();
   if (nav != 0) {
      // own position
      double navLat = nav->getLatitude();
      double navLon = nav->getLongitude();

      if (loiterState == 0 && nav->isPositionDataValid()) {
         // Just entered the loiter orbit pattern ...
         // 1) record our current position as the anchor point
         // 2) computer the mirror point
         loiterAnchorLat = navLat;
         loiterAnchorLon = navLon;
         loiterCourse = nav->getHeadingDeg();
         computerOrbitHoldingPatternMirrorWaypoint(
            loiterAnchorLat, loiterAnchorLon,            // Anchor point
            loiterCourse,                                // In-bound course
            getLoiterPatternLengthNM(),                  // Min length (nm)
            nav->getTrueAirspeedKts(),                   // Speed (kts)
            isLoiterPatternCounterClockwise(),           // Counter-Clockwise pattern flag
            &loiterMirrorLat, &loiterMirrorLon           // Mirror point
         );
         loiterState = 2;     // Start heading toward the mirror point
      }

      if (loiterState > 0) {
         // Select destination lat/lon
         double destLat = loiterAnchorLat;
         double destLon = loiterAnchorLon;
         if (loiterState == 2) {
            destLat = loiterMirrorLat;
            destLon = loiterMirrorLon;
         }

         // Compute bearing and distance to the destination
         double brg = 0;
         double dist = 0;
         Basic::Nav::gll2bd(navLat, navLon, destLat, destLon, &brg, &dist);

         // Check for passing destination (less than 2 NM and destination is behind us)
         // (if so then swap anchor/mirror as destination)
         if ( (brg > 90.0 || brg < -90.0) && dist < 2.0) {
            // Swap and don't change the current commanded heading
            if (loiterState != 2) loiterState = 2;
            else loiterState = 1;
         }
         else {
            // Command heading to the current distination
            setCommandedHeadingD( LCreal( brg ) );
         }
      }

      ok = true;
   }

   if (!ok) setLoiterMode( false );
   return ok;
}

//------------------------------------------------------------------------------
// processModeFollowTheLead() -- Process for the "follow the lead" mode
//------------------------------------------------------------------------------
bool Autopilot::processModeFollowTheLead()
{
   bool ok = false;
   
   // attempt to get our lead player, if we don't have one initially
   if (lead == 0) getLeadPlayer();
   
   // if we got a lead player, great... if not, we break out of follow the lead mode.
   if (lead->isActive() && lead->getDamage() < 0.5f) {

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
         double leadHdg0 = 0.98 * leadHdg + 0.02 * newHdg;
         leadHdg = Basic::Angle::aepcdRad( leadHdg0 );
      }

      // ---
      // Compute sin/cos of new heading
      // ---
      double aa1 = sin(leadHdg);
      double aa2 = cos(leadHdg);

      // ---
      // Compute displacement vector
      // ---
      osg::Vec3d posLead = lead->getPosition();
      osg::Vec3d posOwn  = getOwnship()->getPosition();
      double xi = posLead.x() - posOwn.x();
      double yi = posLead.y() - posOwn.y();

      // ---
      // Rotate to lead's (heading) coord system
      // ---
      double dx =  xi * aa2 + yi * aa1;
      double dy = -xi * aa1 + yi * aa2;

      // ---
      // Translate to station keeping point
      // ---
      double dx0 = dx + leadOffset.x();
      double dy0 = dy + leadOffset.y();

      // ---
      // Compute cmd velocity vel (lead's coord sys) ...
      // ... match leaders velocity vector [ vp  0 ] plus
      //     a delta velocity vector for position errors.
      // ---
      double vx = lead->getTotalVelocity() + KX * dx0;
      double vy =                      0 + KY * dy0;

      // ---
      // Compute total velocity
      // ---
      double vt = sqrt(vx*vx + vy*vy);

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
      double dhdg = atan2(uy,ux);
      double chhdg = Basic::Angle::aepcdRad(dhdg + leadHdg);
      setCommandedHeadingD( chhdg * Basic::Angle::R2DCC );

      double calt = lead->getAltitude() + (-leadOffset.z());
      setCommandedAltitudeFt( calt * Basic::Distance::M2FT );

      double vKts = vt * Basic::Time::H2S / Basic::Distance::NM2M;
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
bool Autopilot::computerOrbitHoldingPatternMirrorWaypoint(
            const double alat,      // In:   Anchor point latitude (degs)
            const double alon,      // In:   Anchor' point longitude (degs)
            const double crs,       // In:   In-bound 'true' course (degs)
            const double length,    // In:   Pattern length (nm)
            const double speed,     // In:   True airspeed speed (kts)
            const bool ccwFlg,      // In:   True if counter-clockwise orbit (else clockwise orbit)
            double* const mlat,     // Out:  Mirror point latitude (degs)
            double* const mlon      // Out:  Mirror point longitude (degs)
            )
{
   bool ok = false;
   if (mlat != 0 && mlon != 0) {

      // Turn radius (ft)
      double radius  = 0.053833866 * speed * speed;

      // Offset (width) of the orbit pattern (ft)
      double orbitOffsetFt  = 2.0 * radius;

      // ---
      // Pattern length (ft)
      //  -- must be at least 1 nm greater than the offset
      // ---
      double orbitLengthFt = length * Basic::Distance::NM2FT;
      const double lengthLowerLimit = orbitOffsetFt + Basic::Distance::NM2FT;
      if (orbitLengthFt < lengthLowerLimit) {
         orbitLengthFt = lengthLowerLimit;
      }

      // ---
      // Bearing to mirror point (degs)
      // ---
      double relBrgDeg = atan2(orbitOffsetFt, -orbitLengthFt) * Basic::Angle::R2DCC;
      double trueBrgDeg = 0;
      if (ccwFlg) {
         // Counter-clockwise (left) turn
         trueBrgDeg = Basic::Angle::aepcdDeg( crs - relBrgDeg );
      }
      else {
         // Clockwise (right) turn
         trueBrgDeg = Basic::Angle::aepcdDeg( crs + relBrgDeg );
      }

      // ---
      // Distance to mirror point (nm)
      // ---
      double distFt = sqrt( orbitLengthFt*orbitLengthFt + orbitOffsetFt*orbitOffsetFt );
      double distNM = distFt * Basic::Distance::FT2NM;

      // ---
      // Calculate mirror point
      // ---
      double lat2, lon2;
      Basic::Nav::gbd2ll(alat, alon, trueBrgDeg, distNM, &lat2, &lon2);
      *mlat = lat2;
      *mlon = lon2;

      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// Heading/roll contoller --
//------------------------------------------------------------------------------
bool Autopilot::headingController()
{
   // Re-latch the mode
   setHeadingHoldMode( isHeadingHoldOn() );

   Player* pv = getOwnship();
   if (pv != 0) {
      if ( isHeadingHoldOn() || isNavModeOn() ) {
         int ihdg10 = int( getCommandedHeadingD() * 10.0f );
         LCreal hdg = LCreal( ihdg10 ) / 10.0f;
         pv->setCommandedHeadingD( hdg );
         pv->setHeadingHoldOn( true );
      } else {
         pv->setHeadingHoldOn( false );
         pv->setControlStickRollInput( getControlStickRollInput() );
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Altitude/pitch contoller
//------------------------------------------------------------------------------
bool Autopilot::altitudeController()
{
   setAltitudeHoldMode( isAltitudeHoldOn() );

   Player* pv = getOwnship();
   if (pv != 0) {
      if ( isAltitudeHoldOn() || isNavModeOn() ) {
         pv->setCommandedAltitude( getCommandedAltitudeFt() * Basic::Distance::FT2M );
         pv->setAltitudeHoldOn( true );
      } else {
         pv->setAltitudeHoldOn( false );
         pv->setControlStickPitchInput( getControlStickPitchInput() );
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Velocity/throttle contoller
//------------------------------------------------------------------------------
bool Autopilot::velocityContoller()
{
   Player* pv = getOwnship();
   if (pv != 0) {
      if ( isVelocityHoldOn() ) {
         pv->setCommandedVelocityKts( getCommandedVelocityKts() );
         pv->setVelocityHoldOn(true);
      }
      else {
         pv->setVelocityHoldOn(false);
         if (nThrPos > 0) pv->setThrottles(thrPos, nThrPos);
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
    if (lead == 0 && leadName != 0) {
        // we have no lead player, but we have a lead name, let's try to get this player
        // find the player in the simulation
        const Simulation* const sim = getSimulation();
        if (sim != 0) {
            const Basic::PairStream* players = sim->getPlayers();
            if (players != 0) {
                const Basic::Pair* pair = players->findByName(*leadName);
                if (pair != 0) {
                    setLeadPlayer( static_cast<const Player*>( pair->object() ) );
                }
            }
            players->unref();
            players = 0;
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
   if (b && !altHoldOn && !holdAltSet && getOwnship() != 0) {
      // Altitude hold just came on, sample the vehilces altitude
      setCommandedAltitudeFt( (LCreal) getOwnship()->getAltitudeFt() );
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
      if (pv != 0) {
        LCreal hdg = (LCreal) pv->getHeadingD();
        setCommandedHeadingD(hdg);
        setCommandedAltitudeFt( (LCreal) pv->getAltitudeFt() );
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
   // then make sure all hold mode gets turned off
   if (loiterModeOn && !loiterModeOn1) {
      setHeadingHoldMode(true);
      setAltitudeHoldMode(true);
      setVelocityHoldMode(true);
   }

   // If loiter mode was just turned off ...
   // the set commanded heading to our current values
   if ( !loiterModeOn && loiterModeOn1 ) {
      Player* pv = getOwnship();
      if (pv != 0) {
         LCreal hdg = (LCreal) pv->getHeadingD();
         setCommandedHeadingD(hdg);
      }
   }

   return (flag == loiterModeOn);
}

// Request loiter at given anchor point and (front side) course
bool Autopilot::requestLoiter(const double anchorLat, const double anchorLon, const LCreal course)
{
   bool ok = false;

   const Navigation* nav = getOwnship()->getNavigation();
   if (nav != 0) {
      loiterCourse = course;
      ok = setLoiterMode(true);
      if (ok) {
         loiterAnchorLat = anchorLat;
         loiterAnchorLon = anchorLon;
         computerOrbitHoldingPatternMirrorWaypoint(
            anchorLat, anchorLon,               // Anchor point
            course,                             // In-bound course
            getLoiterPatternLengthNM(),         // Min length (nm)
            nav->getTrueAirspeedKts(),          // Speed (kts)
            isLoiterPatternCounterClockwise(),  // Counter-Clockwise pattern flag
            &loiterMirrorLat, &loiterMirrorLon  // Mirror point
         );

         loiterState = 1;  // start heading toward the anchor point
      }
   }

   return ok;
}

// Get our anchor points for our loiter pattern
bool Autopilot::getLoiterPointAnchors(double* const anLat, double* const anLon, double* const mAnLat, double* const mAnLon) const
{
   if (anLat != 0) *anLat = loiterAnchorLat;
   if (anLon != 0) *anLon = loiterAnchorLon;
   if (mAnLat != 0) *mAnLat = loiterMirrorLat;
   if (mAnLon != 0) *mAnLon = loiterMirrorLon;
   return true;
}

// gets the loiter inbound course
double Autopilot::getLoiterCourse()
{
    return loiterCourse;
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
   if (lead != 0) lead->unref();
   if (leadName != 0) {
      leadName->unref();
      leadName = 0;
   }

   // set our lead
   lead = p;

   if (lead != 0) {
      lead->ref();
      leadHdg = (LCreal) lead->getHeadingR();
      // grab our lead name
      if (lead->getName() != 0) leadName = (Basic::Identifier*)lead->getName()->clone();
   }

   return true;
}

// set the lead player dynamically by name
bool Autopilot::setLeadPlayerName(const Basic::Identifier* const msg)
{
    // find the player in the simulation
    const Simulation* const sim = getSimulation();
    bool found = false;
    if (sim != 0) {
        const Basic::PairStream* players = sim->getPlayers();
        if (players != 0) {
            const Basic::Pair* pair = players->findByName(*msg);
            if (pair != 0) {
                setLeadPlayer( static_cast<const Player*>( pair->object() ) );
                found = true;
            }
        }
        players->unref();
        players = 0;
    }
    // if we didn't find the player, remove the lead name and player
    if (!found) setLeadPlayer(0);

    return true;
}

// set the lead player dynamically by name using char*
bool Autopilot::setLeadPlayerName(const char* x)
{
    // find the player in the simulation
    const Simulation* const sim = getSimulation();
    bool found = false;
    if (sim != 0) {
        const Basic::PairStream* players = sim->getPlayers();
        if (players != 0) {
            const Basic::Pair* pair = players->findByName(x);
            if (pair != 0) {
                setLeadPlayer( static_cast<const Player*>( pair->object() ) );
                found = true;
            }
        }
        players->unref();
        players = 0;
    }
    // if we didn't find the player, remove the lead name and player
    if (!found) setLeadPlayer(0);

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
   if (pos >= -1.0f && pos <= 1.0f) {
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
   if (pos >= -1.0f && pos <= 1.0f) {
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
   if (positions != 0) {
      for (unsigned int i = 0; i < num && i < MAX_THR; i++) {
         LCreal pos = positions[i];
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
    if (msg != 0) {
       ok = setNavMode( msg->getBoolean() );
    }
    return ok;
}

// Hold altiude (alt hold mode)
bool Autopilot::setSlotHoldAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setCommandedAltitudeFt( Basic::Feet::convertStatic( *msg ) );
       holdAltSet = ok;
    }
    return ok;
}

// Altitude hold mode flag
bool Autopilot::setSlotAltitudeHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setAltitudeHoldMode( msg->getBoolean() );
    }
    return ok;
}

// Hold velocity (kts)
bool Autopilot::setSlotHoldVelocityKts(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setCommandedVelocityKts( msg->getReal() );
       holdSpdSet = ok;
    }
    return ok;
}

// Velocity hold mode flag
bool Autopilot::setSlotVelocityHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setVelocityHoldMode( msg->getBoolean() );
    }
    return ok;
}


// Hold heading
bool Autopilot::setSlotHoldHeading(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setCommandedHeadingD( (LCreal)Basic::Degrees::convertStatic( *msg ) );
       holdHdgSet = ok;
    }
    return ok;
}

// Heading altitude mode flag
bool Autopilot::setSlotHeadingHoldMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setHeadingHoldMode( msg->getBoolean() );
    }
    return ok;
}

// Loiter mode flag
bool Autopilot::setSlotLoiterMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       ok = setLoiterMode( msg->getBoolean() );
    }
    return ok;
}

// Set slot: Loiter orbit pattern length
bool Autopilot::setSlotLoiterPatternLength(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       LCreal lenNM = Basic::NauticalMiles::convertStatic(*msg);
       ok = setLoiterPatternLengthNM( lenNM );
    }
    return ok;
}

// Set slot: Loiter orbit pattern length (NM)
bool Autopilot::setSlotLoiterPatternLength(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = setLoiterPatternLengthNM( msg->getReal() );
    }
    return ok;
}

// Set slot: Loiter orbit pattern counter-clockwise flag
bool Autopilot::setSlotLoiterPatternCcwFlag(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = setLoiterPatternCounterClockwise( msg->getBoolean() );
    }
    return ok;
}

// Set slot: Desired distance behind(+) the lead
bool Autopilot::setSlotLeadFollowingDistanceTrail(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDistanceTrail( distM );
    }
    return ok;
}

// Set slot: Desired distance (meters) behind(+) the lead
bool Autopilot::setSlotLeadFollowingDistanceTrail(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = setLeadFollowingDistanceTrail( msg->getReal() );
    }
    return ok;
}

// Set slot: Desired distance right(+) of the lead
bool Autopilot::setSlotLeadFollowingDistanceRight(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDistanceRight( distM );
    }
    return ok;
}

// Set slot: Desired distance (meters) right(+) of the lead
bool Autopilot::setSlotLeadFollowingDistanceRight(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = setLeadFollowingDistanceRight( msg->getReal() );
    }
    return ok;
}

// Set slot: Desired delta altitude above(+) the lead
bool Autopilot::setSlotLeadFollowingDeltaAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       LCreal distM = Basic::Meters::convertStatic(*msg);
       ok = setLeadFollowingDeltaAltitude( distM );
    }
    return ok;
}

// Set slot: Desired delta altitude (meters) above(+) the lead
bool Autopilot::setSlotLeadFollowingDeltaAltitude(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = setLeadFollowingDeltaAltitude( msg->getReal() );
    }
    return ok;
}

// Initial name of our lead player
bool Autopilot::setSlotLeadPlayerName(const Basic::Identifier* const p)
{
   if (leadName != 0) leadName->unref();
   leadName = p;
   if (leadName != 0) leadName->ref();
   return true;
}


// Set slot: "Follow the lead" mode flag
bool Autopilot::setSlotFollowTheLeadMode(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
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


// serializer
std::ostream& Autopilot::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName();
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
    if (leadName != 0) {
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
