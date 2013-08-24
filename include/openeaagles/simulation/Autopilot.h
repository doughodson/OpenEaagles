//------------------------------------------------------------------------------
// Class: Autopilot
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Autopilot_H__
#define __Eaagles_Simulation_Autopilot_H__

#include "openeaagles/simulation/Pilot.h"

namespace Eaagles {
   namespace Basic { class Angle; class Distance; class Identifier; class Number; }

namespace Simulation {

//------------------------------------------------------------------------------
// Class: Autopilot
//
// Form name: Autopilot
//
// Slots:
//    navMode                    <Number>    ! Nav (route follow) mode flag (default: true)
//
//    holdAltitude               <Distance>  ! Hold altitude (Distance) [default: player's altitude]
//    altitudeHoldMode           <Number>    ! Altitude hold mode flag [ default: true ]
//
//    holdVelocityKts            <Number>    ! Hold velocity (Kts) (Number) [default: player's velocity]
//    velocityHoldMode           <Number>    ! Velocity hold mode flag [ default: true ]
//
//    holdHeading                <Distance>  ! Hold heading (Angle) [default: player's heading]
//    headingHoldMode            <Number>    ! Heading hold mode flag [ default: true ]
//
//    loiterMode                 <Number>    ! Loiter mode flag (default: false)
//    loiterPatternLength        <Distance>  ! Loiter pattern length (default: 10.0f)
//    loiterPatternLength        <Number>    ! Loiter pattern length (nautical miles)
//    loiterPatternCcwFlag       <Number>    ! Loiter pattern counter-clockwise flag (default: false, clockwise)
//
//    leadFollowingDistanceTrail <Distance>  ! Desired distance behind(+) the lead (Default: 1 NM trail)
//    leadFollowingDistanceTrail <Number>    ! Desired distance behind(+) the lead (meters)
//
//    leadFollowingDistanceRight <Distance>  ! Desired distance right(+) of the lead (Default: 1 NM right)
//    leadFollowingDistanceRight <Number>    ! Desired distance right(+) of the lead (meters)
//
//    leadFollowingDeltaAltitude <Distance>  ! Desired delta altitude above(+) the lead (Default: 2000ft below)
//    leadFollowingDeltaAltitude <Number>    ! Desired delta altitude above(+) the lead (meters)
//
//    leadPlayerName             <Identifier> ! Name of our lead player (default: 0)
//    followTheLeadMode          <Number>     ! "Follow the lead" mode flag (must be set after 'leadPlayer') (default: false)
//
// Notes:
//   1) Default is Nav (route follow) mode, which forces heading, altitude and
//      velocity hold modes.
//
//   2) During reset(), if heading, altitude or velocity modes are set and their
//      hold values have not been set by a slot function, then the player's
//      current values are used.
//
//   3) setLoiter(true) will turn loiter mode on at the current player location and heading, 
//      while requestLoiter() will turn on loiter mode with a given anchor point and course.
//      setLointer(false) will turn off loiter mode in both cases.
//
//   4) Limiting the autopilot inputs using maxBankAngleDegs and maxTurnRateDps will work if the dynamics model can support the limits
//      as well.  If not, the dynamics model will limit the bank angle and turn rate, and these inputs will have no effect.  For example,
//      if the user limits the bank angle to 90 degrees, but the dynamics model only support 40, the bank angle will be limited to 40.
//      These autopilot limits are set as "pilot" limits, not "aircraft" limits.
//
//------------------------------------------------------------------------------
class Autopilot : public Pilot
{
   DECLARE_SUBCLASS(Autopilot,Pilot)

public:
   Autopilot();


   // Standard limiting definition
   static const double STD_RATE_TURN_DPS;      // default to 3.0 degrees per second
   static const double STD_MAX_BANK_ANGLE;     // default to +-30 degrees bank max
   static const double STD_MAX_CLIMB_RATE;     // defaul to 2000 fpm, or apprx 10.16 mps
   static const double STD_MAX_PITCH_ANGLE;    // default to +-10 degrees pitch max

   virtual bool isHeadingHoldOn() const { return hdgHoldOn; }        // True if heading hold mode
   virtual double getCommandedHeadingD() const { return cmdHdg; }    // Returns the hold heading (degs)
   virtual bool setCommandedHeadingD(const double v);                // Sets the hold heading (degs)
   virtual bool setHeadingHoldMode(const bool f);                    // Sets the heading hold flag

   virtual bool isAltitudeHoldOn() const { return altHoldOn; }       // True if altitude hold mode
   virtual double getCommandedAltitudeFt() const { return cmdAlt; }  // Returns the hold altitude (ft)
   virtual bool setCommandedAltitudeFt(const double v);              // Sets the hold altitude (ft)
   virtual bool setAltitudeHoldMode(const bool f);                   // Sets the altitude hold flag

   virtual bool isVelocityHoldOn() const { return spdHoldOn; }       // True if velocity hold mode
   virtual double getCommandedVelocityKts() const { return cmdSpd; } // Returns the hold velocity (Kts)
   virtual bool setCommandedVelocityKts(const double v);             // Sets the hold velocity (Kts)
   virtual bool setVelocityHoldMode(const bool f);                   // Sets the velocity hold flag

   virtual bool isNavModeOn() const { return navModeOn; }            // True if Nav (route follow) mode
   virtual bool setNavMode(const bool f);                            // Sets the nav mode flag

   virtual bool isLoiterModeOn() const { return loiterModeOn; }      // True if loiter mode
   virtual bool setLoiterMode(const bool f);                         // Sets the loiter flag (loiter in position)

   // Returns the loiter pattern length (nm)
   virtual double getLoiterPatternLengthNM() const { return loiterLength; }

   // get our loiter point anchor points
   virtual bool getLoiterPointAnchors(double* const anLat, double* const anLon, double* const mAnLat, double* const mAnLon) const;
   virtual double getLoiterCourse();

   // Returns true if the Loiter pattern is counter-clockwise
   virtual bool isLoiterPatternCounterClockwise() const { return loiterCcwFlag; }

   // Sets the loiter pattern length (nm)
   virtual bool setLoiterPatternLengthNM(const double nm);

   // Sets the Loiter pattern is counter-clockwise flag
   virtual bool setLoiterPatternCounterClockwise(const bool ccw);

   // Request loiter at given anchor point and (front side) course
   virtual bool requestLoiter(const double anchorLat, const double anchorLon, const LCreal course);

   virtual double getLeadFollowingDistanceTrail() const { return -leadOffset[0]; }
   virtual double getLeadFollowingDistanceRight() const { return  leadOffset[1]; }
   virtual double getLeadFollowingDeltaAltitude() const { return -leadOffset[2]; }
   virtual bool isFollowTheLeadModeOn() const           { return followLeadModeOn; }  // "Follow the lead" mode flag
   virtual const Basic::Identifier* getLeadPlayerName() { return leadName; }
   virtual const Player* getLeadPlayer();                            // Our lead player

   virtual bool setLeadFollowingDistanceTrail(const double trail);   // Desired distance (meters) behind(+) the lead
   virtual bool setLeadFollowingDistanceRight(const double right);   // Desired distance (meters) right(+) of the lead
   virtual bool setLeadFollowingDeltaAltitude(const double above);   // Desired delta altitude (meters) above(+) the lead
   virtual bool setFollowTheLeadMode(const bool f);                  // "Follow the lead" mode flag
   virtual bool setLeadPlayerName(const Basic::Identifier* const);   // Changes the name of our lead player
   virtual bool setLeadPlayerName(const char* x);                    // set the lead player name by characters
   virtual bool setLeadPlayer(const Player* const);                  // Our lead player

   virtual bool isRollSasOn() const { return rollSasOn; }
   virtual bool isPitchSasOn() const { return pitchSasOn; }
   virtual bool isYawSasOn() const { return yawSAsOn; }
   virtual bool setRollSasMode(const bool f);
   virtual bool setPitchSasMode(const bool f);
   virtual bool setYawSasMode(const bool f);


   // Roll Control Input 
   //      Normalized inputs
   //      roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual LCreal getControlStickRollInput() const { return stickRollPos; }

   // Sets the stick roll input (see getControlStickRollInput()
   virtual bool setControlStickRollInput(const LCreal roll);

   // Pitch Control Input
   //      Normalized inputs
   //      pitch:  -1.0 -> max forward; 0.0 -> center;  1.0 -> max back
   virtual LCreal getControlStickPitchInput() const { return stickPitchPos; }

   // Sets the control stick pitch input (getControlStickPitchInput xxx())
   virtual bool setControlStickPitchInput(const LCreal pitch);

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
   virtual int setThrottles(const LCreal* const positions, const unsigned int num);

   //// Compute the mirror waypoint for a orbit holding pattern
   //bool computerOrbitHoldingPatternMirrorWaypoint(
   //            const double alat,      // In:   Anchor point latitude (degs)
   //            const double alon,      // In:   Anchor' point longitude (degs)
   //            const double crs,       // In:   In-bound course (degs)
   //            const double length,    // In:   Pattern length (nm)
   //            const double speed,     // In:   True airspeed speed (kts)
   //            const bool ccwFlg,      // In:   True if counter-clockwise orbit (else clockwise orbit)
   //            double* const mlat,     // Out:  Mirror point latitude (degs)
   //            double* const mlon      // Out:  Mirror point longitude (degs)
   //            );

   // max turn rate (degrees per second)
   virtual bool setMaxTurnRateDps(const double x);
   // maximum bank angle (degrees)
   virtual bool setMaxBankAngleDeg(const double x);
   // maximum climb/dive rate
   virtual bool setMaxClimbRateMps(const double x);
   // maximum pitch angle
   virtual bool setMaxPitchAngleDeg(const double x);
   
   // Eaagles::Basic::Component interface methods
   virtual void reset();
    
protected:
   // Slot functions
   bool setSlotNavMode(const Basic::Number* const msg);                       // Nav (route follow) mode flag
   bool setSlotHoldAltitude(const Basic::Distance* const msg);                // Hold altitude
   bool setSlotAltitudeHoldMode(const Basic::Number* const msg);              // Altitude hold mode flag
   bool setSlotHoldVelocityKts(const Basic::Number* const msg);               // Hold velocity (kts)
   bool setSlotVelocityHoldMode(const Basic::Number* const msg);              // Velocity hold mode flag
   bool setSlotHoldHeading(const Basic::Angle* const msg);                    // Hold heading
   bool setSlotHeadingHoldMode(const Basic::Number* const msg);               // Heading altitude mode flag
   bool setSlotLoiterMode(const Basic::Number* const msg);                    // Loiter mode flag
   bool setSlotLoiterPatternLength(const Basic::Distance* const msg);         // Loiter orbit pattern length
   bool setSlotLoiterPatternLength(const Basic::Number* const msg);           // Loiter orbit pattern length (NM)
   bool setSlotLoiterPatternCcwFlag(const Basic::Number* const msg);          // Loiter orbit pattern counter-clockwise flag
   bool setSlotLeadFollowingDistanceTrail(const Basic::Distance* const msg);  // Desired distance behind(+) the lead
   bool setSlotLeadFollowingDistanceTrail(const Basic::Number* const msg);    // Desired distance (meters) behind(+) the lead
   bool setSlotLeadFollowingDistanceRight(const Basic::Distance* const msg);  // Desired distance right(+) of the lead
   bool setSlotLeadFollowingDistanceRight(const Basic::Number* const msg);    // Desired distance (meters) right(+) of the lead
   bool setSlotLeadFollowingDeltaAltitude(const Basic::Distance* const msg);  // Desired delta altitude above(+) the lead
   bool setSlotLeadFollowingDeltaAltitude(const Basic::Number* const msg);    // Desired delta altitude (meters) above(+) the lead
   bool setSlotLeadPlayerName(const Basic::Identifier* const msg);            // Name of the player we are following
   bool setSlotFollowTheLeadMode(const Basic::Number* const msg);             // "Follow the lead" mode flag
   bool setSlotMaxRateOfTurnDps(const Basic::Number* const msg);              // Maximum turn rate - degrees per second
   bool setSlotMaxBankAngle(const Basic::Number* const msg);                  // Maximum bank angle - degrees
   bool setSlotMaxClimbRateMps(const Basic::Number* const msg);               // Max climb/dive rate - meters per second
   bool setSlotMaxClimbRateFpm(const Basic::Number* const msg);               // Max climb/dive rate - feet per minute
   bool setSlotMaxPitchAngle(const Basic::Number* const msg);                 // Max pitch angle - degrees

   virtual bool modeManager();
   virtual bool headingController();
   virtual bool altitudeController();
   virtual bool velocityContoller();

   virtual bool processModeNavigation();
   virtual bool processModeLoiter();
   virtual bool processModeFollowTheLead();

   // System class Interface -- phase() callbacks
   virtual void process(const LCreal dt);     // Phase 3

private:
   // Input controls
   LCreal   stickRollPos;     // Stick roll position:  -1.0 -> max left;    0.0 -> center;  1.0 -> max right
   LCreal   stickPitchPos;    // Stick pitch position: -1.0 -> max forward; 0.0 -> center;  1.0 -> max back

   // Throttles
   static const unsigned int MAX_THR = 8;
   LCreal   thrPos[MAX_THR];  // Throttle positions: min(0.0) to full(1.0)
   unsigned int nThrPos;      // Number of throttles

   // Commanded values
   double   cmdHdg;           // Commanded Heading  (deg)
   double   cmdAlt;           // Commanded Altitude (ft)
   double   cmdSpd;           // Commanded Velocity (kts)
   bool     holdHdgSet;       // Hold heading set by slot flag
   bool     holdAltSet;       // Hold altitude set by slot flag
   bool     holdSpdSet;       // Hold velocity set by slot flag

   // Mode flags
   bool     rollSasOn;        // Roll SAS flag
   bool     pitchSasOn;       // Pitch SAS flag
   bool     yawSAsOn;         // Yaw SAS flag
   bool     hdgHoldOn;        // Heading hold flag
   bool     altHoldOn;        // Altitude hold flag
   bool     spdHoldOn;        // Velocity hold flag
   bool     navModeOn;        // Nav mode flag
   bool     loiterModeOn;     // Loiter mode flag

   // Loiter mode data
   // Entry modes for loiter
   enum EntryMode { 
      PREENTRY, 
      SIMPLE, 
      DIRECT, 
      PARALLEL, 
      TEARDROP, 
      LOITER
   };  
   bool flyLoiterEntry();           // flies our entry pattern into the loiter
   bool flyLoiter();                // flies the loiter pattern
   double   loiterAnchorLat;        // Loiter orbit pattern anchor point latitude  (degs)
   double   loiterAnchorLon;        // Loiter orbit pattern anchor point longitude (degs)
   double   loiterMirrorLat;        // Loiter orbit pattern mirror point latitude  (degs)
   double   loiterMirrorLon;        // Loiter orbit pattern mirror point longitude (degs)
   double   loiterLength;           // Loiter pattern length (nm)
   bool     loiterCcwFlag;          // Loiter pattern counter-clockwise flag 
   double   loiterInboundCourse;    // Loiter course we started the loiter pattern on (degs)
   EntryMode  loiterEntryMode;      // Entry mode into the loiter
   unsigned int loiterEntryPhase;   // Phase of the entry mode 

   // Pilot limits
   double maxTurnRateDps;           // maximum turn rate
   double maxBankAngleDegs;         // maximum bank angle
   double maxClimbRateMps;          // maximum climb/dive rate (meters per second)
   double maxPitchAngleDegs;        // maximum pitch angle

   // Follow that lead mode data
   osg::Vec3d leadOffset;     // Offsets from lead player (meters) Default -1NM and 2NM and 2000ft
   const Player* lead;        // Our lead player
   const Basic::Identifier* leadName;   // Name of our lead player
   double leadHdg;            // lead's heading (rad)
   bool   followLeadModeOn;   // Follow the lead mode flag

};

inline bool Autopilot::setMaxTurnRateDps(const double x)    { maxTurnRateDps = x; return true; }
inline bool Autopilot::setMaxBankAngleDeg(const double x)   { maxBankAngleDegs = x; return true; }
inline bool Autopilot::setMaxClimbRateMps(const double x)   { maxClimbRateMps = x; return true; }
inline bool Autopilot::setMaxPitchAngleDeg(const double x)  { maxPitchAngleDegs = x; return true; }

} // End Simulation namespace
} // End Eaagles namespace

#endif
