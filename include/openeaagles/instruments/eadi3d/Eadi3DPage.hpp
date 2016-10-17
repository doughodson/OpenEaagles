
#ifndef __oe_instruments_Eadi3DPage_H__
#define __oe_instruments_Eadi3DPage_H__

#include "openeaagles/graphics/Page.hpp"
#include "openeaagles/instruments/eadi3d/Eadi3DObjects.hpp"

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Eadi3DPage
//------------------------------------------------------------------------------
class Eadi3DPage : public graphics::Page
{
    DECLARE_SUBCLASS(Eadi3DPage, graphics::Page)

public:
    Eadi3DPage();

    void setAltitude(const double alt);                 // feet
    void setAirspeed(const double spd);                 // knots
    void setHeading(const double hdg);                  // deg
    void setAOA(const double aoa);                      // deg
    void setVVI(const double vvi);                      // ft/min
    void setPitch(const double pitch);                  // deg
    void setRoll(const double roll);                    // deg
    void setMach(const double mach);                    // mach number
    void setGLoad(const double gload);                  // Gs
    void setLandingMode(const bool landMode);           // T = landing mode
    void setPitchSteeringCmd(const double pitchCmd);    // [-1, 1]
    void setRollSteeringCmd(const double rollCmd);      // [-1, 1]
    void setPitchSteeringValid(const bool psValid);     // T = valid
    void setRollSteeringValid(const bool rcValid);      // T = valid
    void setGlideslopeDev(const double gsDev);          // dots [-2, 2]
    void setLocalizerDev(const double locDev);          // dots [-2, 2]
    void setTurnRate(const double turnRate);            // dots [-2, 2]
    void setSlipInd(const double slipInd);              // dots [-2, 2]
    void setGlideslopeValid(const bool gsValid);        // T = valid
    void setLocalizerValid(const bool locValid);        // T = valid

    virtual void draw() override;

    virtual bool event(const int event, base::Object* const obj = nullptr) override;

private:
    // event functions
    bool onEventSetAltitude(const base::Number* const x);
    bool onEventSetAirspeed(const base::Number* const x);
    bool onEventSetHeading(const base::Number* const x);
    bool onEventSetAOA(const base::Number* const x);
    bool onEventSetVVI(const base::Number* const x);
    bool onEventSetPitch(const base::Number* const x);
    bool onEventSetRoll(const base::Number* const x);
    bool onEventSetMach(const base::Number* const x);
    bool onEventSetGLoad(const base::Number* const x);
    bool onEventSetLandingMode(const base::Number* const x);
    bool onEventSetPitchSteeringCmd(const base::Number* const x);
    bool onEventSetRollSteeringCmd(const base::Number* const x);
    bool onEventSetPitchSteeringValid(const base::Number* const x);
    bool onEventSetRollSteeringValid(const base::Number* const x);
    bool onEventSetGlideslopeDev(const base::Number* const x);
    bool onEventSetLocalizerDev(const base::Number* const x);
    bool onEventSetTurnRate(const base::Number* const x);
    bool onEventSetSlipInd(const base::Number* const x);
    bool onEventSetGlideslopeValid(const base::Number* const x);
    bool onEventSetLocalizerValid(const base::Number* const x);

    void globeBall(double pitch, double roll, double pitchCmd, double rollCmd, bool psValid, bool rcValid, bool landMode);
    void scales(double gsDev, double locDev, double turnRate, double slipInd, bool gsValid, bool locValid, bool landMode);
    void windows(double cas, double alt, double aoa, double mach, double vvi, const char* airSpeedType, double gload);
    void heading(double hdg, double hdgCmd);
    void background();

    double radius;
    Eadi3DObjects eadiObjs;

    double altitudeFT;
    double airspeedKTS;
    double headingDEG;
    double aoaDEG;
    double vviFPM;
    double pitchDEG;
    double rollDEG;
    double machNo;

    double Gload;
    bool   landingMode;
    double pitchSteeringCmd;
    double rollSteeringCmd;
    bool   pitchSteeringValid;
    bool   rollSteeringValid;
    double glideslopeDevDOTS;
    double localizerDevDOTS;
    double turnRateDOTS;
    double slipIndDOTS;
    bool   glideslopeDevValid;
    bool   localizerDevValid;
};

}
}

#endif
