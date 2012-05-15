//
// Class: Arbiter
//
// Base class: Basic::Component -> Behavior -> Arbiter
//
// Description: Abstract base class for arbiters that produces an
//              action based on the recommendations generated
//              by each of the included behaviors
//
// Form name: Arbiter
//

#ifndef __Eaagles_Ubf_Arbiter_H__
#define __Eaagles_Ubf_Arbiter_H__

#include "Behavior.h"

namespace Eaagles {

namespace Basic { class List; }

namespace Ubf {

class Action;
class State;

class Arbiter : public Behavior
{
   DECLARE_SUBCLASS(Arbiter, Behavior)

public:

   Arbiter();

   // generate action based on current state
   virtual Action* genAction(const State* const state, const LCreal dt);

   // add new behavior to list
   void addBehavior(Behavior* const);
   Basic::List* getBehaviors() const      { return behaviors; }

   // slot functions
   bool setSlotBehaviors(Basic::PairStream* const);

   // evalutes a list of actions and creates an action
   virtual Action* genComplexAction(const Basic::List* const actionSet) = 0;

private:

   Basic::List* behaviors;

};

}
}

#endif
