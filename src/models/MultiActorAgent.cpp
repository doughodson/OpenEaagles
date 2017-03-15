
#include "openeaagles/models/MultiActorAgent.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/WorldModel.hpp"

#include "openeaagles/simulation/Station.hpp"

#include "openeaagles/base/ubf/AbstractAction.hpp"
#include "openeaagles/base/ubf/AbstractBehavior.hpp"
#include "openeaagles/base/ubf/AbstractState.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/String.hpp"

namespace oe {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(MultiActorAgent, "MultiActorAgent")
EMPTY_SERIALIZER(MultiActorAgent)
EMPTY_COPYDATA(MultiActorAgent)

BEGIN_SLOTTABLE(MultiActorAgent)
   "state",                      //  1) state
   "agentList"                   //  2) behavior pairstream
END_SLOTTABLE(MultiActorAgent)

BEGIN_SLOT_MAP(MultiActorAgent)
   ON_SLOT(1,  setSlotState, base::ubf::AbstractState)
   ON_SLOT(2,  setSlotAgentList, base::PairStream)
END_SLOT_MAP()

MultiActorAgent::MultiActorAgent()
{
   STANDARD_CONSTRUCTOR()
}

void MultiActorAgent::deleteData()
{
   // unref state
   if ( state != nullptr )    { state->unref(); state = nullptr; }
   clearAgentList();
}

void MultiActorAgent::reset()
{
   WorldModel* sim = getWorldModel();
   if (sim != nullptr) {
      // convert component names to component ptrs, for all behaviors in the list
      for (unsigned int i=0; i<nAgents; i++) {
         base::Component* c = sim->findPlayerByName(agentList[i].actorName->getString());
         if (c != nullptr) {
            agentList[i].actor = c;
            // send reset to each
            agentList[i].behavior->reset();
         }
      }
   }
   //if (state != 0) {
   //   state->reset();
   //}
   // state is a component, so it will get the reset() this way
   BaseClass::reset();
}

void MultiActorAgent::updateData(const double dt)
{
   // update base class stuff first
   BaseClass::updateData(dt);
   controller(dt);
}

void MultiActorAgent::controller(const double dt)
{
   if ( (getState() != nullptr) && (nAgents>0) ) {
      // update global state once for all agents
      getState()->updateGlobalState();

      // for each behavior/actor pair, update state and generate action
      for (unsigned int i=0; i<nAgents; i++) {
         if (agentList[i].actor != nullptr) {

            setActor(agentList[i].actor);
            base::ubf::AbstractBehavior* behavior = agentList[i].behavior;

            // update ubf state
            getState()->updateState(agentList[i].actor);

            // generate an action
            base::ubf::AbstractAction* action = behavior->genAction(getState(), dt);
            if (action) { // allow possibility of no action returned
               action->execute(getActor());
               action->unref();
            }
         }
      }
      setActor(nullptr);
   }
}

void MultiActorAgent::setState(base::ubf::AbstractState* const x)
{
   if (x == nullptr)
      return;
   if (state != nullptr)
      state->unref();
   state = x;
   state->ref();
   state->container(this);
   const auto p = new base::Pair("", state);
   addComponent(p);
   p->unref();
}

simulation::Station* MultiActorAgent::getStation()
{
   if ( myStation == nullptr ) {
      const auto s = dynamic_cast<simulation::Station*>(findContainerByType(typeid(simulation::Station)));
      if (s != nullptr) {
         myStation = s;
      }
   }
   return myStation;
}

WorldModel* MultiActorAgent::getWorldModel()
{
   WorldModel* sim = nullptr;
   simulation::Station* s = getStation();
   if (s != nullptr) {
      sim = dynamic_cast<WorldModel*>(s->getSimulation());
   }
   return sim;
}

// Clears the input entity type table
bool MultiActorAgent::clearAgentList()
{
   // Clear our agent list
   while (nAgents > 0) {
      nAgents--;
      agentList[nAgents].actorName = nullptr;
      agentList[nAgents].behavior = nullptr;
      agentList[nAgents].actor = nullptr;
   }
   return true;
}

// Adds an item to the input entity type table
bool MultiActorAgent::addAgent(base::String* name, base::ubf::AbstractBehavior* const b)
{
   bool ok = false;
   if (nAgents < MAX_AGENTS) {
      agentList[nAgents].actorName = name;
      agentList[nAgents].behavior = b;
      agentList[nAgents].actor = nullptr;
      nAgents++;
      b->container(this);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// set slot functions
//------------------------------------------------------------------------------

// Sets the state object for this agent
bool MultiActorAgent::setSlotState(base::ubf::AbstractState* const state)
{
   bool ok = false;
   if (state != nullptr) {
      setState(state);
      ok = true;
   }
   return ok;
}

// Sets the actor/behavior list
bool MultiActorAgent::setSlotAgentList(base::PairStream* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       // First clear the old list
       clearAgentList();

       // Now scan the pair stream and put all Ntm objects into the table.
       base::List::Item* item = msg->getFirstItem();
       while (item != nullptr) {
          const auto pair = static_cast<base::Pair*>(item->getValue());
          //std::cerr << "MultiActorAgent::setSlotagentList: slot: " << *pair->slot() << std::endl;
          const auto b = dynamic_cast<base::ubf::AbstractBehavior*>( pair->object() );
          if (b != nullptr) {
             // We have an  object, so put it in the table
             addAgent(pair->slot(), b);
          }
          item = item->getNext();
       }
       ok = true;
    }
    return ok;
}

}
}
