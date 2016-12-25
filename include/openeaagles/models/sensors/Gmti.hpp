#ifndef __oe_models_Gmti_H__
#define __oe_models_Gmti_H__

#include "openeaagles/models/systems/Radar.hpp"
#include "openeaagles/base/osg/Vec3d"

namespace oe {

namespace base { class List; }

namespace models {

//------------------------------------------------------------------------------
// Class: Gmti
// Description: Very simple, Ground-Moving-Target-Indication (GMTI) mode radar
//------------------------------------------------------------------------------
class Gmti : public Radar
{
    DECLARE_SUBCLASS(Gmti, Radar)

public:
    Gmti();

    const osg::Vec3d& getPoi() const                                { return poiVec; }
    void setPoi(const double x, const double y, const double z);
    void setPoi(const osg::Vec3d& newPos);

protected:
    bool setSlotPoi(base::List* const numList);                // Set Point of Interest

    virtual void dynamics(const double dt) override;

private:
    osg::Vec3d poiVec;    // Point Of Interest vector  (m) [ x, y, z ] NED
};

}
}

#endif
