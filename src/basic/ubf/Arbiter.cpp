//------------------------------------------------------------------------------
// Class: Arbiter
//------------------------------------------------------------------------------

#include "openeaagles/basic/ubf/Arbiter.h"
#include "openeaagles/basic/ubf/Action.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

IMPLEMENT_SUBCLASS(Arbiter, "UbfArbiter")
EMPTY_COPYDATA(Arbiter)
EMPTY_SERIALIZER(Arbiter)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Arbiter)
   "behaviors"                    //  1) behaviors
END_SLOTTABLE(Arbiter)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Arbiter)
   ON_SLOT(1, setSlotBehaviors, Basic::PairStream)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Arbiter::Arbiter()
{
   STANDARD_CONSTRUCTOR()
   behaviors = new Basic::List();
}

void Arbiter::deleteData()
{
   // unref behaviors
   if ( behaviors!=nullptr ) { behaviors->unref();   behaviors = nullptr; }
}

//------------------------------------------------------------------------------
// genAction() - generate an action
//------------------------------------------------------------------------------
Action* Arbiter::genAction(const State* const state, const LCreal dt)
{
   // create list for action set
   Basic::List* actionSet = new Basic::List();

   // fill out list of recommended actions by behaviors
   Basic::List::Item* item = behaviors->getFirstItem();
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
Action* Arbiter::genComplexAction(Basic::List* const actionSet)
{
   Action* complexAction = nullptr;
   unsigned int maxVote = 0;

   // process entire action set
   Basic::List::Item* item = actionSet->getFirstItem();
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

bool Arbiter::setSlotBehaviors(Basic::PairStream* const x)
{
   bool ok = true;

   // First, make sure they are all behaviors
   {
      Basic::List::Item* item = x->getFirstItem();
      while (item != nullptr && ok) {
         Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
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
      Basic::List::Item* item = x->getFirstItem();
      while (item != nullptr) {
         Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
         item = item->getNext();
         Behavior* b = static_cast<Behavior*>(pair->object());
         addBehavior(b);
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Arbiter::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

