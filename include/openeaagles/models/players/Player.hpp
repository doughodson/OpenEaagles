
#ifndef __oe_models_Player_H__
#define __oe_models_Player_H__

#include "openeaagles/simulation/AbstractPlayer.hpp"

#include "openeaagles/models/SynchronizedState.hpp"

#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/osg/Quat"

#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/util/unit_utils.hpp"
#include <array>

namespace oe {
namespace base { class Vec2d;    class Vec3d;  class Angle; class Boolean;
                 class Distance; class LatLon; class List;  class Time; }
namespace simulation { class AbstractNib; }
namespace models {
class WorldModel;

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
class AbstractWeapon;

// Other item types
class Emission;
class IrQueryMsg;
class IrSignature;
class RfSignature;
class Track;

//------------------------------------------------------------------------------
// Class: Player
//
// Description: Abstract interface for all players (e.g., aircraft, ground vehicles, etc.)
//              Provides interfaces for player IDs, types, status, state vectors,
//              control flags, subcomponent lists, event handling, etc.
//
//
//------------------------------------------------------------------------------
// Factory name: AbstractPlayer
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
//    initXPos       <base::Distance>    ! X position (+north) (default: 0)
//    initXPos       <base::Number>      ! X position (+north) (meters)
//
//    initYPos       <base::Distance>    ! Y position (+east) (default: 0)
//    initYPos       <base::Number>      ! Y position (+east)(meters)
//
//    initAlt        <base::Distance>    ! Altitude (+up) (default: 0)
//    initAlt        <base::Number>      ! Altitude (+up) (meters)
//
//    initPosition   <base::List>        ! Position Vector: meters [ north east down ]
//                                        !   *** initPosition will be removed in later release (use below slots instead) ***
//
//    ! 2: Player's initial geodetic position
//
//    initLatitude   <base::LatLon>      ! Latitude (default: 0)
//    initLatitude   <base::Angle>       ! Latitude
//    initLatitude   <base::Number>      ! Latitude (degs)
//
//    initLongitude  <base::LatLon>      ! Longitude (default: 0)
//    initLongitude  <base::Angle>       ! Longitude
//    initLongitude  <base::Number>      ! Longitude (degs)
//
//    initAlt        <base::Distance>    ! Altitude (HAE) (+up)  ### NES Is this being used?
//    initAlt        <base::Number>      ! Altitude (HAE) (+up) (meters)
//
//
//    ! 3: Player's initial world position
//
//    initGeocentric <base::List>        ! Position vector [ x y z ] (ECEF) (meters) (default: 0,0,0)
//
//
//    ! ---
//    ! Player's initial Euler angles
//    ! ---
//    initRoll       <base::Angle>       ! Initial roll angle (default: 0)
//    initRoll       <base::Number>      ! Initial roll angle (radians)
//
//    initPitch      <base::Angle>       ! Initial pitch angle (default: 0)
//    initPitch      <base::Number>      ! Initial pitch angle (radians)
//
//    initHeading    <base::Angle>       ! Initial heading angle (default: 0)
//    initHeading    <base::Number>      ! Initial heading angle (radians)
//
//    initEuler      <base::List>        ! Initial Euler angles: radians [ roll pitch yaw ]
//
//    ! ---
//    ! Player's initial velocity
//    ! ---
//    initVelocity   <base::Number>      ! Initial Velocity: meters/sec (default: 0)
//
//    ! ---
//    ! Player's type, id and and other parameters
//    ! ---
//    type           <base::String>      ! Type of player vehicle ("F-16A", "Tank", "SA-6", etc.) (default: 0)
//    id             <base::Integer>     ! Player id  [ 1 .. 65535 ] (default: 0)
//    side           <base::String>      ! Which side? { BLUE, RED, YELLOW, CYAN, GRAY, WHITE } (default: GRAY)
//    mode           <base::String>      ! Initial player mode ( INACTIVE, ACTIVE, DEAD ) (default: ACTIVE)
//    useCoordSys    <base::String>      ! Coord system to use for updating player position { WORLD, GEOD, LOCAL }
//                                       !   (default: based on the coord system used to set the initial position)
//
//    signature      <RfSignature>       ! Player's RCS signature (default: 0)
//    irSignature    <IrSignature>       ! Player's IR signature (default: 0)
//    camouflageType <base::Integer>     ! User defined camouflage type (positive integer or zero for none) (default: 0)
//
//    terrainElevReq <base::Boolean>     ! Terrain elevation from the OTW system is requested; otherwise use DTED (default: false)
//    interpolateTerrain <base::Boolean> ! Interpolate our DTED terrain elevation data (default: false)
//    terrainOffset  <base::Distance>    ! Ground clamp offset from terrain to player's CG (base::Distance)
//
//    positionFreeze <base::Boolean>     ! Position freeze (default: false)
//    altitudeFreeze <base::Boolean>     ! Altitude freeze (default: false)
//    attitudeFreeze <base::Boolean>     ! Attitude freeze (default: false)
//    fuelFreeze     <base::Boolean>     ! Fuel freeze (default: false)
//    crashOverride  <base::Boolean>     ! Crash Override (i.e., ignore collision and crash events)(default: false)
//    killOverride   <base::Boolean>     ! Kill/Damage Override -- player can not be killed/damaged (default: false)
//    killRemoval    <base::Boolean>     ! If true destroyed players are set to KILLED and are eventually removed (default: false)
//    enableNetOutput <base::Boolean>    ! Enable network output of this player (default: true)
//
//    dataLogTime    <base::Time>        ! Time between player data samples to an optional data
//                                       ! logger, or zero if none (default: 0)
//
//    ! ---
//    ! Angular test rates:
//    !     If non-zero the Euler angles are updated using the body angular rates.
//    !     For use only without a dynamics model.
//    ! ---
//    testRollRate   <base::Angle>       ! Test roll rate (body) (units per second) (default: 0)
//    testPitchRate  <base::Angle>       ! Test pitch rate (body) (units per second) (default: 0)
//    testYawRate    <base::Angle>       ! Test heading rate (body) (units per second) (default: 0)
//    testBodyAxis   <base::Boolean>     ! Test rates are in the body axis else they're Euler rates (default: false)
//
//
// --------------------
// Events:
//    KILL_EVENT              <Player>             ! Killed by
//    CRASH_EVENT             <Player>             ! Collision with
//    CRASH_EVENT             (none)               ! Crashed
//    RF_EMISSION             <Emission>           ! Hit by RF emission
//    RF_REFLECTIONS_REQUEST  <base::Component>    ! Requests for reflected R/F emissions (must re-request once a second)
//    RF_REFLECTIONS_CANCEL   <base::Component>    ! Cancel requests for reflected R/F emissions
//    WPN_REL_EVENT           <base::Boolean>      ! Weapon release switch with position
//    WPN_REL_EVENT           (none)               ! Weapon release switch (single shot)
//    TRIGGER_SW_EVENT        <base::Boolean>      ! Trigger switch with position
//    TRIGGER_SW_EVENT        (none)               ! Trigger switch (single shot)
//    TGT_STEP_EVENT          (none)               ! Target step switch
//    DATALINK_MESSAGE        <base::Object>       ! Hit with a datalink message
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
//    c) World model's gaming area coordinates (NED); the XY plane is tangent to
//       and centered at the simulation's geodetic gaming area reference point;
//       the Z or down axes is perpendicular to the tangent plane (see WorldModel.hpp).
//       Use models::getWorldMat() to transform vectors between ECEF and
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
//             -- Use simulation::getWorldMat() to get the ECEF to NED
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
//       test rates should be used only on players without a dynamics model
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
//          datalink:      ( Datalink ...        )  // Datalink model
//          dynamicsModel: ( DynamicsModel ...   )  // Dynamics model
//          gimbals:       ( Gimbal ...          )  // Gimbal, antenna and/or seeker model
//          nav:           ( Navigation ...      )  // Navigation model
//          obc:           ( OnboardComputer ... )  // Onboard computer model
//          pilot:         ( Pilot ...           )  // Pilot model
//          radios:        ( Radio ...           )  // Radio model
//          sensors:       ( RfSensor ...        )  // R/F Sensor model
//          irSystems:     ( IrSystem ...        )  // IR system model
//          sms:           ( StoresMgr ...       )  // Stores management model
//
//          Where 'datalink' is the name given to the top level datalink object,
//          'dynamicsModel' is the name given to the dynamics model, etc.
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
class Player : public simulation::AbstractPlayer
{
   DECLARE_SUBCLASS(Player, simulation::AbstractPlayer)

public:
   Player();

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

   // ---
   // Type and ID
   // ---

   virtual unsigned int getMajorType() const;                             // The player's 'major type' enum
   virtual bool isMajorType(const unsigned int tst) const;                // True if player is of these (bit-wise or'd) major types

   virtual const base::String* getType() const;                           // The player's type string (.e.g, "F-16C")

   virtual Side getSide() const;                                          // The 'side' that the player is on.
   virtual bool isSide(const unsigned int tst) const;                     // True if player is with one of these (bit-wise or'd) sides
   virtual bool isNotSide(const unsigned int tst) const;                  // True if player is not with one one of these (bit-wise or'd) sides

   // ---
   // State data
   // ---

   virtual double getRoll() const;                                  // Roll Euler angle (radians) by default
   virtual double getRollR() const;                                 // Roll Euler angle (radians)
   virtual double getRollD() const;                                 // Roll Euler angle (degrees)
   virtual double getSinRoll() const;                               // Sin of the Euler roll angle
   virtual double getCosRoll() const;                               // Cos of the  Euler roll angle

   virtual double getPitch() const;                                 // Pitch Euler angle (radians) by default
   virtual double getPitchR() const;                                // Pitch Euler angle (radians)
   virtual double getPitchD() const;                                // Pitch Euler angle (degrees)
   virtual double getSinPitch() const;                              // Sin of the pitch Euler angle
   virtual double getCosPitch() const;                              // Cos of the  pitch Euler angle

   virtual double getHeading() const;                               // Yaw Euler angle (radians) by default
   virtual double getHeadingR() const;                              // Yaw Euler angle (radians)
   virtual double getHeadingD() const;                              // Yaw Euler angle (degrees)
   virtual double getSinHeading() const;                            // Sin of the yaw Euler angle
   virtual double getCosHeading() const;                            // Cos of the  yaw Euler angle

   virtual const base::Vec3d& getEulerAngles() const;               // Euler angles (radians); geodetic (body/NED)
   virtual const base::Quat& getQuaternions() const;                // Rotational Quaternions

   virtual const base::Matrixd& getRotMat() const;                  // Rotational Matrix: (directional cosines)
                                                                    //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw]
                                                                    //    Usage:
                                                                    //       Vb = M * Vi
                                                                    //       Vi  = Vb * M
                                                                    //    Where: 'Vb' is a body vector; 'Vi' is an inertial vector

   virtual const base::Matrixd& getRotMatW2B() const;               // Rotational Matrix: world to body
                                                                    //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw] * Ry[-(90+lat)] * Rz[lon]
                                                                    //    Usage:
                                                                    //       Vb = M * Vw
                                                                    //       Vw  = Vb * M
                                                                    //    Where: 'Vb' is a body vector; 'Vw' is a world (ECEF) vector

   virtual const base::Vec3d& getGeocEulerAngles() const;           // Geocentric (body/ECEF) Euler angles

   virtual const base::Vec3d& getAngularVelocities() const;         // Body angular rates (radians/second)
   virtual const base::Vec3d& getGeocAngularVelocities() const;     // Geocentric angular rates (radians/second)

   virtual const base::Vec3d& getGeocPosition() const;              // Geocentric (ECEF) position vector [ x y z ] (meters)

   virtual double getLatitude() const;                              // Player's latitude (degrees)
   virtual double getLongitude() const;                             // Player's longitude (degrees)

   virtual const base::Matrixd& getWorldMat() const;                // World transformation matrix:
                                                                    //    Local inertial tangent plane (NED) <==> World (ECEF)
                                                                    //    Matrix: M = Ry[-(90+lat)] * Rz[lon]
                                                                    //    Usage:
                                                                    //       Vi = M * Vw;
                                                                    //       Vw  = Vi * M
                                                                    //    Where: 'Vw' is a world(ECEF) vector; 'Vi' is an inertial (NED) vector

   virtual double getEarthRadius() const;                           // Earth radius at player's location (meters)

   virtual bool getPositionLL(double* const lat, double* const lon) const;                     // Player's Lat/lon (degrees)
   virtual bool getPositionLLA(double* const lat, double* const lon, double* const alt) const; // Player's Lat/Lon (degrees) and altitude (meters)

   virtual double getXPosition() const;                             // North(+) or south(-) of the sim reference point (meters)
   virtual double getYPosition() const;                             // East(+) or west(-) of the sim reference point (meters)
   virtual const base::Vec3d& getPosition() const;                  // Position vector; NED from sim reference point (meters)
   virtual bool isPositionVectorValid() const;                      // Is the position vector valid

   virtual double getAltitude() const;                              // Altitude HAE (meters) default
   virtual double getAltitudeM() const;                             // Altitude HAE (meters)
   virtual double getAltitudeFt() const;                            // Altitude HAE (feet)

   virtual double getAltitudeAgl() const;                           // Altitude AGL (meters) default
   virtual double getAltitudeAglM() const;                          // Altitude AGL (meters)
   virtual double getAltitudeAglFt() const;                         // Altitude AGL (feet)

   virtual bool isTerrainElevationValid() const;                    // Terrain elevation is valid (or at least was where it was set)
   virtual double getTerrainElevation() const;                      // Terrain elevation at player (meters) default
   virtual double getTerrainElevationM() const;                     // Terrain elevation at player (meters)
   virtual double getTerrainElevationFt() const;                    // Terrain elevation at player (feet)

   virtual double getTotalVelocity() const;                         // Total velocity (meters/second)
   virtual double getTotalVelocityFPS() const;                      // Total velocity (feet/second)
   virtual double getTotalVelocityKts() const;                      // Total velocity (knots)

   virtual double getGroundSpeed() const;                           // Ground speed (meters/second)
   virtual double getGroundSpeedFPS() const;                        // Ground speed (feet/second)
   virtual double getGroundSpeedKts() const;                        // Ground speed (knots)

   virtual double getGroundTrack() const;                           // Ground track (radians) default
   virtual double getGroundTrackR() const;                          // Ground track (radians)
   virtual double getGroundTrackD() const;                          // Ground track (degrees)

   virtual const base::Vec3d& getVelocity() const;                  // Velocity vector; NED (meters/second)
   virtual const base::Vec3d& getAcceleration() const;              // Acceleration vector; NED ((meters/second)/second)

   virtual const base::Vec3d& getGeocVelocity() const;              // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual const base::Vec3d& getGeocAcceleration() const;          // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)

   virtual const base::Vec3d& getVelocityBody() const;              // Body velocity vector: Nose/right/down (meters/second)
   virtual const base::Vec3d& getAccelerationBody() const;          // Body Acceleration vector: Nose/right/down ((meters/second)/second)

   virtual double getGrossWeight() const;                           // Player's gross weight (lbs)
   virtual double getMach() const;                                  // Player's mach number
   virtual double getCG() const;                                    // Player's Center-of-Gravity (%)


   // ---
   // Appearance
   // ---

   virtual unsigned int getCamouflageType() const;                 // Returns the user defined camouflage type (or zero for none)

   virtual bool isDestroyed() const;                               // True if player is destroyed
   virtual double getDamage() const;                               // Returns the player's damage state - no damage(0.0) to destroyed (1.0)
   virtual double getSmoke() const;                                // Returns the player's smoke state - (0.0) to maximum (1.0)
   virtual double getFlames() const;                               // Returns the player's flames state - (0.0) to maximum (1.0)

   virtual bool isJustKilled() const;                              // True if the player was just killed
   virtual int killedByPlayerID() const;                           // ID of the attacking player that killed us

   // ---
   // Initialization data
   // ---
   virtual const base::Vec3d& getInitGeocentricPosition() const;   // Geocentric (ECEF) initial position vector [ x y z ] (meters)
   virtual bool isInitGeocentricPositionValid() const;

   virtual double getInitLatitude() const;                         // Player's initial latitude (degrees)
   virtual double getInitLongitude() const;                        // Player's initial longitude (degrees)
   virtual bool isInitLatLonValid() const;
   virtual double getInitVelocity() const;                         // Player's initial total velocity

   virtual const base::Vec2d& getInitPosition() const;             // Initial position vector; north and east of sim reference point (meters)
   virtual bool isInitPositionValid() const;

   virtual double getInitAltitude() const;                         // Initial altitude HAE (meters)

   virtual const base::Vec3d& getInitAngles() const;               // Initial Euler angles (radians)

   // ---
   // Control parameters
   // ---
   virtual bool isPositionFrozen() const;                          // True if the player is frozen (paused)
   virtual bool isAltitudeFrozen() const;                          // True if the player's altitude is frozen
   virtual bool isAttitudeFrozen() const;                          // True if the player's attitude is frozen
   virtual bool isFuelFrozen() const;                              // True if the player's fuel is frozen
   virtual bool isCrashOverride() const;                           // True if the player is in crash override mode
   virtual bool isKillOverride() const;                            // True if the player is in kill override mode
   virtual bool isKillRemovalEnabled() const;                      // True if the player's kill removal flag is set
   virtual bool isAltitudeSlaved() const;                          // True if altitude is slaved to an external model
   virtual bool isPositionSlaved() const;                          // True if position is slaved to an external model

   virtual double getTerrainOffset() const;                        // Ground clamping offset from the terrain to our CG (meters)
   virtual bool isDtedTerrainInterpolationEnabled() const;         // Interpolate our terrain from DTED if true

   virtual bool isTerrainElevationRequired() const;                // True if terrain elevation is required from the OTW system;
                                                                   // otherwise we use DTED, if available

   virtual CoordSys getCoordSystemInUse() const;                   // Returns the coordinate system currently being used to
                                                                   // update the player's position
   // ---
   // Internal autopilot controls
   // ---
   virtual bool isHeadingHoldOn() const;                           // True if heading-hold mode is on?
   virtual double getCommandedHeading() const;                     // Commanded (true) heading, default (radians)
   virtual double getCommandedHeadingD() const;                    // Commanded (true) heading (degrees)
   virtual double getCommandedHeadingR() const;                    // Commanded (true) heading (radians)

   virtual bool isVelocityHoldOn() const;                          // True if velocity-hold mode is on?
   virtual double getCommandedVelocityKts() const;                 // Commanded (true) velocity (knots)
   virtual double getCommandedVelocityFps() const;                 // Commanded (true) velocity (feet/second)
   virtual double getCommandedVelocityMps() const;                 // Commanded (true) velocity (meters/second)

   virtual bool isAltitudeHoldOn() const;                          // True if altitude-hold mode is on?
   virtual double getCommandedAltitude() const;                    // Get commanded (HAE) altitude, default (meters)
   virtual double getCommandedAltitudeM() const;                   // Get commanded (HAE) altitude (meters)
   virtual double getCommandedAltitudeFt() const;                  // Get commanded (HAE) altitude (feet)

   // ---
   // Sync State Changes
   // ---
   virtual const SynchronizedState& getSynchronizedState() const;

   // ---
   // Systems
   // ---

   WorldModel* getWorldModel();
   const WorldModel* getWorldModel() const;

   DynamicsModel* getDynamicsModel();                                 // Player's dynamics model
   const DynamicsModel* getDynamicsModel() const;                     // Player's dynamics model (const version)
   const base::Identifier* getDynamicsModelName() const;              // Name of the player's dynamics model

   Pilot* getPilot();                                                 // Player's top level pilot model
   const Pilot* getPilot() const;                                     // Player's top level pilot model (const version)
   const base::Identifier* getPilotName() const;                      // Name of the player's top level pilot model
   Pilot* getPilotByName(const char* const name);                     // Returns a pilot model by its name
   base::Pair* getPilotByType(const std::type_info& type);            // Returns a pilot model by its type

   StoresMgr* getStoresManagement();                                  // Player's stores (weapons, fuel, etc...) manager model
   const StoresMgr* getStoresManagement() const;                      // Player's stores (weapons, fuel, etc...) manager model (const version)
   const base::Identifier* getStoresManagementName() const;           // Name of the player's stores (weapons, fuel, etc...) manager model

   Datalink* getDatalink();                                           // Player's top level Datalink model
   const Datalink* getDatalink() const;                               // Player's top level Datalink (const version)
   const base::Identifier* getDatalinkName() const;                   // Name of the player's top level Datalink model
   Datalink* getDatalinkByName(const char* const name);               // Returns a Datalink model by its name
   base::Pair* getDatalinkByType(const std::type_info& type);         // Returns a Datalink model by its type

   Gimbal* getGimbal();                                               // Player's top level Gimbal model
   const Gimbal* getGimbal() const;                                   // Player's top level Gimbal (const version)
   const base::Identifier* getGimbalName() const;                     // Name of the player's top level Gimbal model
   Gimbal* getGimbalByName(const char* const name);                   // Returns a Gimbal model by its name
   base::Pair* getGimbalByType(const std::type_info& type);           // Returns a Gimbal model by its type

   Navigation* getNavigation();                                       // Player's top level Navigation model
   const Navigation* getNavigation() const;                           // Player's top level Navigation (const version)
   const base::Identifier* getNavigationName() const;                 // Name of the player's top level Navigation model
   Navigation* getNavigationByName(const char* const name);           // Returns a Navigation model by its name
   base::Pair* getNavigationByType(const std::type_info& type);       // Returns a Navigation model by its type

   OnboardComputer* getOnboardComputer();                             // Player's top level OnboardComputer model
   const OnboardComputer* getOnboardComputer() const;                 // Player's top level OnboardComputer (const version)
   const base::Identifier* getOnboardComputerName() const;            // Name of the player's top level OnboardComputer model
   OnboardComputer* getOnboardComputerByName(const char* const name); // Returns an OnboardComputer model by its name
   base::Pair* getOnboardComputerByType(const std::type_info& type);  // Returns an OnboardComputer model by its type

   Radio* getRadio();                                          // Player's top level Radio model
   const Radio* getRadio() const;                              // Player's top level Radio (const version)
   const base::Identifier* getRadioName() const;               // Name of the player's top level Radio model
   Radio* getRadioByName(const char* const name);              // Returns a Radio model by its name
   base::Pair* getRadioByType(const std::type_info& type);     // Returns a Radio model by its type

   RfSensor* getSensor();                                      // Player's top level R/F sensor model
   const RfSensor* getSensor() const;                          // Player's top level R/F sensor model (const version)
   const base::Identifier* getSensorName() const;              // Name of the player's top level R/F sensor model
   RfSensor* getSensorByName(const char* const name);          // Returns a R/F sensor model by its name
   base::Pair* getSensorByType(const std::type_info& type);    // Returns a R/F sensor model by its type

   IrSystem* getIrSystem();                                    // Player's top level IR sensor model
   const IrSystem* getIrSystem() const;                        // Player's top level IR sensor model (const version)
   const base::Identifier* getIrSystemName() const;            // Name of the player's top level IR sensor model
   IrSystem* getIrSystemByName(const char* const name);        // Returns a IR sensor model by its name
   base::Pair* getIrSystemByType(const std::type_info& type);  // Returns a IR sensor model by its type

   // ---
   // Set functions
   // ---

   virtual bool setType(const base::String* const newTypeString);          // Sets the player's type string
   virtual void setSide(const Side newSide);                               // Sets the player's side enum
   virtual bool setUseCoordSys(const CoordSys);                            // Sets the coord system to use for updating position

   virtual bool setFuelFreeze(const bool f);                               // Sets the player's fuel freeze flag
   virtual bool setCrashOverride(const bool f);                            // Sets the player's crash override flag
   virtual bool setKillOverride(const bool f);                             // Sets the player's kill override flag
   virtual bool setKillRemoval(const bool f);                              // Sets the player's kill removal flag
   virtual void resetJustKilled();                                         // Resets the just killed flag
   virtual bool setDamage(const double v);                                 // Sets the player's damage state
   virtual bool setSmoke(const double v);                                  // Sets the player's smoke state
   virtual bool setFlames(const double v);                                 // Sets the player's flames state
   virtual bool setCamouflageType(const unsigned int v);                   // Sets the user defined camouflage type (or zero for none)
   virtual bool setPositionFreeze(const bool f);                           // Sets the player's freeze flag
   virtual bool setAltitudeFreeze(const bool f);                           // Sets the player's altitude freeze flag
   virtual bool setAttitudeFreeze(const bool f);                           // Sets the player's attitude freeze flag

   virtual bool setHeadingHoldOn(const bool b);                            // Turns heading-hold mode on/off
   virtual bool setCommandedHeading(const double h);                       // Sets the commanded (true) heading default (radians)
   virtual bool setCommandedHeadingD(const double h);                      // Sets the commanded (true) heading (degrees)
   virtual bool setCommandedHeadingR(const double h);                      // Sets the commanded (true) heading (radians)

   virtual bool setVelocityHoldOn(const bool b);                           // Turns velocity-hold mode on/off
   virtual bool setCommandedVelocityKts(const double a);                   // Sets the commanded (true) velocity (knots)

   virtual bool setAltitudeHoldOn(const bool b);                           // Turns altitude-hold mode on/off
   virtual bool setCommandedAltitude(const double a);                      // Sets commanded (HAE) altitude, default (meters)
   virtual bool setCommandedAltitudeM(const double a);                     // Sets commanded (HAE) altitude (meters)
   virtual bool setCommandedAltitudeFt(const double a);                    // Sets commanded (HAE) altitude (feet)

   virtual void setTerrainElevation(const double v);                       // Sets the elevation of the terrain at this player's location (meters)
   virtual bool setTerrainOffset(const double v);                          // Sets the ground clamping offset (meters)
   virtual bool setInterpolateTerrain(const bool f);                       // sets the DTED terrain interpolation flag
   virtual bool setTerrainElevationRequired(const bool b);                 // Sets OTW terrain elevation request flag

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
   virtual bool setPosition(const base::Vec3d& newPos, const bool slaved = false);

   // Sets lat/long position; (degrees)
   virtual bool setPositionLL(const double lat, const double lon, const bool slaved = false);

   // Sets lat/long position; (degrees) and altitude (meters)
   virtual bool setPositionLLA(const double lat, const double lon, const double alt, const bool slaved = false);

   // Geocentric (ECEF) position vector (meters)
   virtual bool setGeocPosition(const base::Vec3d& gcPos, const bool slaved = false);

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
   virtual bool setEulerAngles(const base::Vec3d& newAngles);

   // Sets geocentric (body/ECEF) Euler angles: (radians) [ roll pitch yaw ]
   virtual bool setGeocEulerAngles(const base::Vec3d& newAngles);

   // Sets the rotational matrix
   virtual bool setRotMat(const base::Matrixd&);

   // Sets the quaternions
   virtual bool setQuaternions(const base::Quat&);

   // ---
   // Set the player's angular velocities:
   //    body and geocentric (ecef) coordinate systems
   //
   // 1) Setting the angular rate in either coordinate system will set the
   //    rates in both coordinate systems ( body and ecef ).
   //
   // 2) The player's position and rotation (see above) must be set prior to
   //    setting these angular rates to ensure that the transformational matrices
   //    have been computed.
   // ---

   // Sets the body angular velocities (radians/second)
   virtual bool setAngularVelocities(const double pa, const double qa, const double ra);
   virtual bool setAngularVelocities(const base::Vec3d& newAngVel);

   // Sets the body angular velocities (radians/second)
   virtual bool setGeocAngularVelocities(const base::Vec3d& newAngVel);

   // ---
   // Set the player's velocity and acceleration vectors
   //
   // 1) Setting velocity or acceleration in any one of the coordinate system
   //    will set the vectors in all three coordinate systems ( body, ned and ecef ).
   //
   // 2) The player's position and rotation (see above) must be set prior to
   //    setting these vectors to ensure that the transformational matrices
   //    have been computed.
   // ---

   // Sets local tangent plane velocities (NED); (meters/second)
   virtual bool setVelocity(const double ue, const double ve, const double we);
   virtual bool setVelocity(const base::Vec3d& newVel);

   // Sets local tangent plane accelerations (NED);
   virtual bool setAcceleration(const double due, const double dve, const double dwe);
   virtual bool setAcceleration(const base::Vec3d& newAccel);

   // Sets body velocities: (meters/second) [ ua -> fwd(+), va -> right(+), wa -> down(+) ]
   virtual bool setVelocityBody(const double ua, const double va, const double wa);
   virtual bool setVelocityBody(const base::Vec3d& newVelBody);

   // Sets body accelerations
   virtual bool setAccelerationBody(const double dua, const double dva, const double dwa);
   virtual bool setAccelerationBody(const base::Vec3d& newAccelBody);

   // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual bool setGeocVelocity(const double vx, const double vy, const double vz);
   virtual bool setGeocVelocity(const base::Vec3d& newVelEcef);

   // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   virtual bool setGeocAcceleration(const double dvx, const double dvy, const double dvz);
   virtual bool setGeocAcceleration(const base::Vec3d& newAccelEcef);

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
   virtual bool setInitGeocentricPosition(const base::Vec3d& newPos);

   // Initial position (after reset) relative to the simulation reference point (meters)
   virtual bool setInitPosition(const double north, const double east);

   // Initial position vector (after reset); north and east of simulation reference point (meters)
   virtual bool setInitPosition(const base::Vec2d& newPos);

   // ---
   // Roll Control Input
   //      Normalized inputs
   //      roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   // ---
   virtual void setControlStickRollInput(const double roll);

   // ---
   // Pitch Control Input
   //      Normalized inputs
   //      pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
   // ---
   virtual void setControlStickPitchInput(const double pitch);

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
   virtual int setThrottles(const double* const positions, const int num);

   // ---
   // Process weapon detonation
   // ---
   virtual void processDetonation(const double detRange, AbstractWeapon* const wpn = nullptr);

   // ---
   // Event handler(s)
   // ---
   virtual bool killedNotification(Player* const killedBy = nullptr);         // Handles the KILL_EVENT event
   virtual bool collisionNotification(Player* const msg);                     // Handles the CRASH_EVENT event with another player
   virtual bool crashNotification();                                          // Handles the CRASH_EVENT event
   virtual bool onWpnRelEvent(const base::Boolean* const msg = nullptr);      // Handles the WPN_REL_EVENT event
   virtual bool onTriggerSwEvent(const base::Boolean* const msg = nullptr);   // Handles the TRIGGER_SW_EVENT event
   virtual bool onTgtStepEvent();                                             // Handles the TGT_STEP_EVENT event
   virtual bool onRfEmissionEventPlayer(Emission* const msg);                 // Handles the RF_EMISSION event
   virtual bool onRfReflectedEmissionEventPlayer(Emission* const msg);        // Handles the RF_EMISSION event reflected to a 3rd party
   virtual bool onReflectionsRequest(base::Component* const msg);             // Handles the RF_REFLECTIONS_REQUEST event
   virtual bool onReflectionsCancel(const base::Component* const msg);        // Handles the RF_REFLECTIONS_CANCEL event
   virtual bool onIrMsgEventPlayer(IrQueryMsg* const msg);                    // Handle IR_QUERY_MSG event
   virtual bool onDatalinkMessageEventPlayer(base::Object* const msg);        // Handles the DATALINK_MESSAGE event
   virtual bool onDeEmissionEvent(base::Object* const msg);                   // Handles the DE_EMISSION event

   // Component methods
   virtual bool isFrozen() const override;
   virtual void reset() override;
   virtual void updateTC(const double dt = 0.0) override;
   virtual void updateData(const double dt = 0.0) override;
   virtual bool event(const int event, base::Object* const obj = nullptr) override;

public:

   // ---
   // Slot functions
   // ---
   virtual bool setSlotInitXPos(const base::Distance* const msg);
   virtual bool setSlotInitXPos(const base::Number* const msg);
   virtual bool setSlotInitYPos(const base::Distance* const msg);
   virtual bool setSlotInitYPos(const base::Number* const msg);
   virtual bool setSlotInitAlt(const base::Distance* const msg);
   virtual bool setSlotInitAlt(const base::Number* const msg);
   virtual bool setSlotInitPosition(const base::List* const msg);

   virtual bool setSlotInitLat(const base::LatLon* const msg);
   virtual bool setSlotInitLat(const base::Angle* const msg);
   virtual bool setSlotInitLat(const base::Number* const msg);
   virtual bool setSlotInitLon(const base::LatLon* const msg);
   virtual bool setSlotInitLon(const base::Angle* const msg);
   virtual bool setSlotInitLon(const base::Number* const msg);

   virtual bool setSlotInitGeocentric(const base::List* const msg);

   virtual bool setSlotInitRoll(const base::Angle* const msg);
   virtual bool setSlotInitRoll(const base::Number* const msg);
   virtual bool setSlotInitPitch(const base::Angle* const msg);
   virtual bool setSlotInitPitch(const base::Number* const msg);
   virtual bool setSlotInitHeading(const base::Angle* const msg);
   virtual bool setSlotInitHeading(const base::Number* const msg);
   virtual bool setSlotInitEulerAngles(const base::List* const msg);

   virtual bool setSlotInitVelocity(const base::Number* const msg);
   virtual bool setSlotInitVelocityKts(const base::Number* const msg);

   virtual bool setSlotID(const base::Number* const msg);
   virtual bool setSlotSide(base::String* const msg);
   virtual bool setSlotInitMode(base::String* const msg);
   virtual bool setSlotUseCoordSys(base::String* const msg);

   virtual bool setSlotSignature(RfSignature* const msg);
   virtual bool setSlotIrSignature(IrSignature* const msg);
   virtual bool setSlotCamouflageType(const base::Number* const msg);

   virtual bool setSlotTerrainElevReq(const base::Number* const msg);
   virtual bool setSlotInterpolateTerrain(const base::Number* const msg);
   virtual bool setSlotTerrainOffset(const base::Distance* const msg);

   virtual bool setSlotPositionFreeze(const base::Number* const msg);
   virtual bool setSlotAltitudeFreeze(const base::Number* const msg);
   virtual bool setSlotAttitudeFreeze(const base::Number* const msg);
   virtual bool setSlotFuelFreeze(const base::Number* const msg);
   virtual bool setSlotCrashOverride(const base::Number* const msg);
   virtual bool setSlotKillOverride(const base::Number* const msg);
   virtual bool setSlotKillRemoval(const base::Number* const msg);
   virtual bool setSlotEnableNetOutput(const base::Number* const msg);
   virtual bool setSlotDataLogTime(const base::Time* const msg);

   virtual bool setSlotTestRollRate(const base::Angle* const msg);
   virtual bool setSlotTestPitchRate(const base::Angle* const msg);
   virtual bool setSlotTestYawRate(const base::Angle* const msg);
   virtual bool setSlotTestBodyAxis(const base::Number* const msg);

protected:

   // Vehicle Dynamics -- called by updateTC() during phase zero
   virtual void dynamics(const double dt = 0.0);

   // Position update (local players only)
   void positionUpdate(const double dt);

   // Dead-reckoning (networked I-players only)
   void deadReckonPosition(const double dt);

   // Update the player's internal systems pointers
   // (e.g., after new system components have been added)
   virtual void updateSystemPointers();

   // Update terrain elevation at our location
   virtual void updateElevation();

   virtual bool shutdownNotification() override;
   virtual void printTimingStats() override;

   // These systems, from our subcomponent list, can only be set by reset()
   virtual bool setDynamicsModel(base::Pair* const sys); // Sets our dynamics model
   virtual bool setDatalink(base::Pair* const n);        // Sets our Datalink models
   virtual bool setGimbal(base::Pair* const n);          // Sets our gimbal/antenna/optic models
   virtual bool setNavigation(base::Pair* const n);      // Sets our navigation model
   virtual bool setOnboardComputer(base::Pair* const s); // Sets our onboard computer model
   virtual bool setPilot(base::Pair* const p);           // Sets our pilot model
   virtual bool setRadio(base::Pair* const n);           // Sets our radio models
   virtual bool setSensor(base::Pair* const n);          // Sets our sensor models
   virtual bool setIrSystem(base::Pair* const n);        // Sets our IR sensors models
   virtual bool setStoresMgr(base::Pair* const s);       // Sets our stores management model

   virtual void processComponents(              // Process our subcomponent list (which should be other steer points)
      base::PairStream* const list,             // Source list of components
      const std::type_info& filter,             // Type filter
      base::Pair* const add = nullptr,          // Optional pair to add
      base::Component* const remove = nullptr   // Optional subcomponent to remove
      ) override;

private:
   void initData();
   WorldModel* getSimulationImp();

   // ---
   // Player identity
   // ---
   base::safe_ptr<base::String> type;    // Type of vehicle
   Side side {GRAY};                     // Which side (see above)

   // ---
   // Player State
   // ---
   CoordSys    useCoordSys {CS_NONE};    // Coordinate system being used to update position
   CoordSys    useCoordSysN1 {CS_NONE};  // Previous 'useCoordSys'

   double      latitude {};         // Latitude                          (degrees)
   double      longitude {};        // Longitude                         (degrees)
   double      altitude {};         // Altitude                          (meters) (HAE)
   base::Vec3d  posVecNED;          // Local gaming area position vector (meters) [ x, y, z ] NED
   base::Vec3d  posVecECEF;         // Geocentric position vector        (meters)  (ECEF)

   base::Vec3d  velVecNED;          // Inertial axes velocity vector     (meters/second)  [ ue, ve, we ] NED
   base::Vec3d  velVecECEF;         // Geocentric velocity vector        (meters/second)  (ECEF)
   base::Vec3d  velVecBody;         // Body axes velocity vector         (meters/second)  [ ua  va  wa ]
   base::Vec3d  velVecN1;           // Previous velocity vector          (meters/second)  (based on 'useCoordSys')

   base::Vec3d  accelVecNED;        // Inertial axes acceleration vector ((meters/second)/second) [ due, dve, dwe ] NED
   base::Vec3d  accelVecECEF;       // Geocentric acceleration vector    ((meters/second)/second) (ECEF)
   base::Vec3d  accelVecBody;       // Body axes acceleration vector     ((meters/second)/second) [ dua, dva, dwa ]

   double      vp {};       // Total Velocity   (meters/second)
   double      gndSpd {};   // Ground Speed     (meters/second)
   double      gndTrk {};   // Ground Track     (radians)

   base::Vec3d  angles;     // Geodetic (body/NED) Euler angles (radians) [ roll pitch yaw ] AKA [ phi theta psi ]
   base::Vec2d  scPhi;      // Sin/Cos of roll (phi)
   base::Vec2d  scTheta;    // Sin/Cos of pitch (theta)
   base::Vec2d  scPsi;      // Sin/Cos of yaw (psi)

   base::Vec3d  anglesW;    // World (body/ECEF) Euler angles (radians)
   base::Vec2d  scPhiW;     // Sin/Cos of world phi
   base::Vec2d  scThetaW;   // Sin/Cos of world theta
   base::Vec2d  scPsiW;     // Sin/Cos of world psi

   base::Vec3d  angularVel; // Body angular velocities (radians/seconds)
   base::Vec3d  gcAngVel;   // Geocentric (ECEF) angular velocities (radians/seconds)

   base::Quat   q;          // Quaternions for the rotational matrix

   base::Matrixd rm;        // Rotational Matrix: inertial to body directional cosines
                                 //    RM = Rx[roll] * Ry[pitch] * Rz[yaw]

   base::Matrixd wm;        // World transformation matrix:
                            //    Local inertial tangent plane (NED) <==> World (ECEF)
                            //    WM = Ry[-(90+lat)] * Rz[lon]

   base::Matrixd rmW2B;     // Rotational Matrix: world to body directional cosines
                            //    RM = Rx[gcRoll] * Ry[gcPitch] * Rz[gcYaw]

   double tElev {};         // Terrain Elevation  (meters -- up+)
   bool   tElevValid {};    // Terrain elevation is valid
   bool   tElevReq {};      // Height-Of-Terrain is required from the OTW system (default: terrain height isn't required)
   bool   interpTrrn {};    // interpolate between terrain elevation posts (local terrain database only)
   double tOffset {};       // Offset from the terrain to the player's CG for ground clamping
   bool   posVecValid {};   // Local position vector valid
   bool   altSlaved {};     // Player's altitude is slaved to the dynamics software (default: false)
   bool   posSlaved {};     // Player's position is slaved to the dynamics software (default: false)
   bool   posFrz {};        // Player's position is frozen
   bool   altFrz {};        // Player's altitude is frozen
   bool   attFrz {};        // Player's attitude is frozen
   bool   fuelFrz {};       // Player's fuel quantity is frozen
   bool   crashOverride {}; // If true, player can NOT crash
   bool   killOverride {};  // If true, player can NOT be killed
   bool   killRemoval {};   // If true then on kill notification the player will be set to KILLED and eventually removed

   // ---
   // Appearance
   // ---
   base::safe_ptr<RfSignature> signature;    // Player's RCS signature
   base::safe_ptr<IrSignature> irSignature;  // Player's IR signature
   unsigned int camouflage {};               // Camouflage type (0 is none)
   double      damage {};                    // Damage state from no damage(0.0) to destroyed (1.0)
   double      smoking {};                   // Smoke state from no smoke (0.0) to maximum (1.0)
   double      flames {};                    // Flames state from no flames (0.0) to maximum (1.0)
   bool        justKilled {};                // Just killed flag
   unsigned short killedBy {};               // Killed by player ID

   // ---
   // Initialization, Controls, Freeze and Reset
   // ---
   base::Vec2d initPosVec;        // Initial Position vector [x y ]  (meters)
   bool        initPosFlg {};     // Initial position vector is valid

   base::Vec3d initGeoPosVec;     // Initial geocentric position vector [ x y z ] (ECEF) (meters)
   bool        initGeoPosFlg {};  // Initial position vector is valid

   double initLat {};             // Initial latitude (degrees)
   double initLon {};             // Initial longitude (degrees)
   bool initLatLonFlg {};         // Initial lat/lon valid

   double      initAlt {};        // Initial altitude (HAE)
   double      initVp {};         // Initial Total Velocity
   base::Vec3d  initAngles;       // Initial (Euler) angles (radians)
   base::Vec3d  testAngRates;     // Test angular rates (radians/sec)
   bool        testBodyAxis {};   // Test angular rates are in the body axis else they're Euler rates

   // ---
   // Data collection timer
   // ---
   double dataLogTimer {};        // Data log timer (seconds)
   double dataLogTime {};         // Data log time (seconds)

   // ---
   // System pointers
   // ---
   WorldModel* sim {};            // World model we operate within (not ref()'d)

   base::Pair* dynamicsModel {};  // Dynamics Model (ref()'d)
   base::Pair* datalink {};       // Datalink model (ref()'d)
   base::Pair* gimbal {};         // Gimbal/antenna/optic models (ref()'d)
   base::Pair* nav {};            // Navigation (ref()'d)
   base::Pair* obc {};            // Onboard Computer (ref()'d)
   base::Pair* pilot {};          // Pilot Model (e.g., autopilot, PDL) (ref()'d)
   base::Pair* radio {};          // Radio models (ref()'d)
   base::Pair* sensor {};         // R/F Sensor models (ref()'d)
   base::Pair* irSystem {};       // IR Sensor models (ref()'d)
   base::Pair* sms {};            // Stores Management System (ref()'d)
   bool loadSysPtrs {true};       // Load system pointers flag

   // ---
   // Reflected emissions
   // ---
   static const unsigned int MAX_RF_REFLECTIONS = 4;               // Max number of reflected emissions we'll send (let's keep it small)
   std::array<base::Component*, MAX_RF_REFLECTIONS> rfReflect {};  // Objects that are interested in the emissions hitting us
   std::array<double, MAX_RF_REFLECTIONS> rfReflectTimer {};       // Request for reflected emissions will timeout

   // ---
   // sync state changes
   // ---
   bool              syncState1Ready {};
   bool              syncState2Ready {};
   SynchronizedState syncState1;
   SynchronizedState syncState2;
};

#include "openeaagles/models/players/Player.inl"

}
}

#endif
