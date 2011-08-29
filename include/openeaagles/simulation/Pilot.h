//------------------------------------------------------------------------------
// Class: Pilot
//------------------------------------------------------------------------------
#ifndef __Pilot_H_3735E456_019C_41e5_B40F_2D9A1C75D24D__
#define __Pilot_H_3735E456_019C_41e5_B40F_2D9A1C75D24D__

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
// Form name: Pilot
//------------------------------------------------------------------------------
class Pilot : public System
{
   DECLARE_SUBCLASS(Pilot,System)

public:
    Pilot();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Pilot_H_3735E456_019C_41e5_B40F_2D9A1C75D24D__
