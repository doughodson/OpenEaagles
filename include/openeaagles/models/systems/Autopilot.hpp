
#ifndef __oe_models_Autopilot_H__
#define __oe_models_Autopilot_H__

#include "openeaagles/models/systems/Pilot.hpp"
#include "openeaagles/base/osg/Vec3d"

namespace oe {
namespace base { class Angle; class Distance; class Identifier; class Number; class Time; }
namespace models {

//------------------------------------------------------------------------------
// Class: Autopilot
//
// Factory name: Autopilot
//
// Slots:
//    navMode                    <Number>          ! Nav (route follow) mode flag (default: true)
//
//    holdAltitude               <Distance>        ! Hold altitude (Distance) [default: player's altitude]
//    altitudeHoldMode           <Number>          ! Altitude hold mode flag [ default: true ]
//
//    holdVelocityKts            <Number>          ! Hold velocity (Kts) (Number) [default: player's velocity]
//    velocityHoldMode           <Number>          ! Velocity hold mode flag [ default: true ]
//
//    holdHeading                <Angle>           ! Hold heading (AbstractAngle) [default: player's heading]
//    headingHoldMode            <Number>          ! Heading hold mode flag [ default: true ]
//
//    loiterMode                 <Number>          ! Loiter mode flag (default: false)
//    loiterPatternLength        <Distance>        ! Loiter pattern length (default: 10.0f)
//    loiterPatternLength        <Number>          ! Loiter pattern length (nautical miles)
//    loiterPatternTime          <Time>            ! Loiter pattern time (seconds on inbound / outbound course)
//    loiterPatternCcwFlag       <Number>          ! Loiter pattern counter-clockwise flag (default: false, clockwise)
//
//    leadFollowingDistanceTrail <Distance>        ! Desired distance behind(+) the lead (Default: 1 NM trail)
//    leadFollowingDistanceTrail <Number>          ! Desired distance behind(+) the lead (meters)
//
//    leadFollowingDistanceRight <Distance>        ! Desired distance right(+) of the lead (Default: 1 NM right)
//    leadFollowingDistanceRight <Number>          ! Desired distance right(+) of the lead (meters)
//
//    leadFollowingDeltaAltitude <Distance>        ! Desired delta altitude above(+) the lead (Default: 2000ft below)
//    leadFollowingDeltaAltitude <Number>          ! Desired delta altitude above(+) the lead (meters)
//
//    leadPlayerName             <Identifier>      ! Name of our lead player (default: 0)
//    followTheLeadMode          <Number>          ! "Follow the lead" mode flag (must be set after 'leadPlayer') (default: false)
//
//    maxRateOfTurnDps           <Number>          ! Maximum rate of turn (degrees per second)
//    maxBankAngle               <Number>          ! Maximum bank angle (degrees)
//    maxClimbRateFpm            <Number>          ! Maximum climb / dive rate (feet per minute)
//    maxClimbRateMps            <Number>          ! Maximum climb / dive rate (meters per second)
//    maxPitchAngle              <Number>          ! Maximum pitch angle (degrees)
//    maxAcceleration            <Number>          ! Maximum velocity acceleration (NPS)
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
//   4) Limiting the autopilot inputs using maxBankAngleDegs and maxTurnRateDps will work if
//      the dynamics model can support the limits as well.  If not, the dynamics model will
//      limit the bank angle and turn rate, and these inputs will have no effect.  For example,
//      if the user limits the bank angle to 90 degrees, but the dynamics model only support 40,
//      the bank angle will be limited to 40. These autopilot limits are set as "pilot" limits,
//      not "aircraft" limits.
//
//------------------------------------------------------------------------------
class Autopilot : public Pilot
{
   DECLARE_SUBCLASS(Autopilot, Pilot)

public:
   Autopilot();

   // Default limits for pilot inputs
   static const double STD_RATE_TURN_DPS;      // default to 3.0 degrees per second
   static const double STD_MAX_BANK_ANGLE;     // default to +-30 degrees bank max
   static const double STD_MAX_CLIMB_RATE;     // default to 2000 fpm, or apprx 10.16 mps
   static const double STD_MAX_PITCH_ANGLE;    // default to +-10 degrees pitch max
   static const double STD_MAX_ACCEL_NPS;      // default to 5.0

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

   virtual bool isLoiterTimeBased() const { return loiterTimeBased; }
   virtual double getLoiterTime() const   { return loiterTime; }

   // Sets the loiter pattern length (nm)
   virtual bool setLoiterPatternLengthNM(const double nm);

   // Sets the Loiter pattern is counter-clockwise flag
   virtual bool setLoiterPatternCounterClockwise(const bool ccw);

   // Request loiter at given anchor point and (front side) course
   virtual bool requestLoiter(const double anchorLat, const double anchorLon, const double course);

   virtual double getLeadFollowingDistanceTrail() const { return -leadOffset[0]; }
   virtual double getLeadFollowingDistanceRight() const { return  leadOffset[1]; }
   virtual double getLeadFollowingDeltaAltitude() const { return -leadOffset[2]; }
   virtual bool isFollowTheLeadModeOn() const           { return followLeadModeOn; }  // "Follow the lead" mode flag
   virtual const base::Identifier* getLeadPlayerName()  { return leadName; }
   virtual const Player* getLeadPlayer();                                             // Our lead player

   // get pilot limits
   double getMaxTurnRate() const;
   double getMaxBankAngle() const;
   double getMaxClimbRate() const;
   double getMaxPitchAngle() const;
   double getMaxVelAcc() const;

   virtual bool setLeadFollowingDistanceTrail(const double trail);   // Desired distance (meters) behind(+) the lead
   virtual bool setLeadFollowingDistanceRight(const double right);   // Desired distance (meters) right(+) of the lead
   virtual bool setLeadFollowingDeltaAltitude(const double above);   // Desired delta altitude (meters) above(+) the lead
   virtual bool setFollowTheLeadMode(const bool f);                  // "Follow the lead" mode flag
   virtual bool setLeadPlayerName(const base::Identifier* const);    // Changes the name of our lead player
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
   virtual double getControlStickRollInput() const                     { return stickRollPos; }

   // Sets the stick roll input (see getControlStickRollInput()
   virtual bool setControlStickRollInput(const double roll);

   // Pitch Control Input
   //      Normalized inputs
   //      pitch:  -1.0 -> max forward; 0.0 -> center;  1.0 -> max back
   virtual double getControlStickPitchInput() const                    { return stickPitchPos; }

   // Sets the control stick pitch input (getControlStickPitchInput xxx())
   virtual bool setControlStickPitchInput(const double pitch);

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
   virtual int setThrottles(const double* const positions, const unsigned int num);

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
   // maximum velocity acceleration
   virtual bool setMaxVelAccNps(const double x);

   virtual void reset() override;

protected:
   // Slot functions
   bool setSlotNavMode(const base::Number* const msg);                       // Nav (route follow) mode flag
   bool setSlotHoldAltitude(const base::Distance* const msg);                // Hold altitude
   bool setSlotAltitudeHoldMode(const base::Number* const msg);              // Altitude hold mode flag
   bool setSlotHoldVelocityKts(const base::Number* const msg);               // Hold velocity (kts)
   bool setSlotVelocityHoldMode(const base::Number* const msg);              // Velocity hold mode flag
   bool setSlotHoldHeading(const base::Angle* const msg);                    // Hold heading
   bool setSlotHeadingHoldMode(const base::Number* const msg);               // Heading altitude mode flag
   bool setSlotLoiterMode(const base::Number* const msg);                    // Loiter mode flag
   bool setSlotLoiterPatternLength(const base::Distance* const msg);         // Loiter orbit pattern length
   bool setSlotLoiterPatternLength(const base::Number* const msg);           // Loiter orbit pattern length (NM)
   bool setSlotLoiterPatternTime(const base::Time* const msg);               // Loiter orbit pattern length (seconds)
   bool setSlotLoiterPatternCcwFlag(const base::Number* const msg);          // Loiter orbit pattern counter-clockwise flag
   bool setSlotLeadFollowingDistanceTrail(const base::Distance* const msg);  // Desired distance behind(+) the lead
   bool setSlotLeadFollowingDistanceTrail(const base::Number* const msg);    // Desired distance (meters) behind(+) the lead
   bool setSlotLeadFollowingDistanceRight(const base::Distance* const msg);  // Desired distance right(+) of the lead
   bool setSlotLeadFollowingDistanceRight(const base::Number* const msg);    // Desired distance (meters) right(+) of the lead
   bool setSlotLeadFollowingDeltaAltitude(const base::Distance* const msg);  // Desired delta altitude above(+) the lead
   bool setSlotLeadFollowingDeltaAltitude(const base::Number* const msg);    // Desired delta altitude (meters) above(+) the lead
   bool setSlotLeadPlayerName(const base::Identifier* const msg);            // Name of the player we are following
   bool setSlotFollowTheLeadMode(const base::Number* const msg);             // "Follow the lead" mode flag
   bool setSlotMaxRateOfTurnDps(const base::Number* const msg);              // Maximum turn rate - degrees per second
   bool setSlotMaxBankAngle(const base::Number* const msg);                  // Maximum bank angle - degrees
   bool setSlotMaxClimbRateMps(const base::Number* const msg);               // Max climb/dive rate - meters per second
   bool setSlotMaxClimbRateFpm(const base::Number* const msg);               // Max climb/dive rate - feet per minute
   bool setSlotMaxPitchAngle(const base::Number* const msg);                 // Max pitch angle - degrees
   bool setSlotMaxVelAccNps(const base::Number* const msg);                  // Maximum velocity acceleration (Nps)

   virtual bool modeManager();
   virtual bool headingController();
   virtual bool altitudeController();
   virtual bool velocityController();

   virtual bool processModeNavigation();
   virtual bool processModeLoiter();
   virtual bool processModeFollowTheLead();

   // System class Interface -- phase() callbacks
   virtual void process(const double dt) override;     // Phase 3

   // base::Component protected interface
   virtual bool shutdownNotification() override;

private:
   bool flySRT();                   // flies a standard rate of turn
   bool flyLoiterEntry();           // flies our entry pattern into the loiter
   bool flyLoiter();                // flies the loiter pattern
   bool calcMirrorLatLon();
   // fly to a given point using the course
   bool flyCRS(const double latDeg, const double lonDeg, const double crsCmdDeg);

   // Input controls
   double stickRollPos {};     // Stick roll position:  -1.0 -> max left;    0.0 -> center;  1.0 -> max right
   double stickPitchPos {};    // Stick pitch position: -1.0 -> max forward; 0.0 -> center;  1.0 -> max back

   // Throttles
   static const unsigned int MAX_THR = 8;
   double thrPos[MAX_THR] {};    // Throttle positions: min(0.0) to full(1.0)
   unsigned int nThrPos {};      // Number of throttles

   // Commanded values
   double cmdHdg {};           // Commanded Heading  (deg)
   double cmdAlt {};           // Commanded Altitude (ft)
   double cmdSpd {};           // Commanded Velocity (kts)
   bool   holdHdgSet {};       // Hold heading set by slot flag
   bool   holdAltSet {};       // Hold altitude set by slot flag
   bool   holdSpdSet {};       // Hold velocity set by slot flag

   // Mode flags
   bool rollSasOn {true};    // Roll SAS flag
   bool pitchSasOn {true};   // Pitch SAS flag
   bool yawSAsOn {true};     // Yaw SAS flag
   bool hdgHoldOn {true};    // Heading hold flag
   bool altHoldOn {true};    // Altitude hold flag
   bool spdHoldOn {true};    // Velocity hold flag
   bool navModeOn {true};    // Nav mode flag
   bool loiterModeOn {};     // Loiter mode flag

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

   double   loiterAnchorLat {};            // Loiter orbit pattern anchor point latitude  (degs)
   double   loiterAnchorLon {};            // Loiter orbit pattern anchor point longitude (degs)
   double   loiterMirrorLat {};            // Loiter orbit pattern mirror point latitude  (degs)
   double   loiterMirrorLon {};            // Loiter orbit pattern mirror point longitude (degs)
   double   loiterLength {10.0};           // Loiter pattern length (nm)
   bool     loiterCcwFlag {};              // Loiter pattern counter-clockwise flag
   double   loiterInboundCourse {};        // Loiter course we started the loiter pattern on (degs)
   EntryMode loiterEntryMode {PREENTRY};   // Entry mode into the loiter
   unsigned int loiterEntryPhase {};       // Phase of the entry mode
   bool isInbound {};                      // are we on the inbound loiter
   double loiterTime {};                   // loiter time (seconds)
   bool loiterTimeBased {};                // is our loiter based on time instead of length?

   // Pilot limits
   double maxTurnRateDps {};               // maximum turn rate
   double maxBankAngleDegs {};             // maximum bank angle
   double maxClimbRateMps {};              // maximum climb/dive rate (meters per second)
   double maxPitchAngleDegs {};            // maximum pitch angle
   double maxVelAccNps {};                 // maximum velocity acceleration

   // Follow that lead mode data
   base::Vec3d leadOffset;                 // Offsets from lead player (meters) Default -1NM and 2NM and 2000ft
   const Player* lead {};                  // Our lead player
   const base::Identifier* leadName {};    // Name of our lead player
   double leadHdg {};                      // lead's heading (rad)
   bool   followLeadModeOn {};             // Follow the lead mode flag
};

inline bool Autopilot::setMaxTurnRateDps(const double x)    { maxTurnRateDps = x; return true; }
inline bool Autopilot::setMaxBankAngleDeg(const double x)   { maxBankAngleDegs = x; return true; }
inline bool Autopilot::setMaxClimbRateMps(const double x)   { maxClimbRateMps = x; return true; }
inline bool Autopilot::setMaxPitchAngleDeg(const double x)  { maxPitchAngleDegs = x; return true; }
inline bool Autopilot::setMaxVelAccNps(const double x)      { maxVelAccNps = x; return true; }
inline double Autopilot::getMaxTurnRate() const             { return maxTurnRateDps; }
inline double Autopilot::getMaxBankAngle() const            { return maxBankAngleDegs; }
inline double Autopilot::getMaxClimbRate() const            { return maxClimbRateMps; }
inline double Autopilot::getMaxPitchAngle() const           { return maxPitchAngleDegs; }
inline double Autopilot::getMaxVelAcc() const               { return maxVelAccNps; }

}
}

#endif
