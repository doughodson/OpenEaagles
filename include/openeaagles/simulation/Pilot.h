//------------------------------------------------------------------------------
// Class: Pilot
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Pilot_H__
#define __Eaagles_Simulation_Pilot_H__

#include "openeaagles/simulation/System.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class: Pilot
// Description: Base pilot model ...
//
//    Base class for all pilot, autopilot and pilot decision logic models for
//    any Eaagles Player type, and not just those related to air vehicles.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
// Factory name: Pilot
//------------------------------------------------------------------------------
class Pilot : public System
{
   DECLARE_SUBCLASS(Pilot,System)

public:
    Pilot();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
