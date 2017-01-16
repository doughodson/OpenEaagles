
#ifndef __oe_base_ubf_AbstractState_H__
#define __oe_base_ubf_AbstractState_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base {
namespace ubf {

//------------------------------------------------------------------------------
// Class: AbstractState
//
// Description: The actor's state vector, as seen by the Behaviors.
//
// Factory name: UbfState
//------------------------------------------------------------------------------
class AbstractState : public base::Component
{
   DECLARE_SUBCLASS(AbstractState, base::Component)
public:
   AbstractState();

   virtual void updateGlobalState();
   virtual void updateState(const base::Component* const actor);

   virtual const AbstractState* getUbfStateByType(const std::type_info& type) const;
};

}
}
}

#endif
