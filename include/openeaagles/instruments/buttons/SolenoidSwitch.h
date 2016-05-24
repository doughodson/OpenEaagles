//------------------------------------------------------------------------------
// Class: SolenoidSwitch
//
// Description: This is a custom button manager, which will hold 3 buttons, and
// will time when the user holds up or down from the center on the button, and will
// switch at that time.  A click on the center button will simply return to the
// center position (no holding).
// You can retrieve the button position at any time, or
//------------------------------------------------------------------------------
#ifndef __oe_instruments_SolenoidSwitch_H__
#define __oe_instruments_SolenoidSwitch_H__

#include "openeaagles/graphics/Graphic.h"
#include "openeaagles/instruments/buttons/Button.h"

namespace oe {
namespace base { class UpTimer; }
namespace instruments {

class SolenoidSwitch : public graphics::Graphic
{
    DECLARE_SUBCLASS(SolenoidSwitch,graphics::Graphic)

public:
    SolenoidSwitch();

    virtual bool setHoldTimer(const double x);

    // button enums
    enum { CENTER_BUTTON = 1, TOP_BUTTON = 2, BOTTOM_BUTTON = 3, NUM_BUTTONS = 3 };

    // set functions for setting latched
    void setOkToLatch(bool x)   { latched = x; }
    bool okToLatch() const      { return latched; }

    // hit when our buttons are pressed and we are trying to "latch" the switch in place
    // (either by timer or logic)
    void latch(const int buttonId);
    // button is release, switch will determine if latch will "stick"
    void determineLatch();
    // return to initial spot
    void resetButton();
    // get function
    int getSwitchPosition()   { return currButtonId; }
    base::UpTimer* getTimer() { return timer; }

    virtual void updateData(const double dt = 0.0) override;
    virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // event function
    bool selectLatch(const base::Number* const x);
    // slot functions
    bool setSlotHoldTimer(const base::Number* const x);
    bool setSlotEventMap(const base::PairStream* const x);

private:
    bool picked[NUM_BUTTONS];   // tells our buttons if they are currently picked or not
    SendData pickedSD[NUM_BUTTONS];
    int eventMap[NUM_BUTTONS];  // actual event id we want to generate for each button hit
    int currButtonId;           // button ID of our current hold button (one being clicked)
    int lastButtonId;           // button ID of the last button that was "pushed"
    base::UpTimer* timer;      // our selection timer
    bool latched;               // our latch flag, which, without a timer, will be a logical
                                // flag to determine when to keep the switch up or down, or
                                // make the switch return to it's starting position
};

//------------------------------------------------------------------------------
// Class: SolenoidButton
// Description: Custom button that understands that it belongs to a SolenoidSwitch,
// and will let it know when its respective button has been hit.
//------------------------------------------------------------------------------
class SolenoidButton : public Button
{
    DECLARE_SUBCLASS(SolenoidButton,Button)
public:
    SolenoidButton();

    bool onSingleClick() override;

    // The left mouse button has been depressed
    virtual bool onMouseDown();
    virtual bool onPicked(const base::Number* const x);

    virtual bool event(const int event, base::Object* const obj = nullptr) override;
    virtual void updateData(const double dt = 0.0) override;

private:
    bool noTimer;    // this flag tells this button that we don't time, we simply switch
    bool pushed;     // flag that tells us we have the mouse down on our current graphic
    SendData pushedSD;

};

}
}

#endif
