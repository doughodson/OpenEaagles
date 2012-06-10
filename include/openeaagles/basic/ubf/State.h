//------------------------------------------------------------------------------
// Class: State
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Ubf_State_H__
#define __Eaagles_Basic_Ubf_State_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

//------------------------------------------------------------------------------
// Class: State
//
// Description:
//    Simple base class that supports derived state functionality;
//    the object does "nothing," but supports state/component infrastructure -
//    forwards the updateState and updateGlobalState calls to all state components
//
// Form name: UbfState
//------------------------------------------------------------------------------
class State : public Basic::Component
{
   DECLARE_SUBCLASS(State, Basic::Component)
public:
   State();
   virtual void updateGlobalState(void);
   virtual void updateState(const Basic::Component* const actor);

   virtual const State* getUbfStateByType(const std::type_info& type) const;
};

} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

#endif

