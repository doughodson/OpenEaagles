//------------------------------------------------------------------------------
// Class:       RotarySwitch
// Base class:  base::Object -> base::Component -> graphics::Graphic -> Button -> RotarySwitch
// Description: Generic multi-position selector switch that knows when it is left or right
//                mouse clicked and moves to next or previous switch position.
//------------------------------------------------------------------------------
#ifndef __oe_instruments_RotarySwitch_H__
#define __oe_instruments_RotarySwitch_H__

#include "openeaagles/instruments/buttons/Button.hpp"

namespace oe {
namespace instruments {

class RotarySwitch : public Button
{
    DECLARE_SUBCLASS(RotarySwitch,Button)

public:
   RotarySwitch();

   // Get current switch position
   int getCurrentPosition() const         { return currentPosition; };

   virtual bool onSingleClick() override;

   virtual void updateData(const double dt = 0.0) override;

protected:
    bool setSlotAngles(const base::PairStream* const stream);
    bool setSlotStartPosition(const base::Number* const x);

private:
    static const int MAX_ANGLES = 500;

    int currentPosition;    //current switch position
    double angles[MAX_ANGLES];
    int numAngs;
    int startPosition;
    SendData angleSD;       // angle to send to our rotator
};

}
}

#endif
