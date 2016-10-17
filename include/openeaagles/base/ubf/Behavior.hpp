//------------------------------------------------------------------------------
// Class: Behavior
//------------------------------------------------------------------------------
#ifndef __oe_base_ubf_Behavior_H__
#define __oe_base_ubf_Behavior_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base {
namespace ubf {

class State;
class Action;

//------------------------------------------------------------------------------
// Class: Behavior
// Description: Abstract base class for all behaviors.  Generates an optional
//              action based on our current state.
//
// Factory name: UbfBehavior
// Slots:
//    vote     <Number>    ! default vote/weight value for actions generated
//                         ! by this behavior
//------------------------------------------------------------------------------
class Behavior : public base::Component
{
   DECLARE_SUBCLASS(Behavior, base::Component)

public:
   Behavior();

   // Returns a pre-ref'd Action (or zero if no action is generated)
   virtual Action* genAction(const State* const state, const double dt) = 0;

protected:
   unsigned int getVote() const;
   virtual void setVote(const unsigned int x);

   bool setSlotVote(const base::Number* const num);

private:
   unsigned int vote;
};

inline void Behavior::setVote(const unsigned int x)    { vote = x; }
inline unsigned int Behavior::getVote() const          { return vote; }

}
}
}

#endif
