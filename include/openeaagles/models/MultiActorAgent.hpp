
#ifndef __oe_models_MultiActorAgent_H__
#define __oe_models_MultiActorAgent_H__

#include "openeaagles/base/Component.hpp"

namespace oe {

namespace base {
namespace ubf { class Behavior; class State; }
}

namespace simulation {
class ISimulation;
class Station;
}

namespace models {

//------------------------------------------------------------------------------
// Class: MultiActorAgent
//
// Description: Generic agent class to control a list of actors, each specified with their own behavior
//    the only reason to use this class is if there is state shared between multiple actors
//    (if there is not shared state, just use a list of standard Agent instances)
//
// This class assumes that state has two elements - shared state for all actors, and
// local state for each actor
//
// Factory name: MultiActorAgent
// Slots:
//    state       <State>           ! state
//    agentList   <PairStream>      ! behavior pairstream
//------------------------------------------------------------------------------
class MultiActorAgent : public base::Component
{
   DECLARE_SUBCLASS(MultiActorAgent, base::Component)

public:
   MultiActorAgent();

   virtual void updateData(const double dt = 0.0) override;
   virtual void reset() override;

protected:
   // generic controller
   virtual void controller(const double dt = 0.0);

   void setState(base::ubf::State* const);
   base::ubf::State* getState() const                { return state; }

   void setActor(base::Component* c);
   base::Component*      getActor() { return actor;}

   simulation::Station*    getStation();
   simulation::ISimulation* getSimulation();

   struct AgentItem
   {
      base::safe_ptr<base::String> actorName;
      base::safe_ptr<base::ubf::Behavior> behavior;
      base::safe_ptr<base::Component> actor;
   };

   static const unsigned int MAX_AGENTS = 10;
   bool clearAgentList();
   bool addAgent( base::String* name, base::ubf::Behavior* const b);

   // slot functions
   bool setSlotState(base::ubf::State* const state);
   bool setSlotAgentList(base::PairStream* const msg);

private:
   base::Component* actor;
   base::ubf::State* state;
   simulation::Station* myStation;

   // agent/behavior list
   unsigned int nAgents;          // Number of input behavior/agent pairs
   AgentItem agentList[MAX_AGENTS];
};

inline void MultiActorAgent::setActor(base::Component* c) { actor=c; }

}
}


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

