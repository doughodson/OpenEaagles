//------------------------------------------------------------------------------
// Classes:	SpaceVehicle, BoosterSpaceVehicle, MannedSpaceVehicle, UnmannedSpaceVehicle
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_SpaceVehicle_H__
#define __Eaagles_Simulation_SpaceVehicle_H__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class SpaceVehicle
// Description: Generic Space Vehicles
// Form name: SpaceVehicle
//==============================================================================
class SpaceVehicle : public Player  
{
   DECLARE_SUBCLASS(SpaceVehicle,Player)

public:
   SpaceVehicle();

   // Vehicle data
   virtual LCreal getFuelWt() const;                   // wt: lbs
   virtual LCreal getFuelWtMax() const;                // wt: lbs

   // Engine data
   virtual int getNumberOfEngines() const;
   virtual int getEngThrust(LCreal* const fn, const int max) const; // Thrust: lbs

   // setControlStickYawInput(const LCreal yaw)
   //    yaw inputs: normalized
   //    yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setControlStickYawInput(const LCreal yaw);

   // setTranslateXInput(const LCreal transx)
   //    transx inputs: normalized
   //    transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateXInput(const LCreal transx);

   // setTranslateYInput(const LCreal transy)
   //    transy inputs: normalized
   //    transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateYInput(const LCreal transy);

   // setTranslateZInput(const LCreal transz)
   //    transz inputs: normalized
   //    transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateZInput(const LCreal transz);

   // Player class interface
   virtual LCreal getGrossWeight() const; 
   virtual unsigned int getMajorType() const;
};


//==============================================================================
// Class BoosterSpaceVehicle
// Form name: BoosterSpaceVehicle
//==============================================================================
class BoosterSpaceVehicle : public SpaceVehicle  
{
   DECLARE_SUBCLASS(BoosterSpaceVehicle,SpaceVehicle)

public:
   BoosterSpaceVehicle();
};


//==============================================================================
// Class MannedSpaceVehicle
// Form name: MannedSpaceVehicle
//==============================================================================
class MannedSpaceVehicle : public SpaceVehicle  
{
   DECLARE_SUBCLASS(MannedSpaceVehicle,SpaceVehicle)

public:
   MannedSpaceVehicle();
};


//==============================================================================
// Class UnmannedSpaceVehicle
// Form name: UnmannedSpaceVehicle
//==============================================================================
class UnmannedSpaceVehicle : public SpaceVehicle  
{
   DECLARE_SUBCLASS(UnmannedSpaceVehicle,SpaceVehicle)

public:
   UnmannedSpaceVehicle();

protected:
   // Vehicle Dynamics -- called by updateTC() during phase zero
   virtual void dynamics(const LCreal  dt = 0.0);
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
