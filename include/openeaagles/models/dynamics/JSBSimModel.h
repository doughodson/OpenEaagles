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

    virtual double getGload() const override;
    virtual double getMach() const override;
    virtual double getAngleOfAttack() const override;
    virtual double getSideSlip() const override;
    virtual double getFlightPath() const override;
    virtual double getCalibratedAirspeed() const override;
    virtual double getGrossWeight() const override;
    virtual double getFuelWt() const override;
    virtual double getFuelWtMax() const override;
    virtual double getSpeedBrakePosition() const override;
    virtual double getLandingGearPosition() const override;
    virtual bool isWeightOnWheels() const override;
    virtual int getNumberOfEngines() const override;
    virtual int getEngThrust(double* const fn, const int max) const override;
    virtual int getEngRPM(double* const rpm, const int max) const override;
    virtual int getEngFuelFlow(double* const ff, const int max) const override;
    virtual int getEngOilPressure(double* const oil, const int max) const override;    // Pressure: psi
    virtual int getEngInletTemp(double* const tmp, const int max) const override;      // Temp: C
    virtual int getEngNozzle(double* const noz, const int max) const override;         // Nozzle Pos: %
    virtual int getEngPLA(double* const pla, const int max) const override;
    virtual void setControlStickRollInput(const double roll) override;
    virtual void setControlStickPitchInput(const double pitch) override;
    virtual void setTrimSwitchRollInput(const double rollTrim) override;
    virtual void setTrimSwitchPitchInput(const double pitchTrim) override;
    virtual void setGearHandleSwitch(const double sw) override;
    virtual void setSpeedBrakesSwitch(const double sw) override;
    virtual void setBrakes(const double left, const double right) override;
    virtual int setThrottles(const double* const positions, const int num) override;
    virtual  void setRudderPedalInput(const double pedal) override;

    virtual void reset() override;

    virtual void dynamics(const double  dt = 0.0) override;

    virtual bool isHeadingHoldOn() const override;
    virtual double getCommandedHeadingD() const override;
    virtual bool setHeadingHoldOn(const bool b) override;
    virtual bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) override;

    virtual bool isVelocityHoldOn() const override;
    virtual double getCommandedVelocityKts() const override;
    virtual bool setVelocityHoldOn(const bool b) override;
    virtual bool setCommandedVelocityKts(const double v, const double vNps = 0) override;

    virtual bool isAltitudeHoldOn() const override;
    virtual double getCommandedAltitude() const override;
    virtual bool setAltitudeHoldOn(const bool b) override;
    virtual bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) override;

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
