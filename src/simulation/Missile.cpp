
#include "openeaagles/simulation/Missile.h"

#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Missile,"Missile")

// Slot table
BEGIN_SLOTTABLE(Missile)
"minSpeed",         //  1: Minimum Velocity           (m/s)
"maxSpeed",         //  2: Maximum Velocity           (m/s)
"speedMaxG",        //  3: Velocity we reach max G
"maxg",             //  4: Max G's (at "speedMaxG" or above)
"maxAccel",         //  5: Maximum Acceleration       (m/s/s)
"cmdPitch",         //  6: Command Pitch              (rad)
"cmdHeading",       //  7: Command Heading            (rad)
"cmdSpeed",         //  8: Command speed              (m/s)
END_SLOTTABLE(Missile)

// Map slot table to handles 
BEGIN_SLOT_MAP(Missile)
   ON_SLOT(1, setSlotVpMin, Basic::Number)
   ON_SLOT(2, setSlotVpMax, Basic::Number)
   ON_SLOT(3, setSlotVpMaxG, Basic::Number)
   ON_SLOT(4, setSlotMaxG, Basic::Number)
   ON_SLOT(5, setSlotMaxAccel, Basic::Number)
   ON_SLOT(6, setSlotCmdPitch, Basic::Number)
   ON_SLOT(7, setSlotCmdHeading, Basic::Number)
   ON_SLOT(8, setSlotCmdVelocity, Basic::Number)
END_SLOT_MAP()

// Event() map
BEGIN_EVENT_HANDLER(Missile)
END_EVENT_HANDLER()

int Missile::getCategory() const               { return (MISSILE | GUIDED); }
const char* Missile::getDescription() const    { return "AAM"; }
const char* Missile::getNickname() const       { return "AAM"; }

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Missile::Missile()
{
   STANDARD_CONSTRUCTOR()

   static Basic::String generic("GenericMissile");
   setType(&generic);

   setMaxTOF(60.0);
   setLethalRange(30.0f);
   setMaxBurstRng(150.0f);
   setTSG(1.0);
   setSOBT(0.0f);
   setEOBT(60.0f);

   setVpMin(0.0);
   setVpMax(800.0f);
   setVpMaxG(800.0f);
   setMaxG(4.0);
   setMaxAccel(50.0);

   trng = 0;
   trngT = 0;
   trdot = 0;
   trdotT = 0;
   cmdPitch = 0.0;
   cmdHeading = 0.0;
   cmdVelocity = 0.0;

}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Missile::copyData(const Missile& org, const bool)
{
   BaseClass::copyData(org);

   trng = org.trng;
   trngT = org.trngT;
   trdot = org.trdot;
   trdotT = org.trdotT;
   vpMin = org.vpMin;
   vpMax = org.vpMax;
   vpMaxG = org.vpMaxG;
   maxG = org.maxG;
   maxAccel = org.maxAccel;
   cmdPitch = org.cmdPitch;
   cmdHeading = org.cmdHeading;
   cmdVelocity = org.cmdVelocity;
}

void Missile::deleteData()
{
}

//------------------------------------------------------------------------------
// reset() -- Reset vehicle dynamics
//------------------------------------------------------------------------------
void Missile::reset()
{
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// atReleaseInit() -- Init weapon data at release
//------------------------------------------------------------------------------
void Missile::atReleaseInit()
{
   // First the base class will setup the initial conditions
   BaseClass::atReleaseInit();

   if (getDynamicsModel() == 0) {
      // set initial commands
      cmdPitch = (LCreal) getPitch();
      cmdHeading = (LCreal) getHeading();
      cmdVelocity = vpMax;

      if (getTargetTrack() != 0) {
         // Set initial range and range dot
         osg::Vec3 los = getTargetTrack()->getPosition();
         trng = los.length();
         trngT = trng;
      }
      else if (getTargetPlayer() != 0) {
         // Set initial range and range dot
         osg::Vec3 los = getTargetPosition();
         trng = los.length();
         trngT = trng;
      }
      else {
         trng = 0;
      }

      // Range dot
      trdot = 0;
      trdotT = 0;
   }
}

//------------------------------------------------------------------------------
// calculateVectors() --
//------------------------------------------------------------------------------
bool Missile::calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3* const los, osg::Vec3* const vel, osg::Vec3* const posx) const
{
   if (trk != 0) {
      //los = trk->getPosition();
      //vel = trk->getVelocity();
      const Player* tgt0 = trk->getTarget();
      osg::Vec3 p0 = getPosition();
      if (los != 0) *los = tgt0->getPosition() -  p0;
      if (vel != 0) *vel = tgt0->getVelocity();
      if (posx != 0) *posx = tgt0->getPosition();
   }
   else if (tgt != 0) {
      osg::Vec3 p0 = getPosition();
      if (los != 0) *los = tgt->getPosition() -  p0;
      if (vel != 0) *vel = tgt->getVelocity();
      if (posx != 0) *posx = tgt->getPosition();
   }
   else {
      // no guidance until we have a target
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Missile::setSlotVpMin(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setVpMin(msg->getReal());
      ok = true;
   }
   return ok;
}

bool Missile::setSlotVpMax(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setVpMax(msg->getReal());
      ok = true;
   }
   return ok;
}

bool Missile::setSlotVpMaxG(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setVpMaxG(msg->getReal());
      ok = true;
   }
   return ok;
}

bool Missile::setSlotMaxG(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setMaxG(msg->getReal());
      ok = true;
   }
   return ok;
}

bool Missile::setSlotMaxAccel(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      setMaxAccel(msg->getReal());
      ok = true;
   }
   return ok;
}

bool Missile::setSlotCmdPitch(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      cmdPitch = msg->getReal();
      ok = true;
   }
   return ok;
}

bool Missile::setSlotCmdHeading(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      cmdHeading = msg->getReal();
      ok = true;
   }
   return ok;
}

bool Missile::setSlotCmdVelocity(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      cmdVelocity = msg->getReal();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// weaponGuidance() -- default guidance; using Robot Aircraft (RAC) guidance
//------------------------------------------------------------------------------
void Missile::weaponGuidance(const LCreal dt)
{

   // ---
   // Control velocity:  During burn time, accel to max velocity,
   //  after burn time, deaccelerate to min velocity.
   // ---
   if (isEngineBurnEnabled()) cmdVelocity = vpMax;
   else cmdVelocity = vpMin;

   // ---
   // If the target's already dead,
   //    then don't go away mad, just go away.
   // ---
   const Player* tgt = getTargetPlayer();
   const Track* trk = getTargetTrack();
   if (trk != 0) tgt = trk->getTarget();

   if (tgt != 0 && !tgt->isActive()) return;

   osg::Vec3 los; // Target Line of Sight
   osg::Vec3 vel; // Target velocity

   // ---
   // Basic guidance
   // ---
   {
      // ---
      // Get position and velocity vectors from the target/track
      // ---
      osg::Vec3 posx;
      calculateVectors(tgt, trk, &los, &vel, &posx);

      // compute range to target
      LCreal trng0 = trng;
      trng = los.length();

      // compute range rate,
      LCreal trdot0 = trdot;
      if (dt > 0)
         trdot = (trng - trng0)/dt;
      else
         trdot = 0;

      // Target total velocit
      LCreal totalVel = vel.length();

      // compute target velocity parallel to LOS,
      LCreal vtplos = (los * vel/trng);

      // ---
      // guidance - fly to intercept point
      // ---

      // if we have guidance ...
      if ( isGuidanceEnabled() && trng > 0) {

         // get missile velocity (must be faster than target),
         LCreal v = vpMax;
         if (v < totalVel) v = totalVel + 1;

         // compute target velocity normal to LOS squared,
         LCreal tgtVp = totalVel;
         LCreal vtnlos2 = tgtVp*tgtVp - vtplos*vtplos;

         // and compute missile velocity parallex to LOS.
         LCreal vmplos = lcSqrt( v*v - vtnlos2 );

         // Now, use both velocities parallel to LOS to compute
         //  closure rate.
         LCreal vclos = vmplos - vtplos;

         // Use closure rate and range to compute time to intercept.
         LCreal dt1 = 0;
         if (vclos > 0) dt1 = trng/vclos;

         // Use time to intercept to extrapolate target position.
         osg::Vec3 p1 = (los + (vel * dt1));

         // Compute missile commanded heading and
         cmdHeading = lcAtan2(p1.y(),p1.x());

         // commanded pitch.
         LCreal grng = lcSqrt(p1.x()*p1.x() + p1.y()*p1.y());
         cmdPitch = -lcAtan2(p1.z(),grng);

      }
   }

   // ---
   // fuzing logic  (let's see if we've scored a hit)
   //  (compute range at closest point and compare to max burst radius)
   //  (use target truth data)
   // ---
   {
      // ---
      // Get position and velocity vectors from the target (truth)
      // (or default to the values from above)
      // ---
      if (tgt != 0) {
         calculateVectors(tgt, 0, &los, &vel, 0);
      }

      // compute range to target
      LCreal trng0 = trngT;
      trngT = los.length();

      // compute range rate,
      LCreal trdot0 = trdotT;
      if (dt > 0)
         trdotT = (trngT - trng0)/dt;
      else
         trdotT = 0;

      // when we've just passed the target ...
      if (trdotT > 0 && trdot0 < 0 && !isDummy() && getTOF() > 2.0f) {
         bool missed = true;   // assume the worst

         // compute relative velocity vector.
         osg::Vec3 velRel = (vel - getVelocity());

         // compute missile velocity squared,
         LCreal vm2 = velRel.length2();
         if (vm2 > 0) {

            // relative range (dot) relative velocity
            LCreal rdv = los * velRel;

            // interpolate back to closest point
            LCreal ndt = -rdv/vm2;
            osg::Vec3 p0 = los + (velRel*ndt);

            // range squared at closest point
            LCreal r2 = p0.length2();

            // compare to burst radius squared
            if (r2 <= (getMaxBurstRng()*getMaxBurstRng()) ) {

               // We've detonated
               missed = false;
               setMode(DETONATED);
               setDetonationResults( DETONATE_ENTITY_IMPACT );

               // compute location of the detonation relative to the target
               osg::Vec3 p0n = -p0;
               if (tgt != 0) p0n = tgt->getRotMat() * p0n;
               setDetonationLocation(p0n);

               // Did we hit anyone?
               checkDetonationEffect();

               // Log the event
               LCreal detRange = getDetonationRange();
               if (isMessageEnabled(MSG_INFO)) {
                  std::cout << "DETONATE_ENTITY_IMPACT rng = " << detRange << std::endl;
               }
               if (getAnyEventLogger() != 0) {
                  TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(2, getLaunchVehicle(), this, getTargetPlayer(), DETONATE_ENTITY_IMPACT, detRange); // type 2 for "detonate"
                  getAnyEventLogger()->log(evt);
                  evt->unref();
               }
            }
         }

         // Did we miss the target?
         if (missed) {
            // We've detonated ...
            setMode(DETONATED);
            setDetonationResults( DETONATE_DETONATION );

            // because we've just missed the target
            setTargetPlayer(0,false);
            setTargetTrack(0,false);

            // Log the event
            LCreal detRange = trngT;
            if (isMessageEnabled(MSG_INFO)) {
               std::cout << "DETONATE_OTHER rng = " << detRange << std::endl;
            }
            if (getAnyEventLogger() != 0) {
               TabLogger::TabLogEvent* evt = new TabLogger::LogWeaponActivity(2, getLaunchVehicle(), this, getTargetPlayer(), DETONATE_DETONATION, getDetonationRange()); // type 2 for "detonate"
               getAnyEventLogger()->log(evt);
               evt->unref();
            }
         }

      }
   }
}

//------------------------------------------------------------------------------
// weaponDynamics -- default missile dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void Missile::weaponDynamics(const LCreal dt)
{
   static const LCreal g = ETHG;              // Acceleration of Gravity

   // ---
   // Max turning G (Missiles: Use Gmax)
   // ---
   LCreal gmax = maxG;

   // ---
   // Computer max turn rate, max/min pitch rates
   // ---

   // Turn rate base on vp and g,s
   LCreal ra_max = gmax * g / getTotalVelocity();

   // Set max (pull up) pitch rate same as turn rate
   LCreal qa_max = ra_max;

   // Set min (push down) pitch rate
   LCreal qa_min = -qa_max;

   // ---
   // Get old angular values 
   // ---
   const osg::Vec3 oldRates = getAngularVelocities();
   //LCreal pa1 = oldRates[IROLL];
   LCreal qa1 = oldRates[IPITCH];
   LCreal ra1 = oldRates[IYAW];

   // ---
   // Find pitch rate and update pitch
   // ---
   LCreal qa = lcAepcRad(cmdPitch - (LCreal) getPitchR());
   if(qa > qa_max) qa = qa_max;
   if(qa < qa_min) qa = qa_min;

   // Using Pitch rate, integrate pitch
   LCreal newTheta = (LCreal) (getPitch() + (qa + qa1) * dt / 2.0);

   // Find turn rate
   LCreal ra = lcAepcRad(cmdHeading - (LCreal) getHeadingR());
   if(ra > ra_max) ra = ra_max;
   if(ra < -ra_max) ra = -ra_max;

   // Use turn rate integrate heading
   LCreal newPsi = (LCreal) (getHeading() + (ra + ra1) * dt / 2.0);
   if(newPsi > 2.0f*PI) newPsi -= (LCreal)(2.0*PI);
   if(newPsi < 0.0f) newPsi += (LCreal)(2.0*PI);

   // Roll angle proportional to max turn rate - filtered
   LCreal pa = 0.0;
   LCreal newPhi = (LCreal) ( 0.98 * getRollR() + 0.02 * ((ra / ra_max) * (Basic::Angle::D2RCC * 60.0)) );

   // Sent angular values
   setEulerAngles(newPhi, newTheta, newPsi);
   setAngularVelocities(pa, qa, ra);

   // Find Acceleration
   LCreal vpdot = (cmdVelocity - getTotalVelocity());
   if(vpdot > maxAccel)  vpdot = maxAccel;
   if(vpdot < -maxAccel) vpdot = -maxAccel;

   // Set acceleration vector
   osg::Vec3 aa(vpdot, 0.0, 0.0);
   osg::Vec3 ae = aa * getRotMat();
   setAcceleration(ae);

   // Comute new velocity
   LCreal newVP = getTotalVelocity() + vpdot * dt;

   // Set acceleration vector
   osg::Vec3 ve0 = getVelocity();
   osg::Vec3 va(newVP, 0.0, 0.0);
   osg::Vec3 ve1 = va * getRotMat();
   setVelocity(ve1);
   setVelocityBody(newVP, 0.0, 0.0);
}

// setVpMin() -- set min Vp
bool Missile::setVpMin(const LCreal v)
{
   vpMin =  v;
   return true;
}

// setVpMax() -- set max Vp
bool Missile::setVpMax(const LCreal v)
{
   vpMax =  v;
   return true;
}

// setVpMaxG() -- Set Vp with max G's
bool Missile::setVpMaxG(const LCreal v)
{
   vpMaxG =  v;
   return true;
}

// setMaxG() -- Set max G's (g)
bool Missile::setMaxG(const LCreal v)
{
   maxG = v;
   return true;
}

// setMaxAccel() -- Max acceleration (m/s/s)
bool Missile::setMaxAccel(const LCreal v)
{
   maxAccel =  v;
   return true;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Missile::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Missile::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFormName() << std::endl;
      j = 4;
   }


   indent(sout,i+j);
   sout << "minSpeed: " << vpMin << std::endl;

   indent(sout,i+j);
   sout << "maxSpeed: " << vpMax << std::endl;

   indent(sout,i+j);
   sout << "speedMaxG: " << vpMaxG << std::endl;

   indent(sout,i+j);
   sout << "maxg: " << maxG << std::endl;

   indent(sout,i+j);
   sout << "maxAccel: " << maxAccel << std::endl;

   indent(sout,i+j);
   sout << "cmdPitch: " << cmdPitch << std::endl;

   indent(sout,i+j);
   sout << "cmdHeading: " << cmdHeading << std::endl;

   indent(sout,i+j);
   sout << "cmdSpeed: " << cmdVelocity << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
