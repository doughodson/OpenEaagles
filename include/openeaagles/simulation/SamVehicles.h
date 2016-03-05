//------------------------------------------------------------------------------
// Classes: SamVehicle
//------------------------------------------------------------------------------
#ifndef __oe_simulation_SamVehicle_H__
#define __oe_simulation_SamVehicle_H__

#include "openeaagles/simulation/GroundVehicle.h"

namespace oe {
   namespace base {
      class Distance;
      class Identifier;
      class Time;
   }
namespace simulation {

//==============================================================================
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
//==============================================================================
class SamVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(SamVehicle,GroundVehicle)

public:
   SamVehicle();

   // Access functions
   virtual unsigned int getNumberOfMissiles() const;  // Number of missiles

   virtual LCreal getMaxLaunchRange() const;          // Max missile launch range (meters)
   virtual LCreal getMinLaunchRange() const;          // Min missile launch range (meters)

   virtual bool isLauncherReady() const;              // Return true if launcher is ready to fire.

   // Set functions
   virtual bool setMaxLaunchRange(const LCreal rng);
   virtual bool setMinLaunchRange(const LCreal rng);

   // Slot functions
   virtual bool setSlotMinLaunchRange(const base::Distance* const msg);
   virtual bool setSlotMaxLaunchRange(const base::Distance* const msg);

   void updateData(const LCreal dt = 0.0) override;

private:
   LCreal            maxMslRng;        // Max Launch Range        (meters)
   LCreal            minMslRng;        // Min Launch Range        (meters)
   unsigned int      numMsl;           // Number of missile available
};

} // End simulation namespace
} // End oe namespace

#endif
