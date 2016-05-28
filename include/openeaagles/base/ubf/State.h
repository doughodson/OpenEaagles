//------------------------------------------------------------------------------
// Class: State
//------------------------------------------------------------------------------
#ifndef __oe_base_ubf_State_H__
#define __oe_base_ubf_State_H__

#include "openeaagles/base/Component.h"

namespace oe {
namespace base {
namespace ubf {

//------------------------------------------------------------------------------
// Class: State
//
// Description: The actor's state vector, as seen by the Behaviors.
//
// Factory name: UbfState
//------------------------------------------------------------------------------
class State : public base::Component
{
   DECLARE_SUBCLASS(State, base::Component)
public:
   State();

   virtual void updateGlobalState();
   virtual void updateState(const base::Component* const actor);

   virtual const State* getUbfStateByType(const std::type_info& type) const;
};

}
}
}

#endif
