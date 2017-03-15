
#include "openeaagles/models/navigation/Ins.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Ins, "Ins")
EMPTY_SLOTTABLE(Ins)
EMPTY_SERIALIZER(Ins)
EMPTY_DELETEDATA(Ins)

Ins::Ins() : gyroBias(), accelBias()
{
    STANDARD_CONSTRUCTOR()
}

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
void Ins::setDopplerBias(const double b)
{
   dBias = b;
}

void Ins::setDopplerScaleFactor(const double sf)
{
   dSFact = sf;
}

void Ins::setAlignmentMode(const AlignMode mode)
{
   alignMode = mode;
}

void Ins::setGyroBias(const base::Vec3d* const p)
{
    if (p != nullptr) {
        gyroBias = *p;
    }
}

void Ins::setAccelBias(const base::Vec3d* const p)
{
    if (p != nullptr) {
        accelBias = *p;
    }
}

void Ins::setWanderAngle(const double v)
{
    wander = v;
}

void Ins::setAlignmentTTG(const double v)
{
    alignTTG = v;
}

void Ins::setQuality(const double v)
{
    quality = v;
}

}
}
