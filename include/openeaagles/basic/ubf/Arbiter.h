//------------------------------------------------------------------------------
// Class: Arbiter
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Ubf_Arbiter_H__
#define __Eaagles_Basic_Ubf_Arbiter_H__

#include "Behavior.h"

namespace Eaagles {

namespace Basic {
   class List;

namespace Ubf {
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
   Basic::List* getBehaviors();

   // evaluates a list of actions and return an optional "complex action"
   // (default: returns the action with the highest vote value)
   virtual Action* genComplexAction(List* const actionSet);

   // add new behavior to list
   void addBehavior(Behavior* const);

   // slot functions
   bool setSlotBehaviors(Basic::PairStream* const);

private:
   Basic::List* behaviors;
};

inline Basic::List* Arbiter::getBehaviors()                 { return behaviors; }

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

#endif

