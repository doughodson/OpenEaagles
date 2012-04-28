//------------------------------------------------------------------------------
// Class: AgentStation
//
// Base class: Simulation::Station -> AgentStation
//------------------------------------------------------------------------------

#ifndef __Eaagles_Ubf_AgentStation_H__
#define __Eaagles_Ubf_AgentStation_H__

#include "openeaagles/simulation/Station.h"

namespace Eaagles {

namespace Basic { class PairStream; }

namespace Ubf {

//------------------------------------------------------------------------------
// Class: AgentStation
//
// Description:
//
// Form Name: AgentStation
// Slots:
//      agents    <Basic::PairStream>     ! Agent list (Basic::PairStream of Agent)
//------------------------------------------------------------------------------

class AgentStation : public Simulation::Station
{
   DECLARE_SUBCLASS(AgentStation, Simulation::Station)

public:

   AgentStation();

   // Basic::Component Interface
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

   // slot functions
   virtual bool setSlotAgents(Basic::PairStream* const);

private:

   SPtr<Basic::PairStream> agents;  // Main agent list

};

}
}

#endif

