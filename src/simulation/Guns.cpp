
#include "openeaagles/simulation/Guns.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/DataRecorder.h"
#include "openeaagles/simulation/TabLogger.h"
#include "openeaagles/simulation/Track.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/PairStream.h"

#include <cmath>

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: Bullet
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Bullet,"Bullet")
EMPTY_SERIALIZER(Bullet)

// Default Parameters
const LCreal Bullet::DEFAULT_MUZZLE_VEL = 1000.0f;     // Meters / second
const LCreal Bullet::DEFAULT_MAX_TOF = 3.0f;           // Seconds

int Bullet::getCategory() const               { return (GRAVITY); }
const char* Bullet::getDescription() const    { return "Bullets"; }
const char* Bullet::getNickname() const       { return "Bullets"; }


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Bullet::Bullet()
{
   STANDARD_CONSTRUCTOR()

   static Basic::String generic("Bullet");
   setType(&generic);

   nbt = 0;

   setMaxTOF( DEFAULT_MAX_TOF );

   muzzleVel = DEFAULT_MUZZLE_VEL;
   hitPlayer = nullptr;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Bullet::copyData(const Bullet& org, const bool)
{

   BaseClass::copyData(org);

   nbt = 0;
   hitPlayer = nullptr;

   for (int i = 0; i < MBT; i++) {
      bursts[i].bPos = org.bursts[i].bPos;
      bursts[i].bVel = org.bursts[i].bVel;
      bursts[i].bTof = org.bursts[i].bTof;
      bursts[i].bNum = org.bursts[i].bNum;
      bursts[i].bRate = org.bursts[i].bRate;
      bursts[i].bEvent = org.bursts[i].bEvent;
   }
}

void Bullet::deleteData()
{
   setHitPlayer(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool Bullet::shutdownNotification()
{
   setHitPlayer(nullptr);
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Bullet::reset()
{
   BaseClass::reset();
   resetBurstTrajectories();
   setHitPlayer(nullptr);
}

//------------------------------------------------------------------------------
// weaponDynamics() -- Bullet dynamics
//------------------------------------------------------------------------------
void Bullet::weaponDynamics(const LCreal dt)
{
   if (isMode(ACTIVE)) {
      updateBurstTrajectories(dt);
      checkForTargetHit();

      // This weapon is slaved to the first burst!
      if (nbt > 0) {

         // We control the position and altitude!
         setPosition( bursts[0].bPos[0], bursts[0].bPos[1], bursts[0].bPos[2], true );

         setVelocity( bursts[0].bVel );

         setAcceleration( 0, 0, 0 );

         setEulerAngles(  0, 0, getGroundTrack() );

         setAngularVelocities( 0, 0, 0 );

         setVelocityBody ( bursts[0].bVel.length(), 0, 0 );
      }
   }
}

//------------------------------------------------------------------------------
// updateTOF -- update time of flight (TOF) and set the missed status
//------------------------------------------------------------------------------
void Bullet::updateTOF(const LCreal)
{
   // As long as we're active ...
   if (isMode(ACTIVE)) {

      // count the number of active bursts and remove aged bullet bursts.
      int n = 0;
      int nhits = 0;
      for (int i = 0; i < nbt; i++) {
         if (bursts[i].bStatus == Burst::ACTIVE) {
            n++;
            if ( bursts[i].bTof >= getMaxTOF() ) {
               bursts[i].bStatus = Burst::MISS;
            }
         }
         else if (bursts[i].bStatus == Burst::HIT) {
            nhits++;
         }
      }

      // If we have no active bursts .. we've detonated (so to speak)
      if (n == 0) {
         setMode(DETONATED);
         // final detonation results (hit or miss) are located with each burst ...
         if (nhits > 0) {
            setDetonationResults( DETONATE_ENTITY_IMPACT );
         }
         else {
            setDetonationResults( DETONATE_NONE );
         }
         // final time of flight (slave to the first burst)
         setTOF( bursts[0].bTof );
      }
   }
}

//------------------------------------------------------------------------------
// resetBulletTrajectories() -- reset the bullet trajectories
//------------------------------------------------------------------------------
void Bullet::resetBurstTrajectories()
{
   nbt = 0;
}

//------------------------------------------------------------------------------
// burstOfBullets() -- add a burst of bullets
//------------------------------------------------------------------------------
bool Bullet::burstOfBullets(const osg::Vec3* const pos, const osg::Vec3* const vel, const int num, const int rate, const int e)
{
   if (nbt < MBT && pos != nullptr && vel != nullptr) {
      bursts[nbt].bPos = *pos;  // Burst positions -- world  (m)
      bursts[nbt].bVel = *vel;  // Burst velocities -- world (m)
      bursts[nbt].bTof = 0;    // Burst time of flight      (sec)
      bursts[nbt].bNum = num;  // Number of rounds in burst
      bursts[nbt].bRate = rate;// Round rate for this burst (rds per sec)
      bursts[nbt].bEvent = e;  // Release event number for burst
      bursts[nbt].bStatus = Burst::ACTIVE;
      nbt++;
   }
   return true;
}

//------------------------------------------------------------------------------
// updateBurstTrajectories() -- update the bursts trajectories
//------------------------------------------------------------------------------
void Bullet::updateBurstTrajectories(const LCreal dt)
{
   static const LCreal g = ETHG * Basic::Distance::FT2M;      // Acceleration of Gravity (m/s/s)

   // For all active bursts
   for (int i = 0; i < nbt; i++) {
      if (bursts[i].bStatus == Burst::ACTIVE) {
         bursts[i].bVel[Player::IDOWN] = bursts[i].bVel[Player::IDOWN] + (g*dt);  // falling bullets

         bursts[i].bPos = bursts[i].bPos + (bursts[i].bVel * dt);
         bursts[i].bTof += dt;
      }
   }
}

//------------------------------------------------------------------------------
// checkForTargetHit() -- check to see if we hit anything
//------------------------------------------------------------------------------
bool Bullet::checkForTargetHit()
{
   Player* ownship = getLaunchVehicle();
   Player* tgt = getTargetPlayer();
   if (ownship != nullptr && tgt != nullptr) {
      osg::Vec3 osPos = tgt->getPosition();

      // For all active bursts ...
      for (int i = 0; i < nbt; i++) {
         if (bursts[i].bStatus == Burst::ACTIVE) {

            // Check if we're within range of the target
            osg::Vec3 rPos = bursts[i].bPos - osPos;
            LCreal rng = rPos.length();
            if (rng < 10.0) {
               // Yes -- it's a hit!
               bursts[i].bStatus = Burst::HIT;
               setHitPlayer(tgt);
               setLocationOfDetonation();
               tgt->processDetonation(rng,this);
            }

         }
      }
   }
   // if we are just flying along, check our range to the nearest player and tell him we killed it
   else {
        //osg::Vec3 old = getEulerAngles();
        osg::Vec3 myPos = getPosition();
        osg::Vec3 tgtPos;
        osg::Vec3 vecPos;
        //LCreal az = 0;
        //LCreal el = 0;
        LCreal range = 0;
        //LCreal diffAz = 0;
        //LCreal diffEl = 0;
        LCreal maxRange = 1; // close range of detonation
        Simulation* sim = getSimulation();
        if (sim != nullptr) {
            Basic::PairStream* players = sim->getPlayers();
            if (players != nullptr) {
                Basic::List::Item* item = players->getFirstItem();
                while (item != nullptr) {
                    Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
                    if (pair != nullptr) {
                        Player* player = dynamic_cast<Player*>(pair->object());
                        if (player != nullptr && player != ownship && player->isMajorType(LIFE_FORM) && !player->isDestroyed()) {
                            // ok, calculate our position from this guy
                            tgtPos = player->getPosition();
                            vecPos = tgtPos - myPos;
                            //az = lcAtan2(vecPos.y(), vecPos.x());
                            range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                            range = std::sqrt(range);
                            if (range < maxRange) {
                                // tell this target we hit it
                                player->processDetonation(range, this);
                            }
                        }
                    }
                    item = item->getNext();
                }
                players->unref();
                players = nullptr;
            }
        }

   }
   return false;
}

//------------------------------------------------------------------------------
// setHitPlayer() -- set a pointer to the player we just hit
//------------------------------------------------------------------------------
void Bullet::setHitPlayer(Player* p)
{
   hitPlayer = p;
}


//==============================================================================
// Class: Gun
//==============================================================================
IMPLEMENT_SUBCLASS(Gun,"Gun")

// Default Parameters (based on GE M61A1)
const int Gun::DEFAULT_ROUNDS_PER_MINUTE = 6600;   // Default rate (rds per min)
const int Gun::DEFAULT_NUM_ROUNDS = 510;           // Default load
const int Gun::DEFAULT_BURST_RATE = 10;            // Default burst rate

// Slot table
BEGIN_SLOTTABLE(Gun)
    "bulletType",       //  1: Type of bullet (have have bullets to work)
    "rounds",           //  2: Number of rounds
    "unlimited",         //  3: Unlimited rounds flag
    "rate",             //  4: Rate of fire (rounds per minute)
    "burstRate",        //  5: Rate to generate small bursts of bullets

    // The following are used to position the gun on the ownship player
    "position",         //  6: Position vector; relative to ownship axis [ nose right down ]  (meters)
    "roll",             //  7: roll angle; relative to ownship axis (radians, Basic::Angle)
    "pitch",            //  8: pitch; relative to ownship axis (radians, Basic::Angle)
    "yaw"               //  9: heading; relative to ownship axis (radians, Basic::Angle)
END_SLOTTABLE(Gun)

// Map slot table to handles
BEGIN_SLOT_MAP(Gun)
    ON_SLOT(1, setBulletType, Bullet)
    ON_SLOT(2, setSlotNumRounds,  Basic::Number)
    ON_SLOT(3, setSlotUnlimited,  Basic::Number)
    ON_SLOT(4, setSlotRate,       Basic::Number)
    ON_SLOT(5, setSlotBurstRate,  Basic::Number)
    ON_SLOT(6, setSlotPosition,Basic::List)
    ON_SLOT(7, setSlotRoll,  Basic::Number)
    ON_SLOT(8, setSlotPitch, Basic::Number)
    ON_SLOT(9, setSlotYaw,   Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Gun::Gun()
{
   STANDARD_CONSTRUCTOR()

   bullet = nullptr;

   armed = false;
   fire = false;
   unlimited = false;

   burstFrameTimer = 0;
   burstFrameTime = 1.0f / static_cast<LCreal>(DEFAULT_BURST_RATE);
   rcount = 0.0;

   shortBurstTimer = 0.0;
   shortBurstTime  = 0.5;

   rounds = 0;
   initRounds = DEFAULT_NUM_ROUNDS;

   rpm = DEFAULT_ROUNDS_PER_MINUTE;

   setPosition(0.0, 0.0, 0.0);
   setAngles(0.0, 0.0, 0.0);
   // Note: rotation matrix (mm) was initialized by setAngles()

}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Gun::copyData(const Gun& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      bullet = nullptr;
   }

   if (org.getBulletType() != nullptr) {
      Bullet* b = org.getBulletType()->clone();
      setBulletType( b );
      b->unref();
   }
   else setBulletType(nullptr);

   armed = org.armed;

   burstFrameTimer = org.burstFrameTimer;
   burstFrameTime = org.burstFrameTime;
   rcount = org.rcount;

   fire = org.fire;
   unlimited = org.unlimited;

   shortBurstTimer = org.shortBurstTimer;
   shortBurstTime  = org.shortBurstTime;

   rounds = org.rounds;
   initRounds = org.initRounds;

   rpm = org.rpm;
}

void Gun::deleteData()
{
   setBulletType(nullptr);
}

bool Gun::shutdownNotification()
{
   if (bullet != nullptr) {
      bullet->event(SHUTDOWN_EVENT);
      bullet->unref();
      bullet = nullptr;
   }

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Gun::reset()
{
   BaseClass::reset();
   if (getBulletType() != nullptr) getBulletType()->reset();
   reload();
}

//------------------------------------------------------------------------------
// reload() -- Reload the gun
//------------------------------------------------------------------------------
void Gun::reload()
{
   rounds = initRounds;
}

//------------------------------------------------------------------------------
// Process our time-critical tasks
//------------------------------------------------------------------------------
void Gun::process(const LCreal dt)
{
   BaseClass::process(dt);

   // ---
   // Are we firing?
   // ---
   if (fire && (getRoundsRemaining() > 0 || isUnlimited()) ) {
      const LCreal rps = computeBulletRatePerSecond();
      const LCreal bpi = rps * dt;
      rcount += bpi;
   }

   // ---
   // Generate small burst of bullets at 10 hz
   // ---
   burstFrameTimer += dt;
   if (burstFrameTimer >= burstFrameTime) {
      burstFrameTimer = 0;
      if (rcount > 0) burstFrame();
   }

   // ---
   // Burst timer
   // ---
   if (shortBurstTimer > 0 && fire) {
      shortBurstTimer -= dt;
      if (shortBurstTimer <= 0) {
         shortBurstTimer = 0;
         fire = false;
      }
   }
}

//------------------------------------------------------------------------------
// computeBulletRatePerSecond() -- Compute the rate of bullets fired per second
//------------------------------------------------------------------------------
LCreal Gun::computeBulletRatePerSecond()
{
   LCreal spinUp = 1.0;
   return (static_cast<LCreal>(rpm) / 60.0f) * spinUp;
}

//------------------------------------------------------------------------------
// fireControl() -- Starts/stops gun firing;
//    'trigger' will start or stop firing;
//    if 'burst' is true, only a single burst is fired;
//    returns true if gun is now firing
//------------------------------------------------------------------------------
bool Gun::fireControl(const bool trigger, const bool burst)
{
   if (trigger && isGunArmed()) {
      // Start firing ...
      fire = true;
      initBullet();
      if (burst) shortBurstTimer = shortBurstTime;
      else shortBurstTimer = 0;
   }
   else {
      // Stop firing ...
      burstFrame(); // clear the gun of bullets to be fired
      fire = false;
      shortBurstTimer = 0;
   }
   return fire;
}

//------------------------------------------------------------------------------
// setUnlimited() -- Set the unlimited rounds flag
//------------------------------------------------------------------------------
bool Gun::setUnlimited(const bool flg)
{
   unlimited = flg;
   return true;
}

//------------------------------------------------------------------------------
// setMaxRounds() -- Set the maximum rounds (loaded)
//------------------------------------------------------------------------------
bool Gun::setMaxRounds(const int value)
{
   initRounds = value;
   return true;
}

//------------------------------------------------------------------------------
// setRoundsPerMinute() -- Set the gun's rate of fire
//------------------------------------------------------------------------------
bool Gun::setRoundsPerMinute(const int nvalue)
{
   rpm = nvalue;
   return true;
}

//------------------------------------------------------------------------------
// setGunArmed() -- Set the gun's armed flag
//------------------------------------------------------------------------------
bool Gun::setGunArmed(const bool flg)
{
   armed = flg;
   return true;
}

//------------------------------------------------------------------------------
// setBulletType() -- Set the gun's bullet type
//------------------------------------------------------------------------------
bool Gun::setBulletType(Bullet* const b)
{
   if (bullet != nullptr) {
      bullet->unref();
   }
   bullet = b;
   if (bullet != nullptr) {
      bullet->ref();
      bullet->container(this);
   }
   return true;
}

//------------------------------------------------------------------------------
// initBullet() -- init the bullet
//------------------------------------------------------------------------------
void Gun::initBullet()
{
   Bullet* p = getBulletType();
   if (p != nullptr) p->reset();
}

//------------------------------------------------------------------------------
// burstFrame() -- generate a small burst of bullets (usually every 1/10 seconds)
//------------------------------------------------------------------------------
void Gun::burstFrame()
{
   // Compute the number of rounds this burst
   int ibullets = static_cast<int>(rcount + 0.5f);

   // Do we have any bullets to fire ...
   if (ibullets > 0){

      // Decrease the number of rounds remaining
      if ( !isUnlimited() ) {
         if (ibullets > rounds) {
            ibullets = rounds;
            rcount = static_cast<LCreal>(ibullets);
         }
         rounds -= ibullets;
      }


      // Update the rounds fired count
      rcount -= ibullets;

      // Log this event
      Player* ownship = static_cast<Player*>( findContainerByType(typeid(Player)) );

      if (ownship != nullptr) {
         BEGIN_RECORD_DATA_SAMPLE( getSimulation()->getDataRecorder(), REID_GUN_FIRED )
            SAMPLE_1_OBJECT( ownship )
            SAMPLE_1_VALUE( rcount )
         END_RECORD_DATA_SAMPLE()
      }

      // TabLogger is deprecated
      if (ownship != nullptr && getAnyEventLogger() != nullptr) {
         TabLogger::TabLogEvent* evt = new TabLogger::LogGunActivity(1, ownship, ibullets); // type 1 == gun fired
         getAnyEventLogger()->log(evt);
         evt->unref();
      }

      // When we have a bullet model ... we're going to create a bullet (weapon)
      // player to flyout the rounds.
      Bullet* wpn = getBulletType();
      Simulation* sim = static_cast<Simulation*>( findContainerByType(typeid(Simulation)) );
      if (wpn != nullptr && ownship != nullptr && sim != nullptr) {

         // Compute the bullet burst's initial position and velocity
         osg::Vec3 ipos = computeInitBulletPosition();
         osg::Vec3 ivel = computeInitBulletVelocity();

         // Get the bullet player being used to fly-out the bullets
         Bullet* flyout = static_cast<Bullet*>( wpn->getFlyoutWeapon() );
         if (flyout == nullptr) {
            // If we don't have the flyout bullet (i.e., weapon and player) ... create it
            wpn->setLaunchVehicle(ownship);
            flyout = static_cast<Bullet*>( wpn->release() );
         }

            // The flyout bullet (player) will handle this burst of bullets.
         if (flyout != nullptr) {
            flyout->burstOfBullets(&ipos, &ivel, ibullets, getRoundsPerMinute(), sim->getNewWeaponEventID() );
         }

         // Cleanup
         if (flyout != nullptr) { flyout->unref(); flyout = nullptr; }

      }
   }
   else rcount = 0;
}


//------------------------------------------------------------------------------
// computeInitBulletPosition() -- compute the initial bullet position
//------------------------------------------------------------------------------
osg::Vec3d Gun::computeInitBulletPosition()
{
   osg::Vec3d pe1 = posVec;
   Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship != nullptr) {
      // Body position to earth (NED) position
      osg::Vec3d gunPosE = posVec * ownship->getRotMat();
      pe1 = gunPosE + ownship->getPosition();
   }
   return pe1;
}

//------------------------------------------------------------------------------
// computeInitBulletVelocity() -- compute the initial bullet position
//------------------------------------------------------------------------------
osg::Vec3d Gun::computeInitBulletVelocity()
{
   osg::Vec3d ve1(0,0,0);   // velocity -- earth (m/s)
   Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship != nullptr) {
      // compute the earth (NED) to gun matrix
      osg::Matrixd mm = getRotMat() * ownship->getRotMat();

      // Get muzzle velocity in gun axis and transform to earth axis
      LCreal muzzleVel = Bullet::DEFAULT_MUZZLE_VEL;
      if (getBulletType() != nullptr) muzzleVel = getBulletType()->getMuzzleVelocity();
      osg::Vec3d va(muzzleVel, 0.0, 0.0);
      ve1 = va * mm;  // same as:  ve1 = mm(T) * va
   }
   return ve1;
}


//------------------------------------------------------------------------------
// setPosition() -- set the gun's local position vector; relative to ownship axis (m)
//------------------------------------------------------------------------------
bool Gun::setPosition(const double x, const double y, const double z)
{
    posVec.set(x, y, z);
    return true;
}

//------------------------------------------------------------------------------
// setAngles() -- set the gun's angles (roll, pitch, yaw) relative to ownship axis (m)
//------------------------------------------------------------------------------
bool Gun::setAngles(const double r, const double p, const double y)
{
    // Set angles
    angles.set(r,p,y);

    // Set rotational matrix
    Basic::Nav::computeRotationalMatrix(getRoll(), getPitch(), getYaw(), &rm);

    return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// Number of rounds
bool Gun::setSlotNumRounds(const Basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setMaxRounds( num->getInt() );
   }
   return ok;
}

// Unlimited rounds flag
bool Gun::setSlotUnlimited(const Basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setUnlimited( num->getBoolean() );
   }
   return ok;
}

// Rate of fire (rds per min)
bool Gun::setSlotRate(const Basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      ok = setRoundsPerMinute( num->getInt() );
   }
   return ok;
}

// Burst rate
bool Gun::setSlotBurstRate(const Basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      const int rate = num->getInt();
      if (rate > 0 && rate <= 20) {
         burstFrameTime = 1.0f / static_cast<LCreal>(rate);
         ok = true;
      }
      else {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Player::setSlotBurstRate: invalid burst rate, valid range is 1 .. 20hz" << std::endl;
         }
   }
   }
   return ok;
}

// Gun position relative to ownship
bool Gun::setSlotPosition(Basic::List* const numList)
{
   bool ok = false;
   LCreal values[3];
   const int n = numList->getNumberList(values, 3);
   if (n == 3) {
      setPosition(values[0], values[1], values[2]);
      ok = true;
   }
   return ok;
}

// Gun roll angle to ownship
bool Gun::setSlotRoll(const Basic::Number* const num)
{
   bool ok = false;
   double value = -1000.0;

   const Basic::Angle* p = dynamic_cast<const Basic::Angle*>(num);
   if (p != nullptr) {
      Basic::Radians radian;
      value = radian.convert(*p);
   }
   else if (num != nullptr) {
      value = num->getDouble();
   }

   if (value >= -PI && value <= PI) {
      setAngles( value, getPitch(), getYaw() );
      ok = true;
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Player::setSlotInitRoll: invalid roll angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun pitch angle to ownship
bool Gun::setSlotPitch(const Basic::Number* const num)
{
   bool ok = false;
   double value = -1000.0;

   const Basic::Angle* p = dynamic_cast<const Basic::Angle*>(num);
   if (p != nullptr) {
      Basic::Radians radian;
      value = radian.convert(*p);
   }
   else if (num != nullptr) {
      value = num->getDouble();
   }

   if (value >= -PI && value <= PI) {
      setAngles( getRoll(), value, getYaw() );
      ok = true;
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotPitch: invalid pitch angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun heading angle to ownship
bool Gun::setSlotYaw(const Basic::Number* const num)
{
   bool ok = false;
   double value = -1000.0;

   const Basic::Angle* p = dynamic_cast<const Basic::Angle*>(num);
   if (p != nullptr) {
      Basic::Radians radian;
      value = radian.convert(*p);
   }
   else if (num != nullptr) {
      value = num->getDouble();
   }

   if (value >= -PI && value <= 2.0f*PI) {
      setAngles( getRoll(), getPitch(), value );
      ok = true;
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotYaw: invalid yaw angle, valid range is [-pi ... +2*pi]" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Gun::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Gun::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
