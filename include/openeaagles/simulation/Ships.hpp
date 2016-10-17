
#ifndef __oe_simulation_Ship_H__
#define __oe_simulation_Ship_H__

#include "openeaagles/simulation/Player.hpp"

namespace oe {
namespace simulation {

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
