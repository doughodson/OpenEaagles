//------------------------------------------------------------------------------
// Classes: AvionicsPod
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_AvionicsPod_H__
#define __Eaagles_Simulation_AvionicsPod_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace Eaagles {
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
} // end Eaagles namespace

#endif
