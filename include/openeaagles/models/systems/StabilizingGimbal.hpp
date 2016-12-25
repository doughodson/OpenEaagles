
#ifndef __oe_models_StabilizingGimbal_H__
#define __oe_models_StabilizingGimbal_H__

#include "openeaagles/models/systems/Gimbal.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
// Factory name: StabilizingGimbal
// Slots:
//    stabilizingMode  <base::String>    ! Sets the type of stabilization we desire  (elevation, roll, horizon)
//                                       ! (default: HORIZON)
//------------------------------------------------------------------------------
class StabilizingGimbal : public Gimbal
{
    DECLARE_SUBCLASS(StabilizingGimbal, Gimbal)

public:
    enum StabilizingMode { ELEVATION, ROLL, HORIZON };
    enum MountPosition { NOSE, TAIL, LEFT_WING, RIGHT_WING };

public:
   StabilizingGimbal();

   StabilizingMode getStabilizingMode() const                  { return stabilizingMode; }
   virtual bool setStabilizingMode(const StabilizingMode m);

   MountPosition getMountPosition() const                      { return mountPosition; }
   virtual bool setMountPosition(const MountPosition m);

   virtual bool setSlotStabilizingMode(base::String* const msg);
   virtual bool setSlotMountPosition(base::String* const msg);

protected:
   virtual void stabilizingController(const double dt = 0.0);
   virtual void rollStabilizingController(const double dt = 0.0);
   virtual void elevationStabilizingController(const double dt = 0.0);

   virtual void dynamics(const double dt) override;

private:
   StabilizingMode  stabilizingMode; // Gimbal stabilization mode
   MountPosition    mountPosition;   // Gimbal Mount position
};

}
}

#endif
