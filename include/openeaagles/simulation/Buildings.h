//------------------------------------------------------------------------------
// Classes:	Building
// Description: Generic building
//------------------------------------------------------------------------------
#if !defined(__Building_H_AE3C9E91_7D1D_494c_A1FD_A71CBBD62BE5__)
#define __Building_H_AE3C9E91_7D1D_494c_A1FD_A71CBBD62BE5__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class Building
// Description: Generic building
// Form name: Building
// GUID: {AE3C9E91-7D1D-494c-A1FD-A71CBBD62BE5}
//==============================================================================
class Building : public Player  
{
    DECLARE_SUBCLASS(Building,Player)

public:
    Building();

    // Player interface
    virtual unsigned int getMajorType() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__Building_H_AE3C9E91_7D1D_494c_A1FD_A71CBBD62BE5__)
