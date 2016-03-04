#include "openeaagles/simulation/IrQueryMsg.h"

#include "openeaagles/simulation/IrSensor.h"

#include "openeaagles/base/List.h"

namespace oe {
namespace simulation {

//==============================================================================
// Class Emission
//==============================================================================
IMPLEMENT_SUBCLASS(IrQueryMsg,"IrQueryMsg")
EMPTY_SLOTTABLE(IrQueryMsg)
EMPTY_SERIALIZER(IrQueryMsg)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrQueryMsg::IrQueryMsg()
{
    STANDARD_CONSTRUCTOR()

   lowerWavelength = 0.0;
   upperWavelength = 0.0;
   instantaneousFieldOfView = 0.0;
   nei = 0.0;
   signatureByWaveband = nullptr;
   signatureAtRange = 0.0;
   emissivity = 1.0;
   projectedArea = 0.0;
   signalToNoiseRatio = 0.0;     // ratio of Signal to Noise
   backgroundNoiseRatio = 0.0;
   sendingSensor = nullptr;
   pos.set(0,0,0);
   vel.set(0,0,0);
   accel.set(0,0,0);
   angleAspect = 0.0;
   angleOffBoresight = 0.0;
   rel = 0.0;
   raz = 0.0;
   mergedQueryStatus = NOT_MERGED;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrQueryMsg::copyData(const IrQueryMsg& org, const bool cc)
{
    BaseClass::copyData(org);

   // If copy constructor, init these pointers
   if (cc) {
      signatureByWaveband = nullptr;
      sendingSensor = nullptr;
   }

//   if (org.signatureByWaveband != 0) {
//      setSignatureByWaveband((Lcreal*) org.signatureByWaveband->clone());
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
void IrQueryMsg::setRange(const LCreal r)
{
   BaseClass::setRange(r);
}

//Set the signature by waveband
void IrQueryMsg::setSignatureByWaveband(LCreal* const sig)
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

} // End simulation namespace
} // End oe namespace

