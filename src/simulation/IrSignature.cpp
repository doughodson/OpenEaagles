//------------------------------------------------------------------------------
// Class: IrSignature
//------------------------------------------------------------------------------

#include "openeaagles/simulation/IrSignature.h"
#include "openeaagles/simulation/IrShapes.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/functors/Tables.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Areas.h"

namespace oe {
namespace simulation {

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
   ON_SLOT(1,setSlotWaveBandSizes,basic::Table1)
   ON_SLOT(2,setSlotIrShapeSignature, IrShape)
   ON_SLOT(3,setSlotBaseHeatSignature,basic::Number)
   ON_SLOT(4,setSlotEmissivity,basic::Number)
   ON_SLOT(5,setSlotEffectiveArea,basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IrSignature::IrSignature()
{
   STANDARD_CONSTRUCTOR()

   waveBandTable = nullptr;
   numWaveBands = 0;
   baseHeatSignature = 0.0;
   emissivity = 0.0;
   effectiveArea = 1e-12;    // default area to near-zero but non-zero
   irShapeSignature = nullptr;

   // create a default waveband, middle infrared band : 3-5 micron (100 THz - 60 THz)
   //LCreal xdata[8] = { 4.0f };
   //LCreal gdata[8] = { 2.0f };
   //waveBandTable = new basic::Table1(gdata, 1, xdata, 1);
   //numWaveBands=1;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrSignature::copyData(const IrSignature& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      waveBandTable = nullptr;
      numWaveBands = 0;
      baseHeatSignature = 0.0;
      emissivity = 0.0;
      effectiveArea = 0;
      irShapeSignature = nullptr;
   }

   baseHeatSignature = org.baseHeatSignature;
   emissivity        = org.emissivity;
   effectiveArea     = org.effectiveArea;

   if (org.waveBandTable != nullptr) {
      basic::Table1* copy = org.waveBandTable->clone();
      setSlotWaveBandSizes( copy );
      copy->unref();
   }
   else {
      setSlotWaveBandSizes(nullptr);
   }

   if (org.irShapeSignature != nullptr) {
      IrShape* copy = org.irShapeSignature->clone();
      setSlotIrShapeSignature( copy );
      copy->unref();
   }
   else {
      setSlotIrShapeSignature(nullptr);
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSignature::deleteData()
{
   if (irShapeSignature != nullptr)   { irShapeSignature->unref(); irShapeSignature = nullptr; }

   if (waveBandTable != nullptr) {
      waveBandTable->unref();
      waveBandTable = nullptr;
      numWaveBands = 0;
   }
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
basic::Object* IrSignature::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// setSlotBinSizes() --  set number of separate bands
//------------------------------------------------------------------------------
bool IrSignature::setSlotWaveBandSizes(const basic::Table1* const tbl)
{
   if (waveBandTable != nullptr) {
      waveBandTable->unref();
      waveBandTable = nullptr;
      numWaveBands = 0;
   }
   if (tbl != nullptr) {
      waveBandTable = tbl;
      tbl->ref();
      numWaveBands = tbl->getNumXPoints();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotIrShapeSignature() --  set IR shape of the signature
//------------------------------------------------------------------------------
bool IrSignature::setSlotIrShapeSignature(IrShape* const s)
{
   if (irShapeSignature != nullptr) {
      irShapeSignature->unref();
      irShapeSignature = nullptr;
   }
   if (s != nullptr) {
      irShapeSignature = s;
      irShapeSignature->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotBaseHeatSignature() -- set base heat signature
//------------------------------------------------------------------------------
bool IrSignature::setSlotBaseHeatSignature(basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      const LCreal x = num->getReal();
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
bool IrSignature::setSlotEmissivity(oe::basic::Number* const num)
{
   bool ok = false;
   if (num != nullptr) {
      const LCreal x = num->getReal();
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
bool IrSignature::setSlotEffectiveArea(oe::basic::Number* const num)
{
   bool ok = false;
   LCreal value = 0.0;

   const basic::Area* a = dynamic_cast<const basic::Area*>(num);
   if (a != nullptr) {
      basic::SquareMeters sm;
      value = static_cast<LCreal>(sm.convert(*a));
   } else if (num != nullptr) {
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
   if (msg != nullptr) {
      LCreal projectedAreaInFOV = getSignatureArea(msg);
      msg->setProjectedArea(projectedAreaInFOV);
      // if no projectedAreaInFOV, then target was not in FOV
      if (projectedAreaInFOV > 0.0){
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
LCreal IrSignature::getSignatureArea(IrQueryMsg* msg)
{
   if (irShapeSignature == nullptr) {
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
const LCreal* IrSignature::getWaveBandCenters() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getXData() : nullptr);
}

//------------------------------------------------------------------------------
// getWaveBandWidths() -- Return widths for all wave band frequencies
//------------------------------------------------------------------------------
const LCreal* IrSignature::getWaveBandWidths() const
{
   return ((waveBandTable != nullptr) ? waveBandTable->getDataTable() : nullptr);
}

} // End simulation namespace
} // End oe namespace
