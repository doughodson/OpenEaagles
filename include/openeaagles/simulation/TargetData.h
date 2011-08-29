//------------------------------------------------------------------------------
// Classes:     TargetData
//------------------------------------------------------------------------------
#ifndef __TargetData_H_9208838A_6336_4d68_B097_961991628378__
#define __TargetData_H_9208838A_6336_4d68_B097_961991628378__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
   namespace Basic {
      class Boolean;
      class Integer;
      class Number;
      class Identifier;
      class String;
   }
namespace Simulation {

//------------------------------------------------------------------------------
// Class:     TargetData
// Base class:  Basic::Object -> TargetData
//
// Description: Target-specific data for a steerpoint
//
// Form name: TargetData
// Slots:
//    enabled          <Basic::Boolean>    ! Target enabled flag (default: false)
//    completed        <Basic::Boolean>    ! Target completed flag (default: false)
//    weaponType       <Basic::String>     ! Type of weapon to release (default: 0)
//    quantity         <Basic::Integer>    ! Number of weapons to release (default: 0)
//    stickDistance    <Basic::Number>     ! Weapon stick length (feet) (default: 0)
//    stickType        <Basic::Identifier> ! Weapon stick option (MIDPOINT, LEADING_EDGE) (default: MIDPOINT)
//    interval         <Basic::Number>     ! Time between weapon releases (ms) (default: 0)
//    maxMissDistance  <Basic::Number>     ! Maximum miss distance (feet) (default: 0)
//    manualAssign     <Basic::Boolean>    ! Manually assign weapon to target? (default: false)
//    armDelay         <Basic::Number>     ! Arming delay (0-99.9 sec) (default: 0)
//    angle            <Basic::Number>     ! Impact angle (deg) (default: 0)
//    azimuth          <Basic::Number>     ! Azimuth angle (deg) (default: 0)
//    velocity         <Basic::Number>     ! Impact velocity (ft/sec) (default: 0)
//------------------------------------------------------------------------------
class TargetData : public Basic::Object
{
   DECLARE_SUBCLASS(TargetData,Basic::Object)

public:
    enum StickType { MIDPOINT, LEADING_EDGE };

public:
    TargetData();

    bool isEnabled() { return enabled; }
    bool isCompleted() { return completed; }
    const Basic::String* const getWpnType() { return wpnType; }
    int getQuantity() { return quantity; }
    StickType getStickType() { return stickType; }
    LCreal getStickDistance() { return stickDist; }
    LCreal getMaxMissDistance() { return maxMissDist; }
    LCreal getInterval() { return interval; }
    bool getManualAssign() { return manualAssign; }
    LCreal getArmDelay() { return armDelay; }
    LCreal getAngle() { return angle; }
    LCreal getAzimuth() { return azimuth; }
    LCreal getVelocity() { return velocity; }

    void setEnabled(bool b) { enabled = b; }
    void setCompleted(bool b) { completed = b; }
    void setWpnType(const Basic::String* s);
    void setQuantity(int q) { quantity = q; }
    void setStickType(StickType t) { stickType = t; }
    void setStickDistance(LCreal d) { stickDist = d; }
    void setMaxMissDistance(LCreal d) { maxMissDist = d; }
    void setInterval(LCreal t) { interval = t; }
    void setManualAssign(bool b) { manualAssign = b; }
    void setArmDelay(LCreal n) { armDelay = n; }
    void setAngle(LCreal n) { angle = n; }
    void setAzimuth(LCreal n) { azimuth = n; }
    void setVelocity(LCreal n) { velocity = n; }

protected:
    bool setSlotEnabled(const Basic::Boolean* b);
    bool setSlotCompleted(const Basic::Boolean* b);
    bool setSlotWpnType(const Basic::String* s);
    bool setSlotQuantity(const Basic::Integer* i);
    bool setSlotStickType(const Basic::Identifier* s);
    bool setSlotStickDistance(const Basic::Number* n);
    bool setSlotMaxMissDistance(const Basic::Number* n);
    bool setSlotInterval(const Basic::Number* n);
    bool setSlotManualAssign(const Basic::Boolean* b);
    bool setSlotArmDelay(const Basic::Number* n);
    bool setSlotAngle(const Basic::Number* n);
    bool setSlotAzimuth(const Basic::Number* n);
    bool setSlotVelocity(const Basic::Number* n);

private:
    bool enabled;
    bool completed;
    const Basic::String* wpnType;

    LCreal armDelay;
    LCreal angle;
    LCreal azimuth;
    LCreal velocity;

    int quantity;
    StickType stickType;
    LCreal stickDist;
    LCreal maxMissDist;
    LCreal interval;
    bool manualAssign;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __TargetData_H_9208838A_6336_4d68_B097_961991628378__
