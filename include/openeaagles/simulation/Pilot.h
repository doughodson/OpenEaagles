//------------------------------------------------------------------------------
// Class: Pilot
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Pilot_H__
#define __oe_simulation_Pilot_H__

#include "openeaagles/simulation/System.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Pilot
// Description: Base pilot model ...
//
//    Base class for all pilot, autopilot and pilot decision logic models for
//    any oe Player type, and not just those related to air vehicles.
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

}
}

#endif
