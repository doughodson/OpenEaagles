
#ifndef __oe_simulation_AbstractOtw_H__
#define __oe_simulation_AbstractOtw_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base { class PairStream; }
namespace simulation {
class AbstractPlayer;

//------------------------------------------------------------------------------
// Class: AbstractOtw
// Description: Abstract Out-The-Window (OTW) visual system interface class
//
// Notes:
//    1) Derived versions of this class are used to interface with various
//       visual systems and HUDs (see otw library).
//
//    2) One or more Otw objects can be attached to and managed by a Station object.
//
//    3) To allow different visual database to be used with the same scenario,
//       the visual database reference point, which is used by this class,
//       is a different reference point then the Simulation class.
//       Player's (see Player.hpp) position vectors [ x y z ] are north(x),
//       east(y) and down(z) from the reference point.
//
// Factory name: AbstractOtw
//------------------------------------------------------------------------------
class AbstractOtw : public base::Component
{
   DECLARE_SUBCLASS(AbstractOtw, base::Component)

public:
   AbstractOtw();

   // Sets our ownship pointer; public version, which is usually called by the Station class.  Derived classes
   // can override this function and control the switching of the ownship using setOwnship0()
   virtual void setOwnship(AbstractPlayer* const newOwnship) =0;
   virtual void setPlayerList(base::PairStream* const newPlayerList) =0; // Sets the player list that we're to use to generate player/models
};

}
}

#endif
