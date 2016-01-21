//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Base class: Basic::Object -> Basic::Component -> System -> Gimbal -> StabilizingGimbal
//
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
//
// Public Member functions:
//
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_StabilizingGimbal_H__
#define __oe_Simulation_StabilizingGimbal_H__

#include "openeaagles/simulation/Gimbal.h"

namespace oe {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
// Factory name: StabilizingGimbal
// Slots:
//    stabilizingMode  <Basic::String>    ! Sets the type of stabilization we desire  (elevation, roll, horizon)
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

   virtual bool setSlotStabilizingMode(Basic::String* const msg);
   virtual bool setSlotMountPosition(Basic::String* const msg);

protected:
   virtual void stabilizingController(const LCreal dt = 0.0);
   virtual void rollStabilizingController(const LCreal dt = 0.0);
   virtual void elevationStabilizingController(const LCreal dt = 0.0);

   void dynamics(const LCreal dt) override;

private:
   StabilizingMode  stabilizingMode; // Gimbal stabilization mode
   MountPosition    mountPosition;   // Gimbal Mount position
};

} // End Simulation namespace
} // End oe namespace

#endif
