#include "openeaagles/simulation/TargetData.h"

#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Identifier.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(TargetData,"TargetData")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(TargetData)
    "enabled",
    "completed",
    "weaponType",
    "quantity",
    "interval",
    "stickType",
    "stickDistance",
    "maxMissDistance",
    "manualAssign",
    "armDelay",
    "angle",
    "azimuth",
    "velocity",
END_SLOTTABLE(TargetData)

// Map slot table to handles 
BEGIN_SLOT_MAP(TargetData)
    ON_SLOT( 1,setSlotEnabled,Basic::Boolean)
    ON_SLOT( 2,setSlotCompleted,Basic::Boolean)
    ON_SLOT( 3,setSlotWpnType,Basic::String)
    ON_SLOT( 4,setSlotQuantity,Basic::Integer)
    ON_SLOT( 5,setSlotInterval,Basic::Number)
    ON_SLOT( 6,setSlotStickType,Basic::Identifier)
    ON_SLOT( 7,setSlotStickDistance,Basic::Number)
    ON_SLOT( 8,setSlotMaxMissDistance,Basic::Number)
    ON_SLOT( 9,setSlotManualAssign,Basic::Boolean)
    ON_SLOT(10,setSlotArmDelay,Basic::Number)
    ON_SLOT(11,setSlotAngle,Basic::Number)
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
    wpnType      = 0;
    quantity     = 0;
    stickType    = MIDPOINT;
    stickDist    = 0;
    maxMissDist  = 0;
    interval     = 0;
    manualAssign = false;
    armDelay     = 0;
    angle        = 0;
    azimuth      = 0;
    velocity     = 0;
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void TargetData::copyData(const TargetData& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
      wpnType = 0;
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
    setWpnType(0);
}

bool TargetData::setSlotEnabled(const Basic::Boolean* b)
{
    if (b==0) return false;
    enabled = b->getBoolean();
    return true;
}
bool TargetData::setSlotCompleted(const Basic::Boolean* b)
{
    if (b==0) return false;
    completed = b->getBoolean();
    return true;
}
bool TargetData::setSlotWpnType(const Basic::String* s)
{
    if (s==0) return false;
    setWpnType(s);
    return true;
}
bool TargetData::setSlotQuantity(const Basic::Integer* i)
{
    if (i==0) return false;
    quantity = i->getInt();
    return true;
}
bool TargetData::setSlotStickType(const Basic::Identifier* const s)
{
    if (s==0) return false;
    if      (*s == "MIDPOINT" || *s == "midpoint")         stickType = MIDPOINT;
    else if (*s == "LEADING_EDGE" || *s == "leading_edge") stickType = LEADING_EDGE;
    else {
        std::cerr << "TargetData::setSlotStickType(): invalid type: "
                  << *s << std::endl;
        std::cerr << " -- valid types are { MIDPOINT, LEADING_EDGE }"
                  << std::endl;
        return false;
    }
    return true;
}
bool TargetData::setSlotStickDistance(const Basic::Number* n)
{
    if (n==0) return false;
    stickDist = n->getReal();
    return true;
}
bool TargetData::setSlotMaxMissDistance(const Basic::Number* n)
{
    if (n==0) return false;
    maxMissDist = n->getReal();
    return true;
}
bool TargetData::setSlotInterval(const Basic::Number* n)
{
    if (n==0) return false;
    interval = n->getReal();
    return true;
}
bool TargetData::setSlotManualAssign(const Basic::Boolean* b)
{
    if (b==0) return false;
    manualAssign = b->getBoolean();
    return true;
}
bool TargetData::setSlotArmDelay(const Basic::Number* n)
{
    if (n==0) return false;
    armDelay = n->getReal();
    return true;
}
bool TargetData::setSlotAngle(const Basic::Number* n)
{
    if (n==0) return false;
    angle = n->getReal();
    return true;
}
bool TargetData::setSlotAzimuth(const Basic::Number* n)
{
    if (n==0) return false;
    azimuth = n->getReal();
    return true;
}
bool TargetData::setSlotVelocity(const Basic::Number* n)
{
    if (n==0) return false;
    velocity = n->getReal();
    return true;
}

Basic::Object* TargetData::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

void TargetData::setWpnType(const Basic::String* s)
{
    if (wpnType != 0) wpnType->unref();
    wpnType = s;
    if (wpnType != 0) wpnType->ref();
}

std::ostream& TargetData::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
	int j = 0;
	if ( !slotsOnly ) {
		sout << "( " << getFormName() << std::endl;
		j = 4;
	}

	BaseClass::serialize(sout,i+j,true);

	indent(sout,i+j);
	sout << "enabled:         " << enabled      << std::endl;

	indent(sout,i+j);
	sout << "completed:       " << completed    << std::endl;

	indent(sout,i+j);
	sout << "weaponType:      " << wpnType      << std::endl;

	indent(sout,i+j);
	sout << "quantity:        " << quantity     << std::endl;

	indent(sout,i+j);
	sout << "stickDistance:   " << stickDist    << std::endl;

	indent(sout,i+j);
	sout << "stickType:       " << stickType    << std::endl;

	indent(sout,i+j);
	sout << "interval:        " << interval     << std::endl;

	indent(sout,i+j);
	sout << "maxMissDistance: " << maxMissDist  << std::endl;

	indent(sout,i+j);
	sout << "manualAssign:    " << manualAssign << std::endl;

	indent(sout,i+j);
	sout << "armDelay:        " << armDelay     << std::endl;

	indent(sout,i+j);
	sout << "angle:           " << angle        << std::endl;

	indent(sout,i+j);
	sout << "azimuth:         " << azimuth      << std::endl;

	indent(sout,i+j);
	sout << "velocity:        " << velocity     << std::endl;
	
	if ( !slotsOnly ) {
		indent(sout,i);
		sout << ")" << std::endl;
	}
	return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
