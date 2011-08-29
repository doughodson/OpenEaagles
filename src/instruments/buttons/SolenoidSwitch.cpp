#include "openeaagles/instruments/buttons/SolenoidSwitch.h"

#include "openeaagles/basicGL/Display.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Timers.h"

namespace Eaagles {
namespace Instruments {

IMPLEMENT_SUBCLASS(SolenoidSwitch,"SolenoidSwitch")
EMPTY_SERIALIZER(SolenoidSwitch)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(SolenoidSwitch)
    "holdTimeSec",      // how long to hold the button before the timer goes off
    "eventMap",         // turns our button "sticks" into events, from position 1 to 3 (center, top, bottom)
END_SLOTTABLE(SolenoidSwitch)

//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(SolenoidSwitch)
    ON_SLOT(1, setSlotHoldTimer, Basic::Number)
    ON_SLOT(2, setSlotEventMap, Basic::PairStream)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(SolenoidSwitch)
    ON_EVENT_OBJ(SELECT, selectLatch, Basic::Number)
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
SolenoidSwitch::SolenoidSwitch()
{
    STANDARD_CONSTRUCTOR()

    timer = 0;  // we will create our timer if we set a timer slot
    currButtonId = CENTER_BUTTON;  // default to first current button
    picked[0] = true;   // center button picked
    picked[1] = false;
    picked[2] = false;
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pickedSD[i].empty();
        eventMap[i] = -1;
    }
    lastButtonId = CENTER_BUTTON;
    latched = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void SolenoidSwitch::copyData(const SolenoidSwitch& org, const bool)
{
   BaseClass::copyData(org);

   if (org.timer != 0) {
       if (timer != 0) {
           timer->unref();
           timer = (Basic::UpTimer*)org.timer->clone();
       }
   }    
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pickedSD[i].empty();
        picked[i] = org.picked[i];
        eventMap[i] = org.eventMap[i];
    }
    currButtonId = org.currButtonId;
    lastButtonId = org.lastButtonId;
    latched = org.latched;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void SolenoidSwitch::deleteData()
{
    if (timer != 0) {
        timer->unref();
        timer = 0;
    }
}

//------------------------------------------------------------------------------
// setSlotHoldTimer() - set how long to hold our button before state change
//------------------------------------------------------------------------------
bool SolenoidSwitch::setSlotHoldTimer(const Basic::Number* const x)
{
    bool ok = false;
    if (x != 0) ok = setHoldTimer(x->getReal());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotEventMap() - this sets the list of the eventId's we want generated
// when a button is activated.  For example, if you hold the top button until
// the timer alarms out, instead of sending an event of 2 (top button id), it 
// will map that event to this one and will send that to the display to be
// processed.
//------------------------------------------------------------------------------
bool SolenoidSwitch::setSlotEventMap(const Basic::PairStream* const x)
{
    if (x != 0) {
        if (x->entries() != 3) std::cout << "SolenoidSwitch::setSlotEventMap() - Need 3 eventIds for the button, will not send eventIds for the ones without it" << std::endl;
        int count = 0;
        const Basic::List::Item* item = x->getFirstItem();
        while (item != 0 && count < 3) {
            Basic::Pair* pair = (Basic::Pair*)item->getValue();
            Basic::Number* num = dynamic_cast<Basic::Number*>(pair->object());
            if (num != 0) eventMap[count] = num->getInt();
            count++;
            item = item->getNext();
        }
        //for (int i = 0; i < NUM_BUTTONS; i++) std::cout << "EVENT ID #" << i << " = " << eventMap[i] << std::endl;
    }
    return true;
}

//------------------------------------------------------------------------------
// selectLatch() - tells our switch if it's ok to "latch" 
//------------------------------------------------------------------------------
bool SolenoidSwitch::selectLatch(const Basic::Number* const x)
{
    if (x != 0) {
        latched = x->getBoolean();
    }
    return true;
}


//------------------------------------------------------------------------------
// setHoldTimer() - sets our hold timer
//------------------------------------------------------------------------------
bool SolenoidSwitch::setHoldTimer(const LCreal x)
{
    if (timer != 0) timer->alarm(x);
    else {
        timer = new Basic::UpTimer();
        timer->alarm(x);
    }
    return true;
}

//------------------------------------------------------------------------------
// latch() - called when our button has been pressed and we either want timing to
// begin or awaiting logical latching.
//------------------------------------------------------------------------------
void SolenoidSwitch::latch(const int buttonId)
{
    // clear out our buttons
    if (buttonId == CENTER_BUTTON) resetButton();
    else {
        if (buttonId != currButtonId) {
            for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
            lastButtonId = currButtonId;
            currButtonId = buttonId;
            picked[currButtonId-1] = true;
            // send the event ID IF we are using LOGIC instead of TIMING
            if (timer == 0) {
                if (eventMap[currButtonId-1] != -1) {
                    BasicGL::Display* myDisplay = (BasicGL::Display*)findContainerByType(typeid(BasicGL::Display));
                    if (myDisplay != 0) {
                        myDisplay->buttonEvent(eventMap[currButtonId-1]);
                        //std::cout << "EVENT ID " << eventMap[currButtonId-1] << " sent!" << std::endl;
                    }
                }   
            }
            // if we are using a timer, restart
            else timer->restart();
        }
    }
}

//------------------------------------------------------------------------------
// determineLatch() - called when a button has been released
//------------------------------------------------------------------------------
void SolenoidSwitch::determineLatch()
{
    if (timer != 0) {
        if (timer->isRunning()) {
            // ok, we didn't hold the timer long enough, we don't switch
            // we stopped it, so we go back to our last current button
            timer->stop();
            for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
            currButtonId = lastButtonId;
            picked[currButtonId-1] = true;
        }
    }
    // no timer, we are going by logical latch button
    else {
        if (!latched) {
            currButtonId = CENTER_BUTTON;
            resetButton();
        }
    }
}

//------------------------------------------------------------------------------
// resetButton() - this forces a timer stop, and returns to the center
// button position
//------------------------------------------------------------------------------
void SolenoidSwitch::resetButton()
{
    // timer logic
    if (timer != 0) {
        if (currButtonId != CENTER_BUTTON) {
            BasicGL::Display* myDisplay = (BasicGL::Display*)findContainerByType(typeid(BasicGL::Display));
            if (myDisplay != 0) {
                //std::cout << "EVENT ID " << eventMap[CENTER_BUTTON-1] << " sent!" << std::endl;
                myDisplay->buttonEvent(eventMap[CENTER_BUTTON-1]);
            }
        }
        currButtonId = CENTER_BUTTON;
        lastButtonId = currButtonId;
        timer->stop();
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        picked[currButtonId-1] = true;       
    }
    else {
        if (currButtonId != CENTER_BUTTON) {
            BasicGL::Display* myDisplay = (BasicGL::Display*)findContainerByType(typeid(BasicGL::Display));
            if (myDisplay != 0) {
                //std::cout << "EVENT ID " << eventMap[CENTER_BUTTON-1] << " sent!" << std::endl;
                myDisplay->buttonEvent(eventMap[CENTER_BUTTON-1]);
            }
        }
        // we are using logic, don't send a reset, just go back to the center
        currButtonId = CENTER_BUTTON;
        lastButtonId = currButtonId;
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        picked[currButtonId-1] = true;       
    }
}

//------------------------------------------------------------------------------
// updateData() - background thread
//------------------------------------------------------------------------------
void SolenoidSwitch::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

    // timer latching
    if (timer != 0 && timer->alarm()) {
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        lastButtonId = currButtonId;
        picked[currButtonId-1] = true;
        timer->stop();
        if (eventMap[currButtonId-1] != -1) {
            BasicGL::Display* myDisplay = (BasicGL::Display*)findContainerByType(typeid(BasicGL::Display));
            if (myDisplay != 0) {
                myDisplay->buttonEvent(eventMap[currButtonId-1]);
                //std::cout << "TIMER EVENT ID " << eventMap[currButtonId-1] << " sent!" << std::endl;
            }
        }   
        timer->stop();
        timer->reset();
    }

    // tell our buttons what position they have
    send("button%d", USER_KEY_EVENT, picked, pickedSD, NUM_BUTTONS);
}

//------------------------------------------------------------------------------
// getSlotByIndex() 
//------------------------------------------------------------------------------
Basic::Object* SolenoidSwitch::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

// Hold Button -------------------------------------------------------------------------------------
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(SolenoidButton,"SolenoidButton")
EMPTY_SERIALIZER(SolenoidButton)

//------------------------------------------------------------------------------
// Event Table
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(SolenoidButton)
   ON_EVENT(INPUT_LEFT_EDGE, onMouseDown)
   ON_EVENT_OBJ(USER_KEY_EVENT, onPicked, Basic::Number)
END_EVENT_HANDLER()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
SolenoidButton::SolenoidButton()
{
   STANDARD_CONSTRUCTOR()
   noTimer = false;
   pushed = false;
   pushedSD.empty();
}

void SolenoidButton::copyData(const SolenoidButton& org, const bool)
{
    noTimer = org.noTimer;
    pushed = org.pushed;
    pushedSD.empty();
}

EMPTY_DELETEDATA(SolenoidButton)

//------------------------------------------------------------------------------
// onMouseDown() - when the mouse is pressed
//------------------------------------------------------------------------------
bool SolenoidButton::onMouseDown()
{
    if (!noTimer) {
        // tell our switch to latch
        SolenoidSwitch* hs = (SolenoidSwitch*)findContainerByType(typeid(SolenoidSwitch));
        if (hs != 0) hs->latch(getEventId());
    }
    return true;
}

//------------------------------------------------------------------------------
// onSingleClick() - override this to talk to our Hold Switch, not our display
//------------------------------------------------------------------------------
bool SolenoidButton::onSingleClick()
{
    if (!noTimer) {
        // if we are timing, we need to tell our switch that it needs to start timing
        SolenoidSwitch* hs = (SolenoidSwitch*)findContainerByType(typeid(SolenoidSwitch));
        if (hs != 0) hs->determineLatch();
    }
    return true;
}

bool SolenoidButton::onPicked(const Basic::Number* const x)
{
    if (x != 0) pushed = x->getBoolean();
    return true;
}

void SolenoidButton::updateData(LCreal dt)
{
    BaseClass::updateData(dt);
    send("push", SELECT, pushed, pushedSD);
}



}  // end Instruments namespace
}  // end Eaagles namespace
