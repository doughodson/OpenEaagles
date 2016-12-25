
#ifndef __oe_models_Ship_H__
#define __oe_models_Ship_H__

#include "openeaagles/models/players/Player.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Ship
// Description: Generic ship model
// Factory name: Ship
//------------------------------------------------------------------------------
class Ship : public Player
{
    DECLARE_SUBCLASS(Ship, Player)

public:
    Ship();

    virtual unsigned int getMajorType() const override;
};

}
}

#endif
