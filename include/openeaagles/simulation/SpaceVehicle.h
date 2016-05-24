//------------------------------------------------------------------------------
// Classes: SpaceVehicle, BoosterSpaceVehicle, MannedSpaceVehicle, UnmannedSpaceVehicle
//------------------------------------------------------------------------------
#ifndef __oe_simulation_SpaceVehicle_H__
#define __oe_simulation_SpaceVehicle_H__

#include "openeaagles/simulation/Player.h"

namespace oe {
namespace simulation {

//==============================================================================
// Class SpaceVehicle
// Description: Generic Space Vehicles
// Factory name: SpaceVehicle
//==============================================================================
class SpaceVehicle : public Player
{
   DECLARE_SUBCLASS(SpaceVehicle,Player)

public:
   SpaceVehicle();

   // Vehicle data
   virtual double getFuelWt() const;                   // wt: lbs
   virtual double getFuelWtMax() const;                // wt: lbs

   // Engine data
   virtual int getNumberOfEngines() const;
   virtual int getEngThrust(double* const fn, const int max) const; // Thrust: lbs

   // setControlStickYawInput(const double yaw)
   //    yaw inputs: normalized
   //    yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setControlStickYawInput(const double yaw);

   // setTranslateXInput(const double transx)
   //    transx inputs: normalized
   //    transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateXInput(const double transx);

   // setTranslateYInput(const double transy)
   //    transy inputs: normalized
   //    transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateYInput(const double transy);

   // setTranslateZInput(const double transz)
   //    transz inputs: normalized
   //    transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateZInput(const double transz);

   double getGrossWeight() const override;
   unsigned int getMajorType() const override;
};


//==============================================================================
// Class BoosterSpaceVehicle
// Factory name: BoosterSpaceVehicle
//==============================================================================
class BoosterSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(BoosterSpaceVehicle,SpaceVehicle)

public:
   BoosterSpaceVehicle();
};


//==============================================================================
// Class MannedSpaceVehicle
// Factory name: MannedSpaceVehicle
//==============================================================================
class MannedSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(MannedSpaceVehicle,SpaceVehicle)

public:
   MannedSpaceVehicle();
};


//==============================================================================
// Class UnmannedSpaceVehicle
// Factory name: UnmannedSpaceVehicle
//==============================================================================
class UnmannedSpaceVehicle : public SpaceVehicle
{
   DECLARE_SUBCLASS(UnmannedSpaceVehicle,SpaceVehicle)

public:
   UnmannedSpaceVehicle();

protected:
   void dynamics(const double  dt = 0.0) override;
};

}
}

#endif
