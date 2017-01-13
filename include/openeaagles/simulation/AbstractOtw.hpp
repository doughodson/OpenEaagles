
#ifndef __oe_simulation_AbstractOtw_H__
#define __oe_simulation_AbstractOtw_H__

#include "openeaagles/base/Component.hpp"

namespace oe {

namespace base { class PairStream; }

namespace simulation {
class AbstractPlayer;

class AbstractOtw : public base::Component
{
   DECLARE_SUBCLASS(AbstractOtw, base::Component)

public:
   AbstractOtw();

   // required by Station

   // Sets our ownship pointer; public version, which is usually called by the Station class.  Derived classes
   // can override this function and control the switching of the ownship using setOwnship0()
   virtual void setOwnship(AbstractPlayer* const newOwnship) =0;
   virtual void setPlayerList(base::PairStream* const newPlayerList) =0; // Sets the player list that we're to use to generate player/models

};

}
}

#endif
