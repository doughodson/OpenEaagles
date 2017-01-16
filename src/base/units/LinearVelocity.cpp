
#include "openeaagles/base/units/LinearVelocity.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"
#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

const double LinearVelocity::FPS2KTSCC = 0.59248384864341771016513018598233;

IMPLEMENT_SUBCLASS(LinearVelocity, "LinearVelocity")
EMPTY_SERIALIZER(LinearVelocity)
EMPTY_DELETEDATA(LinearVelocity)

BEGIN_SLOTTABLE(LinearVelocity)
    "distance",    // 1: distance
    "time",        // 2: time
END_SLOTTABLE(LinearVelocity)

BEGIN_SLOT_MAP(LinearVelocity)
    ON_SLOT(1, setSlotDistance, Distance)
    ON_SLOT(2, setSlotTime, Time)
END_SLOT_MAP()

LinearVelocity::LinearVelocity()
{
    STANDARD_CONSTRUCTOR()

    //Set a default distance, time, and linearVelocity
    distance = 1;
    time = 1;
    val = 1;
}

LinearVelocity::LinearVelocity(const double newLinearVelocityMetersPerSec)
{
    STANDARD_CONSTRUCTOR()

    //Set the distance to the input and linearVelocity to the input and make seconds 1 to get meters per second
    distance = newLinearVelocityMetersPerSec;
    time = 1;
    val = newLinearVelocityMetersPerSec;
}

LinearVelocity::LinearVelocity(const Distance* const newDistance, const Time* const newTime)
{
    STANDARD_CONSTRUCTOR()

    distance = 1;
    time = 1;

    //Set checks to false
    bool okDistance = false;
    bool okTime = false;

    //Check and convert the distance to meters
    if (newDistance != nullptr)
    {
        double finalDistance = Meters::convertStatic( *newDistance );
        okDistance = setMeters(finalDistance);
    }

    //Check and convert the time to seconds
    if (newTime != nullptr)
    {
        double finaltime = Seconds::convertStatic( *newTime );
        okTime = setSeconds(finaltime);
    }

    //Check that both were set correctly - if not give error
    if ( !okTime || !okDistance )
    {
        //Give error if something was not set correctly:
        std::cerr << "Distance or Time not set correctly - new LinearVelocity Object bad." << std::endl;
    }
}

void LinearVelocity::copyData(const LinearVelocity& org, const bool)
{
    BaseClass::copyData(org);

    distance = org.distance;
    time = org.time;
}

double LinearVelocity::getMetersPerSecond() const
{
    return static_cast<double>(val);
};

//---------------------------------------------------------------------
// convert() -- converts from meters/sec to desired distance/time ratio:
//    NOTE: This ignores values of input - only the object type is used
//---------------------------------------------------------------------
double LinearVelocity::convert(Distance* newDistanceUnit, Time* newTimeUnit)
{

    //Init a num to -1 as a check:
    double desiredDistance = -1.0f;
    double desiredTime = -1.0f;
    double desiredResult = -1.0f;

    //Set input object's internal value to 1 as a precaution:
    newDistanceUnit->setValue(1);
    newTimeUnit->setValue(1);

    //Take the internal unit and create an object of Meters to convert distances:
    const auto internalMeters = new Meters(static_cast<double>(distance));

    //Find out what units the distance is in:
    if (dynamic_cast<Feet*>(newDistanceUnit) != nullptr)
    {
        //New distance is in Feet:
        const auto feet = new Feet;
        desiredDistance = feet->convert(*internalMeters);
        feet->unref();
    }
    else if (dynamic_cast<Meters*>(newDistanceUnit) != nullptr)
    {
        //New distance is in Meters:
        const auto meters = new Meters;
        desiredDistance = meters->convert(*internalMeters);
        meters->unref();
    }
    else if (dynamic_cast<CentiMeters*>(newDistanceUnit) != nullptr)
    {
        //New distance is in CentiMeters:
        const auto centimeters = new CentiMeters;
        desiredDistance = centimeters->convert(*internalMeters);
        centimeters->unref();
    }
    else if (dynamic_cast<KiloMeters*>(newDistanceUnit) != nullptr)
    {
        //New distance is in KiloMeters:
        const auto kilometers = new KiloMeters;
        desiredDistance = kilometers->convert(*internalMeters);
        kilometers->unref();
    }
    else if (dynamic_cast<Inches*>(newDistanceUnit) != nullptr)
    {
        //New distance is in Inches:
        const auto inches = new Inches;
        desiredDistance = inches->convert(*internalMeters);
        inches->unref();
    }
    else if (dynamic_cast<NauticalMiles*>(newDistanceUnit) != nullptr)
    {
        //New distance is in NauticalMiles:
        const auto nauticalmiles = new NauticalMiles;
        desiredDistance = nauticalmiles->convert(*internalMeters);
        nauticalmiles->unref();
    }
    else if (dynamic_cast<StatuteMiles*>(newDistanceUnit) != nullptr)
    {
        //New distance is in StatuteMiles:
        const auto statutemiles = new StatuteMiles;
        desiredDistance = statutemiles->convert(*internalMeters);
        statutemiles->unref();
    }
    else
    {
        //Give Error - Not sure what type it is:
        std::cerr << "Distance Conversion Type Not Found." << std::endl;
    }
    internalMeters->unref();

    //Find out what units the time input is in - do not use built in convert - very easy to do by hand:
    const auto q = dynamic_cast<Seconds*>(newTimeUnit);
    if (q != nullptr)
    {
        desiredTime = time;
    }
    else if (dynamic_cast<MilliSeconds*>(newTimeUnit) != nullptr)
    {
        //Time in milliseconds:
        desiredTime = time*1000;
    }
    else if (dynamic_cast<Minutes*>(newTimeUnit) != nullptr)
    {
        //Time in minutes:
        desiredTime = time/60;
    }
    else if (dynamic_cast<Hours*>(newTimeUnit) != nullptr)
    {
        //Time in hours:
        desiredTime = time/3600;
    }
    else if (dynamic_cast<Days*>(newTimeUnit) != nullptr)
    {
        //Time in days:
        desiredTime = time/86400;
    }
    else
    {
        //Give Error - Not sure what type it is:
        std::cerr << "Time Conversion Type Not Found." << std::endl;
    };

    desiredResult = desiredDistance/desiredTime;

    return desiredResult;
}

bool LinearVelocity::setMetersPerSecond(const double newLinearVelocity)
{
    //Set distance and time - units in meters per second -> num = input; den = 1
    bool ok1 = setMeters(newLinearVelocity);
    bool ok2 = setSeconds(1);

    //Check both values for ok:
    ok1 = (ok1)&&(ok2);

    return ok1;
}

bool LinearVelocity::setSlotDistance(const Distance* const msg)
{
    bool ok = false;

    //Try to convert Number to a distance
    if( msg != nullptr ) {
        const double finalNumber = Meters::convertStatic(*msg);
        ok = setMeters(finalNumber);
    }
    return ok;
}

bool LinearVelocity::setSlotTime(const Time* const msg)
{
    bool ok = false;

    //Try to convert Number to a distance
    if (msg != nullptr) {
        const double finalNumber = Seconds::convertStatic(*msg);
        ok = setSeconds(finalNumber);
    }
    return ok;
}

bool LinearVelocity::setMeters(const double newDistance)
{
    distance = newDistance;
    // update linear velocity
    val = distance/time;
    return true;
}

bool LinearVelocity::setSeconds(const double newTime)
{
    //Set Time:
    time = newTime;
    // update linear velocity
    val = distance/time;
    return true;
}

}
}
