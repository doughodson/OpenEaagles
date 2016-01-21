//------------------------------------------------------------------------------
// Class AngularVelocity
//------------------------------------------------------------------------------

#ifndef __oe_basic_AngularVelocity_H__
#define __oe_basic_AngularVelocity_H__

#include "openeaagles/basic/Number.h"

namespace oe {
namespace basic {

class Angle;
class Time;

//------------------------------------------------------------------------------
// Class AngularVelocity
// Base class:  Object -> Number -> AngularVelocity
//
// Description: Angular velocity with internal units of radians/second.
//                Will convert to desired units if needed.
//                    Angles: Radians, Degrees, Semi-Circles
//                    Time: Milliseconds, Minutes, Hours, Days
//                Will convert input units to radians/second from any angle/time unit.
//
// Factory name: AngularVelocity
// Slots:
//    angle  <Angle>  ! angle (default: 1)
//    time   <Time>   ! time (default: 1)
//
//------------------------------------------------------------------------------
//
// Input File Example:
//
//    myAngularVelocity1:
//        ( AngularVelocity
//            angle:    ( Radians 1.5 )
//            time:     ( Seconds 1 )
//        )
//    myAngularVelocity2:
//        ( AngularVelocity
//            angle:    ( Degrees 90 )
//            time:     ( Milliseconds 1 )
//        )
//
// Class Detail:
//
// AngularVelocity();
//  Constructor for input parser:
//
// AngularVelocity(LCreal newAngularVelocityRadiansPerSec);
//  Constructor with input as radians per second:
//
// AngularVelocity(Angle* newAngle, Time* newTime);
//  Constructor with angle in units desired and time in units desired:
//
// LCreal getRadiansPerSecond();
//  Returns in radians/second:
//
// bool setRadiansPerSecond(LCreal newAngularVelocity);
//  Sets in radians/second - returns true if successful:
//
// bool setDegrees(LCreal newAngle);
//  Input angle in (degrees)
//
// bool setRadians(LCreal newAngle);
//  Input angle in (radians)
//
// bool setSeconds(LCreal newTime);
//  Input time in (seconds)
//
// bool setSlotAngle(Angle* angle);
//  Input as type of angle (Radians, Degrees, ...)
//
// bool setSlotTime(Time* time);
//  Input as type of time (Seconds, MilliSeconds, ...)
//
// LCreal convert(Angle* newAngle, Time* newTime);
//  Convert routine - converts angular velocity and outputs in units specified -
// NOTE: This ignores the value of the input objects - the output will be in the
//      units that are used as inputs (Input Degrees and MilliSeconds and convert(~)
//      will output the answer in degrees per millisecond)
//
//--------------------------------------------------------------------
class AngularVelocity : public Number
{
    DECLARE_SUBCLASS(AngularVelocity, Number)

public:

//------------------------------------------------------------------------------
//Constructors:
//------------------------------------------------------------------------------

    //Constructor for input parser:
    AngularVelocity();
    //Constructor with input as radians per second:
    AngularVelocity(const LCreal newAngularVelocityRadiansPerSec);
    //Constructor with angle in units desired and time in units desired:
    AngularVelocity(const Angle* const newAngle, const Time* const newTime);

//------------------------------------------------------------------------------
//Get functions:
//------------------------------------------------------------------------------

    //Returns in radians/second:
    LCreal getRadiansPerSecond() const;
    //Convert routine - converts angular velocity and outputs in units specified:
    LCreal convert(Angle* newAngle, Time* newTime);

//------------------------------------------------------------------------------
//Set functions:
//------------------------------------------------------------------------------

    //Sets Value of Object - Be sure input is in (rad/sec)
    bool setRadiansPerSecond(const LCreal newAngularVelocity);

//------------------------------------------------------------------------------
//Slot Table functions
//------------------------------------------------------------------------------

    bool setDegrees(const LCreal newAngle);         //Input angle in (degrees)
    bool setRadians(const LCreal newAngle);         //Input angle in (radians)
    bool setSeconds(const LCreal newTime);          //Input time in (seconds)
    bool setSlotAngle(const Angle* const msg);      //Input as type of angle (Radians, Degrees, ...)
    bool setSlotTime(const Time* const msg);        //Input as type of time (Seconds, MilliSeconds, ...)

private:

    LCreal        angle;                            // holds our angle (radians)
    LCreal        time;                             // holds our time (seconds)

};

} // End basic namespace
} // End oe namespace

#endif
