#include "openeaagles/simulation/IrQueryMsg.h"

#include "openeaagles/simulation/IrSensor.h"

#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Simulation {

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

   lowerWavelength = 0.0f;
   upperWavelength = 0.0f;
   instantaneousFieldOfView = 0.0f;
   nei = 0.0f;
   signatureByWaveband = 0;
   signatureAtRange = 0.0f;
   emissivity = 1.0f;
   projectedArea = 0.0f;
   signalToNoiseRatio = 0.0f;			// ratio of Signal to Noise 
   backgroundNoiseRatio = 0.0f;	
   sendingSensor = 0; 
   pos.set(0,0,0);
   vel.set(0,0,0);
   accel.set(0,0,0);
   angleAspect = 0.0f;
   angleOffBoresight = 0.0f;
   rel = 0.0f;
   raz = 0.0f;
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
      signatureByWaveband = 0;
      sendingSensor = 0; 
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
   if (signatureByWaveband != 0)  delete signatureByWaveband;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Clear the IR signature data
void IrQueryMsg::clearIrSignature()
{
   setSignatureByWaveband(0);
}

// Sets the range to the target
void IrQueryMsg::setRange(const LCreal r)
{
   BaseClass::setRange(r);
}

//Set the signature by waveband
void IrQueryMsg::setSignatureByWaveband (LCreal* const sig)
{
//   if (signatureByWaveband != 0) signatureByWaveband->unref();
   signatureByWaveband = sig;
//   if (signatureByWaveband != 0) signatureByWaveband->ref();
}

void IrQueryMsg::setSendingSensor(IrSensor* const newSensor)
{
   if (sendingSensor != 0) sendingSensor->unref();
   sendingSensor = newSensor;
   if (sendingSensor != 0) sendingSensor->ref();
}

//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void IrQueryMsg::clear()
{
   setSignatureByWaveband(0);
   setSendingSensor(0);

   BaseClass::clear();
}

} // End Simulation namespace
} // End Eaagles namespace

