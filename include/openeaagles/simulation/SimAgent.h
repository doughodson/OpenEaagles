//------------------------------------------------------------------------------
// Class: Agent
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_SimAgent_H__
#define __Eaagles_Simulation_SimAgent_H__

#include "openeaagles/basic/ubf/Agent.h"

namespace Eaagles {
namespace Simulation { 
	class Simulation; class Station;
//
// Class: UbfAgent
//
// Base class: UbfAgent -> SimAgent
//
// Description: Generic agent class to control a component in the simulation - the agent's "actor"
//    newUbf version allows component behaviors to return null actions.
//    newUbf actions know how to execute themselves, so agent does not need to know anything about action class.
//    newUbf agent's state is initialized by slot, so agent does not need to know anything about state class.
//
class SimAgent : public Basic::Agent
{
   DECLARE_SUBCLASS(SimAgent, Basic::Agent)
public:
   SimAgent();

protected:

   virtual void initActor();

   Station*     getStation();
   Simulation*  getSimulation();

   // sets which component ("actor") the agent is for
   void setActorPlayerByName(const char*);
   void setActorComponentByName(const char*);

   // slot functions
   // slot to set agent's actor (component to be controlled)
   bool setSlotActorPlayerName(const Basic::String* const);
   bool setSlotActorComponentName(const Basic::String* const);

private:
   const Basic::String*    actorPlayerName;
   const Basic::String*    actorComponentName;
   Station*    myStation;
};


// A SimAgent can be configured as shown below:  (assuming the existence of the "abc" state and behavior classes)
//( Station
//
//   components: {
//      simpleSimAgent: ( SimAgent
//         actorPlayerName: p01
//         state: ( abcState )
//         behavior: ( abcBehavior1 )
//      )
//   } // components
//
//	simulation: ( Simulation
//    
//		players: {
//		
//			p01: ()
//			
//			p02: ()
//		} // players
//	) // simulation
//) // Station

} // End Simulation namespace
} // End Eaagles namespace

#endif
