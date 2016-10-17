
#ifndef __oe_simulation_SamVehicle_H__
#define __oe_simulation_SamVehicle_H__

#include "openeaagles/simulation/GroundVehicle.hpp"

namespace oe {
   namespace base {
      class Distance;
      class Identifier;
      class Time;
   }
namespace simulation {

//------------------------------------------------------------------------------
// Class SamVehicle
// Description: Sam vehicle (TEL, TELAR, etc)
//
// Factory name: SamVehicle
// Slots:
//    minLaunchRange    <base::Distance>   ! Min launch range (base::Distance)
//                                          ! (default: DEFAULT_MIN_LAUNCH_RANGE)
//    maxLaunchRange    <base::Distance>   ! Max launch range (base::Distance)
//                                          ! (default: DEFAULT_MAX_LAUNCH_RANGE)
//
//------------------------------------------------------------------------------
class SamVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(SamVehicle, GroundVehicle)

public:
   SamVehicle();

   // Access functions
   virtual unsigned int getNumberOfMissiles() const;  // Number of missiles

   virtual double getMaxLaunchRange() const;          // Max missile launch range (meters)
   virtual double getMinLaunchRange() const;          // Min missile launch range (meters)

   virtual bool isLauncherReady() const;              // Return true if launcher is ready to fire.

   // Set functions
   virtual bool setMaxLaunchRange(const double rng);
   virtual bool setMinLaunchRange(const double rng);

   // Slot functions
   virtual bool setSlotMinLaunchRange(const base::Distance* const msg);
   virtual bool setSlotMaxLaunchRange(const base::Distance* const msg);

   virtual void updateData(const double dt = 0.0) override;

private:
   double            maxMslRng;        // Max Launch Range        (meters)
   double            minMslRng;        // Min Launch Range        (meters)
   unsigned int      numMsl;           // Number of missile available
};

}
}

#endif
