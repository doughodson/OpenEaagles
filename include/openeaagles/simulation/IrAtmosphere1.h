//------------------------------------------------------------------------------
// Class: IrAtmosphere1
//------------------------------------------------------------------------------
#ifndef __oe_simulation_IrAtmosphere1_H__
#define __oe_simulation_IrAtmosphere1_H__

#include "openeaagles/base/Component.h"

#include "openeaagles/simulation/IrAtmosphere.h"

namespace oe {

   namespace base {
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
// Class: IrAtmosphere1
//
// Description: Class for managing the atmospheric data for determining transmissivity
//              and background radiation for infrared wavelengths
//
// Factory name: IrAtmosphere1
// Slots:
//    solarRadiationTable        <Table2>       The table containing solar radiation tables
//    backgroundRadiationTable   <Table3>       The background radiation table
//    transmissivityTable        <Table4>       The table containing transmissivity data
//
// Public Member Functions:
//
//     double getTransmissivity(const double lowerWavelength, // The lower range of the wave band (microns)
//                              const double upperWavelength, // The upper range of the wave band (microns)
//                              const double seekerAltitude,  // The altitude of the seeker (meters)
//                              const double targetAltitude,  // Altitude of the target (meters)
//                              const double range)           // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region
//        of the spectrum defined by the upper and lower wavelengths as a function
//        of the positions of the seeker and the target. The return value is between
//        0.0 (no power gets through) and 1.0 (All power gets through)

//    double getTransmissivity(const double wavebandCenter,       // The waveband center (microns)
//                             const double seekerAltitude,       // The altitude of the seeker (meters)
//                             const double targetAltitude,       // Altitude of the target (meters)
//                             const double range)                // Ground range to the target (meters)
//        Return the fraction of infrared radiation transmitted in the region surrounded
//        by the center of the waveband as a function of the positions of the seeker and the target.
//        The routine does a table
//        lookup in a 3-D table where the x-coordinate is the center of the user defined waveband, the
//        y-coordinate is the altitude of the seeker, the z-coordinate is the altitude of the target,
//        and the w-coordinate is the ground range to the target. The return value is between 0.0
//        (no power gets through) and 1.0 (All power gets through)

//    double getSolarRadiation(const double lowerWavelength,     // Lower wavelength of the wave band (microns)
//                              const double upperWavelength,     // Upper wavelength of the wave band (microns)
//                              const double targetAltitude)      // Altitude of the target (meters)
//        Return the amount of solar radiation in the region of the spectrum defined by the
//        upper and lower wavelengths as a function of the target altitude. The output is in
//        the units watts/steradian sq-m.
//
//    double getSolarRadiation(const double wavebandCenter,       // The waveband center (microns)
//                             const double targetAltitude)       // The altitude of the target (meters)
//        Return the amount of solar radiation in the region surrounded by the center of the
//        waveband as a function of the target altitude. The routine does a table lookup in a 2-D
//        table where the x-coordinate is the center of the user defined waveband of interest and the
//        y-coordinate is the altitude of the target. The unit of output is Watts/steradian sq-m.

//     double getBackgroundRadiation(const double lowerWavelength,  // Lower wavelength of the wave band (microns)
//                                 const double upperWavelength,    // Upper wavelength of the wave band (microns)
//                                 const double seekerAltitude,     // The altitude of the seeker (meters)
//                                 const double viewAngle)          // The view angle (radians)
//        Return the amount of background radiation in the region of the spectrum defined by the
//        upper and lower wavelengths as a function the altitude of the seeker and angle of viewing.
//        The unit of output is Watts/steradian sq-m.

//     double getBackgroundRadiation(const double wavebandCenter,       // The waveband center (microns)
//                                   const double seekerAltitude,       // The altitude of the seeker (meters)
//                                   const double viewAngle)            // The view angle (radians)
//        Return the amount of background radiation in the region surrounded by the center of the
//        waveband as a function the altitude of the seeker and angle of viewing. The routine does a table
//        lookup in a 3-D table where the x-coordinate is the center of the user defined waveband of interest,
//        the y-coordinate is the altitude of the seeker, and the z-coordinate is the viewing angle.
//        The unit of output is Watts/steradian sq-m.
//
//     virtual void IrAtmosphere::getSolarRadiationSignatures(double* signatureArray,  // Container for the solar radiation data
//                                                            const double lowerBound, // Lower bound (microns) of interest
//                                                            const double upperBound, // Upper bound (microns) of interest
//                                                            const double altitude)   // Altitude of interest (meters)
//        Returns an array which holds, for each defined bin, the upper lower and wavelength and the
//        amount of solar radiation in the bin. The array is of size 3 * numBins.
//
// Notes:
//    1) The first index of each table represents the center frequency of the bins
//------------------------------------------------------------------------------

class IrAtmosphere1 : public IrAtmosphere
{
   DECLARE_SUBCLASS(IrAtmosphere1, IrAtmosphere)

public:
   IrAtmosphere1();
   virtual bool calculateAtmosphereContribution(IrQueryMsg* const msg, double* totalSignal, double* totalBackground) override;

protected:

   double getTransmissivity(
      const double lowerWavelength,      // The lower wavelength (microns)
      const double upperWavelength,      // The upper wavelength (microns)
      const double seekerAltitude,       // The altitude of the seeker (meters)
      const double targetAltitude,       // Altitude of the target (meters)
      const double range                 // Ground range to the target (meters)
   ) const;

   double getTransmissivity(
      const double wavebandCenter,       // The waveband center (microns)
      const double seekerAltitude,       // The altitude of the seeker (meters)
      const double targetAltitude,       // Altitude of the target (meters)
      const double range                 // Ground range to the target
   ) const;

   void getSolarRadiationSignatures(
                                    double* signatureArray,  // Container for the solar radiation data
                                    const double lowerBound, // Lower bound (microns) of interest
                                    const double upperBound, // Upper bound (microns) of interest
                                    const double altitude) const;  // Altitude of interest (meters)

   virtual double getSolarRadiation(
      const double lowerWavelength,  // Lower wavelength of the wave band (microns)
      const double upperWavelength,  // Upper wavelength of the wave band (microns)
      const double targetAltitude) const;  // Altitude of the target (meters)

   virtual double getSolarRadiation(
      const double wavebandCenter,       // The waveband center (microns)
      const double targetAltitude        // Upper wavelength of the wave band (microns)
   ) const;

   double getBackgroundRadiation(
      const double lowerWavelength,      // Lower wavelength of the wave band (microns)
      const double upperWavelength,      // Upper wavelength of the wave band (microns)
      const double seekerAltitude,       // The altitude of the seeker (meters)
      const double viewAngle             // View Angle (Radians)
   ) const;

   double getBackgroundRadiation(
      const double wavebandCenter,       // The waveband center (microns)
      const double seekerAltitude,       // The altitude of the seeker (meters)
      const double viewAngle             // View Angle (Radians)
   ) const;

   //Slot functions
   virtual bool setSlotSolarRadiationTable(const base::Table2* const tbl);
   virtual bool setSlotBackgroundRadiationTable(const base::Table3* const tbl);
   virtual bool setSlotTransmissivityTable(const base::Table4* const tbl);

private:
   const base::Table2* solarRadiationTable;
   const base::Table3* backgroundRadiationTable;
   const base::Table4* transmissivityTable;
};

}
}

#endif
