//------------------------------------------------------------------------------
// Class: Autopilot
//------------------------------------------------------------------------------
#ifndef __Autopilot_H_A766D68B_4ABD_442c_812A_0897903FD55E__
#define __Autopilot_H_A766D68B_4ABD_442c_812A_0897903FD55E__

#include "openeaagles/simulation/Pilot.h"

namespace Eaagles {
   namespace Basic { class Angle; class Distance; class Identifier; class Number; }

namespace Simulation {

//------------------------------------------------------------------------------
// Class: Autopilot
// Base class:  Basic::Object -> ... -> Pilot -> Autopilot
//
// Form name: Autopilot
//
// Slots:
//    navMode                    <Number>    ! Nav (route follow) mode flag (default: true)
//
//    holdAltitude               <Distance>  ! Hold altitude (Distance) [default: player's altitude]
//    altitudeHoldMode           <Number>    ! Altitude holde mode flag [ default: true ]
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
// GUID: {A766D68B-4ABD-442c-812A-0897903FD55E}
//------------------------------------------------------------------------------
class Autopilot : public Pilot
{
   DECLARE_SUBCLASS(Autopilot,Pilot)

public:
   Autopilot();

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
   virtual bool isFollowTheLeadModeOn() { return (followLeadModeOn && getLeadPlayer() != 0); }  // "Follow the lead" mode flag
   virtual const Basic::Identifier* getLeadPlayerName() { return leadName; }
   virtual const Player* getLeadPlayer();                            // Our lead player

   virtual bool setLeadFollowingDistanceTrail(const double trail);   // Desired distance (meters) behind(+) the lead
   virtual bool setLeadFollowingDistanceRight(const double right);   // Desired distance (meters) right(+) of the lead
   virtual bool setLeadFollowingDeltaAltitude(const double above);   // Desired delta altitude (meters) above(+) the lead
   virtual bool setFollowTheLeadMode(const bool f);                  // "Follow the lead" mode flag
   virtual bool setLeadPlayerName(const Basic::Identifier* const);      // Name of our lead player
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

   // Compute the mirror waypoint for a orbit holding pattern
   bool computerOrbitHoldingPatternMirrorWaypoint(
               const double alat,      // In:   Anchor point latitude (degs)
               const double alon,      // In:   Anchor' point longitude (degs)
               const double crs,       // In:   In-bound course (degs)
               const double length,    // In:   Pattern length (nm)
               const double speed,     // In:   True airspeed speed (kts)
               const bool ccwFlg,      // In:   True if counter-clockwise orbit (else clockwise orbit)
               double* const mlat,     // Out:  Mirror point latitude (degs)
               double* const mlon      // Out:  Mirror point longitude (degs)
               );


   // Slot functions
   virtual bool setSlotNavMode(const Basic::Number* const msg);                       // Nav (route follow) mode flag
   virtual bool setSlotHoldAltitude(const Basic::Distance* const msg);                // Hold altiude
   virtual bool setSlotAltitudeHoldMode(const Basic::Number* const msg);              // Altitude hold mode flag
   virtual bool setSlotHoldVelocityKts(const Basic::Number* const msg);               // Hold velocity (kts)
   virtual bool setSlotVelocityHoldMode(const Basic::Number* const msg);              // Velocity hold mode flag
   virtual bool setSlotHoldHeading(const Basic::Angle* const msg);                    // Hold heading
   virtual bool setSlotHeadingHoldMode(const Basic::Number* const msg);               // Heading altitude mode flag
   virtual bool setSlotLoiterMode(const Basic::Number* const msg);                    // Loiter mode flag
   virtual bool setSlotLoiterPatternLength(const Basic::Distance* const msg);         // Loiter orbit pattern length
   virtual bool setSlotLoiterPatternLength(const Basic::Number* const msg);           // Loiter orbit pattern length (NM)
   virtual bool setSlotLoiterPatternCcwFlag(const Basic::Number* const msg);          // Loiter orbit pattern counter-clockwise flag
   virtual bool setSlotLeadFollowingDistanceTrail(const Basic::Distance* const msg);  // Desired distance behind(+) the lead
   virtual bool setSlotLeadFollowingDistanceTrail(const Basic::Number* const msg);    // Desired distance (meters) behind(+) the lead
   virtual bool setSlotLeadFollowingDistanceRight(const Basic::Distance* const msg);  // Desired distance right(+) of the lead
   virtual bool setSlotLeadFollowingDistanceRight(const Basic::Number* const msg);    // Desired distance (meters) right(+) of the lead
   virtual bool setSlotLeadFollowingDeltaAltitude(const Basic::Distance* const msg);  // Desired delta altitude above(+) the lead
   virtual bool setSlotLeadFollowingDeltaAltitude(const Basic::Number* const msg);    // Desired delta altitude (meters) above(+) the lead
   virtual bool setSlotFollowTheLeadMode(const Basic::Number* const msg);             // "Follow the lead" mode flag

   // Basic::Component interface
   virtual void reset();
    
protected:
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
   double   loiterAnchorLat;  // Loiter orbit pattern anchor point lattiude  (degs)
   double   loiterAnchorLon;  // Loiter orbit pattern anchor point longitude (degs)
   double   loiterMirrorLat;  // Loiter orbit pattern mirror point lattidue  (degs)
   double   loiterMirrorLon;  // Loiter orbit pattern mirror point longitude (degs)
   unsigned int loiterState;  // Loiter state machine
   double   loiterLength;     // Loiter pattern length (nm)
   bool     loiterCcwFlag;    // Loiter pattern counter-clockwise flag 

   // Follow that lead mode data
   osg::Vec3d leadOffset;     // Offsets from lead player (meters) Default -1NM and 2NM and 2000ft
   const Player* lead;        // Our lead player
   const Basic::Identifier* leadName;   // Name of our lead player
   double leadHdg;            // lead's heading (rad)
   bool     followLeadModeOn; // Loiter mode flag
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Autopilot_H_A766D68B_4ABD_442c_812A_0897903FD55E__
