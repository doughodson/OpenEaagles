//------------------------------------------------------------------------------
// Classes: IrSignature
//------------------------------------------------------------------------------

#include "openeaagles/simulation/IrSignature.h"
#include "openeaagles/simulation/IrShapes.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Areas.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(IrSignature,"IrSignature")
EMPTY_SERIALIZER(IrSignature)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(IrSignature)
   "binSizes",                // 1 The size of the wavelength bins used in the tables
   "irShapeSignature",        // 2 IR shape
   "baseHeatSignature",       // 3 Base heat signature in watts per steradian
   "emissivity",              // 4 Emissivity
   "effectiveArea",           // 5 Effective area
END_SLOTTABLE(IrSignature)

// Map slot table to handles
BEGIN_SLOT_MAP(IrSignature)
   ON_SLOT(1,setSlotWaveBandSizes,Basic::Table1)
   ON_SLOT(2,setSlotIrShapeSignature, IrShape)
   ON_SLOT(3,setSlotBaseHeatSignature,Basic::Number)
   ON_SLOT(4,setSlotEmissivity,Basic::Number)
   ON_SLOT(5,setSlotEffectiveArea,Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrSignature::IrSignature()
{
   STANDARD_CONSTRUCTOR()

   waveBandTable = 0;
   numWaveBands = 0;
   baseHeatSignature = 0;
   emissivity = 0;
   effectiveArea = 1e-12;    // default area to near-zero but non-zero
   irShapeSignature = 0;

   // create a default waveband, middle infrared band : 3-5 micron (100 THz - 60 THz)
   //LCreal xdata[8] = { 4.0f };
   //LCreal gdata[8] = { 2.0f };
   //waveBandTable = new Basic::Table1(gdata, 1, xdata, 1);
   //numWaveBands=1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrSignature::copyData(const IrSignature& org, const bool cc)
{
   BaseClass::copyData(org);

   if(cc){
      waveBandTable = 0;
      numWaveBands = 0;
      baseHeatSignature = 0;
      emissivity = 0;
      effectiveArea = 0;
      irShapeSignature = 0;
   }

   baseHeatSignature = org.baseHeatSignature;
   emissivity        = org.emissivity;
   effectiveArea     = org.effectiveArea;

   if (org.waveBandTable != 0) {
     setSlotWaveBandSizes( (Basic::Table1*) org.waveBandTable->clone() );
   }

   if (org.irShapeSignature != 0) {
     setSlotIrShapeSignature( (IrShape*) org.irShapeSignature->clone() );
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSignature::deleteData()
{
   if (irShapeSignature != 0)   { irShapeSignature->unref(); irShapeSignature = 0; }

   if (waveBandTable != 0) {
      waveBandTable->unref();
      waveBandTable = 0;
      numWaveBands = 0;
   }
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* IrSignature::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// setSlotBinSizes() --  set number of separate bands
//------------------------------------------------------------------------------
bool IrSignature::setSlotWaveBandSizes(const Basic::Table1* const tbl)
{
   bool ok = false;
   if (tbl != 0) {
      if (waveBandTable != 0){
          waveBandTable->unref();
      }
      tbl->ref();
      waveBandTable = tbl;
      numWaveBands = tbl->getNumXPoints();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotIrShapeSignature() --  set IR shape of the signature
//------------------------------------------------------------------------------
bool IrSignature::setSlotIrShapeSignature(IrShape* const s) {
   bool ok = false;
   if (s != 0) {
      if (irShapeSignature != 0) {
         irShapeSignature->unref();
      }
      irShapeSignature = s;
      irShapeSignature->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotBaseHeatSignature() -- set base heat signature
//------------------------------------------------------------------------------
bool IrSignature::setSlotBaseHeatSignature(Basic::Number* const num)
{
   bool ok = false;
   if (num != 0) {
      LCreal x = num->getReal();
      ok = setBaseHeatSignature(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSignature::setBaseHeatSignature: Error setting Base Heat Signature!" << std::endl;
         }
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotEmissivity() -- set emissivity
//------------------------------------------------------------------------------
bool IrSignature::setSlotEmissivity(Eaagles::Basic::Number *const num)
{
   bool ok = false;
   if (num != 0) {
      LCreal x = num->getReal();
      ok = setEmissivity(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSignature::setSlotEmissivity: Error setting Emissivity!" << std::endl;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotEffectiveArea() -- set effective area
//------------------------------------------------------------------------------
bool IrSignature::setSlotEffectiveArea(Eaagles::Basic::Number *const num)
{
   bool ok = false;
   LCreal value = 0.0f;

   const Basic::Area* a = dynamic_cast<const Basic::Area*>(num);
   if (a != 0) {
      Basic::SquareMeters sm;
      value = (LCreal)sm.convert(*a);
   } else if (num != 0) {
      value = num->getReal();
   }
   ok = setEffectiveArea(value);
   if (!ok) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "IrSignature::setSlotEffectiveArea: Error setting effective area!" << std::endl;
      }
   }
   return ok;
}

bool IrSignature::getIrSignature(IrQueryMsg* const msg)
{
   bool ok = false;
   //IrQueryMsg* msg = dynamic_cast<IrQueryMsg*>( msg0 );     // FAB - do we really need to cast away const?
   if (msg != 0) {
      LCreal projectedAreaInFOV = getSignatureArea(msg);
      msg->setProjectedArea(projectedAreaInFOV);
      // if no projectedAreaInFOV, then target was not in FOV
      if (projectedAreaInFOV > 0.0f){
         ok = true;
         // FAB - set simple signature value
         msg->setSignatureAtRange(getCalculatedHeatSignature());
         msg->setEmissivity(getEmissivity());
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSignatureArea() -- Determine target's surface area in the FOV (all or nothing)
//------------------------------------------------------------------------------
LCreal IrSignature::getSignatureArea(IrQueryMsg* msg) {
   if (irShapeSignature == 0) {
      LCreal angleOffBoresight = msg->getAngleOffBoresight();
      LCreal maxAngle = msg->getSendingSensor()->getIFOVTheta();
      if (angleOffBoresight > maxAngle) return 0;
      return getEffectiveArea();
   }
   else {
      LCreal reflectorArea = irShapeSignature->getReflectorAreaInFieldOfView(msg);
      return reflectorArea;
   }
}

//------------------------------------------------------------------------------
// getWaveBandCenters() -- Return center frequency of all wave bands
//------------------------------------------------------------------------------
const LCreal* IrSignature::getWaveBandCenters() const {
   return ((waveBandTable!=0) ? waveBandTable->getXData() : 0);
}

//------------------------------------------------------------------------------
// getWaveBandWidths() -- Return widths for all wave band frequencies
//------------------------------------------------------------------------------
const LCreal* IrSignature::getWaveBandWidths() const {
   return ((waveBandTable != 0) ? waveBandTable->getDataTable() : 0);
}

} // End Simulation namespace
} // End Eaagles namespace
