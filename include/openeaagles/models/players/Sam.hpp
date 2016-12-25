
#ifndef __oe_models_Sam_H__
#define __oe_models_Sam_H__

#include "openeaagles/models/players/Missile.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Sam
//
// Description: Base class for Surface to Air (SAM) Missiles
//
// Factory name: Sam
//------------------------------------------------------------------------------
class Sam : public Missile
{
    DECLARE_SUBCLASS(Sam, Missile)

public:
    Sam();

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

}
}

#endif
