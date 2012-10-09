//------------------------------------------------------------------------------
// Class: Bullseye
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Bullseye_H__
#define __Eaagles_Simulation_Bullseye_H__

#include "openeaagles/simulation/Steerpoint.h"

namespace Eaagles {
namespace Simulation {
//------------------------------------------------------------------------------
// Class: Bullseye
//
// Description: Generic Bullseye, used as a reference point
//
// Form name: Bullseye
//
//------------------------------------------------------------------------------
class Bullseye : public Steerpoint
{
   DECLARE_SUBCLASS(Bullseye,Steerpoint)

public:
    Bullseye();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
