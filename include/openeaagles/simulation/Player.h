//------------------------------------------------------------------------------
// Class: Player
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Player_H__
#define __Eaagles_Simulation_Player_H__

#include "openeaagles/basic/Component.h"

#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/osg/Quat"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

#include "openeaagles/simulation/SynchronizedState.h"

namespace Eaagles {
   namespace Basic {
      class Angle;
      class Boolean;
      class Distance;
      class LatLon;
      class List;
      class Time;
   }
namespace Simulation {

// Major component types
class Datalink;
class DynamicsModel;
class Gimbal;
class IrSystem;
class Navigation;
class OnboardComputer;
class Pilot;
class Radio;
class RfSensor;
class StoresMgr;
class Weapon;

// Other item types
class Emission;
class Nib;
class IrQueryMsg;
class IrSignature;
class RfSignature;
class Simulation;
class Track;

//------------------------------------------------------------------------------
// Class: Player
//
// Description: Common base class for all players (e.g., aircraft, ground vehicles, etc.)
//              Provides interfaces for player IDs, types, status, state vectors,
//              control flags, subcomponent lists, event handling, etc.
//
//
// --------------------
// Form name: Player
// Slots --
//
//    ! ---
//    ! Player's initial position can be set using any one (but only one)
//    ! of the following three coordinate systems.  Which initial coordinate
//    ! system used will determine the default coordinate system is used to
//    ! update the player's position.
//    ! ---
//
//    ! 1: Player's initial position vector (NED) on the simulation's gaming area
//
//    initXPos       <Basic::Distance>    ! X position (+north) (default: 0)
//    initXPos       <Basic::Number>      ! X position (+north) (meters)
//
//    initYPos       <Basic::Distance>    ! Y position (+east) (default: 0)
//    initYPos       <Basic::Number>      ! Y position (+east)(meters)
//
//    initAlt        <Basic::Distance>    ! Altitude (+up) (default: 0)
//    initAlt        <Basic::Number>      ! Altitude (+up) (meters)
//
//    initPosition   <Basic::List>        ! Position Vector: meters [ north east down ]
//                                        !   *** initPosition will be removed in later release (use below slots instead) ***
//
//    ! 2: Player's initial geodetic position
//
//    initLatitude   <Basic::LatLon>      ! Latitude (default: 0)
//    initLatitude   <Basic::Angle>       ! Latitude
//    initLatitude   <Basic::Number>      ! Latitude (degs)
//
//    initLongitude  <Basic::LatLon>      ! Longitude (default: 0)
//    initLongitude  <Basic::Angle>       ! Longitude
//    initLongitude  <Basic::Number>      ! Longitude (degs)
//
//    initAlt        <Basic::Distance>    ! Altitude (HAE) (+up)  ### NES Is this being used?
//    initAlt        <Basic::Number>      ! Altitude (HAE) (+up) (meters)
//
//
//    ! 3: Player's initial world position
//
//    initGeocentric <Basic::List>        ! Position vector [ x y z ] (ECEF) (meters) (default: 0,0,0)
//
//
//    ! ---
//    ! Player's initial Euler angles
//    ! ---
//    initRoll       <Basic::Angle>       ! Initial roll angle (default: 0)
//    initRoll       <Basic::Number>      ! Initial roll angle (radians)
//
//    initPitch      <Basic::Angle>       ! Initial pitch angle (default: 0)
//    initPitch      <Basic::Number>      ! Initial pitch angle (radians)
//
//    initHeading    <Basic::Angle>       ! Initial heading angle (default: 0)
//    initHeading    <Basic::Number>      ! Initial heading angle (radians)
//
//    initEuler      <Basic::List>        ! Initial Euler angles: radians [ roll pitch yaw ]
//
//    ! ---
//    ! Player's initial velocity
//    ! ---
//    initVelocity   <Basic::Number>      ! Initial Velocity: meters/sec (default: 0)
//
//    ! ---
//    ! Player's type, id and and other parameters
//    ! ---
//    type           <Basic::String>      ! Type of player vehicle ("F-16A", "Tank", "SA-6", etc.) (default: 0)
//    id             <Basic::Integer>     ! Player id  [ 1 .. 65535 ] (default: 0)
//    side           <Basic::String>      ! Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE } (default: GRAY)
//    mode           <Basic::String>      ! Initial player mode ( INACTIVE, ACTIVE, DEAD ) (default: ACTIVE)
//    useCoordSys    <Basic::String>      ! Coord system to use for updating player position { WORLD, GEOD, LOCAL }
//                                        !   (default: based on the coord system used to set the initial position)
//
//    signature      <RfSignature>        ! Player's RCS signature (default: 0)
//    irSignature    <IrSignature>        ! Player's IR signature (default: 0)
//    camouflageType <Basic::Integer>     ! User defined camouflage type (positive integer or zero for none) (default: 0)
//
//    terrainElevReq <Basic::Boolean>     ! Terrain elevation from the OTW system is requested; otherwise use DTED (default: false)
//    interpolateTerrain <Basic::Boolean> ! Interpolate our DTED terrain elevation data (default: false)
//    terrainOffset  <Basic::Distance>    ! Ground clamp offset from terrain to player's CG (Basic::Distance)
//
//    positionFreeze <Basic::Boolean>     ! Position freeze (default: false)
//    altitudeFreeze <Basic::Boolean>     ! Altitude freeze (default: false)
//    attitudeFreeze <Basic::Boolean>     ! Attitude freeze (default: false)
//    fuelFreeze     <Basic::Boolean>     ! Fuel freeze (default: false)
//    crashOverride  <Basic::Boolean>     ! Crash Override (i.e., ignore collision and crash events)(default: false)
//    killOverride   <Basic::Boolean>     ! Kill/Damage Override -- player can not be killed/damaged (default: false)
//    killRemoval    <Basic::Boolean>     ! If true destroyed players are set to KILLED and are eventually removed (default: false)
//    enableNetOutput <Basic::Boolean>    ! Enable network output of this player (default: true)
//
//    dataLogTime    <Basic::Time>        ! Time between player data samples to an optional data
//                                        ! logger, or zero if none (default: 0)
//
//    ! ---
//    ! Angular test rates:
//    !     If non-zero the Euler angles are updated using the body angular rates.
//    !     For use only without a dynamics model.
//    ! ---
//    testRollRate   <Basic::Angle>       ! Test roll rate (body) (units per second) (default: 0)
//    testPitchRate  <Basic::Angle>       ! Test pitch rate (body) (units per second) (default: 0)
//    testYawRate    <Basic::Angle>       ! Test heading rate (body) (units per second) (default: 0)
//    testBodyAxis   <Basic::Boolean>     ! Test rates are in the body axis else they're Euler rates (default: false)
//
//
// --------------------
// Events:
//    KILL_EVENT              <Player>             ! Killed by
//    CRASH_EVENT             <Player>             ! Collision with
//    CRASH_EVENT             (none)               ! Crashed
//    RF_EMISSION             <Emission>           ! Hit by RF emission
//    RF_REFLECTIONS_REQUEST  <Basic::Component>   ! Requests for reflected R/F emissions (must re-request once a second)
//    RF_REFLECTIONS_CANCEL   <Basic::Component>   ! Cancel requests for reflected R/F emissions
//    WPN_REL_EVENT           <Basic::Boolean>     ! Weapon release switch with position
//    WPN_REL_EVENT           (none)               ! Weapon release switch (single shot)
//    TRIGGER_SW_EVENT        <Basic::Boolean>     ! Trigger switch with position
//    TRIGGER_SW_EVENT        (none)               ! Trigger switch (single shot)
//    TGT_STEP_EVENT          (none)               ! Target step switch
//    DATALINK_MESSAGE        <Basic::Object>      ! Hit with a datalink message
//    IR_QUERY_MSG            <IrQueryMsg>         ! IR seeker requests signature
//
//
//
// Coordinate systems
//
//    a) Geocentric or world coordinates (Earth Centered, Earth Fixed - ECEF); origin
//       is at the center X+ points to zero latitude and zero longitude of the earth;
//       (i.e., equator at the Greenwich meridian), Z+ points to the north pole and Y
//       is the cross product of Z into X; ECEF rotates with the earth
//
//    b) Geodetic coordinates: latitude, longitude and height above the
//       ellipsoid (HAE)
//
//    c) Simulation's gaming area coordinates (NED); the XY plane is tangent to
//       and centered at the simulation's geodetic gaming area reference point;
//       the Z or down axes is perpendicular to the tangent plane (see Simulation.h).
//       Use Simulation::getWorldMat() to transform vectors between ECEF and
//       this tangent plane.
//
//    d) Inertial coordinates or local tangent plane (NED); the XY plane is tangent
//       to and centered at the player's geodetic present position; the Z or
//       down axes is perpendicular to the tangent plane.  Use Player::getWorldMat()
//       to transform vectors from ECEF to this tangent plane.
//
//    e) Body coordinates: nose(x+), right wing(y+), and down(z+);  the orientation
//       of the player's body coordinates to the player's local tangent plane is
//       defined by the Euler angles (see below).  Use Player::getRotMat() to transform
//       vectors between the player's inertial coordinates and body coordinates.
//
//
// Player's position:
//
//    a) Represented in three of the coordinate systems
//
//          1) Geocentric or world (ECEF) position vector [ x y z ]
//
//          2) Geodetic position: latitude, longitude and altitude (HAE)
//
//          3) NED position vector on the simulation's gaming area.
//             -- May not be valid for all player types (e.g., SPACE_VEHICLE), so
//                check isPositionVectorValid()
//             -- Use Simulation::getWorldMat() to get the ECEF to NED
//                transformation matrix for this tangent plane.
//
//    b) Setting the position in any one coordinate system will set the
//       position for all three coordinate systems and will compute the
//       world matrix, getWorldMat().
//
//    c) Set the initial (i.e., reset) present position using one of the three
//       sets of initial position slots defined above.
//
//    d) The coordinate system that's used to set the player's position will
//       determine which coordinate system is used to update the player's position
//       (see "Updating Position" below).
//
//
// Player's orientation:
//    Inertial (player's local tangent plane) <==> body
//
//    a) Represented in three formats
//       1) As Euler angles; Phi (roll), theta (pitch) and psi (yaw; true heading)
//       2) As a rotational matrix
//       3) As a quaternion
//
//    b) Setting the player's orientation in any one of the three formats will set
//       the player's orientation in the other formats as well.
//
//    c) The player's dynamics model is responsible for updating the player's
//       orientation (e.g., rotational equations of motion).  Also, the
//       dynamics model is responsible for implementing the attitude freeze
//       (i.e., setAttitudeFreeze()).
//
//
// Player's velocity and acceleration vectors:
//
//    a) The velocity and acceleration vectors exist for three coordinate systems;
//       world, inertial and body coordinates.
//
//    b) Setting the player's velocity or acceleration in any one of the three
//       coordinate systems will set the vectors in the other systems as well.
//
//    c) Set the velocity and acceleration vectors after the player's position and
//       orientation have been set (i.e., after the transformation matrices have
//       been computed).
//
//
// Player's angular rates:
//
//    a) The angular rates exist for both body and world coordinates.
//
//    b) Setting the player's angular rates in either coordinate system will set
//       the angular rates in both systems.
//
//    c) Set the angular rates after the player's position and orientation have
//       been set (i.e., after the transformation matrices have been computed).
//
//    d) Angular test rates:
//
//       The slots 'testRollRate', 'testPitchRate' and 'testYawRate' are used to
//       define test angular rates.  If the slot 'testBodyAxis' is true then
//       these are body rates otherwise they're Euler rates.
//
//       When any one of these test rates are non-zero then the player's Euler
//       angles are updated by this Player class.  Since the player's dynamics
//       model is normally responsible for updating the Euler angles, these
//       test rates should be used only on player's without a dynamics model
//       (e.g., a player that is part of a test-rig).  With each update of the
//       Euler angles, the player's total velocity along the body X axis is
//       reset to 'initVelocity'.
//
//
// Updating Position:
//
//    The function positionUpdate(), which is called from dynamics(), updates
//    the player's position unless the position is frozen or is slaved to the
//    dynamics model (i.e., the dynamics model is computing the position).
//    The positionUpdate() functions is also responsible for updating the
//    Euler angles if the 'angular test rates' are not zero.
//
//    The position is updated using either the player's world coordinates, geodetic
//    coordinates or gaming area position vector.  Which coordinate system used
//    is determined by which coordinate system was used to initially set the
//    player's position.  For example, the player's position will be updated in
//    world coordinates if the player's position was initially set using world
//    coordinates.
//
//    Dynamics models can inhibit positionUpdate() from updating the position
//    and/or altitude by using the 'slaved' flags on the set player position
//    functions (e.g., setPositionLLA()).
//
//    The functions setPositionFreeze() and setAltitudeFreeze() will freeze
//    (i.e., stop updating) the player's position and altitude as follows.
//
//       Coord Sys         Position Freeze      Altitude Freeze
//       -----------       ---------------      ---------------
//       Gaming area       X and Y              Z
//       Geodetic          lat and lon          altitude
//       World             X, Y and Z
//
//
//
// Player systems and subcomponents:
//
//    1) Player's top level system models are all defined using the player's
//       'components' slot.  Examples:
//
//          datalink: ( Datalink ... )             // Datalink model
//          dynamicsModel: ( DynamicsModel ... }   // Dynamics model
//          gimbals: ( Gimbal ... )                // Gimbal, antenna and/or seeker model
//          nav:   ( Navigation ... )              // Navigation model
//          obc:   ( OnboardComputer ... )         // Onboard computer model
//          pilot: ( Pilot ... )                   // Pilot model
//          radios:  ( Radio ... )                 // Radio model
//          sensors: ( RfSensor ... )              // R/F Sensor model
//          irSystems: (IrSystem ...)              // IR system model
//          sms:   ( StoresMgr ... )               // Stores management model
//
//          Where 'datalink' is the name give to the top level datalink object,
//          'dynamicsMode' is the name give to the dynamics model, etc.
//
//    2) There is one top level model for each major system.  Additional system
//       models are subcomponents to the top level model of the same type.
//       Examples:
//
//          a) Various navigational models (e.g., INS, GPS models) would be
//             subcomponents of one top level Navigational model (e.g., INav),
//             or you might have only a single (top level) GPS model.
//
//          b) Various gimbal models (e.g., antennas and EO/IR seekers) would be
//             subcomponents of one top level Gimbal model, or you may have only
//             a single (top level) antenna model.
//
//          c) A top level sensor manager may contain several R/F and IR sensors
//             as subcomponents.
//
//          d) The main on-board computer model can contain several subsystem
//             on-board computers and track managers.
//
//
// Shutdown event:
//
//    At shutdown, the parent object (e.g., the simulation) must send a SHUTDOWN_EVENT
//    event to this player, which will pass the event to all of the our components.
//
//
//------------------------------------------------------------------------------
class Player : public Basic::Component
{
   DECLARE_SUBCLASS(Player,Basic::Component)

public:

   // Player mode
   enum Mode {
      INACTIVE,         // Player is not being updated and is not being sent to the networks
      ACTIVE,           // Player is being updated and is being sent to the networks
      KILLED,           // Player was killed   (One of the dead conditions)
      CRASHED,          // Player crashed      (One of the dead conditions)
      DETONATED,        // Weapon player has detonated (One of the dead conditions) (Original & flyout weapons)
      PRE_RELEASE,      // Weapon player is created but not released (Flyout weapons only)
      LAUNCHED,         // Weapon player has been launched (Original weapons only)
      DELETE_REQUEST    // Request player removal from the active player list
   };

   // Major player type (bits)
   enum MajorType {
      GENERIC        = 0x01,  // Unknown type of player
      AIR_VEHICLE    = 0x02,  // Air Player
      GROUND_VEHICLE = 0x04,  // Ground Player
      WEAPON         = 0x08,  // Weapon Player
      SHIP           = 0x10,  // Ship or subsurface player
      BUILDING       = 0x20,  // Building
      LIFE_FORM      = 0x40,  // Life Form
      SPACE_VEHICLE  = 0x80   // Space Player
   };

   // Player sides (bits)
   enum Side {
      BLUE     = 0x01,        // Blue force
      RED      = 0x02,        // Red force
      YELLOW   = 0x04,        // 3rd force
      CYAN     = 0x08,        // 4th force
      GRAY     = 0x10,        // Neutral force
      WHITE    = 0x20         // Commercial/Civilian
   };

   // Coordinate system to use (or being used) for updating player position
   enum CoordSys {
      CS_NONE,    //
      CS_LOCAL,   // Local: NED from the center of the simulation gaming area
      CS_GEOD,    // Geodetic coordinates; latitude, longitude and HAE
      CS_WORLD    // World (ECEF) coordinates
   };

   enum { IROLL, IPITCH, IYAW };  //Euler angles
   enum { INORTH, IEAST, IDOWN }; //position vector

public:
   Player();

   // ---
   // Type and ID
   // ---

   virtual unsigned int getMajorType() const;      // The player's 'major type' enum
   bool isMajorType(const unsigned int tst) const; // True if player is of these (bit-wise or'd) major types

   const Basic::String* getType() const;           // The player's type string (.e.g, "F-16C")

   bool isID(const unsigned short tst) const;      // True if player's ID matches
   unsigned short getID() const;                   // The player's ID

   bool isName(const Basic::Identifier* const tst) const; // True if the player's name matches
   bool isName(const char* const tst) const;              // True if the player's name matches
   const Basic::Identifier* getName() const;              // The player's name

   Side getSide() const;                           // The 'side' that the player is on.
   bool isSide(const unsigned int tst) const;      // True if player is with one of these (bit-wise or'd) sides
   bool isNotSide(const unsigned int tst) const;   // True if player is not with one one of these (bit-wise or'd) sides

   // ---
   // State data
   // ---

   Mode getMode() const;                           // Current mode ( INACTIVE, ACTIVE, DETONATED ... )
   bool isActive() const;                          // True if player's mode is active
   bool isKilled() const;                          // True if player's mode is killed
   bool isCrashed() const;                         // True if player's mode is crashed
   bool isDetonated() const;                       // True if player has detonated (weapons only)
   bool isInactive() const;                        // True if player's mode is inactive
   bool isMode(const Mode tst) const;              // True if player is currently this mode
   bool isNotMode(const Mode tst) const;           // True if player is not currently this mode
   bool isDead() const;                            // True if player's mode is dead (Killed, Crashed, Detonated, etc....)

   double getRoll() const;                         // Roll Euler angle (radians) by default
   double getRollR() const;                        // Roll Euler angle (radians)
   double getRollD() const;                        // Roll Euler angle (degrees)
   double getSinRoll() const;                      // Sin of the Euler roll angle
   double getCosRoll() const;                      // Cos of the  Euler roll angle

   double getPitch() const;                        // Pitch Euler angle (radians) by default
   double getPitchR() const;                       // Pitch Euler angle (radians)
   double getPitchD() const;                       // Pitch Euler angle (degrees)
   double getSinPitch() const;                     // Sin of the pitch Euler angle
   double getCosPitch() const;                     // Cos of the  pitch Euler angle

   double getHeading() const;                      // Yaw Euler angle (radians) by default
   double getHeadingR() const;                     // Yaw Euler angle (radians)
   double getHeadingD() const;                     // Yaw Euler angle (degrees)
   double getSinHeading() const;                   // Sin of the yaw Euler angle
   double getCosHeading() const;                   // Cos of the  yaw Euler angle

   const osg::Vec3d& getEulerAngles() const;       // Euler angles (radians); geodetic (body/NED)
   const osg::Quat& getQuaternions() const;        // Rotational Quaternions

   const osg::Matrixd& getRotMat() const;          // Rotational Matrix: (directional cosines)
                                                   //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw]
                                                   //    Usage:
                                                   //       Vb = M * Vi
                                                   //       Vi  = Vb * M
                                                   //    Where: 'Vb' is a body vector; 'Vi' is an inertial vector

   const osg::Matrixd& getRotMatW2B() const;       // Rotational Matrix: world to body
                                                   //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw] * Ry[-(90+lat)] * Rz[lon]
                                                   //    Usage:
                                                   //       Vb = M * Vw
                                                   //       Vw  = Vb * M
                                                   //    Where: 'Vb' is a body vector; 'Vw' is a world (ECEF) vector

   const osg::Vec3d& getGeocEulerAngles() const;   // Geocentric (body/ECEF) Euler angles

   const osg::Vec3d& getAngularVelocities() const;       // Body angular rates (radians/second)
   const osg::Vec3d& getGeocAngularVelocities() const;   // Geocentric angular rates (radians/second)

   const osg::Vec3d& getGeocPosition() const; // Geocentric (ECEF) position vector [ x y z ] (meters)

   double getLatitude() const;                     // Player's latitude (degrees)
   double getLongitude() const;                    // Player's longitude (degrees)

   const osg::Matrixd& getWorldMat() const;        // World transformation matrix:
                                                   //    Local inertial tangent plane (NED) <==> World (ECEF)
                                                   //    Matrix: M = Ry[-(90+lat)] * Rz[lon]
                                                   //    Usage:
                                                   //       Vi = M * Vw;
                                                   //       Vw  = Vi * M
                                                   //    Where: 'Vw' is a world(ECEF) vector; 'Vi' is an inertial (NED) vector

   double getEarthRadius() const;                  // Earth radius at player's location (meters)

   bool getPositionLL(double* const lat, double* const lon) const;                     // Player's Lat/lon (degrees)
   bool getPositionLLA(double* const lat, double* const lon, double* const alt) const; // Player's Lat/Lon (degrees) and altitude (meters)

   double getXPosition() const;                    // North(+) or south(-) of the sim reference point (meters)
   double getYPosition() const;                    // East(+) or west(-) of the sim reference point (meters)
   const osg::Vec3d& getPosition() const;          // Position vector; NED from sim reference point (meters)
   bool isPositionVectorValid() const;             // Is the position vector valid

   double getAltitude() const;                     // Altitude HAE (meters) default
   double getAltitudeM() const;                    // Altitude HAE (meters)
   double getAltitudeFt() const;                   // Altitude HAE (feet)

   double getAltitudeAgl() const;                  // Altitude AGL (meters) default
   double getAltitudeAglM() const;                 // Altitude AGL (meters)
   double getAltitudeAglFt() const;                // Altitude AGL (feet)

   bool isTerrainElevationValid() const;           // Terrain elevation is valid (or at least was where it was set)
   LCreal getTerrainElevation() const;             // Terrain elevation at player (meters) default
   LCreal getTerrainElevationM() const;            // Terrain elevation at player (meters)
   LCreal getTerrainElevationFt() const;           // Terrain elevation at player (feet)

   LCreal getTotalVelocity() const;                // Total velocity (meters/second)
   LCreal getTotalVelocityFPS() const;             // Total velocity (feet/second)
   LCreal getTotalVelocityKts() const;             // Total velocity (knots)

   LCreal getGroundSpeed() const;                  // Ground speed (meters/second)
   LCreal getGroundSpeedFPS() const;               // Ground speed (feet/second)
   LCreal getGroundSpeedKts() const;               // Ground speed (knots)

   LCreal getGroundTrack() const;                  // Ground track (radians) default
   LCreal getGroundTrackR() const;                 // Ground track (radians)
   LCreal getGroundTrackD() const;                 // Ground track (degrees)

   const osg::Vec3d& getVelocity() const;          // Velocity vector; NED (meters/second)
   const osg::Vec3d& getAcceleration() const;      // Acceleration vector; NED ((meters/second)/second)

   const osg::Vec3d& getGeocVelocity() const;      // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   const osg::Vec3d& getGeocAcceleration() const;  // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)

   const osg::Vec3d& getVelocityBody() const;      // Body velocity vector: Nose/right/down (meters/second)
   const osg::Vec3d& getAccelerationBody() const;  // Body Acceleration vector: Nose/right/down ((meters/second)/second)

   virtual LCreal getGrossWeight() const;          // Player's gross weight (lbs)
   virtual LCreal getMach() const;                 // Player's mach number
   virtual LCreal getCG() const;                   // Player's Center-of-Gravity (%)


   // ---
   // Appearance
   // ---

   unsigned int getCamouflageType() const;         // Returns the user defined camouflage type (or zero for none)

   virtual bool isDestroyed() const;               // True if player is destroyed
   LCreal getDamage() const;                       // Returns the player's damage state - no damage(0.0) to destroyed (1.0)
   LCreal getSmoke() const;                        // Returns the player's smoke state - (0.0) to maximum (1.0)
   LCreal getFlames() const;                       // Returns the player's flames state - (0.0) to maximum (1.0)

   bool isJustKilled() const;                      // True if the player was just killed
   int killedByPlayerID() const;                   // ID of the attacking player that killed us

   // ---
   // Initialization data
   // ---
   const osg::Vec3d& getInitGeocentricPosition() const; // Geocentric (ECEF) initial position vector [ x y z ] (meters)
   bool isInitGeocentricPositionValid() const;

   double getInitLatitude() const;                 // Player's initial latitude (degrees)
   double getInitLongitude() const;                // Player's initial longitude (degrees)
   bool isInitLatLonValid() const;

   const osg::Vec2d& getInitPosition() const;      // Initial position vector; north and east of sim reference point (meters)
   bool isInitPositionValid() const;

   double getInitAltitude() const;                 // Initial altitude HAE (meters)

   const osg::Vec3d& getInitAngles() const;        // Initial Euler angles (radians)

   // ---
   // Control parameters
   // ---
   bool isPositionFrozen() const;                  // True if the player is frozen (paused)
   bool isAltitudeFrozen() const;                  // True if the player's altitude is frozen
   bool isAttitudeFrozen() const;                  // True if the player's attitude is frozen
   bool isFuelFrozen() const;                      // True if the player's fuel is frozen
   bool isCrashOverride() const;                   // True if the player is in crash override mode
   bool isKillOverride() const;                    // True if the player is in kill override mode
   bool isKillRemovalEnabled() const;              // True if the player's kill removal flag is set
   bool isAltitudeSlaved() const;                  // True if altitude is slaved to an external model
   bool isPositionSlaved() const;                  // True if position is slaved to an external model

   LCreal getTerrainOffset() const;                // Ground clamping offset from the terrain to our CG (meters)
   bool isDtedTerrainInterpolationEnabled() const; // Interpolate our terrain from DTED if true

   bool isTerrainElevationRequired() const;        // True if terrain elevation is required from the OTW system;
                                                   // otherwise we use DTED, if available

   CoordSys getCoordSystemInUse() const;           // Returns the coordinate system currently being to
                                                   // update the player's position

   // ---
   // Interoperability network data
   // ---
   bool isNetworkedPlayer() const;                 // True if this is a networked player (IPlayer)
   bool isLocalPlayer() const;                     // True if this is a local player

   int getNetworkID() const;                       // ID of a networked player's controlling network model
   Nib* getNib();                                  // Networked player's Nib object
   const Nib* getNib() const;                      // Networked player's Nib object  (const version)
   bool isSmoothing() const;                       // Networked player: is Dead Reckoning position smoothing active
   const osg::Vec3& getSmoothingRate() const;      // Networked player: position smoothing rate (meters/sec) (ECEF)

   bool isNetOutputEnabled() const;                // Is player output to the network enabled?
   Nib* getLocalNib(const unsigned int netId);     // Player's outgoing NIB(s)
   const Nib* getLocalNib(const unsigned int netId) const; // Player's outgoing NIB(s)  (const version)

   // ---
   // Internal autopilot controls
   // ---
   virtual bool isHeadingHoldOn() const;           // True if heading-hold mode is on?
   virtual double getCommandedHeading() const;     // Commanded (true) heading, default (radians)
   virtual double getCommandedHeadingD() const;    // Commanded (true) heading (degrees)
   virtual double getCommandedHeadingR() const;    // Commanded (true) heading (radians)

   virtual bool isVelocityHoldOn() const;          // True if velocity-hold mode is on?
   virtual double getCommandedVelocityKts() const; // Commanded (true) velocity (knots)
   virtual double getCommandedVelocityFps() const; // Commanded (true) velocity (feet/second)
   virtual double getCommandedVelocityMps() const; // Commanded (true) velocity (meters/second)

   virtual bool isAltitudeHoldOn() const;          // True if altitude-hold mode is on?
   virtual double getCommandedAltitude() const;    // Get commanded (HAE) altitude, default (meters)
   virtual double getCommandedAltitudeM() const;   // Get commanded (HAE) altitude (meters)
   virtual double getCommandedAltitudeFt() const;  // Get commanded (HAE) altitude (feet)

   // ---
   // Sync State Changes
   // ---
   const SynchronizedState& getSynchronizedState() const;

   // ---
   // Systems
   // ---

   Simulation* getSimulation();                                // Controlling simulation model
   const Simulation* getSimulation() const;                    // Controlling simulation model (const version)

   DynamicsModel* getDynamicsModel();                          // Player's dynamics model
   const DynamicsModel* getDynamicsModel() const;              // Player's dynamics model (const version)
   const Basic::Identifier* getDynamicsModelName() const;      // Name of the player's dynamics model

   Pilot* getPilot();                                          // Player's top level pilot model
   const Pilot* getPilot() const;                              // Player's top level pilot model (const version)
   const Basic::Identifier* getPilotName() const;              // Name of the player's top level pilot model
   Pilot* getPilotByName(const char* const name);              // Returns a pilot model by its name
   Basic::Pair* getPilotByType(const std::type_info& type);    // Returns a pilot model by its type

   StoresMgr* getStoresManagement();                           // Player's stores (weapons, fuel, etc...) manager model
   const StoresMgr* getStoresManagement() const;               // Player's stores (weapons, fuel, etc...) manager model (const version)
   const Basic::Identifier* getStoresManagementName() const;   // Name of the player's stores (weapons, fuel, etc...) manager model

   Datalink* getDatalink();                                    // Player's top level Datalink model
   const Datalink* getDatalink() const;                        // Player's top level Datalink (const version)
   const Basic::Identifier* getDatalinkName() const;           // Name of the player's top level Datalink model
   Datalink* getDatalinkByName(const char* const name);        // Returns a Datalink model by its name
   Basic::Pair* getDatalinkByType(const std::type_info& type); // Returns a Datalink model by its type

   Gimbal* getGimbal();                                        // Player's top level Gimbal model
   const Gimbal* getGimbal() const;                            // Player's top level Gimbal (const version)
   const Basic::Identifier* getGimbalName() const;             // Name of the player's top level Gimbal model
   Gimbal* getGimbalByName(const char* const name);            // Returns a Gimbal model by its name
   Basic::Pair* getGimbalByType(const std::type_info& type);   // Returns a Gimbal model by its type

   Navigation* getNavigation();                                // Player's top level Navigation model
   const Navigation* getNavigation() const;                    // Player's top level Navigation (const version)
   const Basic::Identifier* getNavigationName() const;         // Name of the player's top level Navigation model
   Navigation* getNavigationByName(const char* const name);    // Returns a Navigation model by its name
   Basic::Pair* getNavigationByType(const std::type_info& type); // Returns a Navigation model by its type

   OnboardComputer* getOnboardComputer();                      // Player's top level OnboardComputer model
   const OnboardComputer* getOnboardComputer() const;          // Player's top level OnboardComputer (const version)
   const Basic::Identifier* getOnboardComputerName() const;    // Name of the player's top level OnboardComputer model
   OnboardComputer* getOnboardComputerByName(const char* const name); // Returns an OnboardComputer model by its name
   Basic::Pair* getOnboardComputerByType(const std::type_info& type); // Returns an OnboardComputer model by its type

   Radio* getRadio();                                             // Player's top level Radio model
   const Radio* getRadio() const;                              // Player's top level Radio (const version)
   const Basic::Identifier* getRadioName() const;              // Name of the player's top level Radio model
   Radio* getRadioByName(const char* const name);              // Returns a Radio model by its name
   Basic::Pair* getRadioByType(const std::type_info& type);    // Returns a Radio model by its type

   RfSensor* getSensor();                                      // Player's top level R/F sensor model
   const RfSensor* getSensor() const;                          // Player's top level R/F sensor model (const version)
   const Basic::Identifier* getSensorName() const;             // Name of the player's top level R/F sensor model
   RfSensor* getSensorByName(const char* const name);          // Returns a R/F sensor model by its name
   Basic::Pair* getSensorByType(const std::type_info& type);   // Returns a R/F sensor model by its type

   IrSystem* getIrSystem();                                    // Player's top level IR sensor model
   const IrSystem* getIrSystem() const;                        // Player's top level IR sensor model (const version)
   const Basic::Identifier* getIrSystemName() const;           // Name of the player's top level IR sensor model
   IrSystem* getIrSystemByName(const char* const name);        // Returns a IR sensor model by its name
   Basic::Pair* getIrSystemByType(const std::type_info& type); // Returns a IR sensor model by its type

   // ---
   // Set functions
   // ---

   virtual bool setType(Basic::String* const newTypeString);   // Sets the player's type string
   virtual void setName(const Basic::Identifier& newName);     // Sets the player's name (Basic::String version)
   virtual void setName(const char* const newName);            // Sets the player's name (char* version)
   virtual void setID(const unsigned short newId);             // Sets the player's ID
   virtual void setSide(const Side newSide);                   // Sets the player's side enum
   virtual void setMode(const Mode newMode);                   // Sets the player's current mode
   virtual void setInitMode(const Mode newMode);               // Sets the player's initial mode (after reset)
   virtual bool setUseCoordSys(const CoordSys);                // Sets the coord system to use for updating position

   virtual bool setFuelFreeze(const bool f);                   // Sets the player's fuel freeze flag
   virtual bool setCrashOverride(const bool f);                // Sets the player's crash override flag
   virtual bool setKillOverride(const bool f);                 // Sets the player's kill override flag
   virtual bool setKillRemoval(const bool f);                  // Sets the player's kill removal flag
   virtual void resetJustKilled();                             // Resets the just killed flag
   virtual bool setDamage(const LCreal v);                     // Sets the player's damage state
   virtual bool setSmoke(const LCreal v);                      // Sets the player's smoke state
   virtual bool setFlames(const LCreal v);                     // Sets the player's flames state
   virtual bool setCamouflageType(const unsigned int v);       // Sets the user defined camouflage type (or zero for none)
   virtual bool setPositionFreeze(const bool f);               // Sets the player's freeze flag
   virtual bool setAltitudeFreeze(const bool f);               // Sets the player's altitude freeze flag
   virtual bool setAttitudeFreeze(const bool f);               // Sets the player's attitude freeze flag

   virtual bool setHeadingHoldOn(const bool b);                // Turns heading-hold mode on/off
   virtual bool setCommandedHeading(const double h);           // Sets the commanded (true) heading default (radians)
   virtual bool setCommandedHeadingD(const double h);          // Sets the commanded (true) heading (degrees)
   virtual bool setCommandedHeadingR(const double h);          // Sets the commanded (true) heading (radians)

   virtual bool setVelocityHoldOn(const bool b);               // Turns velocity-hold mode on/off
   virtual bool setCommandedVelocityKts(const double a);       // Sets the commanded (true) velocity (knots)

   virtual bool setAltitudeHoldOn(const bool b);               // Turns altitude-hold mode on/off
   virtual bool setCommandedAltitude(const double a);          // Sets commanded (HAE) altitude, default (meters)
   virtual bool setCommandedAltitudeM(const double a);         // Sets commanded (HAE) altitude (meters)
   virtual bool setCommandedAltitudeFt(const double a);        // Sets commanded (HAE) altitude (feet)

   virtual bool setNib(Nib* const p);                          // Sets the networked player's Nib object

   virtual bool setEnableNetOutput(const bool f);              // Sets the network output enabled flag
   virtual bool setOutgoingNib(Nib* const p, const unsigned int id); // Sets the outgoing NIB for network 'id'

   virtual void setTerrainElevation(const LCreal v);           // Sets the elevation of the terrain at this player's location (meters)
   virtual bool setTerrainOffset(const LCreal v);              // Sets the ground clamping offset (meters)
   virtual bool setInterpolateTerrain(const bool f);           // sets the DTED terrain interpolation flag
   virtual bool setTerrainElevationRequired(const bool b);     // Sets OTW terrain elevation request flag

   // ---
   // Set the player's position
   //
   // 1) Setting the position in any one of the three coordinate systems,
   //    i.e., geocentric(ECEF), geodetic lat/lon or local gaming area NED, will set
   //    the player's position in the other two systems as well.
   //
   // 2) The world transformation matrix, getWorldMat(), is computed by
   //    these set position functions.
   //
   // 3) By default the optional 'slaved' parameter is FALSE, which enables this
   //    Player class to update the position.  If set to true then the position
   //    is 'slaved' to the callers parameters and not updated by this class.
   // ---

   // Sets the 'slaved' altitude (meters)
   virtual bool setAltitude(const double alt, const bool slaved = false);

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const bool slaved = false);

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const double down, const bool slaved = false);

   // Position vector; NED from simulation reference point (meters)
   virtual bool setPosition(const osg::Vec3d& newPos, const bool slaved = false);

   // Sets lat/long position; (degrees)
   virtual bool setPositionLL(const double lat, const double lon, const bool slaved = false);

   // Sets lat/long position; (degrees) and altitude (meters)
   virtual bool setPositionLLA(const double lat, const double lon, const double alt, const bool slaved = false);

   // Geocentric (ECEF) position vector (meters)
   virtual bool setGeocPosition(const osg::Vec3d& gcPos, const bool slaved = false);

   // ---
   // Set the player's orientation angles (roll, pitch and yaw)
   //
   // 1) Setting the orientation in any one of the formats (i.e., Euler angles,
   //    geocentric Euler angles, rotational matrix or quaternions) will set the
   //    orientation in all other formats.
   //
   // 2) Set the orientation after setting the player's position to ensure
   //    the world transformation matrix is correct.
   // ---

   // Sets Euler angles (body/NED): (radians) [ roll pitch yaw ]
   virtual bool setEulerAngles(const double r, const double p, const double y);
   virtual bool setEulerAngles(const osg::Vec3d& newAngles);

   // Sets geocentric (body/ECEF) Euler angles: (radians) [ roll pitch yaw ]
   virtual bool setGeocEulerAngles(const osg::Vec3d& newAngles);

   // Sets the rotational matrix
   virtual bool setRotMat(const osg::Matrixd&);

   // Sets the quaternions
   virtual bool setQuaternions(const osg::Quat&);

   // ---
   // Set the player's angular velocities:
   //    body and geocentric (ecef) coordinate systems
   //
   // 1) Setting the angular rate in eithercoordinate system will set the
   //    rates in both coordinate systems ( body and ecef ).
   //
   // 2) The player's position and rotation (see above) must be set prior to
   //    setting these angular rates to ensure that the transformational matrices
   //    have been computed.
   // ---

   // Sets the body angular velocities (radians/second)
   virtual bool setAngularVelocities(const double pa, const double qa, const double ra);
   virtual bool setAngularVelocities(const osg::Vec3d& newAngVel);

   // Sets the body angular velocities (radians/second)
   virtual bool setGeocAngularVelocities(const osg::Vec3d& newAngVel);

   // ---
   // Set the player's veloctity and acceleration vectors
   //
   // 1) Setting velocity or acceleration in any one of the coordinate system
   //    will set the vectors in all three coordinate systems ( body, ned and ecef ).
   //
   // 2) The player's position and rotation (see above) must be set prior to
   //    setting these vectors to ensure that the transformational matrices
   //    have been computed.
   // ---

   // Sets local tangent plane velocities (NED); (meters/second)
   virtual bool setVelocity(const LCreal ue, const LCreal ve, const LCreal we);
   virtual bool setVelocity(const osg::Vec3& newVel);

   // Sets local tangent plane accelerations (NED);
   virtual bool setAcceleration(const LCreal due, const LCreal dve, const LCreal dwe);
   virtual bool setAcceleration(const osg::Vec3& newAccel);

   // Sets body velocities: (meters/second) [ ua -> fwd(+), va -> right(+), wa -> down(+) ]
   virtual bool setVelocityBody(const LCreal ua, const LCreal va, const LCreal wa);
   virtual bool setVelocityBody(const osg::Vec3& newVelBody);

   // Sets body accelerations
   virtual bool setAccelerationBody(const LCreal dua, const LCreal dva, const LCreal dwa);
   virtual bool setAccelerationBody(const osg::Vec3& newAccelBody);

   // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual bool setGeocVelocity(const LCreal vx, const LCreal vy, const LCreal vz);
   virtual bool setGeocVelocity(const osg::Vec3& newVelEcef);

   // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   virtual bool setGeocAcceleration(const LCreal dvx, const LCreal dvy, const LCreal dvz);
   virtual bool setGeocAcceleration(const osg::Vec3& newAccelEcef);

   // ---
   // Set the initial conditions, which are used by reset()
   // ---

   // Sets the player's initial (reset) latitude (degrees)
   virtual bool setInitLat(const double x);

   // Sets the player's initial (reset) longitude (degrees)
   virtual bool setInitLon(const double x);

   // Initial altitude (HAE) (meters)
   virtual bool setInitAltitude(const double altitude);

   // Initial geocentric (ECEF) position vector
   virtual bool setInitGeocentricPosition(const osg::Vec3d& newPos);

   // Initial position (after reset) relative to the simulation reference point (meters)
   virtual bool setInitPosition(const double north, const double east);

   // Initial position vector (after reset); north and east of simulation reference point (meters)
   virtual bool setInitPosition(const osg::Vec2d& newPos);

   // ---
   // Roll Control Input
   //      Normalized inputs
   //      roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   // ---
   virtual void setControlStickRollInput(const LCreal roll);

   // ---
   // Pitch Control Input
   //      Normalized inputs
   //      pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
   // ---
   virtual void setControlStickPitchInput(const LCreal pitch);

   // ---
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
   // ---
   virtual int setThrottles(const LCreal* const positions, const int num);

   // ---
   // Process weapon detonation
   // ---
   virtual void processDetonation(const LCreal detRange, Weapon* const wpn = 0);

   // ---
   // Event handler(s)
   // ---
   virtual bool killedNotification(Player* const killedBy = 0);         // Handles the KILL_EVENT event
   virtual bool collisionNotification(Player* const msg);               // Handles the CRASH_EVENT event with another player
   virtual bool crashNotification();                                    // Handles the CRASH_EVENT event
   virtual bool onWpnRelEvent(const Basic::Boolean* const msg = 0);     // Handles the WPN_REL_EVENT event
   virtual bool onTriggerSwEvent(const Basic::Boolean* const msg = 0);  // Handles the TRIGGER_SW_EVENT event
   virtual bool onTgtStepEvent();                                       // Handles the TGT_STEP_EVENT event
   virtual bool onRfEmissionEventPlayer(Emission* const msg);           // Handles the RF_EMISSION event
   virtual bool onRfReflectedEmissionEventPlayer(Emission* const msg);  // Handles the RF_EMISSION event reflected to a 3rd party
   virtual bool onReflectionsRequest(Basic::Component* const msg);      // Handles the RF_REFLECTIONS_REQUEST event
   virtual bool onReflectionsCancel(const Basic::Component* const msg); // Handles the RF_REFLECTIONS_CANCEL event
   virtual bool onIrMsgEventPlayer(IrQueryMsg* const msg);              // Handle IR_QUERY_MSG event
   virtual bool onDatalinkMessageEventPlayer(Basic::Object* const msg); // Handles the DATALINK_MESSAGE event
   virtual bool onDeEmissionEvent(Basic::Object* const msg);            // Handles the DE_EMISSION event

   // ---
   // Slot functions
   // ---
   virtual bool setSlotInitXPos(const Basic::Distance* const msg);
   virtual bool setSlotInitXPos(const Basic::Number* const msg);
   virtual bool setSlotInitYPos(const Basic::Distance* const msg);
   virtual bool setSlotInitYPos(const Basic::Number* const msg);
   virtual bool setSlotInitAlt(const Basic::Distance* const msg);
   virtual bool setSlotInitAlt(const Basic::Number* const msg);
   virtual bool setSlotInitPosition(const Basic::List* const msg);

   virtual bool setSlotInitLat(const Basic::LatLon* const msg);
   virtual bool setSlotInitLat(const Basic::Angle* const msg);
   virtual bool setSlotInitLat(const Basic::Number* const msg);
   virtual bool setSlotInitLon(const Basic::LatLon* const msg);
   virtual bool setSlotInitLon(const Basic::Angle* const msg);
   virtual bool setSlotInitLon(const Basic::Number* const msg);

   virtual bool setSlotInitGeocentric(const Basic::List* const msg);

   virtual bool setSlotInitRoll(const Basic::Angle* const msg);
   virtual bool setSlotInitRoll(const Basic::Number* const msg);
   virtual bool setSlotInitPitch(const Basic::Angle* const msg);
   virtual bool setSlotInitPitch(const Basic::Number* const msg);
   virtual bool setSlotInitHeading(const Basic::Angle* const msg);
   virtual bool setSlotInitHeading(const Basic::Number* const msg);
   virtual bool setSlotInitEulerAngles(const Basic::List* const msg);

   virtual bool setSlotInitVelocity(const Basic::Number* const msg);
   virtual bool setSlotInitVelocityKts(const Basic::Number* const msg);

   virtual bool setSlotID(const Basic::Number* const msg);
   virtual bool setSlotSide(Basic::String* const msg);
   virtual bool setSlotInitMode(Basic::String* const msg);
   virtual bool setSlotUseCoordSys(Basic::String* const msg);

   virtual bool setSlotSignature(RfSignature* const msg);
   virtual bool setSlotIrSignature(IrSignature* const msg);
   virtual bool setSlotCamouflageType(const Basic::Number* const msg);

   virtual bool setSlotTerrainElevReq(const Basic::Number* const msg);
   virtual bool setSlotInterpolateTerrain(const Basic::Number* const msg);
   virtual bool setSlotTerrainOffset(const Basic::Distance* const msg);

   virtual bool setSlotPositionFreeze(const Basic::Number* const msg);
   virtual bool setSlotAltitudeFreeze(const Basic::Number* const msg);
   virtual bool setSlotAttitudeFreeze(const Basic::Number* const msg);
   virtual bool setSlotFuelFreeze(const Basic::Number* const msg);
   virtual bool setSlotCrashOverride(const Basic::Number* const msg);
   virtual bool setSlotKillOverride(const Basic::Number* const msg);
   virtual bool setSlotKillRemoval(const Basic::Number* const msg);
   virtual bool setSlotEnableNetOutput(const Basic::Number* const msg);
   virtual bool setSlotDataLogTime(const Basic::Time* const msg);

   virtual bool setSlotTestRollRate(const Basic::Angle* const msg);
   virtual bool setSlotTestPitchRate(const Basic::Angle* const msg);
   virtual bool setSlotTestYawRate(const Basic::Angle* const msg);
   virtual bool setSlotTestBodyAxis(const Basic::Number* const msg);

   // ---
   // Basic::Component interface
   // ---
   virtual bool isFrozen() const;
   virtual void reset();
   virtual void updateTC(const LCreal dt = 0.0);
   virtual void updateData(const LCreal dt = 0.0);
   virtual bool event(const int event, Basic::Object* const obj = 0);

protected:
   // Vehicle Dynamics -- called by updateTC() during phase zero
   virtual void dynamics(const LCreal  dt = 0.0);

   // Position update (local players only)
   void positionUpdate(const LCreal dt);

   // Dead-reckoning (networked I-players only)
   void deadReckonPosition(const LCreal dt);

   // Update the player's internal systems pointers (e.g., after new system components have been added)
   virtual void updateSystemPointers();

   // Update terrain elevation at our location
   virtual void updateElevation();

   // Basic::Component Interface
   virtual void printTimingStats();

   // These systems, from our subcomponent list, can only be set by reset()
   virtual bool setDynamicsModel(Basic::Pair* const sys); // Sets our dynamics model
   virtual bool setDatalink(Basic::Pair* const n);        // Sets our Datalink models
   virtual bool setGimbal(Basic::Pair* const n);          // Sets our gimbal/antenna/optic models
   virtual bool setNavigation(Basic::Pair* const n);      // Sets our navigation model
   virtual bool setOnboardComputer(Basic::Pair* const s); // Sets our onboard computer model
   virtual bool setPilot(Basic::Pair* const p);           // Sets our pilot model
   virtual bool setRadio(Basic::Pair* const n);           // Sets our radio models
   virtual bool setSensor(Basic::Pair* const n);          // Sets our sensor models
   virtual bool setIrSystem(Basic::Pair* const n);        // Sets our IR sensors models
   virtual bool setStoresMgr(Basic::Pair* const s);       // Sets our stores management model

   // Basic::Component interface
   virtual void processComponents(   // Process our subcomponent list (which should be other steerpoints)
      Basic::PairStream* const list,        // Source list of components
      const std::type_info& filter,         // Type filter
      Basic::Pair* const add = 0,           // Optional pair to add
      Basic::Component* const remove = 0    // Optional subcomponent to remove
      );

private:
   void initData();

   // ---
   // Player identity
   // ---
   SPtr<Basic::String>  type;    // Type of vehicle
   unsigned short id;            // Player ID
   Basic::Identifier  pname;     // Player's Name
   Side           side;          // Which side (see above)

   // ---
   // Player State
   // ---
   Mode        mode;             // Player mode (see above)
   CoordSys    useCoordSys;      // Coordinate system being used to update position
   CoordSys    useCoordSysN1;    // Previous 'useCoordSys'

   double      latitude;         // Latitude                         (degrees)
   double      longitude;        // Longitude                        (degrees)
   double      altitude;         // Altitude                         (meters) (HAE)
   osg::Vec3d  posVecNED;        // Local gaming area position vector (meters) [ x, y, z ] NED
   osg::Vec3d  posVecECEF;       // Geocentric position vector       (meters)  (ECEF)

   osg::Vec3d  velVecNED;        // Inertial axes velocity vector    (meters/second)  [ ue, ve, we ] NED
   osg::Vec3d  velVecECEF;       // Geocentric velocity vector       (meters/second)  (ECEF)
   osg::Vec3d  velVecBody;       // Body axes velocity vector        (meters/second)  [ ua  va  wa ]
   osg::Vec3d  velVecN1;         // Previous velocity vector         (meters/second)  (based on 'useCoordSys')

   osg::Vec3d  accelVecNED;      // Inertial axes acceleration vector ((meters/second)/second) [ due, dve, dwe ] NED
   osg::Vec3d  accelVecECEF;     // Geocentric acceleration vector   ((meters/second)/second) (ECEF)
   osg::Vec3d  accelVecBody;     // Body axes acceleration vector    ((meters/second)/second) [ dua, dva, dwa ]

   LCreal      vp;               // Total Velocity                   (meters/second)
   LCreal      gndSpd;           // Ground Speed                     (meters/second)
   LCreal      gndTrk;           // Ground Track                     (radians)

   osg::Vec3d  angles;           // Geodetic (body/NED) Euler angles (radians) [ roll pitch yaw ] AKA [ phi theta psi ]
   osg::Vec2d  scPhi;            // Sin/Cos of roll (phi)
   osg::Vec2d  scTheta;          // Sin/Cos of pitch (theta)
   osg::Vec2d  scPsi;            // Sin/Cos of yaw (psi)

   osg::Vec3d  anglesW;          // World (body/ECEF) Euler angles (radians)
   osg::Vec2d  scPhiW;           // Sin/Cos of world phi
   osg::Vec2d  scThetaW;         // Sin/Cos of world theta
   osg::Vec2d  scPsiW;           // Sin/Cos of world psi

   osg::Vec3d  angularVel;       // Body angular velocities (radians/seconds)
   osg::Vec3d  gcAngVel;         // Geocentric (ECEF) angular velocities (radians/seconds)

   osg::Quat   q;                // Quaternions for the rotational matrix

   osg::Matrixd rm;              // Rotational Matrix: inertial to body directional cosines
                                 //    RM = Rx[roll] * Ry[pitch] * Rz[yaw]

   osg::Matrixd wm;              // World transformation matrix:
                                 //    Local inertial tangent plane (NED) <==> World (ECEF)
                                 //    WM = Ry[-(90+lat)] * Rz[lon]

   osg::Matrixd rmW2B;           // Rotational Matrix: world to body directional cosines
                                 //    RM = Rx[gcRoll] * Ry[gcPitch] * Rz[gcYaw]

   LCreal      tElev;            // Terrain Elevation  (meters -- up+)
   bool        tElevValid;       // Terrain elevation is valid
   bool        tElevReq;         // Height-Of-Terrain is required from the OTW system
   bool        interpTrrn;       // interpolate between terrain elevation posts (local terrain database only)
   LCreal      tOffset;          // Offset from the terrain to the player's CG for ground clamping
   bool        posVecValid;      // Local position vector valid
   bool        altSlaved;        // Player's altitude is slaved to the dynamics software (default: false)
   bool        posSlaved;        // Player's position is slaved to the dynamics software (default: false)
   bool        posFrz;           // Player's position is frozen
   bool        altFrz;           // Player's altitude is frozen
   bool        attFrz;           // Player's attitude is frozen
   bool        fuelFrz;          // Player's fuel quanity is frozen
   bool        crashOverride;    // If true, player can NOT crash
   bool        killOverride;     // If true, player can NOT be killed
   bool        killRemoval;      // If true then on kill notification the player will be set to KILLED and eventually removed

   // ---
   // Appearance
   // ---
   SPtr<RfSignature> signature;  // Player's RCS signature
   SPtr<IrSignature> irSignature; // Player's IR signature
   unsigned int camouflage;      // Camouflage type (0 is none)
   LCreal      damage;           // Damage state from no damage(0.0) to destroyed (1.0)
   LCreal      smoking;          // Smoke state from no smoke (0.0) to maximum (1.0)
   LCreal      flames;           // Flames state from no flames (0.0) to maximum (1.0)
   bool        justKilled;       // Just killed flag
   unsigned short killedBy;      // Killed by player ID

   // ---
   // Initialization, Controls, Freeze and Reset
   // ---
   osg::Vec2d  initPosVec;       // Initial Position vector [x y ]  (meters)
   bool        initPosFlg;       // Initial position vector is valid

   osg::Vec3d  initGeoPosVec;    // Initial geocentric position vector [ x y z ] (ECEF) (meters)
   bool        initGeoPosFlg;    // Initial position vector is valid

   double      initLat;          // Initial latitude (degrees)
   double      initLon;          // Initial longitude (degrees)
   bool        initLatLonFlg;    // Initial lat/lon valid

   double      initAlt;          // Initial altitude (HAE)
   LCreal      initVp;           // Initial Total Velocity
   Mode        initMode;         // Initial mode
   osg::Vec3d  initAngles;       // Initial (Euler) angles (radians)
   osg::Vec3d  testAngRates;     // Test angular rates (radians/sec)
   bool        testBodyAxis;     // Test angular rates are in the body axis else they're Euler rates

   // ---
   // Data collection timer
   // ---
   LCreal      dataLogTimer;     // Data log timer (seconds)
   LCreal      dataLogTime;      // Data log time (seconds)

   // ---
   // Incoming IPlayer network support
   // ---
   Nib*       nib;               // Network Interface Block (ref()'d)
   int        netID;             // Network id

   // ---
   // Outgoing network support data
   // ---
   bool       enableNetOutput;   // Allow output to the network
   Nib**      nibList;           // Pointer to a list of outgoing NIBs

   // ---
   // System pointers
   // ---
   Simulation* sim;              // The simulation system that owns us (not ref()'d)

   Basic::Pair* dyn;             // Dynamics Model (ref()'d)
   Basic::Pair* datalink;        // Datalink model (ref()'d)
   Basic::Pair* gimbal;          // Gimbal/antenna/optic models (ref()'d)
   Basic::Pair* nav;             // Navigation (ref()'d)
   Basic::Pair* obc;             // Onboard Computer (ref()'d)
   Basic::Pair* pilot;           // Pilot Model (e.g., autopilot, PDL) (ref()'d)
   Basic::Pair* radio;           // Radio models (ref()'d)
   Basic::Pair* sensor;          // R/F Sensor models (ref()'d)
   Basic::Pair* irSystem;        // IR Sensor models (ref()'d)
   Basic::Pair* sms;             // Stores Management System (ref()'d)
   bool loadSysPtrs;             // Load system pointers flag

   // ---
   // Reflected emissions
   // ---
   static const unsigned int MAX_RF_REFLECTIONS = 4;   // Max number of reflected emissions we'll send (let's keep it small)
   Basic::Component* rfReflect[MAX_RF_REFLECTIONS];    // Objects that are interested in the emissions hitting us
   LCreal  rfReflectTimer[MAX_RF_REFLECTIONS];         // Request for reflected emissions will timeout

   // ---
   // sync state changes
   // ---
   bool                 syncState1Ready;
   bool                 syncState2Ready;
   SynchronizedState    syncState1;
   SynchronizedState    syncState2;
};

// -----------------------------------------------------------------------------
// Inline functions
// -----------------------------------------------------------------------------
#include "openeaagles/simulation/Player.inl"

} // End Simulation namespace
} // End Eaagles namespace

#endif
