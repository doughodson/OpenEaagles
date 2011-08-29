//------------------------------------------------------------------------------
// Class: Ins
//------------------------------------------------------------------------------
#ifndef __Ins_H_AEC74A05_B5CD_4410_8C21_D111AA4B85FE__
#define __Ins_H_AEC74A05_B5CD_4410_8C21_D111AA4B85FE__


#include "openeaagles/simulation/Navigation.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Classes:     Ins
// Base class:  Basic::Object -> ... -> Navigation -> Ins
//
// Description: Generic Inerial Navigation System (INS) Data
// Form name: Ins
// GUID: {AEC74A05-B5CD-4410-8C21-D111AA4B85FE}
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
    LCreal      wander;         // Wander angle		(degs)
    LCreal      dBias;          // Doppler bias
    LCreal      dSFact;         // Doppler Scale Factor

    // Alignment data
    AlignMode   alignMode;      // Alignment mode
    double      alignTTG;       // Alignment Time to Go (sec)
    LCreal      quality;        // Quality
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __Ins_H_AEC74A05_B5CD_4410_8C21_D111AA4B85FE__
