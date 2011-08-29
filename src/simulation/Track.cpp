#include "openeaagles/simulation/Track.h"

#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/SensorMsg.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: Track
//==============================================================================

IMPLEMENT_SUBCLASS(Track,"Track")
EMPTY_SLOTTABLE(Track)
EMPTY_SERIALIZER(Track)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Track::Track() : tgt(0)
{
    STANDARD_CONSTRUCTOR()

    id = 0;
    clear();
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Track::copyData(const Track& org, const bool cc)
{
   BaseClass::copyData(org);

   // If copy constructor, init these pointers
   if (cc) {
      tgt = 0;
   }

   setTarget( org.tgt );

   // General track ID and status
   id = org.id;
   type = org.type;
   iffCode = org.iffCode;
   age = org.age;
   quality = org.quality;

   //Track Class
   trackClass = org.trackClass;

   // Track State Vector
   latitude = org.latitude;
   longitude = org.longitude;
   llValid = org.llValid;
   pos = org.pos;
   los = org.los;
   vel = org.vel;
   accel = org.accel;
   rng = org.rng;
   rngRate = org.rngRate;
   cErr = org.cErr;
   vErr = org.vErr;

   // Data computed from state vector
   gndRng = org.gndRng;
   taz = org.taz;
   aa  = org.aa;
   gndSpd = org.gndSpd;
   gndTrk = org.gndTrk;
   relGndTrk = org.relGndTrk;
   for (unsigned int i = 0; i < 3; i++) {
      raz[i] = org.raz[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      rel[i] = org.rel[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      predictedRaz[i] = org.predictedRaz[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      predictedRel[i] = org.predictedRel[i];
   }

   // Shoot list support
   shootList = org.shootList;
   wpnRel = org.wpnRel;
   rejected = org.rejected;

   // Ownship data
   osGndTrk = org.osGndTrk;
   osVel = org.osVel;
   osAccel = org.osAccel;
}

void Track::deleteData()
{
   setTarget(0);
   clear();
}


//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void Track::clear()
{
   // General track ID and status
   id = 0;
   type = 0;
   iffCode = UNKNOWN;
   age = 0.0;
   quality = 0.0;

   // Track State Vector
   latitude = 0.0;
   longitude = 0.0;
   llValid = false;
   pos.set(0,0,0);
   los.set(0,0,0);
   vel.set(0,0,0);
   accel.set(0,0,0);
   rng = 0.0;
   rngRate = 0.0;
   cErr = 0.0;
   vErr = 0.0;

   // Data computed from state vector
   gndRng = 0.0;
   taz = 0.0;
   aa  = 0.0;
   gndSpd = 0.0;
   gndTrk = 0.0;
   relGndTrk = 0.0;
   for (int i = 0; i < 3; i++) {
      raz[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      rel[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      predictedRaz[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      predictedRel[i] = 0.0;
   }

   // Shoot list support
   shootList = 0;
   wpnRel = false;
   rejected = false;

   // Ownship data
   osGndTrk = 0;
   osVel.set(0,0,0);
   osAccel.set(0,0,0);


   setTarget(0);
}


//------------------------------------------------------------------------------
// ownshipDynamics() -- apply ownship dynamics to predicted track position
//------------------------------------------------------------------------------
void Track::ownshipDynamics(const LCreal gtrk, const osg::Vec3 velOS, const osg::Vec3 accelOS, const LCreal)
{
    osGndTrk = gtrk;
   osVel = velOS;
   osAccel = accelOS;
}

//------------------------------------------------------------------------------
// getLatLonPosition() -- set track's Lat/Lon position; returns true if latitude
// and longitude are valid, and 'lat' and 'lon' are set; returns false if latitude
// and longitude are not valid, and 'lat' and 'lon' are not changed.
//------------------------------------------------------------------------------
bool Track::getLatLonPosition(double* const lat, double* const lon) const
{
   bool ok = false;
   if (llValid && lat != 0 && lon != 0) {
      *lat = latitude;
      *lon = longitude;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setTrackID() -- set local track ID
bool Track::setTrackID(const int v)
{
   id = v;
   return true;
}
// setType() -- set all of the track type bits to 't'
bool Track::setType(const short t)
{
   type = t;
   return true;
}

// setSubtype() -- bitwise-OR the type bits with 't'
bool Track::setSubtype(const short t)
{
   type = (t | type);
   return true;
}

// setClass() -- set all of the track class with 'c'
bool Track::setClass(const TrackClass c)
{
    trackClass = c;
    return true;
}

// updateTrackAge() -- Update the age of the track by delta time, dt (seconds)
bool Track::updateTrackAge(const LCreal dt)
{
   age += dt;
   return true;
}

// resetTrackAge() -- Reset the age of the track to zero
bool Track::resetTrackAge()
{
   age = 0.0;
   return true;
}

// setQuality() -- Sets the normalized track quality value
bool Track::setQuality(const LCreal v)
{
   quality = v;
   return true;
}

// setLatLonPosition() -- set track's position vector
bool Track::setLatLonPosition(const double lat, const double lon)
{
   llValid = false;
   latitude = lat;
   longitude = lon;
   if (latitude <= 90.0 && latitude >= -90.0) {
      if (longitude <= 180.0 && longitude >= -180.0) {
         llValid = true;
      }
   }
   return llValid;
}

// clearLatLonValid() -- clears the track's latitude/longitude position validity
void Track::clearLatLonValid()
{
   llValid = false;
}

// setPosition() -- set track's position vector
bool Track::setPosition(const osg::Vec3& p)
{
   // set position vector
   pos = p;

   // compute ranges
   LCreal gndRng2 = pos.x()*pos.x() + pos.y()*pos.y();
   gndRng = lcSqrt(gndRng2);
   rng = lcSqrt(gndRng2 +  pos.z()*pos.z());

   // compute angles
   taz = lcAtan2(pos.y(),pos.x());
   raz[0] = lcAepcRad(taz - osGndTrk);
   rel[0] = lcAtan2(-pos.z(), gndRng);

   // Set LOS unit vector
   if (rng > 0) los.set( pos.x()/rng, pos.y()/rng, pos.z()/rng );
   else los.set(0,0,0);

   return true;
}


// setPosition() -- set track's position vector with a 4D vector; use only x, y and z
bool Track::setPosition(const osg::Vec4& p)
{
   osg::Vec3 pos3(p.x(),p.y(),p.z());
   return setPosition(pos3);
}

// setCircularError() -- sets the track's estimated position error, meters, 
bool Track::setCircularError(const LCreal err)
{
   cErr = err;
   return true;
}

// setVerticalError() -- sets the track's estimated altitude error, meters, 
bool Track::setVerticalError(const LCreal err)
{
   vErr = err;
   return true;
}

// setVelocity() -- set track's velocity vector and computer ground speed & track
bool Track::setVelocity(const osg::Vec3 v)
{
   vel = v;

   // Total velocity (NED) (m/s)
   osg::Vec3 totalVel = vel + osVel;

   gndSpd = lcSqrt(totalVel[0]*totalVel[0] + totalVel[1]*totalVel[1]);
   gndTrk = lcAtan2(totalVel[1],totalVel[0]);
   relGndTrk = lcAepcRad(gndTrk - osGndTrk);

   LCreal tmp1 = pos[1] * totalVel[0] - pos[0] * totalVel[1];
   LCreal tmp2 = pos[0] * totalVel[0] + pos[1] * totalVel[1];
   aa = lcAtan2(-tmp1,tmp2);

   return true;
}

// setAcceleration() -- set track's acceleration vector
bool Track::setAcceleration(const osg::Vec3 a)
{
   accel = a;
   return true;
}

// setRange() -- sets slant range to track, meters
bool Track::setRange(const LCreal r)
{
   rng = r;
   return true;
}

// setRangeRate() -- sets the rate of change of the slate range to target, m/s
bool Track::setRangeRate(const LCreal rr)
{
   rngRate = rr;
   return true;
}

// setIffCode() -- sets the IFF code
bool Track::setIffCode(const IffCode s)
{
   iffCode = s;
   return true;
}

// setShootListIndex() -- sets the track's shootlist index
bool Track::setShootListIndex(const int idx)
{
   shootList = idx;
   return true;
}

// setWeaponReleased() -- flags that we've released a weapon at this track
bool Track::setWeaponReleased(const bool f)
{
   wpnRel = f;
   return true;
}

// setRejected() -- rejects this track from the shootlist
bool Track::setRejected(const bool f)
{
   rejected = f;
   return true;
}

// setTarget() -- set the target pointer
bool Track::setTarget(Player* const p)
{
   if (tgt != 0) tgt->unref();
   tgt = p;
   if (tgt != 0) tgt->ref();
   return true;
}

//==============================================================================
// Class: RfTrack
//==============================================================================

IMPLEMENT_SUBCLASS(RfTrack,"RfTrack")
EMPTY_SLOTTABLE(RfTrack)
EMPTY_SERIALIZER(RfTrack)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
RfTrack::RfTrack() : lastEM(0)
{
    STANDARD_CONSTRUCTOR()

    clear();
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void RfTrack::copyData(const RfTrack& org, const bool cc)
{
    BaseClass::copyData(org);

    // If copy constructor, init these pointers
    if (cc) {
        lastEM = 0;
    }
    
    setLastEmission( org.lastEM );

    mslWarn = org.mslWarn;

    avgSig = org.avgSig;
    maxSig = org.maxSig;
    nSig = org.nSig;
    iSig = org.iSig;
    for (int i = 0; i < MAX_SIG; i++) {
       lastSN[i] = org.lastSN[i];
    }
}

void RfTrack::deleteData()
{
    setLastEmission(0);
}

//------------------------------------------------------------------------------
// setSignal() -- set the track's last signal
//------------------------------------------------------------------------------
bool RfTrack::setSignal(const LCreal snDbl, const Emission* const em)
{
    // Save the emission
    setLastEmission(em);

    // Save the signal
    lastSN[iSig++] = snDbl;
    if (iSig >= MAX_SIG) iSig = 0;
    if (nSig < MAX_SIG) nSig++;

    // Compute average signal
    LCreal sum = 0.0f;
    LCreal avg = 0.0f;
    LCreal maxs = 0.0f;
    if (nSig > 0) {
        for (int i = 0; i < nSig; i++) {
            sum += lastSN[i];
            if (lastSN[i] > maxs) maxs = lastSN[i];
        }
        avg = sum/LCreal(nSig);
    }
    avgSig = avg;
    maxSig = maxs;

    return true;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLastEmission() -- set the last emission pointer
bool RfTrack::setLastEmission(const Emission* const em)
{
   if (lastEM != 0) lastEM->unref(); 
   lastEM = em;
   if (lastEM != 0) lastEM->ref();
   return true;
}

// Set missile warning flag
bool RfTrack::setMissileWarning(const bool b)
{
   mslWarn = b;
   return true;
}

//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void RfTrack::clear()
{
   mslWarn = false;

   setLastEmission(0);
   avgSig = 0;
   maxSig = 0;
   nSig = 0;
   iSig = 0;

   trackClass = RANGE_AND_ANGLE;

   BaseClass::clear();
}

//==============================================================================
// Class: IrTrack
//==============================================================================

IMPLEMENT_SUBCLASS(IrTrack,"IrTrack")
EMPTY_SLOTTABLE(IrTrack)
EMPTY_SERIALIZER(IrTrack)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrTrack::IrTrack() : lastQuery(0)
{
    STANDARD_CONSTRUCTOR()
    
    clear();
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void IrTrack::copyData(const IrTrack& org, const bool cc)
{
    BaseClass::copyData(org);

    // If copy constructor, init these pointers
    if (cc) {
        lastQuery = 0;
    }
    
    setLastQuery( org.lastQuery );

    mslWarn = org.mslWarn;

    avgSig = org.avgSig;
    maxSig = org.maxSig;
    nSig = org.nSig;
    iSig = org.iSig;
    for (int i = 0; i < MAX_SIG; i++) {
       lastSN[i] = org.lastSN[i];
    }
}

void IrTrack::deleteData()
{
    setLastQuery(0);
}

//------------------------------------------------------------------------------
// setSignal() -- set the track's last signal
//------------------------------------------------------------------------------
bool IrTrack::setSignal(const LCreal snDbl, const IrQueryMsg* const q)
{
    // Save the emission
    setLastQuery(q);

    // Save the signal
    lastSN[iSig++] = snDbl;
    if (iSig >= MAX_SIG) iSig = 0;
    if (nSig < MAX_SIG) nSig++;

    // Compute average signal
    LCreal sum = 0.0f;
    LCreal avg = 0.0f;
    LCreal maxs = 0.0f;
    if (nSig > 0) {
        for (int i = 0; i < nSig; i++) {
            sum += lastSN[i];
            if (lastSN[i] > maxs) maxs = lastSN[i];
        }
        avg = sum/LCreal(nSig);
    }
    avgSig = avg;
    maxSig = maxs;

    return true;
}

// setPosition() -- set track's position vector - but do not set rel az or el
bool IrTrack::setPosition(const osg::Vec3& p)
{
   // set position vector
   pos = p;

   // compute ranges
   LCreal gndRng2 = pos.x()*pos.x() + pos.y()*pos.y();
   gndRng = lcSqrt(gndRng2);
   rng = lcSqrt(gndRng2 +  pos.z()*pos.z());

   // compute angles
   taz = lcAtan2(pos.y(),pos.x());

   // Set LOS unit vector
   if (rng > 0) los.set( pos.x()/rng, pos.y()/rng, pos.z()/rng );
   else los.set(0,0,0);

   return true;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLastQuery() -- set the last emission pointer
bool IrTrack::setLastQuery(const IrQueryMsg* const q)
{
   if (lastQuery != 0) lastQuery->unref(); 
   lastQuery = q;
   if (lastQuery != 0) lastQuery->ref();
   return true;
}

// Set missile warning flag
bool IrTrack::setMissileWarning(const bool b)
{
   mslWarn = b;
   return true;
}

//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void IrTrack::clear()
{
   mslWarn = false;

   setLastQuery(0);
   avgSig = 0;
   maxSig = 0;
   nSig = 0;
   iSig = 0;

   trackClass = ANGLE_ONLY;

   BaseClass::clear();
}

} // End Simulation namespace
} // End Eaagles namespace
