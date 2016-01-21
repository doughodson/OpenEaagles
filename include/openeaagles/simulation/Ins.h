//------------------------------------------------------------------------------
// Class: Ins
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Ins_H__
#define __oe_Simulation_Ins_H__

#include "openeaagles/simulation/Navigation.h"

namespace oe {
namespace Simulation {

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
    LCreal getWanderAngle() const           { return wander; }
    LCreal getDopplerBias() const           { return dBias; }
    LCreal getDopplerScaleFactor() const    { return dSFact; }
    AlignMode getAlignmentMode() const      { return alignMode; }
    double getAlignmentTTG() const          { return alignTTG; }
    LCreal getQuality() const               { return quality; }

    virtual void setDopplerBias(const LCreal dBias);
    virtual void setDopplerScaleFactor(const LCreal dSFact);
    virtual void setAlignmentMode(const AlignMode mode);

protected:
    virtual void setGyroBias(const osg::Vec3* const p);
    virtual void setAccelBias(const osg::Vec3* const p);
    virtual void setWanderAngle(const LCreal v);
    virtual void setAlignmentTTG(const double v);
    virtual void setQuality(const LCreal v);

private:
    osg::Vec3   gyroBias;       // Gyro Bias
    osg::Vec3   accelBias;      // Acceleration Bias
    LCreal      wander;         // Wander angle (degs)
    LCreal      dBias;          // Doppler bias
    LCreal      dSFact;         // Doppler Scale Factor

    // Alignment data
    AlignMode   alignMode;      // Alignment mode
    double      alignTTG;       // Alignment Time to Go (sec)
    LCreal      quality;        // Quality
};

} // End Simulation namespace
} // End oe namespace

#endif
