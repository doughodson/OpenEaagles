//------------------------------------------------------------------------------
// class: Ins
//------------------------------------------------------------------------------

#include "openeaagles/simulation/Ins.h"

namespace Eaagles {
namespace Simulation {


//==============================================================================
// class: Ins
//==============================================================================

IMPLEMENT_SUBCLASS(Ins,"Ins")
EMPTY_SLOTTABLE(Ins)
EMPTY_SERIALIZER(Ins)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Ins::Ins() : gyroBias(), accelBias()
{
    STANDARD_CONSTRUCTOR()

    wander      = 0.0f;
    dBias       = 0.0f;
    dSFact      = 0.0f;

    // Alignment/Kalman filter data
    alignMode   = SHDG;
    alignTTG    = 0.0;
    quality     = 0.0f;
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
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Ins::deleteData()
{
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
    if (p != 0) {
        gyroBias = *p;
    }
}

void Ins::setAccelBias(const osg::Vec3* const p)
{
    if (p != 0) {
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


} // End Simulation namespace
} // End Eaagles namespace
