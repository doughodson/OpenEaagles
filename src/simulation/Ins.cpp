//------------------------------------------------------------------------------
// class: Ins
//------------------------------------------------------------------------------

#include "openeaagles/simulation/Ins.h"

namespace oe {
namespace simulation {


//==============================================================================
// class: Ins
//==============================================================================

IMPLEMENT_SUBCLASS(Ins,"Ins")
EMPTY_SLOTTABLE(Ins)
EMPTY_SERIALIZER(Ins)
EMPTY_DELETEDATA(Ins)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Ins::Ins() : gyroBias(), accelBias()
{
    STANDARD_CONSTRUCTOR()

    wander      = 0.0;
    dBias       = 0.0;
    dSFact      = 0.0;

    // Alignment/Kalman filter data
    alignMode   = SHDG;
    alignTTG    = 0.0;
    quality     = 0.0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Ins::copyData(const Ins& org, const bool)
{
    BaseClass::copyData(org);

    gyroBias    = org.gyroBias;
    accelBias   = org.accelBias;
    wander      = org.wander;
    dBias       = org.dBias;
    dSFact      = org.dSFact;

    alignMode   = org.alignMode;
    alignTTG    = org.alignTTG;
    quality     = org.quality;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Ins::setDopplerBias(const LCreal b)
{
   dBias = b;
}

void Ins::setDopplerScaleFactor(const LCreal sf)
{
   dSFact = sf;
}

void Ins::setAlignmentMode(const AlignMode mode)
{
   alignMode = mode;
}

void Ins::setGyroBias(const osg::Vec3* const p)
{
    if (p != nullptr) {
        gyroBias = *p;
    }
}

void Ins::setAccelBias(const osg::Vec3* const p)
{
    if (p != nullptr) {
        accelBias = *p;
    }
}

void Ins::setWanderAngle(const LCreal v)
{
    wander = v;
}

void Ins::setAlignmentTTG(const double v)
{
    alignTTG = v;
}

void Ins::setQuality(const LCreal v)
{
    quality = v;
}


} // End simulation namespace
} // End oe namespace
