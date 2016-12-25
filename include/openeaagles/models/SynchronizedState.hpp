
#ifndef __oe_models_SynchronizedState_H__
#define __oe_models_SynchronizedState_H__

#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/osg/Vec3d"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: SynchronizedState
//
// Description: A snapshot of position state at a given time
//------------------------------------------------------------------------------
class SynchronizedState : public base::Object
{
   DECLARE_SUBCLASS(SynchronizedState, base::Object)

public:
   SynchronizedState();

   const osg::Vec3d& getGeocPosition() const      { return posVecECEF; }    // Geocentric (ECEF) position vector [ x y z ] (meters)
   const osg::Vec3d& getGeocVelocity() const      { return velVecECEF; }    // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   const osg::Vec3d& getGeocAcceleration() const  { return accelVecECEF; }  // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   const osg::Vec3d& getGeocEulerAngles() const   { return anglesW; }       // Geocentric (body/ECEF) Euler angles
   const osg::Vec3d& getAngularVelocities() const { return angVel; }        // Body angular rates (radians/second)
   double getTimeExec() const                     { return timeExec; }      // Sim exec time of state data (seconds)
   double getTimeUtc() const                      { return timeUtc; }       // Sim time of day for the state data (seconds)
   virtual bool isValid() const override          { return valid; }

   virtual bool setGeocPosition(const osg::Vec3d& gcPos);
   virtual bool setGeocVelocity(const osg::Vec3d& newVelEcef);
   virtual bool setGeocAcceleration(const osg::Vec3d& newAccelEcef);
   virtual bool setGeocEulerAngles(const osg::Vec3d& newAngles);
   virtual bool setAngularVelocities(const osg::Vec3d& newAngVel);
   void setValid(const bool b)                                        { valid = b; }
   void setTimeExec(const double d)                                   { timeExec = d; }
   void setTimeUtc(const double d)                                    { timeUtc = d; }

   virtual void clear();         // clear state data

private:

   osg::Vec3d  posVecECEF;       // Geocentric position vector     (meters)  (ECEF)
   osg::Vec3d  velVecECEF;       // Geocentric velocity vector     (meters/second)  (ECEF)
   osg::Vec3d  accelVecECEF;     // Geocentric acceleration vector ((meters/second)/second) (ECEF)
   osg::Vec3d  anglesW;          // World (body/ECEF) Euler angles (radians)
   osg::Vec3d  angVel;           // Body angular velocities (radians/seconds)
   double timeExec;
   double timeUtc;
   bool valid;
};

}
}

#endif

