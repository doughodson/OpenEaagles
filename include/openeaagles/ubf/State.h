//
// Class: State
//
// Base class: Basic::Object -> State
//
// Description: Base state class
//

#ifndef __Eaagles_Ubf_State_H__
#define __Eaagles_Ubf_State_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {

namespace Simulation { class Player; }

namespace Ubf {

class State : public Basic::Object
{
   DECLARE_SUBCLASS(State, Basic::Object)

public:

   State();

   virtual void updateState(const Simulation::Player* const ownship) = 0;

private:

};

}
}

#endif

