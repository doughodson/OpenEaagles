//------------------------------------------------------------------------------
// Class: Agm
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Agm_H__
#define __oe_simulation_Agm_H__

#include "openeaagles/simulation/Missile.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Classes: Agm
// Description: Base class for Air to Ground (A/G) missiles
//
//    Contains a simple aero and guidance model, which derived models can
//    override at will.
//
// Factory name: AgmMissile
//------------------------------------------------------------------------------
class Agm : public Missile
{
    DECLARE_SUBCLASS(Agm, Missile)

public:
    Agm();

    const char* getDescription() const override;  // returns "AGM"
    const char* getNickname() const override;     // returns "GenericAgm"
    int getCategory() const override;             // returns MISSILE | GUIDED

private:
    bool calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3* const los,
          osg::Vec3* const vel, osg::Vec3* const posx) const override;
};

} // End simulation namespace
}

#endif
