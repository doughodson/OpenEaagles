#include "openeaagles/simulation/AirVehicle.h"

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// class AirVehicle
//==============================================================================
IMPLEMENT_SUBCLASS(AirVehicle,"AirVehicle")
EMPTY_SERIALIZER(AirVehicle)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(AirVehicle)
    "initGearPos",      // 1: initial gear position (default: up),
                        //      string: "up" or "down"; or
                        //      number: zero for up, non-zero for down
END_SLOTTABLE(AirVehicle)

// Map slot table to handles
BEGIN_SLOT_MAP(AirVehicle)
    ON_SLOT(1,setInitGearPos,Basic::String)
    ON_SLOT(1,setInitGearPos,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AirVehicle::AirVehicle()
{
    STANDARD_CONSTRUCTOR()

    static Basic::String generic("GenericAirVehicle");
    setType(&generic);

    initGearPos = 0.0;
    gearPos = 100.0;
    wpnBayDoorPos = 100.0;
    wingSweep = 0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void AirVehicle::copyData(const AirVehicle& org, const bool)
{
   BaseClass::copyData(org);

   initGearPos = org.initGearPos;
   gearPos = org.gearPos;
   wpnBayDoorPos = org.wpnBayDoorPos;
   wingSweep = org.wingSweep;
}

void AirVehicle::deleteData()
{
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int AirVehicle::getMajorType() const
{
    return AIR_VEHICLE;
}

//------------------------------------------------------------------------------
// reset() -- Reset handler
//------------------------------------------------------------------------------
void AirVehicle::reset()
{
   // reset our base class
   BaseClass::reset();

   if (isLocalPlayer()) {
      // reset the gear switch
      setGearHandleSwitch(initGearPos);
      if (initGearPos > 0) gearPos = 100.0;
      else gearPos = 0.0;
   }
}

//------------------------------------------------------------------------------
// Data access functions that need conversion
//------------------------------------------------------------------------------
LCreal AirVehicle::getAngleOfAttackD() const
{
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getAngleOfAttack() );
}

LCreal AirVehicle::getFlightPathD() const
{
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getFlightPath() );
}

LCreal AirVehicle::getSideSlipD() const
{
   return ( static_cast<LCreal>(Basic::Angle::R2DCC) * getSideSlip() );
}

AerodynamicsModel* AirVehicle::getAerodynamicsModel()
{
   return dynamic_cast<AerodynamicsModel*>( getDynamicsModel() );
}

const AerodynamicsModel* AirVehicle::getAerodynamicsModel() const
{
   return dynamic_cast<const AerodynamicsModel*>( getDynamicsModel() );
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* AirVehicle::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//-----------------------------------------------------------------------------
// Slot functions
//-----------------------------------------------------------------------------

// Set initial gear position by name: up, down
bool AirVehicle::setInitGearPos(const Basic::String* const pos)
{
   bool ok = false;
   if (pos != nullptr) {
      if (*pos == "up" || *pos == "UP") {
         initGearPos = 0.0;
         gearPos = 0.0;
         ok = true;
      }
      else if (*pos == "down" || *pos == "DOWN") {
         initGearPos = 1.0;
         gearPos = 100.0;
         ok = true;
      }
   }
   if (ok) {
      setGearHandleSwitch(initGearPos);
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "AirVehicle::setInitGearPos(): ERROR -- invalid position name; should be \"up\" or \"down\"" << std::endl;
      }
   }
   return ok;
}

// Set initial gear position by number: zero is up, non-zero is down
bool AirVehicle::setInitGearPos(const Basic::Number* const pos)
{
   bool ok = false;
   if (pos != nullptr) {
      if (pos->getReal() == 0.0f) {
         initGearPos = 0.0;
         gearPos = 0.0;
         ok = true;
      }
      else {
         initGearPos = 1.0;
         gearPos = 100.0;
         ok = true;
      }
   }
   if (ok) setGearHandleSwitch(initGearPos);
   return ok;
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
LCreal AirVehicle::getGload() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getGload();
   }
   return value;
}

LCreal AirVehicle::getMach() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) value = aero->getMach();
   else value = BaseClass::getMach();
   return value;
}

LCreal AirVehicle::getAngleOfAttack() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getAngleOfAttack();
   }
   return value;
}

LCreal AirVehicle::getSideSlip() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getSideSlip();
   }
   return value;
}

LCreal AirVehicle::getFlightPath() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getFlightPath();
   }
   return value;
}

LCreal AirVehicle::getFuelWt() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getFuelWt();
   }
   return value;
}

LCreal AirVehicle::getFuelWtMax() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getFuelWtMax();
   }
   return value;
}

LCreal AirVehicle::getGrossWeight() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getGrossWeight();
   }
   return value;
}

LCreal AirVehicle::getCalibratedAirspeed() const
{
   LCreal value = getTotalVelocityKts();      // Default: use total velocity
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getCalibratedAirspeed();
   }
   return value;
}

LCreal AirVehicle::getAmbientPressureRatio() const
{
   LCreal value = 1.0;      // Default: 1.0
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getAmbientPressureRatio();
   }
   return value;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void AirVehicle::setTrimSwitchRollInput(const LCreal value)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setTrimSwitchRollInput(value);
   }
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void AirVehicle::setTrimSwitchPitchInput(const LCreal value)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setTrimSwitchPitchInput(value);
   }
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void AirVehicle::setRudderPedalInput(const LCreal value)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setRudderPedalInput(value);
   }
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void AirVehicle::setBrakes(const LCreal left, const LCreal right)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setBrakes(left, right);
   }
}

//------------------------------------------------------------------------------
// setFlaps() --
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void AirVehicle::setFlaps(const LCreal value)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setFlaps(value);
   }
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
LCreal AirVehicle::getLandingGearPosition() const
{
   LCreal value = gearPos;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getLandingGearPosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open
//------------------------------------------------------------------------------
LCreal AirVehicle::getWeaponBayDoorPosition() const
{
   LCreal value = wpnBayDoorPos;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getWeaponBayDoorPosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool AirVehicle::isWeightOnWheels() const
{
   bool flg = false;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      flg = aero->isWeightOnWheels();
   }
   return flg;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> handle up;  1 -> handle down
//------------------------------------------------------------------------------
void AirVehicle::setGearHandleSwitch(const LCreal value)
{
   if (value > 0) gearPos = 100.0;
   else gearPos = 0;

   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setGearHandleSwitch(value);
   }
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
LCreal AirVehicle::getSpeedBrakePosition() const
{
   LCreal value = 0.0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getSpeedBrakePosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// getWingSweepAngle() --   Returns the wing sweep position angle (radians)
//------------------------------------------------------------------------------
LCreal AirVehicle::getWingSweepAngle() const
{
   LCreal value = wingSweep;

   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      value = aero->getWingSweepAngle();
   }
   return value;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void AirVehicle::setSpeedBrakesSwitch(const LCreal value)
{
   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setSpeedBrakesSwitch(value);
   }
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void AirVehicle::setWeaponBayDoorSwitch(const LCreal value)
{
   if (value > 0) wpnBayDoorPos = 100.0;
   else wpnBayDoorPos = 0;

   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setWeaponBayDoorSwitch(value);
   }
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void AirVehicle::setCmdWingSweepAngle(const LCreal value)
{
   wingSweep = value;

   AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      aero->setCmdWingSweepAngle(value);
   }
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int AirVehicle::getNumberOfEngines() const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getNumberOfEngines();
   }
   return n;
}

int AirVehicle::getEngThrust(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngThrust(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustIdle(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngThrustIdle(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustMil(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngThrustMil(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustAb(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngThrustAb(data, max);
   }
   return n;
}

int AirVehicle::getEngRPM(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngRPM(data, max);
   }
   return n;
}

int AirVehicle::getEngN1(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngN1(data, max);
   }
   return n;
}

int AirVehicle::getEngPLA(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngPLA(data, max);
   }
   return n;
}

int AirVehicle::getEngFuelFlow(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngFuelFlow(data, max);
   }
   return n;
}

int AirVehicle::getEngOilPressure(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngOilPressure(data, max);
   }
   return n;
}

int AirVehicle::getEngInletTemp(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngInletTemp(data, max);
   }
   return n;
}

int AirVehicle::getEngNozzle(LCreal* const data, const int max) const
{
   int n = 0;
   const AerodynamicsModel* aero = getAerodynamicsModel();
   if (aero != nullptr) {
      n = aero->getEngNozzle(data, max);
   }
   return n;
}

//------------------------------------------------------------------------------
// Radar altimeter
//------------------------------------------------------------------------------
bool AirVehicle::isRadarAltValid() const
{
    LCreal ra = getRadarAltitude();
    return (ra >= 0 && ra <= 5000);
}

LCreal AirVehicle::getRadarAltitude() const
{
    return static_cast<LCreal>(getAltitudeAglFt());
}


//==============================================================================
// class Aircraft
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Aircraft,"Aircraft")
EMPTY_SERIALIZER(Aircraft)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Aircraft::Aircraft()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("GenericAircraft");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Aircraft::copyData(const Aircraft& org, const bool)
{
    BaseClass::copyData(org);
}

void Aircraft::deleteData()
{
}


//==============================================================================
// class Helicopter
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Helicopter,"Helicopter")
EMPTY_SERIALIZER(Helicopter)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Helicopter::Helicopter()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("GenericHelicopter");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Helicopter::copyData(const Helicopter& org, const bool)
{
    BaseClass::copyData(org);
}

void Helicopter::deleteData()
{
}


//==============================================================================
// class UnmannedAirVehicle
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(UnmannedAirVehicle,"UnmannedAirVehicle")
EMPTY_SERIALIZER(UnmannedAirVehicle)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
UnmannedAirVehicle::UnmannedAirVehicle()
{
    STANDARD_CONSTRUCTOR()
    static Basic::String generic("GenericUnmannedAirVehicle");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void UnmannedAirVehicle::copyData(const UnmannedAirVehicle& org, const bool)
{
    BaseClass::copyData(org);
}

void UnmannedAirVehicle::deleteData()
{
}

} // End Simulation namespace
} // End Eaagles namespace
