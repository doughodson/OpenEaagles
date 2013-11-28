
#include "openeaagles/simulation/Effects.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/osg/Vec2"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: Effects
//==============================================================================
IMPLEMENT_SUBCLASS(Effects,"Effects")
EMPTY_SERIALIZER(Effects)

// Slot table
BEGIN_SLOTTABLE(Effects)
    "dragIndex",        // 1: drag index used by default dynamics
END_SLOTTABLE(Effects)

// Map slot table to handles 
BEGIN_SLOT_MAP(Effects)
    ON_SLOT(1,setSlotDragIndex,Basic::Number)
END_SLOT_MAP()

// Weapon data for general bomb
int Effects::getCategory() const                { return GRAVITY; }
const char* Effects::getDescription() const     { return "Generic Effects"; }
const char* Effects::getNickname() const        { return "Effects"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Effects::Effects()
{
    STANDARD_CONSTRUCTOR()

    static Basic::String generic("Effects");
    setType(&generic);

    setDragIndex(0.0006f);

    // Default: unguided, ballistic
    setMaxTOF(10.0);
    setLethalRange(50.0f);
    setMaxBurstRng(500.0f);
    setTSG(9999.0f);
    setSOBT(9999.0f);
    setEOBT(0.0f);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Effects::copyData(const Effects& org, const bool)
{
    BaseClass::copyData(org);
    dragIndex = org.dragIndex;
}

void Effects::deleteData()
{
}

//------------------------------------------------------------------------------
// crashNotification() -- We just hit the ground (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool Effects::crashNotification()
{
    bool ok = killedNotification();
    setDetonationResults(DETONATE_NONE);
    setMode(DETONATED);
    return ok;
}

//------------------------------------------------------------------------------
// collisionNotification() -- We just impacted with another player (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool Effects::collisionNotification(Player* const p)
{
    bool ok = killedNotification(p);
    setMode(DETONATED);
    setDetonationResults(DETONATE_NONE);
    return ok;
}

//------------------------------------------------------------------------------
// updateTOF -- default time of flight
//------------------------------------------------------------------------------
void Effects::updateTOF(const LCreal dt)
{
   // As long as we're active ...
   if (isMode(ACTIVE)) {

      // update time of flight,
      setTOF( getTOF() + dt );

      // and check for the end of the flight
      if (getTOF() >= getMaxTOF()) {
         setMode(DETONATED);
         setDetonationResults(DETONATE_NONE);
         return;
      }
   }
}

//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void Effects::weaponDynamics(const LCreal dt)
{
   // Useful constant
   static const LCreal g = ETHG * Basic::Distance::FT2M;      // Acceleration of Gravity (m/s/s)

   // ---
   // Compute & Set acceleration vector (earth)
   // ---

   // First drag
   osg::Vec3 tmp = getVelocity() * (-dragIndex);

   // then gravity
   osg::Vec3 ae1 = tmp;
   ae1[IDOWN] += g;

   setAcceleration(ae1);

   // ---
   // Compute & set new velocity vector (earth)
   // ---
   osg::Vec3 ve1 = getVelocity() + (ae1 * dt);
   setVelocity(ve1);

   // ---
   // .. Only after setVelocity has been called ...
   // ---
   LCreal vp = getTotalVelocity();
   LCreal vg = getGroundSpeed();

   // ---
   // Set velocity vector (body)
   //  (total velocity is along X)
   // ---
   setVelocityBody(vp, 0.0, 0.0);

   // ---
   // Sent angular values
   // ---
   LCreal newPsi   = lcAtan2(ve1[IEAST],ve1[INORTH]);
   LCreal newTheta = lcAtan2( -ve1[IDOWN], vg );
   setEulerAngles(0.0, newTheta, newPsi);
   setAngularVelocities(0.0, 0.0, 0.0);

}


//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

// dragIndex: drag index used by default dynamics
bool Effects::setSlotDragIndex(Basic::Number* const p)
{
    setDragIndex( p->getReal() );
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Effects::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Class: Chaff
//==============================================================================
IMPLEMENT_SUBCLASS(Chaff,"Chaff")
EMPTY_SLOTTABLE(Chaff)
EMPTY_SERIALIZER(Chaff)

// Weapon data for general bomb
int Chaff::getCategory() const                { return GRAVITY; }
const char* Chaff::getDescription() const     { return "Generic Chaff"; }
const char* Chaff::getNickname() const        { return "Chaff"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Chaff::Chaff()
{
    STANDARD_CONSTRUCTOR()
    
    static Basic::String generic("Chaff");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Chaff::copyData(const Chaff& org, const bool)
{
    BaseClass::copyData(org);
}

void Chaff::deleteData()
{
}


//==============================================================================
// Class: Flare
//==============================================================================
IMPLEMENT_SUBCLASS(Flare,"Flare")
EMPTY_SLOTTABLE(Flare)
EMPTY_SERIALIZER(Flare)

// Weapon data for general bomb
int Flare::getCategory() const                { return GRAVITY; }
const char* Flare::getDescription() const     { return "Generic Flare"; }
const char* Flare::getNickname() const        { return "Flare"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Flare::Flare()
{
    STANDARD_CONSTRUCTOR()

    static Basic::String generic("Flare");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Flare::copyData(const Flare& org, const bool)
{
    BaseClass::copyData(org);
}

void Flare::deleteData()
{
}

//==============================================================================
// Class: Decoy
//==============================================================================
IMPLEMENT_SUBCLASS(Decoy,"Decoy")
EMPTY_SLOTTABLE(Decoy)
EMPTY_SERIALIZER(Decoy)

// Weapon data for general bomb
int Decoy::getCategory() const                { return GRAVITY; }
const char* Decoy::getDescription() const     { return "Generic Decoy"; }
const char* Decoy::getNickname() const        { return "Decoy"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Decoy::Decoy()
{
    STANDARD_CONSTRUCTOR()

    static Basic::String generic("Decoy");
    setType(&generic);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Decoy::copyData(const Decoy& org, const bool)
{
    BaseClass::copyData(org);
}

void Decoy::deleteData()
{
}

} // End Simulation namespace
} // End Eaagles namespace
