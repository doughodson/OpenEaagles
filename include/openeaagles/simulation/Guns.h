//------------------------------------------------------------------------------
// Classes: Gun, Bullet
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Gun_H__
#define __oe_simulation_Gun_H__

#include "openeaagles/simulation/ExternalStore.h"
#include "openeaagles/simulation/Weapon.h"

namespace oe {
namespace simulation {
   class Bullet;

//==============================================================================
// Class: Gun
// Description: Base class for guns; with a simple, default gun model
//
//    The Gun class is derived from the ExternalStore class because it's a system
//    that's attached to the aircraft.  It is not released and can not become an
//    independent player, and it is updated by the background and time-critical
//    threads.
//
// Factory name: Gun
// Slots:
//    bulletType  <Bullet>   ! Type of bullet (have to have bullets to work) (default: 0)
//    rounds      <Number>   ! Number of rounds (default: 510)
//    unlimited    <Number>   ! Unlimited rounds flag (default: false)
//    rate        <Number>   ! Rate of fire (rounds per minute) (default: 6600)
//    burstRate   <Number>   ! Rate to generate small bursts of bullets (default: 10)
//
//    The following are used to position the gun on the ownship player
//    position    <List>     ! Position vector; relative to ownship axis [ nose right down ]  (meters)
//    roll        <Number>   ! roll angle; relative to ownship axis (radians, base::Angle) (default: 0)
//    pitch       <Number>   ! pitch; relative to ownship axis (radians, base::Angle) (default: 0)
//    yaw         <Number>   ! heading; relative to ownship axis (radians, base::Angle) (default: 0)
//
//==============================================================================
class Gun : public ExternalStore
{
    DECLARE_SUBCLASS(Gun,ExternalStore)

public:
   static const int DEFAULT_ROUNDS_PER_MINUTE;  // Default rate (rds per min)
   static const int DEFAULT_NUM_ROUNDS;         // Default load
   static const int DEFAULT_BURST_RATE;         // Default subburst frame rate

public:
   Gun();

   int getRoundsRemaining() const                  { return rounds; }

   int getMaxRounds() const                        { return initRounds; }
   virtual bool setMaxRounds(const int value);

   bool isUnlimited() const                        { return unlimited; }
   virtual bool setUnlimited(const bool flg);

   int getRoundsPerMinute() const                  { return rpm; }
   virtual bool setRoundsPerMinute(const int value);

   // Gun arming
   bool isGunArmed() const                         { return armed; }
   virtual bool setGunArmed(const bool flg);

   // Gun fire
   bool isGunFiring() const                         { return fire; }
   virtual bool fireControl(const bool trigger, const bool burst = false);

   // Reload the gun & bullets
   virtual void reload();
   Bullet* getBulletType()                         { return bullet; }
   const Bullet* getBulletType() const             { return bullet; }
   virtual bool setBulletType(Bullet* const b);

   // Gun position relative to ownship player
   const osg::Vec3d& getPosition() const           { return posVec; }
   const osg::Vec3d& getAngles() const             { return angles; }
   const osg::Matrixd& getRotMat() const           { return rm; }
   double getRoll() const                          { return angles[0]; }
   double getPitch() const                         { return angles[1]; }
   double getYaw() const                           { return angles[2]; }
   virtual void setPitch(const double x)           { angles[1] = x; }
   bool setPosition(const double x, const double y, const double z);
   bool setAngles(const double r, const double p, const double y);

   // Slot functions
   virtual bool setSlotNumRounds(const base::Number* const num);  // Number of rounds
   virtual bool setSlotUnlimited(const base::Number* const num);  // Unlimited rounds flag
   virtual bool setSlotRate(const base::Number* const num);       // Rate of fire (rds per min)
   virtual bool setSlotBurstRate(const base::Number* const num);  // Burst rate
   virtual bool setSlotPosition(base::List* const numList);       // Gun position relative to ownship
   virtual bool setSlotRoll(const base::Number* const num);       // Gun roll angle to ownship
   virtual bool setSlotPitch(const base::Number* const num);      // Gun pitch angle to ownship
   virtual bool setSlotYaw(const base::Number* const num);        // Gun heading angle to ownship

   void reset() override;

protected:
   virtual LCreal computeBulletRatePerSecond();

   virtual void initBullet();
   virtual void burstFrame();
   virtual osg::Vec3d computeInitBulletPosition();
   virtual osg::Vec3d computeInitBulletVelocity();

   void process(const LCreal dt) override;

   bool shutdownNotification() override;

private:
   Bullet*  bullet;           // Type of bullet
   LCreal   shortBurstTimer;  // Single short burst timer (when firing a single burst) (sec)
   LCreal   shortBurstTime;   // Length of the single short burst                 (sec)
   LCreal   burstFrameTimer;  // Timer used to generate small bursts bullets      (sec)
   LCreal   burstFrameTime;   // Length of a small burst of bullets               (sec)
   LCreal   rcount;           // Rounds being fired this burst
   int      rounds;           // Number of rounds remaining
   int      initRounds;       // Initial rounds
   int      rpm;              // Rounds per minute
   bool     fire;             // Gun is firing
   bool     armed;            // Armed flag
   bool     unlimited;        // Unlimited bullets (aka old western movie mode)

   osg::Vec3d posVec;         // Position vector; relative to ownship body axis (m)
   osg::Vec3d angles;         // (Euler) angles; relative to ownship body axis (rad)
   osg::Matrixd rm;           // Rotational Matrix; relative to ownship body axis
};

//==============================================================================
// Class: Bullet
//
// Description: General bullet class.
//
//    Provides a description of the bullet.  It is used to create the "flyout"
//    weapon player.  During flyout, the bullets are grouped into bursts.
//
// Factory name: Bullet
//==============================================================================
class Bullet : public Weapon
{
    DECLARE_SUBCLASS(Bullet,Weapon)

public:
   static const LCreal DEFAULT_MUZZLE_VEL;         // Meters / second
   static const LCreal DEFAULT_MAX_TOF;            // Seconds

public:
   Bullet();

   LCreal getMuzzleVelocity() const                { return muzzleVel; }

   // Fire (add) a burst of bullets
   virtual bool burstOfBullets(
      const osg::Vec3* const pos,     // Initial burst position
      const osg::Vec3* const vel,     // Initial velocity of bullets
      const int num,                  // Number of bullets in burst
      const int rate,                 // Rate of fire (rounds per minute)
      const int re                    // Release event id
   );

   const char* getDescription() const override;
   const char* getNickname() const override;
   int getCategory() const override;

   void reset() override;

protected:
   virtual void resetBurstTrajectories();
   virtual void updateBurstTrajectories(const LCreal dt);
   virtual bool checkForTargetHit();

   Player* getHitPlayer()                 { return hitPlayer; }
   const Player* getHitPlayer() const     { return hitPlayer; }
   void setHitPlayer(Player* p);

   void weaponDynamics(const LCreal dt) override;
   void updateTOF(const LCreal dt) override;

   bool shutdownNotification() override;

   struct Burst {
      enum Status { ACTIVE, HIT, MISS };
      Burst() : bPos(0,0,0), bVel(0,0,0), bTof(0), bNum(0), bRate(0), bEvent(0), bStatus(ACTIVE) {}
      osg::Vec3 bPos;       // Burst positions -- world  (m)
      osg::Vec3 bVel;       // Burst velocities -- world (m)
      LCreal    bTof;       // Burst time of flight      (sec)
      int       bNum;       // Number of rounds in burst
      int       bRate;      // Round rate for this burst (rds per min)
      int       bEvent;     // Release event number for burst
      Status    bStatus;    // Burst status
   };

private:
   enum { MBT = 100 };        // Max number of burst trajectories

   LCreal   muzzleVel;                 // Muzzle velocity (m/s)
   base::safe_ptr<Player> hitPlayer;  // Player we hit (if any)

   // Bullet trajectories
   int       nbt;             // Number of burst trajectories
   Burst bursts[MBT];         // Bursts
};

} // End simulation namespace
} // End oe namespace

#endif
