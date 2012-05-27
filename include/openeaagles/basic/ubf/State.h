//------------------------------------------------------------------------------
// Class: State
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_State_H__
#define __Eaagles_Basic_State_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {

//
// Class: State
//
// Description: Simple base class that supports derived state functionality;
//    newUbf version does nothing, but supports state/component infrastructure -
//    forwards the updateState and updateGlobalState calls to all state components
//
class State : public Basic::Component
{
   DECLARE_SUBCLASS(State, Basic::Component)
public:
   State();
   virtual void updateGlobalState(void);
   virtual void updateState(const Basic::Component* const actor);

   virtual const State* getUbfStateByType(const std::type_info& type) const;
};

} // End Basic namespace
} // End Eaagles namespace

#endif

