//------------------------------------------------------------------------------
// Class: FuelTank
//------------------------------------------------------------------------------
#ifndef __oe_simulation_FuelTank_H__
#define __oe_simulation_FuelTank_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: FuelTank
//
// Description: Generic fuel tank
//
// Factory name: FuelTank
// Slots:
//    fuelWt      <Number>  ! Fuel wt (lb) (default: -1)
//    capacity    <Number>  ! Tank capacity (lb) (default: -1)
//
//------------------------------------------------------------------------------
class FuelTank : public ExternalStore
{
   DECLARE_SUBCLASS(FuelTank,ExternalStore)

public:
   FuelTank();

   // Tank capacity (lb)
   virtual double getCapacity() const;
   virtual bool setCapacity(const double cap);
   virtual bool isCapacityValid() const;

   // Fuel weight (lb)
   virtual double getFuelWt() const;
   virtual bool setFuelWt(const double wt);
   virtual bool isFuelWtValid() const;

   void reset() override;

protected:
   virtual bool setSlotFuelWt(const base::Number* const msg);
   virtual bool setSlotCapacity(const base::Number* const msg);

private:
   double capacity;      // Fuel capacity (lb)
   double fuelWt;        // Fuel contents (lb)
   double initFuelWt;    // Fuel contents at reset (lb)
};

} // end simulation namespace
} // end oe namespace

#endif
