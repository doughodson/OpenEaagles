
#ifndef __oe_models_Player_H__
#define __oe_models_Player_H__

#include "openeaagles/simulation/IPlayer.hpp"

#include "openeaagles/models/SynchronizedState.hpp"

#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/osg/Matrixd"
#include "openeaagles/base/osg/Quat"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"

namespace oe {

namespace osg {
class Vec2d;
class Vec3d;
}

namespace base {
class Angle;
class Boolean;
class Distance;
class LatLon;
class List;
class Time;
}

namespace simulation { class INib; }

namespace models {

class Simulation;

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
class IrQueryMsg;
class IrSignature;
class RfSignature;
class Track;

class Player : public simulation::IPlayer
{
   DECLARE_SUBCLASS(Player, simulation::IPlayer)

public:
   Player();

   // ---
   // Type and ID
   // ---

   virtual unsigned int getMajorType() const override;                    // The player's 'major type' enum
   virtual bool isMajorType(const unsigned int tst) const override;       // True if player is of these (bit-wise or'd) major types

   virtual const base::String* getType() const override;                  // The player's type string (.e.g, "F-16C")

   virtual bool isID(const unsigned short tst) const override;            // True if player's ID matches
   virtual unsigned short getID() const override;                         // The player's ID

   virtual bool isName(const base::Identifier* const tst) const override; // True if the player's name matches
   virtual bool isName(const char* const tst) const override;             // True if the player's name matches
   virtual const base::Identifier* getName() const override;              // The player's name

   virtual Side getSide() const override;                                 // The 'side' that the player is on.
   virtual bool isSide(const unsigned int tst) const override;            // True if player is with one of these (bit-wise or'd) sides
   virtual bool isNotSide(const unsigned int tst) const override;         // True if player is not with one one of these (bit-wise or'd) sides

   // ---
   // State data
   // ---

   virtual Mode getMode() const override;                           // Current mode ( INACTIVE, ACTIVE, DETONATED ... )
   virtual bool isActive() const override;                          // True if player's mode is active
   virtual bool isKilled() const override;                          // True if player's mode is killed
   virtual bool isCrashed() const override;                         // True if player's mode is crashed
   virtual bool isDetonated() const override;                       // True if player has detonated (weapons only)
   virtual bool isInactive() const override;                        // True if player's mode is inactive
   virtual bool isMode(const Mode tst) const override;              // True if player is currently this mode
   virtual bool isNotMode(const Mode tst) const override;           // True if player is not currently this mode
   virtual bool isDead() const override;                            // True if player's mode is dead (Killed, Crashed, Detonated, etc....)

   virtual double getRoll() const override;                         // Roll Euler angle (radians) by default
   virtual double getRollR() const override;                        // Roll Euler angle (radians)
   virtual double getRollD() const override;                        // Roll Euler angle (degrees)
   virtual double getSinRoll() const override;                      // Sin of the Euler roll angle
   virtual double getCosRoll() const override;                      // Cos of the  Euler roll angle

   virtual double getPitch() const override;                        // Pitch Euler angle (radians) by default
   virtual double getPitchR() const override;                       // Pitch Euler angle (radians)
   virtual double getPitchD() const override;                       // Pitch Euler angle (degrees)
   virtual double getSinPitch() const override;                     // Sin of the pitch Euler angle
   virtual double getCosPitch() const override;                     // Cos of the  pitch Euler angle

   virtual double getHeading() const override;                      // Yaw Euler angle (radians) by default
   virtual double getHeadingR() const override;                     // Yaw Euler angle (radians)
   virtual double getHeadingD() const override;                     // Yaw Euler angle (degrees)
   virtual double getSinHeading() const override;                   // Sin of the yaw Euler angle
   virtual double getCosHeading() const override;                   // Cos of the  yaw Euler angle

   virtual const osg::Vec3d& getEulerAngles() const override;       // Euler angles (radians); geodetic (body/NED)
   virtual const osg::Quat& getQuaternions() const override;        // Rotational Quaternions

   virtual const osg::Matrixd& getRotMat() const override;          // Rotational Matrix: (directional cosines)
                                                                    //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw]
                                                                    //    Usage:
                                                                    //       Vb = M * Vi
                                                                    //       Vi  = Vb * M
                                                                    //    Where: 'Vb' is a body vector; 'Vi' is an inertial vector

   virtual const osg::Matrixd& getRotMatW2B() const override;       // Rotational Matrix: world to body
                                                                    //    Matrix: M = Rx[roll] * Ry[pitch] * Rz[yaw] * Ry[-(90+lat)] * Rz[lon]
                                                                    //    Usage:
                                                                    //       Vb = M * Vw
                                                                    //       Vw  = Vb * M
                                                                    //    Where: 'Vb' is a body vector; 'Vw' is a world (ECEF) vector

   virtual const osg::Vec3d& getGeocEulerAngles() const override;   // Geocentric (body/ECEF) Euler angles

   virtual const osg::Vec3d& getAngularVelocities() const override;       // Body angular rates (radians/second)
   virtual const osg::Vec3d& getGeocAngularVelocities() const override;   // Geocentric angular rates (radians/second)

   virtual const osg::Vec3d& getGeocPosition() const override; // Geocentric (ECEF) position vector [ x y z ] (meters)

   virtual double getLatitude() const override;                     // Player's latitude (degrees)
   virtual double getLongitude() const override;                    // Player's longitude (degrees)

   virtual const osg::Matrixd& getWorldMat() const override;        // World transformation matrix:
                                                   //    Local inertial tangent plane (NED) <==> World (ECEF)
                                                   //    Matrix: M = Ry[-(90+lat)] * Rz[lon]
                                                   //    Usage:
                                                   //       Vi = M * Vw;
                                                   //       Vw  = Vi * M
                                                   //    Where: 'Vw' is a world(ECEF) vector; 'Vi' is an inertial (NED) vector

   virtual double getEarthRadius() const override;                  // Earth radius at player's location (meters)

   virtual bool getPositionLL(double* const lat, double* const lon) const override;                     // Player's Lat/lon (degrees)
   virtual bool getPositionLLA(double* const lat, double* const lon, double* const alt) const override; // Player's Lat/Lon (degrees) and altitude (meters)

   virtual double getXPosition() const override;                    // North(+) or south(-) of the sim reference point (meters)
   virtual double getYPosition() const override;                    // East(+) or west(-) of the sim reference point (meters)
   virtual const osg::Vec3d& getPosition() const override; // Position vector; NED from sim reference point (meters)
   virtual bool isPositionVectorValid() const override;             // Is the position vector valid

   virtual double getAltitude() const override;                     // Altitude HAE (meters) default
   virtual double getAltitudeM() const override;                    // Altitude HAE (meters)
   virtual double getAltitudeFt() const override;                   // Altitude HAE (feet)

   virtual double getAltitudeAgl() const override;                  // Altitude AGL (meters) default
   virtual double getAltitudeAglM() const override;                 // Altitude AGL (meters)
   virtual double getAltitudeAglFt() const override;                // Altitude AGL (feet)

   virtual bool isTerrainElevationValid() const override;           // Terrain elevation is valid (or at least was where it was set)
   virtual double getTerrainElevation() const override;             // Terrain elevation at player (meters) default
   virtual double getTerrainElevationM() const override;            // Terrain elevation at player (meters)
   virtual double getTerrainElevationFt() const override;           // Terrain elevation at player (feet)

   virtual double getTotalVelocity() const override;                // Total velocity (meters/second)
   virtual double getTotalVelocityFPS() const override;             // Total velocity (feet/second)
   virtual double getTotalVelocityKts() const override;             // Total velocity (knots)

   virtual double getGroundSpeed() const override;                  // Ground speed (meters/second)
   virtual double getGroundSpeedFPS() const override;               // Ground speed (feet/second)
   virtual double getGroundSpeedKts() const override;               // Ground speed (knots)

   virtual double getGroundTrack() const override;                  // Ground track (radians) default
   virtual double getGroundTrackR() const override;                 // Ground track (radians)
   virtual double getGroundTrackD() const override;                 // Ground track (degrees)

   virtual const osg::Vec3d& getVelocity() const override;          // Velocity vector; NED (meters/second)
   virtual const osg::Vec3d& getAcceleration() const override;      // Acceleration vector; NED ((meters/second)/second)

   virtual const osg::Vec3d& getGeocVelocity() const override;      // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual const osg::Vec3d& getGeocAcceleration() const override;  // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)

   virtual const osg::Vec3d& getVelocityBody() const override;      // Body velocity vector: Nose/right/down (meters/second)
   virtual const osg::Vec3d& getAccelerationBody() const override;  // Body Acceleration vector: Nose/right/down ((meters/second)/second)

   virtual double getGrossWeight() const override;          // Player's gross weight (lbs)
   virtual double getMach() const override;                 // Player's mach number
   virtual double getCG() const override;                   // Player's Center-of-Gravity (%)


   // ---
   // Appearance
   // ---

   virtual unsigned int getCamouflageType() const override;     // Returns the user defined camouflage type (or zero for none)

   virtual bool isDestroyed() const override;                   // True if player is destroyed
   virtual double getDamage() const override;                   // Returns the player's damage state - no damage(0.0) to destroyed (1.0)
   virtual double getSmoke() const override;                    // Returns the player's smoke state - (0.0) to maximum (1.0)
   virtual double getFlames() const override;                   // Returns the player's flames state - (0.0) to maximum (1.0)

   virtual bool isJustKilled() const override;                  // True if the player was just killed
   virtual int killedByPlayerID() const override;               // ID of the attacking player that killed us

   // ---
   // Initialization data
   // ---
   virtual const osg::Vec3d& getInitGeocentricPosition() const override; // Geocentric (ECEF) initial position vector [ x y z ] (meters)
   virtual bool isInitGeocentricPositionValid() const override;

   virtual double getInitLatitude() const override;                 // Player's initial latitude (degrees)
   virtual double getInitLongitude() const override;                // Player's initial longitude (degrees)
   virtual bool isInitLatLonValid() const override;
   virtual double getInitVelocity() const override;                 // Player's initial total velocity

   virtual const osg::Vec2d& getInitPosition() const override;      // Initial position vector; north and east of sim reference point (meters)
   virtual bool isInitPositionValid() const override;

   virtual double getInitAltitude() const override;                 // Initial altitude HAE (meters)

   virtual const osg::Vec3d& getInitAngles() const override;        // Initial Euler angles (radians)

   // ---
   // Control parameters
   // ---
   virtual bool isPositionFrozen() const override;                  // True if the player is frozen (paused)
   virtual bool isAltitudeFrozen() const override;                  // True if the player's altitude is frozen
   virtual bool isAttitudeFrozen() const override;                  // True if the player's attitude is frozen
   virtual bool isFuelFrozen() const override;                      // True if the player's fuel is frozen
   virtual bool isCrashOverride() const override;                   // True if the player is in crash override mode
   virtual bool isKillOverride() const override;                    // True if the player is in kill override mode
   virtual bool isKillRemovalEnabled() const override;              // True if the player's kill removal flag is set
   virtual bool isAltitudeSlaved() const override;                  // True if altitude is slaved to an external model
   virtual bool isPositionSlaved() const override;                  // True if position is slaved to an external model

   virtual double getTerrainOffset() const override;                // Ground clamping offset from the terrain to our CG (meters)
   virtual bool isDtedTerrainInterpolationEnabled() const override; // Interpolate our terrain from DTED if true

   virtual bool isTerrainElevationRequired() const override;        // True if terrain elevation is required from the OTW system;
                                                                    // otherwise we use DTED, if available

   virtual CoordSys getCoordSystemInUse() const override;           // Returns the coordinate system currently being used to
                                                                    // update the player's position

   // ---
   // Interoperability network data
   // ---
   virtual bool isNetworkedPlayer() const override;                 // True if this is a networked player (IPlayer)
   virtual bool isLocalPlayer() const override;                     // True if this is a local player

   virtual int getNetworkID() const override;                        // ID of a networked player's controlling network model
   virtual simulation::INib* getNib() override;                      // Networked player's Nib object
   virtual const simulation::INib* getNib() const override;          // Networked player's Nib object  (const version)

   virtual bool isNetOutputEnabled() const override;                                     // Is player output to the network enabled?
   virtual simulation::INib* getLocalNib(const unsigned int netId) override;             // Player's outgoing NIB(s)
   virtual const simulation::INib* getLocalNib(const unsigned int netId) const override; // Player's outgoing NIB(s)  (const version)

   // ---
   // Internal autopilot controls
   // ---
   virtual bool isHeadingHoldOn() const override;           // True if heading-hold mode is on?
   virtual double getCommandedHeading() const override;     // Commanded (true) heading, default (radians)
   virtual double getCommandedHeadingD() const override;    // Commanded (true) heading (degrees)
   virtual double getCommandedHeadingR() const override;    // Commanded (true) heading (radians)

   virtual bool isVelocityHoldOn() const override;          // True if velocity-hold mode is on?
   virtual double getCommandedVelocityKts() const override; // Commanded (true) velocity (knots)
   virtual double getCommandedVelocityFps() const override; // Commanded (true) velocity (feet/second)
   virtual double getCommandedVelocityMps() const override; // Commanded (true) velocity (meters/second)

   virtual bool isAltitudeHoldOn() const override;          // True if altitude-hold mode is on?
   virtual double getCommandedAltitude() const override;    // Get commanded (HAE) altitude, default (meters)
   virtual double getCommandedAltitudeM() const override;   // Get commanded (HAE) altitude (meters)
   virtual double getCommandedAltitudeFt() const override;  // Get commanded (HAE) altitude (feet)

   // ---
   // Sync State Changes
   // ---
   virtual const SynchronizedState& getSynchronizedState() const;

   // ---
   // Systems
   // ---

   simulation::Simulation* getSimulation();                           // Controlling simulation model
   const simulation::Simulation* getSimulation() const;               // Controlling simulation model (const version)

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

   virtual bool setType(const base::String* const newTypeString) override; // Sets the player's type string
   virtual void setName(const base::Identifier& newName) override;         // Sets the player's name (base::String version)
   virtual void setName(const char* const newName) override;               // Sets the player's name (char* version)
   virtual void setID(const unsigned short newId) override;                // Sets the player's ID
   virtual void setSide(const Side newSide) override;                      // Sets the player's side enum
   virtual void setMode(const Mode newMode) override;                      // Sets the player's current mode
   virtual void setInitMode(const Mode newMode) override;                  // Sets the player's initial mode (after reset)
   virtual bool setUseCoordSys(const CoordSys) override;                   // Sets the coord system to use for updating position

   virtual bool setFuelFreeze(const bool f) override;                      // Sets the player's fuel freeze flag
   virtual bool setCrashOverride(const bool f) override;                   // Sets the player's crash override flag
   virtual bool setKillOverride(const bool f) override;                    // Sets the player's kill override flag
   virtual bool setKillRemoval(const bool f) override;                     // Sets the player's kill removal flag
   virtual void resetJustKilled() override;                                // Resets the just killed flag
   virtual bool setDamage(const double v) override;                        // Sets the player's damage state
   virtual bool setSmoke(const double v) override;                         // Sets the player's smoke state
   virtual bool setFlames(const double v) override;                        // Sets the player's flames state
   virtual bool setCamouflageType(const unsigned int v) override;          // Sets the user defined camouflage type (or zero for none)
   virtual bool setPositionFreeze(const bool f) override;                  // Sets the player's freeze flag
   virtual bool setAltitudeFreeze(const bool f) override;                  // Sets the player's altitude freeze flag
   virtual bool setAttitudeFreeze(const bool f) override;                  // Sets the player's attitude freeze flag

   virtual bool setHeadingHoldOn(const bool b) override;                   // Turns heading-hold mode on/off
   virtual bool setCommandedHeading(const double h) override;              // Sets the commanded (true) heading default (radians)
   virtual bool setCommandedHeadingD(const double h) override;             // Sets the commanded (true) heading (degrees)
   virtual bool setCommandedHeadingR(const double h) override;             // Sets the commanded (true) heading (radians)

   virtual bool setVelocityHoldOn(const bool b) override;                  // Turns velocity-hold mode on/off
   virtual bool setCommandedVelocityKts(const double a) override;          // Sets the commanded (true) velocity (knots)

   virtual bool setAltitudeHoldOn(const bool b) override;                  // Turns altitude-hold mode on/off
   virtual bool setCommandedAltitude(const double a) override;             // Sets commanded (HAE) altitude, default (meters)
   virtual bool setCommandedAltitudeM(const double a) override;            // Sets commanded (HAE) altitude (meters)
   virtual bool setCommandedAltitudeFt(const double a) override;           // Sets commanded (HAE) altitude (feet)

   virtual bool setNib(simulation::INib* const p) override;                 // Sets the networked player's Nib object

   virtual bool setEnableNetOutput(const bool f) override;                 // Sets the network output enabled flag
   virtual bool setOutgoingNib(simulation::INib* const p, const unsigned int id) override; // Sets the outgoing NIB for network 'id'

   virtual void setTerrainElevation(const double v) override;              // Sets the elevation of the terrain at this player's location (meters)
   virtual bool setTerrainOffset(const double v) override;                 // Sets the ground clamping offset (meters)
   virtual bool setInterpolateTerrain(const bool f) override;              // sets the DTED terrain interpolation flag
   virtual bool setTerrainElevationRequired(const bool b) override;        // Sets OTW terrain elevation request flag

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
   virtual bool setAltitude(const double alt, const bool slaved = false) override;

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const bool slaved = false) override;

   // Position relative(grandfather)to the simulation reference point (meters)
   virtual bool setPosition(const double north, const double east, const double down, const bool slaved = false) override;

   // Position vector; NED from simulation reference point (meters)
   virtual bool setPosition(const osg::Vec3d& newPos, const bool slaved = false) override;

   // Sets lat/long position; (degrees)
   virtual bool setPositionLL(const double lat, const double lon, const bool slaved = false) override;

   // Sets lat/long position; (degrees) and altitude (meters)
   virtual bool setPositionLLA(const double lat, const double lon, const double alt, const bool slaved = false) override;

   // Geocentric (ECEF) position vector (meters)
   virtual bool setGeocPosition(const osg::Vec3d& gcPos, const bool slaved = false) override;

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
   virtual bool setEulerAngles(const double r, const double p, const double y) override;
   virtual bool setEulerAngles(const osg::Vec3d& newAngles) override;

   // Sets geocentric (body/ECEF) Euler angles: (radians) [ roll pitch yaw ]
   virtual bool setGeocEulerAngles(const osg::Vec3d& newAngles) override;

   // Sets the rotational matrix
   virtual bool setRotMat(const osg::Matrixd&) override;

   // Sets the quaternions
   virtual bool setQuaternions(const osg::Quat&) override;

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
   virtual bool setAngularVelocities(const double pa, const double qa, const double ra) override;
   virtual bool setAngularVelocities(const osg::Vec3d& newAngVel) override;

   // Sets the body angular velocities (radians/second)
   virtual bool setGeocAngularVelocities(const osg::Vec3d& newAngVel) override;

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
   virtual bool setVelocity(const double ue, const double ve, const double we) override;
   virtual bool setVelocity(const osg::Vec3d& newVel) override;

   // Sets local tangent plane accelerations (NED);
   virtual bool setAcceleration(const double due, const double dve, const double dwe) override;
   virtual bool setAcceleration(const osg::Vec3d& newAccel) override;

   // Sets body velocities: (meters/second) [ ua -> fwd(+), va -> right(+), wa -> down(+) ]
   virtual bool setVelocityBody(const double ua, const double va, const double wa) override;
   virtual bool setVelocityBody(const osg::Vec3d& newVelBody) override;

   // Sets body accelerations
   virtual bool setAccelerationBody(const double dua, const double dva, const double dwa) override;
   virtual bool setAccelerationBody(const osg::Vec3d& newAccelBody) override;

   // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   virtual bool setGeocVelocity(const double vx, const double vy, const double vz) override;
   virtual bool setGeocVelocity(const osg::Vec3d& newVelEcef) override;

   // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   virtual bool setGeocAcceleration(const double dvx, const double dvy, const double dvz) override;
   virtual bool setGeocAcceleration(const osg::Vec3d& newAccelEcef) override;

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
   virtual void processDetonation(const double detRange, Weapon* const wpn = nullptr);

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
   simulation::Simulation* getSimulationImp();

   // ---
   // Player identity
   // ---
   base::safe_ptr<base::String> type;    // Type of vehicle
   unsigned short id;                    // Player ID
   base::Identifier  pname;              // Player's Name
   Side side;                            // Which side (see above)

   // ---
   // Player State
   // ---
   Mode        mode;             // Player mode (see above)
   CoordSys    useCoordSys;      // Coordinate system being used to update position
   CoordSys    useCoordSysN1;    // Previous 'useCoordSys'

   double      latitude;         // Latitude                          (degrees)
   double      longitude;        // Longitude                         (degrees)
   double      altitude;         // Altitude                          (meters) (HAE)
   osg::Vec3d  posVecNED;        // Local gaming area position vector (meters) [ x, y, z ] NED
   osg::Vec3d  posVecECEF;       // Geocentric position vector        (meters)  (ECEF)

   osg::Vec3d  velVecNED;        // Inertial axes velocity vector     (meters/second)  [ ue, ve, we ] NED
   osg::Vec3d  velVecECEF;       // Geocentric velocity vector        (meters/second)  (ECEF)
   osg::Vec3d  velVecBody;       // Body axes velocity vector         (meters/second)  [ ua  va  wa ]
   osg::Vec3d  velVecN1;         // Previous velocity vector          (meters/second)  (based on 'useCoordSys')

   osg::Vec3d  accelVecNED;      // Inertial axes acceleration vector ((meters/second)/second) [ due, dve, dwe ] NED
   osg::Vec3d  accelVecECEF;     // Geocentric acceleration vector    ((meters/second)/second) (ECEF)
   osg::Vec3d  accelVecBody;     // Body axes acceleration vector     ((meters/second)/second) [ dua, dva, dwa ]

   double      vp;               // Total Velocity                    (meters/second)
   double      gndSpd;           // Ground Speed                      (meters/second)
   double      gndTrk;           // Ground Track                      (radians)

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

   double      tElev;            // Terrain Elevation  (meters -- up+)
   bool        tElevValid;       // Terrain elevation is valid
   bool        tElevReq;         // Height-Of-Terrain is required from the OTW system
   bool        interpTrrn;       // interpolate between terrain elevation posts (local terrain database only)
   double      tOffset;          // Offset from the terrain to the player's CG for ground clamping
   bool        posVecValid;      // Local position vector valid
   bool        altSlaved;        // Player's altitude is slaved to the dynamics software (default: false)
   bool        posSlaved;        // Player's position is slaved to the dynamics software (default: false)
   bool        posFrz;           // Player's position is frozen
   bool        altFrz;           // Player's altitude is frozen
   bool        attFrz;           // Player's attitude is frozen
   bool        fuelFrz;          // Player's fuel quantity is frozen
   bool        crashOverride;    // If true, player can NOT crash
   bool        killOverride;     // If true, player can NOT be killed
   bool        killRemoval;      // If true then on kill notification the player will be set to KILLED and eventually removed

   // ---
   // Appearance
   // ---
   base::safe_ptr<RfSignature> signature;   // Player's RCS signature
   base::safe_ptr<IrSignature> irSignature; // Player's IR signature
   unsigned int camouflage;                  // Camouflage type (0 is none)
   double      damage;                       // Damage state from no damage(0.0) to destroyed (1.0)
   double      smoking;                      // Smoke state from no smoke (0.0) to maximum (1.0)
   double      flames;                       // Flames state from no flames (0.0) to maximum (1.0)
   bool        justKilled;                   // Just killed flag
   unsigned short killedBy;                  // Killed by player ID

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
   double      initVp;           // Initial Total Velocity
   Mode        initMode;         // Initial mode
   osg::Vec3d  initAngles;       // Initial (Euler) angles (radians)
   osg::Vec3d  testAngRates;     // Test angular rates (radians/sec)
   bool        testBodyAxis;     // Test angular rates are in the body axis else they're Euler rates

   // ---
   // Data collection timer
   // ---
   double      dataLogTimer;     // Data log timer (seconds)
   double      dataLogTime;      // Data log time (seconds)

   // ---
   // Incoming IPlayer network support
   // ---
   simulation::INib* nib;        // Network Interface Block (ref()'d)
   int netID;                    // Network id

   // ---
   // Outgoing network support data
   // ---
   bool enableNetOutput;         // Allow output to the network
   simulation::INib** nibList;   // Pointer to a list of outgoing NIBs

   // ---
   // System pointers
   // ---
   simulation::Simulation* sim;   // The simulation system that owns us (not ref()'d)

   base::Pair* dynamicsModel;   // Dynamics Model (ref()'d)
   base::Pair* datalink;        // Datalink model (ref()'d)
   base::Pair* gimbal;          // Gimbal/antenna/optic models (ref()'d)
   base::Pair* nav;             // Navigation (ref()'d)
   base::Pair* obc;             // Onboard Computer (ref()'d)
   base::Pair* pilot;           // Pilot Model (e.g., autopilot, PDL) (ref()'d)
   base::Pair* radio;           // Radio models (ref()'d)
   base::Pair* sensor;          // R/F Sensor models (ref()'d)
   base::Pair* irSystem;        // IR Sensor models (ref()'d)
   base::Pair* sms;             // Stores Management System (ref()'d)
   bool loadSysPtrs;             // Load system pointers flag

   // ---
   // Reflected emissions
   // ---
   static const unsigned int MAX_RF_REFLECTIONS = 4;   // Max number of reflected emissions we'll send (let's keep it small)
   base::Component* rfReflect[MAX_RF_REFLECTIONS];    // Objects that are interested in the emissions hitting us
   double  rfReflectTimer[MAX_RF_REFLECTIONS];         // Request for reflected emissions will timeout

   // ---
   // sync state changes
   // ---
   bool                 syncState1Ready;
   bool                 syncState2Ready;
   SynchronizedState    syncState1;
   SynchronizedState    syncState2;
};

#include "openeaagles/models/players/Player.inl"

}
}

#endif
