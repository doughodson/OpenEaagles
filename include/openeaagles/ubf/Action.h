//
// Class: Action
//
// Base class: Basic::Object -> Action
//
// Description: Generic action class
//

#ifndef __Eaagles_Ubf_Action_H__
#define __Eaagles_Ubf_Action_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {

namespace Ubf {

class Sensor;

class Action : public Basic::Object
{
   DECLARE_SUBCLASS(Action, Basic::Object)

public:

   Action();

   void setVote(const unsigned int x)      { vote = x; return; }
   unsigned int getVote() const            { return vote; }

private:

   unsigned int vote;

};

}
}

#endif

