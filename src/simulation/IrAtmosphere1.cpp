//==============================================================================
// class IrAtmosphere1 --
//==============================================================================

#include "openeaagles/simulation/IrAtmosphere1.h"

#include "openeaagles/simulation/IrQueryMsg.h"   // FAB
#include "openeaagles/simulation/IrSensor.h"     // FAB
#include "openeaagles/simulation/Gimbal.h"       // FAB
#include "openeaagles/simulation/Player.h"       // FAB
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/Number.h"

#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

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
   ON_SLOT(1,setSlotSolarRadiationTable,Basic::Table2)
   ON_SLOT(2,setSlotBackgroundRadiationTable,Basic::Table3)
   ON_SLOT(3,setSlotTransmissivityTable,Basic::Table4)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
IrAtmosphere1::IrAtmosphere1()
{
   STANDARD_CONSTRUCTOR()

   solarRadiationTable = 0;
   backgroundRadiationTable = 0;
   transmissivityTable = 0;
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
   if (solarRadiationTable != 0) {
      solarRadiationTable->unref();
      solarRadiationTable = 0;
   }

   if (backgroundRadiationTable != 0) {
      backgroundRadiationTable->unref();
      backgroundRadiationTable = 0;
   }

   if (transmissivityTable != 0) {
      transmissivityTable->unref();
      transmissivityTable = 0;
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool IrAtmosphere1::setSlotSolarRadiationTable(const Basic::Table2* const tbl)
{
   bool ok = false;
   // tbl->serialize(std::cout);
   if (tbl != 0) {
      if (solarRadiationTable != 0) solarRadiationTable->unref();
      tbl->ref();
      solarRadiationTable = tbl;
      ok = true;
   }
   return ok;
}

bool IrAtmosphere1::setSlotBackgroundRadiationTable(const Basic::Table3* const tbl)
{
   bool ok = false;
   if (tbl != 0) {
      if (backgroundRadiationTable != 0) backgroundRadiationTable->unref();
      tbl->ref();
      backgroundRadiationTable = tbl;
      ok = true;
   }
   return ok;
}

bool IrAtmosphere1::setSlotTransmissivityTable(const Basic::Table4* const tbl)
{
   bool ok = false;
   if (tbl != 0) {
      if (transmissivityTable != 0) transmissivityTable->unref();
      tbl->ref();
      transmissivityTable = tbl;
      ok = true;
   }
   return ok;
}


bool IrAtmosphere1::calculateAtmosphereContribution(IrQueryMsg* const msg, LCreal* totalSignal, LCreal* totalBackground)
{
   // Sum the total signal that reaches the seeker of the target represented by the message
   // and the background noise observed by the seeker

   const LCreal* centerWavelengths = getWaveBandCenters();
   const LCreal* widths = getWaveBandWidths();
   const LCreal* sigArray = msg->getSignatureByWaveband();
   Player* ownship = msg->getOwnship();
   Player* target = msg->getTarget();
   
   // FAB - this should be angle of gimbal, not angle to target. (see base class)
   // Determine the angle above the horizon to be used for background radiation lookup
   LCreal range2D = msg->getRange();
   LCreal tanPhi = (LCreal)( (target->getAltitudeM() - ownship->getAltitudeM())/ range2D );
   LCreal tanPhiPrime = tanPhi - ( range2D / 12756776.0f ); // Twice earth radius
   
   // appears that negative angles are down in this calculation
   LCreal viewingAngle = lcAtan(tanPhiPrime);
   
   // table limits are 0 to pi; this correction assumes that 0 in the table is straight down, PI is straight up
   viewingAngle += PI/2.0;
   
   *totalSignal = 0.0;
   *totalBackground = 0.0;

   for (unsigned int i=0; i<getNumWaveBands(); i++) {
      LCreal radiantIntensityInBin;
      LCreal lowerBandBound = centerWavelengths[i] - (widths[i] / 2.0f);
      LCreal upperBandBound = lowerBandBound + widths[i];
      
      // determine ratio of this band's coverage to entire atmosphere waveband
      LCreal fractionOfBandToTotal = (upperBandBound - lowerBandBound) / ((centerWavelengths[getNumWaveBands() - 1] + (widths[getNumWaveBands() - 1] / 2.0f))-(centerWavelengths[0] - (widths[0] / 2.0f)));
      
      // Find the limits of the sensor
      LCreal lowerSensorBound = msg->getLowerWavelength();
      LCreal upperSensorBound = msg->getUpperWavelength();
      
      // Determine how much of this wave band overlaps the sensor limits
      LCreal lowerOverlap = getLowerEndOfWavelengthOverlap(
                                                   lowerBandBound,
                                                   lowerSensorBound);
      LCreal upperOverlap = getUpperEndOfWavelengthOverlap(
                                                   upperBandBound,
                                                   upperSensorBound);
      
      if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;
      
      LCreal overlapRatio = (upperOverlap - lowerOverlap) / (upperBandBound - lowerBandBound);
      
      // Get the background radiation given the sensor altitude and the viewing angle
      LCreal backgroundRadianceInBand = overlapRatio * getBackgroundRadiation(
                                                lowerBandBound,   
                                                upperBandBound,
                                                (LCreal) ownship->getAltitudeM(),
                                                viewingAngle);
      if (sigArray == 0) {
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
      LCreal solarRadiationInBin = ((1.0f - msg->getEmissivity()) * getSolarRadiation(centerWavelengths[i], (LCreal) target->getAltitudeM()));
      radiantIntensityInBin += (solarRadiationInBin * overlapRatio);
      
      // Lookup the transmissivity in the wave band given the altitudes of sensor
      // and target and the ground range between the two
      LCreal transmissivity = getTransmissivity(
                                                lowerBandBound,
                                                upperBandBound,
                                                (LCreal) ownship->getAltitudeM(),
                                                (LCreal) target->getAltitudeM(),
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

LCreal IrAtmosphere1::getTransmissivity(
               const LCreal lowerWavelength,
               const LCreal upperWavelength,
               const LCreal seekerAltitude,
               const LCreal targetAltitude,
               const LCreal range) const
{
   LCreal wavebandCenter = (upperWavelength + lowerWavelength) / 2;
   LCreal trans = 1.0;  //Default to full propagation of energy in the absence of a table.
   if (0 != transmissivityTable){
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

LCreal IrAtmosphere1::getTransmissivity(
                        const LCreal wavebandCenter,
                        const LCreal seekerAltitude,
                        const LCreal targetAltitude,
                        const LCreal range) const
{
   LCreal trans = 1.0;
   if (0 != transmissivityTable){
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

LCreal IrAtmosphere1::getSolarRadiation(
                        const LCreal lowerWavelength,  
                        const LCreal upperWavelength,
                        const LCreal targetAltitude) const
{
   LCreal wavebandCenter = (upperWavelength + lowerWavelength) / 2;
   LCreal solarRadiation = 0;
   if (0 != solarRadiationTable){
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

LCreal IrAtmosphere1::getSolarRadiation(
                        const LCreal wavebandCenter,
                        const LCreal targetAltitude) const
{
   LCreal sr = 0;
   if (0 != solarRadiationTable){
      sr = solarRadiationTable->lfi(wavebandCenter,targetAltitude);
   }
   return sr;
}

//------------------------------------------------------------------------------------------------------
// getBackgroundRadiation() -- Return the amount of background radiation in the region of the spectrum defined by the 
//        upper and lower wavelengths as a function the altitude of the seeker and angle of viewing.
//        The unit of output is Watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

LCreal IrAtmosphere1::getBackgroundRadiation(
                           const LCreal lowerWavelength,
                           const LCreal upperWavelength,
                           const LCreal seekerAltitude,
                           const LCreal viewAngle) const
{
   LCreal wavebandCenter = (upperWavelength + lowerWavelength) / 2;
   LCreal bgRadiation = 0;
   if (0!= backgroundRadiationTable){
      bgRadiation = backgroundRadiationTable->lfi(wavebandCenter,seekerAltitude,viewAngle);
   }
   return bgRadiation;
}

//------------------------------------------------------------------------------------------------------
// getBackgroundRadiation() -- Return the amount of background radiation in the region surrounded by the center of the 
//        waveband as a function the altitude of the seeker and angle of viewing.
//        The unit of output is Watts/steradian sq-m.
//------------------------------------------------------------------------------------------------------

LCreal IrAtmosphere1::getBackgroundRadiation(
                        const LCreal wavebandCenter,
                        const LCreal seekerAltitude,
                        const LCreal viewAngle) const
{
   LCreal bgRadiation = 0;
   if ( 0 != backgroundRadiationTable){
      bgRadiation = backgroundRadiationTable->lfi(wavebandCenter,seekerAltitude,viewAngle);
   }
   return bgRadiation;
}

//------------------------------------------------------------------------------------------------------
// getSolarRadiationSignatures() -- Returns an array which holds, for each defined bin, the upper lower 
//        and wavelength and the amount of solar radiation in the bin. The array is of size 3 * numBins.
//------------------------------------------------------------------------------------------------------

void IrAtmosphere1::getSolarRadiationSignatures(
                                    LCreal* signatureArray,
                                    const LCreal lowerBound,
                                    const LCreal upperBound,
                                    const LCreal altitude) const
{
   const LCreal* centerWavelengths = this->getWaveBandCenters();
   const LCreal* widths = this->getWaveBandWidths();
   for (unsigned int i = 0; i < getNumWaveBands(); i++) {
      LCreal centerWavelength = centerWavelengths[i];
      LCreal lowerWavelength = centerWavelength - (widths[i] / 2.0f);
      LCreal upperWavelength = lowerWavelength + widths[i];
      signatureArray[i*3] = lowerWavelength;
      signatureArray[i*3 + 1] = upperWavelength;
      if (upperBound > lowerWavelength && lowerBound < upperWavelength) {
         signatureArray[i * 3 + 2] = getSolarRadiation(centerWavelength, altitude);
      }
      else signatureArray[i * 3 + 2] = 0.0f;
   }

}


Basic::Object* IrAtmosphere1::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
