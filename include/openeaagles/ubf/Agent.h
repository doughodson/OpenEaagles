//
// Class: Agent
//
// Base class: Basic::Component -> Agent
//
// Description: Generic controller class
//

#ifndef __Eaagles_Ubf_Agent_H__
#define __Eaagles_Ubf_Agent_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {

namespace Simulation { class Player; class Simulation; class Station;  }

namespace Ubf {

class Behavior;
class State;
class Action;

class Agent : public Basic::Component
{
   DECLARE_SUBCLASS(Agent, Basic::Component)

public:

   Agent();

   // component interface
   virtual void updateData(const LCreal dt = 0.0f);

   // sets which player the agent is for
   void setOwnshipByName(const char*);

   // slot to set agents ownship (player)
   bool setSlotOwnshipName(const Basic::String* const);

   Simulation::Player* getOwnship();

   void setBehavior(Behavior* const);
   Behavior* getBehavior() const          { return behavior; }

   // slot functions
   virtual bool setSlotBehavior(Behavior* const);

protected:

   // generic controller
   virtual void controller(const LCreal dt = 0.0f);

   //
   void setState(State* const);
   State* getState() const                { return state; }

   // executes action
   virtual void executeAction(const Action* const action) = 0;

   Simulation::Station* getStation();
   Simulation::Simulation* getSimulation();

private:

   Behavior* behavior;
   State* state;

   Simulation::Station* myStation;
   const Basic::String* ownshipName;

};

}
}

#endif
