
#ifndef __oe_base_ubf_AbstractAction_H__
#define __oe_base_ubf_AbstractAction_H__

#include "openeaagles/base/Object.hpp"

namespace oe {

namespace base {
class Component;

namespace ubf {

//------------------------------------------------------------------------------
// Class: AbstractAction
//
// Description:
//    Abstract base class for all Actions. They are responsible for
//    their own execution.
//
// Factory name: UbfAction
//------------------------------------------------------------------------------
class AbstractAction : public base::Object
{
   DECLARE_SUBCLASS(AbstractAction, base::Object)

public:
   AbstractAction();

   unsigned int getVote() const;
   void setVote(const unsigned int x);

   // Execute the behavior
   virtual bool execute(base::Component* actor)=0;

private:
   unsigned int vote;
};

inline void AbstractAction::setVote(const unsigned int x)      { vote = x; return; }
inline unsigned int AbstractAction::getVote() const            { return vote; }

}
}
}

#endif

