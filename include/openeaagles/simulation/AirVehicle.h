//------------------------------------------------------------------------------
// Classes: AirVehicle, Aircraft, Helicopter, UnmannedAirVehicle
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_AirVehicle_H__
#define __oe_Simulation_AirVehicle_H__

#include "openeaagles/simulation/Player.h"

namespace oe {
namespace Simulation {
   class AerodynamicsModel;

//==============================================================================
// Class AirVehicle
//
// Base class: basic::Object -> basic::Component -> Player -> AirVehicle
//                                           ..  -> AirVehicle -> Aircraft
//                                           ..  -> AirVehicle -> Helicopter
//                                           ..  -> AirVehicle -> UnmannedAirVehicle
//
// Factory name: AirVehicle
// Slots:
//  initGearPos     ! Initial gear position (default: up),
//                  !   <basic::String>: "up" or "down"
//                  !   <basic::Number>: zero(0) for up, non-zero for down
//
//
// Description: Generic Air Vehicles
//
//      setControlStick(LCreal roll, LCreal pitch)
//          Control inputs: normalized
//          roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//          pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//
//      setTrimSwitchRollInput(LCreal rollTrim)
//      setTrimSwitchPitchInput(LCreal pitchTrim)
//      setTrimSwitch()
//          Sets the trim switch positions:
//              PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//              RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//
//      setRudderPedalInput(const LCreal pedal)
//          Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//
//      setBrakes(const LCreal left, const LCreal right);
//          Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//
//      setFlaps(const LCreal flap);
//          Commanded flap position from retracted (0.0) to fully extended (1.0)
//
//      setGearHandleSwitch(const LCreal sw)
//          Set gear handle position:
//              Handle Up   -> 0.0
//              Handle Down -> 1.0
//
//      setSpeedBrakesSwitch(LCreal sw)
//          Sets the speed brake switch position:
//              -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//
//      setWeaponBayDoorSwitch(LCreal sw)
//          Weapon bay door position: 0.0 -> closed; 1.0 -> open
//
//      setCmdWingSweepAngle(const LCreal rad)
//          Sets the commanded wing sweep angle (radians; zero for none)
//
//==============================================================================
class AirVehicle : public Player
{
    DECLARE_SUBCLASS(AirVehicle,Player)

public:
    AirVehicle();

    // Vehicle data
    virtual LCreal getGload() const;                    // Gs (+up, one at level)
    LCreal getMach() const override;                    // mach number
    virtual LCreal getAngleOfAttack() const;            // radians
    virtual LCreal getSideSlip() const;                 // radians
    virtual LCreal getFlightPath() const;               // radians
    virtual LCreal getCalibratedAirspeed() const;       // Kts
    virtual LCreal getFuelWt() const;                   // wt: lbs
    virtual LCreal getFuelWtMax() const;                // wt: lbs
    LCreal getGrossWeight() const override;             // wt: lbs
    virtual LCreal getSpeedBrakePosition() const;       // %
    virtual LCreal getLandingGearPosition() const;      // % (0% up; 100% down)
    virtual LCreal getWeaponBayDoorPosition() const;    // % (0% closed; 100% open)
    virtual LCreal getAmbientPressureRatio() const;     // Air pressure ratio
    virtual LCreal getWingSweepAngle() const;           // radians (or zero for no wing sweep)
    virtual bool   isWeightOnWheels() const;            // True/false

    // Unit specific vehicle data
    LCreal getAngleOfAttackR() const                    { return getAngleOfAttack(); }
    LCreal getAngleOfAttackD() const;
    LCreal getFlightPathR() const                       { return getFlightPath(); }
    LCreal getFlightPathD() const;
    LCreal getSideSlipR() const                         { return getSideSlip(); }
    LCreal getSideSlipD() const;

    // Engine data
    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(LCreal* const fn, const int max) const;          // Thrust: lbs
    virtual int getEngThrustIdle(LCreal* const fn, const int max) const;      // Thrust if at Idle: lbs
    virtual int getEngThrustMil(LCreal* const fn, const int max) const;       // Thrust if at MIL: lbs
    virtual int getEngThrustAb(LCreal* const fn, const int max) const;        // Thrust if at Full afterburner: lbs
    virtual int getEngRPM(LCreal* const rpm, const int max) const;            // (N2) Core %RPM
    virtual int getEngN1(LCreal* const n1, const int max) const;              // N1 %RPM
    virtual int getEngFuelFlow(LCreal* const ff, const int max) const;        // Rate: lbs/hour  (pph)
    virtual int getEngOilPressure(LCreal* const oil, const int max) const;    // Pressure: psi
    virtual int getEngInletTemp(LCreal* const tmp, const int max) const;      // Temp: C
    virtual int getEngNozzle(LCreal* const noz, const int max) const;         // Nozzle Pos: %
    virtual int getEngPLA(LCreal* const pla, const int max) const;            // PLA: degrees

    // Radar altimeter
    virtual bool isRadarAltValid() const;               // true if RA is valid
    virtual LCreal getRadarAltitude() const;            // feet

    // Control Stick Input Positions (not the flight surfaces!)
    void setControlStick(const LCreal roll, const LCreal pitch) {
        setControlStickRollInput(roll);
        setControlStickPitchInput(pitch);
    }

    // Trim switch inputs
    virtual void setTrimSwitchRollInput(const LCreal rollTrim);
    virtual void setTrimSwitchPitchInput(const LCreal pitchTrim);
    void setTrimSwitch(const LCreal rollTrim, const LCreal pitchTrim) {
        setTrimSwitchRollInput(rollTrim);
        setTrimSwitchPitchInput(pitchTrim);
    }

    virtual void setRudderPedalInput(const LCreal pedal);
    virtual void setBrakes(const LCreal left, const LCreal right);
    virtual void setFlaps(const LCreal pos);

    virtual void setGearHandleSwitch(const LCreal sw);
    virtual void setSpeedBrakesSwitch(const LCreal sw);
    virtual void setWeaponBayDoorSwitch(const LCreal sw);

    // Sets the commanded wing sweep angle (radians)
    virtual void setCmdWingSweepAngle(const LCreal rad);

    unsigned int getMajorType() const override;

    void reset() override;

protected:
   AerodynamicsModel* getAerodynamicsModel();
   const AerodynamicsModel* getAerodynamicsModel() const;

   bool setInitGearPos(const basic::String* const pos);
   bool setInitGearPos(const basic::Number* const pos);

private:
   LCreal initGearPos;     // initial gear position
   LCreal gearPos;         // Percent Gear position (0 -> up; 100 -> down)
   LCreal wpnBayDoorPos;   // Percent Weapon bay door position (0 -> closed; 100 -> open)
   LCreal wingSweep;       // Wing sweep angle (radians; zero for no wing sweep)
};


//==============================================================================
// Class Aircraft
// Factory name: Aircraft
//==============================================================================
class Aircraft : public AirVehicle
{
    DECLARE_SUBCLASS(Aircraft,AirVehicle)

public:
    Aircraft();
};


//==============================================================================
// Class Helicopter
// Factory name: Helicopter
//==============================================================================
class Helicopter : public AirVehicle
{
    DECLARE_SUBCLASS(Helicopter,AirVehicle)

public:
    Helicopter();
};


//==============================================================================
// Class UnmannedAirVehicle
// Factory name: UnmannedAirVehicle
//==============================================================================
class UnmannedAirVehicle : public AirVehicle
{
    DECLARE_SUBCLASS(UnmannedAirVehicle,AirVehicle)

public:
    UnmannedAirVehicle();
};

} // End Simulation namespace
} // End oe namespace

#endif
