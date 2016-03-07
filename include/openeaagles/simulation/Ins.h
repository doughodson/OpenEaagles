//------------------------------------------------------------------------------
// Class: Ins
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Ins_H__
#define __oe_simulation_Ins_H__

#include "openeaagles/simulation/Navigation.h"

namespace oe {
namespace simulation {

//------------------------------------------------------------------------------
// Classes: Ins
// Description: Generic Inerial Navigation System (INS) Data
// Factory name: Ins
//------------------------------------------------------------------------------
class Ins : public Navigation
{
    DECLARE_SUBCLASS(Ins,Navigation)

public:
    // Alignment modes
    enum AlignMode { PREC = 1, SHDG = 2, AIR = 3, ATT = 4 };

public:
    Ins();

    const osg::Vec3& getGyroBias() const    { return gyroBias; }
    const osg::Vec3& getAccelBias() const   { return accelBias; }
    double getWanderAngle() const           { return wander; }
    double getDopplerBias() const           { return dBias; }
    double getDopplerScaleFactor() const    { return dSFact; }
    AlignMode getAlignmentMode() const      { return alignMode; }
    double getAlignmentTTG() const          { return alignTTG; }
    double getQuality() const               { return quality; }

    virtual void setDopplerBias(const double dBias);
    virtual void setDopplerScaleFactor(const double dSFact);
    virtual void setAlignmentMode(const AlignMode mode);

protected:
    virtual void setGyroBias(const osg::Vec3* const p);
    virtual void setAccelBias(const osg::Vec3* const p);
    virtual void setWanderAngle(const double v);
    virtual void setAlignmentTTG(const double v);
    virtual void setQuality(const double v);

private:
    osg::Vec3   gyroBias;       // Gyro Bias
    osg::Vec3   accelBias;      // Acceleration Bias
    double      wander;         // Wander angle (degs)
    double      dBias;          // Doppler bias
    double      dSFact;         // Doppler Scale Factor

    // Alignment data
    AlignMode   alignMode;      // Alignment mode
    double      alignTTG;       // Alignment Time to Go (sec)
    double      quality;        // Quality
};

} // End simulation namespace
} // End oe namespace

#endif
