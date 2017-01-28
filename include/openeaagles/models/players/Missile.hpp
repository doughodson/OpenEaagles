
#ifndef __oe_models_Missile_H__
#define __oe_models_Missile_H__

#include "openeaagles/models/players/AbstractWeapon.hpp"

#include "openeaagles/base/util/unit_utils.hpp"

namespace oe {
namespace base { class Number; }
namespace models {
class Player;
class Track;

//------------------------------------------------------------------------------
// Class: Missile
// Description: Base class for missiles; with a simple, default missile model
//
// Factory name: Missile
// Slots:
//   minSpeed     <Number>  ! Minimum Velocity (m/s)  (default: 0.0)
//   maxSpeed     <Number>  ! Maximum Velocity (m/s)  (default: 800.0)
//   speedMaxG    <Number>  ! Velocity we reach max G (default: 800.0)
//   maxg         <Number>  ! Max G's (at "speedMaxG" or above) (default: 4.0)
//   maxAccel     <Number>  ! Maximum Acceleration (m/s/s) (default: 50.0)
//   cmdPitch     <Number>  ! Command Pitch (rad) (default: 0.0)
//   cmdHeading   <Number>  ! Command Heading (rad) (default: 0.0)
//   cmdSpeed     <Number>  ! Command speed (m/s) (default: 0.0)
//
//------------------------------------------------------------------------------
class Missile : public AbstractWeapon
{
    DECLARE_SUBCLASS(Missile, AbstractWeapon)

public:
    Missile();

    // get functions
    double getVpMin() const         { return vpMin; }
    double getVpMax() const         { return vpMax; }
    double getVpMaxG() const        { return vpMaxG; }
    double getMaxG() const          { return maxG; }
    double getMaxAccel() const      { return maxAccel; }
    double getMinAccel() const      { return 0.0; }

    // set functions
    bool setVpMin(const double v);
    bool setVpMax(const double v);
    bool setVpMaxG(const double v);
    bool setMaxG(const double v);
    bool setMaxAccel(const double v);

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
    virtual void atReleaseInit() override;

    virtual void setCmdPitchD(const double x)  { cmdPitch   = x * static_cast<double>(base::angle::D2RCC); }
    virtual void setCmdHdgD(const double x)    { cmdHeading = x * static_cast<double>(base::angle::D2RCC); }

    virtual bool setTargetTrack(Track* const trk, const bool posTrkEnb) override;
    virtual bool setTargetPlayer(Player* const tgt, const bool posTrkEnb) override;

    virtual bool event(const int event, base::Object* const obj = nullptr) override;
    virtual void reset() override;

protected:
   virtual bool setSlotVpMin(const base::Number* const msg);
   virtual bool setSlotVpMax(const base::Number* const msg);
   virtual bool setSlotVpMaxG(const base::Number* const msg);
   virtual bool setSlotMaxG(const base::Number* const msg);
   virtual bool setSlotMaxAccel(const base::Number* const msg);
   virtual bool setSlotCmdPitch(const base::Number* const msg);
   virtual bool setSlotCmdHeading(const base::Number* const msg);
   virtual bool setSlotCmdVelocity(const base::Number* const msg);

   // Weapon interface
   virtual void weaponGuidance(const double dt) override;
   virtual void weaponDynamics(const double dt) override;

private:
    virtual bool calculateVectors(const Player* const tgt, const Track* const trk, base::Vec3d* const los, base::Vec3d* const vel, base::Vec3d* const posx) const;

   // ---
   // Default guidance & dynamics parameters
   // ---
   double      trng;           // target range             (m)
   double      trngT;          // target range (truth)     (m)
   double      trdot;          // target range rate        (m/s)
   double      trdotT;          // target range rate (truth)(m/s)
   double      cmdPitch;       // Commanded Pitch          (rad)
   double      cmdHeading;     // Commanded Heading        (rad)
   double      cmdVelocity;    // Commanded speed          (m/s)
   double      vpMin;          // Minimum Velocity         (m/s)
   double      vpMax;          // Maximum Velocity         (m/s)
   double      maxAccel;       // Max longitudual acceleration ((f/s)/s)
   double      maxG;           // Max lateral G's (pitch/yaw)  (gees)
   double      vpMaxG;         // Velocity for Max G's     (gees)

};

}
}

#endif
