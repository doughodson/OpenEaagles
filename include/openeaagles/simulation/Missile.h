//------------------------------------------------------------------------------
// Class: Missile
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Missile_H__
#define __Eaagles_Simulation_Missile_H__

#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
   namespace Basic { class Number; }
namespace Simulation {

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

    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
    virtual void atReleaseInit();
    virtual void setCmdPitchD(const LCreal x) { cmdPitch = x * (LCreal) Basic::Angle::D2RCC; }
    virtual void setCmdHdgD(const LCreal x) { cmdHeading = x * (LCreal) Basic::Angle::D2RCC; }
   virtual bool setTargetTrack(Track* const trk, const bool posTrkEnb);
   virtual bool setTargetPlayer(Player* const tgt, const bool posTrkEnb);

    // Basic::Component interface
    virtual bool event(const int event, Basic::Object* const obj = 0);
    virtual void reset();    

protected:
   virtual bool setSlotVpMin(const Basic::Number* const msg);
   virtual bool setSlotVpMax(const Basic::Number* const msg);
   virtual bool setSlotVpMaxG(const Basic::Number* const msg);
   virtual bool setSlotMaxG(const Basic::Number* const msg);
   virtual bool setSlotMaxAccel(const Basic::Number* const msg);
   virtual bool setSlotCmdPitch(const Basic::Number* const msg);
   virtual bool setSlotCmdHeading(const Basic::Number* const msg);
   virtual bool setSlotCmdVelocity(const Basic::Number* const msg);

   // Weapon interface
   virtual void weaponGuidance(const LCreal dt);
   virtual void weaponDynamics(const LCreal dt);

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

} // End Simulation namespace
} // End Eaagles namespace

#endif
