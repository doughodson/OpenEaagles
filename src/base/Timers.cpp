#include "openeaagles/base/Timers.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/units/Times.h"

namespace oe {
namespace base {

//==============================================================================
// Class Timer
//==============================================================================
IMPLEMENT_SUBCLASS(Timer,"Timer")
EMPTY_SERIALIZER(Timer)

//------------------------------------------------------------------------------
// Class (static) variables
//------------------------------------------------------------------------------
bool Timer::frz = false;            // Freeze flag
Timer* Timer::timers[MAX_TIMERS];   // List of timers
unsigned int Timer::nTimers = 0;    // Number of timers in the list
long Timer::semaphore = 0;          // Semaphore for the timer list


//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Timer)
   "timerValue",         // 1: Timer interval (default: 0)
   "alarmTime",          // 2: Alarm time (default: 0)
   "active",             // 3: Sets timer active (running) flag (default: false)
END_SLOTTABLE(Timer)

// Map slot table to handles
BEGIN_SLOT_MAP(Timer)
   ON_SLOT(1, setSlotTimerValue,Time)
   ON_SLOT(2, setSlotAlarmTime,Time)
   ON_SLOT(3, setSlotTimerActive,Number)
END_SLOT_MAP()

// -----------------------------------------------------------------------------
// Constructor(s)
// -----------------------------------------------------------------------------

Timer::Timer()
{
    STANDARD_CONSTRUCTOR()

   initData();
}


void Timer::initData()
{
    active = false;
    timerValue = 0.0f;
    ctime  = 0.0f;
    alarmTime = 0.0f;
    dir = DOWN;
    addToTimerList(this);
}


Timer::Timer(const Type direction, const double rtime)
{
    STANDARD_CONSTRUCTOR()

    initData();

    timerValue = rtime;
    ctime  = rtime;
    dir = direction;
}

// -----------------------------------------------------------------------------
// copyData() -- copy member data
// -----------------------------------------------------------------------------
void Timer::copyData(const Timer& org, const bool cc)
{
    active = false;

    BaseClass::copyData(org);
    if (cc) initData();

    timerValue = org.timerValue;
    ctime  = org.ctime;
    alarmTime = org.alarmTime;
    dir = org.dir;
    active = org.active;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Timer::deleteData()
{
   removeFromTimerList(this);
}

// -----------------------------------------------------------------
// Support functions
// -----------------------------------------------------------------
void Timer::start()                          { active = true; }
void Timer::stop()                           { active = false; }
void Timer::reset()                          { stop(); ctime = timerValue; }
void Timer::reset(const double rtime)        { stop(); timerValue = rtime; reset(); }
void Timer::restart()                        { reset(); start(); }
void Timer::restart(const double rtime)      { reset(rtime); start(); }
void Timer::update(const double dt)          { if (active && !frz) { ctime += (dir == UP ? dt : -dt); } }
bool Timer::alarm(const double atime)        { alarmTime = atime; return alarm(); }
bool Timer::setCurrentTime(const double sec) { ctime = sec; return true; }
bool Timer::setAlarmTime(const double sec)   { alarmTime = sec; return true; }
bool Timer::setTimerValue(const double sec)  { timerValue = sec; return true; }

bool Timer::freeze(const bool ff)
{
    bool f = frz;
    frz = ff;
    return f;
}

bool Timer::alarm() const
{
    if (active) return dir == UP ? (ctime >= alarmTime) : (ctime <= alarmTime);
    else return false;
}


// -----------------------------------------------------------------
// Update all timers in the list
// -----------------------------------------------------------------
void Timer::updateTimers(const double dt)
{
    if (!frz) {
      lcLock( semaphore );
      for (unsigned int i = 0; i < nTimers; i++) {
         timers[i]->update(dt);
      }
      lcUnlock( semaphore );
    }
}


// -----------------------------------------------------------------
// Add a new timer to the list
// -----------------------------------------------------------------
void Timer::addToTimerList(Timer* timer)
{
   bool ok = false;
   lcLock( semaphore );
   if (nTimers < MAX_TIMERS) {
      timers[nTimers++] = timer;
      ok = true;
   }
   lcUnlock( semaphore );

   if (!ok) {
      std::cerr << "Timer::addToTimerList() ERROR failed to add a new timer to static timer list" << std::endl;
   }
}


// -----------------------------------------------------------------
// Remove a timer from the list
// -----------------------------------------------------------------
void Timer::removeFromTimerList(Timer* timer)
{
   lcLock( semaphore );

   // Find this timer in the list
   unsigned int found = MAX_TIMERS;
   for (unsigned int i = 0; i < nTimers && found == MAX_TIMERS; i++) {
      if (timers[i] == timer) found = i;
   }

   // If found then remove it by moving all timer in the list that are
   // beyond this timer down position
   if (found != MAX_TIMERS) {
      --nTimers;  // One less timer
      for (unsigned int i = found; i < nTimers; i++) {
         timers[i] = timers[i+1];
    }
}

   lcUnlock( semaphore );
}

// -----------------------------------------------------------------
// Slot functions
// -----------------------------------------------------------------

// Sets the timer value
bool Timer::setSlotTimerValue(const Time* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
      double sec = Seconds::convertStatic(*msg);
      ok = setTimerValue(sec);
      if (ok) setCurrentTime(sec);
   }
   return ok;
}

// Sets the alarm value
bool Timer::setSlotAlarmTime(const Time* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setAlarmTime( Seconds::convertStatic(*msg) );
    }
    return ok;
}

// Sets the timer active (running) flag
bool Timer::setSlotTimerActive(const Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
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
EMPTY_COPYDATA(UpTimer)
EMPTY_DELETEDATA(UpTimer)

UpTimer::UpTimer(const double rtime) : Timer(UP,rtime)
{
}


//==============================================================================
// Class UpTimer
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DownTimer,"DownTimer")
EMPTY_SERIALIZER(DownTimer)
EMPTY_COPYDATA(DownTimer)
EMPTY_DELETEDATA(DownTimer)

DownTimer::DownTimer(const double rtime) : Timer(DOWN,rtime)
{
}


} // End base namespace
} // End oe namespace

