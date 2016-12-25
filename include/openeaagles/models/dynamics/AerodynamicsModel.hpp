
#ifndef __oe_models_AerodynamicsModel_H__
#define __oe_models_AerodynamicsModel_H__

#include "openeaagles/models/dynamics/DynamicsModel.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class AerodynamicsModel
//
// Description:  Aerodynamics model for air vehicles and weapons
//
// Factory name: AerodynamicsModel
//------------------------------------------------------------------------------
class AerodynamicsModel : public DynamicsModel
{
    DECLARE_SUBCLASS(AerodynamicsModel, DynamicsModel)

public:
    AerodynamicsModel();

    // Vehicle data
    virtual double getGload() const;                    // G's (+up, one at level)
    virtual double getMach() const;                     // mach number
    virtual double getAngleOfAttack() const;            // radians
    virtual double getSideSlip() const;                 // radians
    virtual double getFlightPath() const;               // radians
    virtual double getCalibratedAirspeed() const;       // Kts
    virtual double getSpeedBrakePosition() const;       // % (0% retracted; 100% fully extended)
    virtual double getLandingGearPosition() const;      // % (0% up; 100% down)
    virtual double getWeaponBayDoorPosition() const;    // % (0% closed; 100% open)
    virtual double getAmbientPressureRatio() const;     // Air pressure ratio
    virtual double getWingSweepAngle() const;           // radians (or zero for no wing sweep)
    virtual bool   isWeightOnWheels() const;            // True if weight is on wheels

    // Unit specific vehicle data
    virtual double getAngleOfAttackR() const;
    virtual double getAngleOfAttackD() const;
    virtual double getFlightPathR() const;
    virtual double getFlightPathD() const;
    virtual double getSideSlipR() const;
    virtual double getSideSlipD() const;

    // Engine data
    virtual int getEngRPM(double* const rpm, const int max) const;            // (N2) Core %RPM
    virtual int getEngN1(double* const n1, const int max) const;              // N1 %RPM
    virtual int getEngFuelFlow(double* const ff, const int max) const;        // Rate: lbs/hour  (pph)
    virtual int getEngOilPressure(double* const oil, const int max) const;    // Pressure: psi
    virtual int getEngInletTemp(double* const tmp, const int max) const;      // Temp: C
    virtual int getEngNozzle(double* const noz, const int max) const;         // Nozzle Pos: %
    virtual int getEngPLA(double* const pla, const int max) const;            // PLA: degrees
    virtual int getEngThrustIdle(double* const fn, const int max) const;      // Thrust if at Idle: lbs
    virtual int getEngThrustMil(double* const fn, const int max) const;       // Thrust if at MIL: lbs
    virtual int getEngThrustAb(double* const fn, const int max) const;        // Thrust if at Full afterburner: lbs

    // Trim switch inputs
    virtual void setTrimSwitchRollInput(const double rollTrim);
    virtual void setTrimSwitchPitchInput(const double pitchTrim);
    void setTrimSwitch(const double rollTrim, const double pitchTrim) {
        setTrimSwitchRollInput(rollTrim);
        setTrimSwitchPitchInput(pitchTrim);
    }

    virtual void setRudderPedalInput(const double pedal);
    virtual void setFlaps(const double pos);

    virtual void setGearHandleSwitch(const double sw);
    virtual void setSpeedBrakesSwitch(const double sw);
    virtual void setWeaponBayDoorSwitch(const double sw);

    // Sets the commanded wing sweep angle (radians)
    virtual void setCmdWingSweepAngle(const double rad);

};

}
}

#endif
