
#ifndef __oe_instruments_PushButton_H__
#define __oe_instruments_PushButton_H__

#include "openeaagles/instruments/buttons/Button.hpp"

namespace oe {
namespace instruments {

//------------------------------------------------------------------------------
// Class: PushButton
// Description: Button which will select one or another graphic based on being pressed or not.
//------------------------------------------------------------------------------
class PushButton : public Button
{
    DECLARE_SUBCLASS(PushButton,Button)

public:
    PushButton();

   // gets the pushbutton type ie momentary = false or maintained = true
   bool getFunction() const     { return functionType; };

   // gets the current state of the pushbutton ie off = false or on = true
   bool getCurrentState() const { return currentState; };

   // sets the pushbutton type ie momentary = false or maintained = true
   virtual bool setFunction(const bool x);

   // The left mouse button has been depressed
   virtual bool onMouseDown();

   virtual bool onSingleClick() override;
   virtual bool onCancel() override;

   virtual void updateData(const double dt = 0.0) override;
   virtual bool event(const int event, base::Object* const obj = nullptr) override;

protected:
   // sets the pushbutton type to momentary = false or maintained = true
   bool setSlotFunction(const base::Number* const newFunction);
   bool setSlotStartState(const base::Number* const newFunction);

private:
   bool functionType {};     // false = momentary, true = maintained
   bool currentState {};     // false = off, true = on
   bool mouseDown {};        // mouse press down status
   SendData buttonStatusSD;  // what is our button doing?
   bool initState {};        // initial state
};

}
}

#endif
