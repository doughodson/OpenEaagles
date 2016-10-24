
#ifndef __oe_base_ubf_Arbiter_H__
#define __oe_base_ubf_Arbiter_H__

#include "Behavior.hpp"

namespace oe {

namespace base {
   class List;

namespace ubf {
   class State;
   class Action;

//------------------------------------------------------------------------------
// Class: Arbiter
//
// Description:
//    A meta-behavior that generates a "complex action" based on the actions
//    generated our list of behaviors.
//
// Note:
//    The default is to select the Action with the highest vote value.
//
// Factory name: UbfArbiter
// Slots:
//    behaviors   <PairStream>      ! List of behaviors
//------------------------------------------------------------------------------
class Arbiter : public Behavior
{
   DECLARE_SUBCLASS(Arbiter, Behavior)

public:
   Arbiter();

   virtual Action* genAction(const State* const state, const double dt) override;

protected:
   base::List* getBehaviors();

   // evaluates a list of actions and return an optional "complex action"
   // (default: returns the action with the highest vote value)
   virtual Action* genComplexAction(List* const actionSet);

   // add new behavior to list
   void addBehavior(Behavior* const);

   // slot functions
   bool setSlotBehaviors(base::PairStream* const);

private:
   base::List* behaviors;
};

inline base::List* Arbiter::getBehaviors()                 { return behaviors; }

}
}
}

#endif

