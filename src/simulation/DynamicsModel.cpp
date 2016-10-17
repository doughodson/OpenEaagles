
#include "openeaagles/simulation/DynamicsModel.hpp"

#include "openeaagles/base/units/Angles.hpp"

namespace oe {
namespace simulation {

//==============================================================================
// class DynamicsModel
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DynamicsModel,"DynamicsModel")
EMPTY_SERIALIZER(DynamicsModel)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
DynamicsModel::DynamicsModel()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void DynamicsModel::copyData(const DynamicsModel& org, const bool)
{
   BaseClass::copyData(org);
}

void DynamicsModel::deleteData()
{
}

//------------------------------------------------------------------------------
// dynamics() -- one pass update of this dynamics model
//               (called from Player::dynamics)
//------------------------------------------------------------------------------
void DynamicsModel::dynamics(const double)
{
}

//------------------------------------------------------------------------------
// atReleaseInit() -- init the model at the transition from PRE_RELEASE to
// ACTIVE mode.  Default is to call reset().  Used by Weapon players.
//------------------------------------------------------------------------------
void DynamicsModel::atReleaseInit()
{
   reset();
}


//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
double DynamicsModel::getFuelWt() const
{
    return 0.0;
}

double DynamicsModel::getFuelWtMax() const
{
    return 0.0;
}

double DynamicsModel::getGrossWeight() const
{
    return 0.0;
}

int DynamicsModel::getNumberOfEngines() const
{
    return 0;
}

int DynamicsModel::getEngThrust(double* const, const int) const
{
    return 0;
}



//------------------------------------------------------------------------------
// Autopilot controls
//------------------------------------------------------------------------------
bool DynamicsModel::isHeadingHoldOn() const
{
   return false;
}

double DynamicsModel::getCommandedHeadingD() const
{
   return 0;
}

// setHeadingHoldOn() --   Enable/Disable heading hold
bool DynamicsModel::setHeadingHoldOn(const bool)
{
   return false;
}

// setCommandedHeadingD() --   Sets commanded heading (true: degs)
bool DynamicsModel::setCommandedHeadingD(const double, const double, const double)
{
   return false;
}

bool DynamicsModel::isVelocityHoldOn() const
{
   return false;
}

double DynamicsModel::getCommandedVelocityKts() const
{
   return 0;
}
                                                                       
// setVelocityHoldOn() --   Enable/Disable velocity hold
bool DynamicsModel::setVelocityHoldOn(const bool)
{
   return false;
}

// setCommandedVelocityKts() --   Sets commanded velocity (kts)
bool DynamicsModel::setCommandedVelocityKts(const double, const double)
{
   return false;
}

bool DynamicsModel::isAltitudeHoldOn() const
{
   return false;
}

double DynamicsModel::getCommandedAltitude() const
{
   return 0;
}
                                                                             
// setAltitudeHoldOn() --   Enable/Disable altitude hold
bool DynamicsModel::setAltitudeHoldOn(const bool)
{
   return false;
}

// setCommandedAltitude() --   Sets commanded altitude (meters)
bool DynamicsModel::setCommandedAltitude(const double, const double, const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setControlStickRollInput(Roll) --  Control inputs: normalized
//   roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void DynamicsModel::setControlStickRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void DynamicsModel::setControlStickPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// int setThrottles(positions,num) -- Set throttle positions
//
//    positions -> Array of throttle positions
//      (for each throttle)
//              < 0.0       -> Cutoff
//              0.0         -> Idle
//              1.0         -> MIL
//              2.0         -> A/B
//    num -> number of throttle positions to get/set
//    returns the actual number of throttle positions
//------------------------------------------------------------------------------
int DynamicsModel::setThrottles(const double* const, const int)
{
    return 0;
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void DynamicsModel::setBrakes(const double, const double)
{
}

//------------------------------------------------------------------------------
// Sets the fuel weight
//------------------------------------------------------------------------------
bool DynamicsModel::setFuelWt(const double)
{
   return false;
}

}
}
