
#ifndef __oe_simulation_IrSignature_H__
#define __oe_simulation_IrSignature_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
   namespace base {
      class Number;
      class List;
      class Table1;
   }

namespace simulation {
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
class IrSignature : public base::Component
{
    DECLARE_SUBCLASS(IrSignature, base::Component)

public:

   IrSignature();

   // IrSignature class interface
   virtual bool getIrSignature(IrQueryMsg* const);
   virtual double getSignatureArea(IrQueryMsg*); 

   // slot operations 
   virtual bool setSlotWaveBandSizes(const base::Table1* const);
   virtual bool setSlotIrShapeSignature(IrShape* const);
   virtual bool setSlotBaseHeatSignature(base::Number* const);
   virtual bool setSlotEmissivity(base::Number* const);
   virtual bool setSlotEffectiveArea(base::Number* const);

protected:

   //Get the number of waveband bins
   unsigned int getNumWaveBands()                         { return numWaveBands; };

   //Get the waveband centers from each bin
   const double* getWaveBandCenters() const;

   //Get the waveband widths from each bin
   const double* getWaveBandWidths() const;

   // Return the lowest wavelength for which data for the atmosphere is required. 
   //        It is higher of the bottom sensor waveband and the lowest waveband represented by the atmosphere.
   double getLowerEndOfWavelengthOverlap(
      const double lowerRadiationWaveband, // Lower end of the wavebands represented by the atmosphere (microns)
      const double lowerSensorWaveband     // Lower end of the sensor waveband (microns)
   ) const 
   {
     return ((lowerRadiationWaveband > lowerSensorWaveband) ? lowerRadiationWaveband : lowerSensorWaveband);
   }

   // Return the highest wavelength for which data for the atmosphere is required. 
   //        It is lower of the top of the sensor waveband and the highest waveband represented by the atmosphere.
   double getUpperEndOfWavelengthOverlap(
      const double upperRadiationWaveband, // Upper end of the wavebands represented by the atmosphere (microns)
      const double upperSensorWaveband     // Upper end of the sensor waveband (microns)
   ) const
   {
     return ((upperRadiationWaveband < upperSensorWaveband) ? upperRadiationWaveband : upperSensorWaveband);
   }

   // Base heat signature (Watts per steradian)
   double getBaseHeatSignature() const                    { return baseHeatSignature; }

   //Set the Base heat signature (Watts per steradian)
   bool setBaseHeatSignature(const double x)              { baseHeatSignature = x; return true; }

   // The emissivity for the player
   double getEmissivity() const                           { return emissivity; }

   // Set the emissivity for the player
   bool setEmissivity(const double x)                     { emissivity = x; return true; } 

   // The effective area for heat calculations (m^2)
   double getEffectiveArea() const                        { return effectiveArea; }

   // Set the effective area for heat calculations (m^2)
   bool setEffectiveArea(const double x)                  { effectiveArea = x; return true; }

   double getCalculatedHeatSignature() const              { return baseHeatSignature * emissivity; }

private:

   unsigned int numWaveBands;            // Total number of wavebands
   const base::Table1* waveBandTable;   // Table of waveband centers and widths
   IrShape* irShapeSignature;            // Shape of this IR signature
   double baseHeatSignature;             // The base heat signature   (Watts per steradian)
   double emissivity;                    // The emissivity for the player, what part of incident radiation
                                         // is absorbed. The value ranges from 0.0 for total reflection to
                                         // 1.0 for total absorption.
   double effectiveArea;                 // The effective area for heat calculations  (m^2)
};

}
}

#endif
