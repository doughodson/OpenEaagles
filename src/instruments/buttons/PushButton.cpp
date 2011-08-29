#include "openeaagles/instruments/buttons/PushButton.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Instruments {

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
   ON_SLOT(1, setSlotFunction, Basic::Number)
   ON_SLOT(2, setSlotStartState, Basic::Number)
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

   functionType = false;				//false = momentary, true = maintained
   currentState = false;	
   mouseDown    = false;
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
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
EMPTY_DELETEDATA(PushButton)

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotFunction() - sets our pushbutton functionality ie momentary or maintained
//------------------------------------------------------------------------------
bool PushButton::setSlotFunction(const Basic::Number* const newFunction)
{
   bool ok = false;
   if (newFunction != 0) {
      ok = setFunction(newFunction->getBoolean());
   }
   return ok;
}

//------------------------------------------------------------------------------
// setStartFunction() - sets our pushbutton start state  ie not pressed or pressed
//------------------------------------------------------------------------------
bool PushButton::setSlotStartState(const Basic::Number* const newFunction)
{
   bool ok = false;
   if (newFunction != 0) {
      ok = setCurrentState(newFunction->getBoolean());
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
// setCurrentState() - set our current state
//------------------------------------------------------------------------------
bool PushButton::setCurrentState(const bool x)
{
    currentState = x; 
    return true; 
}

//------------------------------------------------------------------------------
// onSingleClick() - the left mouse button has been released
//------------------------------------------------------------------------------
bool PushButton::onSingleClick()
{
   //if button is not maintained (ie momentary) then switch it after mouse button released
   if(!functionType && mouseDown) currentState = !currentState;

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
    currentState = !currentState;
    return true;
}

//------------------------------------------------------------------------------
// onMouseDown() - the left mouse button has been depressed
//------------------------------------------------------------------------------
bool PushButton::onMouseDown()
{
   currentState = !currentState;
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
Basic::Object* PushButton::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

}  // end Instruments namespace
}  // end Eaagles namespace
