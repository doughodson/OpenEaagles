//------------------------------------------------------------------------------
// Class: Agent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Agent_H__
#define __Eaagles_Basic_Agent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {

class Behavior;
class State;
class Action;

//
// Class: Agent
//
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//    newUbf version allows component behaviors to return null actions.
//    newUbf actions know how to execute themselves, so agent does not need to know anything about action class.
//    newUbf agent's state is initialized by slot, so agent does not need to know anything about state class.
//
class Agent : public Basic::Component
{
   DECLARE_SUBCLASS(Agent, Basic::Component)
public:
   Agent();

   // Basic::Component Interface
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0f);

   void setBehavior(Behavior* const);
   Behavior* getBehavior() const          { return behavior; }

   void setState(State* const);
   State* getState() const                { return state; }

   Basic::Component*       getActor();

   // slot functions
   virtual bool setSlotBehavior(Behavior* const);
   bool setSlotState(State* const state);

private:
   Behavior* behavior;
   State* state;
   Basic::Component* myActor;
};

//
// Class: Agent
//
// Base class: Basic::Component -> Agent
//
// Description: Generic agent class to control a component - the agent's "actor"
// - a derived agent class that performs its actions in the TC thread
class AgentTC : public Agent
{
   DECLARE_SUBCLASS(AgentTC, Agent)
public:
   AgentTC();
   // Basic::Component Interface
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
};

} // End Basic namespace
} // End Eaagles namespace

#endif

