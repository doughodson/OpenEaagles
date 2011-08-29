//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship models
//------------------------------------------------------------------------------
#if !defined(__SHIP_H_818CF7F5_AF6D_4d24_AEBB_299FBFB67036__)
#define __SHIP_H_818CF7F5_AF6D_4d24_AEBB_299FBFB67036__

#include "openeaagles/simulation/Player.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class Ship
// Description: Generic ship model
// Form name: Ship
// GUID: {818CF7F5-AF6D-4d24-AEBB-299FBFB67036}
//==============================================================================
class Ship : public Player  
{
    DECLARE_SUBCLASS(Ship,Player)

public:
    Ship();

    // Player interface
    virtual unsigned int getMajorType() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__SHIP_H_818CF7F5_AF6D_4d24_AEBB_299FBFB67036__)
