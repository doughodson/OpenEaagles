
#include "openeaagles/ubf/soar/SoarState.h"

#include "soar/sml_Client.h"

namespace oe {
namespace soar {
    
IMPLEMENT_SUBCLASS(SoarState, "SoarState")
EMPTY_COPYDATA(SoarState)
EMPTY_SERIALIZER(SoarState)
EMPTY_SLOTTABLE(SoarState)
EMPTY_DELETEDATA(SoarState)

SoarState::SoarState()
{
}

SoarState::SoarState(sml::Agent* agent)
{
    setAgent(agent);
}

}
}
