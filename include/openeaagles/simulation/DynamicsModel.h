//------------------------------------------------------------------------------
// Class: DynamicsModel
//------------------------------------------------------------------------------
#ifndef __oe_simulation_DynamicsModel_H__
#define __oe_simulation_DynamicsModel_H__

#include "openeaagles/base/Component.h"

namespace oe {
namespace simulation {

//==============================================================================
// Class DynamicsModel
//
// Description:  Base dynamics model class
//
// Note --
//
//    1) updateTC() and updateData() are called by Component::upateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) To be properly synchronized within a frame, the root of our dynamics
//       model should be in our dynamics() function, which will called from
//       the Player class dynamics() function.
//
//    3) atReleaseInit() is called by Weapon::updateTC() to switch from
//       PRE_RELEASE mode to ACTIVE mode.  Default is to call reset().
//       Not used by air vehicles.
//
//    4) This class is one of the "top level" systems attached to a Player
//       class (see Player.h).
//
// Factory name: DynamicsModel
//
//==============================================================================
class DynamicsModel : public basic::Component
{
    DECLARE_SUBCLASS(DynamicsModel, basic::Component)

public:
    DynamicsModel();

    virtual void atReleaseInit();
    virtual void dynamics(const LCreal dt);

    virtual bool isHeadingHoldOn() const;
    virtual double getCommandedHeadingD() const;
    virtual bool setHeadingHoldOn(const bool b);

    // commanded heading inputs:  true heading in degrees (h),
    //                            degrees per second rate of heading change (hDps)
    //                            and maximum bank angle (maxBank)
    virtual bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0);

    virtual bool isVelocityHoldOn() const;
    virtual double getCommandedVelocityKts() const;
    virtual bool setVelocityHoldOn(const bool b);

    // commanded velocity inputs: commanded velocity in knots (v),
    //                            acceleration limiter (vNps)
    // acceleration limiter is determines how fast the model gets to the actual commanded velocity
    virtual bool setCommandedVelocityKts(const double v, const double vNps = 0);

    virtual bool isAltitudeHoldOn() const;
    virtual double getCommandedAltitude() const;
    virtual bool setAltitudeHoldOn(const bool b);

    // commanded altitude  inputs: commanded altitude in meters (a),
    //                             rate of change (in meters per second) associated with moving to new altitude (aMps)
    virtual bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0);

    virtual LCreal getFuelWt() const;                   // lbs
    virtual LCreal getFuelWtMax() const;                // lbs
    virtual LCreal getGrossWeight() const;              // wt: lbs

    virtual int getNumberOfEngines() const;
    virtual int getEngThrust(LCreal* const fn, const int max) const;          // Thrust: lbs

    // Control Stick Input Positions (not the flight surfaces!)
    virtual void setControlStickRollInput(const LCreal roll);
    virtual void setControlStickPitchInput(const LCreal pitch);
    void setControlStick(const LCreal roll, const LCreal pitch) {
        setControlStickRollInput(roll);
        setControlStickPitchInput(pitch);
    }

    // ---
    // Set throttle positions; returns the actual number of throttle positions
    //    'positions' -> Array of throttle positions (for each throttle)
    //              < 0.0  -> Cutoff
    //              0.0    -> Idle
    //              1.0    -> MIL
    //              2.0    -> A/B
    //    'num '-> number of throttle positions to get/set
    // ---
    virtual int setThrottles(const LCreal* const positions, const int num);

    // ---
    // Sets brake positions (left & right)
    //         No brake force  -> 0.0
    //         Max brake force -> 1.0
    // ---
    virtual void setBrakes(const LCreal left, const LCreal right);

    // Sets the fuel weight
    virtual bool setFuelWt(const LCreal lbs);
};

} // End simulation namespace
} // End oe namespace

#endif
