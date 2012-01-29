#include "openeaagles/simulation/SynchronizedState.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class SynchronizedState
//==============================================================================
IMPLEMENT_SUBCLASS(SynchronizedState,"SynchronizedState")
EMPTY_SLOTTABLE(SynchronizedState)
EMPTY_SERIALIZER(SynchronizedState)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SynchronizedState::SynchronizedState()
{
   STANDARD_CONSTRUCTOR()

   posVecECEF.set(0,0,0);
   velVecECEF.set(0,0,0);
   accelVecECEF.set(0,0,0);
   anglesW.set(0,0,0);
   angVel.set(0,0,0);
   timeExec = 0.0;
   timeUtc = 0.0;
   valid = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SynchronizedState::copyData(const SynchronizedState& org, const bool cc)
{
   BaseClass::copyData(org);

   // If copy constructor, init these pointers
   if (cc) {

   }

   // Copy the data
   posVecECEF = org.posVecECEF;
   velVecECEF = org.velVecECEF;
   accelVecECEF = org.accelVecECEF;
   anglesW = org.anglesW;
   angVel = org.angVel;
   timeExec = org.timeExec;
   timeUtc = org.timeUtc;
   valid = org.valid;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SynchronizedState::deleteData()
{
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clears out the SynchronizedStates
//------------------------------------------------------------------------------
void SynchronizedState::clear()
{
   posVecECEF.set(0,0,0);
   velVecECEF.set(0,0,0);
   accelVecECEF.set(0,0,0);
   anglesW.set(0,0,0);
   angVel.set(0,0,0);
   timeExec = 0.0;
   timeUtc = 0.0;
   valid = false;
}

//------------------------------------------------------------------------------
// Geocentric position vector (meters)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocPosition(const osg::Vec3d& gcPos)
{
   posVecECEF = gcPos;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric velocity vector (meters/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocVelocity(const osg::Vec3& newVelEcef)
{
   velVecECEF = newVelEcef;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric acceleration vector (meters/second/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocAcceleration(const osg::Vec3& newAccelEcef)
{
   accelVecECEF = newAccelEcef;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric euler angles (radians)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocEulerAngles(const osg::Vec3d& newAngles)
{
   anglesW = newAngles;
   return true;
}

//------------------------------------------------------------------------------
// Body angular velocities (radians/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setAngularVelocities(const osg::Vec3d& newAngVel)
{
   angVel = newAngVel;
   return true;
}

} // End Simulation namespace
} // End Eaagles namespace
