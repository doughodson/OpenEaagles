
#ifndef __oe_models_Bomb_H__
#define __oe_models_Bomb_H__

#include "openeaagles/models/players/AbstractWeapon.hpp"

namespace oe {
namespace base { class Identifier; }
namespace models {

//------------------------------------------------------------------------------
// Class: Bomb
// Description: Base class for bombs
//
//    Contains a simple ballistics and guidance model, which derived models can
//    override at will.
//
// Factory name: Bomb
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
class Bomb : public AbstractWeapon
{
    DECLARE_SUBCLASS(Bomb, AbstractWeapon)

public:
    // Arming options
    enum { NONE, FREE_FALL, RETARDED, TIMER, PROXIMITY };

public:
    Bomb();

    virtual unsigned int getArmingOption() const;
    virtual bool isArmingOption(const unsigned int a) const;

    virtual double getFuzeAltitude() const;
    virtual double getFuzeTime() const;
    virtual double getDragIndex() const;

    virtual bool isNoseFuze() const;
    virtual bool isMidFuze() const;
    virtual bool isTailFuze() const;

    // Predict bomb's TOF and final position.
    // -- positions and ground plane are relative to a common ref point.
    virtual bool impactPrediction(
      const base::Vec3d* const initPos, // Initial position (meters)
      const base::Vec3d* const initVel, // Initial velocity (meters per sec)
      const double groundPlane,         // Ground plane (meters)
      const double dt,                  // Time step (sec)
      const double maxTime,             // Max TOF (sec)
      base::Vec3d* const finalPos,      // Final position (meters)
      double* const tof                 // TOF (sec)
    ) const;

    // Predict bomb's TOF and final position.
    // -- positions and ground plane are relative to a common ref point.
    static bool weaponImpactPrediction(
      const base::Vec3d* const initPos, // Initial position (meters)
      const base::Vec3d* const initVel, // Initial velocity (meters per sec)
      const double groundPlane,         // Ground plane (meters)
      const double dt,                  // Time step (sec)
      const double maxTime,             // Max TOF (sec)
      const double dragIndex,           // Drag index
      base::Vec3d* const finalPos,      // Final position (meters)
      double* const tof                 // TOF (sec)
    );

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;

protected:
    virtual bool setNoseFuze(const bool f);
    virtual bool setMidFuze(const bool f);
    virtual bool setTailFuze(const bool f);
    virtual bool setArmingOption(const unsigned int a);
    virtual bool setFuzeAltitude(const double v);
    virtual bool setFuzeTime(const double v);
    virtual bool setDragIndex(const double v);

    // slot functions
    bool setSlotDragIndex(base::Number* const p);
    bool setSlotArmingOption(base::Identifier* const p);
    bool setSlotNoseFuze(base::Number* const p);
    bool setSlotMidFuze(base::Number* const p);
    bool setSlotTailFuze(base::Number* const p);
    bool setSlotFuzeAltitude(base::Number* const p);
    bool setSlotFuzeTime(base::Number* const p);

    virtual void weaponGuidance(const double dt) override;
    virtual void weaponDynamics(const double dt) override;

private:
    void initData();

    double    dragIndex;         // Drag Index

    // common guidance stuff
    base::Vec3d missDistRef;     // Miss distance vector (ref sys)  (meters)
    base::Vec3d tgtRangeRef;     // Target range vector  (ref sys)  (meters)
    double     cmdStrAz;         // Commanded steering azimuth   (radians)
    double     cmdStrEl;         // Commanded steering elevation (radians)
    bool       guidanceValid;    // is guidance data valid.

    // Common arming stuff
    unsigned int arming;    // Arming option
    bool noseFuze;          // Nose fuze flag
    bool midFuze;           // Middle fuze flag
    bool tailFuze;          // Tail fuze flag
    double fuzeAlt;         // Fuze arming Altitude
    double fuzeTime;        // Fuze Arming Time
};

}
}

#endif
