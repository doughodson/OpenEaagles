
#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/base/units/Angles.h"

namespace oe {
namespace simulation {

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

double AerodynamicsModel::getAngleOfAttackR() const
{
   return getAngleOfAttack();
}

double AerodynamicsModel::getAngleOfAttackD() const
{ 
   return ( static_cast<double>(base::Angle::R2DCC) * getAngleOfAttack() );
}

double AerodynamicsModel::getFlightPathR() const
{
   return getFlightPath();
}

double AerodynamicsModel::getFlightPathD() const
{ 
   return ( static_cast<double>(base::Angle::R2DCC) * getFlightPath() );
}

double AerodynamicsModel::getSideSlipR() const
{
   return getSideSlip();
}

double AerodynamicsModel::getSideSlipD() const
{ 
   return ( static_cast<double>(base::Angle::R2DCC) * getSideSlip() );
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
double AerodynamicsModel::getGload() const
{
    return 0.0;
}

double AerodynamicsModel::getMach() const
{
    return 0.0;
}

double AerodynamicsModel::getAngleOfAttack() const
{
    return 0.0;
}

double AerodynamicsModel::getSideSlip() const
{
    return 0.0;
}

double AerodynamicsModel::getFlightPath() const
{
    return 0.0;
}

double AerodynamicsModel::getCalibratedAirspeed() const
{
    return 0.0;
}

double AerodynamicsModel::getAmbientPressureRatio() const
{
   return 1.0;
}

double AerodynamicsModel::getWingSweepAngle() const
{
   return 0.0;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right 
//------------------------------------------------------------------------------
void AerodynamicsModel::setTrimSwitchRollInput(const double)
{
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp 
//------------------------------------------------------------------------------
void AerodynamicsModel::setTrimSwitchPitchInput(const double)
{
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void AerodynamicsModel::setRudderPedalInput(const double)
{
}

//------------------------------------------------------------------------------
// setFlaps() -- 
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void AerodynamicsModel::setFlaps(const double)
{
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended 
//------------------------------------------------------------------------------
double AerodynamicsModel::getLandingGearPosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open 
//------------------------------------------------------------------------------
double AerodynamicsModel::getWeaponBayDoorPosition() const
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
void AerodynamicsModel::setGearHandleSwitch(const double)
{
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended 
//------------------------------------------------------------------------------
double AerodynamicsModel::getSpeedBrakePosition() const
{
    return 0.0;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend 
//------------------------------------------------------------------------------
void AerodynamicsModel::setSpeedBrakesSwitch(const double)
{
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void AerodynamicsModel::setWeaponBayDoorSwitch(const double)
{
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void AerodynamicsModel::setCmdWingSweepAngle(const double)
{
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int AerodynamicsModel::getEngRPM(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngN1(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngPLA(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngFuelFlow(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngOilPressure(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngInletTemp(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngNozzle(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustIdle(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustMil(double* const, const int) const
{
    return 0;
}

int AerodynamicsModel::getEngThrustAb(double* const, const int) const
{
    return 0;
}

}
}
