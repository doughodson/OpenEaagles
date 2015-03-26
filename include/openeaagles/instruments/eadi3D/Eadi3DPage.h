//------------------------------------------------------------------------------
// Class:  BasicGL::Page -> Eadi3DPage
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_Eadi3DPage_H__
#define __Eaagles_Instruments_Eadi3DPage_H__

#include "openeaagles/basicGL/Page.h"
#include "openeaagles/instruments/eadi3D/Eadi3DObjects.h"

namespace Eaagles {
namespace Instruments {

class Eadi3DPage : public BasicGL::Page{

   DECLARE_SUBCLASS(Eadi3DPage,BasicGL::Page)

public:

    Eadi3DPage();
    
    void setAltitude(const LCreal alt);                 // feet
    void setAirspeed(const LCreal spd);                 // knots
    void setHeading(const LCreal hdg);                  // deg
    void setAOA(const LCreal aoa);                      // deg
    void setVVI(const LCreal vvi);                      // ft/min
    void setPitch(const LCreal pitch);                  // deg
    void setRoll(const LCreal roll);                    // deg
    void setMach(const LCreal mach);                    // mach number
    void setGLoad(const LCreal gload);                  // Gs
    void setLandingMode(const bool landMode);           // T = landing mode
    void setPitchSteeringCmd(const LCreal pitchCmd);    // [-1, 1]
    void setRollSteeringCmd(const LCreal rollCmd);      // [-1, 1]
    void setPitchSteeringValid(const bool psValid);     // T = valid
    void setRollSteeringValid(const bool rcValid);      // T = valid
    void setGlideslopeDev(const LCreal gsDev);          // dots [-2, 2]
    void setLocalizerDev(const LCreal locDev);          // dots [-2, 2]
    void setTurnRate(const LCreal turnRate);            // dots [-2, 2]
    void setSlipInd(const LCreal slipInd);              // dots [-2, 2]
    void setGlideslopeValid(const bool gsValid);        // T = valid
    void setLocalizerValid(const bool locValid);        // T = valid

    // BasicGL::Graphic interface
    void draw() override;

    // Basic::Component interface
    bool event(const int event, Basic::Object* const obj = 0) override;

private:
    // event functions
    bool onEventSetAltitude(const Basic::Number* const x);
    bool onEventSetAirspeed(const Basic::Number* const x);
    bool onEventSetHeading(const Basic::Number* const x);
    bool onEventSetAOA(const Basic::Number* const x);
    bool onEventSetVVI(const Basic::Number* const x);
    bool onEventSetPitch(const Basic::Number* const x);
    bool onEventSetRoll(const Basic::Number* const x);
    bool onEventSetMach(const Basic::Number* const x);
    bool onEventSetGLoad(const Basic::Number* const x);
    bool onEventSetLandingMode(const Basic::Number* const x);
    bool onEventSetPitchSteeringCmd(const Basic::Number* const x);
    bool onEventSetRollSteeringCmd(const Basic::Number* const x);
    bool onEventSetPitchSteeringValid(const Basic::Number* const x);
    bool onEventSetRollSteeringValid(const Basic::Number* const x);
    bool onEventSetGlideslopeDev(const Basic::Number* const x);
    bool onEventSetLocalizerDev(const Basic::Number* const x);
    bool onEventSetTurnRate(const Basic::Number* const x);
    bool onEventSetSlipInd(const Basic::Number* const x);
    bool onEventSetGlideslopeValid(const Basic::Number* const x);
    bool onEventSetLocalizerValid(const Basic::Number* const x);

    void globeBall(LCreal pitch, LCreal roll, LCreal pitchCmd, LCreal rollCmd, bool psValid, bool rcValid, bool landMode);
    void scales(LCreal gsDev, LCreal locDev, LCreal turnRate, LCreal slipInd, bool gsValid, bool locValid, bool landMode);
    void windows(LCreal cas, LCreal alt, LCreal aoa, LCreal mach, LCreal vvi, const char* airSpeedType, LCreal gload);
    void heading(LCreal hdg, LCreal hdgCmd);
    void background();

    LCreal radius;
    Eadi3DObjects eadiObjs;

    LCreal altitudeFT;
    LCreal airspeedKTS;
    LCreal headingDEG;
    LCreal aoaDEG;
    LCreal vviFPM;
    LCreal pitchDEG;
    LCreal rollDEG;
    LCreal machNo;

    LCreal Gload;
    bool   landingMode;
    LCreal pitchSteeringCmd;
    LCreal rollSteeringCmd;
    bool   pitchSteeringValid;
    bool   rollSteeringValid;
    LCreal glideslopeDevDOTS;
    LCreal localizerDevDOTS;
    LCreal turnRateDOTS;
    LCreal slipIndDOTS;
    bool   glideslopeDevValid;
    bool   localizerDevValid;
};

}; // end of Instruments namespace
}; // end of Eaagles namespace

#endif
