//------------------------------------------------------------------------------
// Classes: Track, RfTrack, IrTrack
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Track_H__
#define __Eaagles_Simulation_Track_H__

#include "openeaagles/basic/Object.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

class Emission;
class IrQueryMsg;
class Player;

//------------------------------------------------------------------------------
// Class: Track
// Base class: Basic::Object -> Track
// Factory name: Track
// Description: Generic Target Track
//------------------------------------------------------------------------------
class Track : public Basic::Object
{
    DECLARE_SUBCLASS(Track,Basic::Object)

public:
    enum TypeBits {
       AIR_TRACK_BIT       = 0x0001,      // Air Track
       GND_TRACK_BIT       = 0x0002,      // Ground Track
       RWR_TRACK_BIT       = 0x0004,      // Radar Warning Receiver Track
       ONBOARD_SENSOR_BIT  = 0x0100,      // Track from an on-board sensor
       DATALINK_BIT        = 0x0200       // Off-board (datalink) track
    };
    enum IffCode { UNKNOWN, FRIENDLY, FOE, COMMERCIAL, OTHER };

    enum TrackClass { NOT_DEFINED, RANGE_AND_ANGLE, ANGLE_ONLY };

public:
   Track();

   // Local track ID
   int getTrackID() const                          { return id; }
   virtual bool setTrackID(const int v);

   // Track type (see TypeBits above)
   bool isType(const short t) const                { return ((type & t) != 0); }
   short getType() const                           { return type; }
   virtual bool setType(const short t);
   virtual bool setSubtype(const short t);

   // Track class (see TypeClass above)
   bool isClass(const TrackClass c) const          { return ((type & c) != 0); }
   TrackClass getClass() const                     { return trackClass; }
   virtual bool setClass(const TrackClass t);

   bool isAngleOnly() const                        { return (trackClass == ANGLE_ONLY); }
   bool isRangeAndAngle() const                    { return (trackClass == RANGE_AND_ANGLE); }

   // Age of the track (seconds) since last update
   LCreal getTrackAge() const                      { return age; }
   virtual bool updateTrackAge(const LCreal dt);
   virtual bool resetTrackAge();

   // Quality of the Track (normalized: high(1.0) to low(0.0)
   LCreal getQuality() const                       { return quality; }
   virtual bool setQuality(const LCreal v);

   // Track position is in meters, NED and centered at ownship.
   const osg::Vec3& getPosition() const            { return pos; }
   virtual bool setPosition(const osg::Vec3& p);
   virtual bool setPosition(const osg::Vec4& p);

   // Track's estimated position error, meters
   LCreal getCircularError() const                 { return cErr; }
   virtual bool setCircularError(const LCreal err);

   // Track's estimated vertical (altitude) error, meters
   LCreal getVerticalError() const                 { return vErr; }
   virtual bool setVerticalError(const LCreal err);

   // Tracks position (lat/lon)
   bool isLatLonPositionValid() const              { return llValid; }
   bool getLatLonPosition(double* const lat, double* const lon) const;
   virtual bool setLatLonPosition(const double lat, const double lon);
   virtual void clearLatLonValid();

   // Line-Of-Sight (LOS) vector; ownship to track
   const osg::Vec3& getLosVec() const              { return los; }

   // Range to track (meters)
   LCreal getRange() const                         { return rng; }
   LCreal getGroundRange() const                   { return gndRng; }
   virtual bool setRange(const LCreal r);

   // Range rate: ownship-track closer rate, meters/sec, with positive
   // rate for increasing range to track.
   LCreal getRangeRate() const                     { return rngRate; }
   virtual bool setRangeRate(const LCreal rr);

   // Azimuth angle, ownship->track, true north
   LCreal getTrueAzimuth() const                   { return taz; }
   LCreal getTrueAzimuthR() const                  { return taz; }
   LCreal getTrueAzimuthD() const                  { return static_cast<LCreal>(Basic::Angle::R2DCC * taz); }

   // Azimuth angle, ownship->track, relative to ownship
   LCreal getRelAzimuth() const                    { return raz[0]; }
   LCreal getRelAzimuthR() const                   { return raz[0]; }
   LCreal getRelAzimuthD() const                   { return static_cast<LCreal>(Basic::Angle::R2DCC * raz[0]); }
   void setRelAzimuth(LCreal az)                   { raz[0] = az; }

   // Azimuth angle rate, ownship->track, relative to ownship
   LCreal getRelAzimuthRate() const                { return raz[1]; }
   void setRelAzimuthRate(LCreal azRate)           { raz[1] = azRate; }

   // Azimuth angle acceleration, ownship->track, relative to ownship
   LCreal getRelAzimuthAcceleration() const        { return raz[2]; }
   void setRelAzimuthAcceleration(LCreal azAccel)  { raz[2] = azAccel; }

   // Elevation angle, ownship->track, relative to ownship
   LCreal getElevation() const                     { return rel[0]; }
   LCreal getElevationR() const                    { return rel[0]; }
   LCreal getElevationD() const                    { return static_cast<LCreal>(Basic::Angle::R2DCC * rel[0]); }

   void setElevation(LCreal el)                    {rel[0] = el; }

   // Elevation angle rate, ownship->track, relative to ownship
   LCreal getElevationRate() const                 { return rel[1]; }
   void setElevationRate(LCreal elRate)            { rel[1] = elRate; }

   // Elevation angle acceleration, ownship->track, relative to ownship
   LCreal getElevationAcceleration() const         { return rel[2]; }
   void setElevationAcceleration(LCreal elAccel)   { rel[2] = elAccel; }

   // Azimuth angle, ownship->track, relative to ownship
   LCreal getPredictedAzimuth() const              { return predictedRaz[0]; }
   LCreal getPredictedAzimuthR() const             { return predictedRaz[0]; }
   LCreal getPredictedAzimuthD() const             { return static_cast<LCreal>(Basic::Angle::R2DCC * predictedRaz[0]); }

   void setPredictedAzimuth(LCreal az)             { predictedRaz[0] = az; }

   // Azimuth angle rate, ownship->track, relative to ownship
   LCreal getPredictedAzimuthRate() const          { return predictedRaz[1]; }
   void setPredictedAzimuthRate(LCreal azRate)     { predictedRaz[1] = azRate; }

   // Elevation angle, ownship->track, relative to ownship
   LCreal getPredictedElevation() const            { return predictedRel[0]; }
   LCreal getPredictedElevationR() const           { return predictedRel[0]; }
   LCreal getPredictedElevationD() const           { return static_cast<LCreal>(Basic::Angle::R2DCC * predictedRel[0]); }

   void setPredictedElevation(LCreal el)           {predictedRel[0] = el; }

   // Elevation angle rate, ownship->track, relative to ownship
   LCreal getPredictedElevationRate() const                 { return predictedRel[1]; }
   void setPredictedElevationRate(LCreal elRate)            { predictedRel[1] = elRate; }

   // Track's estimated ground speed (m/s)
   LCreal getGroundSpeed() const                   { return gndSpd; }
   LCreal getGroundSpeedFPS() const                { return getGroundSpeed() * static_cast<LCreal>(Basic::Distance::M2FT); }
   LCreal getGroundSpeedKts() const                { return getGroundSpeed() * static_cast<LCreal>(Basic::Distance::M2NM * 3600.0f); }

   // Ground track angle, true north
   LCreal getGroundTrack() const                   { return gndTrk; }
   LCreal getGroundTrackR() const                  { return gndTrk; }
   LCreal getGroundTrackD() const                  { return static_cast<LCreal>(Basic::Angle::R2DCC * gndTrk); }

   // Ground track angle, relative to ownship
   LCreal getRelGroundTrack() const                { return relGndTrk; }
   LCreal getRelGroundTrackR() const               { return relGndTrk; }
   LCreal getRelGroundTrackD() const               { return static_cast<LCreal>(Basic::Angle::R2DCC * relGndTrk); }

   // Track->ownship aspect angle
   LCreal getAspectAngle() const                   { return aa; }
   LCreal getAspectAngleR() const                  { return aa; }
   LCreal getAspectAngleD() const                  { return static_cast<LCreal>(Basic::Angle::R2DCC * aa); }

   // Track velocity vector
   const osg::Vec3& getVelocity() const            { return vel; }
   virtual bool setVelocity(const osg::Vec3 v);

   // Track acceleration vector
   const osg::Vec3& getAcceleration() const        { return accel; }
   virtual bool setAcceleration(const osg::Vec3 a);

   // IFF information
   bool isIffCode(const IffCode s) const           { return (s == iffCode); }
   bool isNotIffCode(const IffCode s) const        { return (s != iffCode); }
   IffCode getIffCode() const                      { return iffCode; }
   virtual bool setIffCode(const IffCode s);

   // Shootlist support
   int getShootListIndex() const                   { return shootList; }
   virtual bool setShootListIndex(const int idx);

   bool isWeaponReleased() const                   { return wpnRel; }
   virtual bool setWeaponReleased(const bool f);

   bool isRejected() const                         { return rejected; }
   virtual bool setRejected(const bool f);

   // OwnshipDynmaics() will apply ownship movements to the track's
   // predicted position, range and velocity.
   virtual void ownshipDynamics(const LCreal gtrk, const osg::Vec3 vel, const osg::Vec3 accel, const LCreal dt);

   // Optional pointer to the track target player
   Player* getTarget()                             { return tgt; }
   const Player* getTarget() const                 { return tgt; }
   virtual bool setTarget(Player* const p);

   // Clear track
   virtual void clear();

protected:
   // General track ID and status
   int         id;             // Track id;
   short       type;           // Track type: the bit-wise OR of various type bits (see enum above)
   IffCode     iffCode;        // Friend or Foe (see above)
   TrackClass  trackClass;     // Track class (see above) [RF or IR]
   LCreal      age;            // Track age                               (sec)
   LCreal      quality;        // Quality of Track; normalized: high(1.0) to low(0.0)

   // Track State Vector
   double      latitude;       // Latitude                                (degs)
   double      longitude;      // Longitude                               (degs)
   osg::Vec3   los;            // Line-of-sight unit vector (NED)
   osg::Vec3   pos;            // position; relative to ownship (NED)     (m)
   osg::Vec3   vel;            // Velocity; relative to ownship (NED)     (m/s)
   osg::Vec3   accel;          // Acceleration; relative to ownship (NED) ((m/s)/s)
   LCreal      rng;            // Range from ownship                      (m)
   LCreal      rngRate;        // Closure rate                            (m/s)
   bool        llValid;        // Latitude & Longitude are valid
   LCreal      cErr;           // Circular position error                 (m)
   LCreal      vErr;           // Vertical (altitude) error               (m)

   // Data computed from state vector
   LCreal      raz[3];           // Relative azimuth angle (ownship 2 track)  (r)
   LCreal      rel[3];           // Relative elevation angle (ownship 2 trk)  (r)
   LCreal      predictedRaz[3];  // Predicted Relative azimuth angle (ownship 2 track)  (r)
   LCreal      predictedRel[3];  // Predicted Relative elevation angle (ownship 2 trk)  (r)
   LCreal      gndRng;         // Ground Range                              (m)
   LCreal      taz;            // True azimuth angle (ownship to track)     (r)
   LCreal      aa;             // Aspect angle                              (r)
   LCreal      gndSpd;         // Ground speed                              (m/s)
   LCreal      gndTrk;         // Ground track angle                        (r)
   LCreal      relGndTrk;      // Ground track angle relative to ownship    (r)

   // Shoot list support
   int         shootList;      // Shoot list index
   bool        wpnRel;         // Released weapon on this track
   bool        rejected;       // Target Rejected (from shoot list)

   // Ownship data
   LCreal      osGndTrk;        // Ownship's ground track angle             (r)
   osg::Vec3   osVel;           // Ownship velocity (NED)                   (m/s)
   osg::Vec3   osAccel;         // Ownship accel (NED)                      ((m/s)/s)

   Player*     tgt;             // (Optional) pointer to the Track Player

   bool        mslWarn;         // Missile warning flag
   static const int MAX_SIG = 4;
   LCreal      lastSN[MAX_SIG]; // Last MAX_SIG signal values               (dB)
   LCreal      avgSig;          // Average signal                           (dB)
   LCreal      maxSig;          // Max Signal                               (dB)
   int         nSig;            // Number of signals
   int         iSig;            // Signal index;
};


//------------------------------------------------------------------------------
// Class: RfTrack
// Base class: Basic::Object -> Track -> RfTrack
// Factory name: RfTrack
// Description: Generic R/F Target Track (onboard sensors)
//------------------------------------------------------------------------------
class RfTrack : public Track
{
   DECLARE_SUBCLASS(RfTrack,Track)

public:
   RfTrack();

   // Signal strength and last emission for onboard R/F sensor tracks
   LCreal getAvgSignal() const                     { return avgSig; }
   LCreal getMaxSignal() const                     { return maxSig; }
   int getNumSignals() const                       { return nSig; }
   const Emission* getLastEmission() const         { return lastEM; }
   virtual bool setSignal(const LCreal snDbl, const Emission* const em);

   // Missile warning (from an RWR)
   bool isMissileWarning() const                   { return mslWarn; }
   virtual bool setMissileWarning(const bool b);

   void clear() override;

//protected:
   virtual bool setLastEmission(const Emission* const em);

protected:
   const Emission* lastEM;      // Last emission

};

//------------------------------------------------------------------------------
// Class: IrTrack
// Factory name: IrTrack
// Description: Generic IR Target Track (onboard sensors)
//------------------------------------------------------------------------------
class IrTrack : public Track
{
   DECLARE_SUBCLASS(IrTrack,Track)

public:
   IrTrack();

   // Signal strength and last emission for onboard IR sensor tracks
   LCreal getAvgSignal() const                                             { return avgSig; }
   LCreal getMaxSignal() const                                             { return maxSig; }
   int getNumSignals() const                                               { return nSig; }
   const IrQueryMsg* getLastQuery() const                                  { return lastQuery; }
   virtual bool setSignal(const LCreal snDbl, const IrQueryMsg* const q);
   bool setPosition(const osg::Vec3& p) override;

   // Missile warning (from an RWR)
   bool isMissileWarning() const                                           { return mslWarn; }
   virtual bool setMissileWarning(const bool b);

   void clear() override;

//protected:
   virtual bool setLastQuery(const IrQueryMsg* const q);

protected:
   const IrQueryMsg* lastQuery; // Last query

};

} // End Simulation namespace
} // End Eaagles namespace

#endif

