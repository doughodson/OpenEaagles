//------------------------------------------------------------------------------
// Classes: AvionicsPod
//------------------------------------------------------------------------------
#ifndef __OpenEaagles_Simulation_AvionicsPod_H__
#define __OpenEaagles_Simulation_AvionicsPod_H__

#include "openeaagles/simulation/ExternalStore.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: AvionicsPod
// Description: Generic avionics pod
//
// Form name: AvionicsPod
//------------------------------------------------------------------------------
class AvionicsPod : public ExternalStore
{
   DECLARE_SUBCLASS(AvionicsPod,ExternalStore)

public:
   AvionicsPod();
};

} // end Simulation namespace
} // end Eaagles namespace

#endif // __OpenEaagles_Simulation_AvionicsPod_H__
