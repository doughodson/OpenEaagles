//==============================================================================
// class IrAtmosphere1 --
//==============================================================================

#include "openeaagles/simulation/IrAtmosphere1.hpp"

#include "openeaagles/simulation/IrQueryMsg.hpp"
#include "openeaagles/simulation/IrSensor.hpp"
#include "openeaagles/simulation/Gimbal.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/base/List.hpp"
#include "openeaagles/base/functors/Tables.hpp"
#include "openeaagles/base/Number.hpp"

#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/units/Distances.hpp"

#include <cmath>

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(IrAtmosphere1,"IrAtmosphere1")
EMPTY_SERIALIZER(IrAtmosphere1)

// slot table
BEGIN_SLOTTABLE(IrAtmosphere1)
   "solarRadiationTable",      // The tables containing solar radiation tables
   "backgroundRadiationTable", // The background radiation table
   "transmissivityTable",      // The tables containing transmissivity data
END_SLOTTABLE(IrAtmosphere1)

// slot map
BEGIN_SLOT_MAP(IrAtmosphere1)
   ON_SLOT(1,setSlotSolarRadiationTable,base::Table2)
   ON_SLOT(2,setSlotBackgroundRadiationTable,base::Table3)
   ON_SLOT(3,setSlotTransmissivityTable,base::Table4)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
IrAtmosphere1::IrAtmosphere1()
{
   STANDARD_CONSTRUCTOR()

   solarRadiationTable = nullptr;
   backgroundRadiationTable = nullptr;
   transmissivityTable = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void IrAtmosphere1::copyData(const IrAtmosphere1& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void IrAtmosphere1::deleteData()
{
   if (solarRadiationTable != nullptr) {
      solarRadiationTable->unref();
      solarRadiationTable = nullptr;
   }

   if (backgroundRadiationTable != nullptr) {
      backgroundRadiationTable->unref();
      backgroundRadiationTable = nullptr;
   }

   if (transmissivityTable != nullptr) {
      transmissivityTable->unref();
      transmissivityTable = nullptr;
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool IrAtmosphere1::setSlotSolarRadiationTable(const base::Table2* const tbl)
{
   bool ok = false;
   // tbl->serialize(std::cout);
   if (tbl != nullptr) {
      if (solarRadiationTable != nullptr) solarRadiationTable->unref();
      tbl->ref();
      solarRadiationTable = tbl;
      ok = true;
   }
   return ok;
}

bool IrAtmosphere1::setSlotBackgroundRadiationTable(const base::Table3* const tbl)
{
   bool ok = false;
   if (tbl != nullptr) {
      if (backgroundRadiationTable != nullptr) backgroundRadiationTable->unref();
      tbl->ref();
      backgroundRadiationTable = tbl;
      ok = true;
   }
   return ok;
}

bool IrAtmosphere1::setSlotTransmissivityTable(const base::Table4* const tbl)
{
   bool ok = false;
   if (tbl != nullptr) {
      if (transmissivityTable != nullptr) transmissivityTable->unref();
      tbl->ref();
      transmissivityTable = tbl;
      ok = true;
   }
   return ok;
}


bool IrAtmosphere1::calculateAtmosphereContribution(IrQueryMsg* const msg, double* totalSignal, double* totalBackground)
{
   // Sum the total signal that reaches the seeker of the target represented by the message
   // and the background noise observed by the seeker

   const double* centerWavelengths = getWaveBandCenters();
   const double* widths = getWaveBandWidths();
   const double* sigArray = msg->getSignatureByWaveband();
   const Player* ownship = msg->getOwnship();
   const Player* target = msg->getTarget();

   // FAB - this should be angle of gimbal, not angle to target. (see base class)
   // Determine the angle above the horizon to be used for background radiation lookup
   const double range2D = msg->getRange();
   const double tanPhi = static_cast<double>( (target->getAltitudeM() - ownship->getAltitudeM())/ range2D );
   const double tanPhiPrime = tanPhi - ( range2D / 12756776.0f ); // Twice earth radius

   // appears that negative angles are down in this calculation
   double viewingAngle = std::atan(tanPhiPrime);

   // table limits are 0 to pi; this correction assumes that 0 in the table is straight down, PI is straight up
   viewingAngle += base::PI / 2.0;

   *totalSignal = 0.0;
   *totalBackground = 0.0;

   for (unsigned int i=0; i<getNumWaveBands(); i++) {
      const double lowerBandBound = centerWavelengths[i] - (widths[i] / 2.0f);
      const double upperBandBound = lowerBandBound + widths[i];

      // determine ratio of this band's coverage to entire atmosphere waveband
      const double fractionOfBandToTotal = (upperBandBound - lowerBandBound) / ((centerWavelengths[getNumWaveBands() - 1] + (widths[getNumWaveBands() - 1] / 2.0f))-(centerWavelengths[0] - (widths[0] / 2.0f)));

      // Find the limits of the sensor
      const double lowerSensorBound = msg->getLowerWavelength();
      const double upperSensorBound = msg->getUpperWavelength();

      // Determine how much of this wave band overlaps the sensor limits
      const double lowerOverlap = getLowerEndOfWavelengthOverlap(lowerBandBound, lowerSensorBound);
      double upperOverlap = getUpperEndOfWavelengthOverlap(upperBandBound, upperSensorBound);

      if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;

      const double overlapRatio = (upperOverlap - lowerOverlap) / (upperBandBound - lowerBandBound);

      // Get the background radiation given the sensor altitude and the viewing angle
      double backgroundRadianceInBand = overlapRatio * getBackgroundRadiation(
                                                lowerBandBound,
                                                upperBandBound,
                                                static_cast<double>(ownship->getAltitudeM()),
                                                viewingAngle);
      double radiantIntensityInBin(0.0);
      if (sigArray == nullptr) {
         // signature is a simple number
         // distribute simple signature evenly across atmosphere bins
         // need to apply overlapRatio to simple signature - already applied for complex signature in IrSignature...
         radiantIntensityInBin = msg->getSignatureAtRange() * fractionOfBandToTotal * overlapRatio;
      }
      else {
         // Find the limits of the bin
         //lowerBandBound = sigArray [i*3];
         //upperBandBound = sigArray [i*3 + 1] ;
         // assuming that signature bands match atmosphere bands
         radiantIntensityInBin = sigArray[i*3 + 2];
      }

      // add in reflected solar radiation
      const double solarRadiationInBin = ((1.0f - msg->getEmissivity()) * getSolarRadiation(centerWavelengths[i],
                                    static_cast<double>(target->getAltitudeM())));
      radiantIntensityInBin += (solarRadiationInBin * overlapRatio);

      // Lookup the transmissivity in the wave band given the altitudes of sensor
      // and target and the ground range between the two
      const double transmissivity = getTransmissivity(
                                                lowerBandBound,
                                                upperBandBound,
                                                static_cast<double>(ownship->getAltitudeM()),
                                                static_cast<double>(target->getAltitudeM()),
                                                range2D);

      *totalSignal += radiantIntensityInBin * transmissivity;

      // Add the background radiance from the this waveband within the sensor limits
      // to the total background radiance received by the sensor, watts/sr-m^2
      *totalBackground += backgroundRadianceInBand * transmissivity;
   }

   return true;
}

//------------------------------------------------------------------------------------------------------
// getTransmissivity() --  Return the fraction of infrared radiation transmitted in the region
//        of the spectrum defined by the upper and lower wavelengths as a function
//        of the positions of the seeker and the target. The return value is between
//        0.0 (no power gets through) and 1.0 (All power gets through)
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getTransmissivity(
               const double lowerWavelength,
               const double upperWavelength,
               const double seekerAltitude,
               const double targetAltitude,
               const double range) const
{
   const double wavebandCenter = (upperWavelength + lowerWavelength) / 2;
   double trans = 1.0;  //Default to full propagation of energy in the absence of a table.
   if (transmissivityTable != nullptr){
      trans = transmissivityTable->lfi(wavebandCenter,seekerAltitude,targetAltitude,range);
   }
   else {
      // use base class' simple transmissivity if no table
      trans = BaseClass::getTransmissivity(wavebandCenter,range);
   }
   return trans;
}

//------------------------------------------------------------------------------------------------------
// getTransmissivity() -- Return the fraction of infrared radiation transmitted in the region surrounded
//        by the center of the waveband as a function of the positions of the seeker and the target.
//        The return value is between 0.0 (no power gets through) and 1.0 (All power gets through)
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getTransmissivity(
                        const double wavebandCenter,
                        const double seekerAltitude,
                        const double targetAltitude,
                        const double range) const
{
   double trans = 1.0;
   if (transmissivityTable != nullptr){
      trans = transmissivityTable->lfi(wavebandCenter,seekerAltitude,targetAltitude,range);
   }
   else {
      // use base class' simple transmissivity if no table
      trans = BaseClass::getTransmissivity(wavebandCenter,range);
   }
   return trans;
}

//------------------------------------------------------------------------------------------------------
// getSolarRadiation() -- Return the amount of solar radiation in the region of the spectrum defined by the
//        upper and lower wavelengths as a function of the target altitude. The output is in
//        the units watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getSolarRadiation(
                        const double lowerWavelength,
                        const double upperWavelength,
                        const double targetAltitude) const
{
   const double wavebandCenter = (upperWavelength + lowerWavelength) / 2.0;
   double solarRadiation = 0.0;
   if (solarRadiationTable != nullptr){
      solarRadiation = solarRadiationTable->lfi(wavebandCenter,targetAltitude);
   }
   return solarRadiation;
}

//------------------------------------------------------------------------------------------------------
// getSolarRadiation() -- Return the amount of solar radiation in the region surrounded by the center of the
//        waveband as a function of the target altitude. The routine does a table lookup in a 2-D
//        table where the x coordinate is the waveband center and the y coordinate is the altitude the
//        target. The unit of output is Watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getSolarRadiation(
                        const double wavebandCenter,
                        const double targetAltitude) const
{
   double sr = 0.0;
   if (solarRadiationTable != nullptr){
      sr = solarRadiationTable->lfi(wavebandCenter,targetAltitude);
   }
   return sr;
}

//------------------------------------------------------------------------------------------------------
// getBackgroundRadiation() -- Return the amount of background radiation in the region of the spectrum defined by the
//        upper and lower wavelengths as a function the altitude of the seeker and angle of viewing.
//        The unit of output is Watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getBackgroundRadiation(
                           const double lowerWavelength,
                           const double upperWavelength,
                           const double seekerAltitude,
                           const double viewAngle) const
{
   const double wavebandCenter = (upperWavelength + lowerWavelength) / 2.0;
   double bgRadiation = 0.0;
   if (backgroundRadiationTable != nullptr){
      bgRadiation = backgroundRadiationTable->lfi(wavebandCenter,seekerAltitude,viewAngle);
   }
   return bgRadiation;
}

//------------------------------------------------------------------------------------------------------
// getBackgroundRadiation() -- Return the amount of background radiation in the region surrounded by the center of the
//        waveband as a function the altitude of the seeker and angle of viewing.
//        The unit of output is Watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

double IrAtmosphere1::getBackgroundRadiation(
                        const double wavebandCenter,
                        const double seekerAltitude,
                        const double viewAngle) const
{
   double bgRadiation = 0.0;
   if (backgroundRadiationTable != nullptr){
      bgRadiation = backgroundRadiationTable->lfi(wavebandCenter,seekerAltitude,viewAngle);
   }
   return bgRadiation;
}

//------------------------------------------------------------------------------------------------------
// getSolarRadiationSignatures() -- Returns an array which holds, for each defined bin, the upper lower
//        and wavelength and the amount of solar radiation in the bin. The array is of size 3 * numBins.
//------------------------------------------------------------------------------------------------------

void IrAtmosphere1::getSolarRadiationSignatures(
                                    double* signatureArray,
                                    const double lowerBound,
                                    const double upperBound,
                                    const double altitude) const
{
   const double* centerWavelengths = this->getWaveBandCenters();
   const double* widths = this->getWaveBandWidths();
   for (unsigned int i = 0; i < getNumWaveBands(); i++) {
      const double centerWavelength = centerWavelengths[i];
      const double lowerWavelength = centerWavelength - (widths[i] / 2.0);
      const double upperWavelength = lowerWavelength + widths[i];
      signatureArray[i*3] = lowerWavelength;
      signatureArray[i*3 + 1] = upperWavelength;
      if (upperBound > lowerWavelength && lowerBound < upperWavelength) {
         signatureArray[i * 3 + 2] = getSolarRadiation(centerWavelength, altitude);
      }
      else signatureArray[i * 3 + 2] = 0.0f;
   }

}


base::Object* IrAtmosphere1::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

}
}
