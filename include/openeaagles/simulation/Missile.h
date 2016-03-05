//------------------------------------------------------------------------------
// Class: Missile
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Missile_H__
#define __oe_simulation_Missile_H__

#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/base/units/Angles.h"

namespace oe {
   namespace base { class Number; }
namespace simulation {

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
class Missile : public Weapon
{
    DECLARE_SUBCLASS(Missile,Weapon)

public:
    Missile();

    // get functions
    LCreal getVpMin() const         { return vpMin; }
    LCreal getVpMax() const         { return vpMax; }
    LCreal getVpMaxG() const        { return vpMaxG; }
    LCreal getMaxG() const          { return maxG; }
    LCreal getMaxAccel() const      { return maxAccel; }
    LCreal getMinAccel() const      { return 0.0; }

    // set functions
    bool setVpMin(const LCreal v);
    bool setVpMax(const LCreal v);
    bool setVpMaxG(const LCreal v);
    bool setMaxG(const LCreal v);
    bool setMaxAccel(const LCreal v);

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
    void atReleaseInit() override;

    virtual void setCmdPitchD(const LCreal x)  { cmdPitch   = x * static_cast<LCreal>(base::Angle::D2RCC); }
    virtual void setCmdHdgD(const LCreal x)    { cmdHeading = x * static_cast<LCreal>(base::Angle::D2RCC); }

    bool setTargetTrack(Track* const trk, const bool posTrkEnb) override;
    bool setTargetPlayer(Player* const tgt, const bool posTrkEnb) override;

    bool event(const int event, base::Object* const obj = nullptr) override;
    void reset() override;

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
   void weaponGuidance(const LCreal dt) override;
   void weaponDynamics(const LCreal dt) override;

private:
    virtual bool calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3* const los, osg::Vec3* const vel, osg::Vec3* const posx) const;

   // ---
   // Default guidance & dynamics parameters
   // ---
   LCreal      trng;           // target range             (m)
   LCreal      trngT;          // target range (truth)     (m)
   LCreal      trdot;          // target range rate        (m/s)
   LCreal      trdotT;          // target range rate (truth)(m/s)
   LCreal      cmdPitch;       // Commanded Pitch          (rad)
   LCreal      cmdHeading;     // Commanded Heading        (rad)
   LCreal      cmdVelocity;    // Commanded speed          (m/s)
   LCreal      vpMin;          // Minimum Velocity         (m/s)
   LCreal      vpMax;          // Maximum Velocity         (m/s)
   LCreal      maxAccel;       // Max longitudual acceleration ((f/s)/s)
   LCreal      maxG;           // Max lateral G's (pitch/yaw)  (gees)
   LCreal      vpMaxG;         // Velocity for Max G's     (gees)

};

} // End simulation namespace
} // End oe namespace

#endif
