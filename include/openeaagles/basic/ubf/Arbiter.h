//------------------------------------------------------------------------------
// Class: Arbiter
//------------------------------------------------------------------------------
#ifndef __oe_basic_ubf_Arbiter_H__
#define __oe_basic_ubf_Arbiter_H__

#include "Behavior.h"

namespace oe {

namespace basic {
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

   Action* genAction(const State* const state, const LCreal dt) override;

protected:
   basic::List* getBehaviors();

   // evaluates a list of actions and return an optional "complex action"
   // (default: returns the action with the highest vote value)
   virtual Action* genComplexAction(List* const actionSet);

   // add new behavior to list
   void addBehavior(Behavior* const);

   // slot functions
   bool setSlotBehaviors(basic::PairStream* const);

private:
   basic::List* behaviors;
};

inline basic::List* Arbiter::getBehaviors()                 { return behaviors; }

} // End ubf namespace
} // End basic namespace
} // End oe namespace

#endif

