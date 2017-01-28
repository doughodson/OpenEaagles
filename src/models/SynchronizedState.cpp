
#include "openeaagles/models/SynchronizedState.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(SynchronizedState, "SynchronizedState")
EMPTY_SLOTTABLE(SynchronizedState)
EMPTY_SERIALIZER(SynchronizedState)
EMPTY_DELETEDATA(SynchronizedState)

SynchronizedState::SynchronizedState()
{
   STANDARD_CONSTRUCTOR()

   clear();
}

void SynchronizedState::copyData(const SynchronizedState& org, const bool cc)
{
   BaseClass::copyData(org);

   // copy data
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
// clear() -- clear state data
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
bool SynchronizedState::setGeocPosition(const base::Vec3d& gcPos)
{
   posVecECEF = gcPos;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric velocity vector (meters/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocVelocity(const base::Vec3d& newVelEcef)
{
   velVecECEF = newVelEcef;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric acceleration vector (meters/second/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocAcceleration(const base::Vec3d& newAccelEcef)
{
   accelVecECEF = newAccelEcef;
   return true;
}

//------------------------------------------------------------------------------
// Geocentric Euler angles (radians)
//------------------------------------------------------------------------------
bool SynchronizedState::setGeocEulerAngles(const base::Vec3d& newAngles)
{
   anglesW = newAngles;
   return true;
}

//------------------------------------------------------------------------------
// Body angular velocities (radians/second)
//------------------------------------------------------------------------------
bool SynchronizedState::setAngularVelocities(const base::Vec3d& newAngVel)
{
   angVel = newAngVel;
   return true;
}

}
}

