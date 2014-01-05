//------------------------------------------------------------------------------
// Class: Behavior
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Ubf_Behavior_H__
#define __Eaagles_Basic_Ubf_Behavior_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {
namespace Ubf {

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
class Behavior : public Basic::Component
{
   DECLARE_SUBCLASS(Behavior, Basic::Component)

public:
   Behavior();

   // Returns a pre-ref'd Action (or zero if no action is generated)
   virtual Action* genAction(const State* const state, const LCreal dt) = 0;

protected:
   unsigned int getVote() const;
   virtual void setVote(const unsigned int x);

   bool setSlotVote(const Basic::Number* const num);

private:
   unsigned int vote;
};

inline void Behavior::setVote(const unsigned int x)    { vote = x; }
inline unsigned int Behavior::getVote() const          { return vote; }


} // End Ubf namespace
} // End Basic namespace
} // End Eaagles namespace

#endif
