//------------------------------------------------------------------------------
// Classes: UpTimer, DownTimer, Timer
//
// Description:	General purpose up/down timers.  
//
//------------------------------------------------------------------------------
#ifndef __Timer_H_3D3A4F1E_B6F3_4a8f_B949_DD77131D420D__
#define __Timer_H_3D3A4F1E_B6F3_4a8f_B949_DD77131D420D__

#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Basic {

class Number;
class Time;


//==============================================================================
// Class: Timer
//
// Description:	General purpose up/down timer.  
//
// Slots:
//      timerValue  <Timer>   ! Timer interval (Time) (default: 0)
//      active      <Number>  ! Sets timer active (running) flag (default: false)
//
//
// Public methods: Base class public methods, plus ...
//
//      Timer(int direction)
//      Timer(int direction, LCreal rtime)
//          Constructor; general purpuse up/down timer.  The direction can
//          be either Timer::up or Timer::down.  The timer is preset
//          to rtime or zero.
//
//      LCreal time()
//          Returns the current value of this timer.
//
//      start()
//      stop()
//          Starts (stops) this timer.
//
//      reset()
//      reset(LCreal rtime)
//          Stops and resets this timer to the timer value.  If 'rtime' is
//          given, the timer value is first set to rtime.  The timer is
//          left inactive (not running).
//
//      restart()
//      restart(LCreal rtime)
//          Resets this timer to the timer value.  If 'rtime' is given, the
//          timer value is first set to rtime.  The timer is left active (running).
//
//      bool alarm()
//      bool alarm(LCreal atime)
//          Sets an alarm to atime seconds.  Returns true if the timer
//          is active and one of the following conditions is met.  
//              direction == up and timer >= atime
//              direction == down and timer <= atime
//
//      bool isRunning()
//      bool isNotRunning()
//          Return true of the timer is (not) active.
//
//      bool freeze()
//      bool freeze(bool ff)
//          When the state of the freeze flag is true, ALL timers
//          are frozen.  Function freeze() returns the current state
//          of the freeze flag.  If 'ff' is given, the freeze flag is
//          set to the value of 'ff' and the previous value of the freeze
//          flag is returned.
//
//      updateTimers(LCreal dt)
//          Updates all of the instances of Timer.  Called by the
//          main application routine.
//
//      update(LCreal dt)
//          Updates this timer.  Usually called by updateTimers().
//
//==============================================================================
class Timer : public Object {
    DECLARE_SUBCLASS(Timer,Object)

public:
    enum { UP, DOWN };

public:
    Timer(int direction, LCreal rtime = 0.0f);
    Timer();

    LCreal time() const                { return ctime; }
    void start()                       { active = true; }
    void stop()                        { active = false; }
    void reset()                       { stop(); ctime = timerValue; }
    void reset(const LCreal rtime)     { stop(); timerValue = rtime; reset(); }
    void restart()                     { reset(); start(); }
    void restart(const LCreal rtime)   { reset(rtime); start(); }
    void update(const LCreal dt);
    bool alarm() const;
    bool alarm(const LCreal atime);
    bool isRunning() const             { return active; }
    bool isNotRunning() const          { return !active; }

    static bool freeze(const bool ff);
    bool freeze() const                { return frz; }

    static void updateTimers(const LCreal dt);

    // Slot functions
    virtual bool setSlotTimerValue(const Time* const msg);    // Sets the timer value
    virtual bool setSlotTimerActive(const Number* const msg); // Sets the timer active (running) flag

private:
    LCreal   ctime;              // Current time (seconds).
    LCreal   alarmTime;          // Alarm time (seconds).
    LCreal   timerValue;         // Timer value (seconds).
    bool     active;             // Active flag.
    int      dir;                // Direction up/down.

    static bool	frz;           // Freeze all timers (freeze time)

    static void addToTimerList(Timer* timer);
    static void removeFromTimerList(Timer* timer);
    static List timers;       // List of timers
};

// -----------------------------------------------------------------
// Member functions
// -----------------------------------------------------------------
inline void Timer::update(const LCreal dt)
{
    if (active && !frz) { ctime += (dir == UP ? dt : -dt); }
}

inline bool Timer::alarm() const
{
    if (active) return dir == UP ? (ctime >= alarmTime) : (ctime <= alarmTime);
    else return false;
}

inline bool Timer::alarm(const LCreal atime)
{
    alarmTime = atime; return alarm();
}

inline bool Timer::freeze(const bool ff)
{
    bool f = frz;
    frz = ff;
    return f;
}


// -----------------------------------------------------------------
// Handle timer list
// -----------------------------------------------------------------
inline void Timer::addToTimerList(Timer* timer)
{
   if (!timers.find(timer)) timers.put(timer);
}


inline void Timer::removeFromTimerList(Timer* timer)
{
   timers.remove(timer);
}

//==============================================================================
// Class: UpTimer
//
// Description: General purpose UP counting timer
// Form name: UpTimer
//
//      UpTimer()
//      UpTimer(LCreal rtime)
//          Constructor; Up timer; time counts in the positive direction.
//          The timer is preset to rtime or zero.
//==============================================================================
class UpTimer : public Timer {
    DECLARE_SUBCLASS(UpTimer,Timer)
public:
    UpTimer(LCreal rtime = 0.0f);
};


//==============================================================================
// Class: DownTimer
//
// Description: General purpose DOWN counting timer
// Form name: DownTimer
//
//      DownTimer()
//      DownTimer(LCreal rtime)
//          Constructor; Down timer; time counts in the negative direction.
//          The timer is preset to rtime or zero.
//==============================================================================
class DownTimer : public Timer {
    DECLARE_SUBCLASS(DownTimer,Timer)
public:
    DownTimer(LCreal rtime = 0.0f);
};

} // End Basic namespace
} // End Eaagles namespace

#endif
