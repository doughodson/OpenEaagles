//------------------------------------------------------------------------------
// Class: LandingLight
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_LandingLight_H__
#define __Eaagles_Instruments_LandingLight_H__

#include "LandingGear.h"

namespace Eaagles {
namespace Basic { class Identifier; }
namespace Instruments {

//------------------------------------------------------------------------------
// Class: LandingLight
//
// Description: Generic landing gear lights, which use the same principle as
// the landing gear, only they don't show a transitional phase, just off
// or on.
//
// Public member functions:
//      setLightRadius() - sets the radius (inches) of the light
//------------------------------------------------------------------------------
class LandingLight : public LandingGear
{
    DECLARE_SUBCLASS(LandingLight,LandingGear)

public:
    LandingLight();

    LCreal getLightRadius() const { return lRadius; }

    virtual bool setLightRadius(const LCreal newLR);

    void updateData(const LCreal dt = 0.0) override;

    void drawFunc() override;

protected:
    // slot functions
    bool setSlotLightRadius(const Basic::Number* const newLR);

private:
    LCreal lRadius;     // our light radius
    LCreal gearCurrent; // our gear current value
    SendData selSD;     // our selection
};

}  // end Instruments namespace
}  // end Eaagles namespace

#endif
