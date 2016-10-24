
#ifndef __oe_base_ubf_Agent_H__
#define __oe_base_ubf_Agent_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base {
namespace ubf {

class Behavior;
class State;
class Action;

//------------------------------------------------------------------------------
// Class: Agent
//
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//
// Notes:
// 1) Use 'Agent' to update the behavior framework via updateData() and use
//    'AgentTC' to update the behavior framework using updateTC().
//
// 2) The updateData() and updateTC() calls are only processed by this Agent
//    class and are not passed to the rest of the behavior framework.
//
//
// Factory name: UbfAgent
// Slots:
//    state       <State>     ! The agent's state object
//    behavior    <Behavior>  ! behavior
//------------------------------------------------------------------------------
class Agent : public base::Component
{
   DECLARE_SUBCLASS(Agent, base::Component)

public:
   Agent();

   virtual void updateTC(const double dt = 0.0) override;
   virtual void updateData(const double dt = 0.0) override;
   virtual void reset() override;

protected:
   // generic controller
   virtual void controller(const double dt = 0.0);

   Behavior* getBehavior() const          { return behavior; }
   void setBehavior(Behavior* const);

   State* getState() const                { return state; }
   void setState(State* const);

   virtual void initActor();

   base::Component* getActor();
   void setActor(base::Component* const myActor);

   // slot functions
   virtual bool setSlotBehavior(Behavior* const);
   bool setSlotState(State* const state);

private:
   Behavior* behavior;
   State* state;
   safe_ptr<base::Component> myActor;
};

inline void Agent::setActor(base::Component* const actor)      { myActor = actor; return; }
inline base::Component* Agent::getActor()                      { return myActor; }


//------------------------------------------------------------------------------
// Class: Agent
//
// Description: Generic agent class to control a component - the agent's "actor"
// - a derived agent class that performs its actions in the TC thread
//
// Factory name: UbfAgentTC
//------------------------------------------------------------------------------
class AgentTC : public Agent
{
   DECLARE_SUBCLASS(AgentTC, Agent)

public:
   AgentTC();

   // Component interface
   virtual void updateTC(const double dt = 0.0) override;
   virtual void updateData(const double dt = 0.0) override;
};

}
}
}

#endif

