//
// Arbiter
//

#include "openeaagles/ubf/Arbiter.h"

#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

#include "openeaagles/ubf/State.h"
#include "openeaagles/ubf/Action.h"

namespace Eaagles {
namespace Ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(Arbiter, "UbfArbiter")
EMPTY_COPYDATA(Arbiter)
EMPTY_SERIALIZER(Arbiter)

// slot table for this class type
BEGIN_SLOTTABLE(Arbiter)
   "behaviors"                    //  1) behaviors
END_SLOTTABLE(Arbiter)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Arbiter)
   ON_SLOT(1, setSlotBehaviors, Basic::PairStream)
END_SLOT_MAP()

Arbiter::Arbiter()
{
   STANDARD_CONSTRUCTOR()

   behaviors = new Basic::List();
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Arbiter::deleteData()
{
   // unref behaviors
   if ( behaviors!=0 ) { behaviors->unref();   behaviors = 0; }
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
   while (item != 0) {
      // get a behavior
      Behavior* behavior = (Behavior*)item->getValue();
      // generate action, we have reference
      Action* action = behavior->genAction(state, dt);
      // add to action set
      actionSet->addTail(action);
      // unref our action reference
      action->unref();
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
// addBehavior() - add a new behavior
//------------------------------------------------------------------------------
void Arbiter::addBehavior(Behavior* const x)
{
   behaviors->addTail(x);
}

//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

bool Arbiter::setSlotBehaviors(Basic::PairStream* const x)
{
   bool ok = true;

   // First, make sure they are all behaviors
   {
      Basic::List::Item* item = x->getFirstItem();
      while (item != 0 && ok) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Behavior* b = dynamic_cast<Behavior*>( pair->object() );
         if (b == 0) {
            // Item is NOT a behavior
            std::cerr << "setSlotBehaviors: slot: " << *pair->slot() << " is NOT of a Ubf::Behavior type!" << std::endl;
            ok = false;
         }
      }
   }

   // next, add behaviors to our list
   if (ok) {
      Basic::List::Item* item = x->getFirstItem();
      while (item != 0) {
         Basic::Pair* pair = (Basic::Pair*) item->getValue();
         item = item->getNext();
         Behavior* b = (Behavior*)( pair->object() );
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

}
}

