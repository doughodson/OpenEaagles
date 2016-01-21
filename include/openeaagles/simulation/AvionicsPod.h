//------------------------------------------------------------------------------
// Classes: AvionicsPod
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_AvionicsPod_H__
#define __oe_Simulation_AvionicsPod_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace oe {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: AvionicsPod
// Description: Generic avionics pod
//
// Factory name: AvionicsPod
//------------------------------------------------------------------------------
class AvionicsPod : public ExternalStore
{
   DECLARE_SUBCLASS(AvionicsPod,ExternalStore)

public:
   AvionicsPod();
};

} // end Simulation namespace
} // end oe namespace

#endif
