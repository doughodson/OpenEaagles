//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Base class: basic::Object -> basic::Component -> System -> Gimbal -> StabilizingGimbal
//
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
//
// Public Member functions:
//
//------------------------------------------------------------------------------
#ifndef __oe_simulation_StabilizingGimbal_H__
#define __oe_simulation_StabilizingGimbal_H__

#include "openeaagles/simulation/Gimbal.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
// Factory name: StabilizingGimbal
// Slots:
//    stabilizingMode  <basic::String>    ! Sets the type of stabilization we desire  (elevation, roll, horizon)
//                                        ! (default: HORIZON)
//------------------------------------------------------------------------------
class StabilizingGimbal : public Gimbal
{
    DECLARE_SUBCLASS(StabilizingGimbal,Gimbal)

public:
    enum StabilizingMode { ELEVATION, ROLL, HORIZON };
    enum MountPosition { NOSE, TAIL, LEFT_WING, RIGHT_WING };

public:
   StabilizingGimbal();

   StabilizingMode getStabilizingMode() const                  { return stabilizingMode; }
   virtual bool setStabilizingMode(const StabilizingMode m);

   MountPosition getMountPosition() const                      { return mountPosition; }
   virtual bool setMountPosition(const MountPosition m);

   virtual bool setSlotStabilizingMode(basic::String* const msg);
   virtual bool setSlotMountPosition(basic::String* const msg);

protected:
   virtual void stabilizingController(const LCreal dt = 0.0);
   virtual void rollStabilizingController(const LCreal dt = 0.0);
   virtual void elevationStabilizingController(const LCreal dt = 0.0);

   void dynamics(const LCreal dt) override;

private:
   StabilizingMode  stabilizingMode; // Gimbal stabilization mode
   MountPosition    mountPosition;   // Gimbal Mount position
};

} // End simulation namespace
} // End oe namespace

#endif
