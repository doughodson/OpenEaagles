
#ifndef __oe_base_ubf_AbstractBehavior_H__
#define __oe_base_ubf_AbstractBehavior_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base {
namespace ubf {

class AbstractState;
class AbstractAction;

//------------------------------------------------------------------------------
// Class: AbstractBehavior
// Description: Abstract base class for all behaviors.  Generates an optional
//              action based on our current state.
//
// Factory name: UbfBehavior
// Slots:
//    vote     <Number>    ! default vote/weight value for actions generated
//                         ! by this behavior
//------------------------------------------------------------------------------
class AbstractBehavior : public base::Component
{
   DECLARE_SUBCLASS(AbstractBehavior, base::Component)

public:
   AbstractBehavior();

   // Returns a pre-ref'd Action (or zero if no action is generated)
   virtual AbstractAction* genAction(const AbstractState* const state, const double dt) = 0;

protected:
   unsigned int getVote() const;
   virtual void setVote(const unsigned int x);

   bool setSlotVote(const base::Number* const num);

private:
   unsigned int vote {};
};

inline void AbstractBehavior::setVote(const unsigned int x)    { vote = x; }
inline unsigned int AbstractBehavior::getVote() const          { return vote; }

}
}
}

#endif
