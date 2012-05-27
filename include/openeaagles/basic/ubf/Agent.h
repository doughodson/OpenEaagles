//------------------------------------------------------------------------------
// Class: Agent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Ubf_Agent_H__
#define __Eaagles_Basic_Ubf_Agent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {

namespace Simulation { class Simulation; class Station;  }

namespace Basic {

namespace Ubf {

class UbfBehavior;
class UbfState;
class UbfAction;

//
// Class: UbfAgent
//
// Base class: Basic::Component -> UbfAgent
//
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//    newUbf version allows component behaviors to return null actions.
//    newUbf actions know how to execute themselves, so agent does not need to know anything about action class.
//    newUbf agent's state is initialized by slot, so agent does not need to know anything about state class.
//
class UbfAgent : public Basic::Component
{
   DECLARE_SUBCLASS(UbfAgent, Basic::Component)
public:
   UbfAgent();

   // Basic::Component Interface
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

protected:
   // generic controller
   virtual void controller(const LCreal dt = 0.0f);

   void setBehavior(UbfBehavior* const);
   UbfBehavior* getBehavior() const          { return behavior; }

   void setState(UbfState* const);
   UbfState* getState() const                { return state; }

   Basic::Component*       getActor();
   Simulation::Station*     getStation();
   Simulation::Simulation*  getSimulation();

   // sets which component ("actor") the agent is for
   void setActorPlayerByName(const char*);
   void setActorComponentByName(const char*);

   // slot functions
   virtual bool setSlotBehavior(UbfBehavior* const);
   // slot to set agent's actor (component to be controlled)
   bool setSlotActorPlayerName(const Basic::String* const);
   bool setSlotActorComponentName(const Basic::String* const);
   bool setSlotState(UbfState* const state);

private:
   UbfBehavior* behavior;
   UbfState* state;
   const Basic::String*    actorPlayerName;
   const Basic::String*    actorComponentName;
   Simulation::Station*    myStation;
   Basic::Component* myActor;
};

//
// Class: Agent
//
// Base class: Basic::Component -> Agent
//
// Description: Generic agent class to control a component - the agent's "actor"
// - a derived agent class that performs its actions in the TC thread
class AgentTC : public UbfAgent
{
   DECLARE_SUBCLASS(AgentTC, UbfAgent)
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
