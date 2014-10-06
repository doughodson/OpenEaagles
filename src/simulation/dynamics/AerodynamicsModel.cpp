
#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AerodynamicsModel, "AerodynamicsModel")
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
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getAngleOfAttack() );
}

LCreal AerodynamicsModel::getFlightPathR() const
{
   return getFlightPath();
}

LCreal AerodynamicsModel::getFlightPathD() const
{ 
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getFlightPath() );
}

LCreal AerodynamicsModel::getSideSlipR() const
{
   return getSideSlip();
}

LCreal AerodynamicsModel::getSideSlipD() const
{ 
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getSideSlip() );
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
