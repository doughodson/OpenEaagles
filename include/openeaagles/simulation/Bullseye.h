//------------------------------------------------------------------------------
// Class: Bullseye
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Bullseye_H__
#define __oe_Simulation_Bullseye_H__

#include "openeaagles/simulation/Steerpoint.h"

namespace oe {
namespace Simulation {
//------------------------------------------------------------------------------
// Class: Bullseye
//
// Description: Generic Bullseye, used as a reference point
//
// Factory name: Bullseye
//
//------------------------------------------------------------------------------
class Bullseye : public Steerpoint
{
   DECLARE_SUBCLASS(Bullseye,Steerpoint)

public:
    Bullseye();
};

} // End Simulation namespace
} // End oe namespace

#endif
