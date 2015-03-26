//------------------------------------------------------------------------------
// Class: RacModel
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dynamics_RacModel_H__
#define __Eaagles_Dynamics_RacModel_H__

#include "openeaagles/simulation/dynamics/AerodynamicsModel.h"

namespace Eaagles {
   namespace Basic { class Angle; class Distance; class Number; }

namespace Dynamics {

//------------------------------------------------------------------------------
// Class: RacModel
//
// Description: Robot Aircraft (RAC) Air Vehicle Dynamics Model
//                -- Very simple dynamics model
//
// Factory name: RacModel
// Slots:
//    minSpeed       <Number>    ! Minimum Velocity        (kts)
//    speedMaxG      <Number>    ! Velocity we reach max G (kts)
//    maxg           <Number>    ! Max G's (at "speedMaxG" or above)
//    maxAccel       <Number>    ! Maximum Acceleration    (m/s/s)
//    cmdAltitude    <Distance>  ! Command Altitude
//    cmdHeading     <Angle>     ! Command Heading
//    cmdSpeed       <Number>    ! Command speed           (kts)
//------------------------------------------------------------------------------
class RacModel : public Simulation::AerodynamicsModel
{
    DECLARE_SUBCLASS(RacModel,Simulation::AerodynamicsModel)

public: // public interface
    RacModel();

    // AerodynamicsModel interface
    virtual LCreal getGload() const;                    // G's (+up, one at level)
    virtual LCreal getMach() const;                     // mach number
    virtual LCreal getAngleOfAttack() const;            // radians
    virtual LCreal getSideSlip() const;                 // radians
    virtual LCreal getFlightPath() const;               // radians
    virtual LCreal getCalibratedAirspeed() const;       // Kts

    // DynamicsModel interface
    virtual bool isHeadingHoldOn() const;
    virtual double getCommandedHeadingD() const;
    virtual bool setHeadingHoldOn(const bool b);
    virtual bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0);
    virtual bool isVelocityHoldOn() const;
    virtual double getCommandedVelocityKts() const;
    virtual bool setVelocityHoldOn(const bool b);
    virtual bool setCommandedVelocityKts(const double v, const double vNps = 0);
    virtual bool isAltitudeHoldOn() const;
    virtual double getCommandedAltitude() const;
    virtual bool setAltitudeHoldOn(const bool b);
    virtual bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0);
    virtual void dynamics(const LCreal dt);             // One pass model update; called from Player::dynamics()

    void reset() override;

    // Slot methods
    virtual bool setSlotMinSpeed(const Basic::Number* const msg);
    virtual bool setSlotSpeedMaxG(const Basic::Number* const msg);
    virtual bool setSlotMaxG(const Basic::Number* const msg);
    virtual bool setSlotMaxAccel(const Basic::Number* const msg);
    virtual bool setSlotCmdAltitude(const Basic::Distance* const msg);
    virtual bool setSlotCmdHeading(const Basic::Angle* const msg);
    virtual bool setSlotCmdVelocity(const Basic::Number* const msg);

private:
    void updateRAC(const LCreal dt);

    LCreal      vpMin;        // Minimum Velocity              (m/s)
    LCreal      vpMaxG;       // Velocity for Max G's          (g's)
    LCreal      gMax;         // Max G's                       (g's)
    LCreal      maxAccel;     // Max longitudinal acceleration (m/s/s)
    double      cmdAltitude;  // Commanded Altitude            (meters)
    double      cmdHeading;   // Commanded Heading             (degs)
    double      cmdVelocity;  // Commanded speed               (kts)
};

} // End Dynamics namespace
} // End Eaagles namespace

#endif
