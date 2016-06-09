 
#ifndef __oe_ubf_soar_SoarState_H__
#define __oe_ubf_soar_SoarState_H__

#include "openeaagles/ubf/behaviors/PlaneState.h"

namespace sml {
   class Agent;
   class FloatElement;
   class IntElement;
}

namespace oe {
namespace soar {

//------------------------------------------------------------------------------
// Class: SoarState
// Description: Exposes PlaneState to Soar
//
// Factory name: SoarState
//------------------------------------------------------------------------------
class SoarState : public oe::behaviors::PlaneState
{
    DECLARE_SUBCLASS(SoarState, oe::behaviors::PlaneState)

public:
    SoarState();
    SoarState(sml::Agent*);

    void setAgent(sml::Agent* agent)            { ourAgent = agent; }
    sml::Agent* getAgent()                      { return ourAgent; }

    sml::FloatElement* getRoll()                { return wmeRoll; };
    sml::FloatElement* getPitch()               { return wmePitch; };
    sml::FloatElement* getHeading()             { return wmeHeading; };
    sml::FloatElement* getRollRate()            { return wmeRollRate; };
    sml::FloatElement* getPitchRate()           { return wmePitchRate; };
    sml::FloatElement* getYawRate()             { return wmeYawRate; };
    sml::FloatElement* getAltitude()            { return wmeAltitude; };
    sml::FloatElement* getThrottle()            { return wmeThrottle; };
    sml::FloatElement* getSpeed()               { return wmeSpeed; };
    sml::FloatElement* getPitchTrim()           { return wmePitchTrim; };
    
    sml::FloatElement* getPitchToTracked()      { return wmePitchToTracked; };
    sml::FloatElement* getHeadingToTracked()    { return wmeHeadingToTracked; };
    sml::FloatElement* getDistanceToTracked()   { return wmeDistanceToTracked; };

    sml::IntElement* getNumTracks()             { return wmeNumTracks; };
    sml::IntElement* getTargetTrack()           { return wmeTargetTrack; };
    sml::IntElement* getNumEngines()            { return wmeNumEngines; };

    sml::IntElement* getAlive()                 { return wmeAlive; };
    sml::IntElement* getTracking()              { return wmeTracking; };
    sml::IntElement* getMissileFired()          { return wmeMissileFired; };
    sml::IntElement* getIncomingMissile()       { return wmeIncomingMissile; };
    
    void setRoll(sml::FloatElement* Roll)                           { wmeRoll = Roll; };
    void setPitch(sml::FloatElement* Pitch)                         { wmePitch = Pitch; };
    void setHeading(sml::FloatElement* Heading)                     { wmeHeading = Heading; };
    void setRollRate(sml::FloatElement* RollRate)                   { wmeRollRate = RollRate; };
    void setPitchRate(sml::FloatElement* PitchRate)                 { wmePitchRate = PitchRate; };
    void setYawRate(sml::FloatElement* YawRate)                     { wmeYawRate = YawRate; };
    void setAltitude(sml::FloatElement* Altitude)                   { wmeAltitude = Altitude; };
    void setThrottle(sml::FloatElement* Throttle)                   { wmeThrottle = Throttle; };
    void setSpeed(sml::FloatElement* Speed)                         { wmeSpeed = Speed; };
    void setPitchTrim(sml::FloatElement* PitchTrim)                 { wmePitchTrim = PitchTrim; };
    
    void setPitchToTracked(sml::FloatElement* PitchToTracked)       { wmePitchToTracked = PitchToTracked; };
    void setHeadingToTracked(sml::FloatElement* HeadingToTracked)   { wmeHeadingToTracked = HeadingToTracked; };
    void setDistanceToTracked(sml::FloatElement* DistanceToTracked) { wmeDistanceToTracked = DistanceToTracked; };

    void setNumTracks(sml::IntElement* NumTracks)                   { wmeNumTracks = NumTracks; };
    void setTargetTrack(sml::IntElement* TargetTrack)               { wmeTargetTrack = TargetTrack; };
    void setNumEngines(sml::IntElement* NumEngines)                 { wmeNumEngines = NumEngines; };

    void setAlive(sml::IntElement* Alive)                           { wmeAlive = Alive; };
    void setTracking(sml::IntElement* Tracking)                     { wmeTracking = Tracking; };
    void setMissileFired(sml::IntElement* MissileFired)             { wmeMissileFired = MissileFired; };
    void setIncomingMissile(sml::IntElement* IncomingMissile)       { wmeIncomingMissile = IncomingMissile; };
    
private:

    // This is our agent who will control us.
    // It is used to set the states from the class PlaneState.
    sml::Agent* ourAgent;

    sml::FloatElement* wmeRoll;
    sml::FloatElement* wmePitch;
    sml::FloatElement* wmeHeading;
    sml::FloatElement* wmeRollRate;
    sml::FloatElement* wmePitchRate;
    sml::FloatElement* wmeYawRate;
    sml::FloatElement* wmeAltitude;
    sml::FloatElement* wmeThrottle;
    sml::FloatElement* wmeSpeed;
    sml::FloatElement* wmePitchTrim;
    
    sml::FloatElement* wmePitchToTracked;
    sml::FloatElement* wmeHeadingToTracked;
    sml::FloatElement* wmeDistanceToTracked;

    sml::IntElement* wmeNumTracks;
    sml::IntElement* wmeTargetTrack;
    sml::IntElement* wmeNumEngines;

    sml::IntElement* wmeAlive;
    sml::IntElement* wmeTracking;
    sml::IntElement* wmeMissileFired;
    sml::IntElement* wmeIncomingMissile;
};

}
}

#endif
