
#ifndef __oe_instruments_BearingPointer_H__
#define __oe_instruments_BearingPointer_H__

#include "openeaagles/instruments/maps/CompassRose.hpp"

namespace oe {
namespace base { class Angle; class Number; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: BearingPointer
//
// Description: This is a bearing pointer, which works in conjunction with
// a compass rose to determine bearing, however, it is independent of CompassRose,
// and can be used as a stand-alone item.  You can also add a head and tail
// graphic to the pointer, to customize it.  It defaults to a standard 0.5 inch
// wide bearing pointer with a line for a tail.
//
// Inputs:
//      UPDATE_VALUE 1 - 6 (from Compass rose) -> handles centered, radius, and such
//      UPDATE_VALUE7 - sets bearing (radians or base::Angle)
//      UPDATE_VALUE8 - sets bearing (degrees)
//------------------------------------------------------------------------------
class BearingPointer : public CompassRose
{
    DECLARE_SUBCLASS(BearingPointer, CompassRose)

public:
    BearingPointer();

    double getBearingRad() const { return bearing; } // radians
    double getBearingDeg() const { return bearing * static_cast<double>(base::angle::R2DCC); }    // degrees
    graphics::Graphic* getHeadGraphic() const { return head; }
    graphics::Graphic* getTailGraphic() const { return tail; }

    bool setBearingRad(const double newB);             // radians
    bool setBearingDeg(const double newB);             // degrees

    virtual void drawFunc() override;
    virtual void draw() override;

    virtual void updateData(const double dt = 0) override;
    virtual bool event(const int key, base::Object* const obj = nullptr) override;

protected:
    // slot functions
    virtual bool setSlotHeadGraphic(const graphics::Graphic* const newH);
    virtual bool setSlotTailGraphic(const graphics::Graphic* const newT);

private:
    // event handlers
    bool onUpdateRadBearingPointer(const base::Angle* const msg);
    bool onUpdateRadBearingPointer(const base::Number* const msg);
    bool onUpdateDegBearingPointer(const base::Number* const msg);

    double bearing {};     // used for bearing pointer, or if anyone else needs it for calculations (radians)
    double myRotation {};  // how much we are going to rotate the compass
    double myRadius {};    // our radius (comes from dial radius)
    graphics::Graphic* head {};    // our head graphic (if we have one)
    graphics::Graphic* tail {};    // our tail graphic (if we have one)
};

}
}

#endif
