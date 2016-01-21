//------------------------------------------------------------------------------
// Class: IrSignature
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_IrSignature_H__
#define __oe_Simulation_IrSignature_H__

#include "openeaagles/basic/Component.h"

namespace oe {
   namespace Basic {
      class Number;
      class List;
      class Table1;
   }

namespace Simulation {
   class IrQueryMsg;
   class IrShape;

//------------------------------------------------------------------------------
// Class: IrSignature
//
// Descriptions: Base class for IR signatures
//
// Factory name: IrSignature
// Slots:
//    binSizes            <Table1>      ! The size of the wavelength bins used in the tables
//    irShapeSignature    <IrShape>     ! Shape of radiation source
//    baseHeatSignature   <Number>      ! Base heat signature - units: watts per steradian
//    emissivity          <Number>      ! Emissivity - unitless, values range from
//                                      !     0.0 (total reflection) to 1.0 (total absorption)
//    effectiveArea       <Number>      ! Effective area - units: meters squared (m^2)
//
// Notes:
//    1) Simple IR signature class, specifies player-specific properties related to IR modeling.
//    2) If no signature area provided, signature is simply baseHeatSignature*emissivity/radius^2
//    3) An irAtmosphere class is not required
//    4) If signature area is specified and if irAtmosphere used can calculate background, 
//       then backgroundBlockedByTarget can be calculated (by irAtmosphere)
//    5) Signature area can be specified either by effectiveArea or by irShapeSignature,
//       where irShapeSignature is used if both are provided
//    6) This specific class really only supports a single band, since it provides no logic for calculating
//       different signature contributions for different bands
//    7) This simple signature class is compatible with a multi-band atmosphere-
//       if a multi-band atmosphere is specified, that atmosphere class should distribute
//       signature through bands
//
// Example Input:
//
//   ( IrSignature
//   
//      irShapeSignature:
//         ( IrBox
//            x: (Meters 1.0)        // dimensions of source shape
//            y: (Meters 1.0)
//            z: (Meters 1.0)
//         )
//      baseHeatSignature: 320.0
//      emissivity: 0.75
//      effectiveArea: (SquareMeters 1.0)
//   )
//
//------------------------------------------------------------------------------
class IrSignature : public Basic::Component
{
    DECLARE_SUBCLASS(IrSignature, Basic::Component)

public:

   IrSignature();

   // IrSignature class interface
   virtual bool getIrSignature(IrQueryMsg* const);
   virtual LCreal getSignatureArea(IrQueryMsg*); 

   // slot operations 
   virtual bool setSlotWaveBandSizes(const Basic::Table1* const);
   virtual bool setSlotIrShapeSignature(IrShape* const);
   virtual bool setSlotBaseHeatSignature(Basic::Number* const);
   virtual bool setSlotEmissivity(Basic::Number* const);
   virtual bool setSlotEffectiveArea(Basic::Number* const);

protected:

   //Get the number of waveband bins
   unsigned int getNumWaveBands()                         { return numWaveBands; };

   //Get the waveband centers from each bin
   const LCreal* getWaveBandCenters() const;

   //Get the waveband widths from each bin
   const LCreal* getWaveBandWidths() const;

   // Return the lowest wavelength for which data for the atmosphere is required. 
   //        It is higher of the bottom sensor waveband and the lowest waveband represented by the atmosphere.
   LCreal getLowerEndOfWavelengthOverlap(
      const LCreal lowerRadiationWaveband, // Lower end of the wavebands represented by the atmosphere (microns)
      const LCreal lowerSensorWaveband     // Lower end of the sensor waveband (microns)
   ) const 
   {
     return ((lowerRadiationWaveband > lowerSensorWaveband) ? lowerRadiationWaveband : lowerSensorWaveband);
   }

   // Return the highest wavelength for which data for the atmosphere is required. 
   //        It is lower of the top of the sensor waveband and the highest waveband represented by the atmosphere.
   LCreal getUpperEndOfWavelengthOverlap(
      const LCreal upperRadiationWaveband, // Upper end of the wavebands represented by the atmosphere (microns)
      const LCreal upperSensorWaveband     // Upper end of the sensor waveband (microns)
   ) const
   {
     return ((upperRadiationWaveband < upperSensorWaveband) ? upperRadiationWaveband : upperSensorWaveband);
   }

   // Base heat signature (Watts per steradian)
   LCreal getBaseHeatSignature() const                    { return baseHeatSignature; }

   //Set the Base heat signature (Watts per steradian)
   bool setBaseHeatSignature(const LCreal x)              { baseHeatSignature = x; return true; }

   // The emissivity for the player
   LCreal getEmissivity() const                           { return emissivity; }

   // Set the emissivity for the player
   bool setEmissivity(const LCreal x)                     { emissivity = x; return true; } 

   // The effective area for heat calculations (m^2)
   LCreal getEffectiveArea() const                        { return effectiveArea; }

   // Set the effective area for heat calculations (m^2)
   bool setEffectiveArea(const LCreal x)                  { effectiveArea = x; return true; }

   LCreal getCalculatedHeatSignature() const              { return baseHeatSignature * emissivity; }

private:

   unsigned int numWaveBands;            // Total number of wavebands
   const Basic::Table1* waveBandTable;   // Table of waveband centers and widths
   IrShape* irShapeSignature;            // Shape of this IR signature
   LCreal baseHeatSignature;             // The base heat signature   (Watts per steradian)
   LCreal emissivity;                    // The emissivity for the player, what part of incident radiation
                                         // is absorbed. The value ranges from 0.0 for total reflection to
                                         // 1.0 for total absorption.
   LCreal effectiveArea;                 // The effective area for heat calculations  (m^2)
};

} // End Simulation namespace
} // End oe namespace

#endif
