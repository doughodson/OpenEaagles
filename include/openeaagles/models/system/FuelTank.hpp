
#ifndef __oe_models_FuelTank_H__
#define __oe_models_FuelTank_H__

#include "openeaagles/models/system/ExternalStore.hpp"

namespace oe {
namespace models {

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
   DECLARE_SUBCLASS(FuelTank, ExternalStore)

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

   virtual void reset() override;

protected:
   virtual bool setSlotFuelWt(const base::Number* const msg);
   virtual bool setSlotCapacity(const base::Number* const msg);

private:
   double capacity {-1.0};      // Fuel capacity (lb)
   double fuelWt {-1.0};        // Fuel contents (lb)
   double initFuelWt {-1.0};    // Fuel contents at reset (lb)
};

}
}

#endif
