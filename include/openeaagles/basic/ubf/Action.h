//------------------------------------------------------------------------------
// Class: Action
//------------------------------------------------------------------------------
#ifndef __oe_basic_ubf_Action_H__
#define __oe_basic_ubf_Action_H__

#include "openeaagles/basic/Object.h"

namespace oe {

namespace basic {
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
class Action : public basic::Object
{
   DECLARE_SUBCLASS(Action, basic::Object)

public:
   Action();

   unsigned int getVote() const;
   void setVote(const unsigned int x);

   // Execute the behavior
   virtual bool execute(basic::Component* actor)=0;

private:
   unsigned int vote;
};

inline void Action::setVote(const unsigned int x)      { vote = x; return; }
inline unsigned int Action::getVote() const            { return vote; }

} // End ubf namespace
} // End basic namespace
} // End oe namespace

#endif

