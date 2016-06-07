
#ifndef __oe_behaviors_PlaneState_H__
#define __oe_behaviors_PlaneState_H__

#include "openeaagles/base/ubf/State.h"

namespace oe {

namespace simulation { class Player; }

namespace behaviors {

//------------------------------------------------------------------------------
// Class: PlaneState
//
// Description: this implementation of PlaneState assumes that player using this
//              state has only one missile (or is ok with firing all missiles at
//              first target)
//------------------------------------------------------------------------------
class PlaneState : public base::ubf::State
{
   DECLARE_SUBCLASS(PlaneState, base::ubf::State)

public:
   PlaneState();

   virtual void reset() override;

   virtual void updateState(const base::Component* const actor) override;

   // set/get
   virtual void setAlive(const bool x)                 { alive = x; return; }
   virtual bool isAlive() const                        { return alive; }

   virtual void setRoll(const double x)                { roll = x; return; }
   virtual double getRoll() const                      { return roll; }

   virtual void setPitch(const double x)               { pitch = x; return; }
   virtual double getPitch() const                     { return pitch; }

   virtual void setHeading(const double x)             { heading = x; return; }
   virtual double getHeading() const                   { return heading; }

   virtual void setRollRate(const double x)            { rollRate = x; return; }
   virtual double getRollRate() const                  { return rollRate; }

   virtual void setPitchRate(const double x)           { pitchRate = x; return; }
   virtual double getPitchRate() const                 { return pitchRate; }

   virtual void setYawRate(const double x)             { yawRate = x; return; }
   virtual double getYawRate() const                   { return yawRate; }

   virtual void setAltitude(const double x)            { altitude = x; return; }
   virtual double getAltitude() const                  { return altitude; }

   virtual void setThrottle(const double x)            { throttle = x; return; }
   virtual double getThrottle() const                  { return throttle; }

   virtual void setSpeed(const double x)               { speed = x; return; }
   virtual double getSpeed() const                     { return speed; }

   virtual void setPitchTrim(const double x)           { pitchTrim = x; return; }
   virtual double getPitchTrim() const                 { return pitchTrim; }

   virtual void setNumTracks(const unsigned int x)     { numTracks = x; return; }
   virtual unsigned int getNumTracks() const           { return numTracks; }

   //tracking setter
   virtual void setTracking(const bool x)              { tracking = x; return; }
   //returns true if plane is currently tracking
   virtual bool isTracking() const                     { return tracking; }

   virtual void setMissileFired(const bool x)          { missileFired = x; return; }
   virtual bool isMissileFired() const                 { return missileFired; }

   virtual void setTargetTrack(const unsigned int x)   { targetTrack = x; return; }
   virtual unsigned int getTargetTrack() const         { return targetTrack; }

   virtual void setNumEngines(const unsigned int x)    { numEngines = x; return; }
   virtual int getNumEngines() const                   { return numEngines; }

   virtual void setIncomingMissile(const bool x)       { incomingMissile = x; return; }
   virtual bool isIncomingMissile() const              { return incomingMissile; }

   //sets the pitch to current object being tracked
   virtual void setPitchToTracked(const unsigned int track, const double angle);
   virtual double getPitchToTracked(const unsigned int track) const;

   virtual void setHeadingToTracked(const unsigned int track, const double angle);
   virtual double getHeadingToTracked(const unsigned int track) const;

   virtual void setDistanceToTracked(const unsigned int track, const double distance);
   virtual double getDistanceToTracked(const unsigned int track) const;

public:
   static const unsigned int MAX_TRACKS = 50;

private:
   void initData();

   bool alive;
   double roll;
   double pitch;
   double rollRate;
   double pitchRate;
   double yawRate;
   double heading;
   double altitude;
   double throttle;
   double speed;
   double pitchTrim;
   double pitchToTracked[MAX_TRACKS];
   double headingToTracked[MAX_TRACKS];
   double distanceToTracked[MAX_TRACKS];
   unsigned int targetTrack;
   unsigned int numTracks;
   bool tracking;
   bool missileFired;
   bool incomingMissile;
   int numEngines;

};

}
}

#endif
