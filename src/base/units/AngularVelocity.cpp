
#include "openeaagles/base/units/AngularVelocity.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/SlotTable.hpp"
#include "openeaagles/base/util/unit_utils.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(AngularVelocity, "AngularVelocity")
EMPTY_SERIALIZER(AngularVelocity)
EMPTY_DELETEDATA(AngularVelocity)

BEGIN_SLOTTABLE(AngularVelocity)
    "angle",    // 1: angle
    "time",     // 2: time
END_SLOTTABLE(AngularVelocity)

// Map slot table to handles
BEGIN_SLOT_MAP(AngularVelocity)
    ON_SLOT(1, setSlotAngle, Angle)
    ON_SLOT(2, setSlotTime, Time)
END_SLOT_MAP()

AngularVelocity::AngularVelocity()
{
    STANDARD_CONSTRUCTOR()

    //Set a default angle, time, and angularVelocity
    angle = 1;
    time = 1;
    val = 1;
}

AngularVelocity::AngularVelocity(const double newAngularVelocityRadiansPerSec)
{
    STANDARD_CONSTRUCTOR()

    //Set the angle to the input and angularVelocity to the input and make seconds 1 to get radians per second:
    angle = newAngularVelocityRadiansPerSec;
    time = 1;
    val = newAngularVelocityRadiansPerSec;
}

AngularVelocity::AngularVelocity(const Angle* const newAngle, const Time* const newTime)
{
    STANDARD_CONSTRUCTOR()

    //Set a default angle, time, and angularVelocity
    angle = 1;
    time = 1;
    val = 1;

    //Set Checks to false:
    bool okAngle = false;
    bool okTime = false;

    //Check and convert the angle to radians
    if ( newAngle != nullptr ) {
        const auto finalAngle = static_cast<double>(Radians::convertStatic(*newAngle));
        okAngle = setRadians(finalAngle);
    }

    //Check and convert the time to seconds
    if ( newTime != nullptr ) {
        double finaltime = Seconds::convertStatic( *newTime );
        okTime = setSeconds(finaltime);
    }

    //Check that both were set correctly - if not give error:
    if ( !okTime || !okAngle )
    {
        //Give error if something was not set correctly:
        std::cerr << "Angle or Time not set correctly - new AngularVelocity Object bad." << std::endl;
    }
}

double AngularVelocity::getRadiansPerSecond() const
{
    return static_cast<double>(val);
}

//------------------------------------------------------------------------------
// convert() -- converts from radians/sec to desired angle/time ratio:
//    NOTE: This ignores values of input - only the object type is used
//------------------------------------------------------------------------------
double AngularVelocity::convert(Angle* newAngleUnit, Time* newTimeUnit)
{

    //Init a num to -1 as a check:
    double desiredAngle = -1.0f;
    double desiredTime = -1.0f;
    double desiredResult = -1.0f;

    //Set input opject's internal value to 1 as a precaution:
    newAngleUnit->setValue(1);
    newTimeUnit->setValue(1);

    //Take the internal unit and create an object of Angle to convert angles:
    const auto internalRadians = new Radians(static_cast<double>(angle));

    //Find out what units the angle is in:
    if (dynamic_cast<Degrees*>(newAngleUnit) != nullptr)
    {
        //New angle is in degrees:
        const auto degrees = new Degrees;
        desiredAngle = static_cast<double>(degrees->convert(*internalRadians));
        degrees->unref();
    }
    else if (dynamic_cast<Radians*>(newAngleUnit) != nullptr)
    {
        //New angle is in radians:
        desiredAngle = angle;
    }
    else if (dynamic_cast<Semicircles*>(newAngleUnit) != nullptr)
    {
        //New angle is in semicircles:
        const auto semicircles = new Semicircles;
        desiredAngle = static_cast<double>(semicircles->convert(*internalRadians));
        semicircles->unref();
    }
    else
    {
        //Give Error - Not sure what type it is:
        std::cerr << "Angle Conversion Type Not Found." << std::endl;
    }
    internalRadians->unref();

    //Find out what units the time input is in - do not use built in convert - very easy to do by hand:
    const auto q = dynamic_cast<Seconds*>(newTimeUnit);
    if (q != nullptr) {
        desiredTime = time;
    } else if (dynamic_cast<MilliSeconds*>(newTimeUnit) != nullptr) {
        //Time in milliseconds:
        desiredTime = time*1000;
    } else if (dynamic_cast<Minutes*>(newTimeUnit) != nullptr) {
        //Time in minutes:
        desiredTime = time/60;
    } else if (dynamic_cast<Hours*>(newTimeUnit) != nullptr) {
        //Time in hours:
        desiredTime = time/3600;
    } else if (dynamic_cast<Days*>(newTimeUnit) != nullptr) {
        //Time in days:
        desiredTime = time/86400;
    } else {
        //Give Error - Not sure what type it is:
        std::cerr << "Time Conversion Type Not Found." << std::endl;
    };
    desiredResult = desiredAngle/desiredTime;
    return desiredResult;
}

bool AngularVelocity::setRadiansPerSecond(const double newAngularVelocity)
{

    //Set angle and time - units in radians per second -> num = input; den = 1:
    bool ok1 = setRadians(newAngularVelocity);
    bool ok2 = setSeconds(1);

    //Check both values for ok:
    ok1 = (ok1)&&(ok2);

    return ok1;
}

bool AngularVelocity::setSlotAngle(const Angle* const msg)
{
    bool ok = false;

    //Try to convert Number to an angle:
    if( msg != nullptr ) {
        double finalNumber = static_cast<double>(Radians::convertStatic(*msg));
        ok = setRadians(finalNumber);
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotTime() -- sets time based on input object and its value:
//------------------------------------------------------------------------------
bool AngularVelocity::setSlotTime(const Time* const msg)
{
    bool ok = false;

    //Try to convert Number to a time:
    if( msg != nullptr ) {
        double finalNumber = Seconds::convertStatic(*msg);
        ok = setSeconds(finalNumber);
    }
    return ok;
}

//------------------------------------------------------------------------------
// setDegrees() -- sets angle of object in (degrees):
//------------------------------------------------------------------------------
bool AngularVelocity::setDegrees(const double newAngle)
{
    //Set the angle in radians:
    bool ok = setRadians( newAngle * static_cast<double>(angle::D2RCC) );

    return ok;
}

//------------------------------------------------------------------------------
// setRadians() -- sets angle of object in (radians):
//------------------------------------------------------------------------------
bool AngularVelocity::setRadians(const double newAngle)
{
    angle = newAngle;
    //Update new angular velocity:
    val = angle/time;
    return true;
}

//------------------------------------------------------------------------------
// setTime() -- sets time of object in (seconds):
//------------------------------------------------------------------------------
bool AngularVelocity::setSeconds(const double newTime)
{
    //Set Time:
    time = newTime;
    //Update new angular velocity:
    val = angle/time;
    return true;
}

void AngularVelocity::copyData(const AngularVelocity& org, const bool)
{
    BaseClass::copyData(org);

    angle = org.angle;
    time = org.time;
}

}
}
