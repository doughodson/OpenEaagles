//------------------------------------------------------------------------------
// Classes: SamVehicle
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_SamVehicle_H__
#define __Eaagles_Simulation_SamVehicle_H__

#include "openeaagles/simulation/GroundVehicle.h"

namespace Eaagles {
   namespace Basic {
      class Distance;
      class Identifier;
      class Time;
   }
namespace Simulation {

//==============================================================================
// Class SamVehicle
// Description: Sam vehicle (TEL, TELAR, etc)
//
// Factory name: SamVehicle
// Slots:
//    minLaunchRange    <Basic::Distance>   ! Min launch range (Basic::Distance)
//                                          ! (default: DEFAULT_MIN_LAUNCH_RANGE)
//    maxLaunchRange    <Basic::Distance>   ! Max launch range (Basic::Distance)
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
   virtual bool setSlotMinLaunchRange(const Basic::Distance* const msg);
   virtual bool setSlotMaxLaunchRange(const Basic::Distance* const msg);

   // Component Interface
   virtual void updateData(const LCreal dt = 0.0f);

private:
   LCreal            maxMslRng;        // Max Launch Range        (meters)
   LCreal            minMslRng;        // Min Launch Range        (meters)
   unsigned int      numMsl;           // Number of missile available
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
