//------------------------------------------------------------------------------
// Class: MultiActorAgent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_MultiActorAgent_H__
#define __Eaagles_Simulation_MultiActorAgent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
   namespace Basic {
      namespace Ubf { class Behavior; class State; }
   }

namespace Simulation {

class Simulation;
class Station;

//------------------------------------------------------------------------------
// Class: MultiActorAgent
//
// Description: Generic agent class to control a list of actors, each specified with their own behavior
//    the only reason to use this class is if there is state shared between multiple actors
//    (if there is not shared state, just use a list of standard Agent instances)
//
// Factory name: MultiActorAgent
// Slots:
//    state       <State>           ! state
//    agentList   <PairStream>      ! behavior pairstream
//------------------------------------------------------------------------------
class MultiActorAgent : public Basic::Component
{
   DECLARE_SUBCLASS(MultiActorAgent, Basic::Component)

public:
   MultiActorAgent();

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0);

   void setState(Basic::Ubf::State* const);
   Basic::Ubf::State* getState() const                { return state; }

   void setActor(Basic::Component* c);
   Basic::Component*      getActor() { return actor;}

   Station*     getStation();
   Simulation*  getSimulation();

   struct AgentItem
   {
      Basic::safe_ptr<Basic::String> actorName;
      Basic::safe_ptr<Basic::Ubf::Behavior> behavior;
      Basic::safe_ptr<Basic::Component> actor;
   };

   static const unsigned int MAX_AGENTS = 10;
   bool clearAgentList();
   bool addAgent( Basic::String* name, Basic::Ubf::Behavior* const b);

   // slot functions
   bool setSlotState(Basic::Ubf::State* const state);
   bool setSlotAgentList(Basic::PairStream* const msg);

private:
   Basic::Component* actor;
   Basic::Ubf::State* state;
   Station*     myStation;

   // agent/behavior list
   unsigned int nAgents;          // Number of input behavior/agent pairs
   AgentItem agentList[MAX_AGENTS];
};

inline void MultiActorAgent::setActor(Basic::Component* c) { actor=c; }

} // End Simulation namespace
} // End Eaagles namespace


// A MultiActorAgent can be configured as shown below: (assuming the existence of the "abc" state and behavior classes)
//( Station
//   components: {
//      agent: ( MultiActorAgent
//         state: ( abcState )
//         agentList: {
//            // use slotname to specify the player name (the "actor") to which to connect the agent/behavior
//            p1: ( abcBehavior1 )
//            p2: ( abcBehavior2 )
//         }
//      )
//   } // components
//
//   simulation: ( Simulation
//
//      players: {
//
//         p1: ()
//
//         p2: ()
//
//      } // players
//   ) // simulation
//) // Station

#endif

