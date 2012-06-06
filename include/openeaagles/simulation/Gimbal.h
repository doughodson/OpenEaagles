//------------------------------------------------------------------------------
// Class: Gimbal
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Gimbal_H__
#define __Eaagles_Simulation_Gimbal_H__

#include "openeaagles/simulation/System.h"

namespace Eaagles {
   namespace Basic {
      class Angle;
      class Distance;
      class List;
      class PairStream;
   }
namespace Simulation {

class Emission;
class SensorMsg;
class Tdb;

//------------------------------------------------------------------------------
// Class: Gimbal
//
// Description: Generic class for all gimbals, RF antennas, IR seekers, etc
//
// Notes:
//    1) This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
//    2) By convention, R/F and IR systems are responsible for filtering their
//    own players of interest.  This gimbal class has its own variation of the
//    processPlayersOfInterest() funtion that filters field of view and player
//    type.  However, it is still the responsibility of the the systems to use
//    or not use our member function.
//
//    3) Gimble coordinates:
//       X+ is along the gimbal/sensor boresight
//       Y+ is to the right of the gimbal boresight
//       Z+ is down
//
//    4) The matrix (see below) is a composite of this gimbal's angles and
//    all of our container gimbals' angles.  Therefore, this matrix transforms
//    vectors to(from) our player's body coordinate and not to(from) our conainer
//    gimbal's coordinate system.
//
//          Usage:
//             Vg = M * Vb
//             Vb  = Vg * M
//          Where:
//             M = Rx[roll] * Ry[pitch] * Rz[yaw] * T[xyz] * M[container]
//             Vg is a vector in gimbal coordinates
//             Vb is a vector in body coordinates
//
//
//
//
// Form name: Gimbal
// Slots:
//
//    type                 (String)  Physical gimbal type: "mechanical" or "electronic" (default: ELECTRONIC)
//
//    location             (List)    Relative location vector (meters) [ x y z ] (default: 0,0,0)
//
//    initPosition         (List)    Initial position vector (radians) [ az el roll ] (default: 0,0,0)
//    initPosAzimuth       (Angle)   Initial azimuth positon (default: 0)
//    initPosElevation     (Angle)   Initial elevation positon (default: 0)
//    initPosRoll          (Angle)   Initial roll positon (default: 0)
//
//    azimuthLimits        (List)    Lower & upper azimuth limits (radians) [ left right ][ range: -pi to pi; else unlimited ]
//    azimuthLimitLeft     (Angle)   Left azimuth limit[ range: -pi to pi; else unlimited ]
//    azimuthLimitRight    (Angle)   Right azimuth limit[ range: -pi to pi; else unlimited ]
//    elevationLimits      (List)    Lower & upper elevation limits (radians) [ lower upper ][ range: -pi to pi; else unlimited ]
//
//    elevationLimitLower  (Angle)   Lower elevation limit[ range: -pi to pi; else unlimited ]
//    elevationLimitUpper  (Angle)   Upper elevation limit[ range: -pi to pi; else unlimited ]
//
//    rollLimits           (Angle)   Roll limit vector (radians) [ lower upper ][ range: -pi to pi; else unlimited ]
//    rollLimitLower       (Angle)   Lower roll limit[ range: -pi to pi; else unlimited ]
//    rollLimitUpper       (Angle)   Upper roll limit[ range: -pi to pi; else unlimited ]
//
//    maxRates             (List)    Max mechanical rate vector (rad/sec) [ az el roll ]
//    maxRateAzimuth       (Angle)   Max "mechanical" azimuth rate    (Angle/sec) (default: D2RCC * 120.0)
//    maxRateElevation     (Angle)   Max "mechanical" elevation rate  (Angle/sec) (default: D2RCC * 120.0)
//    maxRateRoll          (Angle)   Max "mechanical" roll rate       (Angle/sec) (default: D2RCC * 120.0)
//
//    commandPosition      (List)    Commanded position vector (radians) [ az el roll ] (sets POSITION_SERVO)
//    commandPosAzimuth    (Angle)   Commanded azimtuh position    (sets POSITION_SERVO) (default: 0)
//    commandPosElevation  (Angle)   Commanded elevation position  (sets POSITION_SERVO) (default: 0)
//    commandPosRoll       (Angle)   Commanded roll position       (sets POSITION_SERVO) (default: 0)
//
//    commandRates         (List)    Commanded rate vector (rad/sec) [ az el roll ] (sets RATE_SERVO)
//    commandRateAzimuth   (Angle)   Commanded azimuth rate     (Angle/sec) (sets RATE_SERVO)
//    commandRateElevation (Angle)   Commanded elevation rate  (Angle/sec) (sets RATE_SERVO)
//    commandRateRoll      (Angle)   Commanded roll rate        (Angle/sec) (sets RATE_SERVO)
//
//    terrainOcculting     (Number)  Enable terrain occulting of tye players of interest (default: false)
// 
//    playerOfInterestTypes (PairStream)   List of layer of interest types (default: all types )
//                                         Valid types: { "air" "ground" "weapon" "ship" "building" "lifeform" "space" }
//
//    maxPlayersOfInterest  (Number)         Max number of players of interest (default: 200)
//    maxRange2PlayersOfInterest (Distance)  Max range to players of interest or zero for all (default: 0)
//    maxAngle2PlayersOfInterest (Angle)     Max angle of gimbal boresight to players of interest or zero for all (default: 0)
//    localPlayersOfInterestOnly (Number)    Sets the local only players of interest flag (default: false)
//
//    useWorldCoordinates  (Number)          Using player of interest's world (ECEF) coordinate system (default: false)
//    useOwnHeadingOnly    <Number>          Whether only the ownship heading is used by the target data block (default: true)
//    earthRadius          (Distance)        Earth radius or zero to use ownship's earth radius (default: 0 -- use ownship's earth radius)
//
//
// Events:
//    RF_EMISSION       (Emission)  Default handler: Pass emissions to subcomponents.
//
//
//  Handy support functions
//      limitVec(osg::Vec2d& vec, osg::Vec2d& lim)
//      limitVec(osg::Vec2d& vec, osg::Vec2d& ll, osg::Vec2d& ul)
//          Symmetrical and asymmetrical limit checks
//
//------------------------------------------------------------------------------
class Gimbal : public System  
{
   DECLARE_SUBCLASS(Gimbal,System)

public:

    // Servo type 
    enum Type {
       MECHANICAL,   // Mechanical gimbal
       ELECTRONIC    // Electronic servo (e.g., phased array)
    };

    // Servo Mode
    enum ServoMode {
       FREEZE_SERVO,    // Servo is frozen 
       RATE_SERVO,      // Rate servo
       POSITION_SERVO   // Position servo
    };

    // Position/velocy vector indexes
    enum {
       AZ_IDX,    // Azimuth index
       ELEV_IDX,  // Elevation index
       ROLL_IDX   // Roll index
    };

   // Max number of players of interest
    enum { MAX_PLAYERS = 4000 };

public:  // Public section
   Gimbal();

   // Returns 'true' if servo is of type 'tt'.
   bool isGimbalType(const Type tt) const          { return (tt == type); }

   // Returns the type of servo: { MECHANICAL, ELECTRONIC }
   Type getGimbalType() const                      { return type; }

   // Returns the servo mode: { FREEZE_SERVO, RATE_SERVO, POSITION_SERVO }
   ServoMode getServoMode() const                  { return servoMode; }

   // Returns the current position vector [ az el roll ] (radians)
   const osg::Vec3d& getPosition() const           { return pos; }

   double getAzimuth() const     { return pos[AZ_IDX]; }    // Returns current azimuth position (rad)
   double getElevation() const   { return pos[ELEV_IDX]; }  // Returns current elevation position (rad)
   double getRoll() const        { return pos[ROLL_IDX]; }  // Returns current roll position (rad)
   double getAzimuthD() const;                              // Returns current azimuth position (deg)
   double getElevationD() const;                            // Returns current elevation position (deg)
   double getRollD() const;                                 // Returns current roll position (deg)

   // Returns true if the gimbal position is at the commanded position,
   // with a position tolerance of 'tol' radians.  The default tolerance
   // is 1/10th of a degree.
   virtual bool isPositioned(const double tol = -1) const;

   // Returns true if the gimbal is at a 'physical' limit
   virtual bool isAtLimits() const;

   const osg::Vec3d& getRates() const { return rate; }         // Returns the current rate vector (rad/sec)
   double getAzimuthRate() const    { return rate[AZ_IDX]; }   // Returns current azimuth rate (rad/sec)
   double getElevationRate() const  { return rate[ELEV_IDX]; } // Returns current elevation rate (rad/sec)
   double getRollRate() const       { return rate[ROLL_IDX]; } // Returns current roll rate (rad/sec)

   bool isFastSlewMode() const      { return fastSlew; }       // Returns true if the servo is the 'fast' slewing mode.
   bool isSlowSlewMode() const      { return !fastSlew; }      // Returns true if the servo is the 'slow' slewing mode.

   const osg::Vec3d& getCmdPosition() const { return cmdPos; } // Returns the commanded position vector [ az el roll ] (radians)
   double getCmdAz() const          { return cmdPos[AZ_IDX]; } // Returns commanded azimuth position (rad)
   double getCmdElev() const        { return cmdPos[ELEV_IDX]; }  // Returns commanded elevation position (rad)
   double getCmdRoll() const        { return cmdPos[ROLL_IDX]; }  // Returns commanded roll position (rad)
   
   const osg::Vec3d& getCmdRates() const { return cmdRate; }         // Returns the current rate vector (rad/sec)
   double getCmdAzRate() const         { return cmdRate[AZ_IDX]; }   // Returns commanded azimuth rate (rad/sec)
   double getCmdElevRate() const       { return cmdRate[ELEV_IDX]; } // Returns commanded elevation rate (rad/sec)
   double getCmdRollRate() const       { return cmdRate[ROLL_IDX]; } // Returns commanded roll rate (rad/sec)

   const osg::Vec3d& getLocation() const  { return location; } // Returns the location vector of the gimbal on it's parent container  (meters)
   const osg::Matrixd& getRotMat() const { return tm; }        // Returns the gimbal's rotational matrix
                                                               //    body (nose/right/down) <== > this.gimbal(x,y,z)
                                                               //    Usage:
                                                               //       Vg = M * Vb
                                                               //       Vb  = Vg * M
                                                               //    Where:
                                                               //       M = Rx[roll] * Ry[pitch] * Rz[yaw] * T[xyz] * M[container]
                                                               //       Vg is a vector in gimbal coordinates
                                                               //       Vb is a vector in body coordinates

   void getAzimuthLimits(double* const leftLim, double* const rightLim) const;   // Returns the physical azimuth limits (rad)
   void getElevationLimits(double* const lowerLim, double* const upperLim) const;// Returns the physical elevation limits (rad)
   void getRollLimits(double* const lowerLim, double* const upperLim) const;     // Returns the physical roll limits (rad)

   // Returns the maximum mechanical rates (rad/sec)
   void getMaxRates(double* const azMaxRate, double* const ezMaxRate, double* const rollMaxRate) const;

   double getMaxRange2PlayersOfInterest() const { return maxRngPlayers; }   // Max range to players of interest or zero for all (meters)
   double getMaxAngle2PlayersOfInterest() const { return maxAnglePlayers; } // Max angle of gimbal boresight to players of interest or zero for all (rad)
   unsigned int getPlayerOfInterestTypes() const { return playerTypes; }    // Player of interest types (Player::MajorType bit-wise or'd)
   unsigned int getMaxPlayersOfInterest() const  { return maxPlayers; }     // Max number of players of interest (i.e., size of the arrays)
   bool isLocalPlayersOfInterestOnly() const { return localOnly; }          // Local only players of interest flag
   bool isTerrainOccultingEnabled() const  { return terrainOcculting; }     // Terrain occulting enabled flag
   bool isUsingWorldCoordinates() const    { return useWorld; }             // Returns true if using player of interest's world coordinates
   bool isUsingHeadingOnly() const         { return ownHeadingOnly; }       // Returns true if using players heading only
   double getEarthRadius() const;                                           // Returns earth radius (meters)
   virtual bool fromPlayerOfInterest(const Emission* const em);             // Returns true if this emission is from a player of interest

   virtual bool setMaxRange2PlayersOfInterest(const double meters);        // Max range to players of interest or zero for all (meters)
   virtual bool setMaxAngle2PlayersOfInterest(const double radians);       // Max angle of gimbal boresight to players of interest or zero for all (rad)
   virtual bool setPlayerOfInterestTypes(const unsigned int typeMask);     // Player of interest types (Player::MajorType bit-wise or'd)
   virtual bool setMaxPlayersOfInterest(const unsigned int n);             // Max number of players of interest (i.e., size of the arrays)
   virtual bool setLocalPlayersOfInterestOnly(const bool flg);             // Sets the local only players of interest flag
   virtual bool setTerrainOccultingEnabled(const bool flg);                // Sets the terrain occulting enabled flag
   virtual bool setUseWorld(const bool flg);                               // Sets the using world coordinates flag
   virtual bool setOwnHeadingOnly(const bool flg);                         // Use only the ownship player's heading to when transforming between body and local NED

   // Process the Players-Of-Interest (POI) list
   virtual unsigned int processPlayersOfInterest(Basic::PairStream* const poi);

   // Sets the servo mode: { FREEZE_SERVO, RATE_SERVO, POSITION_SERVO }
   // Returns false if the mode could not be changed
   virtual bool setServoMode(const ServoMode m);

   virtual bool setPosition(const double azim, const double elev);                     // Sets the current az & el positions (rad)
   virtual bool setPosition(const double azim, const double elev, const double roll);  // Sets the current az, el and roll positions (rad)

   virtual bool setMaxRates(const double azMaxRate, const double ezMaxRate);           // Sets the max mechanical az & el rates (rad/sec)
   virtual bool setMaxRates(const double azMaxRate, const double ezMaxRate, const double rollMaxRate);   // Sets the max mechanical az, el and roll rates (rad/sec)

   virtual bool setCmdPos(const osg::Vec2d& p);                                        // Sets the commanded position vector (rad)
   virtual bool setCmdPos(const osg::Vec3d& p);                                        // Sets the commanded position vector (rad)
   virtual bool setCmdPos(const double az, const double el, const double roll = 0);    // Sets the commanded az, el and roll positions (rad)

   virtual bool setCmdRate(const osg::Vec2d& r);                                       // Sets the commanded rate vector (rad/sec)
   virtual bool setCmdRate(const osg::Vec3d& r);                                       // Sets the commanded rate vector (rad/sec)
   virtual bool setCmdRate(const double azRate, const double elRate);                  // Sets the commanded az & el rates (rad/sec)
   virtual bool setCmdRate(const double azRate, const double elRate, const double rollRate); // Sets the commanded az, el and roll rates (rad/sec)

   virtual bool setAzimuthLimits(const double leftLim, const double rightLim);         // Sets the mechanical azimuth limits (rad)
   virtual bool setElevationLimits(const double lowerLim, const double upperLim);      // Sets the mechanical elevation limits (rad)
   virtual bool setRollLimits(const double lowerLim, const double upperLim);           // Sets the mechanical roll limits (rad)

   virtual bool setLocation(const double x, const double y, const double z);           // Sets the location vector of the gimbal on it's parent container (meters)

   virtual bool setFastSlewMode(const bool flg);                                       // Sets the servo 'fast' or 'slow' slewing mode


   // Event handler(s)
   virtual bool onRfEmissionEvent(Emission* const);                 // Handles R/F emission events

   // Slot functions
   virtual bool setSlotType(const Basic::String* const msg);             // Physical gimbal type: "mechanical" or "electronic"
   virtual bool setSlotLocation(const Basic::List* const msg);           // Relative location vector (meters) [ x y z ]

   virtual bool setSlotPosition(const Basic::List* const msg);           // Initial positon vector (radians) [ az el roll ]
   virtual bool setSlotPosAzimuth(const Basic::Angle* const msg);        // Initial azimuth positon
   virtual bool setSlotPosElevation(const Basic::Angle* const msg);      // Initial elevation positon
   virtual bool setSlotPosRoll(const Basic::Angle* const msg);           // Initial roll positon 

   virtual bool setSlotAzimuthLimits(const Basic::List* const msg);      // Azimuth limit vector (radians) [ left right ]
   virtual bool setSlotAzimuthLimitLeft(const Basic::Angle* const msg);  // Left azimuth limit
   virtual bool setSlotAzimuthLimitRight(const Basic::Angle* const msg); // Right azimuth limit

   virtual bool setSlotElevationLimits(const Basic::List* const msg);    // Elevation limit vector (radians) [ lower upper ]
   virtual bool setSlotElevationLower(const Basic::Angle* const msg);    // Lower elevation limit
   virtual bool setSlotElevationUpper(const Basic::Angle* const msg);    // Upper elevation limit

   virtual bool setSlotRollLimits(const Basic::List* const msg);         // Roll limit vector (radians) [ lower upper ]
   virtual bool setSlotRollLimitLower(const Basic::Angle* const msg);    // Lower roll limit 
   virtual bool setSlotRollLimitUpper(const Basic::Angle* const msg);    // Upper roll limit 

   virtual bool setSlotMaxRates(const Basic::List* const msg);           // Max "mechanical" rate vector (rad/sec) [ az el roll ]
   virtual bool setSlotMaxRateAzimuth(const Basic::Angle* const msg);    // Max "mechanical" azimuth rate (Basic::Angle/sec)
   virtual bool setSlotMaxRateElevation(const Basic::Angle* const msg);  // Max "mechanical" elevation rate (Basic::Angle/sec)
   virtual bool setSlotMaxRateRoll(const Basic::Angle* const msg);       // Max "mechanical" roll rate (Basic::Angle/sec)

   virtual bool setSlotCmdPos(const Basic::List* const msg);             // Commanded position vector  [ az el roll ] (sets POSITION_SERVO)
   virtual bool setSlotCmdPosAzimuth(const Basic::Angle* const msg);     // Commanded azimtuh position  (sets POSITION_SERVO)
   virtual bool setSlotCmdPosElevation(const Basic::Angle* const msg);   // Commanded elevation position (sets POSITION_SERVO)
   virtual bool setSlotCmdPosRoll(const Basic::Angle* const msg);        // Commanded roll position  (sets POSITION_SERVO)

   virtual bool setSlotCmdRate(const Basic::List* const msg);            // Commanded rate vector (rad/sec) [ az el roll ] (sets RATE_SERVO)
   virtual bool setSlotCmdRateAzimuth(const Basic::Angle* const msg);    // Commanded azimuth rate (sets RATE_SERVO)
   virtual bool setSlotCmdRateElevation(const Basic::Angle* const msg);  // Commanded elevation rate (sets RATE_SERVO)
   virtual bool setSlotCmdRateRoll(const Basic::Angle* const msg);       // Commanded roll rate (sets RATE_SERVO)

   virtual bool setSlotTerrainOcculting(const Basic::Number* const msg); // Enable target terrain occulting (default: false)

   virtual bool setSlotPlayerTypes(const Basic::PairStream* const msg);  // Player of interest types (default: 0 )
   virtual bool setSlotMaxPlayers(const Basic::Number* const msg);       // Max number of players of interest (default: 0)

   // Max range to players of interest or zero for all (meters)
   virtual bool setSlotMaxRange2PlayersOfInterest(const Basic::Distance* const msg);

   // Max angle of gimbal boresight to players of interest or zero for all (rad)
   virtual bool setSlotMaxAngle2PlayersOfInterest(const Basic::Angle* const msg);

   // Sets the local only players of interest flag
   virtual bool setSlotLocalPlayersOfInterestOnly(const Basic::Number* const msg);

   // Using player of interest's world (ECEF) coordinate system
   virtual bool setSlotUseWorldCoordinates(const Basic::Number* const msg);

   // Use only the ownship player's heading to when transforming between body and local NED
   virtual bool setSlotUseOwnHeadingOnly(const Basic::Number* const msg);  

   // Earth radius used by Tdb for earth masking
   virtual bool setSlotEarthRadius(const Basic::Distance* const msg);  

   static void limitVec(osg::Vec2d& vec, const osg::Vec2d& lim);
   static void limitVec(osg::Vec3d& vec, const osg::Vec3d& lim);
   static void limitVec(osg::Vec2d& vec, const osg::Vec2d& ll, const osg::Vec2d& ul);
   static void limitVec(osg::Vec3d& vec, const osg::Vec3d& ll, const osg::Vec3d& ul);

   // Component Interface
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

protected:
   virtual void servoController(const double dt = 0.0);
   virtual bool setType(const Type rt);
   virtual void updateMatrix();

   Tdb* getCurrentTDB();               // Get the current TDB (pre-ref())
   const Tdb* getCurrentTDB() const;   // Get the current TDB (pre-ref(); const version)

   // Set the current TDB
   bool setCurrentTdb(Tdb* const newTdb);

   // System Interface -- Callbacks by phase
   virtual void dynamics(const LCreal dt);    // Phase 0

   // Basic::Component protected interface
   virtual bool shutdownNotification();

private:
   void initData();

   static const double defaultTolerance;

   Type        type;          // Mechanical or Electronic gimbal (affects maxRates)
   ServoMode   servoMode;     // Gimbal's servo mode
   bool        fastSlew;      // Fast slewing mode: tell us if we are slewing fast (true) or scanning (slewing slow (false))

   osg::Matrixd tm;           // Transformation matrix (to/from the player's coordinate system)
   osg::Vec3d  pos;           // Current gimbal position      (rad)
   osg::Vec3d  rate;          // Current velocity             (rad/sec)
   osg::Vec3d  cmdPos;        // Commanded position           (rad)
   osg::Vec3d  cmdRate;       // Commanded rate               (rad/sec)
   osg::Vec3d  location;      // Gimbal's location on parent  (meters)
   bool        atLimit;       // Gimbal is at a limit

   osg::Vec3d  maxRate;       // Max mechanical rate of gimbal (rad/sec)
   osg::Vec3d  lowLimits;     // left/lower gimbal limits     (rad)
   osg::Vec3d  highLimits;    // right/upper gimbal limits    (rad)

   osg::Vec3d  initPos;       // Initial gimbal position      (rad)
   osg::Vec3d  initCmdPos;    // Initial commanded position   (rad)
   osg::Vec3d  initCmdRate;   // Initial commanded rate       (rad/sec)

   double    maxRngPlayers;   // Max range for players of interest or zero for all (meters)
   double    maxAnglePlayers; // Max angle of gimbal boresight for players of interest (or zero for all) (rad)
   unsigned int playerTypes;  // Player of interest type mask
   unsigned int maxPlayers;   // Max number of players of interest (i.e., size of the arrays)
   bool     localOnly;        // Local players of interest only 
   bool     terrainOcculting; // Target terrain occulting enabled flag
   bool     useWorld;         // Using player of interest's world coordinates
   bool     ownHeadingOnly;   // Whether only the ownship heading is used by the target data block
   double   earthRadius;      // Earth radius or zero to use ownship's earth radius (meters)

   SPtr<Tdb> tdb;             // Current Target Data Block

};

} // End Simulation namespace
} // End Eaagles namespace

#endif
