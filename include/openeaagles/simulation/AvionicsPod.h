//------------------------------------------------------------------------------
// Classes: AvionicsPod
//------------------------------------------------------------------------------
#ifndef __oe_simulation_AvionicsPod_H__
#define __oe_simulation_AvionicsPod_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace oe {
namespace simulation {

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

} // end simulation namespace
} // end oe namespace

#endif
