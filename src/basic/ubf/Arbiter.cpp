//------------------------------------------------------------------------------
// Class: Arbiter
//------------------------------------------------------------------------------

#include "openeaagles/basic/ubf/Arbiter.h"
#include "openeaagles/basic/ubf/Action.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Basic {

// this version allows null action to be returned; this code handles that
// also sets container for contained behaviors
IMPLEMENT_SUBCLASS(Arbiter, "Arbiter")
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
      Behavior* behavior = (Behavior*) item->getValue();
      // generate action, we have reference
      Action* action = behavior->genAction(state, dt);
      if (action != 0) {
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


// implements generic priority arbiting scheme - selects component action with highest vote
//
Action* Arbiter::genComplexAction(const Basic::List* const actionSet)
{
   Action* complexAction = 0;
   unsigned int maxVote = 0;

   // process entire action set
   const Basic::List::Item* item = actionSet->getFirstItem();
   while (item != 0) {

      const Action* action = dynamic_cast<const Action*>(item->getValue());
      
      if (maxVote==0 || action->getVote() > maxVote) {
         // unref previous high vote getter
         if (complexAction != 0)
            complexAction->unref();

         complexAction = dynamic_cast<Action*>(action->clone());

         maxVote = action->getVote();
      }
      // next action
      item = item->getNext();
   }

   if (maxVote > 0 && isMessageEnabled(MSG_DEBUG))
      std::cout << "Arbiter: chose action with vote= " << maxVote << std::endl;

   // if a vote value has been set for arbiter in input, should it override? as in:
   //if (getVote() > 0)
   //   complexAction->setVote(getVote());

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
            std::cerr << "setSlotBehaviors: slot: " << *pair->slot() << " is NOT of a Behavior type!" << std::endl;
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

} // End Basic namespace
} // End Eaagles namespace

