//------------------------------------------------------------------------------
// Classes: TargetData
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_TargetData_H__
#define __oe_Simulation_TargetData_H__

#include "openeaagles/basic/Object.h"

namespace oe {
   namespace basic {
      class Angle;
      class Distance;
      class Identifier;
      class Number;
      class String;
      class Time;
   }

namespace Simulation {

//------------------------------------------------------------------------------
// Class:     TargetData
// Description: General purpose target profile data that can be associated
//              with a target steerpoint or a weapon.
//
// Factory name: TargetData
// Slots:
//    enabled          <Boolean>    ! Target enabled flag (default: false)
//    completed        <Boolean>    ! Target completed flag (default: false)
//    weaponType       <String>     ! Type of weapon to release (default: 0)
//    quantity         <Integer>    ! Number of weapons to release (default: 0)
//    manualAssign     <Boolean>    ! Manually assign weapon to target? (default: false)
//    stickType        <Identifier> ! Weapon stick option (MIDPOINT, LEADING_EDGE) (default: MIDPOINT)
//
//    stickDistance    <Distance>   ! Weapon stick length (default: 0)
//    stickDistance    <Number>     ! (feet)
//
//    interval         <Time>       ! Time between weapon releases (default: 0)
//    interval         <Number>     ! (MSec)
//
//    maxMissDistance  <Distance>   ! Maximum miss distance (default: 0)
//    maxMissDistance  <Number>     ! (feet)
// 
//    armDelay         <Time>       ! Arming delay (default: 0)
//    armDelay         <Number>     ! (0-99.9 sec)
//
//    angle            <Angle>      ! Impact angle (default: 0)
//    angle            <Number>     ! (degrees)
//
//    azimuth          <Angle>      ! Azimuth angle (default: 0)
//    azimuth          <Number>     ! (degrees)
//
//    velocity         <Number>     ! Impact velocity (ft/sec) (default: 0)
//------------------------------------------------------------------------------
class TargetData : public basic::Object
{
   DECLARE_SUBCLASS(TargetData,basic::Object)

public:
   // 'Stick' types
   static const unsigned int MIDPOINT = 0;
   static const unsigned int LEADING_EDGE = 1;

public:
    TargetData();

    bool isEnabled() const;
    bool isCompleted() const;
    const basic::String* getWpnType() const;
    unsigned int getQuantity() const;
    unsigned int getStickType() const;
    double getStickDistance() const;      // Feet
    double getMaxMissDistance() const;    // Feet
    double getInterval() const;           // Milli-Seconds
    bool getManualAssign() const;
    double getArmDelay() const;           // Seconds
    double getAngle() const;              // Degrees
    double getAzimuth() const;            // Degrees
    double getVelocity() const;           // Ft/Sec

    bool setEnabled(const bool b);
    bool setCompleted(const bool b);
    bool setWpnType(const basic::String* const s);
    bool setQuantity(const unsigned int q);
    bool setStickType(const unsigned int t);
    bool setStickDistance(const double ft);
    bool setMaxMissDistance(const double ft);
    bool setInterval(const double ms);
    bool setManualAssign(const bool b);
    bool setArmDelay(const double sec);
    bool setAngle(const double degs);
    bool setAzimuth(const double degs);
    bool setVelocity(const double fps);

protected:
    bool setSlotEnabled(const basic::Number* const msg);
    bool setSlotCompleted(const basic::Number* const msg);
    bool setSlotWpnType(const basic::String* const msg);
    bool setSlotQuantity(const basic::Number* const msg);
    bool setSlotManualAssign(const basic::Number* const msg);
    bool setSlotStickType(const basic::Identifier* const msg);
    bool setSlotStickDistance(const basic::Distance* const msg);
    bool setSlotStickDistance(const basic::Number* const msg);
    bool setSlotInterval(const basic::Time* const msg);
    bool setSlotInterval(const basic::Number* const msg);
    bool setSlotMaxMissDistance(const basic::Distance* const msg);
    bool setSlotMaxMissDistance(const basic::Number* const msg);
    bool setSlotArmDelay(const basic::Time* const msg);
    bool setSlotArmDelay(const basic::Number* const msg);
    bool setSlotAngle(const basic::Angle* const msg);
    bool setSlotAngle(const basic::Number* const msg);
    bool setSlotAzimuth(const basic::Angle* const msg);
    bool setSlotAzimuth(const basic::Number* const msg);
    bool setSlotVelocity(const basic::Number* const msg);

private:
    bool enabled;
    bool completed;
    const basic::String* wpnType;

    double angle;          // degs
    double azimuth;        // degs

    double armDelay;       // sec
    double velocity;       // ft/sec
    double stickDist;      // ft
    double maxMissDist;    // ft
    double interval;       // MS

    unsigned int quantity;
    unsigned int stickType;
    bool manualAssign;
};

inline bool TargetData::isEnabled() const                    { return enabled; }
inline bool TargetData::isCompleted() const                  { return completed; }
inline const basic::String* TargetData::getWpnType() const   { return wpnType; }
inline unsigned int TargetData::getQuantity() const          { return quantity; }
inline unsigned int TargetData::getStickType() const         { return stickType; }
inline double TargetData::getStickDistance() const           { return stickDist; }
inline double TargetData::getMaxMissDistance() const         { return maxMissDist; }
inline double TargetData::getInterval() const                { return interval; }
inline bool TargetData::getManualAssign() const              { return manualAssign; }
inline double TargetData::getArmDelay() const                { return armDelay; }
inline double TargetData::getAngle() const                   { return angle; }
inline double TargetData::getAzimuth() const                 { return azimuth; }
inline double TargetData::getVelocity() const                { return velocity; }

} // End Simulation namespace
} // End oe namespace

#endif
