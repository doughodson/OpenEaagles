//------------------------------------------------------------------------------
// Class: Agent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Ubf_Agent_H__
#define __Eaagles_Basic_Ubf_Agent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

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
// Form name: UbfAgent
// Slots:
//    state       <State>     ! The agent's state object
//    behavior    <Behavior>  ! behavior
//------------------------------------------------------------------------------
class Agent : public Basic::Component
{
   DECLARE_SUBCLASS(Agent, Basic::Component)
public:
   Agent();

   // Basic::Component Interface
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0f);

   Behavior* getBehavior() const          { return behavior; }
   void setBehavior(Behavior* const);

   State* getState() const                { return state; }
   void setState(State* const);

   virtual void initActor();

   Basic::Component* getActor();
   void setActor(Basic::Component* const myActor);

   // slot functions
   virtual bool setSlotBehavior(Behavior* const);
   bool setSlotState(State* const state);

private:
   Behavior* behavior;
   State* state;
   SPtr<Basic::Component> myActor;
};

inline void Agent::setActor(Basic::Component* const actor)      { myActor = actor; return; }
inline Basic::Component* Agent::getActor()           { return myActor; }


//------------------------------------------------------------------------------
// Class: Agent
//
// Description: Generic agent class to control a component - the agent's "actor"
// - a derived agent class that performs its actions in the TC thread
//
// Form name: UbfAgentTC
//------------------------------------------------------------------------------
class AgentTC : public Agent
{
   DECLARE_SUBCLASS(AgentTC, Agent)
public:
   AgentTC();
   // Basic::Component Interface
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
};

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

#endif

