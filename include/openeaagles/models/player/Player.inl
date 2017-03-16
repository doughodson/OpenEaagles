//------------------------------------------------------------------------------
// Class: Player
// Description: inline functions
//------------------------------------------------------------------------------
#ifndef __oe_models_Player_Inline__
#define __oe_models_Player_Inline__

// The player's type string (.e.g, "F-16C")
inline const base::String* Player::getType() const
{
   return type;
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
   return (base::angle::R2DCC * angles[IROLL]);
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
   return (base::angle::R2DCC * angles[IPITCH]);
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
   return (base::angle::R2DCC * angles[IYAW]);
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
inline const base::Vec3d& Player::getEulerAngles() const
{
   return angles;
}

// Geocentric Euler angles (rad)
inline const base::Vec3d& Player::getGeocEulerAngles() const
{
   return anglesW;
}

// Rotational Quaternions
inline const base::Quat& Player::getQuaternions() const
{
   return q;
}

// Rotational Matrix
inline const base::Matrixd& Player::getRotMat() const
{
   return rm;
}

// Rotational Matrix: world to body
inline const base::Matrixd& Player::getRotMatW2B() const
{
   return rmW2B;
}

// Body angular rates (radians/second)
inline const base::Vec3d& Player::getAngularVelocities() const
{
   return angularVel;
}

// Geocentric angular rates (radians/second)
inline const base::Vec3d& Player::getGeocAngularVelocities() const
{
   return gcAngVel;
}

// World transformation matrix:
inline const base::Matrixd& Player::getWorldMat() const
{
   return wm;
}

// Is the position vector valid
inline bool Player::isPositionVectorValid() const
{
   return posVecValid;
}

// Position vector; NED from sim ref pt (m)
inline const base::Vec3d& Player::getPosition() const
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
   if (lat != nullptr && lon != nullptr) {
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
   if (lat != nullptr && lon != nullptr) {
      *lat = latitude;
      *lon = longitude;
      *alt = getAltitudeM();
      ok = true;
   }
   return ok;
}

// Geocentric position vector [ x y z ] (meters)
inline const base::Vec3d& Player::getGeocPosition() const
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
   return getAltitude() * base::distance::M2FT;
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
   return getAltitudeAgl() * base::distance::M2FT;
}

// Player's total velocity (m/s)
inline double Player::getTotalVelocity() const
{
   return vp;
}

// Player's total velocity (ft/s)
inline double Player::getTotalVelocityFPS() const
{
   return getTotalVelocity() * base::distance::M2FT;
}

// Player's total velocity (kts)
inline double Player::getTotalVelocityKts() const
{
   return getTotalVelocity() * base::distance::M2NM * 3600.0f;
}

// Player's ground speed (m/s)
inline double Player::getGroundSpeed() const
{
   return gndSpd;
}

// Player's ground speed (ft/s)
inline double Player::getGroundSpeedFPS() const
{
   return getGroundSpeed() * base::distance::M2FT;
}

// Player's ground speed (kts)
inline double Player::getGroundSpeedKts() const 
{
   return getGroundSpeed() * base::distance::M2NM * 3600.0f;
}

// Player's ground track (rad)
inline double Player::getGroundTrack() const
{
   return gndTrk;
}

// Player's ground track (rad)
inline double Player::getGroundTrackR() const
{
   return gndTrk;
}

// Player's ground track (degs)
inline double Player::getGroundTrackD() const
{
   return static_cast<double>(base::angle::R2DCC * gndTrk);
}

// Velocity vector; NED (m/s)
inline const base::Vec3d& Player::getVelocity() const
{
   return velVecNED;
}

// Acceleration vector; NED ((m/s)/s)
inline const base::Vec3d& Player::getAcceleration() const
{
   return accelVecNED;
}

// Body velocity vector: Nose/right/down (m/s)
inline const base::Vec3d& Player::getVelocityBody() const
{
   return velVecBody;
}

// Body Acceleration vector: Nose/right/down ((meters/second)/second)
inline const base::Vec3d& Player::getAccelerationBody() const
{
   return accelVecBody;
}

// Geocentric velocity vector [ x y z ] (m/s)
inline const base::Vec3d& Player::getGeocVelocity() const
{
   return velVecECEF;
}

// Geocentric acceleration vector [ x y z ] ((m/s)/s)
inline const base::Vec3d& Player::getGeocAcceleration() const
{
   return accelVecECEF;
}

// Initial geocentric position vector [ x y z ] (meters)
inline const base::Vec3d& Player::getInitGeocentricPosition() const
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
inline const base::Vec2d& Player::getInitPosition() const
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
inline double Player::getInitVelocity() const
{
   return initVp;
}

// Initial Euler angles (radians)
inline const base::Vec3d& Player::getInitAngles() const
{
   return initAngles;
}

// Returns the user defined camouflage type (or zero for none)
inline unsigned int Player::getCamouflageType() const
{
   return camouflage;
}

// Returns the player's damage state
inline double Player::getDamage() const
{
   return damage;
}

// Returns the player's smoke state
inline double Player::getSmoke() const
{
   return smoking;
}

// Returns the player's flames state
inline double Player::getFlames() const
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
inline double Player::getTerrainElevation() const
{ 
   return tElev;
}

// Terrain elevation at player (meters)
inline double Player::getTerrainElevationM() const
{ 
   return getTerrainElevation();
}

// Returns terrain elevation (Feet)
inline double Player::getTerrainElevationFt() const
{ 
   return getTerrainElevation() * base::distance::M2FT;
}

// Ground clamping offset from the terrain to our CG (meters)
inline double Player::getTerrainOffset() const
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
