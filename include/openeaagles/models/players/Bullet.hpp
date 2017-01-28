
#ifndef __oe_models_Bullet_H__
#define __oe_models_Bullet_H__

#include "openeaagles/models/players/AbstractWeapon.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Bullet
//
// Description: General bullet class.
//
//    Provides a description of the bullet.  It is used to create the "flyout"
//    weapon player.  During flyout, the bullets are grouped into bursts.
//
// Factory name: Bullet
//------------------------------------------------------------------------------
class Bullet : public AbstractWeapon
{
    DECLARE_SUBCLASS(Bullet, AbstractWeapon)

public:
   static const double DEFAULT_MUZZLE_VEL;         // Meters / second
   static const double DEFAULT_MAX_TOF;            // Seconds

public:
   Bullet();

   double getMuzzleVelocity() const                { return muzzleVel; }

   // Fire (add) a burst of bullets
   virtual bool burstOfBullets(
      const base::Vec3d* const pos,     // Initial burst position
      const base::Vec3d* const vel,     // Initial velocity of bullets
      const int num,                    // Number of bullets in burst
      const int rate,                   // Rate of fire (rounds per minute)
      const int re                      // Release event id
   );

   virtual const char* getDescription() const override;
   virtual const char* getNickname() const override;
   virtual int getCategory() const override;

   virtual void reset() override;

protected:
   virtual void resetBurstTrajectories();
   virtual void updateBurstTrajectories(const double dt);
   virtual bool checkForTargetHit();

   Player* getHitPlayer()                 { return hitPlayer; }
   const Player* getHitPlayer() const     { return hitPlayer; }
   void setHitPlayer(Player* p);

   virtual void weaponDynamics(const double dt) override;
   virtual void updateTOF(const double dt) override;

   virtual bool shutdownNotification() override;

   struct Burst {
      enum Status { ACTIVE, HIT, MISS };
      Burst() : bPos(0,0,0), bVel(0,0,0), bTof(0), bNum(0), bRate(0), bEvent(0), bStatus(ACTIVE) {}
      base::Vec3d bPos;        // Burst positions -- world  (m)
      base::Vec3d bVel;        // Burst velocities -- world (m)
      double    bTof;          // Burst time of flight      (sec)
      int       bNum;          // Number of rounds in burst
      int       bRate;         // Round rate for this burst (rds per min)
      int       bEvent;        // Release event number for burst
      Status    bStatus;       // Burst status
   };

private:
   enum { MBT = 100 };        // Max number of burst trajectories

   double   muzzleVel;                 // Muzzle velocity (m/s)
   base::safe_ptr<Player> hitPlayer;  // Player we hit (if any)

   // Bullet trajectories
   int       nbt;             // Number of burst trajectories
   Burst bursts[MBT];         // Bursts
};

}
}

#endif
