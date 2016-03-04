#include "openeaagles/instruments/buttons/PushButton.h"
#include "openeaagles/base/Number.h"

namespace oe {
namespace instruments {

IMPLEMENT_SUBCLASS(PushButton,"PushButton")
EMPTY_SERIALIZER(PushButton)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(PushButton)
   "maintained",        // if this is true, we keep our button state after we release the mouse
   "startdepressed",    // do we start depressed or up?
END_SLOTTABLE(PushButton)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(PushButton)
   ON_SLOT(1, setSlotFunction, basic::Number)
   ON_SLOT(2, setSlotStartState, basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Event Table
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(PushButton)
   ON_EVENT(INPUT_LEFT_EDGE, onMouseDown)
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
PushButton::PushButton()
{
   STANDARD_CONSTRUCTOR()

   functionType = false;     //false = momentary, true = maintained
   currentState = false;
   mouseDown    = false;
   initState = false;
   buttonStatusSD.empty();
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void PushButton::copyData(const PushButton& org, const bool)
{
   BaseClass::copyData(org);
   functionType = org.functionType;
   currentState = org.currentState;
   buttonStatusSD.empty();
   initState = org.initState;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(PushButton)

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotFunction() - sets our pushbutton functionality ie momentary or maintained
//------------------------------------------------------------------------------
bool PushButton::setSlotFunction(const basic::Number* const newFunction)
{
   bool ok = false;
   if (newFunction != nullptr) {
      ok = setFunction(newFunction->getBoolean());
   }
   return ok;
}

//------------------------------------------------------------------------------
// setStartFunction() - sets our pushbutton start state  ie not pressed or pressed
//------------------------------------------------------------------------------
bool PushButton::setSlotStartState(const basic::Number* const newFunction)
{
   bool ok = false;
   if (newFunction != nullptr) {
      initState = newFunction->getBoolean();
      // set our current state initially
      currentState = initState;
      ok = true;
   }
   return ok;
}

// Set functions
//------------------------------------------------------------------------------
// setFunction() - set our function type
//------------------------------------------------------------------------------
bool PushButton::setFunction(const bool x)
{
    functionType = x;
    return true;
}


//------------------------------------------------------------------------------
// onSingleClick() - the left mouse button has been released
//------------------------------------------------------------------------------
bool PushButton::onSingleClick()
{
   //if button is not maintained (ie momentary) then switch it after mouse button released
   if(!functionType && mouseDown) currentState = initState;

   mouseDown = false;
   BaseClass::onSingleClick();

   return true;
}

//------------------------------------------------------------------------------
// onCancel() - cancel our push
//------------------------------------------------------------------------------
bool PushButton::onCancel()
{
    // we go back to our original state
    mouseDown = false;
    currentState = initState;
    return true;
}

//------------------------------------------------------------------------------
// onMouseDown() - the left mouse button has been depressed
//------------------------------------------------------------------------------
bool PushButton::onMouseDown()
{
   currentState = !initState;
   mouseDown = true;
   return true;
}

//------------------------------------------------------------------------------
// updateData() -
//------------------------------------------------------------------------------
void PushButton::updateData(const LCreal dt)
{
   BaseClass::updateData(dt);

   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << "CURRENT STATE = " << currentState << std::endl;
   //}

   send("buttons", SELECT, currentState, buttonStatusSD);
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Button
//------------------------------------------------------------------------------
basic::Object* PushButton::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end instruments namespace
}  // end oe namespace
