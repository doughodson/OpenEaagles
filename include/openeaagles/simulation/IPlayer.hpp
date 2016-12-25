
#ifndef __oe_simulation_IPlayer_H__
#define __oe_simulation_IPlayer_H__

#include "openeaagles/base/Component.hpp"

namespace oe {

namespace osg {
class Vec2d;
class Vec3d;
class Quat;
class Matrixd;
}

namespace base {
class Angle;
class Boolean;
class Distance;
class LatLon;
class List;
class Time;
}

namespace simulation {

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
// Class: IPlayer
//
// Description: Abstract interface for all players (e.g., aircraft, ground vehicles, etc.)
//              Provides interfaces for player IDs, types, status, state vectors,
//              control flags, subcomponent lists, event handling, etc.
//
//
//------------------------------------------------------------------------------
// Factory name: Player
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
//                                        !   (default: based on the coord system used to set the initial position)
//
//    signature      <RfSignature>        ! Player's RCS signature (default: 0)
//    irSignature    <IrSignature>        ! Player's IR signature (default: 0)
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
//                                        ! logger, or zero if none (default: 0)
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
//    RF_REFLECTIONS_REQUEST  <base::Component>   ! Requests for reflected R/F emissions (must re-request once a second)
//    RF_REFLECTIONS_CANCEL   <base::Component>   ! Cancel requests for reflected R/F emissions
//    WPN_REL_EVENT           <base::Boolean>     ! Weapon release switch with position
//    WPN_REL_EVENT           (none)               ! Weapon release switch (single shot)
//    TRIGGER_SW_EVENT        <base::Boolean>     ! Trigger switch with position
//    TRIGGER_SW_EVENT        (none)               ! Trigger switch (single shot)
//    TGT_STEP_EVENT          (none)               ! Target step switch
//    DATALINK_MESSAGE        <base::Object>      ! Hit with a datalink message
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
//       Use simulation::getWorldMat() to transform vectors between ECEF and
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
class IPlayer : public base::Component
{
   DECLARE_SUBCLASS(IPlayer, base::Component)

public:
   IPlayer();

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

   // ---
   // Type and ID
   // ---

   virtual unsigned int getMajorType() const =0;                    // The player's 'major type' enum
   virtual bool isMajorType(const unsigned int tst) const =0;       // True if player is of these (bit-wise or'd) major types

   virtual const base::String* getType() const =0;                  // The player's type string (.e.g, "F-16C")

   virtual bool isID(const unsigned short tst) const =0;            // True if player's ID matches
   virtual unsigned short getID() const =0;                         // The player's ID

   virtual bool isName(const base::Identifier* const tst) const =0; // True if the player's name matches
   virtual bool isName(const char* const tst) const =0;             // True if the player's name matches
   virtual const base::Identifier* getName() const =0;              // The player's name

   virtual Side getSide() const =0;                                 // The 'side' that the player is on.
   virtual bool isSide(const unsigned int tst) const =0;            // True if player is with one of these (bit-wise or'd) sides
   virtual bool isNotSide(const unsigned int tst) const =0;         // True if player is not with one one of these (bit-wise or'd) sides

   // ---
   // State data
   // ---

   virtual Mode getMode() const =0;                           // Current mode ( INACTIVE, ACTIVE, DETONATED ... )
   virtual bool isActive() const =0;                          // True if player's mode is active
   virtual bool isKilled() const =0;                          // True if player's mode is killed
   virtual bool isCrashed() const =0;                         // True if player's mode is crashed
   virtual bool isDetonated() const =0;                       // True if player has detonated (weapons only)
   virtual bool isInactive() const =0;                        // True if player's mode is inactive
   virtual bool isMode(const Mode tst) const =0;              // True if player is currently this mode
   virtual bool isNotMode(const Mode tst) const =0;           // True if player is not currently this mode
   virtual bool isDead() const =0;                            // True if player's mode is dead (Killed, Crashed, Detonated, etc....)

   virtual double getRoll() const =0;                         // Roll Euler angle (radians) by default
   virtual double getRollR() const =0;                        // Roll Euler angle (radians)
   virtual double getRollD() const =0;                        // Roll Euler angle (degrees)
   virtual double getSinRoll() const =0;                      // Sin of the Euler roll angle
   virtual double getCosRoll() const =0;                      // Cos of the  Euler roll angle

   virtual double getPitch() const =0;                        // Pitch Euler angle (radians) by default
   virtual double getPitchR() const =0;                       // Pitch Euler angle (radians)
   virtual double getPitchD() const =0;                       // Pitch Euler angle (degrees)
   virtual double getSinPitch() const =0;                     // Sin of the pitch Euler angle
   virtual double getCosPitch() const =0;                     // Cos of the  pitch Euler angle

   virtual double getHeading() const =0;                      // Yaw Euler angle (radians) by default
   virtual double getHeadingR() const =0;                     // Yaw Euler angle (radians)
   virtual double getHeadingD() const =0;                     // Yaw Euler angle (degrees)
   virtual double getSinHeading() const =0;                   // Sin of the yaw Euler angle
   virtual double getCosHeading() const =0;                   // Cos of the  yaw Euler angle

   virtual const osg::Vec3d& getEulerAngles() const =0;       // Euler angles (radians); geodetic (body/NED)
   virtual const osg::Quat& getQuaternions() const =0;        // Rotational Quaternions

   virtual const osg::Matrixd& getRotMat() const =0;          // Rotational Matrix: (directional cosines)
                                                              //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw]
                                                              //    Usage:
                                                              //       Vb = M * Vi
                                                              //       Vi  = Vb * M
                                                              //    Where: 'Vb' is a body vector; 'Vi' is an inertial vector

   virtual const osg::Matrixd& getRotMatW2B() const =0;       // Rotational Matrix: world to body
                                                              //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw] * Ry[-(90+lat)] * Rz[lon]
                                                              //    Usage:
                                                              //       Vb = M * Vw
                                                              //       Vw  = Vb * M
                                                              //    Where: 'Vb' is a body vector; 'Vw' is a world (ECEF) vector

   virtual const osg::Vec3d& getGeocEulerAngles() const =0;         // Geocentric (body/ECEF) Euler angles

   virtual const osg::Vec3d& getAngularVelocities() const =0;       // Body angular rates (radians/second)
   virtual const osg::Vec3d& getGeocAngularVelocities() const =0;   // Geocentric angular rates (radians/second)

   virtual const osg::Vec3d& getGeocPosition() const =0; // Geocentric (ECEF) position vector [ x y z ] (meters)

   virtual double getLatitude() const =0;                // Player's latitude (degrees)
   virtual double getLongitude() const =0;               // Player's longitude (degrees)

   virtual const osg::Matrixd& getWorldMat() const =0;   // World transformation matrix:
                                                         //    Local inertial tangent plane (NED) <==> World (ECEF)
                                                         //    Matrix: M = Ry[-(90+lat)] * Rz[lon]
                                                         //    Usage:
                                                         //       Vi = M * Vw;
                                                         //       Vw  = Vi * M
                                                         //    Where: 'Vw' is a world(ECEF) vector; 'Vi' is an inertial (NED) vector

   virtual double getEarthRadius() const =0;             // Earth radius at player's location (meters)

   virtual bool getPositionLL(double* const lat, double* const lon) const =0;                     // Player's Lat/lon (degrees)
   virtual bool getPositionLLA(double* const lat, double* const lon, double* const alt) const =0; // Player's Lat/Lon (degrees) and altitude (meters)

   virtual double getXPosition() const =0;                    // North(+) or south(-) of the sim reference point (meters)
   virtual double getYPosition() const =0;                    // East(+) or west(-) of the sim reference point (meters)
   virtual const osg::Vec3d& getPosition() const =0;          // Position vector; NED from sim reference point (meters)
   virtual bool isPositionVectorValid() const =0;             // Is the position vector valid

   virtual double getAltitude() const =0;                     // Altitude HAE (meters) default
   virtual double getAltitudeM() const =0;                    // Altitude HAE (meters)
   virtual double getAltitudeFt() const =0;                   // Altitude HAE (feet)

   virtual double getAltitudeAgl() const =0;                  // Altitude AGL (meters) default
   virtual double getAltitudeAglM() const =0;                 // Altitude AGL (meters)
   virtual double getAltitudeAglFt() const =0;                // Altitude AGL (feet)

   virtual bool isTerrainElevationValid() const =0;           // Terrain elevation is valid (or at least was where it was set)
   virtual double getTerrainElevation() const =0;             // Terrain elevation at player (meters) default
   virtual double getTerrainElevationM() const =0;            // Terrain elevation at player (meters)
   virtual double getTerrainElevationFt() const =0;           // Terrain elevation at player (feet)

   virtual double getTotalVelocity() const =0;                // Total velocity (meters/second)
   virtual double getTotalVelocityFPS() const =0;             // Total velocity (feet/second)
   virtual double getTotalVelocityKts() const =0;             // Total velocity (knots)

   virtual double getGroundSpeed() const =0;                  // Ground speed (meters/second)
   virtual double getGroundSpeedFPS() const =0;               // Ground speed (feet/second)
   virtual double getGroundSpeedKts() const =0;               // Ground speed (knots)

   virtual double getGroundTrack() const =0;                  // Ground track (radians) default
   virtual double getGroundTrackR() const =0;                 // Ground track (radians)
   virtual double getGroundTrackD() const =0;                 // Ground track (degrees)

   virtual const osg::Vec3d& getVelocity() const =0;          // Velocity vector; NED (meters/second)
   virtual const osg::Vec3d& getAcceleration() const =0;      // Acceleration vector; NED ((meters/second)/second)

   virtual const osg::Vec3d& getGeocVelocity() const =0;      // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual const osg::Vec3d& getGeocAcceleration() const =0;  // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)

   virtual const osg::Vec3d& getVelocityBody() const =0;      // Body velocity vector: Nose/right/down (meters/second)
   virtual const osg::Vec3d& getAccelerationBody() const =0;  // Body Acceleration vector: Nose/right/down ((meters/second)/second)

   virtual double getGrossWeight() const =0;          // Player's gross weight (lbs)
   virtual double getMach() const =0;                 // Player's mach number
   virtual double getCG() const =0;                   // Player's Center-of-Gravity (%)

   // ---
   // Appearance
   // ---

   virtual unsigned int getCamouflageType() const =0;    // Returns the user defined camouflage type (or zero for none)

   virtual bool isDestroyed() const =0;                  // True if player is destroyed
   virtual double getDamage() const =0;                  // Returns the player's damage state - no damage(0.0) to destroyed (1.0)
   virtual double getSmoke() const =0;                   // Returns the player's smoke state - (0.0) to maximum (1.0)
   virtual double getFlames() const =0;                  // Returns the player's flames state - (0.0) to maximum (1.0)

   virtual bool isJustKilled() const =0;                 // True if the player was just killed
   virtual int killedByPlayerID() const =0;              // ID of the attacking player that killed us

   // ---
   // Initialization data
   // ---
   virtual const osg::Vec3d& getInitGeocentricPosition() const =0; // Geocentric (ECEF) initial position vector [ x y z ] (meters)
   virtual bool isInitGeocentricPositionValid() const =0;

   virtual double getInitLatitude() const =0;                 // Player's initial latitude (degrees)
   virtual double getInitLongitude() const =0;                // Player's initial longitude (degrees)
   virtual bool isInitLatLonValid() const =0;
   virtual double getInitVelocity() const =0;                 // Player's initial total velocity

   virtual const osg::Vec2d& getInitPosition() const =0;      // Initial position vector; north and east of sim reference point (meters)
   virtual bool isInitPositionValid() const =0;

   virtual double getInitAltitude() const =0;                 // Initial altitude HAE (meters)

   virtual const osg::Vec3d& getInitAngles() const =0;        // Initial Euler angles (radians)

   // ---
   // Control parameters
   // ---
   virtual bool isPositionFrozen() const =0;                  // True if the player is frozen (paused)
   virtual bool isAltitudeFrozen() const =0;                  // True if the player's altitude is frozen
   virtual bool isAttitudeFrozen() const =0;                  // True if the player's attitude is frozen
   virtual bool isFuelFrozen() const =0;                      // True if the player's fuel is frozen
   virtual bool isCrashOverride() const =0;                   // True if the player is in crash override mode
   virtual bool isKillOverride() const =0;                    // True if the player is in kill override mode
   virtual bool isKillRemovalEnabled() const =0;              // True if the player's kill removal flag is set
   virtual bool isAltitudeSlaved() const =0;                  // True if altitude is slaved to an external model
   virtual bool isPositionSlaved() const =0;                  // True if position is slaved to an external model

   virtual double getTerrainOffset() const =0;                // Ground clamping offset from the terrain to our CG (meters)
   virtual bool isDtedTerrainInterpolationEnabled() const =0; // Interpolate our terrain from DTED if true

   virtual bool isTerrainElevationRequired() const =0;        // True if terrain elevation is required from the OTW system;
                                                              // otherwise we use DTED, if available

   virtual CoordSys getCoordSystemInUse() const =0;           // Returns the coordinate system currently being used to
                                                              // update the player's position

   // ---
   // Interoperability network data
   // ---

   virtual bool isNetworkedPlayer() const =0;        // True if this is a networked player (IPlayer)
   virtual bool isLocalPlayer() const =0;            // True if this is a local player

   virtual int getNetworkID() const =0;                       // ID of a networked player's controlling network model
   virtual Nib* getNib() =0;                                  // Networked player's Nib object
   virtual const Nib* getNib() const =0;                      // Networked player's Nib object  (const version)

   virtual bool isNetOutputEnabled() const =0;                        // Is player output to the network enabled?
   virtual Nib* getLocalNib(const unsigned int netId) =0;             // Player's outgoing NIB(s)
   virtual const Nib* getLocalNib(const unsigned int netId) const =0; // Player's outgoing NIB(s)  (const version)

   // ---
   // Internal autopilot controls
   // ---
   virtual bool isHeadingHoldOn() const =0;           // True if heading-hold mode is on?
   virtual double getCommandedHeading() const =0;     // Commanded (true) heading, default (radians)
   virtual double getCommandedHeadingD() const =0;    // Commanded (true) heading (degrees)
   virtual double getCommandedHeadingR() const =0;    // Commanded (true) heading (radians)

   virtual bool isVelocityHoldOn() const =0;          // True if velocity-hold mode is on?
   virtual double getCommandedVelocityKts() const =0; // Commanded (true) velocity (knots)
   virtual double getCommandedVelocityFps() const =0; // Commanded (true) velocity (feet/second)
   virtual double getCommandedVelocityMps() const =0; // Commanded (true) velocity (meters/second)

   virtual bool isAltitudeHoldOn() const =0;          // True if altitude-hold mode is on?
   virtual double getCommandedAltitude() const =0;    // Get commanded (HAE) altitude, default (meters)
   virtual double getCommandedAltitudeM() const =0;   // Get commanded (HAE) altitude (meters)
   virtual double getCommandedAltitudeFt() const =0;  // Get commanded (HAE) altitude (feet)

   // ---
   // Sync State Changes
   // ---

   // DID NOT IMPORT

   // ---
   // Systems
   // ---

   // DID NOT IMPORT



   // ---
   // Set functions
   // ---

   virtual bool setType(const base::String* const newTypeString) =0; // Sets the player's type string
   virtual void setName(const base::Identifier& newName) =0;         // Sets the player's name (base::String version)
   virtual void setName(const char* const newName) =0;               // Sets the player's name (char* version)
   virtual void setID(const unsigned short newId) =0;                // Sets the player's ID
   virtual void setSide(const Side newSide) =0;                      // Sets the player's side enum
   virtual void setMode(const Mode newMode) =0;                      // Sets the player's current mode
   virtual void setInitMode(const Mode newMode) =0;                  // Sets the player's initial mode (after reset)
   virtual bool setUseCoordSys(const CoordSys) =0;                   // Sets the coord system to use for updating position

   virtual bool setFuelFreeze(const bool f) =0;                      // Sets the player's fuel freeze flag
   virtual bool setCrashOverride(const bool f) =0;                   // Sets the player's crash override flag
   virtual bool setKillOverride(const bool f) =0;                    // Sets the player's kill override flag
   virtual bool setKillRemoval(const bool f) =0;                     // Sets the player's kill removal flag
   virtual void resetJustKilled() =0;                                // Resets the just killed flag
   virtual bool setDamage(const double v) =0;                        // Sets the player's damage state
   virtual bool setSmoke(const double v) =0;                         // Sets the player's smoke state
   virtual bool setFlames(const double v) =0;                        // Sets the player's flames state
   virtual bool setCamouflageType(const unsigned int v) =0;          // Sets the user defined camouflage type (or zero for none)
   virtual bool setPositionFreeze(const bool f) =0;                  // Sets the player's freeze flag
   virtual bool setAltitudeFreeze(const bool f) =0;                  // Sets the player's altitude freeze flag
   virtual bool setAttitudeFreeze(const bool f) =0;                  // Sets the player's attitude freeze flag

   virtual bool setHeadingHoldOn(const bool b) =0;                   // Turns heading-hold mode on/off
   virtual bool setCommandedHeading(const double h) =0;              // Sets the commanded (true) heading default (radians)
   virtual bool setCommandedHeadingD(const double h) =0;             // Sets the commanded (true) heading (degrees)
   virtual bool setCommandedHeadingR(const double h) =0;             // Sets the commanded (true) heading (radians)

   virtual bool setVelocityHoldOn(const bool b) =0;                  // Turns velocity-hold mode on/off
   virtual bool setCommandedVelocityKts(const double a) =0;          // Sets the commanded (true) velocity (knots)

   virtual bool setAltitudeHoldOn(const bool b) =0;                  // Turns altitude-hold mode on/off
   virtual bool setCommandedAltitude(const double a) =0;             // Sets commanded (HAE) altitude, default (meters)
   virtual bool setCommandedAltitudeM(const double a) =0;            // Sets commanded (HAE) altitude (meters)
   virtual bool setCommandedAltitudeFt(const double a) =0;           // Sets commanded (HAE) altitude (feet)

   virtual bool setNib(Nib* const p) =0;                             // Sets the networked player's Nib object

   virtual bool setEnableNetOutput(const bool f) =0;                 // Sets the network output enabled flag
   virtual bool setOutgoingNib(Nib* const p, const unsigned int id) =0; // Sets the outgoing NIB for network 'id'

   virtual void setTerrainElevation(const double v) =0;              // Sets the elevation of the terrain at this player's location (meters)
   virtual bool setTerrainOffset(const double v) =0;                 // Sets the ground clamping offset (meters)
   virtual bool setInterpolateTerrain(const bool f) =0;              // sets the DTED terrain interpolation flag
   virtual bool setTerrainElevationRequired(const bool b) =0;        // Sets OTW terrain elevation request flag

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
   virtual bool setAltitude(const double alt, const bool slaved = false) =0;

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const bool slaved = false) =0;

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const double down, const bool slaved = false) =0;

   // Position vector; NED from simulation reference point (meters)
   virtual bool setPosition(const osg::Vec3d& newPos, const bool slaved = false) =0;

   // Sets lat/long position; (degrees)
   virtual bool setPositionLL(const double lat, const double lon, const bool slaved = false) =0;

   // Sets lat/long position; (degrees) and altitude (meters)
   virtual bool setPositionLLA(const double lat, const double lon, const double alt, const bool slaved = false) =0;

   // Geocentric (ECEF) position vector (meters)
   virtual bool setGeocPosition(const osg::Vec3d& gcPos, const bool slaved = false) =0;

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
   virtual bool setEulerAngles(const double r, const double p, const double y) =0;
   virtual bool setEulerAngles(const osg::Vec3d& newAngles) =0;

   // Sets geocentric (body/ECEF) Euler angles: (radians) [ roll pitch yaw ]
   virtual bool setGeocEulerAngles(const osg::Vec3d& newAngles) =0;

   // Sets the rotational matrix
   virtual bool setRotMat(const osg::Matrixd&) =0;

   // Sets the quaternions
   virtual bool setQuaternions(const osg::Quat&) =0;

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
   virtual bool setAngularVelocities(const double pa, const double qa, const double ra) =0;
   virtual bool setAngularVelocities(const osg::Vec3d& newAngVel) =0;

   // Sets the body angular velocities (radians/second)
   virtual bool setGeocAngularVelocities(const osg::Vec3d& newAngVel) =0;

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
   virtual bool setVelocity(const double ue, const double ve, const double we) =0;
   virtual bool setVelocity(const osg::Vec3d& newVel) =0;

   // Sets local tangent plane accelerations (NED);
   virtual bool setAcceleration(const double due, const double dve, const double dwe) =0;
   virtual bool setAcceleration(const osg::Vec3d& newAccel) =0;

   // Sets body velocities: (meters/second) [ ua -> fwd(+), va -> right(+), wa -> down(+) ]
   virtual bool setVelocityBody(const double ua, const double va, const double wa) =0;
   virtual bool setVelocityBody(const osg::Vec3d& newVelBody) =0;

   // Sets body accelerations
   virtual bool setAccelerationBody(const double dua, const double dva, const double dwa) =0;
   virtual bool setAccelerationBody(const osg::Vec3d& newAccelBody) =0;

   // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual bool setGeocVelocity(const double vx, const double vy, const double vz) =0;
   virtual bool setGeocVelocity(const osg::Vec3d& newVelEcef) =0;

   // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   virtual bool setGeocAcceleration(const double dvx, const double dvy, const double dvz) =0;
   virtual bool setGeocAcceleration(const osg::Vec3d& newAccelEcef) =0;


};

}
}

#endif
