//------------------------------------------------------------------------------
// Classes: UpTimer, DownTimer, Timer
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Timer_H__
#define __Eaagles_Basic_Timer_H__

#include "openeaagles/basic/Object.h"

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
//    timerValue  <Time>      ! Timer interval (default: 0)
//    alarmTime   <Time>      ! Alarm time (default: 0)
//    active      <Boolean>   ! Sets timer active (running) flag (default: false)
//
//==============================================================================
class Timer : public Object {
    DECLARE_SUBCLASS(Timer,Object)

public:
   enum Type { UP, DOWN };        // Timer type/direction

   static  const unsigned int MAX_TIMERS = EAAGLES_CONFIG_MAX_INTERVAL_TIMERS;

public:
   Timer();
   Timer(const Type direction, const double time = 0.0);

   Type getType() const;            // Type of interval timer
   double getCurrentTime() const;   // Current value of this timer (seconds)
   double getAlarmTime() const;     // Alarm time (seconds)
   double getTimerValue() const;    // Timer interval (i.e., reset value) (seconds)

   bool isRunning() const;    // Return true of the timer is active.
   bool isNotRunning() const; // Return true of the timer is not active.

   virtual void start();      // Starts (stops) this timer.
   virtual void stop();       // Starts (stops) this timer.

   // Sets the interval timer value (timerValue) to 'time' and resets the timer;
   // the timer is left inactive (not running).
   virtual void reset(const double time);

   // Resets the timer to the interval timer value (timerValue);
   // the timer is left inactive (not running).
   virtual void reset();

   //  Sets the interval timer value (timerValue) to 'time' and restarts the timer
   virtual void restart(const double time);

   // Resets and restarts the timer
   virtual void restart();

   // Sets an alarm to 'atime' seconds.  Returns true if the timer
   // is active and one of the following conditions is met.  
   //    direction == up   and timer >= atime
   //    direction == down and timer <= atime
   virtual bool alarm(const double atime);

   // Returns true if the timer is active and one of the following conditions is met.
   //    direction == up   and timer >= atime
   //    direction == down and timer <= atime
   virtual bool alarm() const;

   // Returns true if all timers are frozen
   bool freeze() const;

   // When the state of the freeze flag is true, ALL timers are frozen;
   // the previous value of the freeze flag is returned.
    static bool freeze(const bool ff);

   virtual bool setCurrentTime(const double sec);    // Set the current time (sec)
   virtual bool setAlarmTime(const double sec);      // Set the alarm time (sec)
   virtual bool setTimerValue(const double sec);     // Set the interval time (i.e., reset value) (sec)

   // Updates all of the instances of Timer.
   // ---Called by the main application routine.
   static void updateTimers(const double dt);

   // Updates this timer.  Usually called by updateTimers().
   virtual void update(const double dt);

protected:
    // Slot functions
    virtual bool setSlotTimerValue(const Time* const msg);    // Sets the timer value
   virtual bool setSlotAlarmTime(const Time* const msg);    // Sets the alarm value
    virtual bool setSlotTimerActive(const Number* const msg); // Sets the timer active (running) flag

private:
   void initData();

   double ctime;           // Current time (seconds).
   double alarmTime;       // Alarm time (seconds).
   double timerValue;      // Timer value (seconds).
    bool     active;             // Active flag.
   Type dir;               // Direction up/down.

    static bool	frz;           // Freeze all timers (freeze time)
   static Timer* timers[MAX_TIMERS];   // List of timers
   static unsigned int nTimers;        // Number of timers in the list
   static long semaphore;              // Semaphore for the timer list 

    static void addToTimerList(Timer* timer);
    static void removeFromTimerList(Timer* timer);
};

// Inline functions
inline Timer::Type Timer::getType() const       { return dir; }
inline double Timer::getCurrentTime() const     { return ctime; }
inline double Timer::getAlarmTime() const       { return alarmTime; }
inline double Timer::getTimerValue() const      { return timerValue; }
inline bool Timer::isRunning() const            { return active; }
inline bool Timer::isNotRunning() const         { return !active; }
inline bool Timer::freeze() const               { return frz; }


//==============================================================================
// Class: UpTimer
// Description: General purpose UP timer; time counts in the positive direction.
//
// Form name: UpTimer
//==============================================================================
class UpTimer : public Timer {
    DECLARE_SUBCLASS(UpTimer,Timer)
public:
    UpTimer(const double rtime = 0.0);
};


//==============================================================================
// Class: DownTimer
// Description: General purpose DOWN timer; time counts in the negative direction.
//
// Form name: DownTimer
//==============================================================================
class DownTimer : public Timer {
    DECLARE_SUBCLASS(DownTimer,Timer)
public:
    DownTimer(const double rtime = 0.0);
};

} // End Basic namespace
} // End Eaagles namespace

#endif
