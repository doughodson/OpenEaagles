
#ifndef __oe_models_Building_H__
#define __oe_models_Building_H__

#include "openeaagles/models/players/Player.hpp"

namespace oe {
namespace models {

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
