//---------------------------------------------------------------------
// Class: LinearVelocity
//---------------------------------------------------------------------
#ifndef __Eaagles_Basic_LinearVelocity_H__
#define __Eaagles_Basic_LinearVelocity_H__

#include "../Number.h"

namespace Eaagles {
namespace Basic {

class Distance;
class Time;
//---------------------------------------------------------------------
// Class: LinearVelocity
// Base class:  Object -> Number -> LinearVelocity
//
// Description: Linear velocity with internal units of meters/second.
//              Will convert to desired units if needed.
//              Distance: Object -> Number -> Distance -> Inches
//                        Object -> Number -> Distance -> Inches
//                        Object -> Number -> Distance -> Feet
//                        Object -> Number -> Distance -> Meters
//                        Object -> Number -> Distance -> CentiMeters
//                        Object -> Number -> Distance -> KiloMeters
//                        Object -> Number -> Distance -> NauticalMiles
//                        Object -> Number -> Distance -> StatuteMiles
//              Time: Object -> Number -> Time -> MilliSeconds
//                    Object -> Number -> Time -> Seconds
//                    Object -> Number -> Time -> Minutes
//                    Object -> Number -> Time -> Hours
//                    Object -> Number -> Time -> Days
//
//              Will convert input units to meters/second.
//
// Factory name: LinearVelocity
// Slots:
//    distance  <distance>  ! distance
//    time      <time>      ! time
//
//---------------------------------------------------------------------
// Input File Example:
//---------------------------------------------------------------------
//
// ...
//    myLinearVelocity1:
//        ( LinearVelocity
//            distance:( Meters 1.5 )
//            time:    ( Seconds 1 )
//        )
//    myLinearVelocity2:
//        ( LinearVelocity
//            distance:( Feet 90 )
//            time:    ( Milliseconds 1 )
//        )
// ...
//
//---------------------------------------------------------------------
// Class Detail:
//---------------------------------------------------------------------
//
// LinearVelocity();
//  Constructor for input parser:
//
// LinearVelocity(LCreal newLinearVelocityMetersPerSec);
//  Constructor with input as meters per second:
//
// LinearVelocity(Distance* newDistance, Seconds* newTime);
//  Constructor with distance in units desired and time in units desired:
//
// LCreal getMetersPerSecond();
//  Returns in meters/second:
//
// bool setMetersPerSecond(LCreal newLinearVelocity);
//  Sets in meters/second - returns true if successful:
//
// bool setMeters(LCreal newDistance);
//  Input distance in (meters)
//
// bool setSeconds(LCreal newTime);
//  Input time in (seconds)
//
// bool setSlotDistance(Number* number);
//  Input as type of distance (Meters, Feet, ...)
//
// bool setSlotTime(Number* number);
//  Input as type of time (Seconds, MilliSeconds, ...)
//
// double convert(Distance* newDistance, Time* newTime);
//  Convert routine - converts Linear velocity and outputs in units specified -
//  NOTE: This will ignore the value of the input objects - the output will be in the
//        units that are used as inputs (Input Degrees and MilliSeconds and convert
//        will output the answer in degrees per millisecond)
//
//--------------------------------------------------------------------
class LinearVelocity : public Number
{
    DECLARE_SUBCLASS(LinearVelocity, Number)

public:

    //---------------------------------------------------------------------
    //Constructors:
    //---------------------------------------------------------------------

    //Constructor for input parser:
    LinearVelocity();
    //Constructor with input as meters per second:
    LinearVelocity(const LCreal newLinearVelocityMetersPerSec);
    //Constructor with distance in units desired and time in units desired:
    LinearVelocity(const Distance* const newDistance, const Time* const newTime);

    //---------------------------------------------------------------------
    //Get functions:
    //---------------------------------------------------------------------

    //Returns in meters/second:
    LCreal getMetersPerSecond() const;
    //Convert routine - converts Linear velocity and outputs in units specified:
    LCreal convert(Distance* newDistance, Time* newTime);

    //---------------------------------------------------------------------
    //Set functions:
    //---------------------------------------------------------------------


    bool setMetersPerSecond(const LCreal newLinearVelocity);

    //---------------------------------------------------------------------
    //Slot Table functions
    //---------------------------------------------------------------------

    bool setMeters(const LCreal newDistance);             //Input distance in (meters)
    bool setSeconds(const LCreal newTime);                //Input time in (seconds)
    bool setSlotDistance(const Distance* const msg);      //Input as type of distance (Meters, Degrees, ...)
    bool setSlotTime(const Time* const msg);              //Input as type of time (Seconds, MilliSeconds, ...)

    //---------------------------------------------------------------------
    //Conversion Constants
    //---------------------------------------------------------------------

    static const double FPS2KTSCC;    // feet per second to knots (conversion constant)

private:
    LCreal        distance;                // holds our distance (meters)
    LCreal        time;                    // holds our time seconds)

};

} // End Basic namespace
} // End Eaagles namespace

#endif
