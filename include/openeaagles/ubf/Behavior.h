//
// Class: Behavior
//
// Base class: Basic::Component -> Behavior
//
// Description: Abstract base class that defines action generation
//
// Form name: Behavior
//

#ifndef __Eaagles_Ubf_Behavior_H__
#define __Eaagles_Ubf_Behavior_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {

namespace Ubf {

class Action;
class State;

class Behavior : public Basic::Component
{
   DECLARE_SUBCLASS(Behavior, Basic::Component)

public:

   Behavior();

   // generate an Action based on the current state
   virtual Action* genAction(const State* const state, const LCreal dt) = 0;

};

}
}

#endif

