//------------------------------------------------------------------------------
// Class: JSBSimModel
// Description: JSBSim Model
//------------------------------------------------------------------------------
#ifndef __oe_models_JSBSimModel_H__
#define __oe_models_JSBSimModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"
#include "openeaagles/base/String.h"

namespace JSBSim {
    class FGFDMExec;
    class FGPropertyManager;
}

namespace oe {

namespace base {
    class String;
    class Integer;
};

namespace models {

class JSBSimModel : public simulation::AerodynamicsModel
{
    DECLARE_SUBCLASS(JSBSimModel,simulation::AerodynamicsModel)

public:
    JSBSimModel();

    double getGload() const override;
    double getMach() const override;
    double getAngleOfAttack() const override;
    double getSideSlip() const override;
    double getFlightPath() const override;
    double getCalibratedAirspeed() const override;
    double getGrossWeight() const override;
    double getFuelWt() const override;
    double getFuelWtMax() const override;
    double getSpeedBrakePosition() const override;
    double getLandingGearPosition() const override;
    bool isWeightOnWheels() const override;
    int getNumberOfEngines() const override;
    int getEngThrust(double* const fn, const int max) const override;
    int getEngRPM(double* const rpm, const int max) const override;
    int getEngFuelFlow(double* const ff, const int max) const override;
    int getEngOilPressure(double* const oil, const int max) const override;    // Pressure: psi
    int getEngInletTemp(double* const tmp, const int max) const override;      // Temp: C
    int getEngNozzle(double* const noz, const int max) const override;         // Nozzle Pos: %
    int getEngPLA(double* const pla, const int max) const override;
    void setControlStickRollInput(const double roll) override;
    void setControlStickPitchInput(const double pitch) override;
    void setTrimSwitchRollInput(const double rollTrim) override;
    void setTrimSwitchPitchInput(const double pitchTrim) override;
    void setGearHandleSwitch(const double sw) override;
    void setSpeedBrakesSwitch(const double sw) override;
    void setBrakes(const double left, const double right) override;
    int setThrottles(const double* const positions, const int num) override;
    void setRudderPedalInput(const double pedal) override;

    void reset() override;

    void dynamics(const double  dt = 0.0) override;

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
    const base::String* getRootDir() const  { return rootDir;    }   // JSBSim root directory
    virtual bool setRootDir(const base::String* const);
    const base::String* getModel() const    { return model;      }   // JSBSim model
    virtual bool setModel(const base::String* const);
    int getDebugLevel() const                { return debugLevel; }   // JSBSim debug level
    virtual bool setDebugLevel(const base::Integer* const);

protected:

    JSBSim::FGFDMExec* fdmex;
    JSBSim::FGPropertyManager* propMgr;

private:
    void initData();

    const base::String* rootDir;   // root directory for JSBSim models
    const base::String* model;     // JSBSim model
    int   debugLevel;

    double pitchTrimPos;    // +/- 1.0
    double pitchTrimRate;   // maxVal(1.0) per sec
    double pitchTrimSw;
    double rollTrimPos;     // +/- 1.0
    double rollTrimRate;    // maxVal(1.0) per sec
    double rollTrimSw;

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

}
}

#endif
