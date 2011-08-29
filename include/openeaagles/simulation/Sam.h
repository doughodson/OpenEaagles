//------------------------------------------------------------------------------
// Class: Sam
//------------------------------------------------------------------------------
#ifndef __SAM_H_65C041B7_FE92_407c_81F3_6600447AE194__
#define __SAM_H_65C041B7_FE92_407c_81F3_6600447AE194__

#include "openeaagles/simulation/Missile.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class:	Sam
// Description: Base class for Surface to Air (SAM) Missiles
// Form name: Sam
//------------------------------------------------------------------------------
class Sam : public Missile  
{
    DECLARE_SUBCLASS(Sam,Missile)

public:
    Sam();
    
    // Weapon interface
    virtual const char* getDescription() const;
    virtual const char* getNickname() const;
    virtual int getCategory() const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // !defined(__SAM_H_65C041B7_FE92_407c_81F3_6600447AE194__)
