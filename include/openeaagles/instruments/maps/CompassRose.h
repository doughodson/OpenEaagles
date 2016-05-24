//------------------------------------------------------------------------------
// Class: CompassRose
// Base Class: base::Object -> graphics::Graphic -> CompassRose
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

    virtual bool setRotationDeg(const double newR);
    virtual bool setRotationRad(const double newR);
    virtual bool setCenteredRadius(const double newR);
    virtual bool setDeCenteredRadius(const double newDR);
    virtual bool setDisplacement(const double newD);
    virtual bool setCentered(const bool newC);

    double getRotationDeg() const      { return rot * static_cast<double>(base::Angle::R2DCC); }
    double getRotationRad() const      { return rot; }
    double getCenteredRadius() const   { return cenRadius; }
    double getDeCenteredRadius() const { return decRadius; }
    bool isCentered() const            { return centered; }
    double getDisplacement() const     { return displacement; }

    virtual void draw() override;

    virtual void updateData(const double dt = 0.0) override;
    virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // slot functions
    bool setSlotCenteredRadius(const base::Number* const newR);
    bool setSlotDeCenteredRadius(const base::Number* const newR);
    bool setSlotDisplacement(const base::Number* const newD);

    // event functions
    bool onUpdateRotDeg(const base::Number* const x);
    bool onUpdateRadius(const base::Number* const x);
    bool onUpdateCenRad(const base::Number* const x);
    bool onUpdateDecRadius(const base::Number* const x);
    bool onUpdateDisplacement(const base::Number* const x);
    bool onUpdateCentered(const base::Number* const x);

private:
    double rot;         // rotation angle (rads)
    double cenRadius;   // our centered radius (inches)
    double decRadius;   // our decentered radius (inches)
    bool centered;      // centered flag
    double displacement;// how far to translate when we go decentered (inches)

    SendData lTicksSD;
    SendData sTicksSD;
    SendData dialSD;
};

}
}

#endif
