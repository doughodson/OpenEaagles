
#ifndef __oe_base_ubf_Arbiter_H__
#define __oe_base_ubf_Arbiter_H__

#include "AbstractBehavior.hpp"

namespace oe {

namespace base {
   class List;

namespace ubf {
   class AbstractState;
   class AbstractAction;

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
class Arbiter : public AbstractBehavior
{
   DECLARE_SUBCLASS(Arbiter, AbstractBehavior)

public:
   Arbiter();

   virtual AbstractAction* genAction(const AbstractState* const state, const double dt) override;

protected:
   base::List* getBehaviors();

   // evaluates a list of actions and return an optional "complex action"
   // (default: returns the action with the highest vote value)
   virtual AbstractAction* genComplexAction(List* const actionSet);

   // add new behavior to list
   void addBehavior(AbstractBehavior* const);

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

