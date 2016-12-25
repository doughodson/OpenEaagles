
#include "openeaagles/base/ubf/Arbiter.hpp"
#include "openeaagles/base/ubf/Action.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace base {
namespace ubf {

IMPLEMENT_SUBCLASS(Arbiter, "UbfArbiter")
EMPTY_COPYDATA(Arbiter)
EMPTY_SERIALIZER(Arbiter)

BEGIN_SLOTTABLE(Arbiter)
   "behaviors"                    //  1) behaviors
END_SLOTTABLE(Arbiter)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Arbiter)
   ON_SLOT(1, setSlotBehaviors, base::PairStream)
END_SLOT_MAP()

Arbiter::Arbiter()
{
   STANDARD_CONSTRUCTOR()
   behaviors = new base::List();
}

void Arbiter::deleteData()
{
   // unref behaviors
   if ( behaviors!=nullptr ) { behaviors->unref();   behaviors = nullptr; }
}

//------------------------------------------------------------------------------
// genAction() - generate an action
//------------------------------------------------------------------------------
Action* Arbiter::genAction(const State* const state, const double dt)
{
   // create list for action set
   base::List* actionSet = new base::List();

   // fill out list of recommended actions by behaviors
   base::List::Item* item = behaviors->getFirstItem();
   while (item != nullptr) {
      // get a behavior
      Behavior* behavior = static_cast<Behavior*>(item->getValue());
      // generate action, we have reference
      Action* action = behavior->genAction(state, dt);
      if (action != nullptr) {
         // add to action set
         actionSet->addTail(action);
         // unref our action reference
         action->unref();
      }
      // goto behavior
      item = item->getNext();
   }

   // given the set of recommended actions, the arbiter
   // decides what action to take
   Action* complexAction = genComplexAction(actionSet);

   // done with action set
   actionSet->unref();

   // return action to perform
   return complexAction;
}


//------------------------------------------------------------------------------
// Default: select the action with the highest vote
//------------------------------------------------------------------------------
Action* Arbiter::genComplexAction(base::List* const actionSet)
{
   Action* complexAction = nullptr;
   unsigned int maxVote = 0;

   // process entire action set
   base::List::Item* item = actionSet->getFirstItem();
   while (item != nullptr) {

      // Is this action's vote higher than the previous?
      Action* action = static_cast<Action*>(item->getValue());
      if (maxVote==0 || action->getVote() > maxVote) {

         // Yes ...
         if (complexAction != nullptr) complexAction->unref();
         complexAction = action;
         complexAction->ref();
         maxVote = action->getVote();
      }

      // next action
      item = item->getNext();
   }

   if (maxVote > 0 && isMessageEnabled(MSG_DEBUG))
      std::cout << "Arbiter: chose action with vote= " << maxVote << std::endl;

   // Use our vote value; if its been set
   if (getVote() > 0 && complexAction != nullptr) {
      complexAction->setVote(getVote());
   }

   // complexAction will have the vote value of whichever component action was selected
   return complexAction;
}

//------------------------------------------------------------------------------
// addBehavior() - add a new behavior
//------------------------------------------------------------------------------
void Arbiter::addBehavior(Behavior* const x)
{
   behaviors->addTail(x);
   x->container(this);
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool Arbiter::setSlotBehaviors(base::PairStream* const x)
{
   bool ok = true;

   // First, make sure they are all behaviors
   {
      base::List::Item* item = x->getFirstItem();
      while (item != nullptr && ok) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         item = item->getNext();
         Behavior* b = dynamic_cast<Behavior*>( pair->object() );
         if (b == nullptr) {
            // Item is NOT a behavior
            std::cerr << "setSlotBehaviors: slot: " << *pair->slot() << " is NOT of a Behavior type!" << std::endl;
            ok = false;
         }
      }
   }

   // next, add behaviors to our list
   if (ok) {
      base::List::Item* item = x->getFirstItem();
      while (item != nullptr) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         item = item->getNext();
         Behavior* b = static_cast<Behavior*>(pair->object());
         addBehavior(b);
      }
   }

   return ok;
}

}
}
}

