//------------------------------------------------------------------------------
// Class: JSBSimModel
// Description: JSBSim Model
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dynamics_JSBSimModel_H__
#define __Eaagles_Dynamics_JSBSimModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/basic/String.h"

namespace JSBSim {
    class FGFDMExec;
    class FGPropertyManager;
}

namespace Eaagles {

namespace Basic {
    class String;
    class Integer;
};

namespace Dynamics {

class JSBSimModel : public Simulation::AerodynamicsModel
{
    DECLARE_SUBCLASS(JSBSimModel,Simulation::AerodynamicsModel)

public:
    JSBSimModel();

    LCreal getGload() const override;
    LCreal getMach() const override;
    LCreal getAngleOfAttack() const override;
    LCreal getSideSlip() const override;
    LCreal getFlightPath() const override;
    LCreal getCalibratedAirspeed() const override;
    LCreal getGrossWeight() const override;
    LCreal getFuelWt() const override;
    LCreal getFuelWtMax() const override;
    LCreal getSpeedBrakePosition() const override;
    LCreal getLandingGearPosition() const override;
    bool isWeightOnWheels() const override;
    int getNumberOfEngines() const override;
    int getEngThrust(LCreal* const fn, const int max) const override;
    int getEngRPM(LCreal* const rpm, const int max) const override;
    int getEngFuelFlow(LCreal* const ff, const int max) const override;
    int getEngOilPressure(LCreal* const oil, const int max) const override;    // Pressure: psi
    int getEngInletTemp(LCreal* const tmp, const int max) const override;      // Temp: C
    int getEngNozzle(LCreal* const noz, const int max) const override;         // Nozzle Pos: %
    int getEngPLA(LCreal* const pla, const int max) const override;
    void setControlStickRollInput(const LCreal roll) override;
    void setControlStickPitchInput(const LCreal pitch) override;
    void setTrimSwitchRollInput(const LCreal rollTrim) override;
    void setTrimSwitchPitchInput(const LCreal pitchTrim) override;
    void setGearHandleSwitch(const LCreal sw) override;
    void setSpeedBrakesSwitch(const LCreal sw) override;
    void setBrakes(const LCreal left, const LCreal right) override;
    int setThrottles(const LCreal* const positions, const int num) override;
    void setRudderPedalInput(const LCreal pedal) override;

    void reset() override;

    void dynamics(const LCreal  dt = 0.0) override;

    bool isHeadingHoldOn() const override;
    double getCommandedHeadingD() const override;
    bool setHeadingHoldOn(const bool b) override;
    bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) override;

    bool isVelocityHoldOn() const override;
    double getCommandedVelocityKts() const override;
    bool setVelocityHoldOn(const bool b) override;
    bool setCommandedVelocityKts(const double v, const double vNps = 0) override;

    bool isAltitudeHoldOn() const override;
    double getCommandedAltitude() const override;
    bool setAltitudeHoldOn(const bool b) override;
    bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) override;

    // slot methods
    const Basic::String* getRootDir() const  { return rootDir;    }   // JSBSim root directory
    virtual bool setRootDir(const Basic::String* const);
    const Basic::String* getModel() const    { return model;      }   // JSBSim model
    virtual bool setModel(const Basic::String* const);
    int getDebugLevel() const                { return debugLevel; }   // JSBSim debug level
    virtual bool setDebugLevel(const Basic::Integer* const);

protected:

    JSBSim::FGFDMExec* fdmex;
    JSBSim::FGPropertyManager* propMgr;

private:
    void initData();

    const Basic::String* rootDir;   // root directory for JSBSim models
    const Basic::String* model;     // JSBSim model
    int   debugLevel;

    LCreal pitchTrimPos;    // +/- 1.0
    LCreal pitchTrimRate;   // maxVal(1.0) per sec
    LCreal pitchTrimSw;
    LCreal rollTrimPos;     // +/- 1.0
    LCreal rollTrimRate;    // maxVal(1.0) per sec
    LCreal rollTrimSw;

    bool        headingHoldOn;
    bool        altitudeHoldOn;
    bool        velocityHoldOn;
    double      commandedHeadingDeg;
    double      commandedAltitudeFt;
    double      commandedVelocityKts;
    bool        hasHeadingHold;
    bool        hasVelocityHold;
    bool        hasAltitudeHold;
};

} // End Dynamics namespace
} // End Eaagles namespace

#endif
