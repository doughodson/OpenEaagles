//------------------------------------------------------------------------------
// Class:     Bullseye
//------------------------------------------------------------------------------
#ifndef __Bullseye_H_317EE5F9_9B22_4d6b_ACE4_98A2CCADC931
#define __Bullseye_H_317EE5F9_9B22_4d6b_ACE4_98A2CCADC931

#include "openeaagles/simulation/Steerpoint.h"

namespace Eaagles {
namespace Simulation {
//------------------------------------------------------------------------------
// Class:     Bullseye
// Base class:  Basic::Object -> Basic::Component -> Steerpoint -> Bullseye
// Form name: Bullseye
//
// Description: Generic Bullseye, used as a reference point
//------------------------------------------------------------------------------
class Bullseye : public Steerpoint
{
   DECLARE_SUBCLASS(Bullseye,Steerpoint)

public:
    Bullseye();
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Bullseye_H_317EE5F9_9B22_4d6b_ACE4_98A2CCADC931
