
#ifndef __oe_models_Agm_H__
#define __oe_models_Agm_H__

#include "openeaagles/models/players/Missile.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Agm
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

    virtual const char* getDescription() const override;  // returns "AGM"
    virtual const char* getNickname() const override;     // returns "GenericAgm"
    virtual int getCategory() const override;             // returns MISSILE | GUIDED

private:
   virtual bool calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3d* const los,
          osg::Vec3d* const vel, osg::Vec3d* const posx) const override;
};

}
}

#endif
