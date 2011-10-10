//------------------------------------------------------------------------------
// Class: Agm
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Agm_H__
#define __Eaagles_Simulation_Agm_H__

#include "openeaagles/simulation/Missile.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Classes:	Agm
// Description: Base class for Air to Ground (A/G) missiles
//
//    Contains a simple aero and guidance model, which derived models can
//    override at will.
//
// Form name: AgmMissile
//------------------------------------------------------------------------------
class Agm : public Missile  
{
    DECLARE_SUBCLASS(Agm,Missile)

public:
    Agm();
    
    // Weapon interface
    virtual const char* getDescription() const;  // returns "AGM"
    virtual const char* getNickname() const;     // returns "GenericAgm"
    virtual int getCategory() const;             // returns MISSILE | GUIDED

private:
    // Missile interface
    virtual bool calculateVectors(const Player* const tgt, const Track* const trk, osg::Vec3* const los, osg::Vec3* const vel, osg::Vec3* const posx) const;
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
