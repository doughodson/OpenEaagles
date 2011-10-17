//------------------------------------------------------------------------------
// Class: Bomb
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Bomb_H__
#define __Eaagles_Simulation_Bomb_H__

#include "openeaagles/simulation/Weapon.h"

namespace Eaagles {
   namespace Basic { class Identifier; }

namespace Simulation {

//------------------------------------------------------------------------------
// Class: Bomb
// Description: Base class for bombs
//
//    Contains a simple ballistics and guidance model, which derived models can
//    override at will.
//
// Form name: Bomb
// Slots:
//    arming         <Identifier>  ! Bomb arming option (default: NONE)
//    noseFuze       <Number>      ! Nose fuze flag (default: false)
//    midFuze        <Number>      ! Middle fuze flag (default: false)
//    tailFuze       <Number>      ! Tail fuze flag (default: 0.0f)
//    fuzeAltitude   <Number>      ! Fuze arming Altitude (default: 0.0f)
//    fuzeTime       <Number>      ! Fuze Arming Time (default: 0.0f)
//    dragIndex      <Number>      ! drag index used by default dynamics (default: 0.0006f)
//
//------------------------------------------------------------------------------
class Bomb : public Weapon  
{
    DECLARE_SUBCLASS(Bomb,Weapon)

public:
    // Arming options
    enum { NONE, FREE_FALL, RETARDED, TIMER, PROXIMITY };

public:
    Bomb();

    virtual unsigned int getArmingOption() const;
    virtual bool isArmingOption(const unsigned int a) const;

    virtual LCreal getFuzeAltitude() const;
    virtual LCreal getFuzeTime() const;
    virtual LCreal getDragIndex() const;

    virtual bool isNoseFuze() const;
    virtual bool isMidFuze() const;
    virtual bool isTailFuze() const;

    // Predict bomb's TOF and final position.
    // -- positions and ground plane are relative to a common ref point.
    virtual bool impactPrediction(
      const osg::Vec3* const initPos,  // Initial position (meters)
      const osg::Vec3* const initVel,  // Initial velocity (meters per sec)
      const LCreal groundPlane,        // Ground plane (meters)
      const LCreal dt,                 // Time step (sec)
      const LCreal maxTime,            // Max TOF (sec)
      osg::Vec3* const finalPos,       // Final position (meters)
      LCreal* const tof                // TOF (sec)
    ) const;

    // Predict bomb's TOF and final position.
    // -- positions and ground plane are relative to a common ref point.
    static bool weaponImpactPrediction(
      const osg::Vec3* const initPos,  // Initial position (meters)
      const osg::Vec3* const initVel,  // Initial velocity (meters per sec)
      const LCreal groundPlane,        // Ground plane (meters)
      const LCreal dt,                 // Time step (sec)
      const LCreal maxTime,            // Max TOF (sec)
      const LCreal dragIndex,          // Drag index
      osg::Vec3* const finalPos,       // Final position (meters)
      LCreal* const tof                // TOF (sec)
    );

    // Weapon class functions
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;

protected:
    virtual bool setNoseFuze(const bool f);
    virtual bool setMidFuze(const bool f);
    virtual bool setTailFuze(const bool f);
    virtual bool setArmingOption(const unsigned int a);
    virtual bool setFuzeAltitude(const LCreal v);
    virtual bool setFuzeTime(const LCreal v);
    virtual bool setDragIndex(const LCreal v);

   // slot functions
   bool setSlotDragIndex(Basic::Number* const p);
    bool setSlotArmingOption(Basic::Identifier* const p);
    bool setSlotNoseFuze(Basic::Number* const p);
    bool setSlotMidFuze(Basic::Number* const p);
    bool setSlotTailFuze(Basic::Number* const p);
    bool setSlotFuzeAltitude(Basic::Number* const p);
    bool setSlotFuzeTime(Basic::Number* const p);

    // Weapon class functions (protected)
    virtual void weaponGuidance(const LCreal dt);
    virtual void weaponDynamics(const LCreal dt);

private:
   void initData();

    LCreal    dragIndex;             // Drag Index

   // common guidance stuff
    osg::Vec3 missDistRef;           // Miss distance vector (ref sys)  (meters)
    osg::Vec3 tgtRangeRef;           // Target range vector  (ref sys)  (meters)
    LCreal    cmdStrAz;              // Commanded steering azimuth   (radians)
    LCreal    cmdStrEl;              // Commanded steering elevation (radians)
    bool      guidanceValid;         // is guidance data valid.

   // Common arming stuff
   unsigned int arming;    // Arming option
   bool noseFuze;          // Nose fuze flag
   bool midFuze;           // Middle fuze flag
   bool tailFuze;          // Tail fuze flag
   LCreal fuzeAlt;         // Fuze arming Altitude
   LCreal fuzeTime;        // Fuze Arming Time
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
