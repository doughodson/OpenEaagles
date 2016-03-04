//------------------------------------------------------------------------------
// Class: State
//------------------------------------------------------------------------------
#ifndef __oe_base_ubf_State_H__
#define __oe_base_ubf_State_H__

#include "openeaagles/base/Component.h"

namespace oe {
namespace basic {
namespace ubf {

//------------------------------------------------------------------------------
// Class: State
//
// Description: The actor's state vector, as seen by the Behaviors.
//
// Factory name: UbfState
//------------------------------------------------------------------------------
class State : public basic::Component
{
   DECLARE_SUBCLASS(State, basic::Component)
public:
   State();

   virtual void updateGlobalState(void);
   virtual void updateState(const basic::Component* const actor);

   virtual const State* getUbfStateByType(const std::type_info& type) const;
};

} // End ubf namespace
} // End basic namespace
} // End oe namespace

#endif

