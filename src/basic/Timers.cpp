#include "openeaagles/basic/Timers.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Basic {

// Class (static) variables
bool Timer::frz = false;
List Timer::timers;


//==============================================================================
// Class Timer
//==============================================================================
IMPLEMENT_SUBCLASS(Timer,"Timer")
EMPTY_SERIALIZER(Timer)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Timer)
   "timerValue",         // 1: Timer interval (Time)
   "active",             // 2: Sets timer active (running) flag
END_SLOTTABLE(Timer)

// Map slot table to handles 
BEGIN_SLOT_MAP(Timer)
   ON_SLOT(1, setSlotTimerValue,Time)
   ON_SLOT(2, setSlotTimerActive,Number)
END_SLOT_MAP()

// -----------------------------------------------------------------------------
// Constructor(s)
// -----------------------------------------------------------------------------

Timer::Timer()
{
    STANDARD_CONSTRUCTOR()

    timerValue = 0.0f;
    ctime  = 0.0f;
    alarmTime = 0.0f;
    active = false;
    dir = DOWN;
    addToTimerList(this);
}


Timer::Timer(int direction, LCreal rtime)
{
    STANDARD_CONSTRUCTOR()

    timerValue = rtime;
    ctime  = rtime;
    alarmTime = 0.0f;
    active = false;
    dir = direction;
    addToTimerList(this);
}

// -----------------------------------------------------------------------------
// copyData() -- copy member data
// -----------------------------------------------------------------------------
void Timer::copyData(const Timer& org, const bool)
{
    BaseClass::copyData(org);
    timerValue = org.timerValue;
    ctime  = org.ctime;
    alarmTime = org.alarmTime;
    active = org.active;
    dir = org.dir;
    addToTimerList(this);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Timer::deleteData()
{
   removeFromTimerList(this);
}

// -----------------------------------------------------------------
// updateTimers -- update all timers
// -----------------------------------------------------------------
void Timer::updateTimers(const LCreal dt)
{
    if (!frz) {
        List::Item* item = timers.getFirstItem();
        while (item != 0) {
            Timer* p = dynamic_cast<Timer*>( item->getValue() );
            if (p != 0) p->update(dt);
            item = item->getNext();
        }
    }
}

// -----------------------------------------------------------------
// Slot functions
// -----------------------------------------------------------------

// Sets the timer value
bool Timer::setSlotTimerValue(const Time* const msg)
{
    bool ok = false;
    if (msg != 0) {
        timerValue = Seconds::convertStatic(*msg);
        ok = true;
    }
    return ok;
}

// Sets the timer active (running) flag
bool Timer::setSlotTimerActive(const Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        active = msg->getBoolean();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Object* Timer::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Class UpTimer
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(UpTimer,"UpTimer")
EMPTY_SERIALIZER(UpTimer)

UpTimer::UpTimer(LCreal rtime) : Timer(UP,rtime)
{
}

EMPTY_COPYDATA(UpTimer)
EMPTY_DELETEDATA(UpTimer)

//==============================================================================
// Class UpTimer
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DownTimer,"DownTimer")
EMPTY_SERIALIZER(DownTimer)

DownTimer::DownTimer(LCreal rtime) : Timer(DOWN,rtime)
{
}

EMPTY_COPYDATA(DownTimer)
EMPTY_DELETEDATA(DownTimer)



} // End Basic namespace
} // End Eaagles namespace

