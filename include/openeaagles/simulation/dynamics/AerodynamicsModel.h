//------------------------------------------------------------------------------
// Class: AerodynamicsModel
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_AerodynamicsModel_H__
#define __Eaagles_Simulation_AerodynamicsModel_H__

#include "openeaagles/simulation/DynamicsModel.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class AerodynamicsModel
//
// Description:  Aerodynamics model for air vehicles and weapons
//
// Factory name: AerodynamicsModel
//==============================================================================
class AerodynamicsModel : public DynamicsModel
{
    DECLARE_SUBCLASS(AerodynamicsModel, DynamicsModel)

public:
    AerodynamicsModel();

    // Vehicle data
    virtual LCreal getGload() const;                    // G's (+up, one at level)
    virtual LCreal getMach() const;                     // mach number
    virtual LCreal getAngleOfAttack() const;            // radians
    virtual LCreal getSideSlip() const;                 // radians
    virtual LCreal getFlightPath() const;               // radians
    virtual LCreal getCalibratedAirspeed() const;       // Kts
    virtual LCreal getSpeedBrakePosition() const;       // % (0% retracted; 100% fully extended)
    virtual LCreal getLandingGearPosition() const;      // % (0% up; 100% down)
    virtual LCreal getWeaponBayDoorPosition() const;    // % (0% closed; 100% open)
    virtual LCreal getAmbientPressureRatio() const;     // Air pressure ratio
    virtual LCreal getWingSweepAngle() const;           // radians (or zero for no wing sweep)
    virtual bool   isWeightOnWheels() const;            // True if weight is on wheels

    // Unit specific vehicle data
    virtual LCreal getAngleOfAttackR() const;
    virtual LCreal getAngleOfAttackD() const;
    virtual LCreal getFlightPathR() const;
    virtual LCreal getFlightPathD() const;
    virtual LCreal getSideSlipR() const;
    virtual LCreal getSideSlipD() const;

    // Engine data
    virtual int getEngRPM(LCreal* const rpm, const int max) const;            // (N2) Core %RPM
    virtual int getEngN1(LCreal* const n1, const int max) const;              // N1 %RPM
    virtual int getEngFuelFlow(LCreal* const ff, const int max) const;        // Rate: lbs/hour  (pph)
    virtual int getEngOilPressure(LCreal* const oil, const int max) const;    // Pressure: psi
    virtual int getEngInletTemp(LCreal* const tmp, const int max) const;      // Temp: C
    virtual int getEngNozzle(LCreal* const noz, const int max) const;         // Nozzle Pos: %
    virtual int getEngPLA(LCreal* const pla, const int max) const;            // PLA: degrees
    virtual int getEngThrustIdle(LCreal* const fn, const int max) const;      // Thrust if at Idle: lbs
    virtual int getEngThrustMil(LCreal* const fn, const int max) const;       // Thrust if at MIL: lbs
    virtual int getEngThrustAb(LCreal* const fn, const int max) const;        // Thrust if at Full afterburner: lbs

    // Trim switch inputs
    virtual void setTrimSwitchRollInput(const LCreal rollTrim);
    virtual void setTrimSwitchPitchInput(const LCreal pitchTrim);
    void setTrimSwitch(const LCreal rollTrim, const LCreal pitchTrim) {
        setTrimSwitchRollInput(rollTrim);
        setTrimSwitchPitchInput(pitchTrim);
    }

    virtual void setRudderPedalInput(const LCreal pedal);
    virtual void setFlaps(const LCreal pos);

    virtual void setGearHandleSwitch(const LCreal sw);
    virtual void setSpeedBrakesSwitch(const LCreal sw);
    virtual void setWeaponBayDoorSwitch(const LCreal sw);

    // Sets the commanded wing sweep angle (radians)
    virtual void setCmdWingSweepAngle(const LCreal rad);

};

} // End Simulation namespace
} // End Eaagles namespace

#endif
