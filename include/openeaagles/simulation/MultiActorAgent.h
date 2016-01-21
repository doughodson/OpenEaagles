//------------------------------------------------------------------------------
// Class: MultiActorAgent
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_MultiActorAgent_H__
#define __oe_Simulation_MultiActorAgent_H__

#include "openeaagles/basic/Component.h"

namespace oe {
   namespace basic {
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
class MultiActorAgent : public basic::Component
{
   DECLARE_SUBCLASS(MultiActorAgent, basic::Component)

public:
   MultiActorAgent();

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0);

   void setState(basic::Ubf::State* const);
   basic::Ubf::State* getState() const                { return state; }

   void setActor(basic::Component* c);
   basic::Component*      getActor() { return actor;}

   Station*     getStation();
   Simulation*  getSimulation();

   struct AgentItem
   {
      basic::safe_ptr<basic::String> actorName;
      basic::safe_ptr<basic::Ubf::Behavior> behavior;
      basic::safe_ptr<basic::Component> actor;
   };

   static const unsigned int MAX_AGENTS = 10;
   bool clearAgentList();
   bool addAgent( basic::String* name, basic::Ubf::Behavior* const b);

   // slot functions
   bool setSlotState(basic::Ubf::State* const state);
   bool setSlotAgentList(basic::PairStream* const msg);

private:
   basic::Component* actor;
   basic::Ubf::State* state;
   Station*     myStation;

   // agent/behavior list
   unsigned int nAgents;          // Number of input behavior/agent pairs
   AgentItem agentList[MAX_AGENTS];
};

inline void MultiActorAgent::setActor(basic::Component* c) { actor=c; }

} // End Simulation namespace
} // End oe namespace


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

