
#ifndef __oe_instruments_RotarySwitch_H__
#define __oe_instruments_RotarySwitch_H__

#include "openeaagles/instruments/buttons/Button.hpp"
#include <array>

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class:       RotarySwitch
// Description: Generic multi-position selector switch that knows when it is left or right
//                mouse clicked and moves to next or previous switch position.
//------------------------------------------------------------------------------
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

    int currentPosition {1};    //current switch position
    std::array<double, MAX_ANGLES> angles {};
    int numAngs {};
    int startPosition {1};
    SendData angleSD;       // angle to send to our rotator
};

}
}

#endif
