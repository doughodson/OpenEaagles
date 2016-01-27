//------------------------------------------------------------------------------
// Class: Gmti
// Description: Very simple, Ground-Moving-Target-Indication (GMTI) mode radar
//------------------------------------------------------------------------------
#ifndef __oe_sensor_Gmti_H__
#define __oe_sensor_Gmti_H__

#include "openeaagles/simulation/Radar.h"

namespace oe {

namespace basic { class List; }

namespace sensor {

class Gmti : public Simulation::Radar
{
    DECLARE_SUBCLASS(Gmti,Simulation::Radar)

public:
    Gmti();

    const osg::Vec3& getPoi() const                                { return poiVec; }
    void setPoi(const LCreal x, const LCreal y, const LCreal z);
    void setPoi(const osg::Vec3& newPos);

protected:
    bool setSlotPoi(basic::List* const numList);                // Set Point of Interest

    void dynamics(const LCreal dt) override;

private:
    osg::Vec3 poiVec;    // Point Of Interest vector  (m) [ x, y, z ] NED
};

} // End sensor namespace
} // End oe namespace

#endif
