//------------------------------------------------------------------------------
// Class: FuelTank
//------------------------------------------------------------------------------
#ifndef __FuelTank_H_A857B4DA_D812_4cbb_B54A_6D15CF500B03__
#define __FuelTank_H_A857B4DA_D812_4cbb_B54A_6D15CF500B03__

#include "openeaagles/simulation/ExternalStore.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class:	FuelTank
//
// Description: Generic fuel tank
//
// Form Name: FuelTank
// Slots:
//    fuelWt      <Number>  ! Fuel wt (lb) (default: -1)
//    capacity    <Number>  ! Tank capacity (lb) (default: -1)
//
// GUID: {A857B4DA-D812-4cbb-B54A-6D15CF500B03}
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

   // Component Interface
   virtual void reset();

protected:
   virtual bool setSlotFuelWt(const Basic::Number* const msg);
   virtual bool setSlotCapacity(const Basic::Number* const msg);

private:
   LCreal capacity;      // Fuel capacity (lb)
   LCreal fuelWt;        // Fuel contents (lb)
   LCreal initFuelWt;    // Fuel contents at reset (lb)
};

} // end Simulation namespace
} // end Eaagles namespace

#endif // __FuelTank_H_A857B4DA_D812_4cbb_B54A_6D15CF500B03__
