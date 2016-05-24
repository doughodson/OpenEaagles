//------------------------------------------------------------------------------
// Class: Action
//------------------------------------------------------------------------------
#ifndef __oe_base_ubf_Action_H__
#define __oe_base_ubf_Action_H__

#include "openeaagles/base/Object.h"

namespace oe {

namespace base {
   class Component;

namespace ubf {

//------------------------------------------------------------------------------
// Class: Action
//
// Description:
//    Abstract base class for all Actions. They are responsible for
//    their own execution.
//
// Factory name: UbfAction
//------------------------------------------------------------------------------
class Action : public base::Object
{
   DECLARE_SUBCLASS(Action, base::Object)

public:
   Action();

   unsigned int getVote() const;
   void setVote(const unsigned int x);

   // Execute the behavior
   virtual bool execute(base::Component* actor)=0;

private:
   unsigned int vote;
};

inline void Action::setVote(const unsigned int x)      { vote = x; return; }
inline unsigned int Action::getVote() const            { return vote; }

}
}
}

#endif

