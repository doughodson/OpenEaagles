
#ifndef __oe_models_IrAtmosphere_H__
#define __oe_models_IrAtmosphere_H__

#include "openeaagles/models/environment/AbstractAtmosphere.hpp"

namespace oe {
namespace base { class Number; class Table1; class Table2; class Table3;
                 class Table4; class Number; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrAtmosphere
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
//     double getTransmissivity(const double lowerWavelength,   // The lower range of the wave band (microns)
//                              const double upperWavelength,   // The upper range of the wave band (microns)
//                              const double range)             // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region
//        of the spectrum defined by the upper and lower wavelengths as a function 
//        of the range from seeker to target. The return value is between 
//        0.0 (no power gets through) and 1.0 (All power gets through)
//
//    double getTransmissivity(const double wavebandCenter,       // The waveband center (microns)
//                             const double range)                // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region surrounded 
//        by the center of the waveband as a function of the range from seeker to target.
//        The routine does a table lookup in a 3-D table where the x-coordinate is the center of the
//        user defined waveband, the y-coordinate is the range of the seeker. The return value is between 0.0 
//        (no power gets through) and 1.0 (All power gets through)
//
//     double getLowerEndOfWavelengthOverlap(const double lowerRadiationWaveband,      // Lower end of the wavebands represented by the atmosphere (microns)
//                                           const double lowerSensorWaveband) const;  // Lower end of the sensor waveband (microns)
//        Return the lowest wavelength for which data for the atmosphere is required. It is higher of the bottom 
//        sensor waveband and the lowest waveband represented by the atmosphere.
//
//     double getUpperEndOfWavelengthOverlap(const double upperRadiationWaveband,      // Upper end of the wavebands represented by the atmosphere (microns)
//                                           const double upperSensorWaveband) const;  // Upper end of the sensor waveband (microns)
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
class IrAtmosphere : public AbstractAtmosphere
{
   DECLARE_SUBCLASS(IrAtmosphere, AbstractAtmosphere)

public:
   IrAtmosphere();

   // IrAtmosphere class interface
   virtual bool calculateAtmosphereContribution(IrQueryMsg* const msg, double* totalSignal, double* totalBackground);

   //Get the number of waveband bins
   unsigned int getNumWaveBands() const              { return numWaveBands; }

   // getWaveBandCenters() -- Return center frequency of all wave bands
   const double* getWaveBandCenters() const;

   // getWaveBandWidths() -- Return widths for all wave band frequencies
   const double* getWaveBandWidths() const;

   // Return the lowest wavelength for which data for the atmosphere is required. 
   // It is higher of the bottom sensor waveband and the lowest waveband represented by the atmosphere.
   double getLowerEndOfWavelengthOverlap (
      const double lowerRadiationWaveband, // Lower end of the wavebands represented by the atmosphere (microns)
      const double lowerSensorWaveband     // Lower end of the sensor waveband (microns)
   ) const 
   {
      return ((lowerRadiationWaveband > lowerSensorWaveband) ? lowerRadiationWaveband : lowerSensorWaveband);
   }

   // Return the highest wavelength for which data for the atmosphere is required. 
   // It is lower of the top of the sensor waveband and the highest waveband represented by the atmosphere.
   double getUpperEndOfWavelengthOverlap(
      const double upperRadiationWaveband, // Upper end of the wavebands represented by the atmosphere (microns)
      const double upperSensorWaveband     // Upper end of the sensor waveband (microns)
   ) const
   {
      return ((upperRadiationWaveband < upperSensorWaveband) ? upperRadiationWaveband : upperSensorWaveband);
   }

protected:

   // slot operations 
   virtual bool setSlotWaveBands(const base::Table1* const tbl);
   virtual bool setSlotTransmissivityTable1(const base::Table1* const tbl);
   virtual bool setSlotSkyRadiance(base::Number* const num);
   virtual bool setSlotEarthRadiance(base::Number* const num);

   double getTransmissivity(const unsigned int i, const double range) const;
   double getTransmissivity(const double waveBandCenter, const double range) const;
   double getSkyRadiance() const {return skyRadiance;}
   double getEarthRadiance() const {return earthRadiance;}

private:

   unsigned int numWaveBands {};
   const base::Table1* waveBandTable {};
   const base::Table1* transmissivityTable1 {};

   double skyRadiance {};      // Simple background radiance for targets in sky
   double earthRadiance {};    // Simple background radiance for targets on ground
};

}
}

#endif
