
#include "openeaagles/simulation/Bomb.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Distances.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: Bomb
//==============================================================================
IMPLEMENT_SUBCLASS(Bomb,"Bomb")

// Slot table
BEGIN_SLOTTABLE(Bomb)
    "arming",           // 1: Bomb arming option
    "noseFuze",         // 2: Nose fuze flag
    "midFuze",          // 3: Middle fuze flag
    "tailFuze",         // 4: Tail fuze flag
    "fuzeAltitude",     // 5: Fuze arming Altitude
    "fuzeTime",         // 6: Fuze Arming Time
    "dragIndex",        // 7: drag index used by default dynamics
END_SLOTTABLE(Bomb)

// Map slot table to handles
BEGIN_SLOT_MAP(Bomb)
   ON_SLOT( 1, setSlotArmingOption, Basic::Identifier)
   ON_SLOT( 2, setSlotNoseFuze,     Basic::Number)
   ON_SLOT( 3, setSlotMidFuze,      Basic::Number)
   ON_SLOT( 4, setSlotTailFuze,     Basic::Number)
   ON_SLOT( 5, setSlotFuzeAltitude, Basic::Number)
   ON_SLOT( 6, setSlotFuzeTime,     Basic::Number)
   ON_SLOT( 7, setSlotDragIndex,    Basic::Number)
END_SLOT_MAP()

// Weapon data for general bomb
int Bomb::getCategory() const                { return GRAVITY; }
const char* Bomb::getDescription() const     { return "Generic Bomb"; }
const char* Bomb::getNickname() const        { return "Bomb"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Bomb::Bomb()
{
   STANDARD_CONSTRUCTOR()

   static Basic::String generic("Bomb");
   setType(&generic);

   initData();
}

void Bomb::initData()
{

   // Default fuze, guidance & ballistic values for generic bomb
   // (derived models will override these values)

   arming    = NONE;
   noseFuze  = false;
   midFuze   = false;
   tailFuze  = false;
   fuzeAlt   = 0.0;
   fuzeTime  = 0.0;
   setDragIndex(0.0006f);

   missDistRef.set(0,0,0);
   tgtRangeRef.set(0,0,0);
   cmdStrAz = 0;
   cmdStrEl = 0;
   guidanceValid = false;

   setMaxTOF(60.0f);
   setLethalRange(50.0f);
   setMaxBurstRng(500.0f);
   setTSG(9999.0f);
   setSOBT(9999.0f);
   setEOBT(0.0f);
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Bomb::copyData(const Bomb& org, const bool cc)
{
    BaseClass::copyData(org);
   if (cc) initData();

    arming    = org.arming;
    noseFuze  = org.noseFuze;
    midFuze   = org.midFuze;
    tailFuze  = org.tailFuze;
    fuzeAlt   = org.fuzeAlt;
    fuzeTime  = org.fuzeTime;
    dragIndex     = org.dragIndex;
    missDistRef   = org.missDistRef;;
    tgtRangeRef   = org.tgtRangeRef;
    cmdStrAz      = org.cmdStrAz;
    cmdStrEl      = org.cmdStrEl;
    guidanceValid = org.guidanceValid;
}

void Bomb::deleteData()
{
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int Bomb::getArmingOption() const
{
   return arming;
}

bool Bomb::isArmingOption(const unsigned int a) const
{
   return (a == arming);
}

LCreal Bomb::getFuzeAltitude() const
{
   return fuzeAlt;
}

LCreal Bomb::getFuzeTime() const
{
   return fuzeTime;
}

LCreal Bomb::getDragIndex() const
{
   return dragIndex;
}

bool Bomb::isNoseFuze() const
{
   return noseFuze;
}

bool Bomb::isMidFuze() const
{
   return midFuze;
}

bool Bomb::isTailFuze() const
{
   return tailFuze;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Bomb::setNoseFuze(const bool f)
{
   noseFuze = f;
   return true;
}

bool Bomb::setMidFuze(const bool f)
{
   midFuze = f;
   return true;
}

bool Bomb::setTailFuze(const bool f)
{
   tailFuze = f;
   return true;
}

bool Bomb::setArmingOption(const unsigned int a)
{
   arming = a;
   return true;
}

bool Bomb::setFuzeAltitude(const LCreal v)
{
   fuzeAlt = v;
   return true;
}

bool Bomb::setFuzeTime(const LCreal v)
{
   fuzeTime = v;
   return true;
}

bool Bomb::setDragIndex(const LCreal v)
{
   dragIndex = v;
   return true;
}

//------------------------------------------------------------------------------
// weaponGuidance() -- default guidance; using Robot Aircraft (RAC) guidance
//------------------------------------------------------------------------------
void Bomb::weaponGuidance(const LCreal)
{
   if (isGuidanceEnabled()) {

      // Get target position
      const osg::Vec3 tgtPos = getTargetPosition();

      // weapon velocities (platform)
      const osg::Vec3 vel = getVelocity();

      // weapon velocities (relative)
      LCreal velG = lcSqrt (vel[0]*vel[0] + vel[1]*vel[1]);
      if (velG < 1.0f) velG = 1.0f;

      // Earth to platform sin/cos
      const LCreal prcos = vel[0] / velG;
      const LCreal prsin = vel[1] / velG;

      // Target position (platform)
      const osg::Vec3 tgtPosP = tgtPos - getPosition();

      // Target range distance
      const LCreal tsrng = tgtPosP.length();

      // Target position (relative)
      tgtRangeRef.set(
            (  prcos * tgtPosP[0] + prsin * tgtPosP[1] ),
            ( -prsin * tgtPosP[0] + prcos * tgtPosP[1] ),
            ( tgtPosP[2] )
         );

      // ---
      // Estimate impact point
      // ---

      // Inputs
      osg::Vec3 initPos(0.0, 0.0, static_cast<LCreal>(-getAltitude()));
      osg::Vec3 initVel = vel;
      LCreal groundPlane = tgtPos[2];
      LCreal timeStep = 0.1;
      LCreal maxTime = 60.0;

      // Outputs
      LCreal tof = 0;
      osg::Vec3 ip;

      // Predict platform impact point
      weaponImpactPrediction(&initPos, &initVel, groundPlane, timeStep, maxTime, dragIndex, &ip, &tof);
      ip[2] = tgtPosP[2];

      // ---
      // Compute miss distance
      // ---

      // miss Dist (Platform)
      const osg::Vec3 pmd = tgtPosP - ip;

      // rotate to ref coord sys
      missDistRef.set(
            (  prcos * pmd[0] + prsin * pmd[1] ),  // X MISS DIST (REF)
            ( -prsin * pmd[0] + prcos * pmd[1] ),  // Y MISS DIST (REF)
            ( pmd[2] )
         );

      // Azimuth & elevation steering
      static const LCreal LIMITS = 1.0f;
      static const LCreal GAIN = 5.0f;
      LCreal caz = lcAtan2( missDistRef.y(), tsrng ) * GAIN;
      if (caz >  LIMITS) caz =  LIMITS;
      if (caz < -LIMITS) caz = -LIMITS;
      cmdStrAz = caz;

      LCreal cel = lcAtan2( missDistRef.x(), tsrng ) * GAIN;
      if (cel >  LIMITS) cel =  LIMITS;
      if (cel < -LIMITS) cel = -LIMITS;
      cmdStrEl = cel;

      guidanceValid = true;

   }
   else {
      missDistRef.set(0,0,0);
      tgtRangeRef.set(0,0,0);
      cmdStrAz = 0;
      cmdStrEl = 0;
      guidanceValid = false;

   }
}


//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void Bomb::weaponDynamics(const LCreal dt)
{
   // Useful constant
   static const LCreal g = ETHG * Basic::Distance::FT2M;      // Acceleration of Gravity (m/s/s)

   // ---
   // Compute & Set acceleration vector (earth)
   // ---

   // First drag
   const osg::Vec3 tmp = getVelocity() * (-dragIndex);

   // then gravity
   osg::Vec3 ae1 = tmp;
   ae1[IDOWN] += g;

   // and the controls from guidance, if any
   if (guidanceValid) {
      const LCreal h = g * 2.0f;
      osg::Vec3 ab0(0, 0, 0); // body accelerations

      ab0[1] = h * cmdStrAz;  // body Y accel (m/s/s)
      if (ab0[1] >  h) ab0[1] = h;
      if (ab0[1] < -h) ab0[1] = -h;

      ab0[2] = -g * cmdStrEl;  // body Z accel (m/s/s)
      if (ab0[2] >  (g/5.0f)) ab0[2] = (g/5.0f);
      if (ab0[2] < -h) ab0[2] = -h;

      // rotate to earth and add to accel vector
      osg::Vec3 ae0 = ab0 * getRotMat();
      ae1 = ae1 + ae0;
   }

   // ---
   // Compute new velocity vector (earth)
   // ---
   const osg::Vec3 ve1 = getVelocity() + (ae1 * dt);

   // ---
   // Compute Euler angles
   // ---
   const LCreal vg = std::sqrt(ve1[INORTH]*ve1[INORTH] + ve1[IEAST]*ve1[IEAST]);
   const LCreal newPsi   = lcAtan2(ve1[IEAST],ve1[INORTH]);
   const LCreal newTheta = lcAtan2( -ve1[IDOWN], vg );


   // ---
   // Set the new values: Euler angles first then velocities
   // ---
   setEulerAngles(0.0, newTheta, newPsi);
   setVelocity(ve1);
   setAcceleration(ae1);
   setAngularVelocities(0.0, 0.0, 0.0);
}

//------------------------------------------------------------------------------
// weaponPrediction -- Predict weapon impact point
//------------------------------------------------------------------------------
bool Bomb::impactPrediction(
      const osg::Vec3* const initPos,
      const osg::Vec3* const initVel,
      const LCreal groundPlane,
      const LCreal dt,
      const LCreal maxTime,
      osg::Vec3* const finalPos,
      LCreal* const tof
   ) const
{
   return weaponImpactPrediction(initPos, initVel, groundPlane, dt, maxTime, dragIndex, finalPos, tof);
}

//------------------------------------------------------------------------------
// Predict weapon impact point
//------------------------------------------------------------------------------
bool Bomb::weaponImpactPrediction(
      const osg::Vec3* const initPos,
      const osg::Vec3* const initVel,
      const LCreal groundPlane,
      const LCreal dt,
      const LCreal maxTime,
      const LCreal dragIndex,
      osg::Vec3* const finalPos,
      LCreal* const tof
   )
{
   // Useful constant
   static const LCreal g = ETHG * Basic::Distance::FT2M;      // Acceleration of Gravity (m/s/s)

   // Make sure we have all of our pointers
   if (initPos == nullptr || initVel == nullptr || finalPos == nullptr || tof == nullptr) return false;

   // Set initial position & velocity
   osg::Vec3 pos = *initPos;
   osg::Vec3 vel = *initVel;

   // Run time
   LCreal time = 0.0;

   // ---
   // Simple ballistic extrapolate of the weapon's position
   // ---

   while (time < maxTime && (pos[IDOWN] < groundPlane)) {

      // Compute acceleration vector (earth) ...

      //    first drag,
      osg::Vec3 ae1 = vel * (-dragIndex);

      //   then gravity
      ae1[IDOWN] += g;

      // Compute new velocity vector (earth)
      vel = vel + (ae1 * dt);

      // Compute new position vector (earth)
      pos = pos + (vel * dt);

      time += dt;
   }

   // Set the results back to the caller
   *finalPos = pos;
   *tof = time;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

bool Bomb::setSlotArmingOption(Basic::Identifier* const p)
{
    bool ok = false;
    if (p != nullptr) {
       if (*p == "NONE" || *p == "none") ok = setArmingOption(NONE);
       else if (*p == "FREE_FALL" || *p == "free_fall") ok = setArmingOption(FREE_FALL);
       else if (*p == "RETARDED" || *p == "retarded") ok = setArmingOption(RETARDED);
       else if (*p == "TIMER" || *p == "timer") ok = setArmingOption(TIMER);
       else if (*p == "PROXIMITY" || *p == "proximity") ok = setArmingOption(PROXIMITY);
       if (!ok) {
          std::cerr << "Bomb::setSlotArmingOption() Invalid arming option: " << *p << std::endl;
    }
    }
    return ok;
}

// noseFuze: Nose fuze flag
bool Bomb::setSlotNoseFuze(Basic::Number* const p)
{
    setNoseFuze( p->getBoolean() );
    return true;
}

// midFuze: Middle fuze flag
bool Bomb::setSlotMidFuze(Basic::Number* const p)
{
    setMidFuze( p->getBoolean() );
    return true;
}

// tailFuze: Tail fuze flag
bool Bomb::setSlotTailFuze(Basic::Number* const p)
{
    setTailFuze( p->getBoolean() );
    return true;
}

// fuzeAltitude:  Fuze arming Altitude
bool Bomb::setSlotFuzeAltitude(Basic::Number* const p)
{
    setFuzeAltitude( p->getFloat() );
    return true;
}

// fuzeTime:  Fuze Arming Time
bool Bomb::setSlotFuzeTime(Basic::Number* const p)
{
    setFuzeTime( p->getFloat() );
    return true;
}

// dragIndex: drag index used by default dynamics
bool Bomb::setSlotDragIndex(Basic::Number* const p)
{
    setDragIndex( p->getReal() );
    return true;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Bomb::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Bomb::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

   {
      unsigned int aa = getArmingOption();
      if (aa <= PROXIMITY) {
      indent(sout,i+j);
         sout << "arming: ";
         if (aa == NONE) sout << "NONE";
         else if (aa == FREE_FALL) sout << "FREE_FALL";
         else if (aa == RETARDED) sout << "RETARDED";
         else if (aa == TIMER) sout << "TIMER";
         else if (aa == PROXIMITY) sout << "PROXIMITY";
         sout << std::endl;
   }
   }

   if ( isNoseFuze() ) {
      indent(sout,i+j);
      sout << "noseFuze: true" << std::endl;
   }

   if ( isMidFuze() ) {
      indent(sout,i+j);
      sout << "midFuze: true" << std::endl;
   }

   if ( isTailFuze() ) {
      indent(sout,i+j);
      sout << "tailFuze: true" << std::endl;
   }

   if ( getFuzeAltitude() != 0.0f ) {
      indent(sout,i+j);
      sout << "fuzeAltitude: " << getFuzeAltitude() << std::endl;
   }

   if ( getFuzeTime() != 0.0f ) {
      indent(sout,i+j);
      sout << "fuzeTime: " << getFuzeTime() << std::endl;
   }

   indent(sout,i+j);
   sout << "dragIndex: " << getDragIndex() << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
