//------------------------------------------------------------------------------
// Class: CompassRose
// Base Class: basic::Object -> graphics::Graphic -> CompassRose
//
// Description: Generic compass used as a direction indicator.  It will
// rotate about a heading either by someone telling it through a member function.
// Inputs:
//      UPDATE_VALUE  -> rotation angle (deg)
//      UPDATE_VALUE2 -> rotation angle (rad)
//      UPDATE_VALUE3 -> centered radius
//      UPDATE_VALUE4 -> decentered radius
//      UPDATE_VALUE5 -> displacement
//      UPDATE_VALUE6 -> centered or not
//------------------------------------------------------------------------------
#ifndef __oe_instruments_CompassRose_H__
#define __oe_instruments_CompassRose_H__

#include "openeaagles/graphics/Graphic.h"
#include "openeaagles/base/units/Angles.h"

namespace oe {
namespace instruments {

class CompassRose : public graphics::Graphic
{
    DECLARE_SUBCLASS(CompassRose, graphics::Graphic)

public:
    CompassRose();

    virtual bool setRotationDeg(const LCreal newR);
    virtual bool setRotationRad(const LCreal newR);
    virtual bool setCenteredRadius(const LCreal newR);
    virtual bool setDeCenteredRadius(const LCreal newDR);
    virtual bool setDisplacement(const LCreal newD);
    virtual bool setCentered(const bool newC);

    LCreal getRotationDeg() const      { return rot * static_cast<LCreal>(basic::Angle::R2DCC); }
    LCreal getRotationRad() const      { return rot; }
    LCreal getCenteredRadius() const   { return cenRadius; }
    LCreal getDeCenteredRadius() const { return decRadius; }
    bool isCentered() const            { return centered; }
    LCreal getDisplacement() const     { return displacement; }

    void draw() override;

    void updateData(const LCreal dt = 0.0) override;
    bool event(const int event, basic::Object* const obj = nullptr) override;

protected:
    // slot functions
    bool setSlotCenteredRadius(const basic::Number* const newR);
    bool setSlotDeCenteredRadius(const basic::Number* const newR);
    bool setSlotDisplacement(const basic::Number* const newD);

    // event functions
    bool onUpdateRotDeg(const basic::Number* const x);
    bool onUpdateRadius(const basic::Number* const x);
    bool onUpdateCenRad(const basic::Number* const x);
    bool onUpdateDecRadius(const basic::Number* const x);
    bool onUpdateDisplacement(const basic::Number* const x);
    bool onUpdateCentered(const basic::Number* const x);

private:
    LCreal rot;         // rotation angle (rads)
    LCreal cenRadius;   // our centered radius (inches)
    LCreal decRadius;   // our decentered radius (inches)
    bool centered;      // centered flag
    LCreal displacement;// how far to translate when we go decentered (inches)

    SendData lTicksSD;
    SendData sTicksSD;
    SendData dialSD;
};

}  // end instruments namespace
}  // end oe namespace

#endif
