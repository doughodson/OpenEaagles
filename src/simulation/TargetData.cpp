#include "openeaagles/simulation/TargetData.h"

#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(TargetData,"TargetData")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(TargetData)
    "enabled",             //  1) Target enabled flag (default: false)
    "completed",           //  2) Target completed flag (default: false)
    "weaponType",          //  3) Type of weapon to release (default: 0)
    "quantity",            //  4) Number of weapons to release (default: 0)
    "manualAssign",        //  5) Manually assign weapon to target? (default: false)
    "stickType",           //  6) Weapon stick option (MIDPOINT, LEADING_EDGE) (default: MIDPOINT)
    "stickDistance",       //  7) Weapon stick length (default: 0
    "interval",            //  8) Time between weapon releases (default: 0)
    "maxMissDistance",     //  9) Maximum miss distance (default: 0)
    "armDelay",            // 10) Arming delay (default: 0)
    "angle",               // 11) Impact angle (default: 0)
    "azimuth",             // 12) Azimuth angle (default: 0)
    "velocity",            // 13) Impact velocity (ft/sec) (default: 0)
END_SLOTTABLE(TargetData)

// Map slot table to handles
BEGIN_SLOT_MAP(TargetData)
    ON_SLOT( 1, setSlotEnabled,Basic::Number)
    ON_SLOT( 2, setSlotCompleted,Basic::Number)
    ON_SLOT( 3,setSlotWpnType,Basic::String)
    ON_SLOT( 4, setSlotQuantity,Basic::Number)
    ON_SLOT( 5, setSlotManualAssign,Basic::Number)
    ON_SLOT( 6,setSlotStickType,Basic::Identifier)

    ON_SLOT( 7, setSlotStickDistance,Basic::Distance)
    ON_SLOT( 7,setSlotStickDistance,Basic::Number)

    ON_SLOT( 8, setSlotInterval,Basic::Time)
    ON_SLOT( 8, setSlotInterval,Basic::Number)

    ON_SLOT( 9, setSlotMaxMissDistance,Basic::Distance)
    ON_SLOT( 9, setSlotMaxMissDistance,Basic::Number)

    ON_SLOT(10, setSlotArmDelay,Basic::Time)
    ON_SLOT(10,setSlotArmDelay,Basic::Number)

    ON_SLOT(11, setSlotAngle,Basic::Angle)
    ON_SLOT(11,setSlotAngle,Basic::Number)

    ON_SLOT(12, setSlotAzimuth,Basic::Angle)
    ON_SLOT(12,setSlotAzimuth,Basic::Number)
    ON_SLOT(13,setSlotVelocity,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
TargetData::TargetData()
{
    STANDARD_CONSTRUCTOR()

    enabled      = false;
    completed    = false;
    wpnType      = nullptr;
    quantity     = 0;
    stickType    = MIDPOINT;
    stickDist    = 0.0;
    maxMissDist  = 0.0;
    interval     = 0.0;
    manualAssign = false;
    armDelay     = 0.0;
    angle        = 0.0;
    azimuth      = 0.0;
    velocity     = 0.0;
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void TargetData::copyData(const TargetData& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
      wpnType = nullptr;
    }

    enabled      = org.enabled;
    completed    = org.completed;
    setWpnType(org.wpnType);
    quantity     = org.quantity;
    stickType    = org.stickType;
    stickDist    = org.stickDist;
    maxMissDist  = org.maxMissDist;
    interval     = org.interval;
    manualAssign = org.manualAssign;
    armDelay     = org.armDelay;
    angle        = org.angle;
    azimuth      = org.azimuth;
    velocity     = org.velocity;
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void TargetData::deleteData()
{
    setWpnType(nullptr);
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool TargetData::setEnabled(const bool b)             { enabled = b; return true; }
bool TargetData::setCompleted(const bool b)           { completed = b; return true; }
bool TargetData::setQuantity(const unsigned int q)    { quantity = q; return true; }
bool TargetData::setStickType(const unsigned int t)   { stickType = t; return true; }
bool TargetData::setStickDistance(const double ft)    { stickDist = ft; return true; }
bool TargetData::setMaxMissDistance(const double ft)  { maxMissDist = ft; return true; }
bool TargetData::setInterval(const double ms)         { interval = ms; return true; }
bool TargetData::setManualAssign(const bool b)        { manualAssign = b; return true; }
bool TargetData::setArmDelay(const double sec)        { armDelay = sec; return true; }
bool TargetData::setAngle(const double degs)          { angle = degs; return true; }
bool TargetData::setAzimuth(const double degs)        { azimuth = degs; return true; }
bool TargetData::setVelocity(const double fps)        { velocity = fps; return true; }

bool TargetData::setWpnType(const Basic::String* const s)
{
   if (wpnType != nullptr) wpnType->unref();
   wpnType = s;
   if (wpnType != nullptr) wpnType->ref();
    return true;
}


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool TargetData::setSlotEnabled(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setEnabled(msg->getBoolean());
   }
   return ok;
}


bool TargetData::setSlotCompleted(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setCompleted(msg->getBoolean());
   }
   return ok;
}


bool TargetData::setSlotWpnType(const Basic::String* const msg)
{
    return setWpnType(msg);
}

bool TargetData::setSlotQuantity(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int v = msg->getInt();
      if (v >= 0) {
         ok = setQuantity(static_cast<unsigned int>(msg->getInt()));
      }
   }
   return ok;
}

bool TargetData::setSlotManualAssign(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setManualAssign(msg->getBoolean());
   }
   return ok;
}

bool TargetData::setSlotStickType(const Basic::Identifier* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      if (*msg == "MIDPOINT" || *msg == "midpoint") {
         ok = setStickType(MIDPOINT);
      }
      else if (*msg == "LEADING_EDGE" || *msg == "leading_edge") {
         ok = setStickType(LEADING_EDGE);
      }
    else {
         std::cerr << "TargetData::setSlotStickType(): invalid type: " << *msg << std::endl;
         std::cerr << " -- valid types are { MIDPOINT, LEADING_EDGE }" << std::endl;
      }
   }
   return ok;
}

bool TargetData::setSlotStickDistance(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setStickDistance( Basic::Feet::convertStatic(*msg) );
   }
   return ok;
}

bool TargetData::setSlotStickDistance(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setStickDistance(msg->getDouble());
   }
   return ok;
}

bool TargetData::setSlotInterval(const Basic::Time* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInterval( Basic::MilliSeconds::convertStatic(*msg) );
   }
   return ok;
}


bool TargetData::setSlotInterval(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setInterval(msg->getDouble());
   }
   return ok;
}


bool TargetData::setSlotMaxMissDistance(const Basic::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxMissDistance( Basic::Feet::convertStatic(*msg) );
   }
   return ok;
}

bool TargetData::setSlotMaxMissDistance(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxMissDistance(msg->getDouble());
   }
   return ok;
}

bool TargetData::setSlotArmDelay(const Basic::Time* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setArmDelay( Basic::Seconds::convertStatic(*msg) );
   }
   return ok;
}

bool TargetData::setSlotArmDelay(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setArmDelay(msg->getDouble());
   }
   return ok;
}


bool TargetData::setSlotAngle(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setAngle( Basic::Degrees::convertStatic(*msg) );
   }
   return ok;
}


bool TargetData::setSlotAngle(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setAngle(msg->getDouble());
   }
   return ok;
}


bool TargetData::setSlotAzimuth(const Basic::Angle* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setAzimuth( Basic::Degrees::convertStatic(*msg) );
   }
   return ok;
}


bool TargetData::setSlotAzimuth(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setAzimuth(msg->getDouble());
   }
   return ok;
}


bool TargetData::setSlotVelocity(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setVelocity(msg->getDouble());
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* TargetData::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& TargetData::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "enabled: " << (isEnabled() ? "true" : "false")  << std::endl;

    indent(sout,i+j);
    sout << "completed: " << (isCompleted() ? "true" : "false")  << std::endl;

    {
        const Basic::String* s = getWpnType();
        if (s != nullptr) {
            indent(sout,i+j);
            sout << "weaponType: " << s  << std::endl;
        }
    }

    indent(sout,i+j);
    sout << "quantity: " << getQuantity()  << std::endl;

    indent(sout,i+j);
    sout << "manualAssign: " <<  (getManualAssign() ? "true" : "false")  << std::endl;

    {
        const unsigned int st = getStickType();
        if (st == MIDPOINT) {
            indent(sout,i+j);
            sout << "stickType: " << "MIDPOINT" << std::endl;
        }
        else if (st == LEADING_EDGE) {
            indent(sout,i+j);
            sout << "stickType: " << "LEADING_EDGE" << std::endl;
        }
    }

    indent(sout,i+j);
    sout << "stickDistance:  ( Feet " << getStickDistance()  << " )"  << std::endl;

    indent(sout,i+j);
    sout << "interval: ( MilliSeconds " << getInterval()  << " )"     << std::endl;

    indent(sout,i+j);
    sout << "maxMissDistance: ( Feet " << getMaxMissDistance()  << " )"  << std::endl;

    indent(sout,i+j);
    sout << "armDelay: ( Seconds " << getArmDelay()  << " )"     << std::endl;

    indent(sout,i+j);
    sout << "angle: ( Degrees  " << getAngle()  << " )"        << std::endl;

    indent(sout,i+j);
    sout << "azimuth: ( Degrees " << getAzimuth()  << " )"      << std::endl;

    indent(sout,i+j);
    sout << "velocity: " << getVelocity()     << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }
    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
