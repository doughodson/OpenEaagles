
#include "openeaagles/base/ubf/Agent.hpp"
#include "openeaagles/base/ubf/AbstractAction.hpp"
#include "openeaagles/base/ubf/AbstractBehavior.hpp"
#include "openeaagles/base/ubf/AbstractState.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/String.hpp"

namespace oe {
namespace base {
namespace ubf {

//
// Class: Agent
//
// Description: An Agent that manages a component (the "actor") with a behavior
//              (either a player, or a player's component)
//

IMPLEMENT_SUBCLASS(Agent, "UbfAgent")
EMPTY_SERIALIZER(Agent)
EMPTY_COPYDATA(Agent)

BEGIN_SLOTTABLE(Agent)
   "state",                      //  1) The agent's state object
   "behavior"                    //  2) behavior
END_SLOTTABLE(Agent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(Agent)
   ON_SLOT(1, setSlotState, AbstractState)
   ON_SLOT(2, setSlotBehavior, AbstractBehavior)
END_SLOT_MAP()

Agent::Agent()
{
   STANDARD_CONSTRUCTOR()
}

void Agent::deleteData()
{
   if ( behavior!=nullptr ) { behavior->unref(); behavior = nullptr; }
   if ( state!=nullptr )    { state->unref(); state = nullptr; }

   myActor = nullptr;
}

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

void Agent::updateData(const double dt)
{
   controller(dt);
}

void Agent::controller(const double dt)
{
   base::Component* actor = getActor();

   if ( (actor!=nullptr) && (getState()!=nullptr) && (getBehavior()!=nullptr) ) {

      // update ubf state
      getState()->updateState(actor);

      // generate an action, but allow possibility of no action returned
      AbstractAction* action = getBehavior()->genAction(state, dt);
      if (action) {
         action->execute(actor);
         action->unref();
      }
   }
}


//------------------------------------------------------------------------------
// Set our behavior model
//------------------------------------------------------------------------------
void Agent::setBehavior(AbstractBehavior* const x)
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
void Agent::setState(AbstractState* const x)
{
   if (x==nullptr)
      return;
   if (state!=nullptr)
      state->unref();
   state = x;
   state->ref();
   state->container(this);
   const auto p = new base::Pair("", state);
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
bool Agent::setSlotState(AbstractState* const state)
{
   bool ok = false;
   if (state != nullptr) {
      setState(state);
      ok = true;
   }
   return ok;
}

bool Agent::setSlotBehavior(AbstractBehavior* const x)
{
   bool ok = false;
   if ( x!=nullptr ) {
      setBehavior(x);
      ok = true;
   }
   return ok;
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
void AgentTC::updateTC(const double dt)
{
   controller(dt);
}

}
}
}
