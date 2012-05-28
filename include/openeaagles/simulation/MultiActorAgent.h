//------------------------------------------------------------------------------
// Class: MultiActorAgent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_MultiActorAgent_H__
#define __Eaagles_Simulation_MultiActorAgent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {

namespace Basic { class Behavior; class State; class Action; }

namespace Simulation {

class Simulation;
class Station;

//
// Class: MultiActorAgent
//
// Description: Generic agent class to control a list of actors, each specified with their own behavior
//    the only reason to use this class is if there is state shared between multiple actors
//    (if there is not shared state, just use a list of standard Agent instances)
//
class MultiActorAgent : public Basic::Component
{
   DECLARE_SUBCLASS(MultiActorAgent, Basic::Component)

public:
   MultiActorAgent();

   // Basic::Component Interface
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0f);

   void setState(Basic::State* const);
   Basic::State* getState() const                { return state; }

   void setActor(Basic::Component* c);
   Basic::Component*      getActor() { return actor;}

   Station*     getStation();
   Simulation*  getSimulation();

   struct AgentItem
   {
      SPtr<Basic::String> actorName;
      SPtr<Basic::Behavior> behavior;
      SPtr<Basic::Component> actor;
   };

   static const unsigned int MAX_AGENTS = 10;
   bool clearAgentList();
   bool addAgent( Basic::String* name, Basic::Behavior* const b);

   // slot functions
   bool setSlotState(Basic::State* const state);
   bool setSlotAgentList(Basic::PairStream* const msg);

private:
   Basic::Component* actor;
   Basic::State* state;
   Station*     myStation;

   // agent/behavior list
   unsigned int nAgents;          // Number of input behavior/agent pairs
   AgentItem agentList[MAX_AGENTS];
};
inline void MultiActorAgent::setActor(Basic::Component* c) { actor=c; }


} // End Simulation namespace
} // End Eaagles namespace

#endif

