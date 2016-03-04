//------------------------------------------------------------------------------
// Class: IrAtmosphere
//------------------------------------------------------------------------------
#ifndef __oe_simulation_IrAtmosphere_H__
#define __oe_simulation_IrAtmosphere_H__

#include "openeaagles/base/Component.h"

namespace oe {

   namespace basic {
      class Number;
      class Table1;
      class Table2;
      class Table3;
      class Table4;
      class List;
      class Number;
   }

namespace simulation {

class IrQueryMsg;

//------------------------------------------------------------------------------
// Class:   simulation::IrAtmosphere
//
// Description: Manages the atmospheric data to determine transmissivity
//              and background radiation for specific infrared wavelengths
//
// Factory name: IrAtmosphere
// Slots:
//    waveBands                  <Table1>      ! A list The size of the wavelength bins used in the tables
//    transmissivityTable        <Table1>      ! The table containing transmissivity data
//    skyRadiance                <Number>      ! Simple background radiance for targets in sky (watts/sr-m^2)
//    earthRadiance              <Number>      ! Simple background radiance for targets on ground (watts/sr-m^2)
//
// Public Member Functions:
//
//     LCreal getTransmissivity(const LCreal lowerWavelength,   // The lower range of the wave band (microns)
//                              const LCreal upperWavelength,   // The upper range of the wave band (microns)
//                              const LCreal range)             // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region
//        of the spectrum defined by the upper and lower wavelengths as a function 
//        of the range from seeker to target. The return value is between 
//        0.0 (no power gets through) and 1.0 (All power gets through)
//
//    LCreal getTransmissivity(const LCreal wavebandCenter,       // The waveband center (microns)
//                             const LCreal range)                // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region surrounded 
//        by the center of the waveband as a function of the range from seeker to target.
//        The routine does a table lookup in a 3-D table where the x-coordinate is the center of the
//        user defined waveband, the y-coordinate is the range of the seeker. The return value is between 0.0 
//        (no power gets through) and 1.0 (All power gets through)
//
//     LCreal getLowerEndOfWavelengthOverlap(const LCreal lowerRadiationWaveband,      // Lower end of the wavebands represented by the atmosphere (microns)
//                                           const LCreal lowerSensorWaveband) const;  // Lower end of the sensor waveband (microns)
//        Return the lowest wavelength for which data for the atmosphere is required. It is higher of the bottom 
//        sensor waveband and the lowest waveband represented by the atmosphere.
//
//     LCreal getUpperEndOfWavelengthOverlap(const LCreal upperRadiationWaveband,      // Upper end of the wavebands represented by the atmosphere (microns)
//                                           const LCreal upperSensorWaveband) const;  // Upper end of the sensor waveband (microns)
//        Return the highest wavelength for which data for the atmosphere is required. It is lower of the top 
//        of the sensor waveband and the highest waveband represented by the atmosphere.
//
//
// Notes:
//    1) The first index of each table represents the center frequency of the bins
//    2) Transmissivity table must use same waveband centers as defined for waveBandTable.
//    3) Values in the table are coefficients of absorption (per km), so transmissivity is calculated as
//       trans = exp (-absorption * range * .001 km/m)
//
// Example Input:
//
//   ( IrAtmosphere
//
//      waveBands: ( Table1
//            x: [ 3.200 3.775 4.325 4.750]    // 4 wavebands with center frequencies at 3.200, 3.775, 4.325 and 4.750
//         data: [ 0.4   0.75  0.35  0.5 ]     // 4 bin widths -  so bands are (3.00-3.40) (3.40-4.15) (4.15-4.50) (4.50-5.00)
//      )
//      transmissivityTable1: ( Table1
//            x: [ 3.200 3.775 4.325 4.750]    // waveBandCenters
//         data: [ 0.011 0.004 0.28  0.017 ]   // transmissivities (coefficient of (absorption+scattering), per km)
//      )
//      skyRadiance:  11.2                     // 11.2 watts/sr-m^2
//      earthRadiance: 1.0                     //  1.0 watts/sr-m^2
//   )
//
//------------------------------------------------------------------------------

class IrAtmosphere : public basic::Component
{
   DECLARE_SUBCLASS(IrAtmosphere,basic::Component)

public:
   IrAtmosphere();

   // IrAtmosphere class interface
   virtual bool calculateAtmosphereContribution(IrQueryMsg* const msg, LCreal* totalSignal, LCreal* totalBackground);

   //Get the number of waveband bins
   unsigned int getNumWaveBands() const              { return numWaveBands; }

   // getWaveBandCenters() -- Return center frequency of all wave bands
   const LCreal* getWaveBandCenters() const;

   // getWaveBandWidths() -- Return widths for all wave band frequencies
   const LCreal* getWaveBandWidths() const;

   // Return the lowest wavelength for which data for the atmosphere is required. 
   // It is higher of the bottom sensor waveband and the lowest waveband represented by the atmosphere.
   LCreal getLowerEndOfWavelengthOverlap (
      const LCreal lowerRadiationWaveband, // Lower end of the wavebands represented by the atmosphere (microns)
      const LCreal lowerSensorWaveband     // Lower end of the sensor waveband (microns)
   ) const 
   {
      return ((lowerRadiationWaveband > lowerSensorWaveband) ? lowerRadiationWaveband : lowerSensorWaveband);
   }

   // Return the highest wavelength for which data for the atmosphere is required. 
   // It is lower of the top of the sensor waveband and the highest waveband represented by the atmosphere.
   LCreal getUpperEndOfWavelengthOverlap(
      const LCreal upperRadiationWaveband, // Upper end of the wavebands represented by the atmosphere (microns)
      const LCreal upperSensorWaveband     // Upper end of the sensor waveband (microns)
   ) const
   {
      return ((upperRadiationWaveband < upperSensorWaveband) ? upperRadiationWaveband : upperSensorWaveband);
   }

protected:

   // slot operations 
   virtual bool setSlotWaveBands(const basic::Table1* const tbl);
   virtual bool setSlotTransmissivityTable1(const basic::Table1* const tbl);
   virtual bool setSlotSkyRadiance(basic::Number* const num);
   virtual bool setSlotEarthRadiance(basic::Number* const num);

   LCreal getTransmissivity(const unsigned int i, const LCreal range) const;
   LCreal getTransmissivity(const LCreal waveBandCenter, const LCreal range) const;
   LCreal getSkyRadiance() const {return skyRadiance;}
   LCreal getEarthRadiance() const {return earthRadiance;}

private:

   unsigned int numWaveBands;
   const basic::Table1* waveBandTable;
   const basic::Table1* transmissivityTable1;

   LCreal skyRadiance;      // Simple background radiance for targets in sky
   LCreal earthRadiance;    // Simple background radiance for targets on ground
};

} // End simulation namespace
} // End oe namespace

#endif
