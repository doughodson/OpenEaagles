/**
 * @file        SoarState.h
 * @brief       Declaration of class SoarState.
 * @details     It is used to expose its parent PlaneState to Soar.
 *
 * @author      Marsil de Athayde Costa e Silva,
 * @author      Instituto Tecnologico de Aeronautica - ITA
 * @author      Laboratorio de Comando e Controle - ITA LAB C2
 * 
 * @date        13/06/2014
 * @version     1.0
 * @pre         
 * @bug         
 * @copyright   Copyright (c) 2014
 *
 */
 
#ifndef _SOAR_STATE_H_
#define _SOAR_STATE_H_

#include "soar/sml_Client.h"

#include "openeaagles/ubf/behaviors/PlaneState.h"

namespace oe {

namespace soar {

using namespace sml;
/** @class SoarState
 *  @brief This class is used to expose PlaneState to Soar.
 */
class SoarState : public oe::behaviors::PlaneState
{
    DECLARE_SUBCLASS(SoarState, oe::behaviors::PlaneState)
public:
    SoarState();
    SoarState( sml::Agent * agent );
    
    void setAgent( sml::Agent * agent ){ ourAgent = agent; };
    sml::Agent * getAgent(){ return ourAgent; };
    
    // float
    FloatElement * getRoll()          { return wmeRoll; };
    FloatElement * getPitch()         { return wmePitch; };
    FloatElement * getHeading()       { return wmeHeading; };
    FloatElement * getRollRate()      { return wmeRollRate; };
    FloatElement * getPitchRate()     { return wmePitchRate; };
    FloatElement * getYawRate()       { return wmeYawRate; };
    FloatElement * getAltitude()      { return wmeAltitude; };
    FloatElement * getThrottle()      { return wmeThrottle; };
    FloatElement * getSpeed()         { return wmeSpeed; };
    FloatElement * getPitchTrim()     { return wmePitchTrim; };
    
    FloatElement * getPitchToTracked()      { return wmePitchToTracked; };
    FloatElement * getHeadingToTracked()    { return wmeHeadingToTracked; };
    FloatElement * getDistanceToTracked()   { return wmeDistanceToTracked; };
    // int
    IntElement * getNumTracks()       { return wmeNumTracks; };
    IntElement * getTargetTrack()     { return wmeTargetTrack; };
    IntElement * getNumEngines()      { return wmeNumEngines; };
    // bool
    IntElement * getAlive()           { return wmeAlive; };
    IntElement * getTracking()        { return wmeTracking; };
    IntElement * getMissileFired()    { return wmeMissileFired; };
    IntElement * getIncomingMissile() { return wmeIncomingMissile; };
    
    void setRoll( FloatElement * Roll )                         { wmeRoll = Roll; };
    void setPitch( FloatElement * Pitch )                       { wmePitch = Pitch; };
    void setHeading( FloatElement * Heading )                   { wmeHeading = Heading; };
    void setRollRate( FloatElement * RollRate )                 { wmeRollRate = RollRate; };
    void setPitchRate( FloatElement * PitchRate )               { wmePitchRate = PitchRate; };
    void setYawRate( FloatElement * YawRate )                   { wmeYawRate = YawRate; };
    void setAltitude( FloatElement * Altitude )                 { wmeAltitude = Altitude; };
    void setThrottle( FloatElement * Throttle )                 { wmeThrottle = Throttle; };
    void setSpeed( FloatElement * Speed )                       { wmeSpeed = Speed; };
    void setPitchTrim( FloatElement * PitchTrim )               { wmePitchTrim = PitchTrim; };
    
    void setPitchToTracked( FloatElement * PitchToTracked )       { wmePitchToTracked = PitchToTracked; };
    void setHeadingToTracked( FloatElement * HeadingToTracked )   { wmeHeadingToTracked = HeadingToTracked; };
    void setDistanceToTracked( FloatElement * DistanceToTracked ) { wmeDistanceToTracked = DistanceToTracked; };
    // int
    void setNumTracks( IntElement * NumTracks )                 { wmeNumTracks = NumTracks; };
    void setTargetTrack( IntElement * TargetTrack )             { wmeTargetTrack = TargetTrack; };
    void setNumEngines( IntElement * NumEngines )               { wmeNumEngines = NumEngines; };
    // bool 
    void setAlive( IntElement * Alive )                         { wmeAlive = Alive; };
    void setTracking( IntElement * Tracking )                   { wmeTracking = Tracking; };
    void setMissileFired( IntElement * MissileFired )           { wmeMissileFired = MissileFired; };
    void setIncomingMissile( IntElement * IncomingMissile )     { wmeIncomingMissile = IncomingMissile; };
    
private:
    // This is our agent who will control us.
    // It is used to set the states from the class PlaneState.
    sml::Agent * ourAgent;
    // float
    FloatElement * wmeRoll;
    FloatElement * wmePitch;
    FloatElement * wmeHeading;
    FloatElement * wmeRollRate;
    FloatElement * wmePitchRate;
    FloatElement * wmeYawRate;
    FloatElement * wmeAltitude;
    FloatElement * wmeThrottle;
    FloatElement * wmeSpeed;
    FloatElement * wmePitchTrim;
    
    FloatElement * wmePitchToTracked;
    FloatElement * wmeHeadingToTracked;
    FloatElement * wmeDistanceToTracked;
    // int
    IntElement * wmeNumTracks;
    IntElement * wmeTargetTrack;
    IntElement * wmeNumEngines;
    // bool
    IntElement * wmeAlive;
    IntElement * wmeTracking;
    IntElement * wmeMissileFired;
    IntElement * wmeIncomingMissile;
};

}
}

#endif
