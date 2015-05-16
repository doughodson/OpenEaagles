//------------------------------------------------------------------------------
// Agent
//------------------------------------------------------------------------------

#include "openeaagles/basic/ubf/Agent.h"
#include "openeaagles/basic/ubf/Action.h"
#include "openeaagles/basic/ubf/Behavior.h"
#include "openeaagles/basic/ubf/State.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/String.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

//
// Class: Agent
//
// Description: An Agent that manages a component (the "actor") with a behavior
//              (either a player, or a player's component)
//

IMPLEMENT_SUBCLASS(Agent, "UbfAgent")
EMPTY_SERIALIZER(Agent)
EMPTY_COPYDATA(Agent)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Agent)
   "state",                      //  1) The agent's state object
   "behavior"                    //  2) behavior
END_SLOTTABLE(Agent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Agent)
   ON_SLOT(1, setSlotState, State)
   ON_SLOT(2, setSlotBehavior, Behavior)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Agent::Agent()
{
   STANDARD_CONSTRUCTOR()
   myActor = nullptr;
   behavior = nullptr;
   state = nullptr;
}

void Agent::deleteData()
{
   if ( behavior!=nullptr ) { behavior->unref(); behavior = nullptr; }
   if ( state!=nullptr )    { state->unref(); state = nullptr; }

   myActor = nullptr;
}

//------------------------------------------------------------------------------
// Reset the system
//------------------------------------------------------------------------------
void Agent::reset()
{
   // Reset our behavior and state objects
   if (behavior != nullptr) {
      behavior->reset();
   }
   if (state != nullptr) {
      state->reset();
   }

   myActor=nullptr;
   initActor();

   // although state is not explicitly initialized as component, the set state
   // method sets up the component relationship since state is a component, it
   // will get the reset() this way (via the component i/f)
   BaseClass::reset();
}


//------------------------------------------------------------------------------
// updateTC() -
//------------------------------------------------------------------------------
void Agent::updateTC(const LCreal dt)
{
}


//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Agent::updateData(const LCreal dt)
{
   controller(dt);
}


//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void Agent::controller(const LCreal dt)
{
   Basic::Component* actor = getActor();

   if ( (actor!=nullptr) && (getState()!=nullptr) && (getBehavior()!=nullptr) ) {

      // update ubf state
      getState()->updateState(actor);

      // generate an action, but allow possibility of no action returned
      Action* action = getBehavior()->genAction(state, dt);
      if (action) {
         action->execute(actor);
         action->unref();
      }
   }
}


//------------------------------------------------------------------------------
// Set our behavior model
//------------------------------------------------------------------------------
void Agent::setBehavior(Behavior* const x)
{
   if (x==nullptr)
      return;
   if (behavior!=nullptr)
      behavior->unref();
   behavior = x;
   behavior->ref();
   behavior->container(this);
}


//------------------------------------------------------------------------------
// Set our state model
//------------------------------------------------------------------------------
void Agent::setState(State* const x)
{
   if (x==nullptr)
      return;
   if (state!=nullptr)
      state->unref();
   state = x;
   state->ref();
   state->container(this);
   Basic::Pair* p = new Basic::Pair("", state);
   addComponent(p);
   p->unref();
}


//------------------------------------------------------------------------------
// finds our actor during reset() processing
//------------------------------------------------------------------------------
void Agent::initActor()
{
   if (getActor()==nullptr) {
      // our actor is our container
      if (container() != nullptr) {
         setActor(container());
      }
   }
}


//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

// Sets the state object for this agent
bool Agent::setSlotState(State* const state)
{
   bool ok = false;
   if (state != nullptr) {
      setState(state);
      ok = true;
   }
   return ok;
}

bool Agent::setSlotBehavior(Behavior* const x)
{
   bool ok = false;
   if ( x!=nullptr ) {
      setBehavior(x);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Agent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}


//==============================================================================
// Class: AgentTC
// Description: An Agent that manages a component (the "actor") with a behavior,
//              using TC thread to perform its activity (instead of BG thread)
//==============================================================================

IMPLEMENT_SUBCLASS(AgentTC, "UbfAgentTC")
EMPTY_SLOTTABLE(AgentTC)
EMPTY_CONSTRUCTOR(AgentTC)
EMPTY_SERIALIZER(AgentTC)
EMPTY_COPYDATA(AgentTC)
EMPTY_DELETEDATA(AgentTC)

//------------------------------------------------------------------------------
// updateTC() - Calls the controller
//------------------------------------------------------------------------------
void AgentTC::updateTC(const LCreal dt)
{
   controller(dt);
}

//------------------------------------------------------------------------------
// updateData() -
//------------------------------------------------------------------------------
void AgentTC::updateData(const LCreal dt)
{
}

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

