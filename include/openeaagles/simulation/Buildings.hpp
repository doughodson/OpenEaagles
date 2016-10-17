
#ifndef __oe_simulation_Building_H__
#define __oe_simulation_Building_H__

#include "openeaagles/simulation/Player.hpp"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
// Factory name: Building
//------------------------------------------------------------------------------
class Building : public Player
{
    DECLARE_SUBCLASS(Building, Player)

public:
    Building();

    virtual unsigned int getMajorType() const override;
};

}
}

#endif
