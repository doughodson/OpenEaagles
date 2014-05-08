//------------------------------------------------------------------------------
// Class: Player
// inline functions
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Player_Inline__
#define __Eaagles_Simulation_Player_Inline__

// The player's type string (.e.g, "F-16C")
inline const Basic::String* Player::getType() const
{
   return type;
}

// True if the player's name matches
inline bool Player::isName(const Basic::Identifier* const tst) const
{
   return (*tst == pname);
}

// True if the player's name matches
inline bool Player::isName(const char* const tst) const
{
   return (tst == pname);
}

// The player's name
inline const Basic::Identifier* Player::getName() const
{
   return &pname;
}

// True if player's ID matches
inline bool Player::isID(const unsigned short tst) const
{
   return (tst == id);
}

// The player's ID
inline unsigned short Player::getID() const
{
   return id;
}

// The 'side' that the player is on.
inline Player::Side Player::getSide() const
{
   return side;
}

// True if player is with one of these (bit-wise or'd) sides
inline bool Player::isSide(const unsigned int tst) const
{
   return ((tst & side) != 0);
}

// True if player is not with one one of these (bit-wise or'd) sides
inline bool Player::isNotSide(const unsigned int tst) const
{
   return ((tst & side) == 0);
}

// True if player is of these (bit-wise or'd) major types
inline bool Player::isMajorType(const unsigned int tst) const
{
   return ((tst & getMajorType()) != 0);
}

// Current mode ( INACTIVE, ACTIVE, DETONATED ... )
inline Player::Mode Player::getMode() const
{
   return mode;
}

// True if player's mode is active
inline bool Player::isActive() const
{
   return mode == ACTIVE;
}

// True if player's mode is killed
inline bool Player::isKilled() const
{
   return (mode == KILLED);
}

// True if player's mode is crashed
inline bool Player::isCrashed() const
{
   return (mode == CRASHED);
}

// True if player has detonated (weapons only)
inline bool Player::isDetonated() const
{
   return (mode == DETONATED);
}

// True if player's mode is inactive
inline bool Player::isInactive() const
{
   return mode == INACTIVE;
}

// True if player is currently this mode
inline bool Player::isMode(const Mode tst) const
{
   return mode == tst;
}

// True if player is not currently this mode
inline bool Player::isNotMode(const Mode tst) const
{
   return mode != tst;
}

// True if player's mode is dead
inline bool Player::isDead() const
{
   return isKilled() || isCrashed() || isDetonated();
}

// Roll Euler angle (Rad)
inline double Player::getRoll() const
{
   return angles[IROLL];
}

// Roll Euler angle (Rad)
inline double Player::getRollR() const
{
   return angles[IROLL];
}

// Roll Euler angle (degs)
inline double Player::getRollD() const
{
   return (Basic::Angle::R2DCC * angles[IROLL]);
}

// Sin of the Euler roll angle
inline double Player::getSinRoll() const
{
   return scPhi[0];
}

// Cos of the  Euler roll angle
inline double Player::getCosRoll() const
{
   return scPhi[1];
}

// Pitch Euler angle (Rad)
inline double Player::getPitch() const
{
   return angles[IPITCH];
}

// Pitch Euler angle (Rad)
inline double Player::getPitchR() const
{
   return angles[IPITCH];
}

// Pitch Euler angle (degs)
inline double Player::getPitchD() const
{
   return (Basic::Angle::R2DCC * angles[IPITCH]);
}

// Sin of the pitch Euler angle
inline double Player::getSinPitch() const
{
   return scTheta[0];
}

// Cos of the  pitch Euler angle
inline double Player::getCosPitch() const
{
   return scTheta[1];
}

// Yaw Euler angle (Rad)
inline double Player::getHeading() const
{
   return angles[IYAW];
}

// Yaw Euler angle (Rad)
inline double Player::getHeadingR() const
{
   return angles[IYAW];
}

// Yaw Euler angle (degs)
inline double Player::getHeadingD() const
{
   return (Basic::Angle::R2DCC * angles[IYAW]);
}

// Sin of the yaw Euler angle
inline double Player::getSinHeading() const
{
   return scPsi[0];
}

// Cos of the  yaw Euler angle
inline double Player::getCosHeading() const
{
   return scPsi[1];
}

// Euler angles (rad)
inline const osg::Vec3d& Player::getEulerAngles() const
{
   return angles;
}

// Geocentric Euler angles (rad)
inline const osg::Vec3d& Player::getGeocEulerAngles() const
{
   return anglesW;
}

// Rotational Quaternions
inline const osg::Quat& Player::getQuaternions() const
{
   return q;
}

// Rotational Matrix
inline const osg::Matrixd& Player::getRotMat() const
{
   return rm;
}

// Rotational Matrix: world to body
inline const osg::Matrixd& Player::getRotMatW2B() const
{
   return rmW2B;
}

// Body angular rates (radians/second)
inline const osg::Vec3d& Player::getAngularVelocities() const
{
   return angularVel;
}

// Geocentric angular rates (radians/second)
inline const osg::Vec3d& Player::getGeocAngularVelocities() const
{
   return gcAngVel;
}

// World transformation matrix:
inline const osg::Matrixd& Player::getWorldMat() const
{
   return wm;
}

// Is the position vector valid
inline bool Player::isPositionVectorValid() const
{
   return posVecValid;
}

// Position vector; NED from sim ref pt (m)
inline const osg::Vec3d& Player::getPosition() const
{
   return posVecNED;
}

// North(+) or south(-) of the sim ref pt (m) 
inline double Player::getXPosition() const
{
   return posVecNED[INORTH];
}

// East(+) or west(-) of the sim ref pt (m) 
inline double Player::getYPosition() const
{
   return posVecNED[IEAST];
}

// Player's latitude (degs)
inline double Player::getLatitude() const
{
   return latitude;
}

// Player's longitude (degs)
inline double Player::getLongitude() const
{
   return longitude;
}

// Player's Lat/lon (degs)
inline bool Player::getPositionLL(double* const lat, double* const lon) const
{
   bool ok = false;
   if (lat != 0 && lon != 0) {
      *lat = latitude;
      *lon = longitude;
      ok = true;
   }
   return ok;
}

// Players Lat/Lon (degs) and altitude (meters)
inline bool Player::getPositionLLA(double* const lat, double* const lon, double* const alt) const
{
   bool ok = false;
   if (lat != 0 && lon != 0) {
      *lat = latitude;
      *lon = longitude;
      *alt = getAltitudeM();
      ok = true;
   }
   return ok;
}

// Geocentric position vector [ x y z ] (meters)
inline const osg::Vec3d& Player::getGeocPosition() const
{
   return posVecECEF;
}

// Altitude HAE (meters)
inline double Player::getAltitude() const
{
   return getAltitudeM();
}

// Altitude HAE (meters)
inline double Player::getAltitudeM() const
{
   return altitude;
}

// Altitude HAE (ft)
inline double Player::getAltitudeFt() const
{
   return getAltitude() * Basic::Distance::M2FT;
}

// Altitude AGL (meters) 
inline double Player::getAltitudeAgl() const
{
   return altitude - static_cast<double>(tElev);
}

// Altitude AGL (meters)
inline double Player::getAltitudeAglM() const
{
   return getAltitudeAgl();
}

// Altitude AGL (ft)
inline double Player::getAltitudeAglFt() const
{
   return getAltitudeAgl() * Basic::Distance::M2FT;
}

// Player's total velocity (m/s)
inline LCreal Player::getTotalVelocity() const
{
   return vp;
}

// Player's total velocity (ft/s)
inline LCreal Player::getTotalVelocityFPS() const
{
   return getTotalVelocity() * Basic::Distance::M2FT;
}

// Player's total velocity (kts)
inline LCreal Player::getTotalVelocityKts() const
{
   return getTotalVelocity() * Basic::Distance::M2NM * 3600.0f;
}

// Player's ground speed (m/s)
inline LCreal Player::getGroundSpeed() const
{
   return gndSpd;
}

// Player's ground speed (ft/s)
inline LCreal Player::getGroundSpeedFPS() const
{
   return getGroundSpeed() * Basic::Distance::M2FT;
}

// Player's ground speed (kts)
inline LCreal Player::getGroundSpeedKts() const 
{
   return getGroundSpeed() * Basic::Distance::M2NM * 3600.0f;
}

// Player's ground track (rad)
inline LCreal Player::getGroundTrack() const
{
   return gndTrk;
}

// Player's ground track (rad)
inline LCreal Player::getGroundTrackR() const
{
   return gndTrk;
}

// Player's ground track (degs)
inline LCreal Player::getGroundTrackD() const
{
   return static_cast<LCreal>(Basic::Angle::R2DCC * gndTrk);
}

// Velocity vector; NED (m/s)
inline const osg::Vec3d& Player::getVelocity() const
{
   return velVecNED;
}

// Acceleration vector; NED ((m/s)/s)
inline const osg::Vec3d& Player::getAcceleration() const
{
   return accelVecNED;
}

// Body velocity vector: Nose/right/down (m/s)
inline const osg::Vec3d& Player::getVelocityBody() const
{
   return velVecBody;
}

// Body Acceleration vector: Nose/right/down ((meters/second)/second)
inline const osg::Vec3d& Player::getAccelerationBody() const
{
   return accelVecBody;
}

// Geocentric velocity vector [ x y z ] (m/s)
inline const osg::Vec3d& Player::getGeocVelocity() const
{
   return velVecECEF;
}

// Geocentric acceleration vector [ x y z ] ((m/s)/s)
inline const osg::Vec3d& Player::getGeocAcceleration() const
{
   return accelVecECEF;
}

// Initial geocentric position vector [ x y z ] (meters)
inline const osg::Vec3d& Player::getInitGeocentricPosition() const
{
   return initGeoPosVec;
}

// Initial position vector is valid
inline bool Player::isInitGeocentricPositionValid() const
{
   return initGeoPosFlg;
}

// Player's initial latitude (degs)
inline double Player::getInitLatitude() const
{
   return initLat;
}

// Player's initial longitude (degs)
inline double Player::getInitLongitude() const
{
   return initLon;
}

// Initial lat/lon valid
inline bool Player::isInitLatLonValid() const
{
   return initLatLonFlg;
}

// Initial position vector; north/east from sim ref pt (m)
inline const osg::Vec2d& Player::getInitPosition() const
{
   return initPosVec;
}

// Initial position vector is valid
inline bool Player::isInitPositionValid() const
{
   return initPosFlg;
}

// Initial altitude HAE (meters)
inline double Player::getInitAltitude() const
{
   return initAlt;
}

// Initial total velocity
inline LCreal Player::getInitVelocity() const
{
   return initVp;
}

// Initial Euler angles (radians)
inline const osg::Vec3d& Player::getInitAngles() const
{
   return initAngles;
}

// Returns the user defined camouflage type (or zero for none)
inline unsigned int Player::getCamouflageType() const
{
   return camouflage;
}

// Returns the player's damage state
inline LCreal Player::getDamage() const
{
   return damage;
}

// Returns the player's smoke state
inline LCreal Player::getSmoke() const
{
   return smoking;
}

// Returns the player's flames state
inline LCreal Player::getFlames() const
{
   return flames;
}

// True if the player was just killed
inline bool Player::isJustKilled() const
{
   return justKilled;
}

// ID of the attacking player that killed us
inline int Player::killedByPlayerID() const
{
   return killedBy;
}

// True if the player is frozen (paused)
inline bool Player::isPositionFrozen() const
{
   return posFrz;
}

// True if the player's altitude is frozen
inline bool Player::isAltitudeFrozen() const
{
   return altFrz;
}

// True if the player's attitude is frozen
inline bool Player::isAttitudeFrozen() const
{
   return attFrz;
}

// True if the player's fuel is frozen
inline bool Player::isFuelFrozen() const
{
   return fuelFrz;
}

// True if the player is in crash override mode
inline bool Player::isCrashOverride() const
{
   return crashOverride;
}

// True if the player is in kill override mode
inline bool Player::isKillOverride() const
{
   return killOverride;
}

// True fi the player's kill removal flag is set
inline bool Player::isKillRemovalEnabled() const
{
   return killRemoval;
}

// True if altitude is slaved to an external model
inline bool Player::isAltitudeSlaved() const
{
   return altSlaved;
}

// True if position is slaved to an external model
inline bool Player::isPositionSlaved() const
{
   return posSlaved;
}

// Terrain elevation is valid (or at least was where it was set)
inline bool Player::isTerrainElevationValid() const
{
   return tElevValid;
}

// Terrain elevation at player (meters)
inline LCreal Player::getTerrainElevation() const
{ 
   return tElev;
}

// Terrain elevation at player (meters)
inline LCreal Player::getTerrainElevationM() const
{ 
   return getTerrainElevation();
}

// Returns terrain elevation (Feet)
inline LCreal Player::getTerrainElevationFt() const
{ 
   return getTerrainElevation() * Basic::Distance::M2FT;
}

// Ground clamping offset from the terrain to our CG (meters)
inline LCreal Player::getTerrainOffset() const
{
   return tOffset;
}

// interpolate our terrain from DTED if true
inline bool Player::isDtedTerrainInterpolationEnabled() const
{ 
   return interpTrrn;
}

// True if terrain elevation is required from the OTW system; otherwise we use DTED, if available
inline bool Player::isTerrainElevationRequired() const
{
   return tElevReq;
}

// True if this is a networked player (IPlayer)
inline bool Player::isNetworkedPlayer() const
{
   return (nib != 0);
}

// True if this is a local player
inline bool Player::isLocalPlayer() const
{
   return (nib == 0);
}

// ID of a networked player's controlling network model
inline int Player::getNetworkID() const
{
   return netID;
}

// Networked player's Nib object
inline Nib* Player::getNib()
{
   return nib;
}

// Networked player's Nib object  (const version)
inline const Nib* Player::getNib() const
{
   return nib;
}

// is player output to the network enabled?
inline bool Player::isNetOutputEnabled() const
{
   return enableNetOutput;
}

// Returns the coordinate system currently being to update the player's position
inline Player::CoordSys Player::getCoordSystemInUse() const
{
   return useCoordSys;
}

// Get the current synchronized state
inline const SynchronizedState& Player::getSynchronizedState() const
{
   if (syncState1Ready)
      return syncState1;
   else
      return syncState2;
}

#endif
