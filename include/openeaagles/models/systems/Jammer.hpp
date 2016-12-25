
#ifndef __oe_models_Jammer_H__
#define __oe_models_Jammer_H__

#include "openeaagles/models/systems/RfSensor.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Jammer
// Description: Example Jammer
// Factory name: Jammer
//
// Default R/F sensor type ID is "JAMMER"
//------------------------------------------------------------------------------
class Jammer : public RfSensor
{
    DECLARE_SUBCLASS(Jammer, RfSensor)

public:
    Jammer();

protected:
   virtual void transmit(const double dt) override;
};

}
}

#endif
