//------------------------------------------------------------------------------
// Class: Bullseye
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Bullseye_H__
#define __oe_simulation_Bullseye_H__

#include "openeaagles/simulation/Steerpoint.h"

namespace oe {
namespace simulation {
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

} // End simulation namespace
}

#endif
