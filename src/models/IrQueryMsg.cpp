
#include "openeaagles/models/IrQueryMsg.hpp"

#include "openeaagles/models/systems/IrSensor.hpp"

#include "openeaagles/base/List.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(IrQueryMsg, "IrQueryMsg")
EMPTY_SLOTTABLE(IrQueryMsg)
EMPTY_SERIALIZER(IrQueryMsg)

IrQueryMsg::IrQueryMsg()
{
   STANDARD_CONSTRUCTOR()
}

void IrQueryMsg::copyData(const IrQueryMsg& org, const bool)
{
    BaseClass::copyData(org);

//   if (org.signatureByWaveband != 0) {
//      setSignatureByWaveband((double*) org.signatureByWaveband->clone());
//   }
//   else setSignatureByWaveband(0);

   // Signature by waveband not copied

   lowerWavelength = org.lowerWavelength;
   upperWavelength = org.upperWavelength;
   instantaneousFieldOfView = org.instantaneousFieldOfView;
   nei = org.nei;
   signatureAtRange = org.signatureAtRange;
   emissivity = org.emissivity;
   projectedArea = org.projectedArea;
   signalToNoiseRatio = org.signalToNoiseRatio;
   backgroundNoiseRatio = org.backgroundNoiseRatio;

   setSendingSensor(org.sendingSensor);

   pos = org.pos;
   vel = org.vel;
   accel = org.accel;
   angleAspect = org.angleAspect;
   angleOffBoresight = org.angleOffBoresight;
   rel = org.rel;
   raz = org.raz;
   mergedQueryStatus = org.mergedQueryStatus;
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrQueryMsg::deleteData()
{
   clear();
   if (signatureByWaveband != nullptr)  delete signatureByWaveband;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Clear the IR signature data
void IrQueryMsg::clearIrSignature()
{
   setSignatureByWaveband(nullptr);
}

// Sets the range to the target
void IrQueryMsg::setRange(const double r)
{
   BaseClass::setRange(r);
}

//Set the signature by waveband
void IrQueryMsg::setSignatureByWaveband(double* const sig)
{
//   if (signatureByWaveband != 0) signatureByWaveband->unref();
   signatureByWaveband = sig;
//   if (signatureByWaveband != 0) signatureByWaveband->ref();
}

void IrQueryMsg::setSendingSensor(IrSensor* const newSensor)
{
   if (sendingSensor != nullptr) sendingSensor->unref();
   sendingSensor = newSensor;
   if (sendingSensor != nullptr) sendingSensor->ref();
}

//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void IrQueryMsg::clear()
{
   setSignatureByWaveband(nullptr);
   setSendingSensor(nullptr);

   BaseClass::clear();
}

}
}

