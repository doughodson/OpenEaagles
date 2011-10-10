//------------------------------------------------------------------------------
// Class:	JSBSimModel
// Description: JSBSim Model
//  
//------------------------------------------------------------------------------
#ifndef __Eaagles_Vehicle_JSBSimModel_H__
#define __Eaagles_Vehicle_JSBSimModel_H__

#include "openeaagles/simulation/DynamicsModels.h"
#include "openeaagles/basic/String.h"

namespace JSBSim {
    class FGFDMExec;
    class FGPropertyManager;
}

namespace Eaagles {

namespace Basic {
    class String;
};

namespace Vehicle {

class JSBSimModel : public Simulation::AerodynamicsModel
{
    DECLARE_SUBCLASS(JSBSimModel,Simulation::AerodynamicsModel)
public:
    JSBSimModel();

    // Vehicle interface
    virtual LCreal getGload() const;
    virtual LCreal getMach() const;
    virtual LCreal getAngleOfAttack() const;
    virtual LCreal getSideSlip() const;
    virtual LCreal getFlightPath() const;
    virtual LCreal getCalibratedAirspeed() const; 
    virtual LCreal getGrossWeight() const;
    virtual LCreal getFuelWt() const;
    virtual LCreal getFuelWtMax() const;
    virtual LCreal getSpeedBrakePosition() const;
    virtual LCreal getLandingGearPosition() const;
    virtual bool   isWeightOnWheels() const;
    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(LCreal* const fn, const int max) const;
    virtual int getEngRPM(LCreal* const rpm, const int max) const;
    virtual int getEngFuelFlow(LCreal* const ff, const int max) const;
    virtual int getEngOilPressure(LCreal* const oil, const int max) const;    // Pressure: psi
    virtual int getEngInletTemp(LCreal* const tmp, const int max) const;      // Temp: C
    virtual int getEngNozzle(LCreal* const noz, const int max) const;         // Nozzle Pos: %
    virtual int getEngPLA(LCreal* const pla, const int max) const;
    virtual void setControlStickRollInput(const LCreal roll);
    virtual void setControlStickPitchInput(const LCreal pitch);
    virtual void setTrimSwitchRollInput(const LCreal rollTrim);
    virtual void setTrimSwitchPitchInput(const LCreal pitchTrim);
    virtual void setGearHandleSwitch(const LCreal sw);
    virtual void setSpeedBrakesSwitch(const LCreal sw);
    virtual void setBrakes(const LCreal left, const LCreal right);
    virtual int setThrottles(const LCreal* const positions, const int num);
    virtual void setRudderPedalInput(const LCreal pedal);

    const Basic::String* getRootDir() const { return rootDir; }	// JSBSim root directory
    virtual bool setRootDir(Basic::String* const dir);			// 

    const Basic::String* getModel() const { return model; }	// JSBSim model
    virtual bool setModel(Basic::String* const msl);			// 

    // DynamicsModel interface
    virtual void dynamics(const LCreal  dt = 0.0);

    // Component interface
    virtual void reset();

    // DynamicsModel interface
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

private:
    JSBSim::FGFDMExec* fdmex;
    JSBSim::FGPropertyManager* propMgr;

    Basic::String* rootDir;         // root directory for JSBSim models
    Basic::String* model;           // JSBSim model

    LCreal	pitchTrimPos;		// +/- 1.0
    LCreal	pitchTrimRate;		// maxVal(1.0) per sec
    LCreal	pitchTrimSw;
    LCreal	rollTrimPos;		// +/- 1.0
    LCreal	rollTrimRate;		// maxVal(1.0) per sec
    LCreal	rollTrimSw;

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

} // End Vehicle namespace
} // End Eaagles namespace

#endif
