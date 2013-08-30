
#include "openeaagles/simulation/DynamicsModels.h"

#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Simulation {

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
void DynamicsModel::dynamics(const LCreal)
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
LCreal DynamicsModel::getFuelWt() const
{
    return 0.0;
}

LCreal DynamicsModel::getFuelWtMax() const
{
    return 0.0;
}

LCreal DynamicsModel::getGrossWeight() const
{
    return 0.0;
}

int DynamicsModel::getNumberOfEngines() const
{
    return 0;
}

int DynamicsModel::getEngThrust(LCreal* const, const int) const
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
void DynamicsModel::setControlStickRollInput(const LCreal)
{
}

//------------------------------------------------------------------------------
// setControlStickPitchInput(Pitch) --  Control inputs: normalized
//  pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//------------------------------------------------------------------------------
void DynamicsModel::setControlStickPitchInput(const LCreal)
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
int DynamicsModel::setThrottles(const LCreal* const, const int)
{
    return 0;
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void DynamicsModel::setBrakes(const LCreal, const LCreal)
{
}

//------------------------------------------------------------------------------
// Sets the fuel weight
//------------------------------------------------------------------------------
bool DynamicsModel::setFuelWt(const LCreal)
{
   return false;
}

//==============================================================================
// class AerodynamicsModel
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AerodynamicsModel,"AerodynamicsModel")
EMPTY_SERIALIZER(AerodynamicsModel)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AerodynamicsModel::AerodynamicsModel()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void AerodynamicsModel::copyData(const AerodynamicsModel& org, const bool)
{
   BaseClass::copyData(org);
}

void AerodynamicsModel::deleteData()
{
}

//------------------------------------------------------------------------------
// Data access functions that need conversion 
//------------------------------------------------------------------------------

LCreal AerodynamicsModel::getAngleOfAttackR() const
{
   return getAngleOfAttack();
}

LCreal AerodynamicsModel::getAngleOfAttackD() const
{ 
   return ( (LCreal)Basic::Angle::R2DCC * getAngleOfAttack() );
}

LCreal AerodynamicsModel::getFlightPathR() const
{
   return getFlightPath();
}

LCreal AerodynamicsModel::getFlightPathD() const
{ 
   return ( (LCreal)Basic::Angle::R2DCC * getFlightPath() );
}

LCreal AerodynamicsModel::getSideSlipR() const
{
   return getSideSlip();
}

LCreal AerodynamicsModel::getSideSlipD() const
{ 
   return ( (LCreal)Basic::Angle::R2DCC * getSideSlip() );
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
LCreal AerodynamicsModel::getGload() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getMach() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getAngleOfAttack() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getSideSlip() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getFlightPath() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getCalibratedAirspeed() const
{
    return 0.0;
}

LCreal AerodynamicsModel::getAmbientPressureRatio() const
{
   return 1.0;
}

LCreal AerodynamicsModel::getWingSweepAngle() const
{
   return 0.0;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right 
//------------------------------------------------------------------------------
void AerodynamicsModel::setTrimSwitchRollInput(const LCreal)
{
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp 
//------------------------------------------------------------------------------
void AerodynamicsModel::setTrimSwitchPitchInput(const LCreal)
{
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void AerodynamicsModel::setRudderPedalInput(const LCreal)
{
}

//------------------------------------------------------------------------------
// setFlaps() -- 
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void AerodynamicsModel::setFlaps(const LCreal)
{
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended 
//------------------------------------------------------------------------------
LCreal AerodynamicsModel::getLandingGearPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open 
//------------------------------------------------------------------------------
LCreal AerodynamicsModel::getWeaponBayDoorPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool AerodynamicsModel::isWeightOnWheels() const
{
    return false;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> Handle up;  1 -> hande down
//------------------------------------------------------------------------------
void AerodynamicsModel::setGearHandleSwitch(const LCreal)
{
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended 
//------------------------------------------------------------------------------
LCreal AerodynamicsModel::getSpeedBrakePosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend 
//------------------------------------------------------------------------------
void AerodynamicsModel::setSpeedBrakesSwitch(const LCreal)
{
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void AerodynamicsModel::setWeaponBayDoorSwitch(const LCreal)
{
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void AerodynamicsModel::setCmdWingSweepAngle(const LCreal)
{
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int AerodynamicsModel::getEngRPM(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngN1(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngPLA(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngFuelFlow(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngOilPressure(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngInletTemp(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngNozzle(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustIdle(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustMil(LCreal* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustAb(LCreal* const, const int) const
{
    return 0;
}

} // End Simulation namespace
} // End Eaagles namespace
