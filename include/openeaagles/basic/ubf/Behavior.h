//------------------------------------------------------------------------------
// Class: Behavior
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Behavior_H__
#define __Eaagles_Basic_Behavior_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {

class State;
class Action;

//
// Class: Behavior
//
// Description: Abstract base class for all behaviors
//
class Behavior : public Basic::Component
{
   DECLARE_SUBCLASS(Behavior, Basic::Component)

public:
   Behavior();

   virtual Action* genAction(const State* const state, const LCreal dt) = 0;

protected:
   virtual void setVote(const unsigned int x);
   unsigned int getVote() const;

protected:
   bool setSlotVote(const Basic::Number* const num);

private:
   unsigned int vote;
};

inline void Behavior::setVote(const unsigned int x)    { vote = x; }
inline unsigned int Behavior::getVote() const          { return vote; }


} // End Basic namespace
} // End Eaagles namespace

#endif

