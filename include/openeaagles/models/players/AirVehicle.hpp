
#ifndef __oe_models_AirVehicle_H__
#define __oe_models_AirVehicle_H__

#include "openeaagles/models/players/Player.hpp"

namespace oe {
namespace models {

class AerodynamicsModel;

//------------------------------------------------------------------------------
// Class AirVehicle
//
// Base class: base::Object -> base::Component -> Player -> AirVehicle
//                                           ..  -> AirVehicle -> Aircraft
//                                           ..  -> AirVehicle -> Helicopter
//                                           ..  -> AirVehicle -> UnmannedAirVehicle
//
// Factory name: AirVehicle
// Slots:
//  initGearPos     ! Initial gear position (default: up),
//                  !   <base::String>: "up" or "down"
//                  !   <base::Number>: zero(0) for up, non-zero for down
//
//
// Description: Generic Air Vehicles
//
//      setControlStick(double roll, double pitch)
//          Control inputs: normalized
//          roll:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//          pitch:  -1.0 -> max forward (nose down); 0.0 -> center;  1.0 -> max back (nose up)
//
//      setTrimSwitchRollInput(double rollTrim)
//      setTrimSwitchPitchInput(double pitchTrim)
//      setTrimSwitch()
//          Sets the trim switch positions:
//              PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//              RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//
//      setRudderPedalInput(const double pedal)
//          Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//
//      setBrakes(const double left, const double right);
//          Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//
//      setFlaps(const double flap);
//          Commanded flap position from retracted (0.0) to fully extended (1.0)
//
//      setGearHandleSwitch(const double sw)
//          Set gear handle position:
//              Handle Up   -> 0.0
//              Handle Down -> 1.0
//
//      setSpeedBrakesSwitch(double sw)
//          Sets the speed brake switch position:
//              -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//
//      setWeaponBayDoorSwitch(double sw)
//          Weapon bay door position: 0.0 -> closed; 1.0 -> open
//
//      setCmdWingSweepAngle(const double rad)
//          Sets the commanded wing sweep angle (radians; zero for none)
//
//------------------------------------------------------------------------------
class AirVehicle : public Player
{
    DECLARE_SUBCLASS(AirVehicle, Player)

public:
    AirVehicle();

    // Vehicle data
    virtual double getGload() const;                    // Gs (+up, one at level)
    virtual double getMach() const override;            // mach number
    virtual double getAngleOfAttack() const;            // radians
    virtual double getSideSlip() const;                 // radians
    virtual double getFlightPath() const;               // radians
    virtual double getCalibratedAirspeed() const;       // Kts
    virtual double getFuelWt() const;                   // wt: lbs
    virtual double getFuelWtMax() const;                // wt: lbs
    virtual double getGrossWeight() const override;     // wt: lbs
    virtual double getSpeedBrakePosition() const;       // %
    virtual double getLandingGearPosition() const;      // % (0% up; 100% down)
    virtual double getWeaponBayDoorPosition() const;    // % (0% closed; 100% open)
    virtual double getAmbientPressureRatio() const;     // Air pressure ratio
    virtual double getWingSweepAngle() const;           // radians (or zero for no wing sweep)
    virtual bool   isWeightOnWheels() const;            // True/false

    // Unit specific vehicle data
    double getAngleOfAttackR() const                    { return getAngleOfAttack(); }
    double getAngleOfAttackD() const;
    double getFlightPathR() const                       { return getFlightPath(); }
    double getFlightPathD() const;
    double getSideSlipR() const                         { return getSideSlip(); }
    double getSideSlipD() const;

    // Engine data
    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(double* const fn, const int max) const;          // Thrust: lbs
    virtual int getEngThrustIdle(double* const fn, const int max) const;      // Thrust if at Idle: lbs
    virtual int getEngThrustMil(double* const fn, const int max) const;       // Thrust if at MIL: lbs
    virtual int getEngThrustAb(double* const fn, const int max) const;        // Thrust if at Full afterburner: lbs
    virtual int getEngRPM(double* const rpm, const int max) const;            // (N2) Core %RPM
    virtual int getEngN1(double* const n1, const int max) const;              // N1 %RPM
    virtual int getEngFuelFlow(double* const ff, const int max) const;        // Rate: lbs/hour  (pph)
    virtual int getEngOilPressure(double* const oil, const int max) const;    // Pressure: psi
    virtual int getEngInletTemp(double* const tmp, const int max) const;      // Temp: C
    virtual int getEngNozzle(double* const noz, const int max) const;         // Nozzle Pos: %
    virtual int getEngPLA(double* const pla, const int max) const;            // PLA: degrees

    // Radar altimeter
    virtual bool isRadarAltValid() const;               // true if RA is valid
    virtual double getRadarAltitude() const;            // feet

    // Control Stick Input Positions (not the flight surfaces!)
    void setControlStick(const double roll, const double pitch) {
        setControlStickRollInput(roll);
        setControlStickPitchInput(pitch);
    }

    // Trim switch inputs
    virtual void setTrimSwitchRollInput(const double rollTrim);
    virtual void setTrimSwitchPitchInput(const double pitchTrim);
    void setTrimSwitch(const double rollTrim, const double pitchTrim) {
        setTrimSwitchRollInput(rollTrim);
        setTrimSwitchPitchInput(pitchTrim);
    }

    virtual void setRudderPedalInput(const double pedal);
    virtual void setBrakes(const double left, const double right);
    virtual void setFlaps(const double pos);

    virtual void setGearHandleSwitch(const double sw);
    virtual void setSpeedBrakesSwitch(const double sw);
    virtual void setWeaponBayDoorSwitch(const double sw);

    // Sets the commanded wing sweep angle (radians)
    virtual void setCmdWingSweepAngle(const double rad);

    virtual unsigned int getMajorType() const override;

    virtual void reset() override;

protected:
   AerodynamicsModel* getAerodynamicsModel();
   const AerodynamicsModel* getAerodynamicsModel() const;

   bool setInitGearPos(const base::String* const pos);
   bool setInitGearPos(const base::Number* const pos);

private:
   double initGearPos;     // initial gear position
   double gearPos;         // Percent Gear position (0 -> up; 100 -> down)
   double wpnBayDoorPos;   // Percent Weapon bay door position (0 -> closed; 100 -> open)
   double wingSweep;       // Wing sweep angle (radians; zero for no wing sweep)
};


//------------------------------------------------------------------------------
// Class Aircraft
// Factory name: Aircraft
//------------------------------------------------------------------------------
class Aircraft : public AirVehicle
{
    DECLARE_SUBCLASS(Aircraft, AirVehicle)

public:
    Aircraft();
};


//------------------------------------------------------------------------------
// Class Helicopter
// Factory name: Helicopter
//------------------------------------------------------------------------------
class Helicopter : public AirVehicle
{
    DECLARE_SUBCLASS(Helicopter, AirVehicle)

public:
    Helicopter();
};


//------------------------------------------------------------------------------
// Class UnmannedAirVehicle
// Factory name: UnmannedAirVehicle
//------------------------------------------------------------------------------
class UnmannedAirVehicle : public AirVehicle
{
    DECLARE_SUBCLASS(UnmannedAirVehicle, AirVehicle)

public:
    UnmannedAirVehicle();
};

}
}

#endif
