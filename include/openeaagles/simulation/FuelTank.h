//------------------------------------------------------------------------------
// Class: FuelTank
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_FuelTank_H__
#define __oe_Simulation_FuelTank_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace oe {
namespace Simulation {

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
   virtual LCreal getCapacity() const;
   virtual bool setCapacity(const LCreal cap);
   virtual bool isCapacityValid() const;

   // Fuel weight (lb)
   virtual LCreal getFuelWt() const;
   virtual bool setFuelWt(const LCreal wt);
   virtual bool isFuelWtValid() const;

   void reset() override;

protected:
   virtual bool setSlotFuelWt(const basic::Number* const msg);
   virtual bool setSlotCapacity(const basic::Number* const msg);

private:
   LCreal capacity;      // Fuel capacity (lb)
   LCreal fuelWt;        // Fuel contents (lb)
   LCreal initFuelWt;    // Fuel contents at reset (lb)
};

} // end Simulation namespace
} // end oe namespace

#endif
