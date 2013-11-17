//------------------------------------------------------------------------------
// Classes: AircraftIrSignature
//------------------------------------------------------------------------------

#include "openeaagles/simulation/AircraftIrSignature.h"
#include "openeaagles/simulation/IrShapes.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Tables.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Areas.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(AircraftIrSignature,"AircraftIrSignature")
EMPTY_SERIALIZER(AircraftIrSignature)

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(AircraftIrSignature)
   "airframeSignatureTable",  // mapping of 
            // signature to  pla, where pla is in % from 0 - 100 
            // x of this table is the pla
            //  y is the velocity (in sim prevailing units -- 
            // knots, I believe) and z is altitude (in sim prevailing units --
            // meters, I believe). Data is the heat signature in watts per steradian. 
   "airframeWavebandFactorTable",
            // airframe wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this 
            // factor to get the plume energy in this particular waveband.  
            // the different factors should all sum to 1.0  .  
   "plumeSignatureTable",
            // mapping of signature in watts/steradian to 
            // x- PLA 0.0 - 2.0 (0.0=idle, 1.0=mil, 2.0=afterburner)
            // y - velocity (mach #)
            // z - altitude (meters) 
            // azimuth (radians)
            // elevation (radians) 
   "plumeWavebandFactorTable",
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this 
            // factor to get the plume energy in this particular waveband.  
            // the different factors should all sum to 1.0  .  
   "hotPartsSignatureTable",
            // mapping of signature in watts/steradian to 
            // x- pla 
            // y - velocity (knots)
            // z - altitude (meters) 
            // azimuth (radians)
            // elevation (radians) 
   "hotPartsWavebandFactorTable",
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this 
            // factor to get the plume energy in this particular waveband.  
            // the different factors should all sum to 1.0  .  

END_SLOTTABLE(AircraftIrSignature)

// Map slot table to handles 
BEGIN_SLOT_MAP(AircraftIrSignature)
   ON_SLOT(1,setSlotAirframeSignatureTable,Basic::Table4)
   ON_SLOT(2,setSlotAirframeWavebandFactorTable,Basic::Table2)
   ON_SLOT(3,setSlotPlumeSignatureTable,Basic::Table5)
   ON_SLOT(4,setSlotPlumeWavebandFactorTable,Basic::Table2)
   ON_SLOT(5,setSlotHotPartsSignatureTable,Basic::Table5)
   ON_SLOT(6,setSlotHotPartsWavebandFactorTable,Basic::Table2)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
AircraftIrSignature::AircraftIrSignature()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void AircraftIrSignature::initData()
{
    airframeSignatureTable = 0; 
    airframeWavebandFactorTable = 0; 
    plumeSignatureTable = 0; 
    plumeWavebandFactorTable = 0; 
    hotPartsSignatureTable = 0; 
    hotPartsWavebandFactorTable = 0; 
    airframeSig = 0; 
    plumeSigs = 0;
    hotPartsSigs = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void AircraftIrSignature::copyData(const AircraftIrSignature& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    if (org.airframeSignatureTable != 0) {
        Basic::Table4* copy = org.airframeSignatureTable->clone();
        setSlotAirframeSignatureTable( copy );
        copy->unref();
    }
    else {
        setSlotAirframeSignatureTable(0);
    }

    if (org.airframeWavebandFactorTable != 0) {
        Basic::Table2* copy = org.airframeWavebandFactorTable->clone();
        setSlotAirframeWavebandFactorTable( copy );
        copy->unref();
    }
    else {
        setSlotAirframeWavebandFactorTable(0);
    }

    if (org.plumeSignatureTable != 0) {
        Basic::Table5* copy = org.plumeSignatureTable->clone();
        setSlotPlumeSignatureTable( copy );
        copy->unref();
    }
    else {
        setSlotPlumeSignatureTable(0);
    }

    if (org.plumeWavebandFactorTable != 0) {
        Basic::Table2* copy = org.plumeWavebandFactorTable->clone();
        setSlotPlumeWavebandFactorTable( copy );
        copy->unref();
    }
    else {
        setSlotPlumeWavebandFactorTable(0);
    }

    if (org.hotPartsSignatureTable != 0) {
        Basic::Table5* copy = org.hotPartsSignatureTable->clone();
        setSlotHotPartsSignatureTable( copy );
        copy->unref();
    }
    else {
        setSlotHotPartsSignatureTable(0);
    }

    if (org.hotPartsWavebandFactorTable != 0) {
        Basic::Table2* copy = org.hotPartsWavebandFactorTable->clone();
        setSlotHotPartsWavebandFactorTable( copy );
        copy->unref();
    }
    else {
        setSlotHotPartsWavebandFactorTable(0);
    }

    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void AircraftIrSignature::deleteData()
{
    if (airframeSignatureTable != 0) {
        airframeSignatureTable->unref(); 
        airframeSignatureTable = 0; 
    }

    if (airframeWavebandFactorTable != 0) {
        airframeWavebandFactorTable->unref(); 
        airframeWavebandFactorTable = 0; 
    }

    if (plumeSignatureTable != 0) {
        plumeSignatureTable->unref(); 
        plumeSignatureTable = 0; 
    }

    if (plumeWavebandFactorTable != 0) {
        plumeWavebandFactorTable->unref(); 
        plumeWavebandFactorTable = 0; 
    }

    if (hotPartsSignatureTable != 0) {
        hotPartsSignatureTable->unref(); 
        hotPartsSignatureTable = 0; 
    }

    if (hotPartsWavebandFactorTable != 0) {
        hotPartsWavebandFactorTable->unref(); 
        hotPartsWavebandFactorTable = 0; 
    }

    if (airframeSig !=0 ) {
        delete[] airframeSig; 
        airframeSig = 0;
    }

    if (plumeSigs != 0) {
        delete[] plumeSigs; 
        plumeSigs = 0;
    }

    if (hotPartsSigs !=0) {
        delete[] hotPartsSigs; 
        hotPartsSigs = 0;
    }
}

//------------------------------------------------------------------------------
// getIrSignature() - Computes the IR signature for this IR query message
//------------------------------------------------------------------------------
bool AircraftIrSignature::getIrSignature(IrQueryMsg* const msg)
{
    bool ok = false;
    //IrQueryMsg* msg = dynamic_cast<IrQueryMsg*>( msg0 );
    if (msg != 0) {
        LCreal projectedAreaInFOV = getSignatureArea(msg);
        msg->setProjectedArea(projectedAreaInFOV);

        // if no projectedAreaInFOV, then target was not in FOV
        if (projectedAreaInFOV > 0.0f){
            ok = true;
            LCreal* heatSignature = getHeatSignature(msg);
            msg->setSignatureByWaveband(heatSignature);
            // FAB - set simple signature value
            msg->setSignatureAtRange(getCalculatedHeatSignature());
            msg->setEmissivity(getEmissivity());
        }
        else{
            msg->setSignatureByWaveband(0);
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() 
//------------------------------------------------------------------------------
Basic::Object* AircraftIrSignature::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// setSlotAirframeSignatureTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotAirframeSignatureTable(const Basic::Table4* const tbl)
{
   if (airframeSignatureTable != 0) {
      airframeSignatureTable->unref();
   }
   airframeSignatureTable = tbl;
   if (airframeSignatureTable != 0) {
      airframeSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotPlumeSignatureTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotPlumeSignatureTable(const Basic::Table5* const tbl)
{
   if (plumeSignatureTable != 0) {
      plumeSignatureTable->unref();
   }
   plumeSignatureTable = tbl;
   if (plumeSignatureTable != 0) {
      plumeSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotAirframeWavebandFactorTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotAirframeWavebandFactorTable(const Basic::Table2* const tbl)
{
   if (airframeWavebandFactorTable != 0) {
      airframeWavebandFactorTable->unref();
   }
   airframeWavebandFactorTable = tbl;
   if (airframeWavebandFactorTable != 0) {
      airframeWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotPlumeWavebandFactorTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotPlumeWavebandFactorTable(const Basic::Table2* const tbl)
{
   if (plumeWavebandFactorTable != 0) {
      plumeWavebandFactorTable->unref();
   }
   plumeWavebandFactorTable = tbl;
   if (plumeWavebandFactorTable != 0) {
      plumeWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotHotPartsSignatureTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotHotPartsSignatureTable(const Basic::Table5* const tbl)
{
   if (hotPartsSignatureTable != 0) {
      hotPartsSignatureTable->unref();
   }
   hotPartsSignatureTable = tbl;
   if (hotPartsSignatureTable != 0) {
      hotPartsSignatureTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotHotPartsWavebandFactorTable() 
//------------------------------------------------------------------------------
bool AircraftIrSignature::setSlotHotPartsWavebandFactorTable(const Basic::Table2* const tbl)
{
   if (hotPartsWavebandFactorTable != 0) {
      hotPartsWavebandFactorTable->unref();
   }
   hotPartsWavebandFactorTable = tbl;
   if (hotPartsWavebandFactorTable != 0) {
      hotPartsWavebandFactorTable->ref();
   }
   return true;
}

//------------------------------------------------------------------------------
// getAirframeSignature() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getAirframeSignature(LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation) 
{
    return (airframeSignatureTable->lfi(velocity, altitude, azimuth, elevation)); 
}

//------------------------------------------------------------------------------
// getAirframeWavebandFactor() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getAirframeWavebandFactor(LCreal midpoint, LCreal width) 
{
    return (airframeWavebandFactorTable->lfi(midpoint, width)); 
}

//------------------------------------------------------------------------------
// getPlumeSignature() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getPlumeSignature(LCreal pla, LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation) 
{
    return (plumeSignatureTable->lfi(pla, velocity, altitude, azimuth, elevation)); 
}

//------------------------------------------------------------------------------
// getPlumeWavebandFactor() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getPlumeWavebandFactor(LCreal midpoint, LCreal width) 
{
    return (plumeWavebandFactorTable->lfi(midpoint, width)); 
}

//------------------------------------------------------------------------------
// getHotPartsSignature() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getHotPartsSignature(LCreal pla, LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation) 
{
   return (hotPartsSignatureTable->lfi(pla, velocity, altitude, azimuth, elevation)); 
}

//------------------------------------------------------------------------------
// getHotPartsWavebandFactor() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getHotPartsWavebandFactor(LCreal midpoint, LCreal width) 
{
   return (hotPartsWavebandFactorTable->lfi(midpoint, width));
}

//------------------------------------------------------------------------------
// getCalculatedAirframeHeatSignature() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getCalculatedAirframeHeatSignature(const IrQueryMsg* const msg) {

    LCreal irPower = 0;
    AirVehicle* targetAircraft = dynamic_cast<AirVehicle*>((Player*)msg->getTarget());
    if(targetAircraft != 0){
        // this will need checks to ensure targetAircraft is , in fact, 
        // an airvehicle and not something else. 
        LCreal targetAlt = (LCreal) targetAircraft->getAltitudeM();
        LCreal targetVel = targetAircraft->getMach();  
        LCreal targetAzimuth = msg->getAzimuthAoi();
        LCreal targetElevation = msg->getElevationAoi(); 
        if (targetAzimuth < 0) {
            targetAzimuth = -targetAzimuth; 
        }
        irPower = getAirframeSignature(targetVel, targetAlt, targetAzimuth, targetElevation);
    }
    return irPower;
}

// FAB -  parallel hotparts and plume
//------------------------------------------------------------------------------
// getAirframeSignatures() 
//------------------------------------------------------------------------------
void AircraftIrSignature::getAirframeSignatures(const IrQueryMsg* const msg, const LCreal lowerBound, const LCreal upperBound)
{
    if (airframeWavebandFactorTable != 0 && airframeSignatureTable != 0) {
        // find airframe factor. 
        const LCreal* centerWavelengths = airframeWavebandFactorTable->getXData();
        const LCreal* widths = airframeWavebandFactorTable->getYData();
        LCreal irPower = getCalculatedAirframeHeatSignature(msg);

        for (unsigned int i = 0; i < (unsigned int) airframeWavebandFactorTable->getNumXPoints(); i++) {
            LCreal centerWavelength = centerWavelengths[i];
            LCreal lowerWavelength = centerWavelength - (widths[i] / 2.0f);
            LCreal upperWavelength = lowerWavelength + widths[i];

            airframeSig[i*3] = lowerWavelength;
            airframeSig[i*3 + 1] = upperWavelength;

            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                LCreal airframeWaveBandFactor  = airframeWavebandFactorTable->lfi(centerWavelength,widths[i]);
                airframeSig[i*3 + 2] = irPower * airframeWaveBandFactor;
            }
            else{
                airframeSig[i*3 + 2] = 0.0f;
            }
        }
    }
}

//------------------------------------------------------------------------------
// getPlumeRadiation() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getPlumeRadiation(const IrQueryMsg* const msg)
{
    LCreal currentPla = 1.0f; 
    LCreal irPower = 0;
    const Player* targetAircraft = msg->getTarget();
    if (targetAircraft != 0) {
        if (targetAircraft->isClassType(typeid(AirVehicle))) {
            currentPla = getPLA((const AirVehicle*) targetAircraft); 
        }

        LCreal targetAlt = (LCreal) targetAircraft->getAltitudeM();
        LCreal targetVel = targetAircraft->getMach(); 
        LCreal targetAzimuth = msg->getAzimuthAoi();
        LCreal targetElevation = msg->getElevationAoi(); 
        if (targetAzimuth < 0) {
            targetAzimuth = -targetAzimuth; 
        }

        irPower = getPlumeSignature(currentPla, targetVel, targetAlt, targetAzimuth, targetElevation); 
    }
    return irPower;
}

//------------------------------------------------------------------------------
// getPlumeSignatures() 
//------------------------------------------------------------------------------
void AircraftIrSignature::getPlumeSignatures(const IrQueryMsg* const msg, const LCreal lowerBound, const LCreal upperBound)
{
    if (plumeWavebandFactorTable != 0) {
        const LCreal* centerWavelengths = plumeWavebandFactorTable->getXData();
        const LCreal* widths = plumeWavebandFactorTable->getYData();
        LCreal irPower = getPlumeRadiation(msg);
        for (unsigned int i = 0; i < (unsigned int) plumeWavebandFactorTable->getNumXPoints(); i++) {
            LCreal centerWavelength = centerWavelengths[i];
            LCreal lowerWavelength = centerWavelength - (widths[i] / 2.0f);
            LCreal upperWavelength = lowerWavelength + widths[i];
            plumeSigs[i*3] = lowerWavelength;
            plumeSigs[i*3 + 1] = upperWavelength;
            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                LCreal plumeFactor = plumeWavebandFactorTable->lfi(centerWavelength,widths[i]);
                // plume has this factor contribution in this bin
                plumeSigs[i*3 + 2] = irPower * plumeFactor;
                // this number would represent the contribution if this bin were completely
                // contained by the sensor band.

                // FAB - overlapRatio could be implemented for each table like this -
                // but only one piece needed if it is desired that bins not all be the same for all tables.
                // also have to fix loop on i in heat signature

                // the overlap ratio is calculated to correct this number if the sensor band only 
                // partially covers this bin.
                //LCreal lowerOverlap = atmos->getLowerEndOfWavelengthOverlap(
                //                                               lowerWavelength,
                //                                               lowerBound);
                //LCreal upperOverlap = atmos->getUpperEndOfWavelengthOverlap(
                //                                               upperWavelength,
                //                                               upperBound);
                //if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;

                // how much of this bin is contained in the sensor band?
                //LCreal overlapRatio = (upperOverlap - lowerOverlap)
                //                   (upperWavelength - lowerWavelength);

                //plumeSigs[i*3 + 2] *= overlapRatio
            }
            else{
                plumeSigs[i*3 + 2] = 0.0f;
            }
        }
    } // if plumeWavebandFactor != 0 
}

//------------------------------------------------------------------------------
// getPLA() - Get the power level angle from the air vehicle. This represents a number
//    between 0.0 and 2.0. O.0 represents idle power on the engine, 1.0 represents 
//    Mil Power, and 2.0 represents after burner
//------------------------------------------------------------------------------

LCreal AircraftIrSignature::getPLA(const AirVehicle* const airModel) {
    // use only for air vehicles. 
    LCreal currentPla = 0.0; 
    LCreal idleValue = 0.0;
    LCreal milValue = 0.0;
    LCreal maxValue = 0.0;
    LCreal currentThrust = 0.0; 

    airModel->getEngThrust(&currentThrust, 1);
    airModel->getEngThrustIdle(&idleValue, 1);
    airModel->getEngThrustMil(&milValue, 1);
    airModel->getEngThrustAb(&maxValue, 1);

    if (currentThrust < milValue) {
        currentPla = ((currentThrust - idleValue) / (milValue - idleValue)); 
    } else if (currentThrust == milValue) {
        currentPla = 1.0f; 
    } else if (currentThrust < maxValue) { 
        currentPla = 1.0f + ((currentThrust - milValue) / (maxValue - milValue)); 
    } else {currentPla = 2.0f;}

    return currentPla; 

}

//------------------------------------------------------------------------------
// getHotPartsRadiation() 
//------------------------------------------------------------------------------
LCreal AircraftIrSignature::getHotPartsRadiation(const IrQueryMsg* const msg)
{
    LCreal currentPla = 1.0f;

    const Player* targetAircraft = msg->getTarget();
    if (targetAircraft != 0) {
        if (targetAircraft->isClassType(typeid(AirVehicle))) {
            currentPla = getPLA((const AirVehicle*) targetAircraft);
        }
    }

    LCreal targetAlt = (LCreal) targetAircraft->getAltitudeM();
    LCreal targetVel = targetAircraft->getMach();
    LCreal targetAzimuth = msg->getAzimuthAoi();
    LCreal targetElevation = msg->getElevationAoi();
    if (targetAzimuth < 0) targetAzimuth = -targetAzimuth;
    LCreal irPower = getHotPartsSignature(currentPla, targetVel, targetAlt, targetAzimuth, targetElevation);

    return irPower;
}

//------------------------------------------------------------------------------
// getHotPartsSignatures()
//------------------------------------------------------------------------------
void AircraftIrSignature::getHotPartsSignatures(const IrQueryMsg* const msg, const LCreal lowerBound, const LCreal upperBound)
{
    if (hotPartsWavebandFactorTable != 0) {
        const LCreal* centerWavelengths = hotPartsWavebandFactorTable->getXData();
        const LCreal* widths = hotPartsWavebandFactorTable->getYData();
        LCreal irPower = getHotPartsRadiation(msg);
        for (unsigned int i = 0; i < (unsigned int) hotPartsWavebandFactorTable->getNumXPoints(); i++) {
            LCreal centerWavelength = centerWavelengths[i];
            LCreal lowerWavelength = centerWavelength - (widths[i] / 2.0f);
            LCreal upperWavelength = lowerWavelength + widths[i];
            hotPartsSigs[i*3] = lowerWavelength;
            hotPartsSigs[i*3 + 1] = upperWavelength;
            if (upperBound >= lowerWavelength && lowerBound <= upperWavelength) {
                // sensor band overlaps this bin
                LCreal hotPartsFactor = hotPartsWavebandFactorTable->lfi(centerWavelength,widths[i]);
                hotPartsSigs[i*3 + 2] = irPower * hotPartsFactor;
            }
            else{
                hotPartsSigs[i*3 + 2] = 0.0f;
            }
        }
    } // if waveband is not null.
}

//------------------------------------------------------------------------------
// getHeatSignature() - Get the heat signature
//------------------------------------------------------------------------------
LCreal* AircraftIrSignature::getHeatSignature(IrQueryMsg* msg)
{
    Player* target = msg->getTarget();
    if (target != 0) {
        IrAtmosphere* atmos = 0;
        Simulation* sim = target->getSimulation();
        if (sim != 0)
            atmos = dynamic_cast<IrAtmosphere*>( sim->getIrAtmosphere() );

        unsigned int numBins = getNumWaveBands();
        if (airframeSig == 0) airframeSig = new LCreal [numBins * 3];
        if (plumeSigs == 0) plumeSigs = new LCreal [numBins * 3];
        if (hotPartsSigs == 0) hotPartsSigs = new LCreal [numBins * 3];

        //LCreal reflectivity = 1.0f - getEmissivity();
        LCreal lowerBound = msg->getLowerWavelength();
        LCreal upperBound = msg->getUpperWavelength();

        if (atmos != 0) {
            if (atmos->getNumWaveBands() != getNumWaveBands()) {
                // warning message
            }
        }

        // apparently no emissivity factor in these airframe signatures
        getAirframeSignatures(msg, lowerBound, upperBound);
        getPlumeSignatures(msg, lowerBound, upperBound);
        getHotPartsSignatures(msg, lowerBound, upperBound);

        const LCreal* centerWavelengths = getWaveBandCenters();
        const LCreal* widths = getWaveBandWidths();
        //LCreal totalWavelengthRange = ((centerWavelengths[getNumWaveBands() - 1] + (widths[getNumWaveBands() - 1] / 2.0f))-(centerWavelengths[0] - (widths[0] / 2.0f)));

        for (unsigned int i=0; i<getNumWaveBands(); i++) {
            // determine if our sensor band overlap this signature band
            LCreal lowerBandBound = centerWavelengths[i] - (widths[i] / 2.0f);
            LCreal upperBandBound = lowerBandBound + widths[i];
            if (upperBound > lowerBandBound && lowerBound < upperBandBound) {

                // calculate how much of this wave band overlaps the sensor limits 
                LCreal lowerOverlap = getLowerEndOfWavelengthOverlap(lowerBandBound, lowerBound);
                LCreal upperOverlap = getUpperEndOfWavelengthOverlap(upperBandBound, upperBound);
                if (upperOverlap < lowerOverlap) upperOverlap = lowerOverlap;
                LCreal overlapRatio = (upperOverlap - lowerOverlap) / (upperBandBound - lowerBandBound);

                // get our main signature piece - airframe
                LCreal baseHeatSignatureInBand = airframeSig[i*3 + 2];

                //if (isMessageEnabled(MSG_INFO)) {
                //std::cout << "For wavelength " << currentWavelength << " Airframe Heat Signature: " << baseHeatSignatureInBand << std::endl;
                //std::cout << "For wavelength " << currentWavelength << " Plume Signature: " << plumeSigs[i *3 +2] << std::endl;
                //std::cout << "For wavelength " << currentWavelength << " Hot Parts: " << hotPartsSigs[i * 3 + 2] << std::endl;
                //}

                // assume plume bins & hot parts bins are same as airframe bins, so we 
                // can re-use the same overlap ratios and fractions. If not, 
                // they will have to be separately calculated. 
                baseHeatSignatureInBand += plumeSigs[i*3 + 2]; 
                baseHeatSignatureInBand += hotPartsSigs[i*3 + 2];

                // if we have an atmosphere model, get the reflected solar radiation contribution
                // Solar signature is solar value * reflectivity i.e. (1 - emissivity)
                // use of reflectivity here suggests that this is solar radiation reflected from the target
                // this now done in the atmosphere model, during query return processing
                //if (atmos != 0)
                //   baseHeatSignatureInBand += (reflectivity * atmos->getSolarRadiation(centerWavelengths[i], (LCreal) target->getAltitudeM()));

                airframeSig[i*3 + 2] = baseHeatSignatureInBand * overlapRatio;
            }
        } // for loop over waveBand
    } // if (target != 0)
    return airframeSig;
}


} // End Simulation namespace
} // End Eaagles namespace

