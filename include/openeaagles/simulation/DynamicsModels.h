//------------------------------------------------------------------------------
// Classes: DynamicsModel, AerodynamicsModel
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_DynamicsModel_H__
#define __Eaagles_Simulation_DynamicsModel_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class DynamicsModel
//
// Description:  Base dynamics model class
//
// Note -- 
//
//    1) updateTC() and updateData() are called by Component::upateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) To be properly synchronized within a frame, the root of our dynamics
//       model should be in our dynamics() function, which will called from
//       the Player class dynamics() function.
//
//    3) atReleaseInit() is called by Weapon::updateTC() to switch from
//       PRE_RELEASE mode to ACTIVE mode.  Default is to call reset().
//       Not used by air vehicles.
//
//    4) This class is one of the "top level" systems attached to a Player
//       class (see Player.h).
//
// Form name: DynamicsModel
//
//==============================================================================
class DynamicsModel : public Basic::Component  
{
    DECLARE_SUBCLASS(DynamicsModel,Basic::Component)

public:
    DynamicsModel();

    virtual void atReleaseInit();
    virtual void dynamics(const LCreal dt);

    virtual bool isHeadingHoldOn() const;
    virtual double getCommandedHeadingD() const;
    virtual bool setHeadingHoldOn(const bool b);
    virtual bool setCommandedHeadingD(const double h);

    virtual bool isVelocityHoldOn() const;
    virtual double getCommandedVelocityKts() const;
    virtual bool setVelocityHoldOn(const bool b);
    virtual bool setCommandedVelocityKts(const double a);
                                                                                
    virtual bool isAltitudeHoldOn() const;
    virtual double getCommandedAltitude() const;
    virtual bool setAltitudeHoldOn(const bool b);
    virtual bool setCommandedAltitude(const double a);

    virtual LCreal getFuelWt() const;                   // lbs
    virtual LCreal getFuelWtMax() const;                // lbs
    virtual LCreal getGrossWeight() const;              // wt: lbs

    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(LCreal* const fn, const int max) const;          // Thrust: lbs

    // Control Stick Input Positions (not the flight surfaces!)
    virtual void setControlStickRollInput(const LCreal roll);
    virtual void setControlStickPitchInput(const LCreal pitch);
    void setControlStick(const LCreal roll, const LCreal pitch) {
        setControlStickRollInput(roll);
        setControlStickPitchInput(pitch);
    }

    // ---
    // Set throttle positions; returns the actual number of throttle positions
    //    'positions' -> Array of throttle positions (for each throttle)
    //              < 0.0  -> Cutoff
    //              0.0    -> Idle
    //              1.0    -> MIL
    //              2.0    -> A/B
    //    'num '-> number of throttle positions to get/set
    // ---
    virtual int setThrottles(const LCreal* const positions, const int num);

    // ---
    // Sets brake positions (left & right)
    //         No brake force  -> 0.0
    //         Max brake force -> 1.0
    // ---
    virtual void setBrakes(const LCreal left, const LCreal right);

    // Sets the fuel weight
    virtual bool setFuelWt(const LCreal lbs);
};

//==============================================================================
// Class AerodynamicsModel
//
// Description:  Aerodynamics model for air vehicles and weapons
//
// Form name: AerodynamicsModel
//==============================================================================
class AerodynamicsModel : public DynamicsModel  
{
    DECLARE_SUBCLASS(AerodynamicsModel,DynamicsModel)

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

