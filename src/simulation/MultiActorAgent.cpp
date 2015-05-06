//------------------------------------------------------------------------------
// Class: MultiActorAgent
//------------------------------------------------------------------------------

#include "openeaagles/simulation/MultiActorAgent.h"

#include "openeaagles/basic/ubf/Action.h"
#include "openeaagles/basic/ubf/Behavior.h"
#include "openeaagles/basic/ubf/State.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

namespace Eaagles {
namespace Simulation {

//
// Class: MultiActorAgent
//
// Description: An Agent that can manage one or a list of actors, each with own behavior
// The only reason to use this class is if there is state shared between multiple agents.
// This class assumes that state has two elements - shared state for all actors, and
// local state for each actor
//
//

IMPLEMENT_ABSTRACT_SUBCLASS(MultiActorAgent, "MultiActorAgent")
EMPTY_SERIALIZER(MultiActorAgent)
EMPTY_COPYDATA(MultiActorAgent)

// slot table for this class type
BEGIN_SLOTTABLE(MultiActorAgent)
   "state",                      //  1) state
   "agentList"                   //  2) behavior pairstream
END_SLOTTABLE(MultiActorAgent)

//  mapping of slots to handles
BEGIN_SLOT_MAP(MultiActorAgent)
   ON_SLOT(1,  setSlotState, Basic::Ubf::State)
   ON_SLOT(2,  setSlotAgentList, Basic::PairStream)
END_SLOT_MAP()

MultiActorAgent::MultiActorAgent()
{
   STANDARD_CONSTRUCTOR()
   nAgents = 0;
   state = nullptr;
   myStation = nullptr;
   actor = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void MultiActorAgent::deleteData()
{
   // unref state
   if ( state != nullptr )    { state->unref(); state = nullptr; }
   clearAgentList();
}

void MultiActorAgent::reset()
{
   Simulation* sim = getSimulation();
   if (sim != nullptr) {
      // convert component names to component ptrs, for all behaviors in the list
      for (unsigned int i=0; i<nAgents; i++) {
         Basic::Component* c = sim->findPlayerByName(agentList[i].actorName->getString());
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

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void MultiActorAgent::updateData(const LCreal dt)
{
   // update base class stuff first
   BaseClass::updateData(dt);
   controller(dt);
}

void MultiActorAgent::controller(const LCreal dt)
{
   if ( (getState() != nullptr) && (nAgents>0) ) {
      // update global state once for all agents
      getState()->updateGlobalState();

      // for each behavior/actor pair, update state and generate action
      for (unsigned int i=0; i<nAgents; i++) {
         if (agentList[i].actor != nullptr) {

            setActor(agentList[i].actor);
            Basic::Ubf::Behavior* behavior = agentList[i].behavior;

            // update ubf state
            getState()->updateState(agentList[i].actor);

            // generate an action
            Basic::Ubf::Action* action = behavior->genAction(getState(), dt);
            if (action) { // allow possibility of no action returned
               action->execute(getActor());
               action->unref();
            }
         }
      }
      setActor(nullptr);
   }
}


void MultiActorAgent::setState(Basic::Ubf::State* const x)
{
   if (x == nullptr)
      return;
   if (state != nullptr)
      state->unref();
   state = x;
   state->ref();
   state->container(this);
   Basic::Pair* p = new Basic::Pair("", state);
   addComponent(p);
   p->unref();
}

Station* MultiActorAgent::getStation()
{
   if ( myStation == nullptr ) {
      Station* s = dynamic_cast<Station*>(findContainerByType(typeid(Station)));
      if (s != nullptr) {
         myStation = s;
      }
   }
   return myStation;
}
//
Simulation* MultiActorAgent::getSimulation()
{
   Simulation* sim = nullptr;
   Station* s = getStation();
   if (s != nullptr) {
      sim = s->getSimulation();
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
bool MultiActorAgent::addAgent(Basic::String* name, Basic::Ubf::Behavior* const b)
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
bool MultiActorAgent::setSlotState(Basic::Ubf::State* const state)
{
   bool ok = false;
   if (state != nullptr) {
      setState(state);
      ok = true;
   }
   return ok;
}

// Sets the actor/behavior list
bool MultiActorAgent::setSlotAgentList(Basic::PairStream* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       // First clear the old list
       clearAgentList();

       // Now scan the pair stream and put all Ntm objects into the table.
       Basic::List::Item* item = msg->getFirstItem();
       while (item != nullptr) {
          Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
          //std::cerr << "MultiActorAgent::setSlotagentList: slot: " << *pair->slot() << std::endl;
          Basic::Ubf::Behavior* b = dynamic_cast<Basic::Ubf::Behavior*>( pair->object() );
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

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* MultiActorAgent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace


